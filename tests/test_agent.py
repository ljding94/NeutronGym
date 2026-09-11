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


def test_extract_instr_picks_the_instrument_block():
    ans = ("Here is the file:\n```c\nDEFINE INSTRUMENT x(a=1)\nTRACE\nEND\n"
           "```\nand a snippet:\n```\nnot an instrument\n```")
    src = agent.extract_instr(ans)
    assert src.startswith("DEFINE INSTRUMENT x") and src.endswith("END\n")
    assert agent.extract_instr("no code here") is None
    bare = "DEFINE INSTRUMENT y()\nTRACE\nEND"
    assert agent.extract_instr(bare).endswith("END\n")


def test_oneshot_writes_transcript_and_meta(tmp_path):
    resp = {"choices": [{"message": {
        "role": "assistant",
        "content": "```\nDEFINE INSTRUMENT z(p=2)\nTRACE\nEND\n```"}}],
        "usage": {"prompt_tokens": 10, "completion_tokens": 5},
        "provider": "Google"}
    meta = agent.run_oneshot("build z", "scripted/model",
                             episode_dir=str(tmp_path),
                             chat_fn=lambda m, t: resp)
    assert meta["scaffold"] == "plain-llm-oneshot"
    assert meta["turns"] == 1 and meta["mcp_calls"] == {}
    assert "DEFINE INSTRUMENT z" in meta["final_answer"]
    events = [json.loads(x) for x in
              open(os.path.join(str(tmp_path), "transcript.jsonl"))]
    assert [e["type"] for e in events] == ["system", "assistant", "result"]


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
    # a model that has used NO tools is nudged for prose too (symmetry fix),
    # so reaching a final answer without tools takes the full nudge budget
    prose = {"choices": [{"message": {"role": "assistant",
                                      "content": "Recovered summary."}}],
             "usage": {}}
    meta = agent.run_episode("plumbing", model="scripted/model",
                             episode_dir=str(tmp_path),
                             chat_fn=scripted_model([empty, prose, prose]))
    assert meta["final_answer"] == "Recovered summary."
    assert meta["empty_responses"] == 3 and meta["turns"] == 3


def test_parse_tool_args_never_raises():
    """Malformed tool arguments must become feedback, not a dead episode
    (2026-09-10: an unguarded json.loads killed 5 of 17 episodes the
    moment a model started emitting truncated arguments)."""
    assert agent.parse_tool_args('{"a": 1}') == ({"a": 1}, None)
    assert agent.parse_tool_args("") == ({}, None)
    args, err = agent.parse_tool_args('{"a": "unterminated')
    assert args == {} and "malformed JSON" in err
    args, err = agent.parse_tool_args("[1,2]")
    assert args == {} and "JSON object" in err


@pytest.mark.slow
def test_malformed_tool_args_are_fed_back_not_fatal(tmp_path):
    bad = {"choices": [{"message": {
        "role": "assistant", "content": None,
        "tool_calls": [{"id": "b1", "type": "function",
                        "function": {"name": "describe_component",
                                     "arguments": '{"name": "PSD'}}]}}],
        "usage": {}}
    done = {"choices": [{"message": {"role": "assistant",
                                     "content": "Recovered."}}], "usage": {}}
    meta = agent.run_episode("plumbing", model="scripted/model",
                             episode_dir=str(tmp_path),
                             chat_fn=scripted_model([bad, done]))
    assert meta["returncode"] == 0 and meta["final_answer"] == "Recovered."
    events = [json.loads(x) for x in
              open(os.path.join(str(tmp_path), "transcript.jsonl"))]
    results = [b for e in events if e["type"] == "user"
               for b in e["message"]["content"]]
    assert results[0]["is_error"] and "malformed JSON" in \
        results[0]["content"][0]["text"]


@pytest.mark.slow
def test_malformed_args_are_not_echoed_back_to_the_provider(tmp_path):
    """Echoing unparseable arguments makes the PROVIDER reject our next
    request (DigitalOcean 400'd three episodes at turn 13, discarding work
    already done). History must carry sanitized arguments."""
    seen = []

    def chat_fn(messages, tools):
        seen.append([m for m in messages if m.get("tool_calls")])
        if len(seen) == 1:
            return {"choices": [{"message": {
                "role": "assistant", "content": None,
                "tool_calls": [{"id": "b1", "type": "function",
                                "function": {"name": "describe_component",
                                             "arguments": '{"name": "PSD'}}]}}],
                "usage": {}}
        return {"choices": [{"message": {"role": "assistant",
                                         "content": "done"}}], "usage": {}}

    agent.run_episode("plumbing", model="scripted/model",
                      episode_dir=str(tmp_path), chat_fn=chat_fn)
    echoed = seen[-1]
    assert echoed, "the assistant turn should be in history"
    for m in echoed:
        for c in m["tool_calls"]:
            json.loads(c["function"]["arguments"])  # must be valid JSON


