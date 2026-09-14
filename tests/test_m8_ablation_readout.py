"""The pre-specified ablation verdict is computed, not eyeballed."""

import os
import sys

import pytest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))), "benchmark", "harness"))

from m8_ablation_readout import readout  # noqa: E402

FAM = "guide_divergence"


def rec(untrained, trained):
    mk = lambda levels: {FAM: {"rows": [{"instance": i, "best_level": lv}
                                        for i, lv in enumerate(levels)]}}
    return {"heldout": {"untrained-8b": mk(untrained), "trained-8b": mk(trained)}}


U = [4] * 40 + [3] * 60                     # untrained 8B: 40/100
TURN = [4] * 20 + [3] * 20 + [3] * 60        # per-turn: lost 20 passes


def test_supported_when_passing_model_holds_and_beats_per_turn():
    passing = [4] * 40 + [3] * 60             # identical to untrained
    res = readout(rec(U, TURN), rec(U, passing))
    assert res["passing_regresses_vs_untrained"] is False
    assert res["passing_beats_per_turn"] is True
    assert res["verdict"] == "supported"
    assert res["label"] == "post-hoc"


def test_refuted_when_passing_model_regresses_as_much():
    res = readout(rec(U, TURN), rec(U, list(TURN)))
    assert res["verdict"] == "refuted"


def test_inconclusive_when_it_neither_holds_clearly_nor_regresses_fully():
    passing = [4] * 32 + [3] * 8 + [3] * 60    # lost 8: not significant either way
    res = readout(rec(U, TURN), rec(U, passing))
    assert res["verdict"] == "inconclusive"


def test_refuses_if_the_comparator_rows_are_not_the_reused_ones():
    other_u = [3] * 40 + [4] * 60
    with pytest.raises(SystemExit):
        readout(rec(U, TURN), rec(other_u, TURN))
