"""M8 phase 3 — trained vs untrained 8B vs untrained 32B, one protocol.

Every policy is measured by the same `rollouts.evaluate()` primitive on the
same held-out procedural instances, at the same calibrated bar, with the
same feedback budget and temperature 0 — so the only difference between
the trained and untrained 8B rows is the weights (the trained model is
served by serve-m8-trained.sh with the base 8B's exact vLLM flags).

Reported, in the plan's pre-registered order (note/m8-execution-plan
§4):

  primary    level migration, trained vs untrained 8B — Cochran–Armitage
             trend test on the L0..L4 histogram (ordinal, one test)
  claim bar  pass-rate gain >= 10 points absolute, or exceeding the
             untrained 32B's rate
  paired     McNemar trained vs untrained 8B on the same instances
  context    the untrained 32B row and the classical optimum the bar is
             defined against (a pass at 1.0x means matching classical search)

Usage:
  NEUTRONGYM_VLLM_URL_8B=... NEUTRONGYM_VLLM_URL_32B=... \
  NEUTRONGYM_VLLM_URL_TRAINED=http://localhost:8139/v1 \
  python benchmark/harness/m8_eval.py --target-fraction 1.0 --n 100
"""

import argparse
import json
import math
import os
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))

from neutrongym import generate, rollouts  # noqa: E402
from m8_difficulty_sweep import summarize  # noqa: E402
from m8_paired import analyze, pair_rows  # noqa: E402

ARMS = {  # arm name -> (served model id, env var holding its base URL)
    "untrained-8b": ("qwen3-8b", "NEUTRONGYM_VLLM_URL_8B"),
    "trained-8b": ("qwen3-8b-m8raft", "NEUTRONGYM_VLLM_URL_TRAINED"),
    "untrained-32b": ("qwen3-32b", "NEUTRONGYM_VLLM_URL_32B"),
}
CLAIM_POINTS = 0.10


def parse_families(spec: str) -> list:
    fams = [f.strip() for f in spec.split(",") if f.strip()]
    unknown = [f for f in fams if f not in generate.FAMILIES]
    if unknown or not fams:
        raise SystemExit(f"unknown or empty families {unknown or spec!r}; "
                         f"have {sorted(generate.FAMILIES)}")
    return fams


def load_reusable(prev: dict, arms: list, *, target_fraction: float, n: int,
                  max_steps: int, families: list, start_index: int = 0) -> dict:
    """Rows for `arms` from an earlier eval record, only if it measured the
    same thing. Reusing a comparator measured under a different bar, n,
    split or feedback budget would silently change what the verdict means."""
    checks = {"target_fraction": target_fraction, "n_per_family": n,
              "max_steps": max_steps, "split": "heldout",
              "temperature": 0.0, "start_index": start_index}
    # match_tolerance and split are compared through the same got() default
    # path below; a row measured at another difficulty is not reusable
    # records written before 2026-09-17 have no start_index: they are slice 0
    got = lambda k: prev.get(k, 0) if k == "start_index" else prev.get(k)
    bad = {k: (got(k), v) for k, v in checks.items() if got(k) != v}
    if bad:
        raise SystemExit(f"refusing to reuse arms: protocol differs {bad}")
    out = {}
    for arm in arms:
        fams = (prev.get("heldout") or {}).get(arm) or {}
        missing = [f for f in families if f not in fams]
        if missing:
            raise SystemExit(f"refusing to reuse {arm}: no rows for {missing}")
        out[arm] = {f: {"rows": fams[f]["rows"]} for f in families}
    return out


