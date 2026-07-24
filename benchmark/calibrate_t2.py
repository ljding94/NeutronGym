"""Calibrate T2 improvement tasks with classical baselines.

For each T2 task: run the baseline configuration, the classical optimizer
(mcrun --optimize via the M4 layer), and a matched-compute random search.
The task's target is set at baseline + target_fraction * (optimizer -
baseline), so it is achievable-but-not-trivial BY CONSTRUCTION, and the
prompt is rendered from the template with real numbers.

Validation printed per task: the optimizer must meet the target (true by
construction); random search should generally miss it (discrimination).

Usage: conda run -n mcstas python benchmark/calibrate_t2.py
"""

import glob
import json
import os
import random
import shutil
import tempfile

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

os.environ.setdefault("MCSTAS_MCP_HOME", tempfile.mkdtemp(prefix="t2_cal_"))

from mcstas_mcp import execution, optimization, registry  # noqa: E402
import grader  # noqa: E402


def install_reference(task):
    """Copy the committed reference spec into the active registry home."""
    name = os.path.splitext(os.path.basename(task["reference"]["instr"]))[0]
    src = os.path.join(REPO, "benchmark", "instruments", name)
    dst = os.path.join(os.environ["MCSTAS_MCP_HOME"], "instruments", name)
    if not os.path.isdir(dst):
        os.makedirs(os.path.dirname(dst), exist_ok=True)
        shutil.copytree(src, dst)
        # keep only the spec; workdir artifacts regenerate
        for f in glob.glob(os.path.join(dst, "*.instr")):
            os.remove(f)
    return name


def run_point(task, params, seed=None, label="cal"):
    """One full-protocol run; returns (fom, constraint_values, summary)."""
    proto = task["protocol"]
    instr = os.path.join(REPO, task["reference"]["instr"])
    p = {**task["reference"]["parameters"], **params}
    job = execution.run_instr_file(
        instr, p, ncount=float(proto["ncount"]),
        seed=seed or int(proto["seed"]),
        workdir=os.path.join(REPO, "runs", "t2_cal", task["id"]),
        timeout=int(proto["timeout"]), wait=None, job_prefix=label)
    if not job.get("ok"):
        raise RuntimeError(f"{task['id']} run failed: {job.get('diagnostics')}")
    from mcstas_mcp import results
    summary = results.summarize(job["output_dir"])
    fom_mon = next(m for m in summary["monitors"]
                   if m["component"] == task["fom"]["monitor"])
    cons = {}
    for c in task.get("constraints", []):
        mon = grader.match_monitor(summary, c["role"])
        cons[f"{c['role']}.{c['observable']}"] = grader._get_observable(
            mon, c["observable"]) if mon else None
    return fom_mon["intensity"], fom_mon["intensity_err"], cons


