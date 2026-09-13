"""SANS direct-beam geometry and the probe's leak classification.

These decide whether a SANS pass is counted as skill or as leakage, so they
are pinned against the numbers that exposed the hole: the constant all-max
policy (both pinholes 0.02 m) leaks on every held-out instance, and pinholes
small enough to keep the direct beam inside the 0.02 m stop do not.
"""

import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))), "benchmark", "harness"))

from neutrongym import generate  # noqa: E402
import m8_family_probe as probe  # noqa: E402

FAM = "sans_collimation"


def test_all_max_pinholes_leak_on_every_heldout_instance():
    act = {k: hi for k, (lo, hi) in generate.FAMILIES[FAM]["free_parameters"].items()}
    for i in range(25):
        ctx = generate.instance(FAM, "heldout", i)["context"]
        assert generate.sans_direct_beam_leaks(ctx, act), i


def test_small_pinholes_stay_inside_the_stop():
    act = {"r_pin1": 0.002, "r_pin2": 0.002}
    for i in range(25):
        ctx = generate.instance(FAM, "heldout", i)["context"]
        assert not generate.sans_direct_beam_leaks(ctx, act), i


def test_radius_hand_value():
    # r1 capped at the focus half-diagonal 0.005*sqrt(2); d = 0.2 + 3.0 - 0.1
    ctx = {"det_dist": 3.0, "L_coll": 5.0}
    r = generate.sans_direct_beam_radius(ctx, {"r_pin1": 0.02, "r_pin2": 0.01})
    r1 = 0.005 * 2 ** 0.5
    assert abs(r - (0.01 + (r1 + 0.01) * 3.1 / 5.0)) < 1e-12


def test_radius_grows_with_either_pinhole():
    ctx = {"det_dist": 3.0, "L_coll": 5.0}
    base = generate.sans_direct_beam_radius(ctx, {"r_pin1": 0.003, "r_pin2": 0.003})
    assert generate.sans_direct_beam_radius(ctx, {"r_pin1": 0.006, "r_pin2": 0.003}) > base
    assert generate.sans_direct_beam_radius(ctx, {"r_pin1": 0.003, "r_pin2": 0.006}) > base


def test_classify_marks_leaking_passes_and_ignores_guide():
    rows = [
        {"instance": 0, "best_level": 4, "pass_action": {"r_pin1": 0.02, "r_pin2": 0.02}},
        {"instance": 1, "best_level": 4, "pass_action": {"r_pin1": 0.002, "r_pin2": 0.002}},
        {"instance": 2, "best_level": 3, "pass_action": None},
    ]
    out = probe.classify_rows(FAM, "heldout", rows)
    assert [r["leak"] for r in out] == [True, False, None]
    assert [r["clean_pass"] for r in out] == [False, True, False]
    guide = probe.classify_rows("guide_divergence", "heldout",
                                [{"instance": 0, "best_level": 4,
                                  "pass_action": {"w_in": 0.09}}])
    assert guide[0]["leak"] is None and guide[0]["clean_pass"] is True


def test_clean_view_demotes_only_leaking_passes():
    rows = [{"instance": 0, "best_level": 4, "leak": True},
            {"instance": 1, "best_level": 4, "leak": False},
            {"instance": 2, "best_level": 2, "leak": None}]
    assert [r["best_level"] for r in probe.as_clean_levels(rows)] == [3, 4, 2]
