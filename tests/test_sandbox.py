"""Benchmark sandbox regressions (note/pilot-leak-audit-2026-07-30.md).

Three layers: (1) server benchmark mode — example tools refuse,
load_instr_file is path-guarded; (2) episode Read deny rules cover the leak
trees; (3) the transcript audit stamps reference_leak and treats refused
attempts as the sandbox holding. The pilot leak (4/7 episodes fetched the
reference via get_example/load_instr_file) must stay impossible.
"""

import asyncio
import json
import os
import shutil
import sys

import pytest
from fastmcp import Client

from mcstas_mcp.config import resources_dir
from mcstas_mcp.server import mcp

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))

import leak_audit  # noqa: E402
import run_episode  # noqa: E402

TEMPLATE_SANS = os.path.join(
    resources_dir(), "examples", "Templates", "templateSANS", "templateSANS.instr")


def call(tool, **kwargs):
    async def _run():
        async with Client(mcp) as client:
            res = await client.call_tool(tool, kwargs)
            return json.loads(res.content[0].text)

    return asyncio.run(_run())


@pytest.fixture
def benchmark_mode(monkeypatch):
    monkeypatch.setenv("MCSTAS_MCP_BENCHMARK", "1")
    monkeypatch.delenv("MCSTAS_MCP_BENCHMARK_ALLOW", raising=False)


# --- layer 1: server benchmark mode ---------------------------------------------

def test_example_tools_refuse_in_benchmark_mode(benchmark_mode):
    for tool, kwargs in (("list_examples", {"search": "sans"}),
                         ("get_example", {"name": "templateSANS"})):
        out = call(tool, **kwargs)
        assert not out["ok"]
        assert "benchmark mode" in out["error"]
        assert "templateSANS" not in json.dumps(out)  # no source, no hints


def test_example_tools_still_work_interactively(monkeypatch):
    monkeypatch.delenv("MCSTAS_MCP_BENCHMARK", raising=False)
    out = call("get_example", name="templateSANS")
    assert out["ok"] and "DEFINE INSTRUMENT" in out["source"]


def test_load_instr_refuses_out_of_tree_paths(benchmark_mode):
    # the exact channel the P3 gemini retry episode used: the conda examples tree
    out = call("load_instr_file", path=TEMPLATE_SANS)
    assert not out["ok"]
    assert "benchmark mode" in out["error"]


def test_load_instr_allows_episode_workspace(benchmark_mode):
    # inside cwd (conftest chdirs to a scratch dir) → guard passes, import runs
    local = os.path.join(os.getcwd(), "mine.instr")
    shutil.copy(TEMPLATE_SANS, local)
    out = call("load_instr_file", path=local, name="sandbox_local")
    assert out["ok"], out.get("error")


def test_load_instr_honors_per_task_exemption(benchmark_mode, tmp_path,
                                              monkeypatch):
    # T2 baselines are task input by design — exempt dir, set by run_episode
    exempt = tmp_path / "baseline"
    exempt.mkdir()
    shutil.copy(TEMPLATE_SANS, exempt / "base.instr")
    out = call("load_instr_file", path=str(exempt / "base.instr"))
    assert not out["ok"]  # not exempt yet
    monkeypatch.setenv("MCSTAS_MCP_BENCHMARK_ALLOW", str(exempt))
    out = call("load_instr_file", path=str(exempt / "base.instr"),
               name="sandbox_exempt")
    assert out["ok"], out.get("error")


def test_benchmark_mode_off_by_default(monkeypatch):
    monkeypatch.delenv("MCSTAS_MCP_BENCHMARK", raising=False)
    from mcstas_mcp.config import benchmark_mode as bm
    assert not bm()
    monkeypatch.setenv("MCSTAS_MCP_BENCHMARK", "0")
    assert not bm()
    monkeypatch.setenv("MCSTAS_MCP_BENCHMARK", "1")
    assert bm()


# --- layer 2: episode Read deny rules --------------------------------------------

def test_deny_rules_cover_the_leak_trees():
    rules = run_episode.sandbox_deny_rules()
    assert all(r.startswith("Read(//") and r.endswith("/**)") for r in rules)
    joined = " ".join(rules)
    assert os.path.realpath(resources_dir()).lstrip("/") in joined
    assert os.path.join(os.path.realpath(REPO), "benchmark").lstrip("/") in joined
    assert ".mcstas-mcp" in joined
    # the skill must NOT be denied — it is the reference baseline config
    assert "skills" not in joined


def test_episode_setup_writes_settings(tmp_path, monkeypatch):
    # run_agent's setup side effects, without spawning claude
    ep = str(tmp_path / "ep")
    monkeypatch.setattr(run_episode.subprocess, "run",
                        lambda *a, **k: type("P", (), {"returncode": 0})())
    task = {"id": "X", "prompt": "p"}
    run_episode.run_agent(task, None, ep, use_skill=True)
    with open(os.path.join(ep, "cwd", ".claude", "settings.json")) as f:
        settings = json.load(f)
    assert settings["permissions"]["deny"] == run_episode.sandbox_deny_rules()
    assert os.path.islink(os.path.join(ep, "cwd", ".claude", "skills",
                                       "mcstas-instrument-design"))


def test_improve_task_exempts_only_its_own_baseline():
    t2 = {"kind": "improve",
          "reference": {"instr": "benchmark/instruments/t2_x/t2_x.instr"}}
    paths = run_episode.task_exempt_paths(t2)
    assert paths == [os.path.join(run_episode.REPO,
                                  "benchmark", "instruments", "t2_x")]
    assert run_episode.task_exempt_paths({"kind": "reproduce"}) == []


