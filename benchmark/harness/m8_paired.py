"""Paired per-instance analysis of two policies on the same instances.

Marginal pass rates hide the thing that matters. M8 phase 0 (2026-09-13)
found untrained Qwen3-8B and Qwen3-32B both at 0.70 on held-out procedural
instances, which reads as "the two models are equivalent". The paired view
says something sharper: they agree on only half the instances, and the
disagreements split evenly in both directions.

That distinction is the whole argument. An even split of discordant pairs
means there is no capability ORDERING to approach — not merely that the
gap is small — so "7B + training approaches a larger untrained model" is
unevaluable on this axis no matter how much data we collect. A skewed
split would have meant the opposite: a real but small gap, worth powering
up. McNemar's exact test is the right test because it conditions on
exactly the discordant pairs and ignores the instances both models get
right or wrong, which carry no information about ordering.

Caveat carried into the output, not left to the reader: measurement runs
at temperature 0, one episode per instance, so a discordant pair is a
deterministic property of that model-instance pair. It cannot be
decomposed into "capability" versus "luck" without re-sampling at
temperature > 0. The printed report always ends with that caveat.

Usage:
  python benchmark/harness/m8_paired.py runs/m8/phase0_calibrated.json
  python benchmark/harness/m8_paired.py runs/m8/phase0_n15.json \
      --json runs/m8/paired_uncalibrated.json
"""

import argparse
import json
import os
import sys
from math import comb

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


def mcnemar_exact_p(b: int, c: int) -> float:
    """Two-sided exact McNemar on discordant counts b and c.

    Conditional on b + c discordant pairs, b ~ Binomial(b + c, 0.5) under
    the null of no ordering. Returns 1.0 when there are no discordant pairs
    (nothing to test), which is the honest answer rather than 0.
    """
    n = b + c
    if n == 0:
        return 1.0
    k = min(b, c)
    p = 2.0 * sum(comb(n, i) for i in range(k + 1)) / (2 ** n)
    return min(p, 1.0)


def pair_rows(record: dict, models: tuple, key: str = "heldout") -> dict:
    """-> {(family, instance): {model: row}} for a phase-0 style record."""
    paired: dict = {}
    for model in models:
        for fam, r in (record.get(key) or {}).get(model, {}).items():
            if fam == "_all" or not isinstance(r, dict):
                continue
            for row in r.get("rows") or []:
                paired.setdefault((fam, row["instance"]), {})[model] = row
    return {k: v for k, v in paired.items() if len(v) == len(models)}


def analyze(paired: dict, models: tuple) -> dict:
    a, b = models
    both = only_a = only_b = neither = 0
    discordant = []
    per_family: dict = {}
    for (fam, idx), v in sorted(paired.items()):
        pa = v[a]["best_level"] == 4
        pb = v[b]["best_level"] == 4
        f = per_family.setdefault(fam, {"only_a": 0, "only_b": 0,
                                        "both": 0, "neither": 0})
        if pa and pb:
            both += 1; f["both"] += 1
        elif pa:
            only_a += 1; f["only_a"] += 1
            discordant.append({"family": fam, "instance": idx, "winner": a})
        elif pb:
            only_b += 1; f["only_b"] += 1
            discordant.append({"family": fam, "instance": idx, "winner": b})
        else:
            neither += 1; f["neither"] += 1
    n = both + only_a + only_b + neither
    p = mcnemar_exact_p(only_a, only_b)
    return {
        "models": list(models), "n_paired": n,
        "both_pass": both, f"only_{a}": only_a, f"only_{b}": only_b,
        "neither": neither,
        "agreement": round((both + neither) / n, 4) if n else None,
        "discordant": only_a + only_b,
        "mcnemar_p": round(p, 6),
        "ordering_supported": bool(p < 0.05),
        # a NON-significant result splits into two very different states,
        # and calling both "no ordering" would misreport one of them: 4-vs-4
        # is evidence OF no ordering, while 1-vs-5 is a real skew that this
        # n simply cannot resolve. Only the former licenses "the bar is
        # unevaluable"; the latter licenses "collect more data".
        "balanced": bool(only_a + only_b > 0
                         and min(only_a, only_b) * 2 >= only_a + only_b - 1),
        "leader": (None if only_a == only_b
                   else a if only_a > only_b else b),
        "n_discordant_for_significance": _n_needed_for_significance(),
        "per_family": per_family,
        "discordant_instances": discordant,
    }


