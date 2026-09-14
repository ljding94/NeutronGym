"""SANS direct-beam hole: closed at L1, before any simulation.

A constant all-max pinhole policy, with no model, used to pass 8/25 held-out
SANS instances at the 1.0x calibrated bar (one at 11,992x the target):
the unscattered beam spilled around the beamstop and the total-intensity
FOM counted it as scattering (note/sans-direct-beam-exploit-2026-09-13.md).
These tests pin the fix: the corner is rejected everywhere, the baseline is
valid everywhere, the guide family is untouched, and calibration can no
longer pick a leaking configuration as its classical optimum.
"""

from neutrongym import calibrate, generate, reward

FAM = "sans_collimation"
N = 200


def _all_max():
    return {k: hi for k, (lo, hi) in generate.FAMILIES[FAM]["free_parameters"].items()}


def test_all_max_is_rejected_at_l1_on_every_instance():
    for split in ("train", "heldout"):
        for i in range(N):
            inst = generate.instance(FAM, split, i)
            res = reward._check_l1(inst, _all_max())
            assert not res["pass"], (split, i)
            assert res["check"] == "direct_beam_on_stop"
            assert "beamstop" in res["detail"]


def test_baseline_is_valid_at_l1_on_every_instance():
    base = generate.FAMILIES[FAM]["baseline"]
    for split in ("train", "heldout"):
        for i in range(N):
            inst = generate.instance(FAM, split, i)
            assert reward._check_l1(inst, dict(base))["pass"], (split, i)


def test_train_collimation_floor_keeps_the_baseline_inside_the_stop():
    """The worst train corner (shortest collimation, longest detector
    distance) must still admit the baseline; lowering the L_coll floor would
    silently reintroduce invalid baselines."""
    ctx = generate.FAMILIES[FAM]["context"]
    worst = {"L_coll": ctx["L_coll"]["train"][0],
             "det_dist": ctx["det_dist"]["train"][1]}
    base = generate.FAMILIES[FAM]["baseline"]
    assert not generate.sans_direct_beam_leaks(worst, base)


def test_guide_family_has_no_static_checks_and_l1_is_unchanged():
    inst = generate.instance("guide_divergence", "heldout", 0)
    assert inst["static_checks"] == []
    corner = {k: hi for k, (lo, hi) in inst["free_parameters"].items()}
    assert reward._check_l1(inst, corner) == {"pass": True}


def test_bounds_are_checked_before_geometry():
    inst = generate.instance(FAM, "heldout", 0)
    res = reward._check_l1(inst, {"r_pin1": 0.5, "r_pin2": 0.005})
    assert not res["pass"] and "outside bounds" in res["detail"]


def test_prompt_discloses_the_beamstop_rule():
    inst = generate.instance(FAM, "heldout", 0)
    assert "0.02 m" in generate.render_prompt(inst)


class _RecordingExecutor:
    def __init__(self):
        self.actions = []

    def run(self, params, ncount=None, seed=None):
        self.actions.append({k: params[k] for k in ("r_pin1", "r_pin2")})
        return {"ok": False}


def test_calibration_never_simulates_a_leaking_candidate():
    inst = generate.instance(FAM, "heldout", 0)
    fx = _RecordingExecutor()
    rec = calibrate.calibrate_instance(inst, fx, {"fom": 1.0, "constraints": {}},
                                       n_samples=60)
    assert fx.actions, "some candidates must pass L1 and be simulated"
    assert all(not generate.sans_direct_beam_leaks(inst["context"], a)
               for a in fx.actions)
    assert rec["ok"] is False  # the recording executor fails every run
