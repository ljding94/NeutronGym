"""Instance specifications that make the procedural families instance-specific.

2026-09-15: with seed noise removed (calibration v2), one fixed configuration
still reached >90% of each instance's optimum on 92% of guide and 68% of
SANS held-out instances. Pure "maximize flux" peaks are broad. Real design is
"maximize flux subject to the experiment's specification", which puts each
instance's optimum on its own specification boundary.
"""

import math

import pytest

from neutrongym import calibrate, generate, reward

N = 300


@pytest.mark.parametrize("family", list(generate.FAMILIES))
@pytest.mark.parametrize("split", ["train", "heldout"])
def test_baseline_meets_every_specification(family, split):
    base = generate.FAMILIES[family]["baseline"]
    for i in range(N):
        inst = generate.instance(family, split, i)
        res = reward._check_l1(inst, dict(base))
        assert res["pass"], (i, res)


def test_guide_divergence_spec_hand_values():
    ctx = {"wl": 5.0, "div_max": 1.0, "det_wh": 0.02}
    assert generate.check_guide_divergence_spec(ctx, {"m_coat": 2.0})["pass"]    # 0.99 deg
    bad = generate.check_guide_divergence_spec(ctx, {"m_coat": 2.1})             # 1.0395 deg
    assert not bad["pass"] and "m_coat must be <= 2.020" in bad["detail"]
    assert generate.guide_max_m(ctx) == pytest.approx(1.0 / (0.099 * 5.0))


def test_guide_beam_size_spec():
    ctx = {"det_wh": 0.02}
    assert generate.check_guide_beam_size_spec(ctx, {"w_out": 0.02})["pass"]
    assert not generate.check_guide_beam_size_spec(ctx, {"w_out": 0.021})["pass"]


def test_sans_resolution_hand_values():
    ctx = {"wl": 6.0, "det_dist": 3.0, "r_sphere": 100.0, "L_coll": 5.0}
    assert generate.sans_resolution_limit(ctx) == pytest.approx(18.0 / (2 * math.pi * 100.0))
    ok = {"r_pin1": 0.01, "r_pin2": 0.005}       # 0.005 + 0.015 * 3.2 / 5 = 0.0146
    assert generate.sans_detector_beam_radius(ctx, ok) == pytest.approx(0.0146)
    assert generate.check_sans_resolution(ctx, ok)["pass"]
    wide = {"r_pin1": 0.02, "r_pin2": 0.02}      # 0.02 + 0.04 * 0.64 = 0.0456
    assert not generate.check_sans_resolution(ctx, wide)["pass"]


def test_sans_resolution_limit_varies_strongly_across_heldout_instances():
    lims = [generate.sans_resolution_limit(generate.instance("sans_collimation", "heldout", i)["context"])
            for i in range(N)]
    assert max(lims) / min(lims) > 4


def test_guide_coating_cap_binds_inside_the_range_on_many_instances():
    caps = [generate.guide_max_m(generate.instance("guide_divergence", "heldout", i)["context"])
            for i in range(N)]
    inside = sum(1.0 < c < 3.0 for c in caps) / N
    assert inside > 0.3, inside
    assert min(caps) >= 1.0          # the m = 1 baseline is always allowed


def test_new_guide_context_is_appended_without_changing_earlier_draws():
    inst = generate.instance("guide_divergence", "heldout", 7)
    assert list(inst["context"])[-1] == "div_max"
    assert 0.85 <= inst["context"]["div_max"] <= 2.4


def test_family_signature_is_stable_distinct_and_tracks_definition(monkeypatch):
    g1 = generate.family_signature("guide_divergence")
    assert g1 == generate.family_signature("guide_divergence")
    assert g1 != generate.family_signature("sans_collimation")
    changed = dict(generate.FAMILIES["guide_divergence"], baseline={"w_in": 0.02, "w_out": 0.012, "m_coat": 1.0})
    monkeypatch.setitem(generate.FAMILIES, "guide_divergence", changed)
    assert generate.family_signature("guide_divergence") != g1


def test_cache_path_is_keyed_by_family_signature(tmp_path):
    inst = generate.instance("sans_collimation", "heldout", 0)
    p = calibrate.cache_path(str(tmp_path), inst)
    assert inst["family_signature"] in p
    assert calibrate.cache_path(str(tmp_path), {"id": "x"}).endswith(f"{calibrate.CAL_DIR}/x.json")


def test_prompt_states_the_numeric_specification():
    g = generate.render_prompt(generate.instance("guide_divergence", "heldout", 0))
    s = generate.render_prompt(generate.instance("sans_collimation", "heldout", 0))
    assert "Specification for this instance" in g and "m_coat <=" in g and "w_out <= det_wh" in g
    assert "Specification for this instance" in s and "resolution: q_min <= 1/r_sphere" in s
