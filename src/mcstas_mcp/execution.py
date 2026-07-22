"""Simulation execution: server-owned async mcrun subprocess.

Deliberately bypasses McStasScript's backengine() (no timeout, discards
diagnostics, silently returns [] on runtime failure). Encodes the server-side
rules from note/m1-server-design-2026-07-09.md (explicit parameters, seed != 0,
stdin=DEVNULL, process-group kill, deterministic output dirs, self-located
toolchain).

M2 job model: run_instr_file launches mcrun detached (own process group,
output to a per-job log file) and a supervisor thread finalizes the record.
Callers may wait (seconds) or poll job_status. Records live in jobs.json
(cross-process file lock, atomic writes) so jobs survive server restarts:
if the supervising server died, get_job reconciles from the pid + on-disk
output. Binaries are cached per instrument (sha of .instr + mpi flag) so
parameter-only iterations skip the ~2 s recompile.
"""

import hashlib
import json
import os
import re
import shutil
import signal
import subprocess
import threading
import time

from . import registry, results
from .config import file_lock, home_dir, mcrun_path, strip_ansi

NCOUNT_CAP = 1e8
DEFAULT_TIMEOUT = 600
MAX_TIMEOUT = 1800
LOG_TAIL_LINES = 15
_NOISE = re.compile(r"^(ld: warning|Info:|INFO: (Regenerating|Recompiling|Using)|CFLAGS)")

_jobs_mutex = threading.Lock()
_counter_lock = threading.Lock()
_counter = 0


class RunError(RuntimeError):
    pass


def _jobs_path():
    return os.path.join(home_dir(), "jobs.json")


def _lock_path():
    return os.path.join(home_dir(), "jobs.lock")


def _load_jobs():
    try:
        with open(_jobs_path()) as f:
            return json.load(f)
    except (OSError, json.JSONDecodeError):
        return {}


def _update_job(job_id: str, **fields) -> dict:
    """Atomic cross-process read-modify-write of one job record."""
    with _jobs_mutex, file_lock(_lock_path()):
        jobs = _load_jobs()
        rec = jobs.get(job_id, {})
        rec.update(fields)
        rec["job_id"] = job_id
        jobs[job_id] = rec
        tmp = _jobs_path() + ".tmp"
        with open(tmp, "w") as f:
            json.dump(jobs, f, indent=2)
        os.replace(tmp, _jobs_path())
        return dict(rec)


def _next_job_id(prefix: str) -> str:
    global _counter
    with _counter_lock:
        _counter += 1
        n = _counter
    return f"{prefix}_{time.strftime('%Y%m%d_%H%M%S')}_{os.getpid()}_{n}"


def _pid_alive(pid) -> bool:
    if not pid:
        return False
    try:
        os.kill(pid, 0)
        return True
    except (ProcessLookupError, PermissionError):
        return False


def _kill_group(pid):
    try:
        os.killpg(os.getpgid(pid), signal.SIGKILL)
    except (ProcessLookupError, PermissionError):
        pass


def _read_log(rec) -> str:
    try:
        with open(rec["log"], errors="replace") as f:
            return f.read()
    except OSError:
        return ""


def _log_lines(text, n=None):
    lines = [ln for ln in strip_ansi(text).splitlines()
             if ln.strip() and not _NOISE.match(ln.strip())]
    return lines[-n:] if n else lines


def _classify(output: str) -> str:
    low = strip_ansi(output)
    # real mcstas cogen failure signatures (verified 2026-07-09)
    if re.search(r"syntax error at line|Errors encountered during parse"
                 r"|Code generation failed|\.instr:\d+", low):
        return "translate"
    if re.search(r"(error:|undefined symbol|ld: error)", low):
        return "compile"
    return "run"


