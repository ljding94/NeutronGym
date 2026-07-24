"""McStasBench grader (pilot version): observable-based, no LLM judge.

Grades a candidate instrument (.instr file or registry spec) against a
reference by running BOTH under the identical protocol (ncount + seed) and
comparing per-monitor observables with statistics-aware tolerances:

    tolerance = max(rtol * |ref|, nsigma * (err_ref + err_cand))

Monitor matching is role-based, not name-based (agents pick their own
names): '2d' -> the 2D monitor with the most events; 'wavelength' -> the 1D
monitor whose x-axis is a wavelength; 'tof' -> time-of-flight axis.

Verdict: hard requirements (compiles, runs, required monitor roles present)
gate the score to 0; otherwise score = passed observables / total.

Usage:
    python benchmark/grader.py <task.json> --candidate <file.instr> [--params k=v ...]
"""

import argparse
import json
import os
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CACHE = os.path.join(REPO, "benchmark", "refcache")

from mcstas_mcp import execution, results  # noqa: E402
from mcstas_mcp.config import resources_dir  # noqa: E402


def _resolve_instr(ref: str) -> str:
    if ref.startswith("shipped:"):
        name = ref.split(":", 1)[1]
        from mcstas_mcp import examples
        return examples.get_example(name)["path"]
    return os.path.join(REPO, ref)


def run_protocol(instr_path: str, params: dict, protocol: dict,
                 workdir: str, label: str) -> dict:
    """Run one side of the comparison; returns the results summary."""
    job = execution.run_instr_file(
        instr_path, params, ncount=float(protocol.get("ncount", 1e6)),
        seed=int(protocol.get("seed", 1234)), workdir=workdir,
        job_prefix=label, wait=None, timeout=int(protocol.get("timeout", 900)))
    if not job.get("ok"):
        return {"ok": False, "stage": job.get("stage"),
                "diagnostics": (job.get("diagnostics") or [])[-15:]}
    summary = results.summarize(job["output_dir"])
    summary["ok"] = True
    return summary


def reference_summary(task: dict) -> dict:
    """Reference observables, cached per task id + protocol."""
    os.makedirs(CACHE, exist_ok=True)
    key = os.path.join(CACHE, f"{task['id']}.json")
    if os.path.isfile(key):
        with open(key) as f:
            return json.load(f)
    ref = task["reference"]
    summary = run_protocol(_resolve_instr(ref["instr"]),
                           ref.get("parameters", {}), task["protocol"],
                           os.path.join(CACHE, task["id"] + "_work"), "ref")
    if not summary.get("ok"):
        raise RuntimeError(f"reference failed to run: {summary}")
    with open(key, "w") as f:
        json.dump(summary, f, indent=2)
    return summary


def _monitor_role(m: dict) -> set:
    roles = set()
    dims = m.get("dims") or []
    xlabel = (m.get("xlabel") or "").lower()
    if len(dims) == 2:
        roles.add("2d")
    if len(dims) == 1:
        roles.add("1d")
        if "wavelength" in xlabel or "[aa]" in xlabel or "angs" in xlabel:
            roles.add("wavelength")
        if "time" in xlabel or "tof" in xlabel:
            roles.add("tof")
        if "energy" in xlabel:
            roles.add("energy")
    return roles


def match_monitor(summary: dict, role: str):
    """Best monitor for a role: most events among matching."""
    candidates = [m for m in summary["monitors"] if role in _monitor_role(m)]
    if not candidates:
        return None
    return max(candidates, key=lambda m: m.get("events") or 0)


def _get_observable(mon: dict, name: str):
    paths = {
        "intensity": ("intensity",),
        "beam_width_x": ("beam_width", "dX"),
        "beam_width_y": ("beam_width", "dY"),
        "beam_center_x": ("beam_center", "X0"),
        "fwhm": ("fwhm",),
        "center_of_mass": ("center_of_mass",),
    }
    if name not in paths:
        raise KeyError(f"unknown observable '{name}'")
    val = mon
    for p in paths[name]:
        val = val.get(p) if isinstance(val, dict) else None
        if val is None:
            return None
    return val


