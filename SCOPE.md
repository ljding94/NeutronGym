# NeutronGym — Goal & Scope

*The anchor document: what this work is, what it claims, and where its edges are.
Stable by design — it changes only when a dated decision note changes it.
The **how/when** lives in `PLAN.md` (milestones, acceptance criteria, gates);
day-to-day conventions live in `CLAUDE.md`. Last updated 2026-07-29.*

## Goal

**Build NeutronGym: a fast, physically-verifiable, inverse-design RL environment
for LLM agents — and prove it works as both an evaluation and a training
substrate in one ICLR 2027 paper.** Neutron optics is the substrate, McStas the
simulation backend. The environment is procedurally generated, densely rewarded
(a three-tier reward ladder, presented as L1 syntax → L2 runtime → L3 structural
→ L4 scientific), and trainable. **McStasBench** — the held-out benchmark slice —
falls out of it; **McStasAgent** (the `mcstas-mcp` server + design skill) is the
reference baseline shipped inside it, not a standalone contribution.

## The three research questions (every task must serve one)

1. **Reproduction** — can an LLM agent reproduce a neutron instrument from its
   instrument-design paper, graded on simulation observables? *(curated,
   non-generatable held-out slice)*
2. **Improvement** — can an LLM agent improve a design against quantitative
   target specs, measured against classical baselines under matched compute?
   *(the trainable core — procedurally generated)*
3. **Trainability** — does physics-verifiable reward train? *(rejection
   sampling → filtered SFT → GRPO on a 7–8B model)*

## What ships

| Artifact | What it is |
|---|---|
| **NeutronGym** (headline) | Gym-style env: reward-ladder API with level-resolved output, procedural instance generator, anti-hacking checks, fast tier at ~25 rollouts/s/core; pip-installable (Docker deferred to camera-ready — trim 2026-07-29) |
| **McStasBench** | Tiered eval slice: T1 reproduce / T2 improve / T3 open design, with contamination controls (held-out 2024–26 instruments, memorization probes, perturbed variants) |
| **McStasAgent** | Reference baseline: 22-tool MCP server wrapping McStasScript + the `mcstas-instrument-design` skill; run through the **NeutronGym reference loop** (minimal model-agnostic scaffold shipped in the env — the measurement instrument for all headline numbers; Claude Code is a comparison arm, decided 2026-07-30) |
| **ICLR 2027 paper** | Bench AND RL as co-equal content: eval matrix across model tiers + the small-model trainability result, with level-resolved failure analysis |

## Claims and their wording (do not inflate)

- **"The first executable environment for *neutron instrument design*"** —
  never the unqualified "first executable scientific environment" (MDGYM et al.
  exist; prior-art re-check mandatory before the M7 positioning section).
- **Trainability claim bar, verbatim:** 7B + training beats the 7B baseline,
  approaching a larger untrained model. The delta validates the *environment*,
  not a frontier agent. Degradation ladder if signal fails, reported honestly:
  GRPO → SFT-only → env+eval-only.
- Structural advantages the claims rest on: seconds-per-rollout (template
  families + compiled-binary cache), fully programmatic reward (**no LLM judge
  anywhere in T1/T2 grading**), Liouville/brilliance-transfer ≤ 1 as
  physics-native hack detection, open-source releasability.

## In scope

- McStas 3.x only, on the conda-forge toolchain; state in `~/.mcstas-mcp/`.
- Single agent + skill + validating tools (no multi-agent split unless evals
  show persistent unforced physics errors).
- Eval matrix: subscription Claude + ~$200 OpenRouter breadth + **open-weights
  arms (required — they are the RL-claim baselines)** served from the 7×A100-40G.
- RL track in the paper: filtered SFT (Aug 11 – Sep 1) → GRPO (Sep 1 – 15) on
  Qwen-family 7–8B, LoRA, numbers frozen ~Sep 17.
- Self-contained episode outputs: every benchmark run yields a folder with the
  report, transcript, and an artifact bundle (built `.instr`, component diagram
  PNG, real-scale geometry trace) — inspectable without re-execution.

## Out of scope (standing decisions — change only with a dated note)

- **McXtrace** (kept in the design, out of the MVP) · **Union components /
  NCrystal-deep work** (introspection exposes them anyway).
- **Cross-project agent frameworks** — this work is McStas only; one substrate,
  done deeply. No autoMartiniAgent suite, no framework paper.
- **Frontier-model training** — training is small-model; frontier models are
  evaluated, not tuned.
- **HPC/SLURM env execution** — laptop-scale env/eval; the A100s are for M8
  training only; async API already accommodates a later SLURM path.
- **LLM-judged grading for T1/T2** — programmatic observables only; the T3
  expert rubric is explicitly outside the scored set until vetted.

## Non-negotiables (uncuttable even under deadline pressure)

Contamination controls · red-team-the-reward analysis (three caught exploits
are paper material) · T2 improvement analysis · the SFT trainability result ·
fresh-seed re-verification of any selected best · env-controlled grading
protocol (never agent-chosen ncount) · grade the artifact the agent built,
never its claims.

Cut order if behind: T3 from the scored set → GRPO (drop to SFT-only) → T1
growth beyond the current 14 (already cut-by-default as of 2026-07-29 — the
14 self-validating tasks are the defensible set; revive only with slack).

## Anchors

- **Venue: ICLR 2027, committed** — abstract 2026-09-19, full paper 2026-09-24
  AoE; NeurIPS 2027 D&B is the fallback, not a co-target.
- **Hardware/budget:** Apple Silicon Mac (env/eval) · 7×A100-40G (~3 generation
  / ~4 training) · ~$200 OpenRouter + subscription Claude.
- **Sources of truth, in order:** dated notes in `note/` (decisions) →
  `PLAN.md` (living plan) → this file (anchor summary; if it disagrees with a
  newer dated note, the note wins and this file needs updating).
