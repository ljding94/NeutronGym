"""The NeutronGym reference loop — the measurement instrument.

A minimal, model-agnostic agent scaffold (note/scaffold-decision-2026-07-30.md):
every headline cross-model number, M8 rejection-sampling rollout, and
trained-model evaluation runs through THIS loop, so baseline and trained
models share one scaffold (no scaffold confound under the trainability
claim). Claude Code is a separate comparison arm.

Design:
- One OpenAI-compatible chat-completions client (httpx): OpenRouter and
  local vLLM plug in directly; Claude rides Anthropic's OpenAI-compat
  surface. Backend resolved from the model id unless base_url is given.
- MCP client over stdio to the mcstas FastMCP server, spawned per episode
  with MCSTAS_MCP_HOME (isolation) and MCSTAS_MCP_BENCHMARK=1 (sandbox
  layer 1). The MCP tool list IS the capability surface.
- Sandbox by construction: the model has no shell and no file Read — only
  MCP tools exist. The transcript audit (sandbox layer 3) stays as backstop.
- Skill = SKILL.md text injected into the system prompt by the caller
  (±skill stays a clean ablation toggle). The system prompt below is part
  of the released instrument — deliberately short and pinned.
- Transcript events mirror Claude Code's stream-json block shapes
  (assistant tool_use / user tool_result / final result), so the existing
  episode parsers and benchmark/harness/leak_audit.py consume loop
  transcripts unchanged.
"""

import asyncio
import json
import os
import sys
import time

import httpx

DEFAULT_MAX_TURNS = 40
TOOL_RESULT_CAP = 50_000
REQUEST_TIMEOUT_S = 300
# Locally served small models generate long files far slower than APIs:
# a 32k-token one-shot on an 8B at single-stream speed is 5-10 min, which
# blew the 300 s ceiling as a ReadTimeout and scored as INFRA (2026-09-10).
# M8 rollouts would hit the same wall.
LOCAL_REQUEST_TIMEOUT_S = 1200
RETRIES = 3

SYSTEM_PROMPT = (
    "You are an expert neutron instrument scientist. You design and validate "
    "McStas instruments exclusively through the provided tools. Iterate at "
    "low ncount; when your instrument runs and matches the task, stop and "
    "summarize what you built and its key simulated observables."
)


def resolve_backend(model: str, base_url: str | None = None,
                    api_key: str | None = None) -> tuple[str, str]:
    """(base_url, api_key) from an explicit base_url, or the model id:
    'vendor/model' -> OpenRouter; 'claude*' -> Anthropic OpenAI-compat."""
    if base_url:
        return base_url.rstrip("/"), (api_key or os.environ.get(
            "OPENAI_API_KEY") or "EMPTY")
    if model.startswith("claude"):
        key = api_key or os.environ.get("ANTHROPIC_API_KEY")
        if not key:
            raise RuntimeError("Claude via the reference loop needs "
                               "ANTHROPIC_API_KEY (subscription auth only "
                               "works through the claude CLI comparison arm)")
        return "https://api.anthropic.com/v1", key
    if "/" in model:
        key = api_key or os.environ.get("OPENROUTER_API_KEY")
        if not key:
            raise RuntimeError("OPENROUTER_API_KEY not set")
        return "https://openrouter.ai/api/v1", key
    raise ValueError(f"cannot infer a backend for model {model!r} — pass "
                     "base_url= (e.g. a local vLLM http://host:8000/v1)")


def mcp_tools_to_openai(tools) -> list:
    return [{"type": "function",
             "function": {"name": t.name, "description": t.description or "",
                          "parameters": t.inputSchema
                          or {"type": "object", "properties": {}}}}
            for t in tools]


def _truncate(text: str, cap: int = TOOL_RESULT_CAP) -> str:
    if len(text) <= cap:
        return text
    return text[:cap] + f"\n...[truncated {len(text) - cap} chars]"


