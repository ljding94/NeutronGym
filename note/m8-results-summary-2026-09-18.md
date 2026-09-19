# M8 results — authoritative summary (2026-09-18)

For the writing session. Every number here is final, measured, and traceable
to a file under `runs/m8/`. The chronological log of how these came about is
`note/m8-grpo-guide-result-2026-09-17.md`; where the two differ, this file is
the summary to quote.

## Bottom line

Physics-verifiable environment reward **does** train a small model, but only
with on-policy RL, and only a task that resists shortcuts shows it as design
skill. Step-level GRPO takes Qwen3-8B from **11.3% to 76.7%** (replication:
69.0%) on a held-out slice of a target-matching family where a one-shot
physics formula reaches 27.7%, the best fixed answer 4.0%, classical search at
the agent's own simulation budget 13.3%, and the untrained 32B is no better
than the untrained 8B. Three rejection-sampling SFT runs on the same
environment all regressed the model, and removing the reward ladder's shaping
costs 60 points.

**Frontier models solve this family** (claude-sonnet-5 99%, gemini-3.6-flash
98% on 100 instances). So the environment discriminates model quality —
untrained 8B 12% → trained 8B 76% → frontier 99% on the same instances — and
the honest claim is that RL closes most of the small-model gap, **not** that a
trained 8B matches frontier models.

## Main result — `guide_match`, held-out instances 300–599

Fresh slice: it was not used to select checkpoints, hyperparameters or the
family design. Pass = simulated spot size AND divergence within ±5% of the
instance's stated targets, 10 turns, temperature 0, 0 errored episodes.

| policy | passed | note |
|---|---|---|
| untrained Qwen3-8B | 34/300 (11.3%) | |
| untrained Qwen3-32B | 34/300 (11.3%) | scale buys nothing here |
| best fixed design / lookup (no model) | 4.0% | n=150, includes every other instance's solution |
| one-shot physics formula (no model) | 83/300 (27.7%) | optics inversion, best of 36 variants |
| **GRPO-trained 8B (seed 1)** | **230/300 (76.7%)** | |
| **GRPO-trained 8B (seed 2)** | **207/300 (69.0%)** | independent repeat of the whole recipe |

Paired, same instances: seed 1 = 213 trained-only vs 17 untrained-only; seed 2
= 180 vs 7 (both McNemar p ~ 0).

**It is iterative design, not a rule or a lookup:**

- **No 8B or 32B policy ever passes on turn 1** (0/300, trained or untrained):
  at that scale targets cannot be hit without measuring. Frontier models do
  hit some instances cold (claude-sonnet-5 17/99, gemini-3.6-flash 1/98), so
  state the claim at the scale it holds for.
- Trained models use a **median of 5 turns**; untrained arms burn all 10.
- **Nearly every pass is a distinct design**: 225/230 (seed 1), 203/207 (seed 2).
- On the selection slice the trained model beat the physics formula on 155
  instances while the formula beat it on 30.
- Re-simulated at fresh random seeds, 72/82 passing designs still match (88%).
- The two seeds agree in magnitude but solve different instances (seed-1-only
  60, seed-2-only 37, agreement 0.68, p = 0.025): **the effect replicates, the
  exact instance set does not.**

## Matched-compute classical search (pre-registered)

Same 300 fresh-slice instances, the agent's own budget of 10 terminal
simulations, optimizers handed the parametrization the agent never sees, each
minimising the worst relative error the environment grades:

| policy | budget 10 | budget 30 (3x, sensitivity) |
|---|---|---|
| Nelder–Mead | 8/300 (2.7%) | 120/300 (40.0%) |
| random search | 22/300 (7.3%) | 60/300 (20.0%) |
| coordinate descent | **40/300 (13.3%)** | **221/300 (73.7%)** |
| GRPO-trained 8B (10 turns) | **230/300 (76.7%)** | — |

Paired vs the best arm at matched budget: 208 GRPO-only vs 18, p = 3.7e-42.
The pre-registered rule (best classical < 69.0% at budget 10) selects the
claim **"RL beats hand-coded physics and classical search at matched
budget"**. State the 3x column too: with three times the simulations,
coordinate descent reaches 73.7%, i.e. roughly the trained model's level — so
what RL buys is *search efficiency per simulation*, from natural language,
without being given the parametrization.

