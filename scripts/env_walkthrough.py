"""NeutronGym environment walkthrough + M5 acceptance run.

    conda run -n mcstas python scripts/env_walkthrough.py [n_instances]

1. Compiles both template families (compile-once cache).
2. Fast-tier timing table (the measure_fast_tier role, now env-native):
   direct-binary rollouts/s per core at several ncount values.
3. THE ACCEPTANCE RUN (PLAN.md M5): >= 100 fresh procedural instances
   (train + held-out, both families) through the gym reset/step loop with a
   mixed action policy; verifies level-resolved fields in every record and
   prints the level histogram. Summary JSON lands in runs/env_acceptance/.
"""

import json
import os
import random
import statistics
import sys
import time

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.environ.setdefault("MCSTAS_MCP_HOME",
                      os.path.join(REPO, "runs", "env_acceptance", "home"))

from neutrongym import generate  # noqa: E402
from neutrongym.env import NeutronGym  # noqa: E402


def action_policy(rng, inst, kind):
    free = inst["free_parameters"]
    if kind == "random":
        return {k: round(rng.uniform(lo, hi), 6) for k, (lo, hi) in free.items()}
    if kind == "baseline":
        return dict(inst["baseline"])
    return {k: hi * 3 for k, (lo, hi) in free.items()}  # out-of-bounds -> L1


def main():
    n_target = int(sys.argv[1]) if len(sys.argv) > 1 else 104
    out_dir = os.path.join(REPO, "runs", "env_acceptance")
    os.makedirs(out_dir, exist_ok=True)
    rng = random.Random(20260731)

    print("=== 1. Family compilation (compile-once cache) ===")
    envs = {}
    for fam in generate.FAMILIES:
        t0 = time.time()
        for split in ("train", "heldout"):
            envs[fam, split] = NeutronGym(family=fam, split=split, max_steps=4)
        print(f"  {fam:22} ready in {time.time()-t0:5.2f} s "
              f"(binary shared across splits)")

    print("\n=== 2. Fast-tier throughput (direct binary, per core) ===")
    fe = envs["guide_divergence", "train"].exec
    inst = generate.instance("guide_divergence", "train", 0)
    params = {**inst["context"], **inst["baseline"]}
    print(f"  {'ncount':>8} {'median':>9} {'p90':>9} {'rollouts/s':>11}")
    throughput_1e5 = None
    for ncount in (1e4, 1e5):
        times = []
        for i in range(15):
            r = fe.run(params, ncount=ncount, seed=100 + i)
            assert r["ok"], r
            times.append(r["elapsed_s"])
        med = statistics.median(times)
        p90 = sorted(times)[13]
        if ncount == 1e5:
            throughput_1e5 = 1 / med
        print(f"  {ncount:8.0e} {med:8.3f}s {p90:8.3f}s {1/med:11.1f}")

    print(f"\n=== 3. Acceptance: {n_target} fresh instances through "
          "reset/step ===")
    per_cell = n_target // 4
    hist = {0: 0, 1: 0, 2: 0, 3: 0, 4: 0}
    n_inst = n_steps = 0
    missing_fields = []
    t0 = time.time()
    for fam in generate.FAMILIES:
        for split in ("train", "heldout"):
            env = envs[fam, split]
            for idx in range(per_cell):
                obs, info = env.reset(index=idx)
                n_inst += 1
                for kind in ("random", "baseline", "outlaw"):
                    _, r, term, trunc, rec = env.step(
                        action_policy(rng, obs["instance"], kind))
                    n_steps += 1
                    hist[rec["level"]] += 1
                    for field in ("level", "levels", "reward", "instance",
                                  "action", "elapsed_s"):
                        if field not in rec:
                            missing_fields.append((rec.get("instance"), field))
                ep = env.episode_record()
                assert len(ep["steps"]) == 3 and ep["baseline"]["fom"]
    wall = time.time() - t0
    print(f"  {n_inst} instances x 3 steps = {n_steps} scored actions "
          f"in {wall:.1f} s ({n_steps/wall:.1f} scores/s incl. baselines)")

    print("\n  deepest-level-reached histogram (mixed policy: "
          "random / baseline / out-of-bounds):")
    labels = {0: "L0 rejected (syntax/bounds)", 1: "L1 static only",
              2: "L2 ran, structure failed", 3: "L3 valid, no improvement",
              4: "L4 improved on baseline"}
    for lv, n in hist.items():
        bar = "#" * int(40 * n / max(n_steps, 1))
        print(f"    {labels[lv]:34} {n:5}  {bar}")

    ok_fields = not missing_fields
    ok_count = n_inst >= 100
    ok_speed = throughput_1e5 and throughput_1e5 >= 10
    print("\n=== Acceptance verdict ===")
    print(f"  [{'PASS' if ok_count else 'FAIL'}] >=100 fresh procedural "
          f"instances end-to-end ({n_inst})")
    print(f"  [{'PASS' if ok_fields else 'FAIL'}] level-resolved fields in "
          f"every episode record")
    print(f"  [{'PASS' if ok_speed else 'FAIL'}] fast tier >= 10 rollouts/s/"
          f"core at 1e5 ({throughput_1e5:.1f}/s)")

    summary = {"date": time.strftime("%Y-%m-%d %H:%M"), "instances": n_inst,
               "scored_actions": n_steps, "level_histogram": hist,
               "rollouts_per_s_1e5": round(throughput_1e5 or 0, 1),
               "wall_s": round(wall, 1),
               "pass": bool(ok_count and ok_fields and ok_speed)}
    with open(os.path.join(out_dir, "acceptance.json"), "w") as f:
        json.dump(summary, f, indent=2)
    print(f"\n  summary -> {os.path.relpath(out_dir, REPO)}/acceptance.json")
    sys.exit(0 if summary["pass"] else 1)


if __name__ == "__main__":
    main()
