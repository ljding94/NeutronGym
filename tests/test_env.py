"""End-to-end NeutronGym env regressions (slow: real compiles + rollouts).
The fast logic surfaces live in test_generate/test_reward/test_executor;
this file proves the assembled loop against real physics."""

import pytest

from neutrongym.env import NeutronGym


@pytest.fixture(scope="module")
def env(tmp_path_factory):
    return NeutronGym(family="guide_divergence", split="train",
                      workdir=str(tmp_path_factory.mktemp("gym")),
                      max_steps=3)


@pytest.mark.slow
def test_reset_yields_contract_and_baseline(env):
    obs, info = env.reset(index=0)
    assert info["instance_id"] == "guide_divergence-train-000000"
    assert obs["baseline_fom"] > 0
    assert "w_in" in obs["prompt"] and "env-controlled" in obs["prompt"]


@pytest.mark.slow
def test_step_improvement_reaches_l4(env):
    obs, _ = env.reset(index=0)
    # the calibrated classical optimum meets the instance's specification and
    # clears the default 0.8x bar by construction (a hardcoded "improvement"
    # broke when the 2026-09-15 specifications were added)
    obs, r, term, trunc, rec = env.step(dict(obs["instance"]["classical_action"]))
    assert rec["level"] == 4 and term and not trunc
    assert r > 1.0
    assert obs["feedback"]["fom_ratio"] > 1


@pytest.mark.slow
def test_step_baseline_is_level_3_not_4(env):
    """Resubmitting the baseline is valid but not an improvement.

    Since calibration (2026-09-13) fom_ratio is measured against the
    CALIBRATED TARGET, not the baseline: ratio = fom / (baseline_fom *
    target_ratio), so the baseline scores exactly 1/target_ratio. Asserting
    that exact value — rather than the old 1.0, which silently encoded
    target_ratio == 1 — is what makes this test notice if calibration ever
    stops being applied.
    """
    obs, _ = env.reset(index=0)
    inst = obs["instance"]
    tr = inst["target_ratio"]
    assert inst["target_calibrated"] and tr > 1.0, (
        "the env must hold the baseline to a classical-derived target; "
        f"target_ratio={tr} means the L4 bar is back to 'beat the baseline'")
    _, r, term, _, rec = env.step(dict(inst["baseline"]))
    assert rec["level"] == 3 and not term
    assert rec["levels"]["L4"]["fom_ratio"] == pytest.approx(1 / tr, rel=1e-3)
    assert r == pytest.approx(0.75 + 0.25 / tr, rel=1e-3)


@pytest.mark.slow
def test_truncation_and_episode_record(env):
    obs, _ = env.reset(index=1)
    bad = {"w_in": 0.5, "w_out": 0.02, "m_coat": 2.0}  # L1 fail, free
    for i in range(3):
        obs, r, term, trunc, rec = env.step(bad)
    assert trunc and not term
    ep = env.episode_record()
    assert len(ep["steps"]) == 3
    assert all(s["level"] == 0 for s in ep["steps"])
    assert ep["instance"]["id"] == "guide_divergence-train-000001"


@pytest.mark.slow
def test_baseline_cached_across_resets(env, monkeypatch):
    env.reset(index=0)
    from neutrongym import reward

    def explode(*a, **k):
        raise AssertionError("baseline recomputed despite cache")

    monkeypatch.setattr(reward, "baseline", explode)
    env.reset(index=0)  # same instance -> cached baseline, no recompute


@pytest.mark.slow
def test_second_family_compiles_and_steps(tmp_path):
    env = NeutronGym(family="sans_collimation", split="heldout",
                     workdir=str(tmp_path))
    obs, info = env.reset(index=0)
    assert info["split"] == "heldout"
    _, r, _, _, rec = env.step({"r_pin1": 0.008, "r_pin2": 0.004})
    assert rec["level"] >= 2, rec  # ran for real; physics outcome may vary
    assert rec["elapsed_s"] < 5