def calibrate(path):
    with open(path) as f:
        task = json.load(f)
    print(f"=== {task['id']}")
    name = install_reference(task)
    spec = registry.load(name)
    proto, cal = task["protocol"], task["calibration"]
    free = task["free_parameters"]

    base_fom, base_err, base_cons = run_point(task, {}, label="baseline")
    print(f"  baseline FOM = {base_fom:.5g} ± {base_err:.2g}  constraints {base_cons}")

    # materialize the constraint band limits from the measured baseline first
    for c in task.get("constraints", []):
        key = f"{c['role']}.{c['observable']}"
        if c.get("max_factor_of_baseline"):
            c["max_value"] = base_cons[key] * c["max_factor_of_baseline"]
        if c.get("min_factor_of_baseline"):
            c["min_value"] = base_cons[key] * c["min_factor_of_baseline"]

    # classical ensemble: mcrun optimizer point (only if in-bounds AND
    # constraint-satisfying — nelder-mead is NOT bound-respecting, verified)
    # plus constrained random search under matched compute
    candidates = []
    guesses = {k: [lo, (lo + hi) / 2, hi] for k, (lo, hi) in free.items()}
    opt = optimization.run_optimize(
        spec, guesses, monitor=task["fom"]["monitor"],
        method=cal["method"], maxiter=cal["maxiter"],
        ncount=float(proto["ncount"]), seed=int(proto["seed"]),
        parameters={k: v for k, v in task["reference"]["parameters"].items()
                    if k not in free},
        wait=None, timeout=1800)
    ores = optimization.optimize_results(opt["job_id"])
    opt_params = {k: ores["best"]["parameters"].get(k) for k in free}
    opt_note = "valid"
    if None in opt_params.values() or not _in_bounds(free, opt_params):
        opt_note = f"DISCARDED: left bounds {opt_params}"
        opt_fom = opt_err = None
    else:
        opt_fom, opt_err, opt_cons = run_point(task, opt_params, label="optcheck")
        if not _constraints_ok(task, opt_cons, base_cons):
            opt_note = f"DISCARDED: violates constraints {opt_cons}"
        else:
            candidates.append(("optimizer", opt_fom, opt_params))
    print(f"  optimizer ({ores['iterations']} iters): {opt_note}"
          + (f" FOM {opt_fom:.5g}" if opt_fom else ""))

    rng = random.Random(int(proto["seed"]))
    rand_best, rand_params = None, None
    for i in range(cal["maxiter"]):
        params = {k: rng.uniform(lo, hi) for k, (lo, hi) in free.items()}
        fom, _err, cons = run_point(task, params, label=f"rand{i}")
        if _constraints_ok(task, cons, base_cons) and \
                (rand_best is None or fom > rand_best):
            rand_best, rand_params = fom, params
    if rand_best is not None:
        candidates.append(("random_search", rand_best, rand_params))
    print(f"  constrained random best = {rand_best and f'{rand_best:.5g}'}")

    if not candidates:
        print("  !! no valid classical candidate — task rejected, fix design")
        return False
    src_name, classical_fom, classical_params = max(candidates, key=lambda c: c[1])
    if classical_fom <= base_fom + 3 * base_err:
        print("  !! classical best does not beat baseline — task rejected")
        return False
    target = base_fom + cal["target_fraction"] * (classical_fom - base_fom)
    task["baselines"] = {
        "initial": {"fom": base_fom, "err": base_err, "constraints": base_cons},
        "classical_best": {"source": src_name, "fom": classical_fom,
                           "parameters": classical_params},
        "optimizer": {"note": opt_note, "fom": opt_fom,
                      "iterations": ores["iterations"]},
        "random_search": {"fom": rand_best, "parameters": rand_params,
                          "samples": cal["maxiter"]},
    }
    task["targets"] = {"fom_min": target}
    task["prompt"] = render_prompt(task)
    with open(path, "w") as f:
        json.dump(task, f, indent=2)
    print(f"  classical best = {classical_fom:.5g} ({src_name}) "
          f"-> target = {target:.5g}")
    return True


def _constraints_ok(task, cons, base_cons):
    for c in task.get("constraints", []):
        key = f"{c['role']}.{c['observable']}"
        val = cons.get(key)
        if val is None:
            return False
        hi = c.get("max_value") or (base_cons[key] * c["max_factor_of_baseline"]
                                    if c.get("max_factor_of_baseline") else None)
        lo = c.get("min_value") or (base_cons[key] * c["min_factor_of_baseline"]
                                    if c.get("min_factor_of_baseline") else None)
        if hi is not None and val > hi:
            return False
        if lo is not None and val < lo:
            return False
    return True


def _in_bounds(free, params):
    return all(lo <= params[k] <= hi for k, (lo, hi) in free.items())


def render_prompt(task):
    t = task["prompt_template"]
    ref = task["reference"]["parameters"]
    free = task["free_parameters"]
    subst = {**{k: v for k, v in ref.items()},
             "ncount": float(task["protocol"]["ncount"]),
             "fom_min": task["targets"]["fom_min"]}
    for k, (lo, hi) in free.items():
        short = {"w_in": ("w_in_min", "w_in_max"), "w_out": ("w_out_min", "w_out_max"),
                 "m_coat": ("m_min", "m_max"), "r_pin1": ("r1_min", "r1_max"),
                 "r_pin2": ("r2_min", "r2_max")}.get(k, (f"{k}_min", f"{k}_max"))
        subst[short[0]], subst[short[1]] = lo, hi
    for c in task.get("constraints", []):
        if c["observable"] == "beam_width_x":
            subst["bwx_max"] = c["max_value"]
            if c.get("min_value") is not None:
                subst["bwx_min"] = c["min_value"]
    return t.format(**subst)


def main():
    ok = 0
    paths = sorted(glob.glob(os.path.join(REPO, "benchmark", "tasks", "T2", "*.json")))
    for p in paths:
        ok += bool(calibrate(p))
    print(f"\n{ok}/{len(paths)} T2 tasks calibrated")


if __name__ == "__main__":
    main()
