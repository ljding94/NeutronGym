"""M8 phase 0 — the HARD GATE before any data generation or training.

Three questions, all answered on held-out procedural instances, all free
(local vLLM). Nothing downstream starts until these pass:

  1. BASELINES exist on the right axis. M6's 8B 1/17 and 32B 3/17 are
     BENCHMARK numbers; a procedural-axis claim needs procedural baselines.
  2. VACUITY: is the untrained 32B meaningfully above the untrained 8B? If
     not, "approaching a larger untrained model" has no target and the
     standing claim bar cannot be evaluated — stop and redefine the readout
     BEFORE generating data.
  3. FEASIBILITY: can the untrained 8B produce rollouts worth training on
     (self-generated RAFT)? Its keep rate on the TRAIN split is the test.
     If it is ~0, RAFT has nothing to learn from and we decide seed-vs-abort
     with evidence.

Usage:
  python benchmark/harness/m8_phase0.py [--n 25] [--out runs/m8/phase0.json]
"""

import argparse
import json
import os
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))

from neutrongym import generate, rollouts  # noqa: E402

MODELS = {"qwen3-8b": "NEUTRONGYM_VLLM_URL_8B",
          "qwen3-32b": "NEUTRONGYM_VLLM_URL_32B"}


def url_for(model):
    return os.environ.get(MODELS[model], "")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--n", type=int, default=25,
                    help="instances per family per model")
    ap.add_argument("--out", default=os.path.join(REPO, "runs", "m8",
                                                  "phase0.json"))
    args = ap.parse_args()
    for m, var in MODELS.items():
        if not url_for(m):
            raise SystemExit(f"{var} not set — phase 0 needs both local "
                             "endpoints (they are the baselines)")

    families = list(generate.FAMILIES)
    record = {"n_per_family": args.n, "families": families,
              "heldout": {}, "train_keeprate": {}}

    print("=== held-out procedural baselines (the claim's denominators) ===")
    for model in MODELS:
        agg = {"level_histogram": {str(i): 0 for i in range(5)},
               "n_valid": 0, "passes": 0}
        for fam in families:
            r = rollouts.evaluate(model, args.n, fam, "heldout",
                                  base_url=url_for(model), temperature=0.0)
            for lv, c in r["level_histogram"].items():
                agg["level_histogram"][str(lv)] += c
            agg["n_valid"] += r["n_valid"]
            agg["passes"] += r["level_histogram"][4]
            print(f"  {model:10} {fam:18} n={r['n']:3} valid={r['n_valid']:3} "
                  f"levels={r['level_histogram']} pass={r['pass_rate']} "
                  f"({r['wall_s']}s)")
            record["heldout"].setdefault(model, {})[fam] = r
        agg["pass_rate"] = (round(agg["passes"] / agg["n_valid"], 4)
                            if agg["n_valid"] else None)
        record["heldout"][model]["_all"] = agg
        print(f"  {model:10} {'TOTAL':18} valid={agg['n_valid']} "
              f"levels={agg['level_histogram']} pass={agg['pass_rate']}")

    print("\n=== RAFT feasibility: 8B keep rate on the TRAIN split ===")
    for fam in families:
        r = rollouts.evaluate("qwen3-8b", args.n, fam, "train",
                              base_url=url_for("qwen3-8b"), temperature=0.7)
        keep = sum(1 for row in r["rows"]
                   if (row.get("best_reward") or 0) >= 1.0)
        rate = round(keep / r["n_valid"], 3) if r["n_valid"] else 0.0
        record["train_keeprate"][fam] = {"keep": keep, "n": r["n_valid"],
                                         "rate": rate, "detail": r}
        print(f"  qwen3-8b {fam:18} keep={keep}/{r['n_valid']} "
              f"rate={rate} levels={r['level_histogram']}")

    # --- gate decisions -------------------------------------------------
    p8 = record["heldout"]["qwen3-8b"]["_all"]["pass_rate"] or 0
    p32 = record["heldout"]["qwen3-32b"]["_all"]["pass_rate"] or 0
    l8 = record["heldout"]["qwen3-8b"]["_all"]["level_histogram"]
    l32 = record["heldout"]["qwen3-32b"]["_all"]["level_histogram"]
    keeps = sum(v["keep"] for v in record["train_keeprate"].values())
    ns = sum(v["n"] for v in record["train_keeprate"].values())
    keeprate = round(keeps / ns, 3) if ns else 0.0

    vacuous = (p32 - p8) < 0.05
    raft_ok = keeps >= 20
    record["gates"] = {
        "baseline_8b_pass": p8, "baseline_32b_pass": p32,
        "vacuity_risk": vacuous, "raft_keep": keeps, "raft_rate": keeprate,
        "raft_feasible": raft_ok}
    print("\n=== GATE ===")
    print(f"  8B held-out pass {p8}  levels {l8}")
    print(f"  32B held-out pass {p32}  levels {l32}")
    print(f"  [{'FAIL' if vacuous else 'PASS'}] vacuity: 32B - 8B = "
          f"{p32 - p8:+.3f} (need > 0.05 for 'approaching a larger "
          f"untrained model' to have a target)")
    print(f"  [{'PASS' if raft_ok else 'FAIL'}] RAFT feasibility: "
          f"{keeps} keepers from {ns} train rollouts (rate {keeprate}; "
          f"need >= 20 to seed self-generated SFT)")
    if vacuous:
        print("  -> ACTION: redefine the readout (level migration / FOM "
              "distribution) before generating data; do NOT claim "
              "'approaching a larger untrained model' on this axis.")
    if not raft_ok:
        print("  -> ACTION: self-generated data too thin; decide "
              "seed-from-frontier + filtered-vs-unfiltered ablation, or "
              "abort to env+eval-only.")
    os.makedirs(os.path.dirname(args.out), exist_ok=True)
    with open(args.out, "w") as f:
        json.dump(record, f, indent=1)
    print(f"\n-> {os.path.relpath(args.out, REPO)}")


if __name__ == "__main__":
    main()