def _n_needed_for_significance() -> int:
    """Smallest number of ALL-ONE-WAY discordant pairs whose two-sided
    exact p clears 0.05 — the honest answer to "how much more data?"."""
    n = 1
    while mcnemar_exact_p(0, n) >= 0.05:
        n += 1
    return n


def min_disc(res: dict) -> int:
    a, b = res["models"]
    return min(res[f"only_{a}"], res[f"only_{b}"])


def report(res: dict) -> str:
    a, b = res["models"]
    out = [
        f"paired instances: {res['n_paired']}",
        f"  both pass          : {res['both_pass']}",
        f"  {(a + ' only'):19}: {res[f'only_{a}']}",
        f"  {(b + ' only'):19}: {res[f'only_{b}']}",
        f"  neither            : {res['neither']}",
        f"  agreement          : {res['agreement']}",
        "",
        f"McNemar exact (discordant {res[f'only_{a}']} vs "
        f"{res[f'only_{b}']}): p = {res['mcnemar_p']}",
    ]
    if res["ordering_supported"]:
        out.append(f"  -> ORDERING SUPPORTED: the discordant pairs skew "
                   f"toward {res['leader']}, so one model is genuinely "
                   f"ahead. A trainability target exists.")
    elif res["balanced"]:
        out.append("  -> NO ORDERING: the disagreements split evenly, so "
                   "equal pass rates are not two models behaving alike — "
                   "they are two models succeeding on DIFFERENT instances "
                   "with no consistent advantage. There is no gap to "
                   "approach, so the claim bar is unevaluable on this axis.")
    else:
        out.append(f"  -> UNDERPOWERED, NOT NULL: the discordant pairs lean "
                   f"toward {res['leader']} "
                   f"({max(res['discordant'] - min_disc(res), min_disc(res))}"
                   f" vs {min_disc(res)}) but this n cannot resolve it. Do "
                   f"NOT read this as 'no difference' — it needs at least "
                   f"{res['n_discordant_for_significance']} one-way "
                   f"discordant pairs to reach p < 0.05.")
    if res["per_family"]:
        out += ["", "per family (only_a / only_b / both / neither):"]
        for fam, f in sorted(res["per_family"].items()):
            out.append(f"  {fam:20} {f['only_a']:3} / {f['only_b']:3} / "
                       f"{f['both']:3} / {f['neither']:3}")
    out += ["", "CAVEAT: temperature 0, one episode per instance — a "
            "discordant pair is deterministic for that model-instance pair "
            "and cannot be split into capability vs luck without "
            "re-sampling at temperature > 0."]
    return "\n".join(out)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("record", help="phase-0 or sweep JSON")
    ap.add_argument("--models", default="qwen3-8b,qwen3-32b")
    ap.add_argument("--json", default=None, help="write the analysis here")
    args = ap.parse_args()
    models = tuple(args.models.split(","))
    if len(models) != 2:
        raise SystemExit("paired analysis needs exactly two models")
    with open(args.record) as f:
        rec = json.load(f)

    paired = pair_rows(rec, models)
    if not paired:
        raise SystemExit(f"no paired instances for {models} in "
                         f"{args.record} — is this a phase-0 style record?")
    res = analyze(paired, models)
    print(report(res))
    if args.json:
        os.makedirs(os.path.dirname(args.json) or ".", exist_ok=True)
        with open(args.json, "w") as f:
            json.dump(res, f, indent=1)
        print(f"\n-> {os.path.relpath(args.json, REPO)}")


if __name__ == "__main__":
    main()
