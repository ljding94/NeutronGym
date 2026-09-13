"""Paired-analysis regressions.

The M8 conclusion turns on this arithmetic, so it is tested against hand-
checked values rather than trusted. In particular the 4-vs-4 split that
produced p = 1.0 is the evidence for "no ordering exists"; if McNemar were
computed wrongly in either direction the project would draw the opposite
conclusion from the same data.
"""

import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))), "benchmark", "harness"))

from m8_paired import (analyze, mcnemar_exact_p, pair_rows,  # noqa: E402
                       report)

M = ("qwen3-8b", "qwen3-32b")


def test_mcnemar_matches_hand_computed_values():
    # no discordant pairs: nothing to test, p = 1 (not 0)
    assert mcnemar_exact_p(0, 0) == 1.0
    # the observed phase-0 split: perfectly balanced -> no evidence
    assert mcnemar_exact_p(4, 4) == 1.0
    # 2 * P(X <= 0) = 2 * (1/2)^5 = 0.0625
    assert mcnemar_exact_p(0, 5) == 0.0625
    # 2 * P(X <= 1) = 2 * (1 + 6) / 2^6 = 0.21875
    assert mcnemar_exact_p(1, 5) == 0.21875
    # 2 * P(X <= 1) over 8 pairs = 2 * (1 + 8) / 2^8 -- note this does NOT
    # clear 0.05, so a 1-vs-7 split is still not decisive evidence
    assert mcnemar_exact_p(1, 7) == 2 * 9 / 256 == 0.0703125
    assert mcnemar_exact_p(1, 7) > 0.05
    # a clean sweep of 7 discordant pairs does: 2 * 1 / 2^7
    assert mcnemar_exact_p(0, 7) == 0.015625
    assert mcnemar_exact_p(0, 7) < 0.05
    # symmetry: direction must not change the p-value
    assert mcnemar_exact_p(7, 1) == mcnemar_exact_p(1, 7)
    # p is a probability in every case
    for b in range(8):
        for c in range(8):
            assert 0.0 <= mcnemar_exact_p(b, c) <= 1.0


def _rec(rows_8b, rows_32b, fam="guide_divergence"):
    def mk(levels):
        return {"rows": [{"instance": i, "best_level": lv}
                         for i, lv in enumerate(levels)]}
    return {"heldout": {"qwen3-8b": {fam: mk(rows_8b), "_all": {}},
                        "qwen3-32b": {fam: mk(rows_32b), "_all": {}}}}


def test_balanced_discordance_reports_no_ordering():
    """The phase-0 shape: equal marginals reached by DIFFERENT instances."""
    rec = _rec([4, 4, 3, 3], [3, 3, 4, 4])
    res = analyze(pair_rows(rec, M), M)
    assert res["n_paired"] == 4
    assert res["both_pass"] == 0 and res["neither"] == 0
    assert res["only_qwen3-8b"] == 2 and res["only_qwen3-32b"] == 2
    assert res["mcnemar_p"] == 1.0
    assert res["ordering_supported"] is False
    assert res["agreement"] == 0.0  # identical pass RATES, zero agreement


def test_skewed_discordance_reports_an_ordering():
    rec = _rec([3] * 8, [4] * 7 + [3])  # 7 discordant, all one way
    res = analyze(pair_rows(rec, M), M)
    assert res["only_qwen3-32b"] == 7 and res["only_qwen3-8b"] == 0
    assert res["mcnemar_p"] < 0.05
    assert res["ordering_supported"] is True


def test_concordant_instances_carry_no_information():
    """Adding instances both models pass must not change the verdict —
    that is exactly what conditioning on discordant pairs means."""
    base = analyze(pair_rows(_rec([4, 3], [3, 4]), M), M)
    padded = analyze(pair_rows(_rec([4, 3] + [4] * 20,
                                    [3, 4] + [4] * 20), M), M)
    assert base["mcnemar_p"] == padded["mcnemar_p"]
    assert padded["both_pass"] == 20