def chat_completion(http: httpx.Client, base_url: str, api_key: str,
                    model: str, messages: list, tools: list,
                    temperature: float, provider_pin: str | None = None,
                    max_tokens: int = 8192,
                    chat_extra: dict | None = None,
                    timeout: int = REQUEST_TIMEOUT_S) -> dict:
    payload = {"model": model, "messages": messages,
               "tool_choice": "auto", "temperature": temperature,
               "max_tokens": max_tokens}  # explicit: providers can truncate
    if chat_extra:
        # extra request-body fields (e.g. Qwen3's chat_template_kwargs
        # enable_thinking=false — the pinned non-thinking baseline config;
        # vLLM 0.28 has no server-side flag, per-request only)
        payload.update(chat_extra)
    if tools:
        payload["tools"] = tools
    else:
        payload.pop("tool_choice")
    if provider_pin:
        # one serving provider per model, no fallbacks — provider drift
        # between episodes is an evaluation confound (measured 2026-08-05:
        # unpinned claude-sonnet-5 moved Google -> Bedrock within an hour)
        payload["provider"] = {"order": [provider_pin],
                               "allow_fallbacks": False}
    last = None
    for attempt in range(RETRIES):
        try:
            resp = http.post(f"{base_url}/chat/completions",
                             headers={"Authorization": f"Bearer {api_key}"},
                             json=payload, timeout=timeout)
        except httpx.HTTPError as e:  # timeouts, resets — retry, never raise raw
            last = f"{type(e).__name__}: {e}"
            time.sleep(2 ** attempt)
            continue
        if resp.status_code == 200:
            data = resp.json()
            if data.get("choices"):
                return data
            # 200 with an error body (provider hiccup) — retryable
            last = f"200 without choices: {str(data)[:300]}"
            time.sleep(2 ** attempt)
            continue
        last = f"HTTP {resp.status_code}: {resp.text[:300]}"
        if resp.status_code in (429, 500, 502, 503):
            time.sleep(2 ** attempt)
            continue
        break
    raise RuntimeError(f"chat completion failed after {RETRIES} tries: {last}")


class Transcript:
    """JSONL transcript in Claude stream-json block shapes (see module doc)."""

    def __init__(self, path: str):
        self.f = open(path, "w")

    def event(self, ev: dict):
        self.f.write(json.dumps(ev) + "\n")
        self.f.flush()

    def close(self):
        self.f.close()


