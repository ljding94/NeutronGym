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


def gate_readout(by_bar, skipped, max_rate, n):
    """Per-bar gate verdict, or None when the run cannot support one.

    Two ways a run fails to support a verdict:

    1. A skipped instance is NOT a neutral loss. Calibration fails on the
       tightest instances -- precisely the ones no constant answer can solve
       -- so dropping them inflates the share a constant fails and flatters
       the family. A v5 SANS run had to be killed by hand for this.
    2. Too few instances. This compares the one-sided 95% upper limit against
       the ceiling, not the observed share: 5/25 = 20.0% reads as "at the
       ceiling" but its interval is [7%, 41%]. `underpowered` marks a run
       whose observed share is fine but which cannot certify it -- a
       different claim from the family being degenerate (2026-09-15).
    """
    if skipped:
        return None
    out = {}
    for b, v in by_bar.items():
        upper = hacks.binomial_upper_bound(v["passes"], n)
        out[b] = {"passes": v["passes"], "share": v["share"],
                  "upper_95": round(upper, 4),
                  "ok": upper <= max_rate,
                  "underpowered": v["share"] <= max_rate < upper}
    return out


_ENVS = {}


def _env(fam):
    """One env per worker process (the family binary is compiled by the parent
    before any worker starts, so workers never race on compilation)."""
    if fam not in _ENVS:
        _ENVS[fam] = NeutronGym(family=fam, split="heldout", target_fraction=1.0,
                                max_steps=10**9)
    return _ENVS[fam]


def _calibrate_one(job):
    fam, i = job
    _env(fam).reset(index=i)          # writes the calibration cache
    return i


def _score_one(job):
    """Ratios of every action to one instance's optimum (0.0 when invalid)."""
    fam, index, opt_fom, actions = job
    env = _env(fam)
    env.reset(index=index)
    out = []
    for a in actions:
        rec = env.step(dict(a))[4]
        ok = rec.get("level", 0) >= 3 and rec.get("fom")
        out.append(rec["fom"] / opt_fom if ok else 0.0)
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--family", required=True, choices=list(generate.FAMILIES))
    ap.add_argument("--n", type=int, default=25)
    ap.add_argument("--focus-bar", type=float, default=0.9,
                    help="bar the local refinement maximizes passes at")
    ap.add_argument("--refine-rounds", type=int, default=4)
    ap.add_argument("--out", default=None)
    ap.add_argument("--workers", type=int, default=1,
                    help="processes for calibration and scoring (both are "
                         "independent per instance)")
    args = ap.parse_args()
    fam = args.family
    out = args.out or os.path.join(REPO, "runs", "diagnostic", f"{fam}_v3_n{args.n}.json")
    os.makedirs(os.path.dirname(out), exist_ok=True)
    t0 = time.time()

    env = NeutronGym(family=fam, split="heldout", target_fraction=1.0, max_steps=10**9)
    ex = None
    if args.workers > 1:
        from concurrent.futures import ProcessPoolExecutor
        ex = ProcessPoolExecutor(max_workers=args.workers)
        for i in ex.map(_calibrate_one, [(fam, i) for i in range(args.n)]):
            print(f"  calibrated {i}  ({time.time() - t0:.0f}s)", flush=True)
    insts, skipped = [], []
    for i in range(args.n):
        obs, _ = env.reset(index=i)
        inst = obs["instance"]
        rec = json.load(open(calibrate.cache_path(os.path.join(env.workdir, fam), inst)))
        if not rec.get("ok"):
            skipped.append({"index": i, "reason": rec.get("reason")})
            print(f"  inst {i}: calibration failed ({rec.get('reason')}) — SKIPPED "
                  f"(biases this summary; see gate_readout)")
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
        # dedupe: several instances can share one optimum, so the same key
        # arrived several times and its ratios were appended once per copy
        # (450 scores for 150 instances), inflating that candidate's passes
        keys = [k for k in dict.fromkeys(keys) if pool[k]["ratios"] is None]
        for k in keys:
            pool[k]["ratios"] = []
        actions = [pool[k]["action"] for k in keys]
        jobs = [(fam, p["index"], p["opt_fom"], actions) for p in insts]
        results = ex.map(_score_one, jobs) if ex else map(_score_one, jobs)
        for ratios in results:           # map preserves instance order
            for k, r in zip(keys, ratios):
                pool[k]["ratios"].append(r)

    def passes(k, bar):
        assert len(pool[k]["ratios"]) == len(insts), (k, len(pool[k]["ratios"]))
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
        "n_requested": args.n,
        "skipped": skipped,
        "summary_trustworthy": not skipped,
        "gate_ok_at_bar": gate_readout(by_bar, skipped,
                                       hacks.CONSTANT_MAX_PASS_RATE, n),
    }
    with open(out, "w") as f:
        json.dump({"summary": summary, "instances": insts,
                   "pool": [{"action": v["action"], "source": v["source"],
                             "ratios": [round(r, 4) for r in v["ratios"]]} for v in pool.values()]},
                  f, indent=1)
    if skipped:
        print(f"\n!! {len(skipped)}/{args.n} instances SKIPPED — this summary is "
              f"biased toward the family and carries no gate verdict")
    print("\n=== SUMMARY ===")
    print(json.dumps(summary, indent=1))
    print(f"-> {os.path.relpath(out, REPO)}")


if __name__ == "__main__":
    main()