def cochran_armitage(hist_a: dict, hist_b: dict) -> dict:
    """Two-sided Cochran–Armitage trend test for a 2 x K ordinal table.

    Rows are the two policies, columns the ladder levels L0..L4 with scores
    0..4. Tests whether the level distribution shifts upward from a to b —
    the pre-registered primary endpoint (level migration), which uses every
    episode rather than only the pass/fail boundary.
    """
    levels = sorted({int(k) for k in hist_a} | {int(k) for k in hist_b})
    a = [int(hist_a.get(str(k), hist_a.get(k, 0))) for k in levels]
    b = [int(hist_b.get(str(k), hist_b.get(k, 0))) for k in levels]
    col = [x + y for x, y in zip(a, b)]
    n_a, n_b = sum(a), sum(b)
    n = n_a + n_b
    if n_a == 0 or n_b == 0:
        return {"z": None, "p": None}
    t = sum(s * (b_k * n_a - a_k * n_b) for s, a_k, b_k in zip(levels, a, b))
    var = (n_a * n_b / n) * (
        sum(s * s * c * (n - c) for s, c in zip(levels, col))
        - 2 * sum(levels[i] * levels[j] * col[i] * col[j]
                  for i in range(len(levels))
                  for j in range(i + 1, len(levels))))
    if var <= 0:
        return {"z": 0.0, "p": 1.0}
    z = t / math.sqrt(var)
    p = math.erfc(abs(z) / math.sqrt(2))
    return {"z": round(z, 4), "p": round(p, 6)}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--n", type=int, default=100,
                    help="held-out instances per family per arm")
    ap.add_argument("--target-fraction", type=float, required=True)
    ap.add_argument("--max-steps", type=int, default=6)
    ap.add_argument("--split", default="heldout", choices=("heldout", "ood"),
                    help="ood = context ranges beyond training and held-out")
    ap.add_argument("--match-tolerance", type=float, default=None,
                    help="override a matching family's pass tolerance (difficulty knob)")
    ap.add_argument("--start-index", type=int, default=0,
                    help="first held-out instance; a slice disjoint from the one "
                         "used to CHOOSE a checkpoint gives an unbiased estimate")
    ap.add_argument("--arms", default=",".join(ARMS))
    ap.add_argument("--families", default="guide_divergence",
                    help="comma-separated; defaults to guide only because "
                         "SANS has an open direct-beam reward hole "
                         "(note/sans-direct-beam-exploit-2026-09-13.md) — "
                         "most SANS passes are leakage, so a SANS number "
                         "would measure the exploit, not the model")
    ap.add_argument("--trained-model", default=ARMS["trained-8b"][0],
                    help="served model id for the trained arm (an ablation "
                         "checkpoint is served under its own name)")
    ap.add_argument("--reuse", default=None, metavar="EVAL_JSON",
                    help="take --reuse-arms rows from an earlier eval instead "
                         "of re-measuring; refused unless its bar, n, "
                         "families, split and feedback budget match")
    ap.add_argument("--reuse-arms", default="untrained-8b,untrained-32b")
    ap.add_argument("--out", default=os.path.join(REPO, "runs", "m8",
                                                  "eval.json"))
    args = ap.parse_args()
    arms = args.arms.split(",")
    arm_models = dict(ARMS, **{"trained-8b": (args.trained_model,
                                              ARMS["trained-8b"][1])})
    reused = {}
    if args.reuse:
        with open(args.reuse) as f:
            reused = load_reusable(json.load(f), args.reuse_arms.split(","),
                                   target_fraction=args.target_fraction,
                                   n=args.n, max_steps=args.max_steps,
                                   families=parse_families(args.families),
                                   start_index=args.start_index)
    for arm in arms:
        if arm not in reused and not os.environ.get(ARMS[arm][1]):
            raise SystemExit(f"{ARMS[arm][1]} not set for arm {arm}")

    families = parse_families(args.families)
    record = {"target_fraction": args.target_fraction, "n_per_family": args.n,
              "families": families,
              "trained_model": args.trained_model,
              "reused_arms": {a: args.reuse for a in reused},
              "max_steps": args.max_steps, "split": args.split,
              "start_index": args.start_index,
              "match_tolerance": args.match_tolerance,
              "temperature": 0.0, "arms": {}, "heldout": {}}
    for arm in arms:
        model, var = arm_models[arm]
        rows = []
        if arm in reused:
            record["heldout"][arm] = reused[arm]
            for fam in families:
                rows += reused[arm][fam]["rows"]
            record["arms"][arm] = summarize(rows)
            s = record["arms"][arm]
            print(f"  {arm:14} {'REUSED':18} pass={s['pass_rate']} "
                  f"mean_level={s['mean_level']} (from {args.reuse})",
                  flush=True)
            continue
        for fam in families:
            r = rollouts.evaluate(model, args.n, fam, args.split,
                                  base_url=os.environ[var], temperature=0.0,
                                  max_steps=args.max_steps,
                                  target_fraction=args.target_fraction,
                                  start_index=args.start_index,
                                  match_tolerance=args.match_tolerance)
            rows += r["rows"]
            record["heldout"].setdefault(arm, {})[fam] = {"rows": r["rows"]}
            print(f"  {arm:14} {fam:18} pass={r['pass_rate']} "
                  f"levels={r['level_histogram']} ({r['wall_s']}s)",
                  flush=True)
        record["arms"][arm] = summarize(rows)
        s = record["arms"][arm]
        print(f"  {arm:14} {'TOTAL':18} pass={s['pass_rate']} "
              f"mean_level={s['mean_level']} levels={s['level_histogram']}",
              flush=True)

    if {"untrained-8b", "trained-8b"} <= set(arms):
        u, t = record["arms"]["untrained-8b"], record["arms"]["trained-8b"]
        gain = (t["pass_rate"] or 0) - (u["pass_rate"] or 0)
        ca = cochran_armitage(u["level_histogram"], t["level_histogram"])
        pr = analyze(pair_rows(record, ("untrained-8b", "trained-8b")),
                     ("untrained-8b", "trained-8b"))
        big = record["arms"].get("untrained-32b", {}).get("pass_rate")
        record["verdict"] = {
            "pass_gain": round(gain, 4),
            "level_migration_cochran_armitage": ca,
            "paired_mcnemar": {k: pr[k] for k in (
                "only_untrained-8b", "only_trained-8b", "mcnemar_p",
                "agreement")},
            "claim_bar_met": bool(gain >= CLAIM_POINTS
                                  or (big is not None
                                      and (t["pass_rate"] or 0) > big)),
            "gap_closed_to_32b": (round(gain / (big - u["pass_rate"]), 3)
                                  if big is not None and u["pass_rate"]
                                  is not None and big > u["pass_rate"]
                                  else None),
        }
        v = record["verdict"]
        print("\n=== VERDICT (trained vs untrained 8B) ===")
        print(f"  pass gain {gain:+.3f}  (claim needs >= {CLAIM_POINTS:+.2f} "
              f"or beating 32B at {big})")
        print(f"  level migration: Cochran-Armitage z={ca['z']} p={ca['p']}")
        print(f"  paired McNemar p={pr['mcnemar_p']} "
              f"(untrained-only {pr['only_untrained-8b']}, trained-only "
              f"{pr['only_trained-8b']})")
        print(f"  fraction of 8B->32B gap closed: {v['gap_closed_to_32b']}")
        print(f"  CLAIM BAR {'MET' if v['claim_bar_met'] else 'NOT MET'}")

    os.makedirs(os.path.dirname(args.out), exist_ok=True)
    with open(args.out, "w") as f:
        json.dump(record, f, indent=1)
    print(f"-> {os.path.relpath(args.out, REPO)}")


if __name__ == "__main__":
    main()
