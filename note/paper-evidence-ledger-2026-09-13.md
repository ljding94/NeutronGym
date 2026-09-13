# Paper evidence ledger — what the manuscript can cite (2026-09-13)

**Purpose:** one place to draft the ICLR 2027 paper from. Every claim here
has a number, a source file, and a strength label. Numbers were copied from
command output on 2026-09-13 (~18:40), not from memory. Where the project's
own documents disagree with each other, the conflict is listed in §6.
**Resolve §6 before citing any affected number.**

Deadlines: **abstract 2026-09-18 (5 days) · full paper 2026-09-25 AoE (12 days).**
Skeleton + Figure 1 draft: `paper/OUTLINE.md`. Regenerable results of record:
`benchmark/RESULTS.md` ← `benchmark/results/m6_results.json` ←
`benchmark/evidence/` (1082 committed files).

Strength labels: **SOLID** (cite as is) · **SUGGESTIVE** (cite only with the
stated caveat) · **PENDING** (data still arriving) · **RETRACTED** (a claim we
made and withdrew; belongs in Limitations/integrity, never in results).

---

## 1. Status snapshot

| Track | State | Source |
|---|---|---|
| Environment (M5/M5.5) | shipped, acceptance passed, `neutrongym` 0.9.0 wheel verified in a clean venv | `runs/env_acceptance/acceptance.json`, PLAN M5.5 |
| Benchmark matrix (M6) | complete; 2 held-out cells stale in evidence (§6 D4) | `benchmark/RESULTS.md` |
| Held-out final pass | run once on 2026-09-10; the 8 qwen cells are still infra, re-runs queued behind the sweep | RESULTS Table 2 |
| RL / trainability (M8) | **stopped by its own phase-0 gate; no training run.** A difficulty-response sweep is **running now** (PID 9545, n=25/family, 0.8/1.0/1.2/1.4×) and may overturn the gate's reading (§5) | `runs/m8/` |
| Prior-art re-check | **marked MANDATORY; last done 2026-07-09; no output recorded since** | PLAN M7 |
| Test suite | 224 tests collected | `pytest --collect-only` |

---

## 2. Contribution → evidence map

### C1. NeutronGym: an executable, physically verifiable environment for neutron instrument design

| Claim | Number | Strength | Source |
|---|---|---|---|
| Fast-tier throughput | **30.3 rollouts/s/core at 1e5 rays** (33 ms median); compile-once per family | SOLID | `runs/env_acceptance/acceptance.json`; `src/neutrongym/executor.py` |
| Why fast: direct binary vs mcrun | mcrun path ~2.4 s flat per rollout regardless of ncount; direct binary ~0.04 s | SOLID | PLAN M5 (timing table in `scripts/env_walkthrough.py`) |
| Env acceptance | 104 instances × 3 scored actions = 312 records, all level-resolved; histogram L0:104 / L1:0 / L2:59 / L3:119 / L4:30; wall 17.7 s | SOLID | `acceptance.json` |
| Level histogram reading | L0 = deliberate out-of-bounds probes; every baseline resubmission stops at L3 (strict-improvement bar holds); random search reaches L4 in 30/104 → dense signal with headroom | SOLID | PLAN M5 accept |
| Clean install | fresh Py3.14 venv + wheel + PyPI deps → 29.4 rollouts/s, exit 0 | SOLID | PLAN M5.5 |
| Reward ladder | L1 static (bounds, free) → L2 truncated-ncount run → L3 FOM monitor present + statistics floor + **band** constraints + Liouville → L4 FOM > target_ratio × baseline. Shaping: +0.25/level L1–L3, L4 adds 0.25·min(fom_ratio,2), max 1.25. Env-controlled ncount/seed | SOLID (design) | `src/neutrongym/reward.py` docstring |
| Procedural generation | 2 template families (`guide_divergence`, `sans_collimation`); instance = deterministic draw from (family, split, index); held-out split = **disjoint context-parameter regimes** | SOLID (design); note: only 2 families | `src/neutrongym/generate.py` |
| Per-instance calibration | target = **0.8×** constraint-filtered, Liouville-checked, fresh-seed-re-verified classical optimum (30-sample search) — same discipline as T2 | SOLID (design) | `src/neutrongym/calibrate.py` |
| Tooling baseline | **22 MCP tools** (verified count) wrapping McStasScript; McStas 3.7.12 | SOLID | `src/mcstas_mcp/server.py` |
| Reference loop | model-agnostic OpenAI-compatible client + MCP stdio; no shell, no file Read; skill injected as system prompt (±skill toggle) | SOLID; **size claim needs care** (§6 D11) | `src/neutrongym/agent.py` |

