"""M8 rejection-sampling collector regressions. The slow tests drive the
REAL env with scripted models — no network, no spend."""

import json
import os

import pytest

from neutrongym import rollouts


def test_parse_action_last_complete_json():
    free = {"w_in": [0.01, 0.09], "w_out": [0.01, 0.09]}
    ans = ('Thinking... {"w_in": 0.02} then final: '
           '{"w_in": 0.05, "w_out": 0.03, "note": 1}')
    assert rollouts.parse_action(ans, free) == {"w_in": 0.05, "w_out": 0.03}
    assert rollouts.parse_action("no json", free) is None
    assert rollouts.parse_action('{"w_in": 0.05}', free) is None  # incomplete


@pytest.mark.slow
def test_rollout_and_collect_filtering(tmp_path):
    good = json.dumps({"w_in": 0.06, "w_out": 0.03, "m_coat": 2.8})
    bad = json.dumps({"w_in": 0.5, "w_out": 0.02, "m_coat": 2.0})

    def improver(messages):
        return f"Here is my action: {good}"

    def outlaw(messages):
        return bad  # out of bounds every time -> reward stays 0

    calls = {"n": 0}

    def alternating(messages):
        calls["n"] += 1
        return improver(messages) if calls["n"] % 2 else outlaw(messages)

    stats = rollouts.collect(
        model="scripted", n_instances=4, out_path=str(tmp_path / "sft.jsonl"),
        max_steps=2, reward_threshold=1.0, chat_fn=alternating)
    assert stats["instances"] == 4
    assert 0 < stats["kept"] <= 4  # improver halves pass, outlaw halves fail
    kept = [json.loads(x) for x in open(tmp_path / "sft.jsonl")]
    assert len(kept) == stats["kept"]
    for ep in kept:
        assert ep["best_reward"] >= 1.0 and ep["best_level"] >= 3
        roles = [m["role"] for m in ep["messages"]]
        assert roles[0] == "system" and "assistant" in roles
        # level-resolved feedback reached the dialogue
        assert any("deepest level" in m["content"] for m in ep["messages"]
                   if m["role"] == "user")


@pytest.mark.slow
def test_unparseable_actions_bounded_by_step_cap(tmp_path):
    stats = rollouts.collect(
        model="scripted", n_instances=2, out_path=str(tmp_path / "s.jsonl"),
        max_steps=3, chat_fn=lambda m: "I refuse to emit JSON.")
    assert stats["kept"] == 0 and stats["instances"] == 2
