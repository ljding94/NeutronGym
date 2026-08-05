# Scaffold decision: NeutronGym reference loop + Claude Code as comparison arm (2026-07-30)

**Status: adopted (user decision, hybrid option).** Supersedes SPEC §7's
"headless Claude Code as a universal scaffold across model tiers" where they
differ. Context: the benchmark needs an agent harness that hosts the LLM with
MCP + skill in a sandbox; we asked whether Claude Code should remain that
harness and what the standard practice is.

## Decision

1. **Build the NeutronGym reference loop** — a minimal (~300-line),
   model-agnostic agent scaffold shipped inside the environment — and use it
   for **all headline cross-model M6 numbers, M8 rejection-sampling rollouts,
   and trained-model evaluation**. Design points:
   - OpenAI-compatible chat-completions client → OpenRouter and local vLLM
     plug in directly (no LiteLLM translation tier); Claude via the
     OpenAI-compat surface or a thin Anthropic adapter, same loop.
   - MCP client speaking to the existing FastMCP server (stdio); the tool
     list IS the capability surface.
   - Skill = the `mcstas-instrument-design` SKILL.md injected as system-prompt
     text by the harness (±skill stays a clean ablation toggle).
   - **Sandbox by construction:** the agent has no shell, no filesystem Read,
     no example-browsing beyond what MCP exposes. Server-side benchmark mode
     (`MCSTAS_MCP_BENCHMARK=1`, leak-audit note) is STILL required — the
     example tools are MCP tools — but the Read/`load_instr_file` path
     channels vanish. Transcript audit (layer 3) stays as the backstop.
   - JSON transcript, token/tool-call/wall-clock accounting, turn cap, fixed
     seeds — same episode-folder contract as today.
2. **Claude Code becomes a comparison arm, not the instrument:** kept on
   subscription Claude only ($0), answering "how much does a production
   agentic harness add over the minimal loop?" — connects the existing pilot
   data and exercises the skill mechanism as designed. **Cuttable if behind**
   (added to the M7 cut order, first position).

## Why

- **RL scaffold parity (the forcing argument):** GRPO generation cannot run
  through Claude Code, so the trained model would be evaluated in a custom
  loop while its untrained baseline was measured in Claude Code — a scaffold
  confound sitting exactly under the paper's trainability claim. One
  reference loop for baseline AND trained model removes it. The M8
  agentic-RL-framework survey item stands, with a new hard requirement:
  the chosen framework must consume the reference loop or replicate its
  exact prompt/tool format for in-training generation.
- **Field standard:** agent benchmarks (SWE-bench/mini-swe-agent, tau-bench,
  WebArena, MLE-bench, Terminal-Bench) ship a minimal model-agnostic
  reference scaffold in-repo; proprietary harnesses appear as comparison
  arms. Reviewers expect this shape.
- **Neutrality + reproducibility:** Claude Code is closed-source,
  auto-updating, opaque-prompted, and Anthropic-flavored — a scaffold-bias
  critique magnet for GPT/Qwen numbers, and its large system prompt likely
  understates small 7–8B baselines (the RL-claim baselines!). The reference
  loop is pinned, open-source, and released with the env.
- **Considered and not chosen:** Inspect AI (community-standard eval infra;
  ~1 week port, our orchestration/grading already exists, M8 still needs a
  custom loop — revisit as a release/packaging option post-deadline);
  OpenHands / SWE-agent-class scaffolds (coding-tuned, same confound, heavier);
  Claude Code status quo (accepts the RL confound; rejected).

## Consequences

- **M5** gains the reference-loop work item (target ~Aug 14, after the reward
  API): loop + shakedown on P1/P3 pilot tasks across one Claude + one
  OpenRouter + one vLLM model before M6 opens (~Aug 17).
- **M6 scaffold line** rewritten: reference loop is the instrument
  everywhere; `claude -p` moves to the comparison arm. The open-weights tier
  drops LiteLLM (direct vLLM). Expect reference-loop scores below Claude
  Code at equal model — that is the honest, defensible number, and the gap
  itself is a paper datapoint.
- **M8**: rejection-sampling rollouts and all trained-vs-baseline evals go
  through the reference loop; framework survey inherits the format-parity
  requirement.
- Pilot episodes (Claude Code) remain valid for the comparison arm and
  mechanics validation; headline-number shakedown is fresh.
- Docs updated: PLAN.md (M5/M6/M7/M8, standing decisions, recap), CLAUDE.md
  framing, SCOPE.md deliverables table.
