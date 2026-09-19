"""Target-matching family guide_match (2026-09-17).

Maximisation families were solvable by short no-model rules; here each
instance's targets are the simulated beam of a hidden design, and a pass
needs every matched observable within tolerance.
"""

import pytest

from neutrongym import calibrate, generate, reward, rollouts

FAM = "guide_match"


def test_hidden_design_is_deterministic_in_bounds_and_never_in_the_prompt():
    a = generate.instance(FAM, "heldout", 3)
    b = generate.instance(FAM, "heldout", 3)
    assert a["hidden_action"] == b["hidden_action"]
    for k, (lo, hi) in a["free_parameters"].items():
        assert lo <= a["hidden_action"][k] <= hi
    p = generate.render_prompt(dict(a, targets=[1.5, 0.4]))
    for v in a["hidden_action"].values():
        assert f"{v}" not in p
    assert "hidden" not in p and "TARGETS" in p and "+/-5%" in p


def test_other_family_signatures_unchanged_by_the_new_family():
    assert generate.family_signature("guide_divergence") == "93617bc8ce"
    assert generate.family_signature("sans_collimation") == "8f8f7e3b09"


def _summary(spot, div, events=5000):
    return {"monitors": [
        {"component": "psd", "intensity": 1.0, "events": events, "beam_width": {"dX": spot}},
        {"component": "divmon", "intensity": 1.0, "events": events, "beam_width": {"dX": div}}]}


def _inst(targets=(2.0, 0.5)):
    return dict(generate.instance(FAM, "heldout", 0), targets=list(targets))


def test_match_scoring_pass_ratio_and_reward():
    inst = _inst()
    rec = reward._score_match(inst, _summary(2.06, 0.49), {"level": 3, "reward": 0.75}, {})
    # worst relative error 3% vs 5% tolerance -> ratio 5/3
    assert rec["level"] == 4 and rec["match"]["rel_err"] == [0.03, 0.02]
    assert rec["reward"] == pytest.approx(0.75 + 0.25 * (0.05 / 0.03), rel=1e-4)
    levels = {}
    rec = reward._score_match(inst, _summary(2.0, 0.56), {"level": 3, "reward": 0.75}, levels)
    assert rec["level"] == 3 and levels["L4"]["pass"] is False    # divergence 12% off
    assert levels["L4"]["fom_ratio"] == pytest.approx(0.05 / 0.12, rel=1e-3)


def test_both_quantities_must_match_not_just_one():
    levels = {}
    reward._score_match(_inst(), _summary(2.0, 0.53), {"level": 3, "reward": 0.75}, levels)
    assert levels["L4"]["pass"] is False                             # spot exact, divergence 6%


def test_feedback_reports_measured_against_target_per_quantity():
    inst = _inst()
    obs = {"instance": inst, "baseline_fom": 1.0, "feedback": {
        "level": 3, "failed_at": "L4", "detail": None, "fom": 1.0, "fom_ratio": 0.4,
        "repeat_of": None, "match": {"measured": [2.2, 0.5], "targets": [2.0, 0.5],
                                     "rel_err": [0.1, 0.0], "tolerance": 0.05}}}
    msg = rollouts.feedback_message(obs)
    assert "spot size (horizontal std) 2.2 cm (target 2, +10.0%)" in msg
    assert "within +/-5%" in msg and "x the baseline" not in msg


class _Exec:
    def __init__(self, summary):
        self.summary = summary

    def run(self, params, ncount=None, seed=None):
        return {"ok": True, "summary": self.summary, "elapsed_s": 0.0}


def test_calibration_uses_the_hidden_design_and_rejects_starved_monitors(tmp_path):
    inst = generate.instance(FAM, "heldout", 1)
    fx = _SeqExec([_summary(9.0, 9.0), _summary(1.7, 0.33)])      # baseline far away
    cal = calibrate.calibration_for(inst, fx, {"fom": 1.0}, str(tmp_path))
    assert cal["targets"] == [1.7, 0.33]
    assert cal["classical_action"] == inst["hidden_action"]
    bad = calibrate.calibrate_match(inst, _Exec(_summary(1.7, 0.33, events=10)))
    assert bad["ok"] is False and bad["reason"] == "no usable hidden design"


