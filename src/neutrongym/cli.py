"""`neutrongym-eval` — the one-command eval slice of the environment.

Runs fresh procedural instances (both families, train + held-out regimes)
through the gym reset/step loop with a mixed action policy, re-measures
fast-tier throughput, and prints the level histogram + the acceptance
verdict (the same bar as PLAN.md M5's environment acceptance). Exits 0 on
PASS. This is the post-install smoke of the released artifact:

    pip install neutrongym    # McStas 3.x via conda-forge is a prerequisite
    neutrongym-eval --instances 24
"""

import argparse
import json
import os
import random
import statistics
import sys
import time


def action_policy(rng, inst, kind):
    free = inst["free_parameters"]
    if kind == "random":
        return {k: round(rng.uniform(lo, hi), 6)
                for k, (lo, hi) in free.items()}
    if kind == "baseline":
        return dict(inst["baseline"])
    return {k: hi * 3 for k, (lo, hi) in free.items()}  # out-of-bounds -> L1


def throughput_probe(fexec, inst: dict, n: int = 10):
    """-> (rollouts_per_s, ncount, diagnostics). Probes at the INSTANCE's own
    protocol: ncount is per family since 2026-09-15 (SANS runs 8e5, the guide
    1e5), so a hardcoded 1e5 here both understated SANS cost by 8x and
    mislabelled the figure it printed."""
    ncount = inst["protocol"]["ncount"]
    times = []
    for i in range(n):
        r = fexec.run({**inst["context"], **inst["baseline"]},
                      ncount=ncount, seed=100 + i)
        if not r["ok"]:
            return 0.0, ncount, r.get("diagnostics")
        times.append(r["elapsed_s"])
    return (1 / statistics.median(times) if times else 0.0), ncount, None


def run_eval(n_instances: int = 24, max_steps: int = 4,
             json_out: str | None = None) -> dict:
    from . import generate
    from .env import NeutronGym

    rng = random.Random(20260823)
    per_cell = max(1, n_instances // (2 * len(generate.FAMILIES)))
    print("=== NeutronGym eval slice ===")
    envs = {}
    for fam in generate.FAMILIES:
        t0 = time.time()
        for split in ("train", "heldout"):
            envs[fam, split] = NeutronGym(family=fam, split=split,
                                          max_steps=max_steps)
        print(f"  {fam:22} ready in {time.time() - t0:5.2f} s")

    fe = envs[next(iter(generate.FAMILIES)), "train"].exec
    inst0 = generate.instance(next(iter(generate.FAMILIES)), "train", 0)
    rps, ncount, diag = throughput_probe(fe, inst0)
    if diag is not None:
        print(f"  throughput probe failed: {diag}")
        rps = 0.0
    print(f"  fast tier: {rps:.1f} rollouts/s/core at {ncount:g}")

    hist = {0: 0, 1: 0, 2: 0, 3: 0, 4: 0}
    n_inst = n_steps = 0
    missing = 0
    t0 = time.time()
    for (fam, split), env in envs.items():
        for idx in range(per_cell):
            obs, _ = env.reset(index=idx)
            n_inst += 1
            for kind in ("random", "baseline", "outlaw"):
                _, _, _, _, rec = env.step(
                    action_policy(rng, obs["instance"], kind))
                n_steps += 1
                hist[rec["level"]] += 1
                if any(f not in rec for f in
                       ("level", "levels", "reward", "elapsed_s")):
                    missing += 1
    wall = time.time() - t0
    print(f"  {n_inst} instances x 3 actions = {n_steps} scores in "
          f"{wall:.1f} s")
    print("  level histogram: " + json.dumps(hist))

    ok = missing == 0 and rps >= 10 and n_inst >= min(n_instances, 4)
    print(f"  [{'PASS' if ok else 'FAIL'}] level-resolved records + "
          f">=10 rollouts/s/core")
    summary = {"instances": n_inst, "scores": n_steps,
               "level_histogram": hist,
               "rollouts_per_s": round(rps, 1), "pass": bool(ok)}
    if json_out:
        with open(json_out, "w") as f:
            json.dump(summary, f, indent=2)
    return summary


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--instances", type=int, default=24)
    ap.add_argument("--json", default=None, help="write summary JSON here")
    args = ap.parse_args()
    if not os.environ.get("MCSTAS_MCP_HOME"):
        # keep eval state out of ~/.mcstas-mcp for casual runs
        os.environ["MCSTAS_MCP_HOME"] = os.path.join(
            os.path.expanduser("~"), ".neutrongym-eval")
    summary = run_eval(args.instances, json_out=args.json)
    sys.exit(0 if summary["pass"] else 1)


if __name__ == "__main__":
    main()