@pytest.mark.slow
def test_prose_without_tools_is_nudged_not_accepted(tmp_path):
    """SYMMETRY (2026-09-10 peer review): a model answering in prose having
    never touched a tool has not attempted the task and must be nudged, the
    same as an empty turn. Previously it ended the episode on turn 1, which
    scored such models L0 'never called a tool' without ever asking twice."""
    prose = {"choices": [{"message": {"role": "assistant",
                                      "content": "I would build a guide."}}],
             "usage": {}}
    tool_turn = {"choices": [{"message": {
        "role": "assistant", "content": None,
        "tool_calls": [_tool_call("t1", "describe_component",
                                  {"name": "PSD_monitor"})]}}], "usage": {}}
    done = {"choices": [{"message": {"role": "assistant",
                                     "content": "Built it."}}], "usage": {}}
    meta = agent.run_episode("plumbing", model="scripted/model",
                             episode_dir=str(tmp_path),
                             chat_fn=scripted_model([prose, tool_turn, done]))
    # nudged once, then it engaged the tools, then prose was accepted as final
    assert meta["turns"] == 3 and meta["mcp_calls"] == {"describe_component": 1}
    assert meta["final_answer"] == "Built it."


def test_provider_pin_lands_in_payload_and_meta(tmp_path):
    captured = {}

    class FakeHttp:
        def post(self, url, headers=None, json=None, timeout=None):
            captured.update(json)
            return type("R", (), {"status_code": 200, "json": lambda s: {
                "provider": "Google",
                "choices": [{"message": {"role": "assistant",
                                         "content": "done"}}],
                "usage": {}}})()

    out = agent.chat_completion(FakeHttp(), "http://x", "k", "m", [], [],
                                0.0, provider_pin="Google")
    assert captured["provider"] == {"order": ["Google"],
                                    "allow_fallbacks": False}
    assert out["provider"] == "Google"
    # unpinned: no provider field at all
    captured.clear()
    agent.chat_completion(FakeHttp(), "http://x", "k", "m", [], [], 0.0)
    assert "provider" not in captured


def test_chat_extra_lands_in_payload_for_local_models():
    """The pinned non-thinking Qwen3 config (m6_config serving blocks) —
    the harness sends the per-request opt-out for base-url models."""
    captured = {}

    class FakeHttp:
        def post(self, url, headers=None, json=None, timeout=None):
            captured.update(json)
            return type("R", (), {"status_code": 200, "json": lambda s: {
                "choices": [{"message": {"role": "assistant",
                                         "content": "ok"}}]}})()

    agent.chat_completion(
        FakeHttp(), "http://gpu:8137/v1", "EMPTY", "qwen3-8b", [], [], 0.0,
        chat_extra={"chat_template_kwargs": {"enable_thinking": False}})
    assert captured["chat_template_kwargs"] == {"enable_thinking": False}

    import importlib
    import run_episode
    importlib.reload(run_episode)
    assert run_episode._local_chat_extra("http://gpu:8137/v1") == \
        {"chat_template_kwargs": {"enable_thinking": False}}
    assert run_episode._local_chat_extra(None) is None  # API models: nothing


def test_chat_completion_retries_network_errors(monkeypatch):
    import httpx

    calls = {"n": 0}

    body = {"choices": [{"message": {"role": "assistant", "content": "hi"}}]}

    class FakeHttp:
        def post(self, *a, **k):
            calls["n"] += 1
            if calls["n"] == 1:
                raise httpx.ReadTimeout("slow provider")
            if calls["n"] == 2:  # 200 with an error body — also retryable
                return type("R", (), {"status_code": 200,
                                      "json": lambda self: {"error": "x"}})()
            return type("R", (), {"status_code": 200,
                                  "json": lambda self: body})()

    monkeypatch.setattr(agent.time, "sleep", lambda s: None)
    out = agent.chat_completion(FakeHttp(), "http://x", "k", "m", [], [], 0.0)
    assert out == body and calls["n"] == 3


@pytest.mark.slow
def test_unrecoverable_api_error_records_infra_failure(tmp_path):
    def broken(messages, tools):
        raise RuntimeError("chat completion failed after 3 tries: HTTP 500")

    meta = agent.run_episode("plumbing", model="scripted/model",
                             episode_dir=str(tmp_path), chat_fn=broken)
    assert meta["returncode"] == 1 and "HTTP 500" in meta["error"]
    events = [json.loads(x) for x in
              open(os.path.join(str(tmp_path), "transcript.jsonl"))]
    assert any(e["type"] == "error" for e in events)
    assert events[-1]["type"] == "result"  # transcript closed, not lost


@pytest.mark.slow
def test_turn_cap_flagged(tmp_path):
    looper = [SCRIPT[0]] * 3  # never finishes
    meta = agent.run_episode("plumbing", model="scripted/model",
                             episode_dir=str(tmp_path), max_turns=3,
                             chat_fn=scripted_model(looper))
    assert meta["turns"] == 3 and meta["hit_turn_cap"]
    assert meta["final_answer"] == ""
