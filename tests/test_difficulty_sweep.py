"""The difficulty knob and the sweep's readouts.

Guards two things the 2026-09-13 gate redefinition depends on:

1. A different target fraction must be a pure RESCALE of the cached
   classical optimum — no new simulations. If this regresses, the sweep
   silently becomes unaffordable (and, worse, would re-run the classical
   search per difficulty and give each level a different optimum, which
   would confound difficulty with search luck).
2. The sweep's summary must separate level migration, efficiency and
   quality, since pass rate alone is what failed to discriminate.
"""

import json
import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))), "benchmark", "harness"))

from neutrongym import calibrate  # noqa: E402

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))), "benchmark", "harness"))
from m8_difficulty_sweep import summarize  # noqa: E402


class _NoRunExecutor:
    """Any simulation call here is a bug — a cached instance must never
    re-run the classical search just because the fraction changed."""

    def run(self, *a, **k):  # noqa: D102
        raise AssertionError("rescaling a cached calibration must not "
                             "invoke the executor")


def _write_cache(tmp_path, inst_id, over):
    d = tmp_path / "calibration"
    d.mkdir(parents=True, exist_ok=True)
    (d / f"{inst_id}.json").write_text(json.dumps({
        "ok": True, "classical_over_baseline": over,
        "target_ratio": round(0.8 * over, 6), "fraction": 0.8,
        "baseline_fom": 1.0, "classical_fom_verified": over}))


def test_fraction_rescales_cached_optimum_without_simulating(tmp_path):
    inst = {"id": "guide_divergence-heldout-000001"}
    _write_cache(tmp_path, inst["id"], over=2.5)
    fx, base = _NoRunExecutor(), {"fom": 1.0}

    # default fraction returns the stored ratio verbatim
    assert calibrate.calibrated_target_ratio(
        inst, fx, base, str(tmp_path)) == 0.8 * 2.5
    # a harder bar is 1.2 x the SAME classical optimum
    assert calibrate.calibrated_target_ratio(
        inst, fx, base, str(tmp_path), fraction=1.2) == 3.0
    # an easier one likewise
    assert calibrate.calibrated_target_ratio(
        inst, fx, base, str(tmp_path), fraction=0.5) == 1.25


def test_legacy_cache_without_classical_over_baseline_still_rescales(tmp_path):
    """Entries written before 2026-09-13 lack the explicit ratio; the
    fraction must still be recoverable from target_ratio / fraction."""
    inst = {"id": "sans_collimation-heldout-000002"}
    d = tmp_path / "calibration"
    d.mkdir(parents=True, exist_ok=True)
    (d / f"{inst['id']}.json").write_text(json.dumps({
        "ok": True, "target_ratio": 1.6, "fraction": 0.8}))
    got = calibrate.calibrated_target_ratio(
        inst, _NoRunExecutor(), {"fom": 1.0}, str(tmp_path), fraction=1.0)
    assert got == 2.0


def test_failed_calibration_returns_none_at_any_fraction(tmp_path):
    inst = {"id": "guide_divergence-heldout-000003"}
    d = tmp_path / "calibration"
    d.mkdir(parents=True, exist_ok=True)
    (d / f"{inst['id']}.json").write_text(json.dumps(
        {"ok": False, "reason": "no constraint-valid sample found"}))
    for frac in (None, 0.8, 1.5):
        assert calibrate.calibrated_target_ratio(
            inst, _NoRunExecutor(), {"fom": 1.0}, str(tmp_path),
            fraction=frac) is None


def test_summarize_separates_migration_efficiency_and_quality():
    rows = [
        {"best_level": 4, "steps": 2, "best_fom_ratio": 1.5},
        {"best_level": 4, "steps": 6, "best_fom_ratio": 3.0},
        {"best_level": 2, "steps": 6, "best_fom_ratio": 0.4},
        {"best_level": None, "error": "boom"},   # excluded everywhere
    ]
    s = summarize(rows)
    assert s["n_valid"] == 3 and s["passes"] == 2
    assert s["pass_rate"] == round(2 / 3, 4)
    assert s["level_histogram"] == {"0": 0, "1": 0, "2": 1, "3": 0, "4": 2}
    # efficiency is over PASSING episodes only — averaging in failures that
    # burned the step budget would make a worse model look faster
    assert s["steps_to_success_median"] == 4.0
    assert s["steps_all_median"] == 6.0
    assert s["fom_ratio_median"] == 1.5
    assert s["fom_ratio_max"] == 3.0


def test_summarize_empty_is_none_not_zero():
    """A cell with no valid episodes must not report a pass rate of 0 —
    that is an infra result, and the M6 taxonomy lesson was that infra
    zeros scored as capability zeros."""
    s = summarize([{"best_level": None, "error": "endpoint down"}])
    assert s["n_valid"] == 0
    assert s["pass_rate"] is None
    assert s["steps_to_success_median"] is None
