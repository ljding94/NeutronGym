"""Matched-compute classical search on a target-matching family (2026-09-18).

Pre-registered in note/prereg-matched-compute-frontier-ablation-2026-09-18.md.
The trained agent gets 10 turns, each costing one terminal simulation, so the
optimizers get exactly the same number of terminal simulations per instance.
Each minimises what the environment grades -- the worst relative error across
the instance's targets -- and passes an instance if ANY design it evaluated
within budget is an L4 pass.

Unlike the agent, these optimizers are handed the parametrization and operate
on numbers directly; the comparison is search efficiency per simulation, not
equivalent interfaces.

Usage:
  python benchmark/harness/classical_baseline.py --family guide_match \\
      --start 300 --n 300 --budget 10 --workers 48
"""

import argparse
import json
import random
from concurrent.futures import ProcessPoolExecutor

from neutrongym import generate, hacks
from neutrongym.env import NeutronGym

METHODS = ("random", "nelder-mead", "coordinate", "physics-coordinate")

# The strongest closed-form inversion the readout probe found for each family,
# used as the STARTING POINT for local search rather than as a one-shot answer.
# This is the arm a reviewer asks for first: a physicist who knows the optics
# does not guess from the baseline, they solve the formula and refine. Labels
# must match hacks.readout_rules exactly.
PHYSICS_SEED_RULE = {
    "guide_match": "physics: optics inversion kdiv=0.577 w_in=0.07",
}


def physics_start(family: str, inst: dict):
    """The family's best closed-form inversion for this instance, or None when
    no rule is registered or the formula has no solution in range."""
    label = PHYSICS_SEED_RULE.get(family)
    if label is None:
        return None
    for lbl, fn in hacks.readout_rules(family, inst["free_parameters"]):
        if lbl == label:
            return fn(inst["context"], inst)
    raise SystemExit(f"no rule named {label!r} for family {family}")


def worst_error(rec: dict):
    """Environment's own miss measure for a scored action; None if invalid."""
    m = rec.get("match")
    return max(m["rel_err"]) if m else None


class Budget:
    """Scores actions through the env, stopping after `budget` simulations."""

    def __init__(self, env, index, budget):
        self.env, self.index, self.budget = env, index, budget
        self.used, self.passed, self.best = 0, False, None

    def __call__(self, action):
        if self.used >= self.budget:
            return 1e6
        self.used += 1
        rec = self.env.step(dict(action))[4]
        err = worst_error(rec)
        if rec.get("level") == 4:
            self.passed = True
        if err is not None and (self.best is None or err < self.best):
            self.best = err
        return 1e6 if err is None else err       # invalid designs are worst


def _run(job):
    family, split, index, budget, method, seed = job
    env = NeutronGym(family=family, split=split, max_steps=10 ** 9)
    obs, _ = env.reset(index=index)
    inst = obs["instance"]
    free = inst["free_parameters"]
    names = list(free)
    b = Budget(env, index, budget)
    rng = random.Random(f"{method}/{index}/{seed}")

    if method == "random":
        while b.used < budget:
            b({k: round(rng.uniform(*free[k]), 6) for k in names})
    elif method == "nelder-mead":
        from scipy.optimize import minimize
        x0 = [inst["baseline"][k] for k in names]
        bounds = [tuple(free[k]) for k in names]

        def f(x):
            a = {k: float(min(max(v, free[k][0]), free[k][1])) for k, v in zip(names, x)}
            return b(a)

        minimize(f, x0, method="Nelder-Mead", bounds=bounds,
                 options={"maxfev": budget, "xatol": 1e-4, "fatol": 1e-4})
        while b.used < budget:                   # spend any leftover budget
            b({k: round(rng.uniform(*free[k]), 6) for k in names})
    else:                                        # coordinate / pattern search
        seeded = None
        if method == "physics-coordinate":
            seeded = physics_start(family, inst)
        cur = dict(seeded) if seeded else dict(inst["baseline"])
        b(cur)
        step = {k: (free[k][1] - free[k][0]) / 8 for k in names}
        best = b.best if b.best is not None else 1e6
        while b.used < budget:
            improved = False
            for k in names:
                for d in (+1, -1):
                    if b.used >= budget:
                        break
                    cand = dict(cur)
                    cand[k] = round(min(max(cur[k] + d * step[k], free[k][0]), free[k][1]), 6)
                    if cand[k] == cur[k]:
                        continue
                    e = b(cand)
                    if e < best:
                        best, cur, improved = e, cand, True
            if not improved:
                for k in names:
                    step[k] /= 2
    out = {"instance": index, "method": method, "passed": b.passed,
           "best_error": b.best, "sims_used": b.used}
    if method == "physics-coordinate":
        # records how often the formula even had a solution in range, so a
        # weak result cannot be blamed on the seed silently falling back
        out["physics_seeded"] = seeded is not None
    return out


def summarize(rows: list) -> dict:
    out = {}
    for m in sorted({r["method"] for r in rows}):
        sub = [r for r in rows if r["method"] == m]
        errs = [r["best_error"] for r in sub if r["best_error"] is not None]
        out[m] = {"n": len(sub), "passes": sum(r["passed"] for r in sub),
                  "pass_rate": round(sum(r["passed"] for r in sub) / len(sub), 4),
                  "median_best_error": round(sorted(errs)[len(errs) // 2], 4) if errs else None,
                  "median_sims": sorted(r["sims_used"] for r in sub)[len(sub) // 2]}
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--family", default="guide_match", choices=list(generate.FAMILIES))
    ap.add_argument("--split", default="heldout")
    ap.add_argument("--start", type=int, default=300)
    ap.add_argument("--n", type=int, default=300)
    ap.add_argument("--budget", type=int, default=10)
    ap.add_argument("--methods", default=",".join(METHODS))
    ap.add_argument("--workers", type=int, default=48)
    ap.add_argument("--seed", type=int, default=20260918)
    ap.add_argument("--out", default=None)
    a = ap.parse_args()
    NeutronGym(family=a.family, split=a.split)          # compile before forking
    methods = a.methods.split(",")
    jobs = [(a.family, a.split, i, a.budget, m, a.seed)
            for m in methods for i in range(a.start, a.start + a.n)]
    with ProcessPoolExecutor(max_workers=a.workers) as ex:
        rows = list(ex.map(_run, jobs, chunksize=4))
    s = summarize(rows)
    out = a.out or f"runs/m8/classical_{a.family}_budget{a.budget}_from{a.start}_n{a.n}.json"
    json.dump({"summary": s, "budget": a.budget, "start": a.start, "n": a.n,
               "rows": rows}, open(out, "w"), indent=1)
    for m, v in s.items():
        print(f"  {m:13} {v['passes']:4}/{v['n']} = {v['pass_rate']:.1%} | median worst error "
              f"{v['median_best_error']} | median sims {v['median_sims']}")
    print(f"-> {out}")


if __name__ == "__main__":
    main()
