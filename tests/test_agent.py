"""Reference-loop regressions. Fast tests cover the pure pieces; the slow
test drives the FULL loop — scripted model, real stdio MCP server subprocess,
benchmark sandbox on — and round-trips the transcript through the leak audit
and the harness episode parser. No network anywhere."""

import json
import os
import sys

import pytest

from neutrongym import agent

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))

import leak_audit  # noqa: E402


# --- fast: pure pieces -----------------------------------------------------------

def test_resolve_backend_routing(monkeypatch):
    monkeypatch.setenv("OPENROUTER_API_KEY", "or-key")
    monkeypatch.setenv("ANTHROPIC_API_KEY", "an-key")
    assert agent.resolve_backend("google/gemini-3.6-flash") == \
        ("https://openrouter.ai/api/v1", "or-key")
    assert agent.resolve_backend("claude-sonnet-5") == \
        ("https://api.anthropic.com/v1", "an-key")
    assert agent.resolve_backend("qwen-7b", "http://gpu:8000/v1/", "k") == \
        ("http://gpu:8000/v1", "k")
    with pytest.raises(ValueError):
        agent.resolve_backend("mystery-model")
    monkeypatch.delenv("ANTHROPIC_API_KEY")
    with pytest.raises(RuntimeError, match="claude CLI comparison arm"):
        agent.resolve_backend("claude-sonnet-5")


def test_tool_schema_translation():
    class T:
        name, description, inputSchema = "describe_component", "docs", \
            {"type": "object", "properties": {"name": {"type": "string"}}}

    class NoSchema:
        name, description, inputSchema = "ping", None, None

    out = agent.mcp_tools_to_openai([T, NoSchema])
    assert out[0] == {"type": "function", "function": {
        "name": "describe_component", "description": "docs",
        "parameters": T.inputSchema}}
    assert out[1]["function"]["parameters"] == {"type": "object",
                                                "properties": {}}


def test_truncation_caps_tool_results():
    big = "x" * (agent.TOOL_RESULT_CAP + 500)
    t = agent._truncate(big)
    assert len(t) < len(big) and "truncated 500 chars" in t
    assert agent._truncate("small") == "small"


# --- slow: full loop against the real stdio server -------------------------------

def scripted_model(script):
    """chat_fn yielding canned OpenAI-style responses in order."""
    it = iter(script)

    def chat_fn(messages, tools):
        assert any(t["function"]["name"] == "describe_component"
                   for t in tools)  # real server tool list reached the model
        return next(it)

    return chat_fn


def _tool_call(cid, name, args):
    return {"id": cid, "type": "function",
            "function": {"name": name, "arguments": json.dumps(args)}}


SCRIPT = [
    # turn 1: try the forbidden example tool AND a legitimate one
    {"choices": [{"message": {"role": "assistant", "content": "Looking.",
                              "tool_calls": [
                                  _tool_call("c1", "get_example",
                                             {"name": "templateSANS"}),
                                  _tool_call("c2", "describe_component",
                                             {"name": "PSD_monitor"}),
                              ]}}],
     "usage": {"prompt_tokens": 100, "completion_tokens": 20}},
    # turn 2: finish
    {"choices": [{"message": {"role": "assistant",
                              "content": "Done: sandbox refused examples; "
                                         "PSD_monitor has nx/ny."}}],
     "usage": {"prompt_tokens": 150, "completion_tokens": 30}},
]


@pytest.fixture(scope="module")
def episode(tmp_path_factory):
    ep = str(tmp_path_factory.mktemp("loop_ep"))
    meta = agent.run_episode("Build nothing; this is a plumbing test.",
                             model="scripted/model", episode_dir=ep,
                             skill_text="SKILL RULE: always fix a seed.",
                             chat_fn=scripted_model(SCRIPT))
    return ep, meta


@pytest.mark.slow
def test_loop_meta_and_accounting(episode):
    _, meta = episode
    assert meta["scaffold"] == "neutrongym-reference-loop"
    assert meta["turns"] == 2 and not meta["hit_turn_cap"]
    assert meta["usage"] == {"prompt_tokens": 250, "completion_tokens": 50}
    assert meta["mcp_calls"] == {"get_example": 1, "describe_component": 1}
    assert meta["skill_used"] and "sandbox refused" in meta["final_answer"]


@pytest.mark.slow
def test_sandbox_refuses_examples_but_serves_components(episode):
    ep, _ = episode
    events = [json.loads(x) for x in
              open(os.path.join(ep, "transcript.jsonl"))]
    results = {b["tool_use_id"]: b["content"][0]["text"]
               for e in events if e["type"] == "user"
               for b in e["message"]["content"]}
    assert "disabled in benchmark mode" in results["c1"]  # layer 1 held
    payload = json.loads(results["c2"])
    assert payload["ok"] and any(p["name"] == "nx"
                                 for p in payload["parameters"])


@pytest.mark.slow
def test_transcript_round_trips_through_leak_audit(episode):
    ep, _ = episode
    audit = leak_audit.audit_transcript(
        os.path.join(ep, "transcript.jsonl"),
        allowed_roots=[os.path.join(ep, "home"), os.path.join(ep, "cwd")],
        forbidden_trees=leak_audit.default_forbidden_trees(REPO))
    # the attempt is recorded; the refusal means it is NOT a leak
    assert not audit["leaked"]
    assert audit["events"] == [{"tool": "get_example",
                                "kind": "example_tool",
                                "target": "templateSANS", "refused": True}]


@pytest.mark.slow
def test_transcript_parses_like_stream_json(episode):
    ep, _ = episode
    events = [json.loads(x) for x in
              open(os.path.join(ep, "transcript.jsonl"))]
    assert events[0]["type"] == "system" and events[0]["subtype"] == "init"
    assert events[-1]["type"] == "result"
    assert events[-1]["result"].startswith("Done:")
    kinds = {e["type"] for e in events}
    assert kinds == {"system", "assistant", "user", "result"}


@pytest.mark.slow
def test_empty_response_nudged_not_accepted(tmp_path):
    empty = {"choices": [{"message": {"role": "assistant", "content": ""},
                          "finish_reason": "stop"}], "usage": {}}
    script = [empty,
              {"choices": [{"message": {"role": "assistant",
                                        "content": "Recovered summary."}}],
               "usage": {}}]
    meta = agent.run_episode("plumbing", model="scripted/model",
                             episode_dir=str(tmp_path),
                             chat_fn=scripted_model(script))
    assert meta["final_answer"] == "Recovered summary."
    assert meta["empty_responses"] == 1 and meta["turns"] == 2


@pytest.mark.slow
def test_turn_cap_flagged(tmp_path):
    looper = [SCRIPT[0]] * 3  # never finishes
    meta = agent.run_episode("plumbing", model="scripted/model",
                             episode_dir=str(tmp_path), max_turns=3,
                             chat_fn=scripted_model(looper))
    assert meta["turns"] == 3 and meta["hit_turn_cap"]
    assert meta["final_answer"] == ""
