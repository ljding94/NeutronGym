"""Fourth archetype: bender (curved guide), 2026-09-20.

Its first prototype failed the lookup check — with a fixed incident spectrum
the design -> observable mapping carries no context, so another instance's
solution transferred on 7 of 10 instances. The spectrum now varies per
instance, and the bars are tight because the cutoff is a broad filter.
"""

import pytest

from neutrongym import generate, hacks, reward

FAM = "bender"


def test_registered_without_disturbing_existing_families():
    assert FAM in generate.FAMILIES
    for fam, sig in (("guide_divergence", "93617bc8ce"), ("sans_collimation", "8f8f7e3b09")):
        assert generate.family_signature(fam) == sig


def test_incident_spectrum_varies_per_instance():
    """The fix for the lookup failure: context must enter the optics."""
    lams = {generate.instance(FAM, "heldout", i)["context"]["lam0"] for i in range(40)}
    dlams = {generate.instance(FAM, "heldout", i)["context"]["dlam"] for i in range(40)}
    assert len(lams) > 35 and len(dlams) > 35


def test_bars_are_far_above_the_measurement_noise():
    specs = generate.FAMILIES[FAM]["fom"]["match"]
    tol = {s["observable"]: s["tolerance"] for s in specs}
    # measured seed-to-seed noise: 0.02% on the mean, 0.05% on the spread
    assert tol["center_of_mass"] == 0.0025 and tol["center_of_mass"] > 10 * 0.0002
    assert tol["beam_width_x"] == 0.01 and tol["beam_width_x"] > 10 * 0.0005


def test_scoring_applies_each_bar():
    inst = dict(generate.instance(FAM, "heldout", 0), targets=[5.0, 1.0])
    def summary(lam, spread):
        return {"monitors": [{"component": "lmon", "intensity": 1.0, "events": 9000,
                              "center_of_mass": lam, "beam_width": {"dX": spread}}]}
    levels = {}
    assert reward._score_match(inst, summary(5.01, 1.008), {"level": 3, "reward": 0.75},
                               levels)["level"] == 4          # 0.2% and 0.8%
    levels = {}
    reward._score_match(inst, summary(5.02, 1.0), {"level": 3, "reward": 0.75}, levels)
    assert levels["L4"]["pass"] is False                       # 0.4% on the mean
    levels = {}
    reward._score_match(inst, summary(5.0, 1.02), {"level": 3, "reward": 0.75}, levels)
    assert levels["L4"]["pass"] is False                       # 2% on the spread


def test_hidden_design_in_bounds_and_absent_from_the_prompt():
    inst = generate.instance(FAM, "heldout", 2)
    for k, (lo, hi) in inst["free_parameters"].items():
        assert lo <= inst["hidden_action"][k] <= hi
    p = generate.render_prompt(dict(inst, targets=[5.0, 1.0]))
    assert "+/-0.25%" in p and "+/-1%" in p
    for v in inst["hidden_action"].values():
        assert f"{v}" not in p


def test_spectrum_is_always_physical():
    """lam0 3.0 with an independent dlam 4.0 gives a negative wavelength and
    Source_simple refuses to run (2026-09-20)."""
    for split in ("train", "heldout"):
        for i in range(200):
            c = generate.instance(FAM, split, i)["context"]
            assert c["dlam"] < c["lam0"], (split, i, c)
            assert c["lam0"] - c["dlam"] > 0.5
            frac = c["dlam"] / c["lam0"]
            lo, hi = generate.BENDER_DLAM_FRACTION
            assert lo - 1e-9 <= frac <= hi + 1e-9


def test_bender_physics_rule_inverts_the_cutoff():
    """The rule solves lam_c = sqrt(2w/r) / (GAMMA * m) for r. Recomputing
    lam_c from the geometry it returns must give back the cutoff implied by
    the targets, or the rule is not the physics model it claims to be."""
    import math
    free = generate.FAMILIES["bender"]["free_parameters"]
    rules = hacks.readout_rules("bender", free)
    assert rules and all(lbl.startswith(hacks.PHYSICS_RULE_PREFIX) for lbl, _ in rules)

    gamma = 0.021 / (4 * math.pi)
    ctx = {"lam0": 5.0, "dlam": 2.0, "L_bend": 18.0}
    lam_lo, lam_hi = 3.0, 7.0
    lam_c_true = 4.0                      # transmitted band [4.0, 7.0]
    mean = (lam_c_true + lam_hi) / 2
    std = (lam_hi - lam_c_true) / math.sqrt(12)
    inst = {"targets": [mean, std]}

    checked = 0
    for label, fn in rules:
        a = fn(ctx, inst)
        if a is None:
            continue
        lam_c = math.sqrt(2 * a["w_ch"] / a["r_curve"]) / (gamma * a["m_coat"])
        assert abs(lam_c - lam_c_true) < 1e-3, (label, lam_c)
        checked += 1
    assert checked >= 10, f"only {checked} rules applied"
    # both readings of the band agree when the targets are self-consistent
    assert lam_lo < lam_c_true < lam_hi