### C2. McStasBench: held-out slice with a contamination architecture

| Claim | Number | Strength | Source |
|---|---|---|---|
| Catalog | 19 T1 (12 seen + 2 dev + 2 held-out + 3 perturbed) + 2 T2 + 2 T3 + 3 pilots | SOLID | PLAN M5.5; `benchmark/tasks/` |
| Auto-authored, self-validating | NL spec sheets derived from machine-verified references; reference passes its own task at a fresh seed; T2: classical best PASSES and unimproved baseline FAILS | SOLID | `validate_tasks.py`, `tasks/*/_validation.json` |
| Scored protocol | pass@1, temperature 0, 50-turn cap, env-controlled seeds, provider pinned per model | SOLID | `benchmark/m6_config.json` |
| Memorization probes (temp 0, pinned) | haiku-4.5 none/16 · sonnet-5 **ILL_H10_IN8, templateSANS** /16 · flash-lite none/16 · gemini-3.6-flash ILL_H10_IN8/16 · maverick probe incomplete | SOLID (maverick row is a stub) | RESULTS Table 4; `benchmark/contamination/` |
| Probe drift despite temp 0 | sonnet 1/16 → 2/16 between Aug 23 and the pre-freeze re-sweep (templateSANS newly flagged; dev split, no scored impact) | SOLID, methodology footnote | PLAN recap 09-07 |
| Held-out instruments | `T1_BOYA_CARR` (single-channel simplification of the CARR multiplexing spectrometer, arXiv:2501.01143) and `T1_VENUS_SNS` (SNS BL-10 TOF imaging); references authored by us, never public, **not facility-validated**; BOYA probe 0.0 both models; VENUS keyfacts 0.667 = generic topology (similarity 0.119) | SOLID, with the "not facility-validated" and "simpler than seen median" caveats | task JSON `provenance`; PLAN M5.5 |
| Perturbed pair-proofs | 3 variants (PSI_DMC source.dist ×1.15, SANS2d source.dist ×1.15, HZB_FLEX guide curvature ×1.4): perturbed reference self-passes AND canonical reference fails the variant → a memorizer fails, a spec-follower passes. 9/12 seen tasks had no valid detectable perturbation | SOLID; coverage 3/12, say so | `tasks/T1_perturbed/_generation.json` |
| Memorization scope | pair-proofs cover the 3 paired canonicals only; passes on unpaired tasks rest on the probes | SOLID wording rule | PLAN M6 (09-02 review fix) |
| Sandbox origin story | 2026-07-30 audit: **4/7 pilot episodes fetched the reference `.instr`** through allowed tools (`get_example`, `load_instr_file`) — no malice, normal McStas practice | SOLID | `note/pilot-leak-audit-2026-07-30.md` |
| Sandbox now | 3 layers: server benchmark mode, episode Read-deny rules, mandatory transcript audit; **0 leak-invalid episodes across the whole campaign** | SOLID | RESULTS totals |
| Held-out discipline structural | held-out tasks live in `heldout_FINAL_PASS_ONLY`; matrix enumerator cannot reach them (regression-tested); touched once | SOLID | `m6_config.json` |

### C3. Red-teaming the reward (five findings, all caught before any scored number)

Source: `note/reward-red-team-2026-08-05.md`, verification `runs/redteam/sweep.json`.

1. **Beamstop-leakage hack**: unconstrained flux maximization found 672 n/s of direct beam; max-only width constraints have the wrong sign for leakage → **band** constraints.
2. **Optimizer bounds escape**: `mcrun --optimize` Nelder–Mead ignored bounds (escaped to w = 2.1 m) → classical baselines are bounds- and constraint-filtered ensembles.
3. **Winner's curse**: best-of-30 noisy evaluations fell below its own target at a fresh seed; the guard rejected our original guide task → fresh-seed re-verification of any selected best (task rebased to w=0.012/m=1.5).
4. **Monitor-matching decoy** (found by the reference loop, not by curation): most-events matching graded an agent's diagnostic monitor against the reference detector. Two frontier models "failed" 3/6 with identical ~1500×-hot signatures → position-aware matching; both regrade to PASS 6/6.
5. **The red-team's own false positive**: the Liouville gate flagged 1.14× the bound on a ~250-event monitor (noise; 0.99 at 10× rays) → gate evaluates only above the statistics floor.

