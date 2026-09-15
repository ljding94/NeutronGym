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

BY_BAR = {"0.8": {"share": 0.16}, "0.9": {"share": 0.44}}


def test_clean_run_yields_a_per_bar_verdict():
    got = gate_readout(BY_BAR, skipped=[], max_rate=0.20)
    assert got == {"0.8": True, "0.9": False}


def test_any_skip_voids_the_verdict_entirely():
    assert gate_readout(BY_BAR, [{"index": 10, "reason": "no valid candidate"}],
                        max_rate=0.20) is None


def test_a_passing_share_is_not_rescued_by_being_generous():
    """Even an all-passing table is void when instances were dropped."""
    assert gate_readout({"0.8": {"share": 0.0}}, [{"index": 3}], 0.20) is None