async def _run(task_prompt, model, episode_dir, home_dir, server_cwd,
               skill_text, base_url, api_key, max_turns, temperature,
               benchmark_mode, exempt, provider_pin, chat_extra,
               chat_fn) -> dict:
    from fastmcp import Client
    from fastmcp.client.transports import StdioTransport

    os.makedirs(episode_dir, exist_ok=True)
    home_dir = home_dir or os.path.join(episode_dir, "home")
    server_cwd = server_cwd or os.path.join(episode_dir, "cwd")
    os.makedirs(home_dir, exist_ok=True)
    os.makedirs(server_cwd, exist_ok=True)

    env = {**os.environ, "MCSTAS_MCP_HOME": os.path.abspath(home_dir)}
    if benchmark_mode:
        env["MCSTAS_MCP_BENCHMARK"] = "1"
    if exempt:
        env["MCSTAS_MCP_BENCHMARK_ALLOW"] = os.pathsep.join(exempt)
    transport = StdioTransport(
        command=sys.executable,
        args=["-c", "from mcstas_mcp.server import main; main()"],
        env=env, cwd=os.path.abspath(server_cwd))

    tr = Transcript(os.path.join(episode_dir, "transcript.jsonl"))
    system = SYSTEM_PROMPT + ("\n\n" + skill_text if skill_text else "")
    messages = [{"role": "system", "content": system},
                {"role": "user", "content": task_prompt}]
    usage = {"prompt_tokens": 0, "completion_tokens": 0}
    tool_counts: dict = {}
    final = ""
    t0 = time.time()

    async with Client(transport) as mcp:
        tools = mcp_tools_to_openai(await mcp.list_tools())
        tr.event({"type": "system", "subtype": "init", "model": model,
                  "scaffold": "neutrongym-reference-loop",
                  "config": {"max_turns": max_turns,
                             "temperature": temperature,
                             "benchmark_mode": benchmark_mode,
                             "skill": bool(skill_text),
                             "n_tools": len(tools)}})
        with httpx.Client() as http:
            if chat_fn is None:
                b_url, key = resolve_backend(model, base_url, api_key)

                req_timeout = (LOCAL_REQUEST_TIMEOUT_S if base_url
                               else REQUEST_TIMEOUT_S)

                def call_model(msgs, tls):
                    return chat_completion(http, b_url, key, model, msgs,
                                           tls, temperature, provider_pin,
                                           chat_extra=chat_extra,
                                           timeout=req_timeout)
            else:
                call_model = chat_fn

            turns = empties = 0
            error = None
            providers_seen: set = set()
            while turns < max_turns:
                turns += 1
                try:
                    resp = call_model(messages, tools)
                except RuntimeError as e:
                    # unrecoverable API failure: record an infra-failed
                    # episode, never lose the transcript to a stack trace
                    error = str(e)
                    tr.event({"type": "error", "error": error})
                    break
                for k in usage:
                    usage[k] += (resp.get("usage") or {}).get(k) or 0
                choice = resp["choices"][0]
                msg = choice["message"]
                calls = msg.get("tool_calls") or []
                blocks = ([{"type": "text", "text": msg["content"]}]
                          if msg.get("content") else [])
                blocks += [{"type": "tool_use", "id": c["id"],
                            "name": c["function"]["name"],
                            "input": json.loads(
                                c["function"]["arguments"] or "{}")}
                           for c in calls]
                providers_seen.add(resp.get("provider") or "?")
                tr.event({"type": "assistant", "message": {"content": blocks},
                          "finish_reason": choice.get("finish_reason"),
                          "provider": resp.get("provider"),
                          # per-request usage: peak single-request context
                          # is a first-class observable (report.json only
                          # holds the cumulative sum — peer-review gap,
                          # 2026-09-07)
                          "usage": resp.get("usage")})
                messages.append({k: v for k, v in msg.items()
                                 if k in ("role", "content", "tool_calls")})
                if not calls:
                    final = msg.get("content") or ""
                    # SYMMETRY FIX (2026-09-10, peer review): a model that
                    # answers in PROSE having never touched a tool has not
                    # attempted the task — it must be nudged exactly like an
                    # empty turn. Previously prose-without-tools ended the
                    # episode on turn 1 unnudged, which scored such models
                    # L0 "never called a tool" without ever asking twice
                    # (maverick: all 17 loop episodes, turns=1). Text AFTER
                    # tools have been used is a legitimate final answer.
                    if final.strip() and not tool_counts:
                        empties += 1
                        if empties < 3:
                            messages.append({
                                "role": "user",
                                "content": "You have not used any tools yet. "
                                           "You must build the instrument "
                                           "with the provided tools — "
                                           "describing it in text does not "
                                           "count. Call a tool now."})
                            continue
                    if not final.strip():
                        # reasoning-only / truncated turn (seen with Gemini
                        # via OpenRouter): nudge instead of accepting an
                        # empty episode as the model's answer
                        empties += 1
                        if empties >= 3:
                            break
                        messages.append({
                            "role": "user",
                            "content": "Your last message was empty. "
                                       "Continue the task: call a tool, or "
                                       "give your final summary as text."})
                        continue
                    break
                results = []
                for c in calls:
                    name = c["function"]["name"]
                    tool_counts[name] = tool_counts.get(name, 0) + 1
                    try:
                        args = json.loads(c["function"]["arguments"] or "{}")
                        out = await mcp.call_tool(name, args,
                                                  raise_on_error=False)
                        text = "\n".join(b.text for b in out.content
                                         if getattr(b, "text", None))
                        is_err = bool(out.is_error)
                    except Exception as e:  # malformed args, transport, ...
                        text, is_err = f"tool call failed: {e}", True
                    text = _truncate(text)
                    results.append({"type": "tool_result",
                                    "tool_use_id": c["id"],
                                    "is_error": is_err,
                                    "content": [{"type": "text",
                                                 "text": text}]})
                    messages.append({"role": "tool", "tool_call_id": c["id"],
                                     "content": text})
                tr.event({"type": "user", "message": {"content": results}})

    duration = time.time() - t0
    result = {"type": "result", "result": final, "num_turns": turns,
              "usage": usage, "duration_ms": int(duration * 1000),
              "total_cost_usd": None}
    tr.event(result)
    tr.close()
    return {"model": model, "scaffold": "neutrongym-reference-loop",
            "provider_pin": provider_pin,
            "providers_seen": sorted(providers_seen),
            "returncode": 1 if error else 0, "error": error, "turns": turns,
            "hit_turn_cap": turns >= max_turns and not final,
            "empty_responses": empties,
            "cost_usd": None, "duration_s": round(duration, 1),
            "usage": usage, "mcp_calls": tool_counts,
            "skill_used": bool(skill_text), "final_answer": final}


ONESHOT_SYSTEM = (
    "You are an expert neutron instrument scientist. Answer with a single "
    "complete McStas 3.x instrument file (.instr) implementing the "
    "requested instrument, in ONE ```-fenced code block, and nothing else. "
    "You have no tools; the file will be compiled and simulated as-is.")


