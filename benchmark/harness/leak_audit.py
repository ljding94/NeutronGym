"""Post-episode transcript leak audit — layer 3 of the benchmark sandbox.

Scans a stream-json episode transcript for leak-class tool calls (example
tools, out-of-tree load_instr_file / Read targets) and produces the
`reference_leak` stamp for report.json. A leaked episode is INVALID
regardless of score. Refused attempts (the sandbox held) are recorded but do
not invalidate; a flagged call whose result is missing from the transcript
counts as a leak — we cannot prove the sandbox held. Belt and braces over
layers 1–2: the audit catches whatever future tool-surface changes miss.
See note/pilot-leak-audit-2026-07-30.md.
"""

import json
import os

# tool names are normalized before classification: Claude Code transcripts
# prefix MCP tools (mcp__mcstas__get_example), the reference loop records
# bare names (get_example) — the audit must catch both
EXAMPLE_TOOLS = {"list_examples", "get_example"}
LOAD_TOOL = "load_instr_file"


def _tool_name(name: str) -> str:
    return name.rsplit("__", 1)[-1]


def _real(path: str) -> str:
    return os.path.realpath(os.path.abspath(os.path.expanduser(path)))


def _within(path: str, roots) -> bool:
    real = _real(path)
    for root in roots:
        root = _real(root)
        if real == root or real.startswith(root.rstrip(os.sep) + os.sep):
            return True
    return False


def default_forbidden_trees(repo: str) -> list:
    """Trees an episode Read must never touch: the conda McStas resources
    tree (shipped examples = reference .instr sources + %Example ground
    truth), benchmark/ (tasks, refcache, committed baselines), and any
    ~/.mcstas-mcp state outside the episode home."""
    trees = [os.path.join(repo, "benchmark"),
             os.path.join(os.path.expanduser("~"), ".mcstas-mcp")]
    from mcstas_mcp.config import resources_dir
    trees.append(resources_dir())
    return trees


def _tool_result_refused(res: dict | None) -> bool | None:
    """None = no result found (conservative: caller treats as not refused)."""
    if res is None:
        return None
    if res.get("is_error"):
        return True
    # MCP tool results carry the server's JSON as text content
    content = res.get("content")
    texts = []
    if isinstance(content, str):
        texts = [content]
    elif isinstance(content, list):
        texts = [b.get("text", "") for b in content
                 if isinstance(b, dict) and b.get("type") == "text"]
    for t in texts:
        try:
            payload = json.loads(t)
        except (json.JSONDecodeError, TypeError):
            continue
        if isinstance(payload, dict) and "ok" in payload:
            return not payload["ok"]
    return False


def audit_transcript(transcript_path: str, allowed_roots, forbidden_trees,
                     exempt=()) -> dict:
    """Return the reference_leak stamp: {"leaked": bool, "events": [...]}.

    allowed_roots — episode home/cwd (+ per-task exemptions): a
    load_instr_file target must resolve inside one of them.
    forbidden_trees — trees a Read must not touch (see
    default_forbidden_trees); exempt paths override (T2 baselines are task
    input by design).
    """
    calls, results = {}, {}
    with open(transcript_path) as f:
        for line in f:
            try:
                ev = json.loads(line)
            except json.JSONDecodeError:
                continue
            for b in (ev.get("message") or {}).get("content", []) or []:
                if not isinstance(b, dict):
                    continue
                if ev.get("type") == "assistant" and b.get("type") == "tool_use":
                    calls[b.get("id")] = b
                elif ev.get("type") == "user" and b.get("type") == "tool_result":
                    results[b.get("tool_use_id")] = b

    allowed = list(allowed_roots) + list(exempt)
    events = []
    for cid, call in calls.items():
        raw, inp = call.get("name", ""), call.get("input") or {}
        name = _tool_name(raw)
        kind = target = None
        if name in EXAMPLE_TOOLS:
            kind = "example_tool"
            target = inp.get("name") or inp.get("search") or ""
        elif name == LOAD_TOOL:
            path = inp.get("path", "")
            if path and not _within(path, allowed):
                kind, target = "out_of_tree_load", path
        elif name == "Read":
            path = inp.get("file_path", "")
            if (path and _within(path, forbidden_trees)
                    and not _within(path, exempt)):
                kind, target = "forbidden_read", path
        if kind:
            refused = _tool_result_refused(results.get(cid))
            events.append({"tool": raw, "kind": kind, "target": target,
                           "refused": bool(refused)})
    return {"leaked": any(not e["refused"] for e in events), "events": events}