def _finalize(job_id: str, returncode, timed_out=False, cancelled=False) -> dict:
    """Idempotent under the lock: first finalizer wins (supervisor thread vs
    cancel vs restart-reconcile can race)."""
    with _jobs_mutex, file_lock(_lock_path()):
        jobs = _load_jobs()
        rec = jobs.get(job_id)
        if rec is None or rec.get("state") != "running":
            return dict(rec or {})
        rec = dict(rec)
        output = _read_log(rec)
        sim_ok = os.path.isfile(os.path.join(rec["output_dir"], "mccode.sim"))
        ok = sim_ok and (returncode == 0 if returncode is not None else True)
        rec["elapsed_s"] = round(time.time() - rec["started"], 1)
        rec["returncode"] = returncode
        rec["ok"] = ok and not timed_out and not cancelled
        if cancelled:
            rec["state"] = "cancelled"
            rec["diagnostics"] = ["cancelled by cancel_job"]
        elif timed_out:
            rec["state"] = "failed"
            rec["stage"] = "run"
            rec["diagnostics"] = (
                [f"timed out after {rec['timeout']}s — reduce ncount, use mpi, "
                 "or raise the timeout"] + _log_lines(output, 10))
        elif rec["ok"]:
            rec["state"] = "done"
            rec["detectors"] = [
                m.groupdict() for m in re.finditer(
                    r"Detector: (?P<name>\S+)_I=(?P<I>\S+) \S+_ERR=(?P<err>\S+) "
                    r"\S+_N=(?P<N>\S+)", strip_ansi(output))
            ]
            if rec.get("instr_sha"):  # remember the now-valid cached binary
                _write_build_meta(rec)
        else:
            rec["state"] = "failed"
            rec["stage"] = _classify(output)
            rec["diagnostics"] = _log_lines(output, 40)
        jobs[job_id] = rec
        tmp = _jobs_path() + ".tmp"
        with open(tmp, "w") as f:
            json.dump(jobs, f, indent=2)
        os.replace(tmp, _jobs_path())
        return dict(rec)


def _supervise(job_id: str, proc: subprocess.Popen, timeout: int):
    try:
        rc = proc.wait(timeout=timeout)
    except subprocess.TimeoutExpired:
        _kill_group(proc.pid)
        proc.wait()
        _finalize(job_id, None, timed_out=True)
        return
    _finalize(job_id, rc)


def _reconcile(rec: dict) -> dict:
    """Recover a 'running' record whose supervising server is gone."""
    job_id = rec["job_id"]
    if _pid_alive(rec.get("pid")):
        # orphan still computing; re-arm the timeout since no supervisor exists
        if time.time() - rec["started"] > rec.get("timeout", DEFAULT_TIMEOUT) * 1.5:
            _kill_group(rec["pid"])
            return _finalize(job_id, None, timed_out=True)
        return rec
    return _finalize(job_id, None)


def get_job(job_id: str) -> dict:
    jobs = _load_jobs()
    if job_id not in jobs:
        known = ", ".join(sorted(jobs)[-8:]) or "none yet"
        raise RunError(f"Unknown job '{job_id}'. Recent jobs: {known}.")
    rec = dict(jobs[job_id])
    rec.setdefault("job_id", job_id)
    if rec.get("state") == "running":
        rec = _reconcile(rec)
    return rec


def job_status(job_id: str) -> dict:
    rec = get_job(job_id)
    out = {"job_id": job_id, "state": rec.get("state")}
    if rec.get("ok") is not None:  # sim success unknown while running
        out["ok"] = rec["ok"]
    if rec.get("state") == "running":
        out["elapsed_s"] = round(time.time() - rec["started"], 1)
        out["log_tail"] = _log_lines(_read_log(rec), LOG_TAIL_LINES)
        out["hint"] = "poll job_status again, or cancel_job to stop"
    else:
        out["elapsed_s"] = rec.get("elapsed_s")
        for k in ("detectors", "stage", "diagnostics"):
            if rec.get(k) is not None:
                out[k] = rec[k]
    return out


def cancel(job_id: str) -> dict:
    rec = get_job(job_id)
    if rec.get("state") != "running":
        raise RunError(f"Job '{job_id}' is not running (state: {rec.get('state')}).")
    _kill_group(rec["pid"])
    return _finalize(job_id, None, cancelled=True)


def _build_meta_path(instr_path):
    return os.path.join(os.path.dirname(instr_path), ".build_meta.json")


def _instr_sha(instr_path: str) -> str:
    """Content hash ignoring comments — McStasScript stamps a generation
    timestamp into the header comment, which must not bust the cache."""
    with open(instr_path, errors="replace") as f:
        lines = [ln for ln in f
                 if not re.match(r"\s*(\*|/\*|\*/|//)", ln)]
    return hashlib.sha1("".join(lines).encode()).hexdigest()


def _compile_plan(instr_path: str, mpi) -> tuple[bool, str]:
    """(need_force_compile, instr_sha). Skip -c when the same .instr content
    was already compiled with the same MPI setting and the binary exists
    (rule 3: always recompile on MPI toggle)."""
    sha = _instr_sha(instr_path)
    binary = instr_path[:-len(".instr")] + ".out"
    try:
        with open(_build_meta_path(instr_path)) as f:
            meta = json.load(f)
    except (OSError, json.JSONDecodeError):
        meta = {}
    cached = (meta.get("sha") == sha and meta.get("mpi") == bool(mpi)
              and os.path.isfile(binary))
    return not cached, sha


