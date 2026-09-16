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
    # one discovery pass, then one scoring pass (no bounds -> no refinement)
    assert env.resets == [0, 1, 2, 3, 0, 1, 2, 3]
    assert probe["n_instances"] == 4
    assert [r["pass_rate"] for r in probe["results"]] == [1.0, 0.0]


def test_gate_fails_when_any_constant_clears_the_ceiling():
    probe = {"n_instances": 100, "results": [
        {"action": {"a": 1}, "passes": 50, "pass_rate": 0.50},
        {"action": {"a": 2}, "passes": 5, "pass_rate": 0.05}]}
    v = hacks.summarize_constant_probe(probe)
    assert v["ok"] is False
    assert v["best_action"] == {"a": 1} and v["best_pass_rate"] == 0.50


def test_gate_will_not_certify_a_rate_sitting_on_the_ceiling():
    """20/100 is AT the ceiling, but its 95% upper limit is 28% — the sample
    cannot tell a clean family from one half again over the limit. The guide
    was certified on exactly this mistake at 5/25 (2026-09-15)."""
    at = {"n_instances": 100, "results": [
        {"action": {"a": 1}, "passes": 20, "pass_rate": 0.20}]}
    v = hacks.summarize_constant_probe(at)
    assert v["ok"] is False
    assert v["underpowered"] is True        # not the same as the family failing
    assert v["best_pass_rate_upper"] > 0.20


def test_gate_passes_when_the_upper_limit_clears_the_ceiling():
    below = {"n_instances": 100, "results": [
        {"action": {"a": 1}, "passes": 3, "pass_rate": 0.03}]}
    v = hacks.summarize_constant_probe(below)
    assert v["ok"] is True and v["underpowered"] is False
    assert v["best_pass_rate_upper"] < 0.20


def test_a_family_over_the_ceiling_is_a_failure_not_an_underpowered_run():
    over = {"n_instances": 150, "results": [
        {"action": {"a": 1}, "passes": 60, "pass_rate": 0.40}]}
    v = hacks.summarize_constant_probe(over)
    assert v["ok"] is False and v["underpowered"] is False


def test_binomial_upper_bound_hand_values():
    # k=0, n=10: exact Clopper-Pearson limit is 1 - 0.05 ** (1/10)
    assert abs(hacks.binomial_upper_bound(0, 10) - (1 - 0.05 ** 0.1)) < 1e-6
    # more evidence at the same rate tightens the limit
    assert (hacks.binomial_upper_bound(18, 150)
            < hacks.binomial_upper_bound(3, 25))
    assert hacks.binomial_upper_bound(0, 0) == 1.0     # no evidence at all
    assert hacks.binomial_upper_bound(10, 10) == 1.0


def _real_env(family):
    from neutrongym.env import NeutronGym
    return NeutronGym(family=family, split="heldout", target_fraction=1.0,
                      max_steps=10**6)


# test_guide_family_is_degenerate_at_the_1x_bar was deleted 2026-09-15, as its
# docstring instructed once it failed. Under calibration v2 (strong optimum,
# same seed as agents) no fixed guide answer strictly beats an instance's
# optimum at 1.0x, so the v1 "~50% at 1.0x" finding was mostly seed noise.
# Degeneracy below 1.0x is measured by benchmark/harness/family_diagnostic.py.


@pytest.mark.slow
def test_sans_all_max_no_longer_passes_after_the_direct_beam_fix():
    """Documents that red-team finding 6 stays fixed."""
    inst = generate.instance("sans_collimation", "heldout", 0)
    all_max = {k: hi for k, (lo, hi) in inst["free_parameters"].items()}
    probe = hacks.constant_policy_probe(_real_env("sans_collimation"),
                                        range(5), [all_max],
                                        classical=False, refine_rounds=0)
    assert probe["results"][0]["pass_rate"] == 0.0



# --- gate v2 (2026-09-15): classical-optima candidates, refinement, baseline --

FREE = {"r_pin1": (0.001, 0.02), "r_pin2": (0.001, 0.02)}
BASE = {"r_pin1": 0.005, "r_pin2": 0.005}


