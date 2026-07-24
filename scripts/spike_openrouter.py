"""De-risk gate 3: tool-calling fidelity of non-Claude models through the
real scaffold (headless Claude Code + ANTHROPIC_BASE_URL=OpenRouter + the
mcstas MCP server).

Per model: isolated episode (fresh MCSTAS_MCP_HOME, scratch cwd so no
CLAUDE.md/skill leaks in), the M1 acceptance prompt, MCP tools allowed only.
Verdicts come from the transcript (tool-use counts) AND from disk (did an
instrument + successful job actually appear?) — never from the model's own
claims alone.

Usage: python scripts/spike_openrouter.py [model ...]
"""

import json
import os
import re
import shutil
import subprocess
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SPIKE = os.path.join(REPO, "runs", "spike_openrouter")
PROMPT = ("build a source → guide → PSD instrument and tell me the flux "
          "at the detector")
DEFAULT_MODELS = ["openai/gpt-5.5", "qwen/qwen3.7-plus"]
MAX_TURNS = 40
TIMEOUT_S = 1200


def run_episode(model: str) -> dict:
    tag = re.sub(r"\W", "_", model)
    ep = os.path.join(SPIKE, tag)
    shutil.rmtree(ep, ignore_errors=True)
    home = os.path.join(ep, "home")
    cwd = os.path.join(ep, "cwd")
    os.makedirs(home)
    os.makedirs(cwd)

    env = dict(os.environ)
    env.update({
        "MCSTAS_MCP_HOME": home,
        "ANTHROPIC_BASE_URL": "https://openrouter.ai/api",
        "ANTHROPIC_AUTH_TOKEN": env["OPENROUTER_API_KEY"],
        "ANTHROPIC_MODEL": model,
        "ANTHROPIC_SMALL_FAST_MODEL": model,  # avoid a haiku-id 404 on OpenRouter
    })
    env.pop("ANTHROPIC_API_KEY", None)

    cmd = ["claude", "-p", PROMPT,
           "--model", model,
           "--mcp-config", os.path.join(REPO, ".mcp.json"),
           "--strict-mcp-config",
           "--allowedTools", "mcp__mcstas",
           "--max-turns", str(MAX_TURNS),
           "--output-format", "stream-json", "--verbose"]
    transcript_path = os.path.join(ep, "transcript.jsonl")
    print(f"\n=== {model} (transcript: {os.path.relpath(transcript_path, REPO)})")
    try:
        with open(transcript_path, "w") as out:
            proc = subprocess.run(cmd, cwd=cwd, env=env, stdout=out,
                                  stderr=subprocess.PIPE, text=True,
                                  timeout=TIMEOUT_S)
    except subprocess.TimeoutExpired:
        return {"model": model, "fatal": f"timed out after {TIMEOUT_S}s"}
    if proc.returncode != 0 and os.path.getsize(transcript_path) == 0:
        return {"model": model, "fatal": proc.stderr[-500:]}

    tool_calls, denied, errors, result, texts = {}, 0, 0, {}, []
    with open(transcript_path) as f:
        for line in f:
            try:
                ev = json.loads(line)
            except json.JSONDecodeError:
                continue
            if ev.get("type") == "assistant":
                for block in ev.get("message", {}).get("content", []):
                    if block.get("type") == "tool_use":
                        tool_calls[block["name"]] = tool_calls.get(block["name"], 0) + 1
                    elif block.get("type") == "text" and block.get("text"):
                        texts.append(block["text"])
            elif ev.get("type") == "user":
                for block in ev.get("message", {}).get("content", []):
                    if isinstance(block, dict) and block.get("type") == "tool_result":
                        text = json.dumps(block.get("content", ""))
                        if "permission" in text.lower() or "not allowed" in text.lower():
                            denied += 1
                        if '\\"ok\\": false' in text or '"ok": false' in text:
                            errors += 1
            elif ev.get("type") == "result":
                result = ev

    # ground truth from disk, not from the model's words
    jobs, ok_jobs, instruments = {}, 0, []
    try:
        with open(os.path.join(home, "jobs.json")) as f:
            jobs = json.load(f)
        ok_jobs = sum(1 for j in jobs.values() if j.get("ok"))
    except (OSError, json.JSONDecodeError):
        pass
    inst_dir = os.path.join(home, "instruments")
    if os.path.isdir(inst_dir):
        instruments = sorted(os.listdir(inst_dir))

    # non-Claude models can leave the result event's text empty — the real
    # final answer is the last assistant text block (M6 harness lesson)
    final = result.get("result", "") or (texts[-1] if texts else "")
    flux = re.search(r"[\d.eE+^{}\\ -]+\s*(?:\\text\{)?\s*(n/s|neutrons)", final)
    mcp_calls = {k: v for k, v in tool_calls.items() if k.startswith("mcp__mcstas")}
    return {
        "model": model,
        "turns": result.get("num_turns"),
        "cost_usd": result.get("total_cost_usd"),
        "duration_s": round((result.get("duration_ms") or 0) / 1000, 1),
        "mcp_tool_calls": sum(mcp_calls.values()),
        "distinct_mcp_tools": len(mcp_calls),
        "tool_breakdown": mcp_calls,
        "tool_errors_recovered": errors,
        "denied_non_mcp": denied,
        "instruments_on_disk": instruments,
        "jobs_ok_on_disk": ok_jobs,
        "flux_reported": flux.group(0) if flux else None,
        "is_error": result.get("is_error", False),
        "final_answer_tail": final[-400:],
    }


def verdict(r: dict) -> str:
    if r.get("fatal"):
        return "FATAL"
    ok = (r["mcp_tool_calls"] >= 4 and r["jobs_ok_on_disk"] >= 1
          and r["instruments_on_disk"] and r["flux_reported"])
    partial = r["mcp_tool_calls"] >= 2
    return "PASS" if ok else ("PARTIAL" if partial else "FAIL")


def main():
    models = sys.argv[1:] or DEFAULT_MODELS
    if not os.environ.get("OPENROUTER_API_KEY"):
        sys.exit("OPENROUTER_API_KEY not set")
    results = []
    for m in models:
        r = run_episode(m)
        r["verdict"] = verdict(r)
        results.append(r)
        print(json.dumps({k: v for k, v in r.items()
                          if k != "final_answer_tail"}, indent=2))
    print("\n=== GATE 3 SUMMARY ===")
    for r in results:
        print(f"{r['verdict']:8} {r['model']:28} "
              f"mcp_calls={r.get('mcp_tool_calls', 0):3} "
              f"jobs_ok={r.get('jobs_ok_on_disk', 0)} "
              f"cost=${r.get('cost_usd') or 0:.2f}")
    out = os.path.join(SPIKE, "summary.json")
    with open(out, "w") as f:
        json.dump(results, f, indent=2)
    print(f"written: {os.path.relpath(out, REPO)}")


if __name__ == "__main__":
    main()