def run_oneshot(task_prompt: str, model: str, episode_dir: str,
                skill_text: str | None = None, base_url: str | None = None,
                api_key: str | None = None, temperature: float = 0.0,
                provider_pin: str | None = None,
                chat_extra: dict | None = None, chat_fn=None) -> dict:
    """The plain-LLM baseline arm (M6): ONE chat completion, no tools —
    anchors what the env/tooling infrastructure adds over raw generation.
    Writes the same stream-json-shaped transcript; the harness grades the
    emitted .instr through the identical tail."""
    os.makedirs(episode_dir, exist_ok=True)
    tr = Transcript(os.path.join(episode_dir, "transcript.jsonl"))
    system = ONESHOT_SYSTEM + ("\n\n" + skill_text if skill_text else "")
    messages = [{"role": "system", "content": system},
                {"role": "user", "content": task_prompt}]
    tr.event({"type": "system", "subtype": "init", "model": model,
              "scaffold": "plain-llm-oneshot",
              "config": {"temperature": temperature,
                         "skill": bool(skill_text)}})
    t0 = time.time()
    usage = {"prompt_tokens": 0, "completion_tokens": 0}
    error, answer, providers = None, "", set()
    try:
        for attempt in range(2):
            if chat_fn is None:
                b_url, key = resolve_backend(model, base_url, api_key)
                with httpx.Client() as http:
                    # generous budget: reasoning models spend heavily on
                    # thinking BEFORE the file (sonnet-5 burned 8192 tokens
                    # of pure reasoning in the shakedown -> empty answer)
                    resp = chat_completion(
                        http, b_url, key, model, messages, [], temperature,
                        provider_pin, max_tokens=32000,
                        chat_extra=chat_extra,
                        timeout=(LOCAL_REQUEST_TIMEOUT_S if base_url
                                 else REQUEST_TIMEOUT_S))
            else:
                resp = chat_fn(messages, [])
            for k in usage:
                usage[k] += (resp.get("usage") or {}).get(k) or 0
            providers.add(resp.get("provider") or "?")
            choice0 = resp["choices"][0]
            answer = choice0["message"].get("content") or ""
            tr.event({"type": "assistant",
                      "message": {"content": [{"type": "text",
                                               "text": answer}]},
                      # finish_reason was missing on the one-shot path, so
                      # truncation could not be ruled out by inspection
                      # (peer review 2026-09-10) — needed before M8.
                      "finish_reason": choice0.get("finish_reason"),
                      "usage": resp.get("usage"),
                      "provider": resp.get("provider")})
            if answer.strip():
                break
            messages.append({"role": "assistant", "content": ""})
            messages.append({"role": "user",
                             "content": "Your answer was empty. Output the "
                                        "complete .instr file now, code "
                                        "only."})
    except RuntimeError as e:
        error = str(e)
        tr.event({"type": "error", "error": error})
    duration = time.time() - t0
    tr.event({"type": "result", "result": answer, "num_turns": 1,
              "usage": usage, "duration_ms": int(duration * 1000),
              "total_cost_usd": None})
    tr.close()
    return {"model": model, "scaffold": "plain-llm-oneshot",
            "provider_pin": provider_pin, "providers_seen": sorted(providers),
            "returncode": 1 if error else 0, "error": error, "turns": 1,
            "hit_turn_cap": False, "empty_responses": 0, "cost_usd": None,
            "duration_s": round(duration, 1), "usage": usage,
            "mcp_calls": {}, "skill_used": bool(skill_text),
            "final_answer": answer}


def extract_instr(answer: str) -> str | None:
    """The .instr source from a one-shot answer: the largest fenced block
    containing DEFINE INSTRUMENT, else the raw answer if it qualifies."""
    import re

    blocks = re.findall(r"```[a-zA-Z]*\n(.*?)```", answer, re.DOTALL)
    blocks = [b for b in blocks if "DEFINE INSTRUMENT" in b]
    if blocks:
        return max(blocks, key=len).strip() + "\n"
    if "DEFINE INSTRUMENT" in answer:
        return answer.strip() + "\n"
    return None


def run_episode(task_prompt: str, model: str, episode_dir: str,
                home_dir: str | None = None, server_cwd: str | None = None,
                skill_text: str | None = None, base_url: str | None = None,
                api_key: str | None = None,
                max_turns: int = DEFAULT_MAX_TURNS,
                temperature: float = 0.0, benchmark_mode: bool = True,
                exempt=(), provider_pin: str | None = None,
                chat_extra: dict | None = None, chat_fn=None) -> dict:
    """Run one reference-loop episode; returns the episode meta dict and
    writes transcript.jsonl into episode_dir. provider_pin routes every
    request through one OpenRouter provider (no fallbacks) — the
    consistency default for scored runs. chat_fn(messages, tools) may be
    injected for tests (scripted model, no network)."""
    return asyncio.run(_run(task_prompt, model, episode_dir, home_dir,
                            server_cwd, skill_text, base_url, api_key,
                            max_turns, temperature, benchmark_mode,
                            tuple(exempt), provider_pin, chat_extra,
                            chat_fn))
