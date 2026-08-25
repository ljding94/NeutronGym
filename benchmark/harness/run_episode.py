"""Run one benchmark task as a headless agent episode, then grade the
artifact the agent actually built (never its claims).

Episode isolation: fresh MCSTAS_MCP_HOME + scratch cwd. Benchmark sandbox
(note/pilot-leak-audit-2026-07-30.md), three layers: server benchmark mode
(MCSTAS_MCP_BENCHMARK=1 — example tools off, load_instr_file path-guarded),
episode-scoped Read deny rules (.claude/settings.json), and a mandatory
post-episode transcript audit stamped as reference_leak in report.json — a
leaked episode is INVALID regardless of score. The design skill is
installed into the episode cwd by default (the reference baseline config);
--no-skill for ablations. Non-Claude models route via OpenRouter
(ANTHROPIC_BASE_URL) — lessons from the 2026-07-24 OpenRouter spike baked in
(see note/spike-and-pilot-2026-07-24.md; the spike script itself is deleted).

Grading: the candidate instrument is discovered from the episode registry
(most recently modified spec with a built .instr), its run parameters are
its own defaults merged with the agent's last successful job parameters,
and it is re-run under the task's protocol (env-controlled ncount + seed)
before grading against the cached reference.

Episode folder contract: report.json + transcript.jsonl + artifacts/ (the
candidate .instr + resolved params, diagram PNG, real-scale webgl trace —
side-by-side partner of the reference visuals in runs/refviz/, see
visualize.py --refs).

Usage:
  conda run -n mcstas python benchmark/harness/run_episode.py T1_PSI_DMC \
      [--model google/gemini-3.6-flash] [--no-skill] [--episode-dir DIR]
  (the task argument is an id resolved against benchmark/tasks/, or a path)
"""

import argparse
import glob
import json
import os
import re
import shutil
import subprocess
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))
import grader  # noqa: E402
import leak_audit  # noqa: E402
import visualize  # noqa: E402

MAX_TURNS = 60
TIMEOUT_S = 1800


def resolve_task(arg: str) -> str:
    """Accept a task id (resolved against benchmark/tasks/) or a path."""
    if os.path.isfile(arg):
        return arg
    hits = glob.glob(os.path.join(REPO, "benchmark", "tasks", "**",
                                  f"{arg}.json"), recursive=True)
    if len(hits) == 1:
        return hits[0]
    known = sorted(os.path.splitext(os.path.basename(p))[0] for p in
                   glob.glob(os.path.join(REPO, "benchmark", "tasks", "**",
                                          "*.json"), recursive=True)
                   if not os.path.basename(p).startswith("_"))
    raise SystemExit(f"error: no task '{arg}'. Known ids: {', '.join(known)}")


def task_exempt_paths(task: dict) -> list:
    """Sandbox exemptions for this task (per-task, not global): an improve
    task's baseline instrument is task input by design — the agent is given
    it and may load it."""
    if task.get("kind") == "improve" and task.get("reference", {}).get("instr"):
        return [os.path.join(REPO, os.path.dirname(task["reference"]["instr"]))]
    return []


def sandbox_deny_rules() -> list:
    """Episode-scoped Read deny rules (sandbox layer 2). The skill stays
    readable — it lives under skills/, none of these trees."""
    return [
        "Read(//" + os.path.realpath(t).lstrip("/") + "/**)"
        for t in leak_audit.default_forbidden_trees(REPO)
    ]


def run_agent_loop(task: dict, model: str | None, ep: str,
                   use_skill: bool, provider: str | None = "Google",
                   max_turns: int | None = None) -> dict:
    """The NeutronGym reference loop — the measurement instrument
    (note/scaffold-decision-2026-07-30.md). Sandbox by construction: the
    model sees only MCP tools (no shell/Read), server benchmark mode on,
    per-task exemptions via env; the leak audit still runs as backstop."""
    from neutrongym import agent

    skill_text = None
    if use_skill:
        import neutrongym
        skill_text = neutrongym.skill_text()  # wheel-shipped canonical copy
    if not model:
        raise SystemExit("error: --scaffold loop needs an explicit --model "
                         "(e.g. google/gemini-3.6-flash, claude-sonnet-5, or "
                         "any id with --base-url for vLLM)")
    return agent.run_episode(
        task["prompt"], model, episode_dir=ep,
        home_dir=os.path.join(ep, "home"),
        server_cwd=os.path.join(ep, "cwd"),
        skill_text=skill_text, exempt=task_exempt_paths(task),
        provider_pin=provider or None,
        max_turns=max_turns or agent.DEFAULT_MAX_TURNS)


