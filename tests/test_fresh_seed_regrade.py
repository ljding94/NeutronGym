"""Fresh-seed re-grade of passing designs (2026-09-24)."""

import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))), "benchmark", "harness"))

import fresh_seed_regrade as fsr  # noqa: E402


def _row(instance, seed, holds, usable=True):
    return {"instance": instance, "seed": seed, "usable": usable, "holds": holds}


def test_summary_separates_per_check_from_per_design():
    """A design that holds at 2 of 3 seeds counts once per check but must NOT
    count as surviving -- the strict figure is holds-at-every-seed."""
    rows = [_row(1, 11, True), _row(1, 12, True), _row(1, 13, False),
            _row(2, 11, True), _row(2, 12, True), _row(2, 13, True)]
    s = fsr.summarize(rows)
    assert s["designs"] == 2
    assert s["holds_per_check"] == 5 and s["usable_checks"] == 6
    assert s["holds_at_every_seed"] == 1          # only design 2
    assert s["holds_at_some_seed"] == 2
    assert s["fails_at_every_seed"] == 0
    assert s["rate_every_seed"] == 0.5


def test_unusable_checks_are_excluded_not_counted_as_failures():
    """A starved monitor is missing data, not a failed design; counting it as
    a failure would understate robustness."""
    rows = [_row(1, 11, True), {"instance": 1, "seed": 12, "usable": False,
                                "error": "observable unavailable"}]
    s = fsr.summarize(rows)
    assert s["unusable_checks"] == 1
    assert s["usable_checks"] == 1
    assert s["rate_per_check"] == 1.0
    assert s["holds_at_every_seed"] == 1


def test_design_failing_everywhere_is_counted():
    rows = [_row(7, 11, False), _row(7, 12, False)]
    s = fsr.summarize(rows)
    assert s["holds_at_every_seed"] == 0
    assert s["fails_at_every_seed"] == 1
    assert s["rate_every_seed"] == 0.0


def test_empty_input_does_not_divide_by_zero():
    s = fsr.summarize([])
    assert s["rate_per_check"] is None and s["rate_every_seed"] is None
    assert s["designs"] == 0
