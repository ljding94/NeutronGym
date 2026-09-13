"""M8 difficulty-response sweep — the gate's prescribed readout redefinition.

Phase 0 (2026-09-13) failed its vacuity gate: at the T2 difficulty
(target = 0.8x the constraint-filtered classical optimum) the untrained
Qwen3-8B and Qwen3-32B both passed 70% of held-out procedural instances.
A 4x parameter increase bought nothing, so "approaching a larger untrained
model" had no target to approach.

Pass rate at ONE difficulty cannot distinguish two hypotheses:

  H_saturated  the bar is too low; both models are on a ceiling and a
               harder bar would separate them.
  H_flat       the task genuinely does not discriminate model scale at any
               bar, because dense per-step FOM feedback reduces it to
               hill-climbing that both models do equally well.

This script measures the whole curve instead of one point. The expensive
half of calibration — the constraint-filtered classical search — depends
only on the instance and is cached, so every extra difficulty level is a
pure rescale of a cached optimum and costs no new simulations beyond the
rollouts themselves. Both models see the SAME instances at the SAME bars,
and the prompt always discloses the bar in force.

Readouts recorded at every level, because the gate said to consider level
migration and FOM distribution, not just pass/fail:

  pass_rate          fraction reaching L4
  level_histogram    L0..L4 (migration, the pre-registered ordinal endpoint)
  steps_to_success   median steps of PASSING episodes (efficiency)
  best_fom_ratio     median and max over episodes (solution quality)

Interpretation is written into the output, not left to the reader:
separation at any level => H_saturated, redefine the bar and proceed;
flat everywhere => H_flat, a measured negative result about the task.

Usage:
  python benchmark/harness/m8_difficulty_sweep.py \
      [--n 25] [--fractions 0.8,1.0,1.2,1.4] [--out runs/m8/sweep.json]
"""

import argparse
import json
import os
import statistics
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))

from neutrongym import generate, rollouts  # noqa: E402

MODELS = {"qwen3-8b": "NEUTRONGYM_VLLM_URL_8B",
          "qwen3-32b": "NEUTRONGYM_VLLM_URL_32B"}

# a real difference must clear this to count as separation, matching the
# plan's pre-registered ">= 10 points absolute" discipline
SEPARATION_THRESHOLD = 0.10


def _median(xs):
    return round(statistics.median(xs), 3) if xs else None


