# Paper evidence ledger — what the manuscript can cite (2026-09-13, rev 2026-09-14)

**Purpose:** one place to draft the ICLR 2027 paper from. Every claim here
has a number, a source file, and a strength label. Numbers were copied from
command output, not from memory. Where the project's own documents disagree,
the conflict is listed in §6. **Resolve §6 before citing an affected number.**

**Rev 2026-09-14:** M8 ran end to end overnight: the SFT stage exists and its
pre-registered result is a regression (§5). The held-out final pass is now
complete (28/28), the SANS reward hole was found and fixed (red-team finding
6), and the prior-art re-check is done (§7). The rev-1 sections that said
"M8 stopped by its gate" are superseded and have been removed.

Deadlines: **abstract 2026-09-18 · full paper 2026-09-25 AoE.**
Skeleton: `paper/OUTLINE.md` (§6 already rewritten for the SFT result).
Results of record: `benchmark/RESULTS.md` ← `benchmark/results/m6_results.json`
← `benchmark/evidence/`. M8 evidence: `benchmark/evidence/m8/`, `runs/m8/`,
dashboard `m8.html`.

Strength labels: **SOLID** (cite as is) · **SUGGESTIVE** (cite only with the
stated caveat) · **PENDING** (data still arriving) · **RETRACTED** (a claim we
made and withdrew; belongs in Limitations/integrity, never in results).

---

## 1. Status snapshot (2026-09-14 afternoon)

| Track | State | Source |
|---|---|---|
| Environment | shipped, acceptance passed, `neutrongym` 0.9.0 clean-venv verified; SANS L1 direct-beam check added 09-13 | `runs/env_acceptance/acceptance.json`, `tests/test_sans_static_check.py` |
| Benchmark matrix (M6) | complete: 225 valid scored-set episodes | RESULTS Table 1 |
| Held-out final pass | **complete: 28/28 valid, 0 infra, 0 leaks** | RESULTS Table 2 |
| M8 SFT (pre-registered) | **done: RAFT SFT regressed the 8B** (40.3% → 31.3%, p = 0.0013) | `runs/m8/eval_guide_1x_n300.json` |
| M8 post-hoc ablation (passing-turn SFT) | **STALLED.** Model trained and merged at 11:57 (`/netdisk/ldq/ckpt/m8-raft-guide1x-pass/merged`), but `eval_guide_1x_n300_passing.log` reads `ABLATION_SERVER_NEVER_UP_EVAL_NOT_LAUNCHED` (13:26). No eval has run | `runs/m8/train/passing/` |
| Prior-art re-check | **done** (`note/prior-art-recheck-2026-09-13.md`); neutron-design "first" survives, several framings must soften | §7 |
| Tests | 224 collected (as of 09-13) | pytest |

---

## 2. Contribution → evidence map

### C1. NeutronGym: an executable, physically verifiable environment for neutron instrument design

| Claim | Number | Strength | Source |
|---|---|---|---|
| Fast-tier throughput | **30.3 rollouts/s/core at 1e5 rays** (33 ms median); compile-once per family | SOLID | `acceptance.json`; `executor.py` |
| Why fast | mcrun ~2.4 s flat per rollout vs direct binary ~0.04 s | SOLID | PLAN M5 |
| Env acceptance | 104 instances × 3 actions = 312 records, all level-resolved; L0:104 / L1:0 / L2:59 / L3:119 / L4:30 | SOLID | `acceptance.json` |
| Clean install | fresh Py3.14 venv + wheel → 29.4 rollouts/s, exit 0 | SOLID | PLAN M5.5 |
| Reward ladder | L1 static (bounds + family static checks, e.g. SANS direct beam) → L2 truncated-ncount run → L3 FOM monitor, statistics floor, band constraints, Liouville → L4 FOM > target_ratio × baseline. +0.25/level, L4 adds 0.25·min(fom_ratio, 2), max 1.25 | SOLID as design. **Present as a level-resolved measurement tool, not new reward design** (OPTIAGENT precedent, §7) | `reward.py` |
| Procedural generation | 2 template families; deterministic (family, split, index); held-out = disjoint context regimes | SOLID. Only 2 families, and only guide is clean for M8 | `generate.py` |
| Per-instance calibration | target = fraction × constraint-filtered, Liouville-checked, fresh-seed-re-verified classical optimum (30 samples); calibration skips L1-rejected candidates | SOLID; "not found in this form" per the prior-art check | `calibrate.py` |
| Tooling baseline | 22 MCP tools (verified count), McStas 3.7.12 | SOLID. Tool-call validation has precedent (SIGA): cite it, don't claim it | `server.py` |
| Reference loop | model-agnostic, no shell or file Read, ±skill toggle | SOLID; size claim §6 D8 | `agent.py` |