## Frontier models (pre-registered, 100 instances, held-out 300–399)

| policy (same 100 instances) | passed | 95% CI | turn-1 passes | cost |
|---|---|---|---|---|
| untrained 8B | 12/100 (12.0%) | [6.4, 20.0] | 0 | — |
| **GRPO-trained 8B** | 76/100 (76.0%) | [66.4, 84.0] | 0 | — |
| google/gemini-3.6-flash | 98/100 (98.0%) | [93.0, 99.8] | 1 | $1.21 |
| anthropic/claude-sonnet-5 | 99/100 (99.0%) | [94.5, 100] | 17 | $8.48 |

Paired vs the trained 8B: sonnet-5 24 model-only vs 1 (p = 1.5e-6);
gemini-3.6-flash 24 vs 2 (p = 1e-5). Errored episodes: 0 and 2 (both under the
10% infra-limited threshold). Total spend $9.69 of the $30 ceiling;
gpt-5.2-pro was therefore in budget but is not required for the claim.

**Reading.** The pre-registered "≥ 50%" branch says to report that the
environment discriminates model quality, which it does — 12% → 76% → 99%
across three capability levels on identical instances. But the branch's
phrasing ("the trained 8B reaches frontier-level performance") is **not**
supported: 76% vs 99% is a real gap, and frontier models also solve faster
(median 2.5–3 turns vs 4). Report the measured version: RL recovers about
three quarters of the untrained-8B-to-frontier gap, and the family is hard for
small models rather than hard in absolute terms.

## Reward-ladder ablation (pre-registered)

Identical recipe, states and seed; only the reward changed to pass/fail (1.0
for an L4 pass, 0.0 otherwise):

| reward | fresh-slice passes |
|---|---|
| level-resolved ladder | 230/300 (76.7%) |
| **pass/fail only** | **50/300 (16.7%)** |

60-point gap; paired 196 ladder-only vs 16 sparse-only (p = 1.5e-40) — past
the pre-registered 10-point bar, so **the ladder is load-bearing for
training**, not only a measurement device. The mechanism is the one the
pre-registration predicted: with pass/fail only, a mean of **0.33 of 8 groups
per step** carried any reward spread (ladder run: 5–7 of 8), because the
untrained model almost never passes, so almost every group is all-zeros and
contributes no gradient.

## Robustness, difficulty and budget (2026-09-18/19)

**Out of distribution.** A third split shifts every context axis beyond BOTH
train and held-out (longer guides, longer wavelengths, larger sources, further
sample distances), n=150:

| policy on OOD instances | passed | 95% CI |
|---|---|---|
| untrained 8B | 26/150 (17.3%) | [11.7, 24.4] |
| **GRPO-trained 8B** | **68/150 (45.3%)** | [37.2, 53.7] |

Paired: 57 trained-only vs 15 (p = 6.5e-7). Performance falls from 76.7% to
45.3% outside the training ranges but the advantage survives, so the policy
generalises the geometry rather than interpolating its training distribution —
and the honest framing is "degrades but holds", not "transfers intact".

**Difficulty knob.** The same fresh slice at tighter tolerances (targets do not
depend on the tolerance, so no recalibration):

| tolerance | untrained 8B | GRPO 8B | ratio |
|---|---|---|---|
| ±5% (reported) | 11.3% | 76.7% | 6.8x |
| ±3% | 7.0% | 73.3% | 10.5x |
| ±2% | 3.7% | 67.0% | 18x |

The trained model degrades gracefully while the untrained one collapses, and
the knob gives the benchmark headroom as models improve.

**Turn budget.** Every one of the trained model's 70 failures at 10 turns used
all 10, so the budget was doubled:

| budget | untrained 8B | GRPO 8B |
|---|---|---|
| 10 turns | 11.3% | 76.7% |
| 20 turns | 19.3% | 79.0% |

Only 9 of 237 solves used more than 10 turns (median 4). **The trained model's
remaining failures are not budget-limited — it plateaus rather than running
out of turns**, while the untrained model does benefit from more attempts.
Failure taxonomy at 10 turns: of 70 failures, 27 end within 10% of target,
36 within 10-25%, 7 beyond 25%, none without a valid design (median miss
12.2% against a 5% tolerance).

## Supporting results

