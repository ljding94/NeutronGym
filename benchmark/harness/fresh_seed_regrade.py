"""Re-grade every passing design at fresh Monte-Carlo seeds (M8, 2026-09-24).

Grading is deterministic at the instance's protocol seed: the agent and the
target are compared under common random numbers, which removes simulation
noise from the comparison but means a design can sit just inside tolerance at
that one seed. The question this answers is how many passes survive when the
dice are re-rolled.

Both sides are re-simulated. The target is not a constant -- it is the hidden
design's measured observables -- so a fair re-grade re-measures the hidden
design at the new seed too, and compares the agent's design against THAT.
Comparing a fresh agent run against the old target would charge the agent for
noise in the target.

Tolerances come from the family's spec (per-observable where it defines them),
so this is the same bar the environment grades on.

Supersedes the 72/82 figure in `verify_match.py`, which sampled 60 instances
of the SELECTION slice at 2 seeds.

Usage:
  python benchmark/harness/fresh_seed_regrade.py \\
      --eval runs/m8/eval_match_fresh_trained-8b.json --arm trained-8b \\
      --family guide_match --split heldout --seeds 3 --workers 48 \\
      --out runs/m8/fresh_seed_regrade_guide_match.json
"""

import argparse
import json
import os
import sys
from concurrent.futures import ProcessPoolExecutor

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))

_ENV = {}


def _env(family, split):
    from neutrongym.env import NeutronGym
    key = (family, split)
    if key not in _ENV:
        _ENV[key] = NeutronGym(family=family, split=split, max_steps=10 ** 9)
    return _ENV[key]


def _regrade_one(job):
    """One instance, one seed: does the passing design still match?"""
    family, split, index, action, seed = job
    from neutrongym import reward
    env = _env(family, split)
    obs, _ = env.reset(index=index)
    inst = obs["instance"]
    ncount = inst["protocol"]["ncount"]
    ctx = inst["context"]
    tols = [spec.get("tolerance", inst["fom"]["tolerance"])
            for spec in inst["fom"]["match"]]
    try:
        out_h = env.exec.run({**ctx, **inst["hidden_action"]}, ncount=ncount, seed=seed)
        out_a = env.exec.run({**ctx, **action}, ncount=ncount, seed=seed)
    except Exception as e:                       # noqa: BLE001 - reported, not hidden
        return {"instance": index, "seed": seed, "usable": False,
                "error": f"{type(e).__name__}: {e}"}
    target = reward.match_measurements(inst, out_h["summary"])
    measured = reward.match_measurements(inst, out_a["summary"])
    if any(v is None for v in target) or any(v is None for v in measured):
        return {"instance": index, "seed": seed, "usable": False,
                "error": "observable unavailable (starved monitor)"}
    rel = [abs(m - t) / abs(t) for m, t in zip(measured, target)]
    return {"instance": index, "seed": seed, "usable": True,
            "holds": all(r <= tl for r, tl in zip(rel, tols)),
            "rel_err": [round(r, 6) for r in rel],
            "tolerances": tols,
            "worst_ratio": round(max(r / tl for r, tl in zip(rel, tols)), 4)}


def summarize(rows: list) -> dict:
    usable = [r for r in rows if r["usable"]]
    held = [r for r in usable if r["holds"]]
    by_inst = {}
    for r in usable:
        by_inst.setdefault(r["instance"], []).append(r["holds"])
    all_seeds = [i for i, v in by_inst.items() if all(v)]
    any_seed = [i for i, v in by_inst.items() if any(v)]
    return {
        "checks": len(rows),
        "usable_checks": len(usable),
        "unusable_checks": len(rows) - len(usable),
        "holds_per_check": len(held),
        "rate_per_check": round(len(held) / len(usable), 4) if usable else None,
        "designs": len(by_inst),
        "holds_at_every_seed": len(all_seeds),
        "rate_every_seed": round(len(all_seeds) / len(by_inst), 4) if by_inst else None,
        "holds_at_some_seed": len(any_seed),
        "fails_at_every_seed": len(by_inst) - len(any_seed),
    }


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--eval", required=True, help="eval json holding the passes")
    ap.add_argument("--arm", required=True)
    ap.add_argument("--family", required=True)
    ap.add_argument("--split", default="heldout")
    ap.add_argument("--seeds", type=int, default=3,
                    help="how many fresh seeds per design")
    ap.add_argument("--seed-base", type=int, default=90001)
    ap.add_argument("--workers", type=int, default=48)
    ap.add_argument("--out", default=None)
    a = ap.parse_args()

    with open(a.eval) as f:
        rec = json.load(f)
    rows = rec[a.split][a.arm][a.family]["rows"]
    passes = [r for r in rows if r.get("best_level") == 4 and r.get("pass_action")]
    print(f"{a.family}/{a.arm}: {len(passes)} passing designs with a recorded action "
          f"({sum(r.get('best_level') == 4 for r in rows)} passes total)")
    if not passes:
        raise SystemExit("nothing to re-grade")

    # seeds must differ from the protocol seed, else this measures nothing
    proto_seeds = set()
    env = _env(a.family, a.split)
    for r in passes[:1]:
        obs, _ = env.reset(index=r["instance"])
        proto_seeds.add(obs["instance"]["protocol"]["seed"])
    fresh = [a.seed_base + 1000 * k for k in range(a.seeds)]
    assert not (set(fresh) & proto_seeds), "fresh seed collides with the protocol seed"
    print(f"  fresh seeds: {fresh} (protocol seed sample: {sorted(proto_seeds)})")

    jobs = [(a.family, a.split, r["instance"], r["pass_action"], s)
            for r in passes for s in fresh]
    with ProcessPoolExecutor(max_workers=a.workers) as pool:
        out = list(pool.map(_regrade_one, jobs))

    s = summarize(out)
    print(f"  usable checks       {s['usable_checks']}/{s['checks']}")
    print(f"  holds per check     {s['holds_per_check']}/{s['usable_checks']} = "
          f"{100 * (s['rate_per_check'] or 0):.1f}%")
    print(f"  holds at EVERY seed {s['holds_at_every_seed']}/{s['designs']} = "
          f"{100 * (s['rate_every_seed'] or 0):.1f}%")
    print(f"  fails at every seed {s['fails_at_every_seed']}/{s['designs']}")

    if a.out:
        with open(a.out, "w") as f:
            json.dump({"eval": a.eval, "arm": a.arm, "family": a.family,
                       "split": a.split, "fresh_seeds": fresh,
                       "summary": s, "rows": out}, f, indent=1)
        print("->", a.out)


if __name__ == "__main__":
    main()