# --- layer 3: transcript audit ----------------------------------------------------

def _transcript(tmp_path, *pairs):
    """Write a minimal stream-json transcript of (tool_use, result_or_None)."""
    lines = []
    for i, (use, res) in enumerate(pairs):
        cid = f"call_{i}"
        lines.append({"type": "assistant",
                      "message": {"content": [{"type": "tool_use", "id": cid,
                                               **use}]}})
        if res is not None:
            lines.append({"type": "user",
                          "message": {"content": [
                              {"type": "tool_result", "tool_use_id": cid,
                               **res}]}})
    p = tmp_path / "transcript.jsonl"
    p.write_text("\n".join(json.dumps(x) for x in lines) + "\n")
    return str(p)


def _mcp_result(ok, **extra):
    return {"content": [{"type": "text",
                         "text": json.dumps({"ok": ok, **extra})}]}


def _audit(path, tmp_path, exempt=()):
    return leak_audit.audit_transcript(
        path, allowed_roots=[str(tmp_path / "home"), str(tmp_path / "cwd")],
        forbidden_trees=[str(tmp_path / "forbidden")], exempt=exempt)


def test_successful_get_example_is_a_leak(tmp_path):
    # the exact P3/T1 pilot leak shape
    t = _transcript(tmp_path,
                    ({"name": "mcp__mcstas__get_example",
                      "input": {"name": "templateSANS"}}, _mcp_result(True)))
    audit = _audit(t, tmp_path)
    assert audit["leaked"]
    assert audit["events"] == [{"tool": "mcp__mcstas__get_example",
                                "kind": "example_tool",
                                "target": "templateSANS", "refused": False}]


def test_refused_attempt_is_recorded_but_not_a_leak(tmp_path):
    t = _transcript(tmp_path,
                    ({"name": "mcp__mcstas__list_examples",
                      "input": {"search": "sans"}},
                     _mcp_result(False, error="disabled in benchmark mode")))
    audit = _audit(t, tmp_path)
    assert not audit["leaked"]
    assert audit["events"][0]["refused"]


def test_out_of_tree_load_flagged_in_tree_clean(tmp_path):
    outside = str(tmp_path / "conda" / "examples" / "t.instr")
    inside = str(tmp_path / "home" / "instruments" / "m" / "m.instr")
    t = _transcript(
        tmp_path,
        ({"name": "mcp__mcstas__load_instr_file", "input": {"path": outside}},
         _mcp_result(True)),
        ({"name": "mcp__mcstas__load_instr_file", "input": {"path": inside}},
         _mcp_result(True)))
    audit = _audit(t, tmp_path)
    assert audit["leaked"]
    assert [e["target"] for e in audit["events"]] == [outside]


def test_forbidden_read_flagged_exempt_read_clean(tmp_path):
    bad = str(tmp_path / "forbidden" / "refcache" / "ref.json")
    exempt_dir = str(tmp_path / "forbidden" / "instruments" / "t2_x")
    t = _transcript(
        tmp_path,
        ({"name": "Read", "input": {"file_path": bad}}, {"content": "data"}),
        ({"name": "Read", "input": {"file_path": exempt_dir + "/t2_x.instr"}},
         {"content": "data"}))
    audit = _audit(t, tmp_path, exempt=[exempt_dir])
    assert audit["leaked"]
    assert [e["target"] for e in audit["events"]] == [bad]
    # denied Read (permission rule held) is recorded, not a leak
    t2 = _transcript(tmp_path,
                     ({"name": "Read", "input": {"file_path": bad}},
                      {"is_error": True, "content": "permission denied"}))
    audit2 = _audit(t2, tmp_path)
    assert not audit2["leaked"] and audit2["events"][0]["refused"]


def test_missing_result_counts_as_leak(tmp_path):
    # truncated transcript: cannot prove the sandbox held → conservative
    t = _transcript(tmp_path,
                    ({"name": "mcp__mcstas__get_example",
                      "input": {"name": "templateSANS"}}, None))
    assert _audit(t, tmp_path)["leaked"]


def test_audit_survives_odd_transcript_shapes(tmp_path):
    """Claude Code emits `message` as a STRING on some events; a crash
    here killed an entire episode's report (2026-09-10). The mandatory
    audit must never be the thing that loses an episode."""
    p = tmp_path / "transcript.jsonl"
    p.write_text("\n".join(json.dumps(x) for x in [
        {"type": "assistant", "message": "a bare string"},
        {"type": "user", "message": {"content": "also a string"}},
        {"type": "system", "message": {"content": ["not a dict", 42]}},
        {"type": "assistant", "message": {"content": [
            {"type": "tool_use", "id": "c1",
             "name": "mcp__mcstas__get_example",
             "input": {"name": "templateSANS"}}]}},
        {"type": "result", "result": "done"},
    ]) + "\n")
    audit = _audit(str(p), tmp_path)
    assert audit["leaked"]  # the real call is still detected
    assert audit["events"][0]["target"] == "templateSANS"


def test_clean_transcript_stamps_clean(tmp_path):
    t = _transcript(tmp_path,
                    ({"name": "mcp__mcstas__describe_component",
                      "input": {"name": "PSD_monitor"}}, _mcp_result(True)),
                    ({"name": "Read",
                      "input": {"file_path": str(tmp_path / "cwd" / "n.md")}},
                     {"content": "notes"}))
    assert _audit(t, tmp_path) == {"leaked": False, "events": []}
