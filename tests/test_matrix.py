"""M6 matrix-runner regressions: enumeration discipline (held-out never
reachable, reserves excluded, slices honored), cost from actual usage."""

import json
import os
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))

import run_matrix  # noqa: E402


CFG = run_matrix.load_config()


def test_heldout_tasks_structurally_unreachable():
    eps = run_matrix.enumerate_episodes(CFG)
    tasks = {t for _, _, _, t, _ in eps}
    assert "T1_BOYA_CARR" not in tasks and "T1_VENUS_SNS" not in tasks
    assert len(eps) > 100  # the matrix is real


def test_reserve_model_never_enters_matrix():
    eps = run_matrix.enumerate_episodes(CFG)
    assert all(m != "anthropic/claude-opus-5" for _, _, m, _, _ in eps)


def test_gpt_slice_honored():
    eps = run_matrix.enumerate_episodes(CFG)
    gpt_tasks = {t for _, _, m, t, _ in eps if m == "openai/gpt-5.2-pro"}
    assert gpt_tasks <= set(CFG["gpt_slice"]) | set(CFG["dev_split"])
    assert len(gpt_tasks) <= 8


def test_priority_and_model_filters():
    p2 = run_matrix.enumerate_episodes(CFG, only_priority=2)
    assert p2 and all(p == 2 for p, *_ in p2)
    assert any(m is None for _, _, m, _, _ in p2)  # subscription arm
    one = run_matrix.enumerate_episodes(
        CFG, only_model="meta-llama/llama-4-maverick")
    assert one and all(m == "meta-llama/llama-4-maverick"
                       for _, _, m, _, _ in one)


def test_episode_cost_from_actual_usage(tmp_path):
    rp = tmp_path / "report.json"
    rp.write_text(json.dumps({"episode": {"usage": {
        "prompt_tokens": 1_000_000, "completion_tokens": 100_000}}}))
    cost = run_matrix.episode_cost(str(rp), CFG, "anthropic/claude-sonnet-5")
    assert abs(cost - (2.0 + 1.0)) < 1e-9  # 1M*$2/M + 0.1M*$10/M
    assert run_matrix.episode_cost(str(rp), CFG, None) == 0.0
