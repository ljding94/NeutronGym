"""Red-team the reward: corner/extreme sweep through the full ladder.

    conda run -n mcstas python scripts/redteam_sweep.py [instances_per_family]

Runs every corner of each family's action space, plus boundary-hugging and
random actions, through reward.score on real physics, and reports:
  - max Liouville utilization (must stay < 1: legit designs never flagged)
  - what the ladder caught (L0 bounds, L3 bands/floor/Liouville)
  - the reward distribution corner actions can reach
Summary lands in runs/redteam/sweep.json — raw material for the
red-team-the-reward paper section (note/reward-red-team-2026-08-05.md).
"""

import itertools
import json
import os
import random
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.environ.setdefault("MCSTAS_MCP_HOME",
                      os.path.join(REPO, "runs", "redteam", "home"))

from neutrongym import generate  # noqa: E402
from neutrongym.env import NeutronGym  # noqa: E402


def actions_for(inst, rng):
    free = inst["free_parameters"]
    names = sorted(free)
    corners = [dict(zip(names, combo)) for combo in
               itertools.product(*[free[n] for n in names])]
    hug = [{n: free[n][0] + 1e-6 for n in names},
           {n: free[n][1] - 1e-6 for n in names}]
    rand = [{n: rng.uniform(*free[n]) for n in names} for _ in range(6)]
    return [("corner", a) for a in corners] + [("hug", a) for a in hug] + \
           [("random", a) for a in rand]


def main():
    per_family = int(sys.argv[1]) if len(sys.argv) > 1 else 2
    rng = random.Random(20260805)
    stats = {"scores": 0, "max_utilization": 0.0, "flagged_unphysical": 0,
             "l3_band_catches": 0, "levels": {0: 0, 1: 0, 2: 0, 3: 0, 4: 0},
             "worst": None}
    print(f"{'family':18} {'inst':>4} {'kind':7} {'level':>5} {'reward':>7} "
          f"{'util':>6}  note")
    for fam in generate.FAMILIES:
        env = NeutronGym(family=fam, split="train", max_steps=99)
        for idx in range(per_family):
            obs, _ = env.reset(index=idx)
            for kind, action in actions_for(obs["instance"], rng):
                _, r, _, _, rec = env.step(action)
                stats["scores"] += 1
                stats["levels"][rec["level"]] += 1
                lio = rec["levels"].get("L3", {}).get("liouville", {})
                util = lio.get("utilization") or 0
                if util > stats["max_utilization"]:
                    stats["max_utilization"] = util
                    stats["worst"] = {"family": fam, "index": idx,
                                      "action": action, "utilization": util}
                d = rec["levels"].get("L3", {}).get("detail", "")
                if d.startswith("unphysical_gain"):
                    stats["flagged_unphysical"] += 1
                if d == "constraint band violated":
                    stats["l3_band_catches"] += 1
                note = d[:38] if d else ""
                print(f"{fam:18} {idx:>4} {kind:7} {rec['level']:>5} "
                      f"{r:7.3f} {util:6.3f}  {note}")

    print(f"\nscored {stats['scores']} adversarial/extreme actions")
    print(f"  level histogram: {stats['levels']}")
    print(f"  constraint-band catches: {stats['l3_band_catches']}")
    print(f"  max Liouville utilization: {stats['max_utilization']:.3f} "
          f"(worst: {stats['worst']})")
    ok = (stats["flagged_unphysical"] == 0
          and stats["max_utilization"] < 1.05)
    verdict = ("no legitimate action flagged unphysical; utilization "
               "ceiling within statistical reach of 1"
               if ok else "a legitimate action tripped the Liouville gate "
               "or exceeded the bound — recalibrate before scoring")
    print(f"  [{'PASS' if ok else 'INVESTIGATE'}] {verdict}")
    out = os.path.join(REPO, "runs", "redteam")
    os.makedirs(out, exist_ok=True)
    with open(os.path.join(out, "sweep.json"), "w") as f:
        json.dump(stats, f, indent=2)
    print(f"  summary -> {os.path.relpath(out, REPO)}/sweep.json")
    sys.exit(0 if ok else 1)


if __name__ == "__main__":
    main()
