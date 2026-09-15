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
    for split in ("train", "heldout"):
        for i in range(N):
            inst = generate.instance(FAM, split, i)
            assert reward._check_l1(inst, dict(inst["baseline"]))["pass"], (split, i)


def test_worst_train_corner_still_gets_a_valid_baseline():
    """Shortest collimation with the longest detector distance is the hardest
    corner to keep inside the stop. The per-instance baseline must solve it
    rather than assume it, so no generator range can produce an unrunnable
    instance."""
    spans = generate.FAMILIES[FAM]["context"]
    worst = dict(generate.instance(FAM, "train", 0)["context"])
    worst.update(L_coll=spans["L_coll"]["train"][0],
                 det_dist=spans["det_dist"]["train"][1],
                 sample_wh=spans["sample_wh"]["train"][0],
                 stop_r=spans["stop_r"]["train"][0])
    base = generate.sans_baseline(worst)
    assert not generate.sans_direct_beam_leaks(worst, base)
    assert generate.sans_sample_beam_radius(worst, base) <= worst["sample_wh"] / 2
    # and it tracks the instance: a longer collimator admits a wider beam
    loose = dict(worst, L_coll=spans["L_coll"]["train"][1])
    assert generate.sans_baseline(loose)["r_pin1"] > base["r_pin1"]


def test_family_static_checks_do_not_cross_over():
    """Each family carries only its own specification checks: the SANS
    beamstop/resolution checks never gate guide actions, and vice versa."""
    guide = generate.instance("guide_divergence", "heldout", 0)
    sans = generate.instance(FAM, "heldout", 0)
    assert set(guide["static_checks"]) == {"guide_divergence_spec", "guide_beam_size_spec"}
    assert set(sans["static_checks"]) == {"direct_beam_on_stop", "sans_resolution",
                                          "sans_beam_fits_sample"}
    ok = {"w_in": 0.05, "w_out": guide["context"]["det_wh"], "m_coat": 1.0}
    assert reward._check_l1(guide, ok) == {"pass": True}


def test_bounds_are_checked_before_geometry():
    inst = generate.instance(FAM, "heldout", 0)
    res = reward._check_l1(inst, {"r_pin1": 0.5, "r_pin2": 0.005})
    assert not res["pass"] and "outside bounds" in res["detail"]


def test_prompt_discloses_this_instance_beamstop_radius():
    inst = generate.instance(FAM, "heldout", 0)
    stop = generate.sans_stop_radius(inst["context"])
    assert f"stop_r = {stop:.4f} m" in generate.render_prompt(inst)
    assert 0.011 <= stop <= 0.025


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
                                       n_random=60, rounds=0)
    assert fx.actions, "some candidates must pass L1 and be simulated"
    assert all(not generate.sans_direct_beam_leaks(inst["context"], a)
               for a in fx.actions)
    assert rec["ok"] is False  # the recording executor fails every run