### C2. McStasBench: benchmark slice with contamination controls

| Claim | Number | Strength | Source |
|---|---|---|---|
| Catalog | 19 T1 (12 seen + 2 dev + 2 held-out + 3 perturbed) + 2 T2 + 2 T3 + 3 pilots | SOLID | `benchmark/tasks/` |
| Self-validating | reference passes its own task at a fresh seed; T2 classical best passes and baseline fails | SOLID | `_validation.json` |
| Protocol | pass@1, temperature 0, 50-turn cap, env seeds, provider pinned | SOLID | `m6_config.json` |
| Probes (temp 0, pinned) | haiku none/16 · sonnet ILL_H10_IN8 + templateSANS /16 · flash-lite none/16 · gemini-3.6-flash ILL_H10_IN8 /16 · maverick incomplete | SOLID | RESULTS Table 4 |
| Probe drift at temp 0 | sonnet 1/16 → 2/16 between Aug 23 and the pre-freeze re-sweep | methods footnote | PLAN |
| Held-out instruments | BOYA/CARR (single-channel simplification, arXiv:2501.01143) and VENUS/SNS BL-10; authored by us, never public, **not facility-validated** | SOLID with caveats. **A modest distinction, not a headline** (§7) | task JSON |
| Perturbed pair-proofs | 3 variants (DMC, SANS2d, FLEX); perturbed reference self-passes and the canonical fails the variant; coverage 3/12 | SOLID; state the coverage | `T1_perturbed/_generation.json` |
| Sandbox origin | 07-30 audit: 4/7 pilot episodes fetched the reference through allowed tools | SOLID | leak-audit note |
| Leaks now | **0 leak-invalid across the whole campaign** | SOLID | RESULTS totals |

### C3. Red-teaming the reward — six findings

Sources: `note/reward-red-team-2026-08-05.md`, `note/sans-direct-beam-exploit-2026-09-13.md`, `runs/redteam/sweep.json`.

1. **Beamstop leakage** (T2 calibration): 672 n/s of direct beam; max-only width constraints have the wrong sign → band constraints.
2. **Optimizer bounds escape**: Nelder–Mead escaped to w = 2.1 m → filtered ensembles.
3. **Winner's curse**: best-of-30 fell below its own target at a fresh seed → fresh-seed re-verification.
4. **Monitor-matching decoy** (found by the loop): diagnostic monitor graded as detector; two models "failed" 3/6 → position-aware matching → PASS 6/6.
5. **Liouville gate false positive**: 1.14× on a ~250-event monitor → floor-gated.
6. **SANS direct-beam hole in the procedural family** (09-13, found *before* training by constant-policy probes): all-max pinholes, no model, pass **8/25** held-out SANS instances at 1.0× (one at FOM ratio **11,992×**). The direct beam radius at the stop is 0.038–0.049 m vs a 0.02 m stop; bands caught 15/25 leaking configs. **Most model SANS passes were the exploit**: 8B 10/25 passes, 8 leaking; 32B 16/25, 11 leaking. **22/35 cached SANS classical optima were leaks** (inflated targets). Fix: free L1 geometric check, train L_coll floor 2.0 → 2.5 m; post-fix all-max 0/25, baseline valid 25/25. Guide family clean (all-max 0/25, all-min 0/25).
   - **This is a recurrence of finding 1 in a new family**, and should be presented as such. The lesson: run constant-policy probes on every family before training. Cite Hack-Verifiable Environments and verifier fuzzing.

