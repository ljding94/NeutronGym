"""Fourth archetype: bender (curved guide), 2026-09-20.

Its first prototype failed the lookup check — with a fixed incident spectrum
the design -> observable mapping carries no context, so another instance's
solution transferred on 7 of 10 instances. The spectrum now varies per
instance, and the bars are tight because the cutoff is a broad filter.
"""

import pytest

from neutrongym import generate, reward

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
