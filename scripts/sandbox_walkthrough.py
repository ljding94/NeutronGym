"""Benchmark-sandbox walkthrough: see all three leak-closing layers work.

    conda run -n mcstas python scripts/sandbox_walkthrough.py

1. Server benchmark mode — the exact tool calls the pilot episodes leaked
   through, refused (and still working in interactive mode).
2. The episode Read deny rules every episode gets.
3. The transcript audit re-run over the REAL pilot transcripts in
   runs/pilot/ — it must reproduce the 2026-07-30 audit table (4/7 leaked).

See note/pilot-leak-audit-2026-07-30.md.
"""

import asyncio
import glob
import json
import os
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))

import leak_audit  # noqa: E402
import run_episode  # noqa: E402


def call(tool, **kwargs):
    from fastmcp import Client

    from mcstas_mcp.server import mcp

    async def _run():
        async with Client(mcp) as client:
            res = await client.call_tool(tool, kwargs)
            return json.loads(res.content[0].text)

    return asyncio.run(_run())


def show(label, out):
    text = json.dumps(out)
    print(f"  {label}\n    -> {text[:160]}{'...' if len(text) > 160 else ''}")


def main():
    from mcstas_mcp.config import resources_dir

    sans = os.path.join(resources_dir(), "examples", "Templates",
                        "templateSANS", "templateSANS.instr")

    print("=== Layer 1: server benchmark mode (MCSTAS_MCP_BENCHMARK=1) ===")
    print("The three channels the pilot episodes leaked through:\n")
    os.environ["MCSTAS_MCP_BENCHMARK"] = "1"
    show("list_examples('sans')          [P3 leak, step 1]",
         call("list_examples", search="sans"))
    show("get_example('templateSANS')    [P3/T1 leak]",
         call("get_example", name="templateSANS"))
    show("load_instr_file(<conda examples>/templateSANS.instr)  [gemini retry leak]",
         call("load_instr_file", path=sans))
    print("\nInteractive mode (benchmark off) keeps the tools — restriction, "
          "not removal:\n")
    del os.environ["MCSTAS_MCP_BENCHMARK"]
    out = call("get_example", name="templateSANS")
    show("get_example('templateSANS')",
         {"ok": out["ok"], "source": f"<{len(out.get('source', ''))} chars>"})

    print("\n=== Layer 2: episode Read deny rules (.claude/settings.json) ===")
    for rule in run_episode.sandbox_deny_rules():
        print(f"  deny {rule}")
    print("  (the design skill stays readable — it is the baseline config)")

    print("\n=== Layer 3: transcript audit over the REAL pilot episodes ===")
    print("Expected from note/pilot-leak-audit-2026-07-30.md: 4 of 7 leaked.\n")
    forbidden = leak_audit.default_forbidden_trees(REPO)
    leaked = 0
    for t in sorted(glob.glob(os.path.join(REPO, "runs", "pilot", "*",
                                           "transcript.jsonl"))):
        ep = os.path.dirname(t)
        audit = leak_audit.audit_transcript(
            t, allowed_roots=[os.path.join(ep, "home"),
                              os.path.join(ep, "cwd")],
            forbidden_trees=forbidden)
        leaked += audit["leaked"]
        tools = ", ".join(f"{e['tool'].replace('mcp__mcstas__', '')}"
                          f"({e['target']})" for e in audit["events"]) or "—"
        print(f"  {'LEAKED' if audit['leaked'] else 'clean ':6} "
              f"{os.path.basename(ep):55} {tools[:70]}")
    print(f"\n  {leaked} leaked episodes found"
          + (" — matches the audit note." if leaked == 4 else
         " — MISMATCH vs the audit note (expected 4); investigate."))


if __name__ == "__main__":
    main()
