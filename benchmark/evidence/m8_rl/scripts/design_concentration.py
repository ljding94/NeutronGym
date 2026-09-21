"""How concentrated are a policy's passing designs? (M8, 2026-09-21)

The constant-policy gate bounds what a single FIXED design can pass. It
cannot bound what a *family* of designs can pass, and that gap let a
degenerate slice through: 24.7% of `bender` v1 instances had a hidden
cutoff below the incident band, so the bender did nothing and the targets
were just the source spectrum's own mean and spread -- solvable by ANY
sufficiently transparent design. No single candidate the gate tried scored
high, so the gate passed the family at 10.7%.

What exposed it was the trained policy itself: 42 distinct designs for 141
passes, 43% of passes from five designs, and one geometry passing 17
instances whose target means spanned nearly the whole held-out range. A
design that solves instances with very different targets is not solving
them through the physics.

**Read it against the family's own constant gate, not against another
family.** Concentration is bounded below by the task's effective degrees of
freedom: `bender`'s two observables are both set by the single cutoff
lam_c, so one design necessarily covers several instances and the trained
policy sits at 28% distinct where `guide_match` (two independent targets)
reaches 98%. The v2 family is sound anyway -- its most-reused design covers
6.0% of instances, BELOW the 8.0% the constant gate's best fixed design
reaches, and the instances it solves cluster 5x more tightly in lam_c than
the population. What condemned v1 was not the 30% itself but that one
design spanned nearly the whole target range while cutting none of the
band. So: low concentration is a flag to investigate, not a verdict.

So this is the diagnostic, and it belongs on every family:

  distinct   passing designs / passes         (1.0 = fully instance-specific)
  top5       share of passes from 5 designs
  reuse      instances solved by the single most-reused design
  span       how much of the target range that design covers

Usage:
  python benchmark/harness/design_concentration.py \\
      --family bender --eval runs/m8/eval_bender_fresh_trained-8b.json \\
      --arm trained-8b [--split heldout] [--out runs/m8/concentration.json]
"""

import argparse
import collections
import json


def _key(action) -> str:
    return json.dumps(action, sort_keys=True)


def passing_designs(rows: list) -> list:
    """(instance, design) for every passing row that recorded its design."""
    return [(r["instance"], _key(r["pass_action"])) for r in rows
            if r.get("best_level", 0) >= 4 and r.get("pass_action")]


def concentration(rows: list, top_n: int = 5) -> dict:
    """Concentration statistics for one arm on one family."""
    pairs = passing_designs(rows)
    passes = len(pairs)
    counts = collections.Counter(d for _, d in pairs)
    if not passes:
        return {"passes": 0, "distinct": 0, "distinct_frac": None,
                "top_n": top_n, "top_n_share": None, "max_reuse": 0,
                "max_reuse_design": None, "max_reuse_instances": []}
    top = counts.most_common(top_n)
    best_design, best_n = top[0]
    return {
        "passes": passes,
        "distinct": len(counts),
        "distinct_frac": round(len(counts) / passes, 4),
        "top_n": top_n,
        "top_n_share": round(sum(n for _, n in top) / passes, 4),
        "max_reuse": best_n,
        "max_reuse_design": json.loads(best_design),
        "max_reuse_instances": [i for i, d in pairs if d == best_design],
    }


def target_span(instances: dict, idx: list) -> dict | None:
    """Range of targets covered by one design's instances, as a fraction of
    the range covered by all evaluated instances. A design solving instances
    across most of the target range is not using the physics."""
    if not idx or not instances:
        return None
    n_obs = len(next(iter(instances.values())))
    out = []
    for j in range(n_obs):
        mine = [instances[i][j] for i in idx if i in instances]
        every = [t[j] for t in instances.values()]
        # an observable that is constant across instances carries no signal;
        # skip it rather than voiding the report, since a family can match a
        # varying quantity and a fixed one together
        if not mine or max(every) == min(every):
            continue
        out.append({
            "observable": j,
            "design_range": [round(min(mine), 6), round(max(mine), 6)],
            "all_range": [round(min(every), 6), round(max(every), 6)],
            "span_fraction": round((max(mine) - min(mine))
                                   / (max(every) - min(every)), 4),
        })
    if not out:
        return None
    return {"observables": out,
            "max_span_fraction": max(o["span_fraction"] for o in out)}


def load_rows(path: str, arm: str, family: str, split: str = "heldout") -> list:
    with open(path) as f:
        d = json.load(f)
    return d[split][arm][family]["rows"]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--family", required=True)
    ap.add_argument("--eval", required=True, help="one eval json, or several comma-separated")
    ap.add_argument("--arm", required=True, help="arm name, or several comma-separated")
    ap.add_argument("--split", default="heldout")
    ap.add_argument("--targets", action="store_true",
                    help="also report the target span of the most-reused design "
                         "(needs the McStas env; skipped when unavailable)")
    ap.add_argument("--out")
    a = ap.parse_args()

    paths = [p.strip() for p in a.eval.split(",") if p.strip()]
    arms = [x.strip() for x in a.arm.split(",") if x.strip()]
    if len(arms) == 1:
        arms *= len(paths)

    report = {"family": a.family, "split": a.split, "arms": {}}
    for path, arm in zip(paths, arms):
        rows = load_rows(path, arm, a.family, a.split)
        rec = concentration(rows)
        rec["eval"] = path
        if a.targets and rec["passes"]:
            rec["target_span"] = _span_via_env(a.family, a.split, rows, rec)
        report["arms"][arm] = rec
        print(f"{a.family:13s} {arm:14s} passes {rec['passes']:3d}  "
              f"distinct {rec['distinct']:3d} ({_pct(rec['distinct_frac'])})  "
              f"top5 {_pct(rec['top_n_share'])}  max-reuse {rec['max_reuse']}")
        span = rec.get("target_span")
        if span:
            print(f"{'':28s}most-reused design covers "
                  f"{_pct(span['max_span_fraction'])} of the target range")

    if a.out:
        with open(a.out, "w") as f:
            json.dump(report, f, indent=1)
        print("->", a.out)


def _pct(x):
    return "-" if x is None else f"{100 * x:.0f}%"


def _span_via_env(family, split, rows, rec):
    from neutrongym.env import NeutronGym
    env = NeutronGym(family=family, split=split, target_fraction=0.85, max_steps=10)
    targets = {}
    for r in rows:
        obs, _ = env.reset(index=r["instance"])
        t = obs["instance"].get("targets")
        if t:
            targets[r["instance"]] = t
    return target_span(targets, rec["max_reuse_instances"])


if __name__ == "__main__":
    main()
