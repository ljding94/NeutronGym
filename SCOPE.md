# NeutronGym — Goal & Scope

*The anchor document: what this work is, what it claims, and where its edges are.
Stable by design — it changes only when a dated decision note changes it.
The **how/when** lives in `PLAN.md` (milestones, acceptance criteria, gates);
day-to-day conventions live in `CLAUDE.md`. Last updated 2026-09-15 (M8 frozen;
prior-art re-check `note/prior-art-recheck-2026-09-13.md` applied).*

## Goal

**Build NeutronGym: an executable, physically verifiable environment and
benchmark for LLM agents designing neutron instruments — and report, in one
ICLR 2027 paper, what it shows about evaluating and training them.** Neutron
optics is the substrate, McStas the simulation backend. The environment is
procedurally generated and densely rewarded, with a level-resolved reward
ladder (L1 syntax → L2 runtime → L3 structural → L4 scientific) presented as a
*measurement tool*, not as novel reward design (four-level gated rewards have
2026 precedent). **McStasBench** — the held-out benchmark slice — falls out of
it; **McStasAgent** (the `mcstas-mcp` server + design skill) is the reference
baseline shipped inside it, not a standalone contribution.

*Wording rule (2026-09-15):* do **not** call NeutronGym an "RL environment" in the
headline or abstract. The pre-registered trainability result was a regression,
and the post-hoc gain was a memorized constant action (see Claims).

## The three research questions (every task must serve one)

1. **Reproduction** — can an LLM agent reproduce a neutron instrument from its
   instrument-design paper, graded on simulation observables? *(curated,
   non-generatable held-out slice)*
2. **Improvement** — can an LLM agent improve a design against quantitative
   target specs, measured against classical baselines under matched compute?
   *(the trainable core — procedurally generated)*
3. **Trainability** — does physics-verifiable reward train? *(answered
   2026-09-15, M8 frozen: self-generated RAFT + LoRA SFT on Qwen3-8B, no GRPO.
   The pre-registered run regressed the 8B; a post-hoc passing-turn run
   matched the untrained 32B only by learning one constant action. Reported as
   a methods result — see Claims.)*

## What ships

| Artifact | What it is |
|---|---|
| **NeutronGym** (headline) | Gym-style env: reward-ladder API with level-resolved output, procedural instance generator, anti-hacking checks, fast tier at ~25 rollouts/s/core; pip-installable (Docker deferred to camera-ready — trim 2026-07-29) |
| **McStasBench** | Tiered eval slice: T1 reproduce / T2 improve / T3 open design, with contamination controls (held-out 2024–26 instruments, memorization probes, perturbed variants) |
| **McStasAgent** | Reference baseline: 22-tool MCP server wrapping McStasScript + the `mcstas-instrument-design` skill; run through the **NeutronGym reference loop** (minimal model-agnostic scaffold shipped in the env — the measurement instrument for all headline numbers; Claude Code is a comparison arm, decided 2026-07-30) |
| **ICLR 2027 paper** | Eval matrix across model tiers with level-resolved failure analysis, plus the M8 trainability study reported as a methods result (pre-registered regression, its mechanism, and the red-team findings it surfaced) |

## Claims and their wording (do not inflate)

- **"The first executable environment for *neutron instrument design*"** —
  never the unqualified "first executable scientific environment" (MDGYM et al.
  exist). Keep it narrow and dated: prior-art re-check 2026-09-13 found no
  McStas/McXtrace LLM work, but McStas developers now use AI assistance.
- **Trainability — the pre-registered bar was NOT met, and the paper says so.**
  Bar, verbatim: 7B + training beats the 7B baseline, approaching a larger
  untrained model. Outcome (guide family, 1.0× calibrated bar, n=300 paired):
  RAFT SFT took Qwen3-8B from 40.3% to 31.3% (McNemar p=0.0013) by cloning
  exploration turns. A post-hoc passing-turn run reached 52.3% (≈ untrained
  32B), but a no-model constant action passes 50–52% and agrees with it on 98%
  of instances. Never present the post-hoc number as a trainability result.
- **Environment-builder lessons are claimable:** uncalibrated reward ladders
  manufacture ceilings; small-n model comparisons reverse; and a no-model
  constant-policy probe exposed both a reward hole (SANS direct beam) and a
  degenerate task (guide at 1.0×). Cite precedent (RLVE for difficulty
  calibration) rather than claiming these ideas as new.
- Structural advantages the claims rest on: seconds-per-rollout (template
  families + compiled-binary cache), fully programmatic reward (**no LLM judge
  anywhere in T1/T2 grading**), Liouville/brilliance-transfer ≤ 1 as
  physics-native hack detection (tight for guides, loose for SANS), a
  **no-model constant-policy gate** every family must pass before its pass
  rates are read as capability (`neutrongym.hacks`, acceptance step 4),
  open-source releasability.

## In scope

- McStas 3.x only, on the conda-forge toolchain; state in `~/.mcstas-mcp/`.
- Single agent + skill + validating tools (no multi-agent split unless evals
  show persistent unforced physics errors).
- Eval matrix: subscription Claude + ~$200 OpenRouter breadth + **open-weights
  arms (Qwen3-8B / Qwen3-32B)** served from the DGX A100s — the baselines for
  both the eval matrix and the M8 study.
- M8 trainability study in the paper — **FROZEN 2026-09-15:** self-generated
  RAFT + LoRA SFT on Qwen3-8B, guide family only (SANS excluded while its
  reward hole was open), pre-registered evaluation plus one pre-specified
  post-hoc ablation. **No GRPO** (dropped 2026-09-12) and no further training
  runs.
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

Contamination controls · red-team-the-reward analysis (seven findings, incl.
the SANS direct-beam leak and the guide family's constant-policy degeneracy) ·
a no-model constant-policy probe for every family · T2 improvement analysis ·
the pre-registered trainability result reported as it came out (a regression),
with the post-hoc ablation labelled post-hoc · fresh-seed re-verification of
any selected best · env-controlled grading protocol (never agent-chosen
ncount) · grade the artifact the agent built, never its claims.

Cut order if behind: T3 from the scored set → T1
growth beyond the current 14 (already cut-by-default as of 2026-07-29 — the
14 self-validating tasks are the defensible set; revive only with slack).

## Anchors

- **Venue: ICLR 2027, committed** — abstract 2026-09-18, full paper 2026-09-25
  AoE; NeurIPS 2027 D&B is the fallback, not a co-target.
- **Hardware/budget:** Apple Silicon Mac (env/eval) · DGX A100-40G (GPUs 0–4
  served the open-weights baselines via vLLM; GPU 7 ran M8 LoRA training and
  checkpoint serving) · ~$200 OpenRouter + subscription Claude.
- **Sources of truth, in order:** dated notes in `note/` (decisions) →
  `PLAN.md` (living plan) → this file (anchor summary; if it disagrees with a
  newer dated note, the note wins and this file needs updating).
