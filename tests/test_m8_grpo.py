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


def test_states_can_be_pooled_across_families_keeping_their_family(tmp_path):
    """Joint training mixes families in one batch, so every state (and every
    scoring request) must carry its own family (2026-09-20)."""
    import json
    a = tmp_path / "a.jsonl"; b = tmp_path / "b.jsonl"
    msgs = [{"role": "system", "content": "s"}, {"role": "user", "content": "t"},
            {"role": "assistant", "content": "x"}]
    a.write_text(json.dumps({"family": "guide_match", "index": 1, "messages": msgs}))
    b.write_text(json.dumps({"family": "tof_chopper", "index": 2, "messages": msgs}))
    pooled = m8_grpo.load_states(str(a)) + m8_grpo.load_states(str(b))
    assert [s["family"] for s in pooled] == ["guide_match", "tof_chopper"]
    assert [s["index"] for s in pooled] == [1, 2]


def test_post_json_retries_then_succeeds(monkeypatch):
    """A dropped connection must not abort training: the joint run died at
    step 12 on one RemoteDisconnected and forfeited two hours (2026-09-20)."""
    import http.client
    calls = {"n": 0}

    class FakeResp:
        def __enter__(self): return self
        def __exit__(self, *a): return False
        def read(self): return b'{"results": [{"reward": 1.0}]}'

    def fake_urlopen(req, timeout=None):
        calls["n"] += 1
        if calls["n"] < 3:
            raise http.client.RemoteDisconnected("closed")
        return FakeResp()

    import urllib.request
    monkeypatch.setattr(urllib.request, "urlopen", fake_urlopen)
    monkeypatch.setattr(m8_grpo.time if hasattr(m8_grpo, "time") else __import__("time"),
                        "sleep", lambda s: None)
    out = m8_grpo.post_json("http://x/score", {"items": []}, attempts=4)
    assert out["results"][0]["reward"] == 1.0
    assert calls["n"] == 3


def test_post_json_raises_after_last_attempt(monkeypatch):
    import http.client
    import urllib.request

    def always_fail(req, timeout=None):
        raise http.client.RemoteDisconnected("closed")

    monkeypatch.setattr(urllib.request, "urlopen", always_fail)
    monkeypatch.setattr(__import__("time"), "sleep", lambda s: None)
    try:
        m8_grpo.post_json("http://x/score", {"items": []}, attempts=2)
    except http.client.RemoteDisconnected:
        return
    raise AssertionError("post_json swallowed a permanent failure")


def test_reward_server_isolates_a_failing_item():
    """One bad item scores 0.0 and is labelled, instead of taking the whole
    batch (and the training run) down with it."""
    sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
        os.path.abspath(__file__))), "benchmark", "harness"))
    import reward_server

    bad = ("no-such-family", "train", 0.85, 10, 0, "{}")
    out = reward_server.score_item(bad)
    assert out["reward"] == 0.0 and out["parsed"] is False
    assert "error" in out and "KeyError" in out["error"]


def test_load_states_drops_duplicate_episodes(tmp_path, capsys):
    """A re-run collection can leave an episode in the file twice, which
    doubles its sampling weight -- the bender pool held indices 53-58 twice
    (2026-09-20). Keep the first, and say so rather than biasing silently."""
    ep = {"family": "bender", "index": 7, "messages": [
        {"role": "user", "content": "u"}, {"role": "assistant", "content": "a"},
        {"role": "user", "content": "u2"}, {"role": "assistant", "content": "a2"}]}
    other = dict(ep, index=8)
    p = tmp_path / "states.jsonl"
    p.write_text("\n".join(json.dumps(e) for e in [ep, other, ep]) + "\n")

    states = m8_grpo.load_states(str(p))
    assert [(s["family"], s["index"], s["turn"]) for s in states] == [
        ("bender", 7, 1), ("bender", 7, 2), ("bender", 8, 1), ("bender", 8, 2)]
    assert "dropped 1 duplicate episode" in capsys.readouterr().out


def test_load_states_keeps_same_index_in_different_families(tmp_path):
    """Joint training pools families; index 7 of two families is two states."""
    mk = lambda fam: {"family": fam, "index": 7, "messages": [  # noqa: E731
        {"role": "user", "content": "u"}, {"role": "assistant", "content": "a"}]}
    p = tmp_path / "states.jsonl"
    p.write_text("\n".join(json.dumps(mk(f)) for f in ("bender", "tof_chopper")) + "\n")
    assert len(m8_grpo.load_states(str(p))) == 2