def run_agent_oneshot(task: dict, model: str | None, ep: str,
                      use_skill: bool, provider: str | None = "Google") -> dict:
    """Plain-LLM baseline arm (M6): one completion, no tools; the emitted
    .instr is written into the standard episode layout so the identical
    grading/audit/artifacts tail applies."""
    from neutrongym import agent, executor
    import neutrongym

    if not model:
        raise SystemExit("error: --scaffold oneshot needs an explicit "
                         "--model")
    skill_text = neutrongym.skill_text() if use_skill else None
    episode = agent.run_oneshot(
        task["prompt"], model, episode_dir=ep, skill_text=skill_text,
        provider_pin=provider or None)
    src = agent.extract_instr(episode.get("final_answer") or "")
    if src:
        import re as _re
        m = _re.search(r"DEFINE\s+INSTRUMENT\s+(\w+)", src)
        name = m.group(1) if m else "oneshot_instr"
        d = os.path.join(ep, "home", "instruments", name)
        os.makedirs(d, exist_ok=True)
        instr_path = os.path.join(d, f"{name}.instr")
        with open(instr_path, "w") as f:
            f.write(src)
        params = [{"name": k, "default": v} for k, v in
                  executor.read_define_params(instr_path).items()]
        with open(os.path.join(d, "spec.json"), "w") as f:
            json.dump({"name": name, "parameters": params}, f, indent=1)
    return episode