**Transfer to a family it never trained on** (`guide_divergence`, held-out
0–299, 0.85x calibrated bar):

| policy on guide | passed | rejected before simulation |
|---|---|---|
| untrained 8B | 54/300 (18.0%) | 73 |
| **`guide_match`-trained 8B (transfer)** | **102/300 (34.0%)** | **20** |
| untrained 32B | 232/300 (77.3%) | 1 |
| guide-trained 8B (in-family) | 296/300 (98.7%) | 0 |

Paired vs untrained 8B: 76 transfer-only vs 28 untrained-only, p = 3e-6.

**More RL is not better.** Checkpoints compared on held-out 0–299: untrained
14.0% → 120 steps 50.0% → **220 steps 73.0%** → 340 steps **56.7%**, while
training reward kept rising (0.95, ~27% of sampled actions passing at step
339). Over-optimisation of the reward is visible before it shows in held-out
performance.

**SFT regressed three times on the same environment** (n=300 paired each):

| run | untrained 8B | trained 8B |
|---|---|---|
| guide, per-turn RAFT SFT | 40.3% | 31.3% (p = 0.0013) |
| SANS, passing-turn SFT (v2 targets) | 31.0% | 27.0% (p = 0.004) |
| SANS, passing-turn SFT (v3 targets) | 8.3% | 1.0% (p = 3e-6) |

