"""No-model constant-action baselines on the M8 evaluation instances.

The post-hoc passing-turn ablation reached 52.3% on 300 held-out guide
instances at the 1.0x bar, but its temperature-0 replays showed it
resubmitting ONE action, (w_in 0.05, w_out 0.03, m_coat 2.5) - the modal
passing action in the training data - on half of all episodes. Before any
trainability reading, the question is how much of that pass rate a
constant action earns with no model at all (the same test that exposed the
SANS direct-beam hole).

For each candidate action this submits it once per held-out instance at the
same bar (a repeated identical action cannot change the outcome), records
per-instance levels, and pairs the constant policy against every arm of an
evaluation record with McNemar.

Candidates (2026-09-15, generalized beyond guide): the five most common
passing actions in a training file (--data), the constant-policy gate's grid
(--grid, hacks.constant_candidates), or the original guide list when neither
is given.

Usage:
  python benchmark/harness/m8_constant_probe.py \
      --eval runs/m8/eval_guide_1x_n300_passing.json --n 300
  python benchmark/harness/m8_constant_probe.py --family sans_collimation \
      --eval runs/m8/eval_sans_1x_n300.json --n 300 --grid \
      --data runs/m8/raft_sans/train.jsonl
"""

import argparse
import json
import os
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))

from neutrongym import generate, hacks, rollouts  # noqa: E402
from m8_paired import analyze, pair_rows  # noqa: E402

FAM = "guide_divergence"
DEFAULT_ACTIONS = [  # the five most common FINAL (passing) actions in train.jsonl
    {"w_in": 0.05, "w_out": 0.03, "m_coat": 2.5},
    {"w_in": 0.09, "w_out": 0.03, "m_coat": 3.0},
    {"w_in": 0.04, "w_out": 0.03, "m_coat": 2.5},
    {"w_in": 0.05, "w_out": 0.07, "m_coat": 2.8},
    {"w_in": 0.05, "w_out": 0.07, "m_coat": 2.5},
]


def modal_passing_actions(path: str, free: dict, k: int = 5) -> list:
    """The k most common FINAL actions of kept episodes — each episode's
    passing move — parsed and restricted to the family's free parameters."""
    import collections
    import re
    counts = collections.Counter()
    for line in open(path):
        if not line.strip():
            continue
        rec = json.loads(line)
        last = [m for m in rec["messages"] if m["role"] == "assistant"]
        if not last:
            continue
        mm = re.search(r"\{.*\}", last[-1]["content"] or "", re.S)
        try:
            act = json.loads(mm.group(0)) if mm else None
        except ValueError:
            act = None
        if not act or set(act) != set(free):
            continue
        counts[tuple(sorted((key, round(float(v), 6)) for key, v in act.items()))] += 1
    return [dict(key) for key, _ in counts.most_common(k)]


def candidate_actions(family: str, data: str | None, grid: bool) -> list:
    inst = generate.instance(family, "heldout", 0)
    out = []
    if data:
        out += modal_passing_actions(data, inst["free_parameters"])
    if grid:
        out += hacks.constant_candidates(inst)
    if not out:
        if family != FAM:
            raise SystemExit("give --data and/or --grid for non-guide families")
        out = list(DEFAULT_ACTIONS)
    seen, uniq = set(), []
    for a in out:
        key = tuple(sorted(a.items()))
        if key not in seen:
            seen.add(key)
            uniq.append(a)
    return uniq


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--eval", required=True)
    ap.add_argument("--family", default=FAM, choices=list(generate.FAMILIES))
    ap.add_argument("--data", default=None,
                    help="training jsonl; adds its five most common passing actions")
    ap.add_argument("--grid", action="store_true",
                    help="add the constant-policy gate's candidate grid")
    ap.add_argument("--n", type=int, default=300)
    ap.add_argument("--target-fraction", type=float, default=1.0)
    ap.add_argument("--out", default=None)
    args = ap.parse_args()
    fam = args.family
    out_path = args.out or os.path.join(
        REPO, "runs", "m8",
        f"constant_probe_{fam.split('_')[0]}_{args.target_fraction:g}x_n{args.n}.json")
    ev = json.load(open(args.eval))
    if ev.get("target_fraction") != args.target_fraction or ev.get("n_per_family") != args.n:
        raise SystemExit("probe must use the evaluation's bar and n")
    arms = list(ev["heldout"])
    actions = candidate_actions(fam, args.data, args.grid)
    record = {"family": fam, "split": "heldout", "n": args.n,
              "target_fraction": args.target_fraction, "eval": os.path.relpath(args.eval, REPO),
              "actions": []}
    best = None
    for act in actions:
        text = json.dumps(act)
        r = rollouts.evaluate("constant", args.n, fam, "heldout", max_steps=1,
                              target_fraction=args.target_fraction,
                              chat_fn=lambda msgs, t=text: t)
        rows = [{"instance": x["instance"], "best_level": x["best_level"]} for x in r["rows"]]
        entry = {"action": act, "pass_rate": r["pass_rate"],
                 "level_histogram": r["level_histogram"], "errors": r["errors"], "vs": {}}
        for arm in arms:
            rec = {"heldout": {"constant": {fam: {"rows": rows}}, arm: {fam: ev["heldout"][arm][fam]}}}
            res = analyze(pair_rows(rec, ("constant", arm)), ("constant", arm))
            entry["vs"][arm] = {"only_constant": res["only_constant"], f"only_arm": res[f"only_{arm}"],
                                "mcnemar_p": res["mcnemar_p"], "agreement": res["agreement"]}
        entry["rows"] = rows
        record["actions"].append(entry)
        if best is None or (r["pass_rate"] or 0) > (best["pass_rate"] or 0):
            best = entry
        vs = "  ".join(f"{a}: agree {v['agreement']} p={v['mcnemar_p']} ({v['only_constant']}v{v['only_arm']})"
                       for a, v in entry["vs"].items())
        print(f"constant {text:44} pass={r['pass_rate']} levels={r['level_histogram']} errors={r['errors']}\n    {vs}", flush=True)
    # oracle over the constant candidates: pass if ANY of the five constants passes
    per = {}
    for e in record["actions"]:
        for x in e["rows"]:
            per.setdefault(x["instance"], False)
            per[x["instance"]] |= x["best_level"] == 4
    record["best_single_constant"] = {"action": best["action"], "pass_rate": best["pass_rate"]}
    record["n_candidates"] = len(actions)
    record["any_constant_pass_rate"] = round(sum(per.values()) / len(per), 4)
    record["any_of_five_constants_pass_rate"] = record["any_constant_pass_rate"]
    print("best single constant:", record["best_single_constant"])
    print(f"any of the {len(actions)} constants (oracle upper bound for a lookup):",
          record["any_constant_pass_rate"])
    with open(out_path, "w") as f:
        json.dump(record, f, indent=1)
    print(f"-> {os.path.relpath(out_path, REPO)}")


if __name__ == "__main__":
    main()
