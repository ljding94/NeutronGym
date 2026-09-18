"""Cochran–Armitage trend test — the M8 pre-registered primary endpoint.

Checked against an independent closed form rather than trusted: for a
2 x K table with scores s, the statistic reduces to

    z = (mean_b - mean_a) * sqrt(n_a * n_b / N) / sigma_pooled

where sigma_pooled is the population SD of the scores over both arms. The
implementation computes it through the textbook double-sum variance, so
agreement between the two forms is a real check.
"""

import math
import os
import sys

import pytest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))), "benchmark", "harness"))

from m8_eval import cochran_armitage  # noqa: E402


def closed_form_z(a: dict, b: dict) -> float:
    xs_a = [lv for lv, c in a.items() for _ in range(c)]
    xs_b = [lv for lv, c in b.items() for _ in range(c)]
    allx = xs_a + xs_b
    n = len(allx)
    mu = sum(allx) / n
    sigma = math.sqrt(sum((x - mu) ** 2 for x in allx) / n)
    diff = sum(xs_b) / len(xs_b) - sum(xs_a) / len(xs_a)
    return diff * math.sqrt(len(xs_a) * len(xs_b) / n) / sigma


def h(**kw):
    return {str(i): kw.get(f"L{i}", 0) for i in range(5)}


def test_matches_independent_closed_form_on_the_sweep_histograms():
    # the 1.0x sweep cells: untrained 8B vs untrained 32B
    a = h(L2=6, L3=26, L4=18)
    b = h(L2=6, L3=15, L4=29)
    got = cochran_armitage(a, b)
    want = closed_form_z({int(k): v for k, v in a.items()},
                         {int(k): v for k, v in b.items()})
    assert got["z"] == pytest.approx(want, abs=1e-3)
    assert got["p"] == pytest.approx(math.erfc(abs(want) / math.sqrt(2)),
                                     abs=1e-5)


def test_identical_distributions_give_no_trend():
    a = h(L2=3, L3=10, L4=12)
    got = cochran_armitage(a, dict(a))
    assert got["z"] == 0.0
    assert got["p"] == pytest.approx(1.0)


def test_direction_is_signed_and_symmetric():
    a, b = h(L3=30, L4=10), h(L3=10, L4=30)
    up, down = cochran_armitage(a, b), cochran_armitage(b, a)
    assert up["z"] > 0 > down["z"]
    assert up["z"] == pytest.approx(-down["z"])
    assert up["p"] == pytest.approx(down["p"])
    assert up["p"] < 1e-4


def test_uses_the_whole_ladder_not_just_pass_fail():
    """Moving failures from L2 to L3 is level migration even with the pass
    count unchanged — the reason this endpoint was pre-registered."""
    a = h(L2=20, L3=10, L4=20)
    b = h(L2=5, L3=25, L4=20)
    assert cochran_armitage(a, b)["z"] > 0


def test_empty_arm_is_untestable_not_significant():
    got = cochran_armitage(h(), h(L4=5))
    assert got == {"z": None, "p": None}


def test_single_level_everywhere_is_degenerate_not_a_crash():
    got = cochran_armitage(h(L4=10), h(L4=12))
    assert got["p"] == pytest.approx(1.0)


def test_eval_defaults_exclude_sans_and_rejects_unknown_families():
    import argparse
    import m8_eval
    assert m8_eval.parse_families("guide_divergence") == ["guide_divergence"]
    assert m8_eval.parse_families(" guide_divergence , sans_collimation ") == [
        "guide_divergence", "sans_collimation"]
    with pytest.raises(SystemExit):
        m8_eval.parse_families("guide_divergence,not_a_family")
    with pytest.raises(SystemExit):
        m8_eval.parse_families(" , ")


def _prev_eval(**over):
    rec = {"target_fraction": 1.0, "n_per_family": 300, "max_steps": 6,
           "split": "heldout", "temperature": 0.0,
           "heldout": {"untrained-8b": {"guide_divergence": {"rows": [
               {"instance": 0, "best_level": 4}]}},
                       "untrained-32b": {"guide_divergence": {"rows": [
               {"instance": 0, "best_level": 3}]}}}}
    rec.update(over)
    return rec


def test_reuse_takes_rows_when_protocol_matches():
    import m8_eval
    got = m8_eval.load_reusable(_prev_eval(), ["untrained-8b", "untrained-32b"],
                                target_fraction=1.0, n=300, max_steps=6,
                                families=["guide_divergence"])
    assert got["untrained-8b"]["guide_divergence"]["rows"][0]["best_level"] == 4
    assert set(got) == {"untrained-8b", "untrained-32b"}


@pytest.mark.parametrize("field,value", [("target_fraction", 0.8),
                                         ("n_per_family", 100),
                                         ("max_steps", 1),
                                         ("split", "train")])
def test_reuse_refuses_a_different_protocol(field, value):
    import m8_eval
    with pytest.raises(SystemExit):
        m8_eval.load_reusable(_prev_eval(**{field: value}), ["untrained-8b"],
                              target_fraction=1.0, n=300, max_steps=6,
                              families=["guide_divergence"])


def test_reuse_refuses_missing_family_rows():
    import m8_eval
    with pytest.raises(SystemExit):
        m8_eval.load_reusable(_prev_eval(), ["untrained-8b"],
                              target_fraction=1.0, n=300, max_steps=6,
                              families=["guide_divergence", "sans_collimation"])


def test_reuse_refuses_arms_measured_on_a_different_instance_slice():
    """Checkpoints were compared on held-out 0-299, so the chosen one is
    re-measured on 300-599 (2026-09-17). Mixing the two slices in one verdict
    would compare arms on different tasks."""
    import pytest
    prev = {"target_fraction": 0.85, "n_per_family": 300, "max_steps": 10,
            "split": "heldout", "temperature": 0.0, "start_index": 0,
            "heldout": {"untrained-8b": {"guide_match": {"rows": [{"instance": 0}]}}}}
    with pytest.raises(SystemExit, match="start_index"):
        m8_eval.load_reusable(prev, ["untrained-8b"], target_fraction=0.85, n=300,
                              max_steps=10, families=["guide_match"], start_index=300)
    ok = m8_eval.load_reusable(prev, ["untrained-8b"], target_fraction=0.85, n=300,
                               max_steps=10, families=["guide_match"], start_index=0)
    assert ok["untrained-8b"]["guide_match"]["rows"] == [{"instance": 0}]
