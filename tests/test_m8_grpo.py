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
