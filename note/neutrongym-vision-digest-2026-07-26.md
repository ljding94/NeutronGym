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
- ~~Repo/directory name stays `McStasBench`~~ **Amended same day (user decision): GitHub repo renamed to `NeutronGym`** (`gh repo rename`; old URLs redirect), and the local working directory was subsequently renamed to `~/Work/NeutronGym` as well. The paper and released package use NeutronGym.

## Decision 2 — Venue: ICLR 2027, committed

- **Abstract 2026-09-19, full paper 2026-09-24 AoE** (web-verified
  2026-07-26; re-verify on the official CFP when M7 starts). ~8.5 weeks out.
- The 07-24 hedge ("NeurIPS D&B primary, ICLR if fast") is resolved:
  **ICLR 2027 is the target.** NeurIPS 2027 D&B (~May 2027) is the fallback
  — with GRPO results folded in — if ICLR slips or rejects.
- ~~**RL content policy for the ICLR paper:** include-if-signal SFT by
  ~Sep 10; GRPO follow-up either way.~~ **Superseded same day — see the
  Addendum below: bench + RL are both committed ICLR paper content.**

### Back-planned schedule (deadline-anchored, part-time; revised by the Addendum below)

| Window | Work |
|---|---|
| now – **Aug 10** | M5 RL-critical path: reward-ladder API (level-resolved output) + env executor |
| **Aug 11 – 18** | M5 remainder: procedural generator, anti-hacking checks, packaging, curation hardening |
| **Aug 11 – Sep 1** | M8: rejection sampling → filtered SFT (Qwen 7–8B), parallel with everything (CPU rollouts / GPU training) |
| **Aug 17 – Sep 5** | M6: eval matrix (prereq first: widen OpenRouter privacy policy — only Google models route today) |
| **Sep 1 – 15** | M8: GRPO (go/no-go on the Sep 1 SFT signal), 2–3 runs on 7×A100-40G; **RL numbers frozen ~Sep 17** |
| **Sep 1 – 24** | M7: analysis + writing, overlapping M6/M8 tails; Figure 1 + skeleton by ~Sep 5; abstract locked Sep 19 |

Cuttable-if-behind, in order: T3 from the scored set (rubric already
PENDING) → GRPO (drop to SFT-only) → T1 growth stops at the current 14
(+ vetted externals only). **Not cuttable:** contamination controls,
red-team-the-reward section, T2 analysis, the SFT trainability result.

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

## Addendum (2026-07-26, later same day — user decisions)

1. **Bench + RL are both ICLR paper content** — supersedes the
   include-if-signal policy above. The trainability result (filtered SFT +
   GRPO on a 7–8B model) is planned, co-equal paper content, not a stretch
   goal: 60 days to the deadline is enough if the M5 reward API lands first
   (~Aug 10). Sequencing: rejection sampling + SFT Aug 11 – Sep 1 → SFT
   delta is the GRPO go/no-go (Sep 1) → GRPO Sep 1 – 15 (2–3 runs) → all
   RL numbers frozen ~Sep 17, fresh-seed re-verified on held-out tasks.
   If training signal genuinely fails, degrade gracefully and report it
   honestly: GRPO → SFT-only → env+eval-only. The claim bar itself is
   unchanged (delta validates the environment, not a frontier agent).
2. **Hardware: 7×A100-40G** (not the 8× recorded on 07-24): ~3 GPUs
   generation (vLLM serving the 7–8B policy) / ~4 training (LoRA r=32–64
   fits comfortably); McStas rollouts are CPU-bound and don't contend.
3. **Local working directory renamed** to `~/Work/NeutronGym`, completing
   the rename (GitHub repo was renamed earlier the same day).
4. **Eval budget (set with the same-day plan review):** Claude via
   subscription ($0); **~$200 OpenRouter credit** for API-model breadth;
   open-weights served from the 7×A100 ($0 API cost) and **REQUIRED, not
   optional** — the untrained Qwen 7–8B and a larger untrained comparator
   are the RL-claim baseline arms, collected during M6. Spend policy:
   ablation grid (±MCP × ±skill, plain-LLM) rides on the free tiers
   (subscription Claude + one mid-tier OpenRouter model on the dev split);
   paid spend buys cross-model breadth on the main arm. The same review
   added: the two-axis held-out discipline (curated T1 contamination axis
   vs procedural-regime generalization axis, each touched once in a single
   ~Sep 15–17 final pass), an environment acceptance test for M5 (gym-loop
   over ≥100 procedural instances), a minimal procedural generator pulled
   into the ~Aug 10 critical path, and de-risk gate 5 (SFT delta by Sep 1).

## Changes made with this note

PLAN.md: status + timeline re-anchored to the ICLR deadline; Purpose names
the artifact; M5 gains the level-resolved reward output + hard date; M6
gains the plain-LLM arm and 2×2 grid; M7 retitled to the ICLR paper with
Figure-1 discipline, committed venue, and claim wording; M8 stage 1 pulled
parallel with M6 with the include-if-signal cutoff and the attribution bar;
standing decisions record naming + venue. CLAUDE.md: framing paragraph,
deliverables, and sources-of-truth list updated to match.