Liouville/brilliance bound: FOM ≤ flux × A × Ω × Δλ, reference-free. **Sharp for guides** (legit optimum 0.93–0.99× bound, so the FOM plateau *is* the Liouville limit); **loose for SANS** (conservation only; bands do the fine work). Corner sweep: **56 adversarial actions, 0 false flags, max legit utilization 1.016, 16 band catches.** Strength: SOLID.

**Sixth finding (candidate for the section):** uncalibrated procedural targets manufactured a ceiling (`target_ratio = 1.0` meant beating a deliberately undersized baseline) — see §5. Same shape as findings 1–5: our own gate caught a reward that looked like signal.

### C4. Cross-model evaluation (M6)

Source: `benchmark/RESULTS.md` (generated 2026-09-13 17:47). Validity rule: INFRA and LEAK excluded from every rate.

**Totals as printed:** 225 valid matrix episodes (scored set, dev excluded) · 18 valid held-out · 10 infra-excluded · 0 leak-invalid · **$122.16** OpenRouter. (For "271" and the 16-vs-17 question see §6 D1–D2.)

**Table 1, main arm (loop + MCP + skill), seen scored set, valid n = 16 per cell:**

| model | loop pass | one-shot pass | loop failures L4/L3/L2/L1/L0 | one-shot failures L4/L3/L2/L1/L0 |
|---|---|---|---|---|
| claude-sonnet-5 | 5 | 7 | 1/0/0/9/1 | 0/1/2/6/0 |
| claude-haiku-4.5 | 4 | 4 | 4/2/2/4/0 | 0/0/0/12/0 |
| gemini-3.6-flash | 4 | 6 | 0/1/0/1/10 | 1/1/2/6/0 |
| gemini-3.5-flash-lite | 2 | 1 | 1/0/1/5/7 | 0/0/0/15/0 |
| llama-4-maverick | 0 | 2 | 0/2/0/1/13 | 0/0/0/14/0 |
| qwen3-32b (vLLM, YaRN, non-thinking) | 3 | 0 | 0/0/0/0/13 | 0/0/0/0/16 |
| qwen3-8b (vLLM, YaRN, non-thinking) | 1 | 0 | 0/1/1/0/13 | 0/0/0/0/16 |

(Level columns count failures by deepest level reached; passes are separate. Rows sum to 16.)

