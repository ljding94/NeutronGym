"""Build the M8 result table with confidence intervals from the eval files.

Generated, never transcribed: every row is read from a runs/m8/*.json that an
evaluation wrote, so the paper's table cannot drift from the evidence.
Intervals are Clopper-Pearson (exact, one row at a time); paired comparisons
use exact McNemar on discordant pairs.

Usage: python benchmark/harness/m8_table.py [--out runs/m8/m8_table.json]
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


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", default=os.path.join(REPO, "runs", "m8", "m8_table.json"))
    a = ap.parse_args()
    R = os.path.join(REPO, "runs", "m8")
    F = "guide_match"
    spec = [
        ("untrained 8B", f"{R}/eval_match_fresh_untrained-8b.json", "untrained-8b", F),
        ("untrained 32B", f"{R}/eval_match_fresh_untrained-32b.json", "untrained-32b", F),
        ("GRPO 8B (seed 1)", f"{R}/eval_match_fresh_trained-8b.json", "trained-8b", F),
        ("GRPO 8B (seed 2)", f"{R}/eval_match_fresh_rep2.json", "trained-8b", F),
        ("GRPO 8B (sparse reward)", f"{R}/eval_match_fresh_sparse.json", "trained-8b", F),
    ]
    table, rows_by_label = [], {}
    for label, path, arm, fam in spec:
        if not os.path.exists(path):
            continue
        rows = rows_of(path, arm, fam)
        rows_by_label[label] = rows
        table.append({"policy": label, **rate(rows), "source": os.path.basename(path)})
    out = {"family": F, "slice": "heldout 300-599", "rows": table, "paired": {}}
    base = rows_by_label.get("untrained 8B")
    for label, rows in rows_by_label.items():
        if base is not None and label != "untrained 8B":
            out["paired"][f"{label} vs untrained 8B"] = paired(rows, base)
    if "GRPO 8B (seed 1)" in rows_by_label and "GRPO 8B (seed 2)" in rows_by_label:
        out["paired"]["seed 1 vs seed 2"] = paired(rows_by_label["GRPO 8B (seed 1)"],
                                                   rows_by_label["GRPO 8B (seed 2)"])
    with open(a.out, "w") as f:
        json.dump(out, f, indent=1)
    print(f"| policy | passed | rate | 95% CI |")
    print(f"|---|---|---|---|")
    for r in table:
        print(f"| {r['policy']} | {r['passes']}/{r['n']} | {r['rate']:.1%} | "
              f"[{r['ci95'][0]:.1%}, {r['ci95'][1]:.1%}] |")
    for k, v in out["paired"].items():
        print(f"  {k}: {v['only_a']} vs {v['only_b']} discordant, McNemar p = {v['mcnemar_p']:.2g}")
    print(f"-> {a.out}")


if __name__ == "__main__":
    main()
