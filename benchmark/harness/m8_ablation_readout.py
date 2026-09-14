"""Readout for the post-hoc passing-turn ablation, as pre-specified.

The criteria were fixed in PLAN.md and the M8 plan note on 2026-09-14,
before the ablation was trained. This computes them from the two evaluation
records instead of reading numbers off a console, so the verdict is
reproducible and cannot drift toward whichever comparison looks better:

  vs untrained  paired McNemar, passing-turn model vs untrained 8B (the
                untrained rows are the SAME reused rows in both records)
  vs per-turn   paired McNemar, passing-turn model vs the pre-registered
                per-turn model, on the same 300 held-out instances

  supported  the passing-turn model does not regress vs untrained (no
             p < 0.05 in the harmful direction) AND beats the per-turn
             model (p < 0.05 in its favour)
  refuted    it regresses vs untrained at p < 0.05 by at least as much as
             the per-turn model did
  otherwise  inconclusive

Usage:
  python benchmark/harness/m8_ablation_readout.py \
      runs/m8/eval_guide_1x_n300.json runs/m8/eval_guide_1x_n300_passing.json
"""

import argparse
import json
import os
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))

from m8_paired import analyze, pair_rows  # noqa: E402

ALPHA = 0.05


def _rows(record, arm, family):
    return {"rows": record["heldout"][arm][family]["rows"]}


def paired(a_rows, b_rows, a, b, family):
    rec = {"heldout": {a: {family: a_rows}, b: {family: b_rows}}}
    res = analyze(pair_rows(rec, (a, b)), (a, b))
    return {"only_a": res[f"only_{a}"], "only_b": res[f"only_{b}"],
            "mcnemar_p": res["mcnemar_p"], "n_paired": res["n_paired"]}


def readout(per_turn: dict, passing: dict, family: str = "guide_divergence") -> dict:
    same = per_turn["heldout"]["untrained-8b"][family]["rows"] == \
        passing["heldout"]["untrained-8b"][family]["rows"]
    if not same:
        raise SystemExit("untrained-8b rows differ between records — the "
                         "ablation must reuse the pre-registered comparator")
    u = _rows(passing, "untrained-8b", family)
    t_turn = _rows(per_turn, "trained-8b", family)
    t_pass = _rows(passing, "trained-8b", family)
    vs_u_turn = paired(u, t_turn, "untrained", "per_turn", family)
    vs_u_pass = paired(u, t_pass, "untrained", "passing", family)
    vs_turn = paired(t_turn, t_pass, "per_turn", "passing", family)

    def rate(r):
        rows = r["rows"]
        return round(sum(x["best_level"] == 4 for x in rows) / len(rows), 4)

    pass_regresses = (vs_u_pass["mcnemar_p"] < ALPHA
                      and vs_u_pass["only_a"] > vs_u_pass["only_b"])
    beats_turn = (vs_turn["mcnemar_p"] < ALPHA
                  and vs_turn["only_b"] > vs_turn["only_a"])
    turn_drop = vs_u_turn["only_a"] - vs_u_turn["only_b"]
    pass_drop = vs_u_pass["only_a"] - vs_u_pass["only_b"]
    if not pass_regresses and beats_turn:
        verdict = "supported"
    elif pass_regresses and pass_drop >= turn_drop:
        verdict = "refuted"
    else:
        verdict = "inconclusive"
    return {"family": family, "reused_untrained_rows_identical": same,
            "pass_rate": {"untrained-8b": rate(u), "per-turn": rate(t_turn),
                          "passing-turn": rate(t_pass)},
            "passing_vs_untrained": vs_u_pass,
            "per_turn_vs_untrained": vs_u_turn,
            "passing_vs_per_turn": vs_turn,
            "passing_regresses_vs_untrained": pass_regresses,
            "passing_beats_per_turn": beats_turn,
            "verdict": verdict, "label": "post-hoc"}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("per_turn_eval")
    ap.add_argument("passing_eval")
    ap.add_argument("--out", default=os.path.join(REPO, "runs", "m8", "ablation_readout.json"))
    args = ap.parse_args()
    res = readout(json.load(open(args.per_turn_eval)), json.load(open(args.passing_eval)))
    print(json.dumps(res, indent=1))
    with open(args.out, "w") as f:
        json.dump(res, f, indent=1)
    print(f"-> {os.path.relpath(args.out, REPO)}")


if __name__ == "__main__":
    main()
