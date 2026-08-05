# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project status

**M0–M4 complete; benchmark harness validated (pilot episodes 4/4 mechanics-PASS — capability signal caveated by the 2026-07-30 reference-leak audit, `note/pilot-leak-audit-2026-07-30.md`; 14 T1 + 2 T2 tasks). Current milestone: M5 (NeutronGym environment build), targeting ICLR 2027 (full paper 2026-09-24 AoE).** Sources of truth, in order:

- `note/mcstas-mcp-feasibility-and-spec.md` (2026-07-08) — feasibility analysis, full SPEC, research plan.
- `note/scope-decision-2026-07-09.md` — scope/framing decision; supersedes §7 framing in the spec where they differ.
- `note/scope-evolution-rl-env-2026-07-24.md` — environment-first reframing (RL env with three-tier reward ladder + procedural generation; benchmark = held-out slice) + M8 RL track; supersedes the 07-09 note's framing where they differ.
- `note/neutrongym-vision-digest-2026-07-26.md` — naming (**NeutronGym** = the environment/headline artifact; **McStasBench** = its held-out benchmark slice; repo + local dir renamed 2026-07-26) + **venue commitment: ICLR 2027** (abstract 2026-09-19, full paper 2026-09-24 AoE) + adopted/rejected items from the external vision note (L1–L4 presentation of the reward ladder, plain-LLM baseline arm, Figure-1 discipline) + **same-day addendum: bench AND RL are both ICLR paper content** (SFT Aug 11 – Sep 1 → GRPO Sep 1 – 15 on 7×A100-40G → RL numbers frozen ~Sep 17; supersedes the note's earlier include-if-signal policy); supersedes earlier notes on naming and venue where they differ.
- `note/scaffold-decision-2026-07-30.md` — the benchmark's measurement instrument is the NeutronGym reference loop (minimal model-agnostic scaffold, sandbox-by-construction); Claude Code demoted to comparison arm; supersedes spec §7's scaffold. Same-day `note/pilot-leak-audit-2026-07-30.md` — reference-leak audit caveating the pilot passes + sandbox fix design.
- `SCOPE.md` — the goal/scope anchor: what the work is, what it claims (with wording rules), in/out-of-scope boundaries, non-negotiables. Read it first for orientation; it is a summary — when it disagrees with a newer dated note, the note wins and SCOPE.md needs updating.
- `PLAN.md` — the concrete implementation plan: milestones M0–M8 with acceptance criteria, de-risk gates, and standing decisions. This is the living document; check items off there. Its `## Recap` block is the user's quick-sync view (rendered as the top card of progress.html) — refresh its bullets and as-of date whenever project status materially changes.
- `note/m1-server-design-2026-07-09.md` — the M1 server design (construction-vs-execution split, 10 server-side rules, revised tool signatures, benchmark spillover). Grounded in three systematic studies: `note/study-mcstas-software-*.md`, `note/study-mcstasscript-api-*.md`, `note/study-instrument-papers-*.md` — consult these before touching McStas/McStasScript integration code; they contain verified error behaviors and gotchas.
- `progress.html` — human-friendly dashboard generated from PLAN.md. **After editing PLAN.md, regenerate it**: `python3 scripts/progress_report.py` (stdlib only, any python). Never edit progress.html by hand.
- `guide.html` — human-friendly explainer: agent→MCP→mcstas pipeline, tool reference (introspected from the live server), how to read results. **After changing server tools, regenerate it**: `conda run -n mcstas python scripts/guide_report.py`. Never edit guide.html by hand.
- `benchmark/tasks.html` — human-friendly benchmark task catalog (provenance chain, per-task references with GitHub links, papers, grading contracts, prompts). **After changing benchmark tasks, regenerate it**: `python3 scripts/tasks_report.py` (stdlib only). Never edit it by hand.

Read these before doing any design or implementation work here; keep them updated when decisions change.

## What this project is

A research program around LLM agents designing neutron instruments with [McStas](https://www.mcstas.org/) (a Monte Carlo ray-tracing simulator whose instruments are text-based `.instr` files).

**Framing (evolved 2026-07-24, named + venue-committed 2026-07-26): NeutronGym is the paper and headline contribution — an executable, physically-verifiable RL environment for neutron instrument design, targeted at ICLR 2027 (full paper 2026-09-24 AoE). McStasBench is its held-out benchmark slice; McStasAgent (MCP server + skill) is the reference baseline shipped inside it**, not a standalone contribution: an agent-tooling paper alone reads as commoditized engineering, and the agent needs the environment to prove anything. The agent's failure modes — level-resolved, L1 syntax through L4 science — are the analysis section of the paper.

Deliverables:

1. **NeutronGym** (headline) — executable RL environment (reward ladder, procedural generation, anti-hacking checks) whose held-out slice, **McStasBench**, is the tiered evaluation (reproduce / optimize / open-ended design) run through the **NeutronGym reference loop** — a minimal model-agnostic scaffold shipped in the env — across model tiers; headless Claude Code is a production-harness comparison arm (`note/scaffold-decision-2026-07-30.md`, supersedes spec §7's scaffold). Benchmark quality bars are load-bearing: contamination controls, difficulty tiers, pipeline-decomposed metrics.
2. **`mcstas-mcp` server** (baseline tooling) — Python (FastMCP) wrapping [McStasScript](https://github.com/PaNOSC-ViNYL/McStasScript): component discovery/introspection, structured instrument construction, async simulation jobs, parameter scans, scipy-driven optimization. Tool signatures in spec §4.
3. **`mcstas-instrument-design` skill** (baseline tooling) — the judgment layer: units/conventions, figures of merit, instrument archetypes, verification checklist (spec §5). Canonical copy: `skills/mcstas-instrument-design/` (loaded into local sessions via the committed `.claude/skills/` symlink). Rules distilled from real agent transcripts live in SKILL.md and are regression-tested (`tests/test_skill.py`) — when an eval transcript shows a new recurring mistake, add a rule AND extend that test.

## Key architecture decisions (from the spec — don't relitigate without reason)

- **Structured construction is primary, raw `.instr` text is the escape hatch.** Every mutating tool call validates immediately against component metadata — fail at tool-call time, not compile time.
- **Async job model** for simulations (`run_simulation` → `job_status` → `get_results`); iterate at low `ncount` (1e5–1e6), production-validate at ≥1e8. Never compare designs on monitors with <1000 events.
- **`get_results` returns summary statistics** (intensity, error, event counts, peak, CoM, FWHM), not raw arrays; agents request PNGs or slices when needed.
- **Server-side scipy optimization** for continuous parameters; agent reasoning reserved for topology choices.
- Target **McStas 3.x only**; state persists in `~/.mcstas-mcp/projects/<name>/`.
- Benchmark requires **contamination controls**: held-out recent instruments, memorization probes, perturbed variants (§7, Study A).

## Environment (working, verified 2026-07-09)

Conda env `mcstas`: McStas 3.7.12 (conda-forge, osx-arm64) + McStasScript 0.0.84 + NCrystal 4.4.6 (required by PowderN-class components) + fastmcp + pytest + ply (needed by `mcdisplay-*` visualization). MPI available (Homebrew Open MPI on PATH). Instrument visualization — use the friendly wrapper: `conda run -n mcstas python scripts/view_instrument.py <file.instr | shipped-example-name> [param=value ...]` (validates parameters, uses defaults automatically, keeps build artifacts in `runs/_view/`, opens browser). Add `--diagram` for the 2D component-connection schematic PNG (McStasScript `show_diagram`; falls back gracefully where the .instr reader fails). Raw tool underneath: `mcdisplay-webgl`. This conda-forge install is the single McStas on this machine — do NOT add the official macOS app bundle alongside it (duplicate toolchains/PATH conflicts). Run anything McStas-related via `conda run -n mcstas ...`. Smoke test: `conda run -n mcstas python scripts/m0_smoke_test.py` (three stages: mcrun CLI → McStasScript data load → programmatic build; also documents two mcrun gotchas — interactive parameter prompting and CWD compilation). Simulation outputs go in `runs/` (gitignored).

## Where to start

Follow `PLAN.md` — milestones M0–M8 with acceptance criteria and dates. M0–M4 (server, robustness, skill, optimization layer) are done. Current ordering: M5 environment build — reward-ladder API + env executor + minimal procedural generator first (~Aug 10, the RL critical path) — then M6 evaluation runs and the M8 RL track in parallel (SFT → GRPO on the 7×A100s), then M7 analysis + ICLR paper writing (full paper deadline 2026-09-24 AoE).
