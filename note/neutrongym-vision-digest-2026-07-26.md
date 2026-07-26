# NeutronGym: naming + ICLR 2027 commitment (digest of the vision note)

**Date:** 2026-07-26 · **Status: adopted (user decision).** Digests the
external "NeutronGym: ICLR 2027 Project Vision" note from the user's prior
discussion. That note is ~80% convergent with
`scope-evolution-rl-env-2026-07-24.md` — independent confirmation of the
environment-first reframe, not a new direction. This note records what the
vision note *adds* (adopted below), what it gets wrong (rejected below), and
the two decisions it forced. Supersedes `scope-decision-2026-07-09.md` and
the 07-24 note on **naming** and **venue** where they differ; the three
research questions in PLAN.md §Purpose stand unchanged.

## Decision 1 — Naming: NeutronGym is the environment, McStasBench is its benchmark slice

- **NeutronGym** = the headline artifact: the executable RL environment
  (tasks + McStas backend + MCP interface + skill + reward ladder +
  procedural generation). The name asserts *environment* (Gym lineage) —
  which the adopted framing requires and which "McStasBench" contradicted.
- **McStasBench** = the held-out benchmark slice inside NeutronGym (curated
  T1 paper-reproduction + held-out T2 parameter regimes). Two-level naming
  is a feature: env name for the RL/agents audience, bench name for the
  eval audience.
- McStas is described as the simulation backend, not the subject. (It is
  spelled **McStas** — the vision note's "MCStas" must not reach the paper.)
- ~~Repo/directory name stays `McStasBench`~~ **Amended same day (user decision): GitHub repo renamed to `NeutronGym`** (`gh repo rename`; old URLs redirect). The local working directory keeps its name (renaming it mid-stream breaks the active session + project-keyed memory; harmless otherwise). The paper and released package use NeutronGym.

## Decision 2 — Venue: ICLR 2027, committed

- **Abstract 2026-09-19, full paper 2026-09-24 AoE** (web-verified
  2026-07-26; re-verify on the official CFP when M7 starts). ~8.5 weeks out.
- The 07-24 hedge ("NeurIPS D&B primary, ICLR if fast") is resolved:
  **ICLR 2027 is the target.** NeurIPS 2027 D&B (~May 2027) is the fallback
  — with GRPO results folded in — if ICLR slips or rejects.
- **RL content policy for the ICLR paper:** M8 stage 1 (rejection sampling →
  filtered SFT on Qwen-family 7–8B) starts as soon as the M5 reward API
  lands (~mid-Aug) and runs in parallel with M6 (McStas rollouts are
  CPU-bound; API-based M6 episodes don't contend). **Include-if-signal
  cutoff ~Sep 10**: an SFT delta by then goes in as the preliminary
  trainability result; otherwise the paper ships as env + frontier eval +
  red-team, and the trainability claim shrinks to "RL-ready by
  construction". GRPO is follow-up work either way.

### Back-planned schedule (deadline-anchored, part-time)

| Window | Work |
|---|---|
| now – **Aug 18** | M5: env executor, procedural generator, reward-ladder API with level-resolved output; curation hardening |
| **Aug 17 – Sep 5** | M6: eval matrix (prereq first: widen OpenRouter privacy policy — only Google models route today) |
| **Aug 18 – Sep 10** | M8 stage 1: rejection sampling → filtered SFT, parallel with M6; include-if-signal |
| **Sep 1 – 24** | M7: analysis + writing, overlapping M6 tail; Figure 1 + skeleton by ~Sep 5; abstract locked Sep 19 |

Cuttable-if-behind, in order: T3 from the scored set (rubric already
PENDING) → the SFT result (policy above) → T1 growth stops at the current 14
(+ vetted externals only). **Not cuttable:** contamination controls,
red-team-the-reward section, T2 analysis.

## Adopted from the vision note (mapped to existing machinery)

1. **L1–L4 hierarchical evaluation as the paper's presentation of the reward
   ladder** — same machinery, better legibility: L1 syntax/compile = static
   tier (registry validation + `validate_instrument`); L2 runtime = cheap
   dynamic tier; L3 structural (component selection, topology, geometry,
   parameter validity) = registry + topology checks; L4 scientific (flux,
   distributions, resolution, constraints) = terminal grader. **New M5 work
   item: every graded episode reports deepest-level-reached** as a
   first-class harness field — the M7 failure taxonomy and M8 attribution
   analysis then fall out for free.
2. **Plain-LLM baseline arm (M6)** — no tools, one-shot `.instr` generation
   from the task prompt, graded by the same harness. Was missing from the
   plan; cheap; it is the arm that proves the environment infrastructure
   matters at all.
3. **2×2 agent ablation grid (M6/M7)** — ±MCP × ±skill plus the plain-LLM
   anchor; cleaner presentation of the existing ablation list.
4. **Figure-1-first writing discipline (M7)** — the loop (NL requirement →
   agent → MCP + skill → NeutronGym → simulation → evaluation → reward →
   improved agent) drawn before any text; the paper should be understandable
   from Figure 1 alone.
5. **"Explain *why* training improves, not that it improves" (M8)** — with
   level-resolved eval this is answerable from data: do failures migrate
   from L1/L2 (mechanics) to L4 (science)? do L4 pass rates move on held-out
   families? Rollout logging must capture per-level outcomes from day one.
6. **Framing discipline** — the story is never prompt engineering, McStas
   scripting, code generation, or benchmark construction; those are
   implementation details. (Qualified below regarding T1.)

## Rejected or qualified

- **"First executable scientific environment" — overclaim; do not write
  it.** MDGYM et al. exist and this space produces papers monthly. The
  defensible claim: "first executable environment for **neutron instrument
  design**" (pending the mandatory prior-art re-check before the M7
  positioning section).
- **The vision note omits contamination and reward hacking entirely** —
  these are the project's moats, not footnotes: the memorization probe
  (real file ≈1.0 vs from-memory 0.07), held-out 2024–26 instruments,
  perturbed variants; three real reward exploits caught before any agent saw
  a task (beamstop leakage, optimizer bound escape, winner's curse) plus
  Liouville/brilliance-transfer hack detection. Both remain headline paper
  sections. A NeutronGym without them is generic vision any team could
  write.
- **"Significantly improve a small open-source LLM" — claim inflation.**
  The recorded bar stands verbatim: the delta validates the environment
  (7B + training beats 7B baseline, approaching a larger untrained model);
  for the ICLR paper this is likely SFT-only evidence.
- **"Avoid describing as benchmark construction" — qualified.** T1
  paper-reproduction is the scientifically distinctive, non-generatable
  content and the credibility layer of the held-out slice (07-24
  qualification 2 stands). Environment-first framing *contains* the
  benchmark; the disavowal is about story emphasis, not content.

## Changes made with this note

PLAN.md: status + timeline re-anchored to the ICLR deadline; Purpose names
the artifact; M5 gains the level-resolved reward output + hard date; M6
gains the plain-LLM arm and 2×2 grid; M7 retitled to the ICLR paper with
Figure-1 discipline, committed venue, and claim wording; M8 stage 1 pulled
parallel with M6 with the include-if-signal cutoff and the attribution bar;
standing decisions record naming + venue. CLAUDE.md: framing paragraph,
deliverables, and sources-of-truth list updated to match.
