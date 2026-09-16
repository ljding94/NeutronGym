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
    for i in range(N):
        inst = generate.instance(family, split, i)
        # per instance since 2026-09-15: a family-constant baseline had to fit
        # the SMALLEST sample, so it was starved on every other instance
        res = reward._check_l1(inst, dict(inst["baseline"]))
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


def test_sans_beam_fits_sample_hand_value():
    ctx = {"L_coll": 5.0, "sample_wh": 0.012}
    a = {"r_pin1": 0.01, "r_pin2": 0.005}       # 0.005 + 0.015 * 0.2 / 5 = 0.0056
    assert generate.sans_sample_beam_radius(ctx, a) == pytest.approx(0.0056)
    assert generate.check_sans_beam_fits_sample(ctx, a)["pass"]
    wide = {"r_pin1": 0.02, "r_pin2": 0.008}    # 0.008 + 0.028 * 0.04 = 0.00912 > 0.006
    bad = generate.check_sans_beam_fits_sample(ctx, wide)
    assert not bad["pass"] and "sample" in bad["detail"]


def test_sans_sample_size_and_beamstop_vary_independently():
    """Two instance-specific bounds on different parameters: with one shared
    bound, a mid-sized constant fitted 40% of instances (2026-09-15)."""
    ctxs = [generate.instance("sans_collimation", "heldout", i)["context"] for i in range(50)]
    samples = {c["sample_wh"] for c in ctxs}
    stops = {c["stop_r"] for c in ctxs}
    assert len(samples) > 45 and len(stops) > 45
    assert min(samples) >= 0.007 and max(samples) <= 0.016
    # the two limits are not the same ranking of instances
    order_s = sorted(range(50), key=lambda i: ctxs[i]["sample_wh"])
    order_b = sorted(range(50), key=lambda i: ctxs[i]["stop_r"])
    assert order_s != order_b


def test_sans_prompt_quotes_the_sample_limit():
    inst = generate.instance("sans_collimation", "heldout", 0)
    half = inst["context"]["sample_wh"] / 2
    assert f"sample_wh / 2 = {half:.4f} m" in generate.render_prompt(inst)


def test_sans_baseline_is_sized_per_instance_and_always_valid():
    """One fixed baseline had to fit the SMALLEST sample and was starved
    everywhere else (0-360 detector events, 2/25 zero; 2026-09-15)."""
    seen = set()
    for split in ("train", "heldout"):
        for i in range(N):
            inst = generate.instance("sans_collimation", split, i)
            b = inst["baseline"]
            seen.add((b["r_pin1"], b["r_pin2"]))
            assert reward._check_l1(inst, dict(b))["pass"], (split, i, b)
    assert len(seen) > 400, "baselines must scale with the instance"


def test_sans_baseline_sits_below_every_specification_limit():
    inst = generate.instance("sans_collimation", "heldout", 0)
    c, b = inst["context"], inst["baseline"]
    assert generate.sans_sample_beam_radius(c, b) <= c["sample_wh"] / 2
    assert generate.sans_direct_beam_radius(c, b) <= generate.sans_stop_radius(c)
    assert generate.sans_detector_beam_radius(c, b) <= generate.sans_resolution_limit(c)
    # and it leaves headroom: the optimum must be able to beat it
    assert generate.SANS_BASELINE_FRACTION < 1.0


def test_guide_baseline_stays_a_fixed_action():
    inst = generate.instance("guide_divergence", "heldout", 0)
    assert inst["baseline"] == generate.FAMILIES["guide_divergence"]["baseline"]


def test_zero_flux_baseline_is_an_error_not_a_silent_uncalibrated_target():
    class _Exec:
        def run(self, params, ncount, seed, **kw):
            return {"ok": True, "elapsed_s": 0.0, "summary": {"monitors": [
                {"component": "detector", "intensity": 0.0, "events": 0}]}}

    inst = generate.instance("sans_collimation", "heldout", 0)
    out = reward.baseline(inst, _Exec())
    assert out["ok"] is False and "collects no signal" in out["detail"]


