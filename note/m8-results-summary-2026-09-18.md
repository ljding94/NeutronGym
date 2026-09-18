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
physics formula reaches 27.7% and the best fixed answer 4.0%, and where the
untrained 32B is no better than the untrained 8B. Three rejection-sampling SFT
runs on the same environment all regressed the model.

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

- **No policy ever passes on turn 1** (0/300, every arm). Targets cannot be hit
  without measuring.
- Trained models use a **median of 5 turns**; untrained arms burn all 10.
- **Nearly every pass is a distinct design**: 225/230 (seed 1), 203/207 (seed 2).
- On the selection slice the trained model beat the physics formula on 155
  instances while the formula beat it on 30.
- Re-simulated at fresh random seeds, 72/82 passing designs still match (88%).
- The two seeds agree in magnitude but solve different instances (seed-1-only
  60, seed-2-only 37, agreement 0.68, p = 0.025): **the effect replicates, the
  exact instance set does not.**

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

1. **One family carries the design claim.** `guide_match` is the only family
   that passes the copy-type probes. SANS is parked as evaluation-only (its
   feedback prints the pinhole limit, so a copy rule reaches 56%); a
   `sans_match` variant was prototyped and rejected for now: intensity noise
   between seeds is 4% median / 11% max, needing ~5x the rays.
2. **Tolerance-edge fragility:** ~12% of passing designs stop matching at a
   fresh seed (72/82 hold).
3. **Seed-to-seed instance sets differ** (agreement 0.68) even though the
   magnitude replicates.
4. **Checkpoint selection used held-out 0–299**, so only the 300–599 numbers
   are unbiased for the chosen model. Both are reported.
5. **A feedback-label bug affected every M8 number before 2026-09-16:** the
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