Liouville bound: **sharp for guides** (legit optimum 0.93–0.99× bound), **loose for SANS** and unable to catch finding 6 (the direct beam is physically allowed). Corner sweep: 56 actions, 0 false flags, max utilization 1.016, 16 band catches. Keep "sharp for guides, loose for SANS" verbatim.

### C4. Cross-model evaluation (M6)

Source: `benchmark/RESULTS.md` (generated 2026-09-13 19:01). **Totals:** 225 valid matrix · 28 valid held-out · 0 infra-excluded · 0 leak-invalid · $122.16.

**Seen scored set, valid n = 16 per cell (failures by deepest level L4/L3/L2/L1/L0):**

| model | loop pass | one-shot pass | loop failures | one-shot failures |
|---|---|---|---|---|
| claude-sonnet-5 | 5 | 7 | 1/0/0/9/1 | 0/1/2/6/0 |
| claude-haiku-4.5 | 4 | 4 | 4/2/2/4/0 | 0/0/0/12/0 |
| gemini-3.6-flash | 4 | 6 | 0/1/0/1/10 | 1/1/2/6/0 |
| gemini-3.5-flash-lite | 2 | 1 | 1/0/1/5/7 | 0/0/0/15/0 |
| llama-4-maverick | 0 | 2 | 0/2/0/1/13 | 0/0/0/14/0 |
| qwen3-32b | 3 | 0 | 0/0/0/0/13 | 0/0/0/0/16 |
| qwen3-8b | 1 | 0 | 0/1/1/0/13 | 0/0/0/0/16 |

**Held-out (2 tasks per cell):** loop passes: haiku 2, sonnet 2, gemini 2, flash-lite 1, maverick 0 (route-confounded), qwen-32b 2, qwen-8b 1. One-shot: sonnet 1, gemini 1, all others 0.