Mechanism, traced by replaying episodes: cloning the model's own successful
turns teaches a rule conditional on states the model already handled ("keep
r_pin1, set r_pin2 to the hinted limit"), which becomes a trap elsewhere — the
trained model resubmitted one design for 8 straight turns from a state it had
never trained on. GRPO trains on decisions from all states, failures included.

## Environment methodology (the part that makes the numbers readable)

**Every family faces a no-model probe suite before its pass rates are read as
capability.** Judged on the one-sided 95% upper limit, ceiling 20%:

| family | fixed design / lookup | copy-type readout rule | physics-model rule (reference only) |
|---|---|---|---|
| `guide_match` | **4.0%** (upper95 7.7%) | none applicable | 27.7–31.3% |
| `guide_divergence` | 10.7% (15.7%) | **96.3%** — fails | — |
| `sans_collimation` | 14.0% (certified) | **56.0%** (62.9%) — fails | — |

- A *copy-type* rule needs no physics: copy a limit the prompt prints, reuse a
  fixed answer, or look up another instance's solution. These are gated.
- A *physics-model* rule (inverting the optics) is design knowledge; it is
  **reported as a reference arm, not used to disqualify**, since every
  physically sensible few-parameter task admits such an estimate.
- Guide's 98.7% must therefore always be quoted with its 96.3% readout row: it
  shows reward-driven strategy discovery, not design skill.
- Three maximisation designs were rejected by these probes before
  `guide_match` was built: guide with printed limits (readout 96–98%), guide
  with limits removed (one constant reached 0.85 of the optimum on 16/16), and
  a Bragg monochromator ("Bragg angle + open collimators" 12/12).

**Calibration and grading discipline** (all pre-existing, all load-bearing):
per-instance targets from a strong classical search scored at the agent's own
seed; targets for `guide_match` from a hidden per-instance design, with
candidates the baseline already matches rejected; statistics floor; fresh-seed
re-verification of any selected best; env-controlled protocol.

## Caveats to state in the paper

1. **Frontier models nearly solve the family** (98–99%), so it separates model
   capability but is not hard in absolute terms; the design claim is about
   small models learning from reward, not about task difficulty per se.
2. **One family carries the design claim.** `guide_match` is the only family
   that passes the copy-type probes. SANS is parked as evaluation-only (its
   feedback prints the pinhole limit, so a copy rule reaches 56%); a
   `sans_match` variant was prototyped and rejected for now: intensity noise
   between seeds is 4% median / 11% max, needing ~5x the rays.
3. **Tolerance-edge fragility:** ~12% of passing designs stop matching at a
   fresh seed (72/82 hold).
4. **Seed-to-seed instance sets differ** (agreement 0.68) even though the
   magnitude replicates.
5. **Checkpoint selection used held-out 0–299**, so only the 300–599 numbers
   are unbiased for the chosen model. Both are reported.
6. **A feedback-label bug affected every M8 number before 2026-09-16:** the
   per-turn figure of merit was reported as a fraction of the target but
   labelled "vs baseline". The three SFT runs and the guide GRPO run predate
   the fix; the `guide_match` results postdate it.

## Training configuration (identical for both seeds)

- **Data:** every episode of the untrained 8B on 300 train instances, failures
  included — ~2,810 decision states, of which ~40 episodes passed.
- **Update:** per state, sample 8 completions (T = 1.0), score each with the
  environment reward, group-normalised advantage, k3 KL to the frozen base,
  loss on completion tokens only.
- **Schedule:** LoRA r = 32 on all seven projections; 120 steps at lr 1e-5
  (KL 0.02), then 100 steps at lr 3e-5 (KL 0.01) from that adapter; 8 states
  per step; grad clip 1.0. ~3.5 h total on one A100-40GB.
- **Why two stages:** continuing at lr 1e-5 plateaued (steps 121–180: reward
  ~0.85, sampled pass 6–8%, KL flat); raising the rate resumed progress.

## Evidence index

Pre-registered experiments (2026-09-18, `note/prereg-matched-compute-frontier-ablation-2026-09-18.md`):
- `runs/m8/classical_guide_match_budget{10,30}_from300_n300.json` — matched-compute search
- `runs/m8/frontier_guide_match_n100_from300.json` — frontier arm with per-model cost
- `runs/m8/eval_match_fresh_sparse.json`, `grpo_matchsparse_{a,b}.log` — reward ablation
- `runs/m8/m8_table.json` — generated table with exact intervals

Main result (fresh slice 300–599):
- `runs/m8/eval_match_fresh_untrained-8b.json`, `..._untrained-32b.json`
- `runs/m8/eval_match_fresh_trained-8b.json` (seed 1), `runs/m8/eval_match_fresh_rep2.json` (seed 2)
- `runs/m8/readout_probe_guide_match_0.85_n300_from300.json` (physics reference)

Selection slice 0–299 and checkpoint curve:
- `runs/m8/eval_match_arm_untrained-{8b,32b}.json`
- `runs/m8/eval_match05grpo_n300.json` (120), `eval_match05grpoLR_n300.json` (220), `eval_match05grpo340_n300.json` (340)
- `runs/m8/match_gate_guide_match_n150.json` (fixed/lookup gate), `readout_probe_guide_match_0.85_n150.json`

Guide family and transfer:
- `runs/m8/eval_guide085grpo_n300.json`, `eval_guide085v3_arm_untrained-{8b,32b}.json`
- `runs/m8/guide_rule_policy_heldout.json` (readout rule 96.3%)
- `runs/m8/eval_transfer_matchLR_on_guide.json`

SFT regressions and SANS:
- `runs/m8/eval_guide_1x_n300.json`, `eval_sans_1x_n300_passing.json`, `eval_sans085v3h_n300_passing.json`
- `runs/m8/readout_probe_sans_collimation_0.85_n150.json`, `readout_probe_sans_split_n150.json`

Training logs, pipelines and verification scripts:
- `runs/m8/grpo_match05grpo{,LR,340}.log`, `grpo_matchrep2_{a,b}.log`, `grpo_guide085grpo.log`
- `runs/m8/match_grpo_dgx.sh`, `match_grpo_lr_dgx.sh`, `match_replicate_dgx.sh`, `match_fresh_slice_dgx.sh`, `guide_grpo_dgx.sh`
- `runs/m8/grpo_verify/` — exploit checks, rule policies, family prototypes

Checkpoints on the DGX: `/netdisk/ldq/ckpt/m8-match05grpoLR` (seed 1, 220
steps — the reported model), `m8-matchrep2-b` (seed 2), `m8-match05grpo`
(120), `m8-match05grpo340` (340), `m8-guide085grpo` (guide).

## Code entry points

- Family and reward: `src/neutrongym/generate.py` (`guide_match`),
  `src/neutrongym/reward.py` (`_score_match`), `src/neutrongym/calibrate.py`
  (`calibrate_match`)
- Probes: `src/neutrongym/hacks.py` (`readout_rules`, `split_rule_results`,
  `binomial_upper_bound`), `benchmark/harness/{readout_probe,match_gate}.py`
- RL: `benchmark/harness/{m8_states,reward_server,m8_grpo}.py`
- Evaluation: `benchmark/harness/m8_eval.py` (`--start-index` pins the slice)
