"""Readout-rule degeneracy probe, in parallel (2026-09-17).

Scores every hacks.readout_rules policy on held-out instances and reports the
best rule's pass share with its one-sided 95% upper limit, like the constant
probe. A family is clean only if no rule that reads the stated limits passes
more than the ceiling.

Usage: python benchmark/harness/readout_probe.py --family guide_divergence \\
          --n 150 --workers 16 --target-fraction 0.85
"""

import argparse
import json
import os
from concurrent.futures import ProcessPoolExecutor

from neutrongym import generate, hacks
from neutrongym.env import NeutronGym


def _chunk(job):
    fam, frac, idxs = job
    env = NeutronGym(family=fam, split="heldout", target_fraction=frac, max_steps=10**9)
    rules = hacks.readout_rules(fam, generate.FAMILIES[fam]["free_parameters"])
    return hacks.readout_policy_probe(env, idxs, rules)


def merge(parts: list) -> dict:
    n = sum(p["n_instances"] for p in parts)
    passes = {}
    for p in parts:
        for r in p["results"]:
            passes[r["action"]] = passes.get(r["action"], 0) + r["passes"]
    return {"n_instances": n, "skipped_no_headroom": sum(p["skipped_no_headroom"] for p in parts),
            "results": [{"action": k, "passes": v, "pass_rate": round(v / n, 4) if n else None,
                         "source": "readout"} for k, v in passes.items()]}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--family", required=True, choices=list(generate.FAMILIES))
    ap.add_argument("--n", type=int, default=150)
    ap.add_argument("--workers", type=int, default=16)
    ap.add_argument("--target-fraction", type=float, default=0.85)
    ap.add_argument("--out", default=None)
    a = ap.parse_args()
    NeutronGym(family=a.family, split="heldout")        # compile before forking
    idx = list(range(a.n))
    chunks = [idx[k::a.workers] for k in range(a.workers) if idx[k::a.workers]]
    with ProcessPoolExecutor(max_workers=a.workers) as ex:
        probe = merge(list(ex.map(_chunk, [(a.family, a.target_fraction, c) for c in chunks])))
    copy, phys = hacks.split_rule_results(probe)
    v = hacks.summarize_constant_probe(copy) if copy["results"] else None
    vp = hacks.summarize_constant_probe(phys) if phys["results"] else None
    top = sorted(probe["results"], key=lambda r: -r["passes"])[:5]
    out = a.out or f"runs/m8/readout_probe_{a.family}_{a.target_fraction}_n{a.n}.json"
    json.dump({"verdict": v, "physics_reference": vp, "probe": probe}, open(out, "w"), indent=1)
    if v:
        print(f"{a.family} @ {a.target_fraction}x, n={probe['n_instances']}: best COPY-type readout rule "
              f"{v['best_action']!r} passes {v['best_pass_rate']:.1%} (upper95 "
              f"{v['best_pass_rate_upper']:.1%}) -> {'CLEAN' if v['ok'] else ('underpowered' if v['underpowered'] else 'DEGENERATE')}")
    else:
        print(f"{a.family}: no copy-type readout rules apply -> CLEAN on this check")
    if vp:
        print(f"   physics-model reference (reported, not gated): {vp['best_action']!r} "
              f"passes {vp['best_pass_rate']:.1%} (upper95 {vp['best_pass_rate_upper']:.1%})")
    for r in top:
        print(f"   {r['passes']:4}/{probe['n_instances']}  {r['action']}")
    print(f"-> {out}")


if __name__ == "__main__":
    main()
