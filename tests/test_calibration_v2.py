"""Calibration v2 (2026-09-15): strong optimum, scored like an agent.

v1 took the best of 30 random samples and re-verified it at a FRESH seed,
while agents were scored at the protocol seed. The seed mismatch (+/-3-10%
at 1e5 rays) let an instance's own optimum pass its own 1.0x target only
about half the time — pass/fail near the optimum was a coin flip.
"""

import json

import pytest

from neutrongym import calibrate, reward

PEAK = 0.3


class _Exec:
    """Smooth one-parameter FOM peaked at a = PEAK; records every run."""

    def __init__(self):
        self.runs = []

    def run(self, params, ncount, seed, **kw):
        self.runs.append((ncount, seed, params["a"]))
        f = 1.0 - (params["a"] - PEAK) ** 2
        return {"ok": True, "elapsed_s": 0.0, "summary": {"monitors": [
            {"component": "det", "intensity": f, "events": 1e6}]}}


def _inst(iid="stub-1"):
    return {"id": iid, "family": "stub", "context": {}, "static_checks": [],
            "free_parameters": {"a": (0.0, 1.0)}, "baseline": {"a": 0.9},
            "protocol": {"ncount": 1e5, "ncount_cheap": 1e4, "seed": 77,
                         "statistics_floor": 500},
            "fom": {"monitor": "det", "metric": "intensity", "maximize": True},
            "constraints": []}


def test_every_simulation_uses_the_protocol_seed():
    fx, inst = _Exec(), _inst()
    base = reward.baseline(inst, fx)
    rec = calibrate.calibrate_instance(inst, fx, base)
    assert rec["ok"] and rec["version"] == 2
    assert {seed for _, seed, _ in fx.runs} == {77}      # no fresh seed anywhere


def test_pattern_search_reaches_the_peak_beyond_random_sampling():
    fx, inst = _Exec(), _inst()
    rec = calibrate.calibrate_instance(inst, fx, reward.baseline(inst, fx))
    assert abs(rec["classical_action"]["a"] - PEAK) < 0.01
    assert rec["evals"] > calibrate.N_RANDOM + 1


def test_optimum_is_never_below_the_baseline():
    fx, inst = _Exec(), _inst()
    inst["free_parameters"] = {"a": (0.85, 1.0)}         # baseline 0.9 is near-best
    rec = calibrate.calibrate_instance(inst, fx, reward.baseline(inst, fx))
    assert rec["classical_over_baseline"] >= 1.0


def test_own_optimum_fails_at_1x_and_passes_just_below(tmp_path):
    """Noise-free comparison: an instance's own optimum scores a ratio of
    exactly 1.0 against its 1.0x target (strict L4 fails), and clears 0.95x."""
    fx, inst = _Exec(), _inst()
    base = reward.baseline(inst, fx)
    cal = calibrate.calibration_for(inst, fx, base, str(tmp_path), fraction=1.0)
    opt = json.load(open(calibrate.cache_path(str(tmp_path), inst)))["classical_action"]
    at_1x = dict(inst, target_ratio=cal["target_ratio"])
    rec = reward.score(at_1x, opt, fx, base)
    assert rec["levels"]["L4"]["fom_ratio"] == pytest.approx(1.0, abs=1e-9)
    assert rec["level"] == 3
    cal95 = calibrate.calibration_for(inst, fx, base, str(tmp_path), fraction=0.95)
    assert reward.score(dict(inst, target_ratio=cal95["target_ratio"]), opt, fx, base)["level"] == 4


def test_v1_cache_directory_is_never_read(tmp_path):
    fx, inst = _Exec(), _inst("stub-2")
    base = reward.baseline(inst, fx)
    v1 = tmp_path / "calibration"
    v1.mkdir()
    (v1 / "stub-2.json").write_text(json.dumps({
        "ok": True, "classical_over_baseline": 99.0, "target_ratio": 79.2,
        "fraction": 0.8, "classical_action": {"a": 0.9}}))
    cal = calibrate.calibration_for(inst, fx, base, str(tmp_path), fraction=1.0)
    assert cal["classical_over_baseline"] < 99.0                 # recomputed, not v1
    assert (tmp_path / calibrate.CAL_DIR / "stub-2.json").is_file()


def test_moving_the_default_bar_rescales_old_caches_instead_of_reusing_them():
    """TARGET_FRACTION moved 0.8 -> 0.85 on 2026-09-16. Cache entries written
    at the old bar store target_ratio = 0.8 * over; reusing that verbatim
    would grade those instances at 0.8 while the module reported 0.85."""
    import json
    import os
    import tempfile
    from neutrongym import calibrate

    class _NoRun:
        def run(self, *a, **k):
            raise AssertionError("rescaling must not simulate")

    with tempfile.TemporaryDirectory() as d:
        inst = {"id": "guide_divergence-heldout-000007"}
        p = os.path.join(d, calibrate.CAL_DIR)
        os.makedirs(p)
        with open(os.path.join(p, inst["id"] + ".json"), "w") as f:
            json.dump({"ok": True, "classical_over_baseline": 4.0,
                       "target_ratio": 0.8 * 4.0, "fraction": 0.8}, f)
        got = calibrate.calibrated_target_ratio(inst, _NoRun(), {"fom": 1.0}, d)
        assert got == round(calibrate.TARGET_FRACTION * 4.0, 12)
        assert got != 0.8 * 4.0


def test_legacy_entries_are_pinned_to_0_8_not_to_the_current_bar():
    """An entry with no `fraction` key was written at 0.8. Defaulting it to
    TARGET_FRACTION would re-derive its optimum wrongly once the bar moves."""
    import json
    import os
    import tempfile
    from neutrongym import calibrate

    assert calibrate.LEGACY_FRACTION == 0.8
    with tempfile.TemporaryDirectory() as d:
        inst = {"id": "guide_divergence-heldout-000008"}
        p = os.path.join(d, calibrate.CAL_DIR)
        os.makedirs(p)
        with open(os.path.join(p, inst["id"] + ".json"), "w") as f:
            json.dump({"ok": True, "target_ratio": 1.6}, f)   # no fraction key
        cal = calibrate.calibration_for(inst, None, {"fom": 1.0}, d)
        assert cal["classical_over_baseline"] == 2.0          # 1.6 / 0.8
