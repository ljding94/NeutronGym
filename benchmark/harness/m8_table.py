"""Build the M8 result table with confidence intervals from the eval files.

Generated, never transcribed: every row is read from a runs/m8/*.json that an
evaluation wrote, so the paper's table cannot drift from the evidence.
Intervals are Clopper-Pearson (exact, one row at a time); paired comparisons
use exact McNemar on discordant pairs.

Covers every gated family (2026-09-21). `sans_match`'s headline lives on the
unbiased slice 600-899 and the others on 300-599, so the slice is recorded
per family rather than assumed.

Usage: python benchmark/harness/m8_table.py [--family all] [--root DIR]
                                            [--out runs/m8/m8_table.json]

`--root` points at the directory holding the eval records: `runs/m8` on the
DGX, or `benchmark/evidence/m8_rl/eval` to regenerate the table from the
frozen copies, which is how a reader verifies the paper's numbers without
access to the machine that produced them.
"""

import argparse
import json
import math
import os
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))

from m8_paired import mcnemar_exact_p as mcnemar_p  # noqa: E402


def _tail_ge(p: float, k: int, n: int) -> float:
    return sum(math.comb(n, i) * p ** i * (1 - p) ** (n - i) for i in range(k, n + 1))


def _tail_le(p: float, k: int, n: int) -> float:
    return sum(math.comb(n, i) * p ** i * (1 - p) ** (n - i) for i in range(k + 1))


def _bisect(f, target, increasing: bool) -> float:
    lo, hi = 0.0, 1.0
    for _ in range(200):
        mid = (lo + hi) / 2
        if (f(mid) < target) == increasing:
            lo = mid
        else:
            hi = mid
    return (lo + hi) / 2


def clopper_pearson(k: int, n: int, alpha: float = 0.05) -> tuple:
    """Exact two-sided (1-alpha) interval for a binomial rate, stdlib only.

    Lower limit: the p at which P(X >= k) = alpha/2 (increasing in p).
    Upper limit: the p at which P(X <= k) = alpha/2 (decreasing in p).
    """
    if n == 0:
        return (0.0, 1.0)
    low = 0.0 if k == 0 else _bisect(lambda p: _tail_ge(p, k, n), alpha / 2, True)
    high = 1.0 if k == n else _bisect(lambda p: _tail_le(p, k, n), alpha / 2, False)
    return (low, high)


def rows_of(path: str, arm: str, family: str) -> list:
    with open(path) as f:
        rec = json.load(f)
    return rec["heldout"][arm][family]["rows"]


def rate(rows: list) -> dict:
    n = len(rows)
    k = sum(r.get("best_level") == 4 for r in rows)
    lo, hi = clopper_pearson(k, n)
    return {"passes": k, "n": n, "rate": round(k / n, 4) if n else None,
            "ci95": [round(lo, 4), round(hi, 4)],
            "errored": sum(r.get("error") is not None for r in rows)}


def paired(a_rows: list, b_rows: list) -> dict:
    a = {r["instance"]: r.get("best_level") == 4 for r in a_rows}
    b = {r["instance"]: r.get("best_level") == 4 for r in b_rows}
    shared = sorted(set(a) & set(b))
    only_a = sum(a[i] and not b[i] for i in shared)
    only_b = sum(b[i] and not a[i] for i in shared)
    return {"n": len(shared), "only_a": only_a, "only_b": only_b,
            "mcnemar_p": mcnemar_p(only_a, only_b)}