def run_agent(task: dict, model: str | None, ep: str, use_skill: bool) -> dict:
    home = os.path.join(ep, "home")
    cwd = os.path.join(ep, "cwd")
    os.makedirs(home, exist_ok=True)
    os.makedirs(cwd, exist_ok=True)
    claude_dir = os.path.join(cwd, ".claude")
    os.makedirs(claude_dir, exist_ok=True)
    with open(os.path.join(claude_dir, "settings.json"), "w") as f:
        json.dump({"permissions": {"deny": sandbox_deny_rules()}}, f, indent=1)
    if use_skill:
        skills = os.path.join(claude_dir, "skills")
        os.makedirs(skills, exist_ok=True)
        link = os.path.join(skills, "mcstas-instrument-design")
        if not os.path.exists(link):
            os.symlink(os.path.join(REPO, "skills", "mcstas-instrument-design"),
                       link)

    env = dict(os.environ, MCSTAS_MCP_HOME=home, MCSTAS_MCP_BENCHMARK="1")
    exempt = task_exempt_paths(task)
    if exempt:
        env["MCSTAS_MCP_BENCHMARK_ALLOW"] = os.pathsep.join(exempt)
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
    ap.add_argument("task", help="task id (e.g. T1_PSI_DMC) or task-JSON path")
    ap.add_argument("--scaffold", choices=["loop", "claude", "oneshot"],
                    default="loop",
                    help="loop = NeutronGym reference loop (the measurement "
                         "instrument, default); claude = headless Claude "
                         "Code (production-harness comparison arm); "
                         "oneshot = plain-LLM baseline (no tools, one "
                         "completion)")
    ap.add_argument("--model", default=None,
                    help="loop: any OpenRouter id / claude* / id+--base-url; "
                         "claude scaffold: OpenRouter id or Claude alias "
                         "(default = local claude default)")
    ap.add_argument("--no-skill", action="store_true")
    ap.add_argument("--episode-dir", default=None)
    ap.add_argument("--no-artifacts", action="store_true",
                    help="skip the post-episode visual bundle")
    ap.add_argument("--provider", default="Google",
                    help="loop only: pin one OpenRouter serving provider, "
                         "no fallbacks (consistency default: Google/Vertex; "
                         "non-Vertex models need their provider from "
                         "note/openrouter-model-roster-2026-08-05.md; "
                         "'' = unpinned)")
    ap.add_argument("--max-turns", type=int, default=None,
                    help="loop turn cap (M6 protocol pins 50 via "
                         "benchmark/m6_config.json; default = loop default)")
    args = ap.parse_args()
    with open(resolve_task(args.task)) as f:
        task = json.load(f)

    tag = re.sub(r"\W", "_", args.model or "claude")
    if args.scaffold != "claude":
        tag = f"{args.scaffold}__{tag}"  # keep arms' episode dirs distinct
    ep = args.episode_dir or os.path.join(REPO, "runs", "pilot",
                                          f"{task['id']}__{tag}")
    # MCSTAS_MCP_HOME reaches the MCP server via env and the server runs with
    # a different cwd — a relative episode dir would scatter the registry
    ep = os.path.abspath(ep)
    shutil.rmtree(ep, ignore_errors=True)
    os.makedirs(ep)

    if args.scaffold == "loop":
        episode = run_agent_loop(task, args.model, ep,
                                 use_skill=not args.no_skill,
                                 provider=args.provider,
                                 max_turns=args.max_turns)
    elif args.scaffold == "oneshot":
        episode = run_agent_oneshot(task, args.model, ep,
                                    use_skill=not args.no_skill,
                                    provider=args.provider)
    else:
        episode = run_agent(task, args.model, ep,
                            use_skill=not args.no_skill)

    instr_path, params = find_candidate(os.path.join(ep, "home"))
    if instr_path is None:
        report = {"task": task["id"], "pass": False, "score": 0.0,
                  "hard_failures": ["agent left no built instrument in the "
                                    "episode registry"], "checks": []}
    elif task.get("kind") == "improve":
        # T2: FOM vs calibrated target + constraint bands — no reference
        # comparison (grade_improvement path; first exercised by the M6
        # matrix — the pilots were all reproduce tasks)
        cand = grader.run_protocol(instr_path, params or {}, task["protocol"],
                                   os.path.join(ep, "grade_work"), "cand")
        report = grader.grade_improvement(task, cand)
        report["candidate_instr"] = instr_path
        report["candidate_params"] = params
    else:
        ref = grader.reference_summary(task)
        cand = grader.run_protocol(instr_path, params or {}, task["protocol"],
                                   os.path.join(ep, "grade_work"), "cand")
        report = grader.grade(task, cand, ref)
        report["candidate_instr"] = instr_path
        report["candidate_params"] = params

    # sandbox layer 3: mandatory leak audit — a leaked episode is INVALID
    # regardless of score
    audit = leak_audit.audit_transcript(
        os.path.join(ep, "transcript.jsonl"),
        allowed_roots=[os.path.join(ep, "home"), os.path.join(ep, "cwd")],
        forbidden_trees=leak_audit.default_forbidden_trees(REPO),
        exempt=task_exempt_paths(task))

    # artifacts/ bundle: harness-rendered from the instrument the agent
    # actually built (never from claims); side-by-side partner in runs/refviz/
    artifacts = None
    if instr_path and not args.no_artifacts:
        artifacts = visualize.bundle(instr_path, params or {},
                                     os.path.join(ep, "artifacts"))

    out = {"task": task["id"], "episode": episode, "grade": report,
           "reference_leak": audit, "artifacts": artifacts}
    with open(os.path.join(ep, "report.json"), "w") as f:
        json.dump(out, f, indent=2)
    print(json.dumps({**out, "episode": {k: v for k, v in episode.items()
                                         if k != "final_answer"}}, indent=2))
    verdict = ("INVALID (reference leak)" if audit["leaked"]
               else "PASS" if report["pass"] else "FAIL")
    print(f"\n{verdict}  score={report['score']}  "
          f"({report.get('checks_passed', '0/0')})  report: {ep}/report.json")


if __name__ == "__main__":
    main()
