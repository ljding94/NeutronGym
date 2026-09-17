# NeutronGym — Goal & Scope

*The anchor document: what this work is, what it claims, and where its edges are.
Stable by design — it changes only when a dated decision note changes it.
The **how/when** lives in `PLAN.md` (milestones, acceptance criteria, gates);
day-to-day conventions live in `CLAUDE.md`. Last updated 2026-09-17 (M8 reopened
for step-level GRPO and a third family; `note/m8-grpo-guide-result-2026-09-17.md`
applied; prior-art re-check `note/prior-art-recheck-2026-09-13.md` still applies).*

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

*Wording rule (revised 2026-09-17):* "RL environment" is now defensible, but only
with the evidence attached: step-level GRPO trains Qwen3-8B from environment
reward alone (guide_match 14.0% -> 50.0% held-out, above a one-shot physics
formula at 31.3%; guide 18.0% -> 98.7%). Never quote the guide number without
its readout-rule baseline (96.3%) — that family is solvable by copying the
limits its prompt prints. SFT regressed three times (see Claims).

## The three research questions (every task must serve one)

1. **Reproduction** — can an LLM agent reproduce a neutron instrument from its
   instrument-design paper, graded on simulation observables? *(curated,
   non-generatable held-out slice)*
2. **Improvement** — can an LLM agent improve a design against quantitative
   target specs, measured against classical baselines under matched compute?
   *(the trainable core — procedurally generated)*
3. **Trainability** — does physics-verifiable reward train? *(answered
   2026-09-17: YES with step-level GRPO, NO with rejection-sampling SFT.
   Three SFT runs regressed the 8B; GRPO on decisions drawn from the model's
   own states — failures included — lifted it on both gated families. Reported
   with every no-model baseline attached — see Claims.)*

## What ships

| Artifact | What it is |
|---|---|
| **NeutronGym** (headline) | Gym-style env: reward-ladder API with level-resolved output, procedural instance generator, anti-hacking checks, fast tier at ~25 rollouts/s/core; three families — two flux-maximisation (guide, SANS) and one target-matching (`guide_match`, added 2026-09-17 because maximisation families proved rule-solvable); pip-installable (Docker deferred to camera-ready — trim 2026-07-29) |
| **McStasBench** | Tiered eval slice: T1 reproduce / T2 improve / T3 open design, with contamination controls (held-out 2024–26 instruments, memorization probes, perturbed variants) |
| **McStasAgent** | Reference baseline: 22-tool MCP server wrapping McStasScript + the `mcstas-instrument-design` skill; run through the **NeutronGym reference loop** (minimal model-agnostic scaffold shipped in the env — the measurement instrument for all headline numbers; Claude Code is a comparison arm, decided 2026-07-30) |
| **ICLR 2027 paper** | Eval matrix across model tiers with level-resolved failure analysis, plus the M8 trainability study reported as a methods result (pre-registered regression, its mechanism, and the red-team findings it surfaced) |

## Claims and their wording (do not inflate)

- **"The first executable environment for *neutron instrument design*"** —
  never the unqualified "first executable scientific environment" (MDGYM et al.
  exist). Keep it narrow and dated: prior-art re-check 2026-09-13 found no
  McStas/McXtrace LLM work, but McStas developers now use AI assistance.
- **Trainability — GRPO meets the bar on a gated family; SFT never did.**
  Step-level GRPO (decisions sampled from the model's own episodes, failures
  included; group-normalised advantage; KL to the frozen base) on `guide_match`,
  n=300 held-out, +/-5%, 10 turns: untrained 8B **14.0%**, untrained 32B 11.0%,
  one-shot physics formula 31.3%, best fixed design / lookup 4.0%, **GRPO 8B
  50.0%** (paired: 122 trained-only vs 14 untrained-only). It is iterative
  design, not a rule: **no arm passes any instance on turn 1**, the trained
  model solves 118/150 on turn 4+, 132 distinct passing designs, and against
  the formula on the same instances it is trained-only 97 / formula-only 41.
  Caveat to state: at fresh seeds 49/58 passes hold (~16% sit near the
  tolerance edge). On the guide family GRPO reaches 98.7% from 18.0%, but a
  no-model readout rule reaches 96.3% there, so that number demonstrates
  reward-driven strategy discovery, not design skill.
- **The three SFT regressions stand as the contrast, and the paper says so.**
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
- M8 trainability study in the paper — **reopened 2026-09-17** (user decision
  after the third SFT regression): self-generated RAFT + LoRA SFT on Qwen3-8B
  (three runs, all regressions) **plus step-level GRPO** on the guide and
  `guide_match` families, each gated before its pass rates are read. Every
  training number is reported beside its no-model baselines (constant, lookup,
  readout rule, physics formula).
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
**a no-model probe suite for every family before its pass rates are read as
capability: fixed designs, other instances' solutions (lookup), and
prompt-readout rules, each judged on the one-sided 95% upper limit, with
physics-model rules reported as a reference arm rather than gated** · T2
improvement analysis ·
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