# Per family: the slice its headline is reported on, and each arm's record.
# sans_match differs -- its 300-599 slice chose the checkpoint, so only
# 600-899 is unbiased for the model that selection picked.
SPECS = {
    "guide_match": {"slice": "heldout 300-599", "arms": [
        ("untrained 8B", "eval_match_fresh_untrained-8b.json", "untrained-8b"),
        ("untrained 32B", "eval_match_fresh_untrained-32b.json", "untrained-32b"),
        ("GRPO 8B (seed 1)", "eval_match_fresh_trained-8b.json", "trained-8b"),
        ("GRPO 8B (seed 2)", "eval_match_fresh_rep2.json", "trained-8b"),
        ("GRPO 8B (sparse reward)", "eval_match_fresh_sparse.json", "trained-8b"),
    ]},
    "sans_match": {"slice": "heldout 600-899 (unbiased)", "arms": [
        ("untrained 8B", "eval_sansmatch_unbiased_untrained-8b.json", "untrained-8b"),
        ("untrained 32B", "eval_sansmatch_unbiased_untrained-32b.json", "untrained-32b"),
        ("GRPO 8B", "eval_sansmatch_unbiased_trained-8b.json", "trained-8b"),
    ]},
    "tof_chopper": {"slice": "heldout 300-599", "arms": [
        ("untrained 8B", "eval_tof_fresh_untrained-8b.json", "untrained-8b"),
        ("untrained 32B", "eval_tof_fresh_untrained-32b.json", "untrained-32b"),
        ("GRPO 8B", "eval_tof_fresh_trained-8b.json", "trained-8b"),
    ]},
    "bender": {"slice": "heldout 300-599", "arms": [
        ("untrained 8B", "eval_bender_fresh_untrained-8b.json", "untrained-8b"),
        ("untrained 32B", "eval_bender_fresh_untrained-32b.json", "untrained-32b"),
        ("GRPO 8B", "eval_bender_fresh_trained-8b.json", "trained-8b"),
    ]},
}
TRAINED = "GRPO 8B"


def build(family: str, root: str) -> dict | None:
    """One family's table, or None when no record for it is present."""
    spec = SPECS[family]
    table, rows_by_label = [], {}
    for label, fname, arm in spec["arms"]:
        path = os.path.join(root, fname)
        if not os.path.exists(path):
            continue
        rows = rows_of(path, arm, family)
        rows_by_label[label] = rows
        table.append({"policy": label, **rate(rows), "source": fname})
    if not table:
        return None
    out = {"family": family, "slice": spec["slice"], "rows": table, "paired": {}}
    # every trained arm against both untrained arms: the four-family claim is
    # about beating the untrained 32B, not only the 8B it was trained from
    for base_label in ("untrained 8B", "untrained 32B"):
        base = rows_by_label.get(base_label)
        if base is None:
            continue
        for label, rows in rows_by_label.items():
            if label.startswith(TRAINED):
                out["paired"][f"{label} vs {base_label}"] = paired(rows, base)
    if {"GRPO 8B (seed 1)", "GRPO 8B (seed 2)"} <= rows_by_label.keys():
        out["paired"]["seed 1 vs seed 2"] = paired(rows_by_label["GRPO 8B (seed 1)"],
                                                   rows_by_label["GRPO 8B (seed 2)"])
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--family", default="all",
                    choices=["all", *SPECS], help="default: every gated family")
    ap.add_argument("--root", default=None,
                    help="directory holding the eval records; defaults to "
                         "runs/m8, falling back to the frozen evidence copy")
    ap.add_argument("--out", default=os.path.join(REPO, "runs", "m8", "m8_table.json"))
    a = ap.parse_args()

    root = a.root or os.path.join(REPO, "runs", "m8")
    if a.root is None and not os.path.isdir(root):
        root = os.path.join(REPO, "benchmark", "evidence", "m8_rl", "eval")
    fams = list(SPECS) if a.family == "all" else [a.family]

    report = {"root": os.path.relpath(root, REPO), "families": {}}
    for fam in fams:
        built = build(fam, root)
        if built is None:
            print(f"  (no records for {fam} under {root})")
            continue
        report["families"][fam] = built
        print(f"\n### {fam} — {built['slice']}")
        print("| policy | passed | rate | 95% CI |")
        print("|---|---|---|---|")
        for r in built["rows"]:
            print(f"| {r['policy']} | {r['passes']}/{r['n']} | {r['rate']:.1%} | "
                  f"[{r['ci95'][0]:.1%}, {r['ci95'][1]:.1%}] |")
        for k, v in built["paired"].items():
            print(f"  {k}: {v['only_a']} vs {v['only_b']} discordant, "
                  f"McNemar p = {v['mcnemar_p']:.2g}")

    os.makedirs(os.path.dirname(a.out), exist_ok=True)
    with open(a.out, "w") as f:
        json.dump(report, f, indent=1)
    print(f"\n-> {a.out}")


if __name__ == "__main__":
    main()
