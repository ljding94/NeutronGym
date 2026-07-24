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


def validate(task_path: str) -> dict:
    with open(task_path) as f:
        task = json.load(f)
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
