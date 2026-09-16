"""A diagnostic that drops instances must not report a gate verdict.

Calibration fails on the TIGHT instances — small sample, short collimation —
which are exactly the ones a single constant answer cannot solve. Dropping
them raises the apparent pass share of "no constant works", so a skipped run
flatters the family. A v5 SANS run had to be killed by hand for this
(2026-09-15); this pins the refusal so it no longer needs to be noticed.
"""

import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))), "benchmark", "harness"))

from family_diagnostic import gate_readout  # noqa: E402

BY_BAR = {"0.8": {"share": 0.12, "passes": 18}, "0.9": {"share": 0.44, "passes": 66}}


def test_clean_powered_run_certifies_the_low_bar_and_fails_the_high_one():
    got = gate_readout(BY_BAR, skipped=[], max_rate=0.20, n=150)
    assert got["0.8"]["ok"] is True and got["0.8"]["underpowered"] is False
    assert got["0.9"]["ok"] is False and got["0.9"]["underpowered"] is False


def test_the_same_shares_certify_nothing_at_n_25():
    """Identical observed shares, a quarter of the evidence: the readout must
    not certify. This is the error the n=25 guide verdict was reported on."""
    small = {"0.8": {"share": 0.16, "passes": 4}}
    got = gate_readout(small, [], max_rate=0.20, n=25)
    assert got["0.8"]["ok"] is False
    assert got["0.8"]["underpowered"] is True     # not "the family is bad"
    assert got["0.8"]["upper_95"] > 0.20


def test_even_n_150_cannot_certify_an_observed_16_percent():
    """Worth knowing before reading a result: at n=150 the ceiling is only
    certifiable up to roughly 12-13% observed. 24/150 = 16% looks
    comfortably under 20% and still cannot support the claim."""
    got = gate_readout({"0.8": {"share": 0.16, "passes": 24}}, [], 0.20, n=150)
    assert got["0.8"]["ok"] is False and got["0.8"]["underpowered"] is True


def test_any_skip_voids_the_verdict_entirely():
    assert gate_readout(BY_BAR, [{"index": 10, "reason": "no valid candidate"}],
                        max_rate=0.20, n=150) is None


def test_a_passing_share_is_not_rescued_by_being_generous():
    """Even an all-passing table is void when instances were dropped."""
    assert gate_readout({"0.8": {"share": 0.0, "passes": 0}},
                        [{"index": 3}], 0.20, n=150) is None
