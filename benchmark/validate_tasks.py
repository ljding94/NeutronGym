"""Validate authored tasks: the reference instrument must PASS its own task
when re-run at a fresh seed (tolerances absorb statistics; grading contract
is satisfiable). Any task failing this is mis-authored, not hard.

Usage: conda run -n mcstas python benchmark/validate_tasks.py [task_dir]
"""

import glob
import json
import os
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, "benchmark"))
import grader  # noqa: E402

FRESH_SEED = 777


def validate_improve(task: dict) -> dict:
    """T2: classical-best params must PASS at a fresh seed; unimproved
    baseline must FAIL (target discriminates; winner's-curse guarded)."""
    import grader as g
    from mcstas_mcp import execution, results
    instr = os.path.join(REPO, task["reference"]["instr"])

    def summarize(params, label):
        p = {**task["reference"]["parameters"], **params}
        job = execution.run_instr_file(
            instr, p, ncount=float(task["protocol"]["ncount"]), seed=FRESH_SEED,
            workdir=os.path.join(REPO, "runs", "task_validation", task["id"]),
            timeout=int(task["protocol"]["timeout"]), wait=None, job_prefix=label)
        if not job.get("ok"):
            return {"ok": False, "stage": job.get("stage")}
        summ = results.summarize(job["output_dir"])
        summ["ok"] = True
        return summ

    best = task["baselines"]["classical_best"]["parameters"]
    rep_best = g.grade_improvement(task, summarize(best, "best"))
    rep_base = g.grade_improvement(task, summarize({}, "base"))
    ok = rep_best["pass"] and not rep_base["pass"]
    return {"task": task["id"], "pass": ok,
            "score": rep_best["score"],
            "checks": f"best:{rep_best['checks_passed']} base_fails:{not rep_base['pass']}",
            "hard_failures": [] if ok else
            [f"classical-best pass={rep_best['pass']}, baseline pass={rep_base['pass']}"],
            "failed": []}


def validate(task_path: str) -> dict:
    with open(task_path) as f:
        task = json.load(f)
    if task.get("kind") == "improve":
        return validate_improve(task)
    if task.get("kind") == "open_design":
        return {"task": task["id"], "pass": True, "score": None,
                "checks": "definition-only (rubric pending)", "hard_failures": [],
                "failed": []}
    ref = grader.reference_summary(task)
    cand = grader.run_protocol(
        grader._resolve_instr(task["reference"]["instr"]),
        task["reference"].get("parameters", {}),
        {**task["protocol"], "seed": FRESH_SEED},
        os.path.join(REPO, "runs", "task_validation", task["id"]), "selfcheck")
    rep = grader.grade(task, cand, ref)
    return {"task": task["id"], "pass": rep["pass"],
            "score": rep["score"], "checks": rep.get("checks_passed"),
            "hard_failures": rep["hard_failures"],
            "failed": [f"{c['role']}.{c['observable']}"
                       for c in rep["checks"] if not c["pass"]]}


def main():
    task_dir = sys.argv[1] if len(sys.argv) > 1 else \
        os.path.join(REPO, "benchmark", "tasks", "T1")
    ok = 0
    reports = []
    for path in sorted(glob.glob(os.path.join(task_dir, "*.json"))):
        r = validate(path)
        reports.append(r)
        ok += r["pass"]
        flag = "PASS" if r["pass"] else "FAIL"
        print(f"{flag}  {r['task']:30} {r['checks'] or '-':7} "
              f"{','.join(r['failed']) or ''} {';'.join(r['hard_failures'])[:60]}",
              flush=True)
    print(f"\n{ok}/{len(reports)} tasks self-validate")
    with open(os.path.join(task_dir, "_validation.json"), "w") as f:
        json.dump(reports, f, indent=2)
    return 0 if ok == len(reports) else 1


if __name__ == "__main__":
    sys.exit(main())
