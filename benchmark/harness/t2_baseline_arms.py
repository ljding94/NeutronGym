"""T2 baseline arms at matched compute (M6) — the "FOM vs baselines" record.

For each T2 task, re-verifies the calibration-era arms at HIGH statistics
and a FRESH seed (the winner's-curse discipline, one level up):

  initial          the task's unimproved baseline parameters
  classical_best   bounds+constraint-filtered classical ensemble winner
  random_search    constrained random search under the matched budget

Each arm re-runs at ncount 1e8 / fresh seed; the classical improvement must
hold at > 3 sigma (errors added in quadrature) or the record says so. The
committed output (benchmark/t2_baseline_arms.json) is the source for the
paper's T2 table columns; agent-arm columns join it from M6 episodes under
the same parametrization + eval budget (recorded here as matched_budget).

Usage: conda run -n mcstas python benchmark/harness/t2_baseline_arms.py
"""

import glob
import json
import math
import os
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))

import grader  # noqa: E402

from neutrongym.reward import get_observable  # noqa: E402

VERIFY_NCOUNT = 1e8
FRESH_SEED = 20260823


def fom_of(task, summary):
    mon = next((m for m in summary["monitors"]
                if m["component"] == task["fom"]["monitor"]), None)
    if mon is None:
        return None, None
    return (get_observable(mon, task["fom"]["metric"]),
            mon.get("intensity_err"))


def run_arm(task, params, label):
    instr = grader._resolve_instr(task["reference"]["instr"])
    out = grader.run_protocol(
        instr, params,
        {**task["protocol"], "ncount": VERIFY_NCOUNT, "seed": FRESH_SEED,
         "timeout": 3600},
        os.path.join(REPO, "runs", "t2_arms", task["id"]), label)
    if not out.get("ok"):
        return {"error": (out.get("diagnostics") or [])[-3:]}
    fom, err = fom_of(task, out)
    return {"fom": fom, "err": err, "params": params}


def main():
    record = {"verify_ncount": VERIFY_NCOUNT, "fresh_seed": FRESH_SEED,
              "tasks": {}}
    for p in sorted(glob.glob(os.path.join(REPO, "benchmark", "tasks", "T2",
                                           "T2_*.json"))):
        task = json.load(open(p))
        base = task["baselines"]
        defaults = {k: task["reference"]["parameters"].get(k)
                    for k in task["free_parameters"]}
        arms = {
            "initial": run_arm(task, {**task["reference"]["parameters"],
                                      **defaults}, "initial"),
            "classical_best": run_arm(
                task, {**task["reference"]["parameters"],
                       **base["classical_best"]["parameters"]}, "classical"),
        }
        rs = base.get("random_search", {})
        if rs.get("parameters") and \
                rs["parameters"] != base["classical_best"]["parameters"]:
            arms["random_search"] = run_arm(
                task, {**task["reference"]["parameters"],
                       **rs["parameters"]}, "random")
        else:
            arms["random_search"] = {**arms["classical_best"],
                                     "note": "identical to classical_best "
                                             "(optimizer arm was discarded "
                                             "for bounds escape)"}
        ok = all("fom" in a and a["fom"] is not None for a in arms.values())
        entry = {"arms": arms,
                 "matched_budget": task.get("calibration", {}),
                 "target_fom_min": task.get("targets", {}).get("fom_min")}
        if ok:
            delta = arms["classical_best"]["fom"] - arms["initial"]["fom"]
            sigma = math.sqrt((arms["classical_best"]["err"] or 0) ** 2
                              + (arms["initial"]["err"] or 0) ** 2)
            entry["improvement_sigma"] = round(delta / sigma, 1) if sigma \
                else None
            entry["verified_3sigma"] = bool(sigma and delta / sigma > 3)
            print(f"{task['id']:24} initial={arms['initial']['fom']:.6g} "
                  f"classical={arms['classical_best']['fom']:.6g} "
                  f"improvement={entry['improvement_sigma']}sigma "
                  f"[{'OK' if entry['verified_3sigma'] else 'BELOW 3sigma'}]")
        else:
            print(f"{task['id']:24} ARM FAILED: "
                  f"{ {k: a.get('error') for k, a in arms.items() if 'error' in a} }")
        record["tasks"][task["id"]] = entry
    out_path = os.path.join(REPO, "benchmark", "t2_baseline_arms.json")
    with open(out_path, "w") as f:
        json.dump(record, f, indent=1)
    print(f"-> {os.path.relpath(out_path, REPO)}")


if __name__ == "__main__":
    main()
