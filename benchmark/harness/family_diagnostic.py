"""Can a fixed answer solve a procedural family? Measured at every bar.

After calibration v2 (2026-09-15) every instance's target comes from a strong
classical optimum scored exactly like an agent (reward.score, protocol seed),
so a candidate action passes bar f on an instance iff

    FOM(action) > f * FOM(classical optimum)          (common random numbers)

This script, for N held-out instances of one family:
  1. reads each instance's calibrated optimum (running calibration if needed)
  2. scores a pool of FIXED candidates on every instance, as a ratio to that
     instance's optimum (0 when the candidate is invalid there):
       grid       5-level grid over the free parameters + baseline
       classical  every probed instance's optimum, reused on all instances
       refined    local search around the best candidate at the focus bar
  3. reports, per bar, the share of instances the best single candidate
     passes, plus how far optima spread and which parameters matter

A family can separate design skill from lookup only at bars where the best
fixed answer passes few instances while each instance's own optimum passes
by construction (ratio exactly 1.0 > f for every f < 1).

Usage:
  python benchmark/harness/family_diagnostic.py --family sans_collimation --n 25
"""

import argparse
import json
import os
import statistics
import sys
import time

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))

from neutrongym import calibrate, generate, hacks  # noqa: E402
from neutrongym.env import NeutronGym  # noqa: E402

BARS = (0.8, 0.85, 0.9, 0.95)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--family", required=True, choices=list(generate.FAMILIES))
    ap.add_argument("--n", type=int, default=25)
    ap.add_argument("--focus-bar", type=float, default=0.9,
                    help="bar the local refinement maximizes passes at")
    ap.add_argument("--refine-rounds", type=int, default=4)
    ap.add_argument("--out", default=None)
    args = ap.parse_args()
    fam = args.family
    out = args.out or os.path.join(REPO, "runs", "diagnostic", f"{fam}_v2_n{args.n}.json")
    os.makedirs(os.path.dirname(out), exist_ok=True)
    t0 = time.time()

    env = NeutronGym(family=fam, split="heldout", target_fraction=1.0, max_steps=10**9)
    insts = []
    for i in range(args.n):
        obs, _ = env.reset(index=i)
        inst = obs["instance"]
        rec = json.load(open(calibrate.cache_path(os.path.join(env.workdir, fam), inst)))
        if not rec.get("ok"):
            print(f"  inst {i}: calibration failed ({rec.get('reason')}) — skipped")
            continue
        insts.append({"index": i, "opt_action": rec["classical_action"],
                      "opt_fom": rec["classical_fom"], "baseline_fom": obs["baseline_fom"],
                      "opt_over_baseline": rec["classical_over_baseline"],
                      "evals": rec.get("evals"), "context": inst["context"]})
        print(f"  inst {i:2}: optimum {rec['classical_action']}  {rec['classical_over_baseline']:.3f}x baseline "
              f"({rec.get('evals')} evals, {time.time() - t0:.0f}s)", flush=True)

    free = generate.FAMILIES[fam]["free_parameters"]
    pool = {}      # key -> {"action", "source", "ratios": [per instance]}

    def add(action, source):
        k = hacks._key(action)
        if k not in pool:
            pool[k] = {"action": dict(action), "source": source, "ratios": None}
        return k

    def score(keys):
        keys = [k for k in keys if pool[k]["ratios"] is None]
        for k in keys:
            pool[k]["ratios"] = []
        for p in insts:
            env.reset(index=p["index"])
            for k in keys:
                rec = env.step(dict(pool[k]["action"]))[4]
                ok = rec.get("level", 0) >= 3 and rec.get("fom")
                pool[k]["ratios"].append(rec["fom"] / p["opt_fom"] if ok else 0.0)

    def passes(k, bar):
        return sum(r > bar + 1e-9 for r in pool[k]["ratios"])

    grid = hacks.constant_candidates(generate.instance(fam, "heldout", 0))
    score([add(a, "grid") for a in grid] + [add(p["opt_action"], "classical") for p in insts])
    best = max(pool, key=lambda k: passes(k, args.focus_bar))
    frac = 1.0 / 8
    for _ in range(args.refine_rounds):
        neigh = [add(a, "refined") for a in hacks._neighbourhood(pool[best]["action"], free, frac)]
        score(neigh)
        challenger = max(pool, key=lambda k: passes(k, args.focus_bar))
        if passes(challenger, args.focus_bar) > passes(best, args.focus_bar):
            best = challenger
        else:
            frac /= 2

    n = len(insts)
    by_bar = {}
    for bar in BARS:
        k = max(pool, key=lambda kk: passes(kk, bar))
        by_bar[str(bar)] = {"best_constant": pool[k]["action"], "source": pool[k]["source"],
                            "passes": passes(k, bar), "share": round(passes(k, bar) / n, 3)}
    # how much of each instance's optimum does the best-at-focus constant reach
    focus_k = max(pool, key=lambda kk: passes(kk, args.focus_bar))
    focus_ratios = pool[focus_k]["ratios"]
    spread = {k: [round(min(p["opt_action"][k] for p in insts), 6),
                  round(max(p["opt_action"][k] for p in insts), 6)] for k in free}
    summary = {
        "family": fam, "n": n, "wall_s": round(time.time() - t0), "n_candidates": len(pool),
        "optimum_over_baseline_median": round(statistics.median(p["opt_over_baseline"] for p in insts), 3),
        "optimum_spread": spread,
        "best_constant_pass_share_by_bar": by_bar,
        "focus_bar": args.focus_bar,
        "focus_constant": pool[focus_k]["action"],
        "focus_constant_ratio_to_optimum": {
            "median": round(statistics.median(focus_ratios), 4),
            "min": round(min(focus_ratios), 4), "max": round(max(focus_ratios), 4)},
        "gate_ok_at_bar": {str(b): by_bar[str(b)]["share"] <= hacks.CONSTANT_MAX_PASS_RATE for b in BARS},
    }
    with open(out, "w") as f:
        json.dump({"summary": summary, "instances": insts,
                   "pool": [{"action": v["action"], "source": v["source"],
                             "ratios": [round(r, 4) for r in v["ratios"]]} for v in pool.values()]},
                  f, indent=1)
    print("\n=== SUMMARY ===")
    print(json.dumps(summary, indent=1))
    print(f"-> {os.path.relpath(out, REPO)}")


if __name__ == "__main__":
    main()
