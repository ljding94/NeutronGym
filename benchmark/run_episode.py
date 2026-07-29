"""Run one benchmark task as a headless agent episode, then grade the
artifact the agent actually built (never its claims).

Episode isolation: fresh MCSTAS_MCP_HOME + scratch cwd. The design skill is
installed into the episode cwd by default (the reference baseline config);
--no-skill for ablations. Non-Claude models route via OpenRouter
(ANTHROPIC_BASE_URL) — lessons from the 2026-07-24 OpenRouter spike baked in
(see note/spike-and-pilot-2026-07-24.md; the spike script itself is deleted).

Grading: the candidate instrument is discovered from the episode registry
(most recently modified spec with a built .instr), its run parameters are
its own defaults merged with the agent's last successful job parameters,
and it is re-run under the task's protocol (env-controlled ncount + seed)
before grading against the cached reference.

Usage:
  conda run -n mcstas python benchmark/run_episode.py benchmark/tasks/P1_sans_reproduce.json \
      [--model google/gemini-3.6-flash] [--no-skill] [--episode-dir DIR]
"""

import argparse
import json
import os
import re
import shutil
import subprocess
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, "benchmark"))
import grader  # noqa: E402

MAX_TURNS = 60
TIMEOUT_S = 1800


def run_agent(task: dict, model: str | None, ep: str, use_skill: bool) -> dict:
    home = os.path.join(ep, "home")
    cwd = os.path.join(ep, "cwd")
    os.makedirs(home, exist_ok=True)
    os.makedirs(cwd, exist_ok=True)
    if use_skill:
        skills = os.path.join(cwd, ".claude", "skills")
        os.makedirs(skills, exist_ok=True)
        link = os.path.join(skills, "mcstas-instrument-design")
        if not os.path.exists(link):
            os.symlink(os.path.join(REPO, "skills", "mcstas-instrument-design"),
                       link)

    env = dict(os.environ, MCSTAS_MCP_HOME=home)
    if model and "/" in model:  # OpenRouter id
        env.update({
            "ANTHROPIC_BASE_URL": "https://openrouter.ai/api",
            "ANTHROPIC_AUTH_TOKEN": env["OPENROUTER_API_KEY"],
            "ANTHROPIC_MODEL": model,
            "ANTHROPIC_SMALL_FAST_MODEL": model,
        })
        env.pop("ANTHROPIC_API_KEY", None)

    cmd = ["claude", "-p", task["prompt"],
           "--mcp-config", os.path.join(REPO, ".mcp.json"),
           "--strict-mcp-config",
           "--allowedTools", "mcp__mcstas", "Skill", "Read",
           "--max-turns", str(MAX_TURNS),
           "--output-format", "stream-json", "--verbose"]
    if model:
        cmd += ["--model", model]

    transcript = os.path.join(ep, "transcript.jsonl")
    with open(transcript, "w") as out:
        proc = subprocess.run(cmd, cwd=cwd, env=env, stdout=out,
                              stderr=subprocess.PIPE, text=True,
                              timeout=TIMEOUT_S)

    tool_calls, texts, result, actual_model, skill_used = {}, [], {}, None, False
    with open(transcript) as f:
        for line in f:
            try:
                ev = json.loads(line)
            except json.JSONDecodeError:
                continue
            if ev.get("type") == "system" and ev.get("subtype") == "init":
                actual_model = ev.get("model")
            elif ev.get("type") == "assistant":
                for b in ev.get("message", {}).get("content", []):
                    if b.get("type") == "tool_use":
                        tool_calls[b["name"]] = tool_calls.get(b["name"], 0) + 1
                        if b["name"] == "Skill":
                            skill_used = True
                    elif b.get("type") == "text" and b.get("text"):
                        texts.append(b["text"])
            elif ev.get("type") == "result":
                result = ev
    final = result.get("result") or (texts[-1] if texts else "")
    return {
        "model": actual_model or model or "claude-default",
        "returncode": proc.returncode,
        "turns": result.get("num_turns"),
        "cost_usd": result.get("total_cost_usd"),
        "duration_s": round((result.get("duration_ms") or 0) / 1000, 1),
        "mcp_calls": {k: v for k, v in tool_calls.items()
                      if k.startswith("mcp__mcstas")},
        "skill_used": skill_used,
        "final_answer": final,
    }


def find_candidate(home: str):
    """(instr_path, params) of the agent's most recent built instrument."""
    inst_root = os.path.join(home, "instruments")
    if not os.path.isdir(inst_root):
        return None, None
    best, best_mtime = None, -1
    for name in os.listdir(inst_root):
        spec_p = os.path.join(inst_root, name, "spec.json")
        instr_p = os.path.join(inst_root, name, f"{name}.instr")
        if os.path.isfile(spec_p) and os.path.isfile(instr_p):
            mt = os.path.getmtime(spec_p)
            if mt > best_mtime:
                best, best_mtime = (spec_p, instr_p), mt
    if not best:
        return None, None
    spec_p, instr_p = best
    with open(spec_p) as f:
        spec = json.load(f)
    params = {p["name"]: p["default"] for p in spec.get("parameters", [])
              if p.get("default") is not None}
    # fill required (no-default) params from the agent's last OK job
    try:
        with open(os.path.join(home, "jobs.json")) as f:
            jobs = json.load(f)
        for j in sorted(jobs.values(), key=lambda j: j.get("started", 0)):
            if j.get("ok") and j.get("instr") == instr_p:
                for k, v in (j.get("params") or {}).items():
                    params.setdefault(k, v)
    except (OSError, json.JSONDecodeError):
        pass
    return instr_p, params


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("task")
    ap.add_argument("--model", default=None,
                    help="OpenRouter id (with /) or Claude model alias; "
                         "default = local claude default")
    ap.add_argument("--no-skill", action="store_true")
    ap.add_argument("--episode-dir", default=None)
    args = ap.parse_args()
    with open(args.task) as f:
        task = json.load(f)

    tag = re.sub(r"\W", "_", args.model or "claude")
    ep = args.episode_dir or os.path.join(REPO, "runs", "pilot",
                                          f"{task['id']}__{tag}")
    # MCSTAS_MCP_HOME reaches the MCP server via env and the server runs with
    # a different cwd — a relative episode dir would scatter the registry
    ep = os.path.abspath(ep)
    shutil.rmtree(ep, ignore_errors=True)
    os.makedirs(ep)

    episode = run_agent(task, args.model, ep, use_skill=not args.no_skill)

    instr_path, params = find_candidate(os.path.join(ep, "home"))
    if instr_path is None:
        report = {"task": task["id"], "pass": False, "score": 0.0,
                  "hard_failures": ["agent left no built instrument in the "
                                    "episode registry"], "checks": []}
    else:
        ref = grader.reference_summary(task)
        cand = grader.run_protocol(instr_path, params or {}, task["protocol"],
                                   os.path.join(ep, "grade_work"), "cand")
        report = grader.grade(task, cand, ref)
        report["candidate_instr"] = instr_path
        report["candidate_params"] = params

    out = {"task": task["id"], "episode": episode, "grade": report}
    with open(os.path.join(ep, "report.json"), "w") as f:
        json.dump(out, f, indent=2)
    print(json.dumps({**out, "episode": {k: v for k, v in episode.items()
                                         if k != "final_answer"}}, indent=2))
    print(f"\n{'PASS' if report['pass'] else 'FAIL'}  score={report['score']}  "
          f"({report.get('checks_passed', '0/0')})  report: {ep}/report.json")


if __name__ == "__main__":
    main()
