"""Third archetype: tof_chopper, the first family in the time domain.

Two prototype attempts failed first: at 5e5 rays every design sat at the
500-event floor, and above ~60 Hz the selected wavelength leaves the source
band so most designs give no counts. Both are pinned here.
"""

import pytest

from neutrongym import generate, hacks, reward

FAM = "tof_chopper"


def test_registered_without_disturbing_existing_families():
    assert FAM in generate.FAMILIES
    assert generate.family_signature("guide_divergence") == "93617bc8ce"
    assert generate.family_signature("sans_collimation") == "8f8f7e3b09"


def test_protocol_and_frequency_range_avoid_the_two_prototype_failures():
    f = generate.FAMILIES[FAM]
    assert generate.family_protocol(FAM)["ncount"] == 5e6      # 5e5 starved it
    lo, hi = f["free_parameters"]["nu"]
    assert (lo, hi) == (20.0, 60.0)                            # above ~60 Hz: no counts
    for split in ("train", "heldout"):
        for i in range(40):
            a = generate.instance(FAM, split, i)["hidden_action"]
            for k, (klo, khi) in f["free_parameters"].items():
                assert klo <= a[k] <= khi


def test_per_observable_tolerances_are_scored_separately():
    inst = dict(generate.instance(FAM, "heldout", 0), targets=[5.0, 0.20])
    def summary(lam, spread):
        return {"monitors": [{"component": "lmon", "intensity": 1.0, "events": 9000,
                              "center_of_mass": lam, "beam_width": {"dX": spread}}]}
    # wavelength bar is 2%, spread bar is 5%
    levels = {}
    rec = reward._score_match(inst, summary(5.09, 0.208), {"level": 3, "reward": 0.75}, levels)
    assert rec["level"] == 4                       # 1.8% and 4% -> both inside
    assert rec["match"]["tolerances"] == [0.02, 0.05]
    levels = {}
    reward._score_match(inst, summary(5.15, 0.20), {"level": 3, "reward": 0.75}, levels)
    assert levels["L4"]["pass"] is False           # 3% on wavelength alone fails
    levels = {}
    reward._score_match(inst, summary(5.0, 0.22), {"level": 3, "reward": 0.75}, levels)
    assert levels["L4"]["pass"] is False           # 10% on spread alone fails


def test_prompt_states_each_bar_not_one_shared_number():
    inst = dict(generate.instance(FAM, "heldout", 0), targets=[5.0, 0.2])
    p = generate.render_prompt(inst)
    assert "+/-2%" in p and "+/-5%" in p
    for v in inst["hidden_action"].values():
        assert f"{v}" not in p


def test_physics_rule_inverts_the_flight_time_relation():
    rules = dict(hacks.readout_rules(FAM, generate.FAMILIES[FAM]["free_parameters"]))
    assert rules and all(k.startswith(hacks.PHYSICS_RULE_PREFIX) for k in rules)
    inst = generate.instance(FAM, "heldout", 0)
    c = inst["context"]
    nu, lam = 30.0, 5.0
    dt = lam * c["L_ch"] / 3956.0
    targets = [lam, lam * (5.0 + 8.0) / (360.0 * nu * dt)]     # theta2 = 8 deg
    fn = [f for k, f in rules.items() if k.endswith("nu=30.0")][0]
    got = fn(c, dict(inst, targets=targets))
    assert got["phase"] == pytest.approx(360.0 * nu * dt, rel=1e-6)
    assert got["theta2"] == pytest.approx(8.0, abs=1e-3)


def test_rules_decline_when_the_solution_is_out_of_bounds():
    rules = dict(hacks.readout_rules(FAM, generate.FAMILIES[FAM]["free_parameters"]))
    fn = list(rules.values())[0]
    inst = generate.instance(FAM, "heldout", 0)
    assert fn(inst["context"], dict(inst, targets=[50.0, 0.1])) is None
    assert fn(inst["context"], inst) is None