def test_bender_physics_rule_clamps_to_the_incident_band():
    """A target mean implying a cutoff below the incident band means "no
    filtering", not a negative radius: lam_c floors at lam_lo."""
    import math
    free = generate.FAMILIES["bender"]["free_parameters"]
    rules = hacks.readout_rules("bender", free)
    ctx = {"lam0": 5.0, "dlam": 2.0, "L_bend": 18.0}
    gamma = 0.021 / (4 * math.pi)
    # mean of the full band -> implied cutoff 3.0 = lam_lo exactly
    inst = {"targets": [5.0, 4.0 / math.sqrt(12)]}
    for label, fn in rules:
        a = fn(ctx, inst)
        if a is None:
            continue
        lam_c = math.sqrt(2 * a["w_ch"] / a["r_curve"]) / (gamma * a["m_coat"])
        assert lam_c >= 3.0 - 1e-6, (label, lam_c)


def test_bender_physics_rules_are_reference_only_not_gated():
    """Physics-model rules are reported, never used to fail the family --
    only copy-type rules are gated at 20%."""
    free = generate.FAMILIES["bender"]["free_parameters"]
    rules = hacks.readout_rules("bender", free)
    probe = {"results": [{"action": lbl, "pass_rate": 0.9} for lbl, _ in rules]}
    copy_type, physics = hacks.split_rule_results(probe)
    assert not copy_type["results"], "bender must expose no copy-type rule"
    assert len(physics["results"]) == len(rules)


def test_hidden_designs_must_actually_filter():
    """Drawn uniformly from the box, a quarter of held-out instances got a
    hidden cutoff BELOW the incident band: the bender did nothing and the
    targets were the source spectrum's own mean and spread, so any
    transparent design solved them. The trained policy exploited exactly
    that -- one geometry passed 17 instances spanning 4.2-8.2 AA of target
    mean (2026-09-21)."""
    lo, hi = generate.BENDER_BAND_CUT
    checked = 0
    for split in ("train", "heldout"):
        for i in range(0, 60):
            inst = generate.instance("bender", split, i)
            cands = generate.match_hidden_candidates(inst)
            assert cands, (split, i)
            for a in cands:
                cut = generate.bender_band_cut(inst["context"], a)
                assert lo <= cut <= hi, (split, i, a, cut)
                checked += 1
    assert checked > 500, checked


def test_hidden_candidate_list_keeps_its_length():
    """The filter must not shorten the sequence -- calibration walks it for a
    design that is runnable and not already matched by the baseline."""
    for i in range(0, 30):
        inst = generate.instance("bender", "heldout", i)
        assert len(generate.match_hidden_candidates(inst)) == generate.MATCH_HIDDEN_CANDIDATES


def test_families_without_a_filter_are_unaffected():
    for fam in ("guide_match", "sans_match", "tof_chopper"):
        inst = generate.instance(fam, "heldout", 3)
        cands = generate.match_hidden_candidates(inst)
        assert len(cands) == generate.MATCH_HIDDEN_CANDIDATES
        assert cands[0] == inst["hidden_action"], fam


def test_band_cut_is_zero_below_the_band_and_one_above():
    ctx = {"lam0": 5.0, "dlam": 2.0}          # band [3, 7]
    import math
    gamma = generate.BENDER_GAMMA

    def geom_for(lam_c, w=0.03, m=2.5):
        return {"w_ch": w, "m_coat": m,
                "r_curve": 2.0 * w / (gamma * m * lam_c) ** 2}

    assert generate.bender_band_cut(ctx, geom_for(2.0)) == 0.0
    assert generate.bender_band_cut(ctx, geom_for(8.0)) == 1.0
    assert abs(generate.bender_band_cut(ctx, geom_for(5.0)) - 0.5) < 1e-9
