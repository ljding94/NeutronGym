"""Parameter scans and classical optimization (M4).

Wraps mcrun's built-in machinery: `-N` linear scans and `--optimize`
(scipy under the hood: powell, nelder-mead, ...; FOM = a monitor's
intensity or an --optimize-eval expression). No hand-rolled loops.

Double duty (PLAN M4): these are agent tools over MCP AND the classical
baselines that benchmark T2 tasks are measured against. Headless baseline
runner: `python -m mcstas_mcp.optimization scan|optimize ...` prints JSON.

mccode.dat gotcha (verified in the 2026-07-09 study): yvars columns are
`<component>_I <component>_ERR` per OUTPUT FILE, and component names repeat
when one component writes several files — columns are keyed by position.
"""

import json
import os
import re

from . import execution, registry
from .execution import RunError

SCAN_MAX_POINTS = 201
OPTIMIZE_METHODS = {
    "powell", "nelder-mead", "cg", "bfgs", "newton-cg", "l-bfgs-b", "tnc",
    "cobyla", "slsqp", "trust-constr",
}
HISTORY_CAP = 50


def _require_instrument_params(spec: dict, names):
    defined = {p["name"] for p in spec["parameters"]}
    missing = [n for n in names if n not in defined]
    if missing:
        raise RunError(
            f"{', '.join(missing)} is not an instrument parameter of "
            f"'{spec['name']}' (defined: {', '.join(sorted(defined)) or 'none'}). "
            "Only instrument parameters can be scanned/optimized — expose the "
            "quantity via add_parameter and reference it in the component."
        )


def run_scan(spec: dict, parameter: str, pmin: float, pmax: float,
             numpoints: int = 7, ncount: float = 1e5,
             parameters: dict | None = None, seed: int | None = None,
             mpi: int | None = None, timeout: int = 900,
             wait: float | None = None) -> dict:
    """Linear scan of one instrument parameter via `mcrun -N`."""
    _require_instrument_params(spec, [parameter])
    if not 2 <= numpoints <= SCAN_MAX_POINTS:
        raise RunError(f"numpoints must be 2..{SCAN_MAX_POINTS} (got {numpoints}).")
    if not pmin < pmax:
        raise RunError(f"scan needs min < max (got {pmin} .. {pmax}).")
    params = dict(parameters or {})
    params[parameter] = f"{pmin},{pmax}"
    return execution.run_spec(
        spec, ncount=ncount, parameters=params, seed=seed, mpi=mpi,
        timeout=timeout, wait=wait, job_prefix=f"{spec['name']}_scan",
        extra_flags=["-N", str(int(numpoints))], kind="scan")


def run_optimize(spec: dict, free_parameters: dict, monitor: str | None = None,
                 eval_expr: str | None = None, minimize: bool = False,
                 method: str = "powell", maxiter: int = 50,
                 tol: float | None = None, ncount: float = 1e5,
                 parameters: dict | None = None, seed: int | None = None,
                 timeout: int = 1800, wait: float | None = None) -> dict:
    """Classical optimization via `mcrun --optimize`.

    free_parameters: {name: [min, guess, max]}. FOM: a monitor's intensity
    (monitor=...) or an --optimize-eval expression over the detector struct
    (d.intensity, d.dX, ...). Maximizes unless minimize=True.
    """
    if not free_parameters:
        raise RunError("free_parameters is empty — nothing to optimize.")
    _require_instrument_params(spec, free_parameters)
    if method not in OPTIMIZE_METHODS:
        raise RunError(f"method '{method}' not in {sorted(OPTIMIZE_METHODS)}.")
    if not (monitor or eval_expr):
        raise RunError("give a FOM: monitor=<component name> or eval_expr=...")
    if not 1 <= int(maxiter) <= 500:
        raise RunError("maxiter must be 1..500 (each iteration is a full run).")

    params = dict(parameters or {})
    for name, bounds in free_parameters.items():
        try:
            mn, guess, mx = (float(x) for x in bounds)
        except (TypeError, ValueError):
            raise RunError(
                f"free_parameters['{name}'] must be [min, guess, max] "
                f"(got {bounds!r}).") from None
        if not (mn <= guess <= mx and mn < mx):
            raise RunError(
                f"free_parameters['{name}']: need min <= guess <= max with "
                f"min < max (got {bounds}).")
        params[name] = f"{mn},{guess},{mx}"

    flags = ["--optimize", f"--optimize-method={method}",
             f"--optimize-maxiter={int(maxiter)}"]
    if monitor:
        flags.append(f"--optimize-monitor={monitor}")
    if eval_expr:
        flags.append(f"--optimize-eval={eval_expr}")
    if minimize:
        flags.append("--optimize-minimize")
    if tol is not None:
        flags.append(f"--optimize-tol={tol}")

    job = execution.run_spec(
        spec, ncount=ncount, parameters=params, seed=seed, timeout=timeout,
        wait=wait, job_prefix=f"{spec['name']}_opt", extra_flags=flags,
        kind="optimize")
    # remember FOM settings for result parsing after restarts
    execution._update_job(job["job_id"], fom_monitor=monitor,
                          fom_minimize=minimize, fom_eval=eval_expr)
    job = execution.get_job(job["job_id"])
    return job


