"""Simulation execution: server-owned mcrun subprocess.

Deliberately bypasses McStasScript's backengine() (no timeout, discards
diagnostics, silently returns [] on runtime failure — see
note/study-mcstasscript-api-2026-07-09.md). Encodes the server-side rules
from note/m1-server-design-2026-07-09.md: explicit parameters on the CLI,
deterministic output dirs, forced recompile, seed != 0, cwd = workdir.
"""

import json
import os
import re
import subprocess
import time

from . import registry, results
from .config import home_dir, mcrun_path, strip_ansi

NCOUNT_CAP = 1e8
DEFAULT_TIMEOUT = 600
_NOISE = re.compile(r"^(ld: warning|Info:|INFO: (Regenerating|Recompiling|Using)|CFLAGS)")


class RunError(RuntimeError):
    pass


def _jobs_path():
    return os.path.join(home_dir(), "jobs.json")


def _load_jobs():
    try:
        with open(_jobs_path()) as f:
            return json.load(f)
    except (OSError, json.JSONDecodeError):
        return {}


def _record_job(job):
    jobs = _load_jobs()
    jobs[job["job_id"]] = job
    with open(_jobs_path(), "w") as f:
        json.dump(jobs, f, indent=2)


def get_job(job_id: str) -> dict:
    jobs = _load_jobs()
    if job_id not in jobs:
        known = ", ".join(sorted(jobs)[-8:]) or "none yet"
        raise RunError(f"Unknown job '{job_id}'. Recent jobs: {known}.")
    return jobs[job_id]


def _diagnostics(output: str, max_lines: int = 40) -> list[str]:
    lines = [
        ln for ln in strip_ansi(output).splitlines()
        if ln.strip() and not _NOISE.match(ln.strip())
    ]
    return lines[-max_lines:]


def _classify(output: str) -> str:
    low = strip_ansi(output)
    if re.search(r"\.instr:\d+|McStas.*[Ee]rror", low):
        return "translate"
    if re.search(r"(error:|undefined symbol|ld: error)", low):
        return "compile"
    return "run"


def run_instr_file(instr_path: str, params: dict, ncount: float = 1e6,
                   workdir: str | None = None, seed: int | None = None,
                   mpi: int | None = None, gravity: bool = False,
                   timeout: int = DEFAULT_TIMEOUT, job_prefix: str = "job") -> dict:
    """Compile + run one .instr; returns a job dict (also persisted)."""
    if ncount > NCOUNT_CAP:
        raise RunError(f"ncount {ncount:g} exceeds cap {NCOUNT_CAP:g} — iterate low, "
                       "validate high, but stay under the cap.")
    if seed == 0:
        raise RunError("seed must be non-zero (mcrun requirement); omit it for random.")

    instr_path = os.path.abspath(instr_path)
    wd = workdir or os.path.dirname(instr_path)
    os.makedirs(wd, exist_ok=True)
    job_id = f"{job_prefix}_{time.strftime('%Y%m%d_%H%M%S')}_{os.getpid() % 1000}"
    outdir = os.path.join(wd, job_id)

    cmd = [mcrun_path(), instr_path, "-c", "-n", str(int(ncount)), "-d", outdir]
    if seed is not None:
        cmd += ["-s", str(seed)]
    if mpi:
        cmd += [f"--mpi={int(mpi)}"]
    if gravity:
        cmd += ["-g"]
    cmd += [f"{k}={v}" for k, v in params.items()]

    t0 = time.time()
    try:
        proc = subprocess.run(cmd, cwd=wd, capture_output=True, text=True, timeout=timeout)
    except subprocess.TimeoutExpired:
        job = {"job_id": job_id, "ok": False, "stage": "run", "instr": instr_path,
               "output_dir": outdir, "params": params, "ncount": ncount, "seed": seed,
               "elapsed_s": round(time.time() - t0, 1),
               "diagnostics": [f"timed out after {timeout}s — reduce ncount or raise timeout"]}
        _record_job(job)
        return job

    output = (proc.stdout or "") + "\n" + (proc.stderr or "")
    ok = proc.returncode == 0 and os.path.isfile(os.path.join(outdir, "mccode.sim"))
    job = {
        "job_id": job_id, "ok": ok, "instr": instr_path, "output_dir": outdir,
        "params": params, "ncount": ncount, "seed": seed,
        "mpi": mpi, "gravity": gravity,
        "elapsed_s": round(time.time() - t0, 1),
    }
    if ok:
        job["detectors"] = [
            m.groupdict() for m in re.finditer(
                r"Detector: (?P<name>\S+)_I=(?P<I>\S+) \S+_ERR=(?P<err>\S+) "
                r"\S+_N=(?P<N>\S+)", strip_ansi(output))
        ]
    else:
        job["stage"] = _classify(output)
        job["diagnostics"] = _diagnostics(output)
        job["returncode"] = proc.returncode
    _record_job(job)
    return job


def run_spec(spec: dict, ncount: float = 1e6, parameters: dict | None = None,
             seed: int | None = None, mpi: int | None = None,
             gravity: bool = False, timeout: int = DEFAULT_TIMEOUT) -> dict:
    """Validate, rebuild .instr from spec, run it."""
    missing = registry.missing_required(spec)
    if missing:
        detail = "; ".join(f"{n}: {', '.join(ps)}" for n, ps in missing)
        raise RunError(
            f"Cannot run — required component parameters unset: {detail}. "
            "Fix with set_parameters."
        )
    if not spec["components"]:
        raise RunError("Instrument has no components yet — add_component first.")

    # every instrument parameter needs a value on the command line (rule 2)
    values = {}
    for p in spec["parameters"]:
        if p["default"] is not None:
            values[p["name"]] = p["default"]
    values.update(parameters or {})
    unset = [p["name"] for p in spec["parameters"] if p["name"] not in values]
    if unset:
        raise RunError(
            f"Instrument parameter(s) without value: {', '.join(unset)} — pass them "
            "in run_simulation(parameters={...}) or give them defaults."
        )
    unknown = set(values) - {p["name"] for p in spec["parameters"]}
    if unknown:
        raise RunError(
            f"Unknown instrument parameter(s): {', '.join(sorted(unknown))}. "
            f"Defined: {', '.join(p['name'] for p in spec['parameters']) or 'none'}."
        )

    instr_path = registry.build_instr_file(spec)
    return run_instr_file(
        instr_path, values, ncount=ncount, workdir=registry.workdir(spec["name"]),
        seed=seed, mpi=mpi, gravity=gravity, timeout=timeout,
        job_prefix=spec["name"],
    )


def job_results(job_id: str) -> dict:
    job = get_job(job_id)
    if not job["ok"]:
        return {"job_id": job_id, "ok": False, "stage": job.get("stage"),
                "diagnostics": job.get("diagnostics")}
    summary = results.summarize(job["output_dir"])
    summary.update({"job_id": job_id, "ok": True, "ncount": job["ncount"],
                    "seed": job["seed"], "params": job["params"]})
    return summary