class _SeqExec:
    """Baseline run first, then one summary per hidden candidate."""

    def __init__(self, summaries):
        self.summaries = list(summaries)

    def run(self, params, ncount=None, seed=None):
        return {"ok": True, "summary": self.summaries.pop(0), "elapsed_s": 0.0}


def test_hidden_designs_the_baseline_already_matches_are_skipped():
    inst = generate.instance(FAM, "heldout", 1)
    fx = _SeqExec([_summary(2.0, 0.5),            # baseline
                   _summary(2.05, 0.51),          # candidate 0: baseline within 2x tol -> skip
                   _summary(3.0, 0.8)])           # candidate 1: usable
    rec = calibrate.calibrate_match(inst, fx)
    assert rec["ok"] and rec["candidate"] == 1 and rec["targets"] == [3.0, 0.8]
    assert rec["rejected"] == [[0, "baseline already matches"]] or rec["rejected"] == [(0, "baseline already matches")]
    assert rec["hidden_action"] == generate.match_hidden_candidates(inst)[1]


@pytest.mark.slow
def test_real_env_hidden_design_matches_exactly_and_baseline_does_not(tmp_path):
    from neutrongym.env import NeutronGym
    env = NeutronGym(family=FAM, split="heldout", workdir=str(tmp_path), max_steps=5)
    obs, _ = env.reset(index=0)
    inst = obs["instance"]
    assert len(inst["targets"]) == 2 and all(t > 0 for t in inst["targets"])
    assert "TARGETS" in obs["prompt"]
    rec = env.step(dict(inst["hidden_action"]))[4]
    assert rec["level"] == 4 and rec["match"]["rel_err"] == [0.0, 0.0]
    rec = env.step(dict(inst["baseline"]))[4]
    assert rec["level"] == 3                 # never a pass: excluded at calibration
    assert max(rec["match"]["rel_err"]) > generate.MATCH_BASELINE_EXCLUSION * generate.MATCH_TOLERANCE


def test_ood_split_is_beyond_train_and_heldout_on_every_axis():
    """OOD separates learned geometry from interpolation of the training
    distribution (2026-09-18)."""
    spans = generate.FAMILIES[FAM]["context"]
    for k, rr in spans.items():
        lo, hi = rr["ood"]
        for other in ("train", "heldout"):
            olo, ohi = rr[other]
            assert lo >= ohi or hi <= olo or k == "dwl", (k, rr)
    for i in range(50):
        inst = generate.instance(FAM, "ood", i)
        for k, (lo, hi) in [(k, spans[k]["ood"]) for k in spans]:
            assert lo <= inst["context"][k] <= hi
    assert generate.instance(FAM, "ood", 0)["id"].startswith(f"{FAM}-ood-")


def test_unknown_split_and_missing_range_fail_loudly():
    with pytest.raises(ValueError, match="train|heldout|ood"):
        generate.instance(FAM, "validation", 0)
    with pytest.raises(ValueError, match="no 'ood' range"):
        generate.instance("sans_collimation", "ood", 0)


def test_match_tolerance_override_changes_the_bar_not_the_targets():
    from neutrongym import reward as rw
    inst = dict(generate.instance(FAM, "heldout", 0), targets=[2.0, 0.5])
    strict = dict(inst, fom=dict(inst["fom"], tolerance=0.02))
    summary = {"monitors": [
        {"component": "psd", "intensity": 1.0, "events": 5000, "beam_width": {"dX": 2.06}},
        {"component": "divmon", "intensity": 1.0, "events": 5000, "beam_width": {"dX": 0.5}}]}
    assert rw._score_match(inst, summary, {"level": 3, "reward": 0.75}, {})["level"] == 4
    assert rw._score_match(strict, summary, {"level": 3, "reward": 0.75}, {})["level"] == 3
