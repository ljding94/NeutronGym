"""Second matching family sans_match (2026-09-18).

Built to test whether the GRPO recipe generalises past one family. Targets are
two geometric widths (beam at the sample, unscattered beam at the beamstop
plane) because the first SANS attempt matched scattered intensity, whose
seed-to-seed noise is 4-11% against 0.7% for widths.
"""

import pytest

from neutrongym import generate, hacks, reward

FAM = "sans_match"


def test_family_is_registered_without_disturbing_the_others():
    assert FAM in generate.FAMILIES
    assert generate.family_signature("sans_collimation") == "8f8f7e3b09"
    assert generate.family_signature("guide_divergence") == "93617bc8ce"


def test_tolerance_is_tighter_than_guide_match_and_protocol_is_cheap():
    f = generate.FAMILIES[FAM]
    assert f["fom"]["tolerance"] == 0.015 < generate.MATCH_TOLERANCE
    # no sample scattering in this instrument, so rays are cheap
    assert generate.family_protocol(FAM)["ncount"] == 2e6
    assert [m["monitor"] for m in f["fom"]["match"]] == ["at_sample", "at_stop"]


def test_hidden_design_is_in_bounds_deterministic_and_unseen():
    a = generate.instance(FAM, "heldout", 4)
    assert a["hidden_action"] == generate.instance(FAM, "heldout", 4)["hidden_action"]
    for k, (lo, hi) in a["free_parameters"].items():
        assert lo <= a["hidden_action"][k] <= hi
    p = generate.render_prompt(dict(a, targets=[1.0, 2.0]))
    assert "TARGETS" in p and "+/-1.5%" in p
    for v in a["hidden_action"].values():
        assert f"{v}" not in p


def test_physics_rules_invert_the_two_width_equations():
    rules = hacks.readout_rules(FAM, generate.FAMILIES[FAM]["free_parameters"])
    assert rules and all(l.startswith(hacks.PHYSICS_RULE_PREFIX) for l, _ in rules)
    inst = generate.instance(FAM, "heldout", 0)
    c = inst["context"]
    # forward-compute the widths a known pair would produce, then invert them
    k, L = 0.5, float(c["L_coll"])
    a_s = generate.SANS_COLL2_TO_SAMPLE / L
    a_t = (generate.SANS_COLL2_TO_SAMPLE + float(c["det_dist"])
           - generate.SANS_STOP_BEFORE_DETECTOR) / L
    r1, r2 = 0.006, 0.004
    targets = [100 * k * (r2 + (r1 + r2) * a_s), 100 * k * (r2 + (r1 + r2) * a_t)]
    fn = dict(rules)[f"physics: width inversion k={k}"]
    got = fn(c, dict(inst, targets=targets))
    assert got["r_pin1"] == pytest.approx(r1, abs=1e-5)
    assert got["r_pin2"] == pytest.approx(r2, abs=1e-5)


def test_rules_decline_when_the_solution_leaves_the_bounds():
    rules = dict(hacks.readout_rules(FAM, generate.FAMILIES[FAM]["free_parameters"]))
    inst = generate.instance(FAM, "heldout", 0)
    fn = rules["physics: width inversion k=0.5"]
    assert fn(inst["context"], dict(inst, targets=[99.0, 99.0])) is None
    assert fn(inst["context"], inst) is None          # no targets yet


def test_match_scoring_uses_the_families_own_tolerance():
    inst = dict(generate.instance(FAM, "heldout", 0), targets=[1.0, 2.0])
    summary = {"monitors": [
        {"component": "at_sample", "intensity": 1.0, "events": 5000, "beam_width": {"dX": 1.01}},
        {"component": "at_stop", "intensity": 1.0, "events": 5000, "beam_width": {"dX": 2.0}}]}
    levels = {}
    rec = reward._score_match(inst, summary, {"level": 3, "reward": 0.75}, levels)
    assert rec["level"] == 4                       # 1% inside the 1.5% tolerance
    summary["monitors"][0]["beam_width"]["dX"] = 1.02
    levels = {}
    reward._score_match(inst, summary, {"level": 3, "reward": 0.75}, levels)
    assert levels["L4"]["pass"] is False            # 2% outside it