| Claim | Strength | Wording / caveat |
|---|---|---|
| **T2 unsolved by every model in every arm**; classical search 2.95× (guide, 628.6σ over initial) / 1.47× (SANS, 510.3σ) | SOLID | **Frame as replication** of arXiv:2601.07580 and 2606.21641 in a new substrate, not a discovery. Two baselines, not three columns |
| Zero reference leaks | SOLID | |
| Failure structure: one-shot failures pile up at L1 (won't compile), loop failures at L0 (tools engaged, nothing finished) | SOLID as distribution | core failure-mode figure |
| Failure kinds: matrix 38 format · 51 incomplete · 96 physics; held-out 5 · 3 · 8 | SOLID | |
| Open-weights ordering on seen tasks: 32B pass set ⊃ 8B pass set, incl. a canonical + perturbed twin | SOLID as ordering; rate p = 0.600 | |
| Open-weights produce nothing one-shot (0/16, all L0) | SOLID | format, not physics |
| Loop vs one-shot on seen tier | **not resolvable** (all p ≥ 0.70) | |
| **Loop > one-shot on held-out, paired over 7 models: 10/14 vs 2/14, Fisher p = 0.006** | SUGGESTIVE, confounded | always with: **held-out tasks are easier (loop arm 10/14 vs 20/113, p = 0.0001)**. Only 2 instruments |
| Serving-route variance (maverick Google vs DigitalOcean) | methods footnote | maverick held-out main cells keep the route-confound caveat |
| ±skill (gemini dev): 2/5 with vs 3/5 without | report, don't bury | n = 5 |
| gpt-5.2-pro 1 episode PASS, $29.21 | footnote | |

### C5. Trainability (M8): the pre-registered negative result

See §5.

---

## 3. Canonical numbers, quick reference

| Quantity | Value | Source |
|---|---|---|
| Rollouts/s/core (1e5) | 30.3 / 29.4 clean venv | acceptance.json; PLAN |
| MCP tools / components | 22 / 374 | server.py; PLAN |
| Red-team findings | 6 | §2 C3 |
| Evidence files (m6+m6_final+pilot, 09-13) | 1082 (m8 evidence added since) | `benchmark/evidence/` |
| Matrix evidence dirs | 253 (main 124, oneshot 119, claude_code 5, noskill 5; incl. dev) | evidence/m6 |
| Valid scored matrix / held-out | 225 / 28 | RESULTS |
| Spend | $122.16 | RESULTS |
| M8 eval | 8B 40.3% · trained 31.3% · 32B 54.7% (n = 300 paired) | eval json |

---

## 4. Retractions and corrections (the integrity paragraph)

1. **Infra counted as capability**: 77/281 episodes; the qwen3-32b "0/17" row was an artifact. Fixed with INFRA exclusion, a pre-flight probe and re-runs.
2. **"One-shot beats the loop for strong models"** → not resolvable statistically.
3. **"A 22-tool surface defeats weak models"** → our harness; maverick makes 132 calls after the fix, still 0 passes.
4. **Pilot 4/4 PASS** → mechanics-only after the leak audit.
5. **Uncalibrated M8 headroom** (8B 80% / 32B 93%) → ceiling from `target_ratio = 1.0`.
6. **"M8 stopped: 8B ≡ 32B, no ordering"** (09-13 afternoon) → artifact of n = 10 per family plus a RAFT keep check that demanded a 100% keep rate. Reversed the same evening.
7. **"Dense feedback makes the task non-discriminating"** → withdrawn (OPT-BENCH also finds stronger models exploit feedback better).
8. **The significant 32B lead in the combined sweep** (1.0×: 4v16, p = 0.012) → built on SANS leakage; leak-free SANS 0v3, p = 0.25.

Record of how the 8B–32B gap read as n grew (one sentence in methods, not a headline): tie at n = 10 → +22 at n = 25 → +7 at n = 100 (p = 0.35) → **+14 at n = 300 (p = 0.0001)**.

---

## 5. M8 — SFT trainability

### 5.1 Path to the pre-registered run

| Step | Result | Source |
|---|---|---|
| Uncalibrated phase 0 (n = 15/family) | 8B 0.80 / 32B 0.933: a ceiling | `phase0_n15.json` |
| Calibrated phase 0 (n = 10/family, 0.8×) | 0.70 / 0.70, McNemar 4v4 p = 1.0 → wrongly read as "no ordering" | `phase0_calibrated.json`, `paired_calibrated.json` |
| Difficulty sweep (n = 25/family) | combined 0.8×: 0.60 vs 0.74 (4v11, p = 0.12); 1.0×: 0.36 vs 0.60 (4v16, p = 0.012); 1.2×/1.4× at the floor. **Clean guide only:** 0.8× 0.72 vs 0.72 (p = 1.0); 1.0× 0.32 vs 0.48 (4v8, p = 0.39). All significant ordering came from leaky SANS | `sweep.json` |
| SANS leak probe (1.0×, n = 25) | leak-free passes 8B 2/25, 32B 5/25; paired 0v3, p = 0.25 → SANS excluded from M8 | `probe_sans_collimation_1x_n25.json` |
| Guide probe (1.0×, n = 100) | 40 vs 47, paired 17v24, p = 0.35, agreement 59% → the 32B cannot be the claim target at this n; claim = **≥ 10-point gain over the untrained 8B**. Power at 41% discordance → n = 300 | `probe_guide_divergence_1x_n100.json` |

### 5.2 Training setup (pre-registered)

- **Self-generated RAFT** (no distillation): untrained Qwen3-8B on guide train instances at 1.0×, strict-L4 keepers: **135/300 kept (0.45)**, zero SANS.
- 135 episodes → **422 per-turn pairs**, ~281k tokens, 13.1k target tokens; chat-template identity checked on all 422 prompts vs the live 8B server.
- LoRA r = 32, α = 64, all seven projections, lr 1e-4, 2 epochs, 16k tokens/step → 34 optimizer steps in 340 s on one A100-40G. Loss ~0.04–0.13, ending ~0.05.
- Merged checkpoint served with the base 8B's exact vLLM flags (YaRN 4.0, 98304 ctx, non-thinking). Pre-eval gate passed: identical `/tokenize`, JSON action parses.

### 5.3 Result (guide, 1.0×, n = 300 paired held-out instances, temperature 0, 6 steps)

| arm | pass | levels L2 / L3 / L4 | mean level | steps-to-success median | FOM/target median |
|---|---|---|---|---|---|
| untrained 8B | **40.3%** (121) | 57 / 122 / 121 | 3.213 | 3 | 0.975 |
| **RAFT-SFT 8B** | **31.3%** (94) | 85 / 121 / 94 | 3.030 | 4 | 0.920 |
| untrained 32B | **54.7%** (164) | 28 / 108 / 164 | 3.453 | — | — |

- Trained vs untrained 8B: pass gain **−0.090**; paired McNemar untrained-only 47 vs trained-only 20, **p = 0.0013**; agreement 0.777; level migration **downward**, Cochran–Armitage z = −2.95, **p = 0.0032**; gap to 32B closed **−0.628**. **Claim bar NOT met; significant regression.**
- 32B vs 8B: McNemar 38 vs 81, **p = 0.0001**, so the ladder had a real target and training moved away from it.
- Strength: **SOLID** (pre-registered endpoint, bar and n).

### 5.4 Mechanism (`benchmark/evidence/m8/mechanism.json`)

"It cloned the search, not the solution":
- Training data: 62/135 episodes open at the all-max corner (0.09, 0.09, 3.0); 25 of those keep w_in = 0.09 on every exploration turn. The modal *final* passing action lowers w_in: (0.05, 0.03, 2.5) in 39/135. 83/135 action sequences are distinct.
- Per-turn SFT weights all 422 turns equally, and exploration turns outnumber the single decisive turn per episode.
- Trained model opens at the corner on **20/20** held-out prompts (untrained 11/20); 26/30 replays go corner → w_out 0.03; w_in pinned the whole episode in 16/30; one exact 6-step trajectory recurs **6/30**; no training trajectory copied verbatim.
- All-max always fails the guide ladder (constant probe 0/25), so the trained policy burns step 1 of 6.
- Strength: SOLID descriptively. **Causal only if the ablation supports it.**
- Caveat: temperature-0 vLLM is not bit-deterministic (an ad-hoc replay read 12/20 and 5/30; cite the committed record).

### 5.5 Post-hoc ablation (pre-specified 09-14, before running)

- **Hypothesis:** the regression comes from over-weighting exploration turns.
- **Data:** same 135 episodes, keep only the L4 turn → 135 pairs.
- **Training and eval:** identical settings; same 300 instances; untrained arms reused.
- **Supported if** the passing-turn model does not regress vs the untrained 8B (harmful-direction McNemar p ≥ 0.05) **and** beats the per-turn model (paired p < 0.05). **Refuted if** it regresses as much.
- Always labelled post-hoc; the pre-registered result stays the headline.
- **Status: training + merge done; serving failed; eval not launched.** Readout script: `benchmark/harness/m8_ablation_readout.py`.

### 5.6 Methods lessons that stand regardless

- **Per-instance calibration:** uncalibrated targets manufactured a ceiling. Soften to "a measured instance of difficulty misalignment" and cite RLVE and GenEnv.
- **Constant-policy probes before training:** they caught finding 6.
- **Turn selection in multi-turn rejection sampling:** a candidate ML lesson, pending the ablation.
- **Small-n gates reverse:** limitations sentence only.

---

## 6. Discrepancies still to fix before citing

| # | Conflict | Resolution |
|---|---|---|
| D1 | **16 vs 17 tasks.** `T2_guide_divergence` is in both the scored T2 list and `dev_split`, so every cell has n = 16. RESULTS prose (Table 1 caption; power header "n=17") and PLAN/OUTLINE still say 17 / 1/17 / 3/17. **Still present in the 19:01 RESULTS.** | Decide scored vs dev; fix `results_report.py` prose; paper uses /16 |
| D2 | Episode count: OUTLINE and prior-art note say "271 scored episodes"; RESULTS now has 225 matrix + 28 held-out = 253, and the 253 matrix evidence dirs include dev/claude_code/noskill | Define "run" vs "scored" once and use it everywhere |
| D3 | Failure-kind counts differ across RESULTS / OUTLINE / PLAN M7 | Cite RESULTS only |
| D5 | Spend $122.16 vs $121.25 / $121.17 in PLAN | Cite RESULTS |
| D8 | "~300-line loop": `agent.py` is 465 lines (384 non-blank non-comment) | "~400-line module" or drop |
| D9 | CLAUDE.md "14 T1 + 2 T2"; PLAN M6 bullets with stale qwen numbers; SCOPE.md goal/RQ3 still promise a trainability delta | doc hygiene; SCOPE needs a dated-note update |
| ~~D4~~ | ~~Stale maverick held-out evidence~~ | **Resolved** 09-13 (28/28 valid) |
| ~~D6~~ | ~~"8B ≡ 32B" vs sweep~~ | **Resolved**: retracted (§4 item 6) |
| ~~D7~~ | ~~7/8 vs 7/10 held-out~~ | **Resolved**: now 10/14 vs 2/14 over 7 models |

---

## 7. Positioning after the prior-art re-check (`note/prior-art-recheck-2026-09-13.md`)

- **Survives:**
  - "To our knowledge, the first executable environment for **neutron instrument design**" (dated; McStas developers are already using AI agents).
  - No LLM judge in T1/T2.
  - The Liouville bound.
  - The six-finding red-team record.
  - The sandbox with zero leaks.
  - Per-instance calibration at a re-verified classical optimum.
- **Soften:**
  - Headline descriptor: "executable, physically verifiable environment and benchmark". **"RL environment" only if a training delta clears its bar**, and the pre-registered one did not.
  - Reward ladder → a measurement instrument (OPTIAGENT has a gated 4-level ladder).
  - T1 reproduce-from-paper is not new in kind (GRACE, Collider-Bench, SciReplicate-Bench).
  - T2 format is not new (Frontier-Eng); the T2 finding replicates 2601.07580.
  - The contamination architecture is rigorous application, not a novelty (BeyondBench, InfiniteScienceGym).
  - Validating tools: precedent in SIGA.
- **Largest risk:** MDAgent2, RLVP, SciAgentGym and OPTIAGENT already show *positive* training on physics-verified environments. With no delta, a reviewer asks why this environment should be believed trainable.
- **Double-blind:** cite SasAgent / EQSANS-CLI in the third person.
- A bibliography-ready must-cite list (~45 entries, each opened) is in the prior-art note.

---

## 8. Figures and tables → data source

| Item | Content | Source | Ready? |
|---|---|---|---|
| Fig 1 | env loop diagram | OUTLINE mermaid | draft |
| Fig 2 | failure-level distribution per model × scaffold | RESULTS T1 | yes (after D1) |
| Fig 3 | M8: level histograms untrained 8B / trained 8B / 32B + McNemar | `eval_guide_1x_n300.json` | yes |
| Fig 4 | mechanism: first-action distribution and w_in trajectories, train data vs trained vs untrained | `mechanism.json` | yes |
| Fig 5 (opt.) | difficulty-response curve, guide vs SANS raw vs leak-free | `sweep.json`, SANS probe | yes |
| Fig 6 (opt.) | Liouville utilization plateau for guides | `runs/redteam/sweep.json` | data exists |
| Tab 1 | catalog + contamination controls | tasks/, contamination/ | yes |
| Tab 2 | seen matrix | RESULTS T1 | after D1 |
| Tab 3 | held-out pass + difficulty confound | RESULTS T2 + power table | yes |
| Tab 4 | six red-team findings | red-team + SANS notes | yes |
| Tab 5 | T2 classical baselines | RESULTS T5 | yes |
| Tab 6 | M8 ablation (per-turn vs passing-turn vs untrained) | pending | **blocked on stalled eval** |

---

## 9. Wording rules

- "The first executable environment for **neutron instrument design**", to our knowledge, dated.
- No LLM judge in T1/T2; grade the built artifact, never the agent's claims.
- Held-out loop advantage: always with p = 0.006 **and** the difficulty confound p = 0.0001.
- T2 unsolved: as replication of prior LLM-vs-classical findings.
- Qwen arms: YaRN 4.0 + enforced non-thinking, identical for trained and untrained.
- M8: "pre-registered", "regressed", "claim bar not met". Any follow-up run is "post-hoc".
- Liouville: "sharp for guides, loose for SANS".