def summarize(rows: list) -> dict:
    """Level migration + efficiency + quality from raw episode rows."""
    valid = [r for r in rows if r.get("best_level") is not None]
    passing = [r for r in valid if r["best_level"] == 4]
    foms = [r.get("best_fom_ratio") or 0 for r in valid]
    return {
        "n_valid": len(valid),
        "passes": len(passing),
        "pass_rate": round(len(passing) / len(valid), 4) if valid else None,
        "level_histogram": {str(lv): sum(1 for r in valid
                                         if r["best_level"] == lv)
                            for lv in range(5)},
        "mean_level": (round(sum(r["best_level"] for r in valid) / len(valid),
                             3) if valid else None),
        "steps_to_success_median": _median([r["steps"] for r in passing]),
        "steps_all_median": _median([r["steps"] for r in valid]),
        "fom_ratio_median": _median(foms),
        "fom_ratio_max": round(max(foms), 3) if foms else None,
    }


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--n", type=int, default=25,
                    help="instances per family per model per difficulty")
    ap.add_argument("--fractions", default="0.8,1.0,1.2,1.4",
                    help="target fractions of the classical optimum")
    ap.add_argument("--out", default=os.path.join(REPO, "runs", "m8",
                                                  "sweep.json"))
    args = ap.parse_args()
    for m, var in MODELS.items():
        if not os.environ.get(var):
            raise SystemExit(f"{var} not set — the sweep compares both "
                             "local endpoints; one alone proves nothing")

    fractions = [float(x) for x in args.fractions.split(",")]
    families = list(generate.FAMILIES)
    record = {"n_per_family": args.n, "fractions": fractions,
              "families": families, "separation_threshold":
              SEPARATION_THRESHOLD, "cells": {}, "curve": [],
              # raw per-episode rows, keyed so m8_paired.py can read this
              # file directly. Marginal pass rates were exactly what hid the
              # phase-0 finding (identical 0.70 rates reached on DIFFERENT
              # instances), so a sweep that stored only summaries would
              # reproduce the same blind spot at four difficulties instead
              # of one.
              "rows_by_fraction": {}}

    for frac in fractions:
        print(f"\n=== target = {frac:.2f}x classical optimum ===")
        per_model = {}
        for model in MODELS:
            rows = []
            for fam in families:
                r = rollouts.evaluate(
                    model, args.n, fam, "heldout",
                    base_url=os.environ[MODELS[model]], temperature=0.0,
                    target_fraction=frac)
                rows += r["rows"]
                record["rows_by_fraction"].setdefault(
                    str(frac), {}).setdefault(model, {})[fam] = {
                        "rows": r["rows"]}
                s = summarize(r["rows"])
                print(f"  {model:10} {fam:18} pass={s['pass_rate']} "
                      f"levels={s['level_histogram']} "
                      f"steps={s['steps_to_success_median']} "
                      f"({r['wall_s']}s)")
                record["cells"][f"{frac}|{model}|{fam}"] = s
            per_model[model] = summarize(rows)
            s = per_model[model]
            print(f"  {model:10} {'TOTAL':18} pass={s['pass_rate']} "
                  f"levels={s['level_histogram']} "
                  f"mean_level={s['mean_level']} "
                  f"steps={s['steps_to_success_median']} "
                  f"fom_med={s['fom_ratio_median']}")

        p8 = per_model["qwen3-8b"]["pass_rate"] or 0
        p32 = per_model["qwen3-32b"]["pass_rate"] or 0
        record["curve"].append({
            "fraction": frac, "qwen3-8b": per_model["qwen3-8b"],
            "qwen3-32b": per_model["qwen3-32b"],
            "pass_gap": round(p32 - p8, 4),
            "level_gap": round((per_model["qwen3-32b"]["mean_level"] or 0)
                               - (per_model["qwen3-8b"]["mean_level"] or 0), 3),
            "separates": abs(p32 - p8) >= SEPARATION_THRESHOLD})
        print(f"  -> gap(32B - 8B) = {p32 - p8:+.3f}  "
              f"{'SEPARATES' if abs(p32 - p8) >= SEPARATION_THRESHOLD else 'flat'}")

    # --- paired view at every difficulty --------------------------------
    # Marginal equality and per-instance equivalence are different claims;
    # only the paired test can tell them apart.
    sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))
    from m8_paired import analyze, pair_rows
    models = tuple(MODELS)
    print("\n=== paired (per-instance) view ===")
    for frac in fractions:
        sub = {"heldout": record["rows_by_fraction"][str(frac)]}
        paired = pair_rows(sub, models)
        if not paired:
            continue
        res = analyze(paired, models)
        record.setdefault("paired", {})[str(frac)] = res
        verdict = ("ORDERING" if res["ordering_supported"]
                   else "balanced" if res["balanced"] else "underpowered")
        print(f"  {frac:.2f}x  agree={res['agreement']}  discordant="
              f"{res[f'only_{models[0]}']}v{res[f'only_{models[1]}']}  "
              f"McNemar p={res['mcnemar_p']}  -> {verdict}")

    # --- verdict --------------------------------------------------------
    sep = [c for c in record["curve"] if c["separates"]]
    best = max(record["curve"], key=lambda c: abs(c["pass_gap"]))
    paired_orderings = [f for f, r in (record.get("paired") or {}).items()
                        if r["ordering_supported"]]
    record["verdict"] = {
        "any_separation": bool(sep),
        "paired_ordering_at": paired_orderings,
        "separating_fractions": [c["fraction"] for c in sep],
        "max_abs_gap": abs(best["pass_gap"]),
        "max_gap_fraction": best["fraction"],
        "hypothesis": "H_saturated" if sep else "H_flat",
    }
    print("\n=== VERDICT ===")
    for c in record["curve"]:
        print(f"  {c['fraction']:.2f}x  8B={c['qwen3-8b']['pass_rate']}  "
              f"32B={c['qwen3-32b']['pass_rate']}  "
              f"gap={c['pass_gap']:+.3f}  "
              f"dlevel={c['level_gap']:+.3f}")
    if paired_orderings:
        print(f"  PAIRED ordering found at fractions {paired_orderings} — "
              f"check these even if the marginal gap is small.")
    if sep:
        print(f"  H_saturated: the models DO separate at "
              f"{record['verdict']['separating_fractions']} — the 0.8x bar "
              f"was a ceiling, not a dead task.")
        print(f"  -> ACTION: re-run phase 0 at "
              f"{best['fraction']:.2f}x and proceed with RAFT there.")
    else:
        print(f"  H_flat: no difficulty separates the models "
              f"(max |gap| = {record['verdict']['max_abs_gap']:.3f} at "
              f"{best['fraction']:.2f}x).")
        print("  -> ACTION: the task does not discriminate model scale at "
              "any bar. Report as a measured negative result; do not train "
              "on this axis.")

    os.makedirs(os.path.dirname(args.out), exist_ok=True)
    with open(args.out, "w") as f:
        json.dump(record, f, indent=1)
    print(f"\n-> {os.path.relpath(args.out, REPO)}")


if __name__ == "__main__":
    main()