class _SansLikeEnv:
    """Instance i passes an action iff both radii are within `tol` of c_i."""

    def __init__(self, centers, tol=0.0015, classical=None, baseline_passes_on=(),
                 no_headroom=()):
        self.centers, self.tol = centers, tol
        self.classical = classical or {}
        self.baseline_passes_on = set(baseline_passes_on)
        self.no_headroom = set(no_headroom)

    def reset(self, index):
        self.i = index
        inst = {"free_parameters": FREE, "baseline": dict(BASE),
                "no_headroom": index in self.no_headroom}
        if index in self.classical:
            inst["classical_action"] = self.classical[index]
        return {"instance": inst}, {}

    def step(self, action):
        c = self.centers[self.i]
        hit = all(abs(action[k] - c) <= self.tol for k in action)
        if action == BASE and self.i in self.baseline_passes_on:
            hit = True
        return {}, 0.0, False, False, {"level": 4 if hit else 3}


def _grid():
    return hacks.constant_candidates({"free_parameters": FREE, "baseline": BASE})


def test_grid_alone_misses_a_sharp_sweet_spot_between_grid_points():
    env = _SansLikeEnv([0.008] * 10)
    v = hacks.summarize_constant_probe(
        hacks.constant_policy_probe(env, range(10), _grid(), classical=False, refine_rounds=0))
    assert v["best_pass_rate"] == 0.0
    # the grid sees nothing — but 10 instances cannot certify a 20% ceiling
    # even at zero observed passes (upper limit 25.9%), so the verdict is
    # "not enough evidence", not "clean". This is the old false pass.
    assert v["ok"] is False and v["underpowered"] is True


def test_classical_optima_expose_the_sweet_spot():
    env = _SansLikeEnv([0.008] * 10, classical={0: {"r_pin1": 0.0079, "r_pin2": 0.0083}})
    v = hacks.summarize_constant_probe(hacks.constant_policy_probe(env, range(10), _grid()))
    assert v["best_pass_rate"] == 1.0
    assert v["ok"] is False and v["best_source"] in ("classical", "refined")


def test_refinement_improves_on_the_best_candidate_with_diagonal_moves():
    centers = [0.007 + 0.0002 * i for i in range(10)]
    env = _SansLikeEnv(centers, tol=0.002)
    grid_only = hacks.constant_policy_probe(env, range(10), _grid(),
                                            classical=False, refine_rounds=0)
    refined = hacks.constant_policy_probe(env, range(10), _grid(), classical=False)
    g = hacks.summarize_constant_probe(grid_only)
    r = hacks.summarize_constant_probe(refined)
    assert r["best_pass_rate"] > g["best_pass_rate"]
    assert r["best_source"] == "refined"


def test_a_passing_baseline_fails_the_gate_even_below_the_rate_ceiling():
    # center 0.0128 sits clear of every grid radius (0.0105, 0.01525)
    env = _SansLikeEnv([0.0128] * 10, baseline_passes_on={0})
    v = hacks.summarize_constant_probe(
        hacks.constant_policy_probe(env, range(10), _grid(), classical=False, refine_rounds=0))
    assert v["best_pass_rate"] <= hacks.CONSTANT_MAX_PASS_RATE
    assert v["baseline_passes"] == 1 and v["ok"] is False


def test_no_headroom_instances_are_skipped_by_the_probe():
    env = _SansLikeEnv([0.019] * 10, no_headroom={0, 1})
    probe = hacks.constant_policy_probe(env, range(10), _grid(), classical=False, refine_rounds=0)
    assert probe["n_instances"] == 8 and probe["skipped_no_headroom"] == 2


def test_neighbourhood_includes_diagonals_and_respects_bounds():
    n = hacks._neighbourhood({"r_pin1": 0.02, "r_pin2": 0.01}, FREE, 1 / 8)
    assert {"r_pin1": 0.017625, "r_pin2": 0.012375} in n     # diagonal move
    assert all(FREE[k][0] <= v <= FREE[k][1] for a in n for k, v in a.items())
    assert {"r_pin1": 0.02, "r_pin2": 0.01} not in n


def test_gate_fraction_must_not_be_vacuous():
    """Since calibration v2 a 1.0x bar means beating each instance's own
    optimum, which nothing fixed does — probing there would clear any
    family (2026-09-15)."""
    import pytest
    assert hacks.check_gate_fraction(0.9) == 0.9
    assert hacks.check_gate_fraction(hacks.CONSTANT_GATE_MAX_FRACTION)
    for bad in (1.0, 1.2, 0.0, -0.5):
        with pytest.raises(ValueError, match="vacuous"):
            hacks.check_gate_fraction(bad)
