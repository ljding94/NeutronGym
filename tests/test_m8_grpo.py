"""Pure pieces of the step-level GRPO trainer and state collector."""

import json
import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))), "benchmark", "harness"))

import m8_grpo  # noqa: E402
import m8_states  # noqa: E402

EP = {"family": "guide_divergence", "index": 7, "messages": [
    {"role": "system", "content": "s"}, {"role": "user", "content": "task"},
    {"role": "assistant", "content": "a1"}, {"role": "user", "content": "fb1"},
    {"role": "assistant", "content": "a2"}, {"role": "user", "content": "fb2"}]}


def test_every_decision_point_is_a_state_including_failed_episodes(tmp_path):
    p = tmp_path / "s.jsonl"
    failed = dict(EP, index=8, best_level=2)
    p.write_text("\n".join(json.dumps(x) for x in (
        EP, failed, {"family": "guide_divergence", "index": 9, "error": "boom"},
        {"family": "guide_divergence", "index": 10, "skipped": "no_headroom", "messages": []})))
    states = m8_grpo.load_states(str(p))
    assert [(s["index"], s["turn"]) for s in states] == [(7, 1), (7, 2), (8, 1), (8, 2)]
    assert states[1]["messages"][-1] == {"role": "user", "content": "fb1"}
    assert len(m8_states.states_from_episode(EP)) == 2


def test_group_advantages_normalise_and_skip_groups_without_spread():
    adv = m8_grpo.group_advantages([0.0, 1.0, 1.0, 0.0])
    assert adv == [-1.0, 1.0, 1.0, -1.0]
    assert m8_grpo.group_advantages([0.75] * 8) is None


def test_trim_completion_keeps_the_end_token_and_drops_padding():
    assert m8_grpo.trim_completion([5, 6, 99, 0, 0], eos_ids={99}, pad_id=0) == [5, 6, 99]
    assert m8_grpo.trim_completion([5, 0, 0], eos_ids={99}, pad_id=0) == [5]


def test_left_padding_right_aligns_every_completion():
    ids, attn, cmask, width = m8_grpo.left_pad_batch([1, 2, 3], [[7, 8, 99], [9, 99]], pad_id=0)
    assert width == 3
    assert ids == [[1, 2, 3, 7, 8, 99], [0, 1, 2, 3, 9, 99]]
    assert attn == [[1, 1, 1, 1, 1, 1], [0, 1, 1, 1, 1, 1]]
    assert cmask == [[1, 1, 1], [0, 1, 1]]
    # the masked window is exactly the completion tokens
    for row, m, c in zip(ids, cmask, [[7, 8, 99], [9, 99]]):
        assert [t for t, keep in zip(row[-width:], m) if keep] == c


def test_sparse_reward_is_pass_fail_only():
    """Ablation of the ladder's shaping (pre-registered 2026-09-18)."""
    results = [{"level": 4, "reward": 1.21}, {"level": 3, "reward": 0.98},
               {"level": 3, "reward": 0.76}, {"level": 0, "reward": 0.0}]
    assert m8_grpo.shape_rewards(results) == [1.21, 0.98, 0.76, 0.0]
    assert m8_grpo.shape_rewards(results, sparse=True) == [1.0, 0.0, 0.0, 0.0]


def test_sparse_reward_removes_signal_when_no_sample_passes():
    """The pre-registered failure mode: with pass/fail only, a group whose
    members all fail carries no gradient, while the ladder still ranks them."""
    near_misses = [{"level": 3, "reward": 0.99}, {"level": 3, "reward": 0.80},
                   {"level": 0, "reward": 0.0}, {"level": 3, "reward": 0.90}]
    assert m8_grpo.group_advantages(m8_grpo.shape_rewards(near_misses)) is not None
    assert m8_grpo.group_advantages(m8_grpo.shape_rewards(near_misses, sparse=True)) is None