def _write_build_meta(rec):
    try:
        with open(_build_meta_path(rec["instr"]), "w") as f:
            json.dump({"sha": rec["instr_sha"], "mpi": bool(rec.get("mpi"))}, f)
    except OSError:
        pass


def run_instr_file(instr_path: str, params: dict, ncount: float = 1e6,
                   workdir: str | None = None, seed: int | None = None,
                   mpi: int | None = None, gravity: bool = False,
                   timeout: int = DEFAULT_TIMEOUT, job_prefix: str = "job",
                   wait: float | None = None) -> dict:
    """Launch one mcrun job. wait=None blocks until done/timeout (M1
    semantics); wait=N blocks at most N seconds; wait=0 returns immediately
    with state 'running'. Always returns the current job record."""
    if ncount < 1:
        raise RunError(f"ncount must be >= 1 (got {ncount:g}).")
    if ncount > NCOUNT_CAP:
        raise RunError(f"ncount {ncount:g} exceeds cap {NCOUNT_CAP:g} — iterate low, "
                       "validate high, but stay under the cap.")
    if seed == 0:
        raise RunError("seed must be non-zero (mcrun requirement); omit it for random.")
    timeout = min(int(timeout), MAX_TIMEOUT)

    instr_path = os.path.abspath(instr_path)
    wd = workdir or os.path.dirname(instr_path)
    os.makedirs(wd, exist_ok=True)
    job_id = _next_job_id(job_prefix)
    outdir = os.path.join(wd, job_id)
    log_path = os.path.join(wd, job_id + ".log")

    need_compile, sha = _compile_plan(instr_path, mpi) \
        if os.path.isfile(instr_path) else (True, None)
    cmd = [mcrun_path(), instr_path]
    if need_compile:
        cmd.append("-c")
    cmd += ["-n", str(int(ncount)), "-d", outdir]
    if seed is not None:
        cmd += ["-s", str(seed)]
    if mpi:
        cmd += [f"--mpi={int(mpi)}"]
    if gravity:
        cmd += ["-g"]
    cmd += [f"{k}={v}" for k, v in params.items()]

    # per-run provenance: snapshot the exact .instr next to the log
    try:
        shutil.copy(instr_path, os.path.join(wd, job_id + ".instr"))
    except OSError:
        pass

    _update_job(job_id, state="running", ok=None, instr=instr_path,
                output_dir=outdir, log=log_path, params=params, ncount=ncount,
                seed=seed, mpi=mpi, gravity=gravity, timeout=timeout,
                compiled=need_compile, instr_sha=sha, started=time.time(),
                cmd=" ".join(cmd))
    log_f = open(log_path, "w")
    try:
        proc = subprocess.Popen(cmd, cwd=wd, stdin=subprocess.DEVNULL,
                                stdout=log_f, stderr=subprocess.STDOUT,
                                text=True, start_new_session=True)
    finally:
        log_f.close()
    _update_job(job_id, pid=proc.pid)

    supervisor = threading.Thread(target=_supervise, args=(job_id, proc, timeout),
                                  daemon=True)
    supervisor.start()
    if wait is None:
        supervisor.join()
    elif wait > 0:
        supervisor.join(wait)
    return get_job(job_id)


def run_spec(spec: dict, ncount: float = 1e6, parameters: dict | None = None,
             seed: int | None = None, mpi: int | None = None,
             gravity: bool = False, timeout: int = DEFAULT_TIMEOUT,
             wait: float | None = None, job_prefix: str | None = None) -> dict:
    """Validate, rebuild .instr from spec, launch it."""
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
        seed=seed, mpi=mpi, gravity=gravity, timeout=timeout, wait=wait,
        job_prefix=job_prefix or spec["name"],
    )


def job_results(job_id: str) -> dict:
    job = get_job(job_id)
    state = job.get("state")
    if state == "running":
        elapsed = round(time.time() - job["started"], 1)
        return {"job_id": job_id, "ok": False, "state": "running",
                "error": f"Job still running ({elapsed}s elapsed) — poll "
                         f"job_status('{job_id}') until state is 'done'."}
    if not job.get("ok"):
        return {"job_id": job_id, "ok": False, "state": state,
                "stage": job.get("stage"), "diagnostics": job.get("diagnostics")}
    summary = results.summarize(job["output_dir"])
    summary.update({"job_id": job_id, "ok": True, "state": state,
                    "ncount": job["ncount"], "seed": job["seed"],
                    "params": job["params"]})
    return summary