def test_pairing_drops_instances_missing_from_one_model():
    rec = _rec([4, 4, 4], [4, 4])
    paired = pair_rows(rec, M)
    assert len(paired) == 2, "an unpaired instance cannot enter a paired test"


def test_pairing_keeps_families_separate():
    """Instance index 0 of two different families is two different tasks."""
    rec = _rec([4], [3], fam="guide_divergence")
    other = _rec([3], [4], fam="sans_collimation")
    for m in M:
        rec["heldout"][m].update(other["heldout"][m])
    res = analyze(pair_rows(rec, M), M)
    assert res["n_paired"] == 2
    assert res["only_qwen3-8b"] == 1 and res["only_qwen3-32b"] == 1
    assert set(res["per_family"]) == {"guide_divergence", "sans_collimation"}


def test_skewed_but_underpowered_is_not_reported_as_no_difference():
    """1-vs-5 (the uncalibrated phase-0 shape) leans toward the 32B but
    cannot be resolved at this n. Reporting it as 'no ordering' — the same
    verdict as a balanced 4-vs-4 — would misstate the evidence."""
    rec = _rec([4] + [3] * 5 + [4] * 3, [3] + [4] * 5 + [4] * 3)
    res = analyze(pair_rows(rec, M), M)
    assert res["only_qwen3-8b"] == 1 and res["only_qwen3-32b"] == 5
    assert res["ordering_supported"] is False   # p = 0.219
    assert res["balanced"] is False             # ...but NOT balanced
    assert res["leader"] == "qwen3-32b"
    text = report(res)
    assert "UNDERPOWERED, NOT NULL" in text
    assert "NO ORDERING" not in text


def test_balanced_split_is_reported_as_no_ordering():
    rec = _rec([4, 4, 3, 3], [3, 3, 4, 4])
    res = analyze(pair_rows(rec, M), M)
    assert res["balanced"] is True and res["leader"] is None
    text = report(res)
    assert "NO ORDERING" in text and "UNDERPOWERED" not in text


def test_odd_balanced_split_still_counts_as_balanced():
    """3-vs-4 is as close to even as an odd number of pairs can get."""
    rec = _rec([4] * 3 + [3] * 4, [3] * 3 + [4] * 4)
    res = analyze(pair_rows(rec, M), M)
    assert (res["only_qwen3-8b"], res["only_qwen3-32b"]) == (3, 4)
    assert res["balanced"] is True


def test_significance_threshold_is_derived_not_hardcoded():
    from m8_paired import _n_needed_for_significance
    n = _n_needed_for_significance()
    assert mcnemar_exact_p(0, n) < 0.05
    assert mcnemar_exact_p(0, n - 1) >= 0.05
    assert n == 6  # 2 / 2^6 = 0.03125


def test_pair_rows_reads_the_sweep_record_shape():
    """m8_difficulty_sweep persists rows as
    rows_by_fraction[frac][model][family]['rows'], and the sweep feeds that
    sub-dict straight into pair_rows under a synthetic 'heldout' key. If
    these shapes drift apart the sweep's paired column silently empties."""
    sweep = {"rows_by_fraction": {"1.0": {
        "qwen3-8b": {"guide_divergence": {"rows": [
            {"instance": 0, "best_level": 4},
            {"instance": 1, "best_level": 3}]}},
        "qwen3-32b": {"guide_divergence": {"rows": [
            {"instance": 0, "best_level": 3},
            {"instance": 1, "best_level": 4}]}}}}}
    paired = pair_rows({"heldout": sweep["rows_by_fraction"]["1.0"]}, M)
    assert len(paired) == 2
    res = analyze(paired, M)
    assert res["only_qwen3-8b"] == 1 and res["only_qwen3-32b"] == 1
    assert res["balanced"] is True