def _parse_mccode_dat(output_dir: str):
    """-> (xvars, yvar_pairs, rows). rows are float lists in `variables` order."""
    path = os.path.join(output_dir, "mccode.dat")
    if not os.path.isfile(path):
        raise FileNotFoundError(
            f"no mccode.dat in {output_dir} — did the scan/optimization produce "
            "any points? Check job_status diagnostics.")
    xvars, pairs, rows = [], [], []
    with open(path, errors="replace") as f:
        for line in f:
            line = line.strip()
            if line.startswith("#"):
                key, _, val = line.lstrip("# ").partition(": ")
                if key == "xvars":
                    xvars = val.split()
                elif key == "yvars":
                    pairs = re.findall(r"\(([^,]+),([^)]+)\)", val)
            elif line:
                try:
                    rows.append([float(x) for x in line.split()])
                except ValueError:
                    continue
    if not rows:
        raise FileNotFoundError(f"mccode.dat in {output_dir} holds no data rows.")
    return xvars, pairs, rows


def _tabulate(xvars, pairs, rows):
    nx = len(xvars)
    scanned = {x: [r[i] for r in rows] for i, x in enumerate(xvars)}
    monitors = []
    for j, (iname, _ename) in enumerate(pairs):
        base = nx + 2 * j
        monitors.append({
            "monitor": iname[:-2] if iname.endswith("_I") else iname,
            "column": j,  # positional key — names can repeat (multi-file comps)
            "I": [r[base] for r in rows],
            "err": [r[base + 1] for r in rows],
        })
    return scanned, monitors


def _running_or_failed(job):
    if job.get("state") == "running":
        return {"job_id": job["job_id"], "ok": False, "state": "running",
                "error": f"still running — poll job_status('{job['job_id']}')"}
    if not job.get("ok"):
        return {"job_id": job["job_id"], "ok": False, "state": job.get("state"),
                "stage": job.get("stage"), "diagnostics": job.get("diagnostics")}
    return None


def scan_results(job_id: str) -> dict:
    job = execution.get_job(job_id)
    early = _running_or_failed(job)
    if early:
        return early
    xvars, pairs, rows = _parse_mccode_dat(job["output_dir"])
    scanned, monitors = _tabulate(xvars, pairs, rows)
    return {"job_id": job_id, "ok": True, "kind": "scan",
            "points": len(rows), "ncount_per_point": job["ncount"],
            "seed": job["seed"], "scanned": scanned, "monitors": monitors,
            "note": "per-point output dirs 0/..N-1 inside output_dir hold full "
                    "monitor data; get_monitor_data does not apply to scans"}