def test_sans_runs_more_rays_than_guide_because_it_needs_them():
    """ncount is per family (2026-09-15). SANS counts only scattered
    neutrons through two pinholes, so at the shared 1e5 its tightest
    instances collected 60 events — under the 500 floor, which left
    calibration with no valid candidate and the instance uncalibrated.
    The guide family sees 13k+ events at 1e5 and must NOT pay 6x for
    statistics it does not need."""
    sans = generate.family_protocol("sans_collimation")
    guide = generate.family_protocol("guide_divergence")
    assert sans["ncount"] > guide["ncount"]
    assert guide["ncount"] == generate.PROTOCOL["ncount"]
    assert sans["statistics_floor"] == generate.PROTOCOL["statistics_floor"]
    for fam, proto in (("sans_collimation", sans), ("guide_divergence", guide)):
        assert generate.instance(fam, "heldout", 0)["protocol"]["ncount"] == proto["ncount"]


def test_protocol_is_part_of_the_family_signature():
    """Calibration caches are keyed on the signature; changing how many rays
    an instance is scored with must invalidate them, or agents get compared
    against targets measured under a different protocol."""
    sig = generate.family_signature("sans_collimation")
    original = generate.FAMILIES["sans_collimation"].get("protocol")
    try:
        generate.FAMILIES["sans_collimation"]["protocol"] = {
            **(original or {}), "ncount": (original or generate.PROTOCOL)["ncount"] * 2}
        assert generate.family_signature("sans_collimation") != sig
    finally:
        if original is None:
            generate.FAMILIES["sans_collimation"].pop("protocol", None)
        else:
            generate.FAMILIES["sans_collimation"]["protocol"] = original
    assert generate.family_signature("sans_collimation") == sig


def test_baseline_below_the_statistics_floor_is_an_error():
    """A baseline that cannot clear L3's own floor cannot define a target:
    every candidate would be judged against a noise estimate."""
    floor = generate.PROTOCOL["statistics_floor"]

    class _Starved:
        def run(self, params, ncount, seed, **kw):
            return {"ok": True, "elapsed_s": 0.0, "summary": {"monitors": [
                {"component": "detector", "intensity": 1.0, "events": floor - 1}]}}

    inst = generate.instance("sans_collimation", "heldout", 0)
    out = reward.baseline(inst, _Starved())
    assert out["ok"] is False and "floor" in out["detail"]


def test_guide_divergence_spec_binds_on_every_instance():
    """It was inert on ~47% of instances: the implied coating limit sat at or
    above the top of the m_coat range, so it could not bind and the task
    reduced to "max out the coating" -- a universal answer (2026-09-16)."""
    for split in ("train", "heldout"):
        for i in range(N):
            inst = generate.instance("guide_divergence", split, i)
            lim = generate.guide_max_m(inst["context"])
            lo, hi = inst["free_parameters"]["m_coat"]
            assert lo < lim < hi, (split, i, lim)


def test_the_maximum_coating_is_never_a_legal_answer():
    """The n=150 probe's best fixed answer was exactly m_coat = 3.0."""
    for split in ("train", "heldout"):
        for i in range(0, N, 5):
            inst = generate.instance("guide_divergence", split, i)
            hi = inst["free_parameters"]["m_coat"][1]
            res = generate.check_guide_divergence_spec(inst["context"],
                                                       {"m_coat": hi})
            assert not res["pass"], (split, i)


def test_derived_div_max_stays_physical_and_instance_specific():
    vals = set()
    for i in range(N):
        inst = generate.instance("guide_divergence", "heldout", i)
        c = inst["context"]
        vals.add(c["div_max"])
        # div_max = m_limit * 0.099 * wl, with m_limit inside GUIDE_M_LIMIT_RANGE
        m_limit = c["div_max"] / (generate.GUIDE_THETA_C_DEG_PER_AA * c["wl"])
        lo, hi = generate.GUIDE_M_LIMIT_RANGE
        assert lo - 1e-6 <= m_limit <= hi + 1e-6, (i, m_limit)
        assert c["div_max"] > 0
    assert len(vals) > N * 0.9, "div_max must vary across instances"


def test_guide_baseline_survives_the_tightened_spec():
    base = generate.FAMILIES["guide_divergence"]["baseline"]
    for split in ("train", "heldout"):
        for i in range(0, N, 5):
            inst = generate.instance("guide_divergence", split, i)
            assert reward._check_l1(inst, dict(base))["pass"], (split, i)