| Claim | Strength | Wording / caveat |
|---|---|---|
| **T2 unsolved by every model in every arm**; classical search reaches **2.95×** (guide, 628.6σ over initial) and **1.47×** (SANS, 510.3σ) at 1e8 rays, fresh seed 20260823 | SOLID | the σ values measure improvement over the *initial* config, not a margin over models. Present **two** baselines (initial + filtered classical best), never three columns |
| **Zero reference leaks** across the campaign | SOLID | |
| **Failure structure differs by scaffold**: one-shot failures pile up at L1 (won't compile: haiku 12, flash-lite 15, maverick 14); loop failures pile up at L0 (tools engaged, no finished instrument: gemini 10, qwen-8b 13, qwen-32b 13, maverick 13) | SOLID as distribution | the core failure-mode figure |
| Failure kinds (matrix): **38 format · 51 incomplete · 96 physics**; held-out 1 · 2 · 6 | SOLID (RESULTS version only, §6 D3) | "incomplete" ≠ protocol failure |
| Open-weights ordering: 32B pass set {SESANS_Delft, ISIS_SANS2d, ISIS_SANS2d_pert} **⊃** 8B {SESANS_Delft}; 32B passes a canonical AND its perturbed twin | SOLID as ordering; **rate difference p = 0.600** | never "32B is 3× better" |
| Open-weights models only produce anything through the loop (one-shot 0/16, all L0, no parseable `.instr`) | SOLID | format failure, not physics |
| Loop vs one-shot on the **seen** tier | **RETRACTED as a claim.** All p ≥ 0.70 (haiku 1.000, sonnet 0.716, flash-lite 1.000, gemini 0.704) | say "not resolvable at n=16" |
| Loop > one-shot on **held-out** (4 API models, paired: 7/8 vs 2/8, Fisher p = 0.041) | SUGGESTIVE, confounded | **always** paired with: held-out tasks are easier (loop arm 7/10 vs 20/113, p = 0.0009). The load-bearing comparison is within-held-out, same tasks |
| Serving-route variance | SOLID as methodology footnote | maverick: zero tool calls on the Google route, working tool calls on DigitalOcean; its 2 held-out main cells ran on the withdrawn route (09-10 09:48–09:53, before the 13:12 re-pin) and are graded, so they are **reported with a caveat, not re-run** |
| ±skill dev cell (gemini): with skill 2/5 vs without 3/5 | report, don't bury | n = 5, noise-compatible, direction backwards |
| Claude Code comparison arm: 2/5 dev | footnote | first in cut order |
| gpt-5.2-pro: 1 episode, PASS 6/6 on HZB_FLEX, $29.21 and 4.6 h | footnote only | n = 1 is not a pass rate |

### C5. Trainability (M8): what the gate found, and what is still running

See §5. **No trained model exists.** The outline's §6 is written around a result that the running sweep may reverse.

---

## 3. Canonical numbers, quick reference

| Quantity | Value | Source |
|---|---|---|
| Rollouts/s/core (1e5) | 30.3 (acceptance) · 29.4 (clean venv) | acceptance.json; PLAN |
| MCP tools | 22 | server.py |
| Components introspected | 374 | PLAN M1 |
| Env acceptance instances / records | 104 / 312 | acceptance.json |
| Red-team corner sweep | 56 actions, 0 false flags, max util 1.016, 16 band catches | red-team note |
| Evidence files committed | 1082 | `find benchmark/evidence -type f` |
| Matrix evidence dirs | 253 (main 124, oneshot 119, claude_code 5, noskill 5; includes dev tasks) | `benchmark/evidence/m6/` |
| Held-out dirs | 28 (7 models × 2 tasks × 2 arms) | `benchmark/evidence/m6_final/` |
| Valid scored-set matrix episodes | 225 | RESULTS |
| OpenRouter spend | $122.16 | RESULTS |
| T2 classical improvement | 2.95× / 1.47× | `t2_baseline_arms.json` |
| Pilot leak audit | 4/7 leaked | leak-audit note |

---

## 4. Retractions and corrections (the integrity paragraph in Limitations)

Say these plainly; the outline already commits to it.

1. **Infra counted as capability** (caught 2026-09-10 by the taxonomy pass): 77 of 281 episodes were infrastructure failures scored as zeros (52 `ConnectError` from a dead SSH tunnel, 24 provider 404/429, 1 harness). The "qwen3-32b 0/17" row was entirely artifact. Fix: INFRA exclusion, pre-flight endpoint probe, re-runs ($0.08).
2. **"One-shot beats the tool loop for strong models"** (Aug 23 headline) → died on statistics (all p ≥ 0.70).
3. **"A 22-tool surface defeats weak models"** (maverick 0 tool calls) → our harness `break`-ed on prose-without-tools. After the fix: 132 MCP calls across the same 17 episodes, still 0 passes.
4. **Pilot 4/4 PASS** → mechanics-only after the leak audit.
5. **Phase-0 uncalibrated "trainability headroom"** (8B 80% / 32B 93%) → ceiling artifact of `target_ratio = 1.0`.

---

## 5. M8: the trainability evidence, in order, with the live caveat

**Pre-registered gate (rev 2 plan, 2026-09-12):** before any spend, check that the untrained 32B is meaningfully above the untrained 8B on held-out procedural instances. Otherwise "approaching a larger untrained model" has no target.

| Run | n per family | 8B pass | 32B pass | Paired | File |
|---|---|---|---|---|---|
| Uncalibrated (target_ratio 1.0) | 15 | **0.80** (guide 0.60, SANS 1.00) | **0.933** (guide 0.867, SANS 1.00) | 30 pairs: 23 both, 1 only-8B, 5 only-32B, 1 neither; McNemar p = 0.219 → underpowered, not null | `phase0_n15.json`, `paired_uncalibrated.json` |
| Calibrated (0.8× classical optimum) | 10 | **0.70** (guide 0.9, SANS 0.5) | **0.70** (guide 0.6, SANS 0.8) | 20 pairs: 10 both, 4 only-8B, 4 only-32B, 2 neither; agreement 0.60; McNemar p = 1.0; per family guide 3–0 to 8B, SANS 1–4 to 32B | `phase0_calibrated.json`, `paired_calibrated.json` |
| **Difficulty sweep (earlier partial run, same deterministic instances)** | 25 | 0.8×: **0.60** · 1.0×: **0.36** · 1.2×: **0.06** | 0.8×: **0.76** · 1.0×: **0.58** · 1.2×: pending | not yet computed | `sweep_partial_2026-09-13.log` |
| **Difficulty sweep (running now)** | 25 | 0.8×: 0.60 (guide 0.72, SANS 0.48, identical to the partial run) | … | … | `sweep.log` → `sweep.json` |

Also from the calibrated phase 0: RAFT keep rate on the 8B train split 10/20 (guide 8/10, SANS 2/10); gate `raft_feasible: false`. Medians: steps to success 3.5 (8B) vs 2.0 (32B); FOM/target median 1.12 vs 1.19, max 1.36 vs 3.58.

**What this means for drafting (important):**

- The outline's §6 and claim-inventory rows **"8B ≡ 32B … SOLID"** and **"no capability ordering at all"** rest on the n = 10/family phase 0. At n = 25/family the sweep log already prints `SEPARATES` at both 0.8× (+0.16) and 1.0× (+0.22). The phase-0 tie looks like **small-n noise in the guide family** (8B guide 0.9 on the first 10 instances vs 0.72 on 25; 32B guide 0.6 vs 0.76).
- So the evidence currently leans **H_saturated** (a harder bar separates scale), not **H_flat** (the task never discriminates). If it holds, the "dense feedback makes the task non-discriminating" argument in the outline is **wrong** and must not appear. The sweep does restore a real target for the claim bar (at 1.0× the 32B is at 0.58 vs 0.36), but there is **no trained checkpoint**, and the M8 hard dates (training started by Sep 14 EOD, evaluated by Sep 16 EOD) are close.
- **Do not draft §6 until `runs/m8/sweep.json` exists and the paired McNemar at each bar is in.** Robust to either outcome: the calibration lesson (uncalibrated ladders manufacture ceilings: 80%/93% → 70%/70%), and the gate-before-spend methodology.
- The paired-vs-marginal point ("same marginal rate, 4–4 discordant") is still a good methods illustration. But it is no longer evidence of "no ordering" once the larger-n numbers separate.

---

## 6. Discrepancies to fix before citing

| # | Conflict | Where | Resolution |
|---|---|---|---|
| D1 | **Scored-set size 17 vs 16.** Config's scored sets = 12 seen + 2 T2 + 3 perturbed = 17, but `T2_guide_divergence` is *also* in `dev_split`, and `results_report.py` drops dev tasks → every cell has valid n = 16. RESULTS prose (report lines 212, 276) still says "17 tasks", and PLAN/OUTLINE quote 1/17, 3/17, 5/17. | RESULTS, PLAN, OUTLINE | Decide whether T2_guide is scored or dev. If dev: fix the report's prose, and the paper says 1/16, 3/16, 5/16 and "T2 = 1 scored task + 1 dev task". |
| D2 | **Episode count 271 vs 225.** 271 = 253 matrix dirs (including dev, claude_code, noskill) + 18 valid held-out; 225 = valid scored-set matrix only. | OUTLINE vs RESULTS | Define both in the paper: "N episodes run, M scored". |
| D3 | **Failure-kind counts, three versions:** RESULTS 38 format / 51 incomplete / 96 physics · OUTLINE "42 format / 118 physics" · PLAN M7 "55 format vs 98 physics over 204". | | Cite RESULTS only (current, and the only one with the incomplete class). Fix the OUTLINE. |
| D4 | **Maverick held-out one-shot cells are stale in evidence.** `runs/m6_final/` holds the recovered episodes (DigitalOcean, graded, translate failure → L1); `benchmark/evidence/m6_final/` still holds the old Google-404 infra reports, so RESULTS Table 2 shows 0 valid / 2 infra and the "10 infra-excluded" total. | evidence vs runs | Run `preserve_evidence.py`, then `results_report.py`. Should give maverick one-shot 2 valid, 0 pass; infra 10 → 8. Verify after regenerating. |
| D5 | Spend $122.16 (RESULTS) vs $121.25 / $121.17 (PLAN) vs "$122" (OUTLINE). | | Cite RESULTS. |
| D6 | M8 "8B ≡ 32B, SOLID" vs sweep separation at n = 25. | OUTLINE §6, claim table; PLAN recap; M8 note §10 | See §5. Hold. |
| D7 | Held-out loop count 7/8 (4 API models, RESULTS) vs 7/10 (PLAN, includes maverick loop 0/2). | | Paper: state which models are in the paired set. |
| D8 | **"~300-line minimal loop"** (scaffold note) / "~280 lines" (PLAN): `agent.py` is now 465 lines, 384 non-blank non-comment. | | Say "a single ~400-line module" or drop the line count. |
| D9 | CLAUDE.md header "14 T1 + 2 T2" vs catalog 19 T1. PLAN M6 bullet "qwen3-32b ZERO valid" superseded by 3/16. PLAN "166 tests" vs 224 now. | docs | Doc hygiene, not paper-blocking. |
| D10 | SCOPE.md still states the trainability claim bar as a planned result. | SCOPE | Update after the M8 decision (dated note). |

---

## 7. Open items that block or shape the draft

1. **M8 sweep outcome + decision** (§5): H_saturated → is there still time for any RAFT/SFT run (Sep 14 EOD start gate)? Or report the calibrated difficulty-response curve as the trainability-*readiness* result, without a delta?
2. **Prior-art re-check (MANDATORY, stale since 07-09).** The positioning claim is "first executable environment for *neutron instrument design*". Check at least: MDGYM; LLM agents for scientific simulation environments; physics-verifiable RL environments; McStas/neutron ML papers from 2025–26. No related-work bibliography exists in the repo yet.
3. **D1 (16 vs 17) and D4 (stale evidence)** before any table is typeset.
4. **qwen held-out cells** (8, infra): re-run when endpoints free up, or state the exclusion. The narrative must not rely on a qwen held-out row.
5. **Ablations listed in PLAN M7 are unchecked** (structured vs raw, ±introspection, ±skill, ±vision, cost curves). Only ±skill exists, at n = 5. Either scope them out explicitly or drop them from the contribution list.
6. **T3** stays outside the scored set (no expert rubric). Describe it as defined, unscored.
7. No LaTeX project exists yet (`paper/` only has `OUTLINE.md`). Need the ICLR 2027 template.

---

## 8. Figures and tables → data source

| Item | Content | Source | Ready? |
|---|---|---|---|
| Fig 1 | env loop (task → agent → MCP → executor → ladder → anti-hacking → eval record / training) | `paper/OUTLINE.md` mermaid draft | draft |
| Fig 2 | failure-level distribution per model × scaffold (stacked bars, L0–L4 + PASS) | RESULTS Table 1 / `m6_results.json` | yes (after D1/D4) |
| Fig 3 | M8 difficulty-response curve (pass rate vs target fraction, both models, paired markers) | `runs/m8/sweep.json` | **pending** |
| Fig 4 (opt.) | Liouville utilization: guide-family plateau vs bound | `runs/redteam/sweep.json` | data exists |
| Tab 1 | catalog + contamination controls (seen / held-out / perturbed / probes) | tasks/, contamination/ | yes |
| Tab 2 | main matrix, loop vs one-shot | RESULTS T1 | after D1 |
| Tab 3 | held-out final pass + difficulty confound | RESULTS T2 + power table | after D4 |
| Tab 4 | red-team findings (finding → symptom → fix → regression test) | red-team note | yes |
| Tab 5 | T2 classical baselines vs best agent partial | RESULTS T5 | yes |
| Tab 6 | statistical power / what is not resolvable | RESULTS power table | yes |

---

## 9. Wording rules carried from the decision notes

- "The first executable environment for **neutron instrument design**", never "first executable scientific environment". Pending the prior-art check.
- No LLM judge anywhere in T1/T2 grading; grade the built artifact, never the agent's claims.
- Held-out loop advantage: always with p = 0.041 **and** the difficulty confound p = 0.0009.
- T2: "unsolved by every model in every arm", and classical search solves it. Frame this as headroom plus a statement that the architecture delegates continuous optimization to scipy.
- Qwen arms: state YaRN (factor 4.0, required to reach the 77k measured peak) and enforced non-thinking. YaRN alters the model relative to stock.
- Story emphasis: environment + contamination + reward integrity + failure analysis. Tooling (MCP server, skill) is the reference baseline, not a contribution by itself.