def optimize_results(job_id: str) -> dict:
    job = execution.get_job(job_id)
    early = _running_or_failed(job)
    if early:
        return early
    xvars, pairs, rows = _parse_mccode_dat(job["output_dir"])
    scanned, monitors = _tabulate(xvars, pairs, rows)

    fom_monitor, minimize = job.get("fom_monitor"), job.get("fom_minimize")
    best_idx, fom_col = len(rows) - 1, None
    if fom_monitor:
        fom_col = next((m for m in monitors if m["monitor"] == fom_monitor), None)
        if fom_col:
            series = fom_col["I"]
            best_idx = (min if minimize else max)(
                range(len(series)), key=series.__getitem__)

    # scipy's convergence summary lands in the log
    tail = [ln for ln in execution._log_lines(execution._read_log(job))
            if re.search(r"[Oo]ptimi[sz]|function value|iterations", ln)][-5:]

    step = max(1, len(rows) // HISTORY_CAP)
    return {
        "job_id": job_id, "ok": True, "kind": "optimize",
        "iterations": len(rows), "ncount_per_iteration": job["ncount"],
        "seed": job["seed"],
        "best": {
            "parameters": {x: scanned[x][best_idx] for x in xvars},
            "fom": fom_col["I"][best_idx] if fom_col else None,
            "fom_err": fom_col["err"][best_idx] if fom_col else None,
            "monitor": fom_monitor,
            "note": None if fom_col else
                    "best point = last iterate (FOM column not identifiable — "
                    "eval-expression FOMs are not recomputable from mccode.dat)",
        },
        "history": {
            "parameters": {x: scanned[x][::step] for x in xvars},
            "fom": fom_col["I"][::step] if fom_col else None,
        },
        "optimizer_log": tail,
        "next_step": "re-verify the optimum with run_simulation at high ncount "
                     "and a fresh seed before accepting it",
    }


def main(argv=None):
    """Headless baseline runner for benchmark T2 tasks."""
    import argparse

    ap = argparse.ArgumentParser(
        prog="mcstas-baseline",
        description="Classical scan/optimize baselines over a registry "
                    "instrument (JSON to stdout).")
    ap.add_argument("mode", choices=["scan", "optimize"])
    ap.add_argument("instrument", help="instrument_id in the registry")
    ap.add_argument("--parameter", help="scan: parameter name")
    ap.add_argument("--range", help="scan: min,max")
    ap.add_argument("--numpoints", type=int, default=7)
    ap.add_argument("--free", action="append", default=[],
                    help="optimize: name=min,guess,max (repeatable)")
    ap.add_argument("--monitor", help="FOM monitor component name")
    ap.add_argument("--method", default="powell")
    ap.add_argument("--maxiter", type=int, default=50)
    ap.add_argument("--minimize", action="store_true")
    ap.add_argument("--ncount", type=float, default=1e5)
    ap.add_argument("--seed", type=int)
    ap.add_argument("--set", action="append", default=[],
                    help="fixed parameter name=value (repeatable)")
    args = ap.parse_args(argv)

    spec = registry.load(args.instrument)
    fixed = dict(kv.split("=", 1) for kv in args.set)
    if args.mode == "scan":
        pmin, pmax = (float(x) for x in args.range.split(","))
        job = run_scan(spec, args.parameter, pmin, pmax, args.numpoints,
                       ncount=args.ncount, parameters=fixed, seed=args.seed,
                       wait=None)
        out = scan_results(job["job_id"])
    else:
        free = {}
        for kv in args.free:
            name, bounds = kv.split("=", 1)
            free[name] = [float(x) for x in bounds.split(",")]
        job = run_optimize(spec, free, monitor=args.monitor,
                           method=args.method, maxiter=args.maxiter,
                           minimize=args.minimize, ncount=args.ncount,
                           parameters=fixed, seed=args.seed, wait=None)
        out = optimize_results(job["job_id"])
    print(json.dumps(out, indent=2))
    return 0 if out.get("ok") else 1


if __name__ == "__main__":
    raise SystemExit(main())
