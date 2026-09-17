"""Parallel constant-design gate for a target-matching family (2026-09-17).

Candidates: the 5-level grid over the free parameters, the baseline, and
every probed instance's hidden design (so a lookup of another instance's
solution is tested), plus one round of neighbourhood refinement around the
best. Verdict on the one-sided 95% upper limit, like every other gate.

Usage: python benchmark/harness/match_gate.py --family guide_match --n 150 --workers 48
"""

import argparse
import json
from concurrent.futures import ProcessPoolExecutor

from neutrongym import generate, hacks
from neutrongym.env import NeutronGym


def _chunk(job):
    fam, idxs, cands = job
    env = NeutronGym(family=fam, split="heldout", max_steps=10**9)
    out = {}
    for i in idxs:
        obs, _ = env.reset(index=i)
        out[i] = [env.step(dict(a))[4].get("level") == 4 for a in cands]
    return out


def score(fam, idxs, cands, workers):
    chunks = [idxs[k::workers] for k in range(workers) if idxs[k::workers]]
    with ProcessPoolExecutor(max_workers=workers) as ex:
        merged = {}
        for part in ex.map(_chunk, [(fam, c, cands) for c in chunks]):
            merged.update(part)
    return [sum(merged[i][j] for i in idxs) for j in range(len(cands))]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--family", default="guide_match")
    ap.add_argument("--n", type=int, default=150)
    ap.add_argument("--workers", type=int, default=48)
    ap.add_argument("--out", default=None)
    a = ap.parse_args()
    idxs = list(range(a.n))
    env = NeutronGym(family=a.family, split="heldout", max_steps=10**9)
    hidden = []
    for i in idxs:                                   # calibration is cached by precalibrate
        obs, _ = env.reset(index=i)
        hidden.append(dict(obs["instance"]["hidden_action"]))
    inst0 = generate.instance(a.family, "heldout", 0)
    grid = hacks.constant_candidates(inst0)
    cands = grid + hidden
    passes = score(a.family, idxs, cands, a.workers)
    best = max(range(len(cands)), key=lambda j: passes[j])
    neigh = hacks._neighbourhood(cands[best], inst0["free_parameters"], 1 / 16)
    npass = score(a.family, idxs, neigh, a.workers)
    pool = [{"action": c, "passes": p, "pass_rate": round(p / a.n, 4),
             "source": ("baseline" if c == inst0["baseline"] else "grid") if j < len(grid) else "hidden-of-other-instance"}
            for j, (c, p) in enumerate(zip(cands, passes))]
    pool += [{"action": c, "passes": p, "pass_rate": round(p / a.n, 4), "source": "refined"}
             for c, p in zip(neigh, npass)]
    v = hacks.summarize_constant_probe({"n_instances": a.n, "results": pool})
    by_src = {}
    for r in pool:
        by_src[r["source"]] = max(by_src.get(r["source"], 0), r["passes"])
    print(f"{a.family} constant gate, n={a.n}, {len(pool)} candidates: best {v['best_action']} "
          f"[{v['best_source']}] passes {v['best_pass_rate']:.1%} (upper95 {v['best_pass_rate_upper']:.1%}); "
          f"baseline passes {v['baseline_passes']} -> {'CLEAN' if v['ok'] else ('underpowered' if v['underpowered'] else 'DEGENERATE')}")
    print("   best pass count by source:", by_src)
    out = a.out or f"runs/m8/match_gate_{a.family}_n{a.n}.json"
    json.dump({"verdict": v, "pool": pool}, open(out, "w"), indent=1)
    print(f"-> {out}")


if __name__ == "__main__":
    main()
