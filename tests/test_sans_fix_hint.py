"""Numeric repair hints for rejected SANS designs (2026-09-16).

RAFT collection kept 2/300 episodes: the untrained 8B opened with a
near-maximum r_pin2 and, told only to "narrow the pinholes", was rejected at
L0 on all 6 turns. The hint states the exact limit, so it must be exact:
following it has to produce a valid design, and exceeding it must not.
"""

import re

from neutrongym import generate, reward

FAM = "sans_collimation"
N = 120


def _valid(inst, a):
    return reward._check_l1(inst, a)["pass"]


def test_following_the_r_pin2_limit_gives_a_valid_design_and_exceeding_it_does_not():
    checked = 0
    for split in ("train", "heldout"):
        for i in range(N):
            inst = generate.instance(FAM, split, i)
            c = inst["context"]
            for r1 in (0.002, 0.005, 0.01):
                m2 = generate.sans_max_r2(c, r1)
                if m2 < 0.0011:
                    continue
                assert _valid(inst, {"r_pin1": r1, "r_pin2": m2 * (1 - 1e-6)}), (split, i, r1)
                assert not _valid(inst, {"r_pin1": r1, "r_pin2": m2 * 1.01}), (split, i, r1)
                checked += 1
    assert checked > 200


def test_rejection_detail_carries_a_hint_whose_number_works():
    inst = generate.instance(FAM, "train", 0)
    bad = {"r_pin1": 0.005, "r_pin2": 0.015}          # traced model move
    res = reward._check_l1(inst, bad)
    assert not res["pass"] and "Hint:" in res["detail"]
    m = re.search(r"r_pin2 must be <= ([0-9.]+) m", res["detail"])
    assert m, res["detail"]
    fixed = {"r_pin1": 0.005, "r_pin2": float(m.group(1)) - 1e-4}
    assert _valid(inst, fixed), res["detail"]


def test_hint_says_to_shrink_r_pin1_when_no_r_pin2_fits():
    inst = generate.instance(FAM, "train", 0)
    c = inst["context"]
    r1 = 0.02
    assert generate.sans_max_r2(c, r1) < 0.001
    hint = generate.sans_fix_hint(c, {"r_pin1": r1, "r_pin2": 0.01})
    m = re.search(r"r_pin1 must be <= ([0-9.]+) m", hint)
    assert m and "no r_pin2" in hint
    assert _valid(inst, {"r_pin1": float(m.group(1)) - 1e-4, "r_pin2": 0.001})


def test_guide_feedback_is_untouched_and_grading_is_unchanged():
    g = generate.instance("guide_divergence", "heldout", 0)
    res = reward._check_l1(g, {"w_in": 0.05, "w_out": 0.09, "m_coat": 1.0})
    assert "Hint:" not in res.get("detail", "")
    # the signature (and so every calibration cache) must not move
    assert generate.family_signature(FAM) == "8f8f7e3b09"
