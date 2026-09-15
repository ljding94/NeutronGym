"""Constant-policy degeneracy gate (red-team findings 6 and 7).

A family whose held-out instances are passed by one fixed configuration,
with no model, cannot tell design skill from a lookup. The fast tests pin the
gate's logic; the slow tests pin the two real findings it exists for.
"""

import pytest

from neutrongym import generate, hacks

GUIDE_DEGENERATE = {"w_in": 0.05, "w_out": 0.03, "m_coat": 2.5}


def test_grid_contains_the_known_guide_degeneracy():
    """A 3-level grid misses (0.05, 0.03, 2.5); the gate must not."""
    inst = generate.instance("guide_divergence", "heldout", 0)
    cands = hacks.constant_candidates(inst)
    assert GUIDE_DEGENERATE in cands
    assert GUIDE_DEGENERATE not in hacks.constant_candidates(inst, levels=3)


def test_grid_includes_baseline_corners_midpoint_and_stays_in_bounds():
    inst = generate.instance("guide_divergence", "heldout", 0)
    cands = hacks.constant_candidates(inst)
    free = inst["free_parameters"]
    assert cands[0] == inst["baseline"]
    assert {k: hi for k, (lo, hi) in free.items()} in cands
    assert {k: lo for k, (lo, hi) in free.items()} in cands
    assert {k: round((lo + hi) / 2, 6) for k, (lo, hi) in free.items()} in cands
    for c in cands:
        assert set(c) == set(free)
        assert all(free[k][0] <= v <= free[k][1] for k, v in c.items())


def test_grid_is_deduplicated_and_sized_by_levels():
    inst = generate.instance("sans_collimation", "heldout", 0)
    cands = hacks.constant_candidates(inst)
    keys = [tuple(sorted(c.items())) for c in cands]
    assert len(keys) == len(set(keys))
    # 5 x 5 grid, plus the baseline unless it already sits on the grid
    assert len(cands) in (25, 26)


class _StubEnv:
    """Passes a candidate iff its first parameter is above a threshold."""

    def __init__(self):
        self.resets = []

    def reset(self, index):
        self.resets.append(index)
        return {}, {}

    def step(self, action):
        level = 4 if action["a"] > 0.5 else 3
        return {}, 0.0, False, False, {"level": level}


def test_probe_scores_every_candidate_on_every_instance_once():
    env = _StubEnv()
    cands = [{"a": 0.9}, {"a": 0.1}]
    probe = hacks.constant_policy_probe(env, range(4), cands)
    assert env.resets == [0, 1, 2, 3]
    assert probe["n_instances"] == 4
    assert [r["pass_rate"] for r in probe["results"]] == [1.0, 0.0]


def test_gate_fails_when_any_constant_clears_the_ceiling():
    probe = {"n_instances": 100, "results": [
        {"action": {"a": 1}, "passes": 50, "pass_rate": 0.50},
        {"action": {"a": 2}, "passes": 5, "pass_rate": 0.05}]}
    v = hacks.summarize_constant_probe(probe)
    assert v["ok"] is False
    assert v["best_action"] == {"a": 1} and v["best_pass_rate"] == 0.50


def test_gate_passes_at_exactly_the_ceiling_and_below():
    at = {"n_instances": 100, "results": [
        {"action": {"a": 1}, "passes": 20, "pass_rate": 0.20}]}
    assert hacks.summarize_constant_probe(at)["ok"] is True
    below = {"n_instances": 100, "results": [
        {"action": {"a": 1}, "passes": 3, "pass_rate": 0.03}]}
    assert hacks.summarize_constant_probe(below)["ok"] is True


def _real_env(family):
    from neutrongym.env import NeutronGym
    return NeutronGym(family=family, split="heldout", target_fraction=1.0,
                      max_steps=10**6)


@pytest.mark.slow
def test_guide_family_is_degenerate_at_the_1x_bar():
    """Documents red-team finding 7. If this starts failing, the guide
    family was hardened: update finding 7 and the M8 write-up, then delete
    this test rather than weakening it."""
    probe = hacks.constant_policy_probe(_real_env("guide_divergence"),
                                        range(10), [GUIDE_DEGENERATE])
    assert probe["results"][0]["pass_rate"] >= hacks.CONSTANT_MAX_PASS_RATE


@pytest.mark.slow
def test_sans_all_max_no_longer_passes_after_the_direct_beam_fix():
    """Documents that red-team finding 6 stays fixed."""
    inst = generate.instance("sans_collimation", "heldout", 0)
    all_max = {k: hi for k, (lo, hi) in inst["free_parameters"].items()}
    probe = hacks.constant_policy_probe(_real_env("sans_collimation"),
                                        range(5), [all_max])
    assert probe["results"][0]["pass_rate"] == 0.0