def grade(task: dict, cand_summary: dict, ref_summary: dict) -> dict:
    checks, hard_failures = [], []
    if not cand_summary.get("ok"):
        return {"task": task["id"], "pass": False, "score": 0.0,
                "hard_failures": [f"candidate failed to run "
                                  f"({cand_summary.get('stage')})"],
                "diagnostics": cand_summary.get("diagnostics"), "checks": []}

    for spec in task["grading"]["monitors"]:
        role = spec["role"]
        ref_m = match_monitor(ref_summary, role)
        cand_m = match_monitor(cand_summary, role)
        if ref_m is None:
            continue  # task authoring error; don't punish candidate
        if cand_m is None:
            hard_failures.append(f"no monitor with role '{role}' in candidate")
            continue
        if (cand_m.get("events") or 0) < task["grading"].get("min_events", 1000):
            hard_failures.append(
                f"role '{role}': only {cand_m.get('events'):.0f} events — "
                "below statistics floor, ungradable")
            continue
        for obs, tol in spec["observables"].items():
            ref_v = _get_observable(ref_m, obs)
            cand_v = _get_observable(cand_m, obs)
            if ref_v is None:
                continue
            entry = {"role": role, "observable": obs,
                     "reference": ref_v, "candidate": cand_v}
            if cand_v is None:
                entry.update({"pass": False, "reason": "missing in candidate"})
            else:
                stat = 0.0
                if obs == "intensity":
                    stat = (tol.get("nsigma", 3)
                            * ((ref_m.get("intensity_err") or 0)
                               + (cand_m.get("intensity_err") or 0)))
                allowed = max(tol.get("rtol", 0.2) * abs(ref_v), stat)
                entry.update({
                    "pass": abs(cand_v - ref_v) <= allowed,
                    "delta": cand_v - ref_v,
                    "allowed": allowed,
                })
            checks.append(entry)

    n_pass = sum(1 for c in checks if c["pass"])
    score = 0.0 if hard_failures else (n_pass / len(checks) if checks else 0.0)
    return {
        "task": task["id"],
        "pass": not hard_failures and checks != [] and n_pass == len(checks),
        "score": round(score, 3),
        "checks_passed": f"{n_pass}/{len(checks)}",
        "hard_failures": hard_failures,
        "checks": checks,
        "protocol": task["protocol"],
    }


def grade_improvement(task: dict, cand_summary: dict) -> dict:
    """Grade a T2 improve-task candidate: FOM vs calibrated target AND every
    constraint within bounds (multi-objective — single-metric gaming fails).
    Baselines are included for context; the classical optimizer is the bar."""
    if not cand_summary.get("ok"):
        return {"task": task["id"], "pass": False, "score": 0.0,
                "hard_failures": [f"candidate failed to run "
                                  f"({cand_summary.get('stage')})"],
                "diagnostics": cand_summary.get("diagnostics")}
    fom_spec = task["fom"]
    mon = next((m for m in cand_summary["monitors"]
                if m["component"] == fom_spec["monitor"]), None) \
        or match_monitor(cand_summary, fom_spec["role"])
    if mon is None:
        return {"task": task["id"], "pass": False, "score": 0.0,
                "hard_failures": [f"no FOM monitor (role '{fom_spec['role']}')"]}
    if (mon.get("events") or 0) < task.get("grading", {}).get("min_events", 1000):
        return {"task": task["id"], "pass": False, "score": 0.0,
                "hard_failures": [f"FOM monitor has {mon.get('events'):.0f} "
                                  "events — below statistics floor"]}
    fom = mon["intensity"]
    target = task["targets"]["fom_min"]
    checks = [{"check": "fom>=target", "fom": fom, "err": mon["intensity_err"],
               "target": target, "pass": fom >= target}]
    for c in task.get("constraints", []):
        cmon = match_monitor(cand_summary, c["role"])
        val = _get_observable(cmon, c["observable"]) if cmon else None
        hi, lo = c.get("max_value"), c.get("min_value")
        ok = (val is not None
              and (hi is None or val <= hi)
              and (lo is None or val >= lo))
        checks.append({"check": f"{c['role']}.{c['observable']} in band",
                       "value": val, "min": lo, "max": hi, "pass": ok})
    n_pass = sum(1 for c in checks if c["pass"])
    base = task.get("baselines", {})
    return {
        "task": task["id"],
        "pass": n_pass == len(checks),
        "score": round(n_pass / len(checks), 3),
        "checks_passed": f"{n_pass}/{len(checks)}",
        "hard_failures": [],
        "checks": checks,
        "context": {
            "baseline_fom": base.get("initial", {}).get("fom"),
            "optimizer_fom": base.get("optimizer", {}).get("fom"),
            "random_search_fom": base.get("random_search", {}).get("fom"),
        },
        "protocol": task["protocol"],
    }


def grade_candidate_file(task: dict, instr_path: str, params: dict | None,
                         workdir: str) -> dict:
    ref = reference_summary(task)
    cand = run_protocol(instr_path, params or task["reference"].get("parameters", {}),
                        task["protocol"], workdir, "cand")
    return grade(task, cand, ref)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("task")
    ap.add_argument("--candidate", required=True, help=".instr file to grade")
    ap.add_argument("--params", nargs="*", default=[], help="k=v run parameters")
    ap.add_argument("--workdir", default=None)
    args = ap.parse_args()
    with open(args.task) as f:
        task = json.load(f)
    params = dict(kv.split("=", 1) for kv in args.params) or None
    wd = args.workdir or os.path.join(REPO, "runs", "grader", task["id"])
    report = grade_candidate_file(task, args.candidate, params, wd)
    print(json.dumps(report, indent=2))
    return 0 if report["pass"] else 1


if __name__ == "__main__":
    sys.exit(main())
