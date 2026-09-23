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

**The recipe generalises across three gated families and two task
formulations**: `sans_match` 26.0% → 44.3% and `tof_chopper` 2.0% → 46.7%. On
all three the trained 8B also beats the untrained 32B.

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

The trained model degrades gracefully while the untrained one collapses. But
the knob does **not** buy headroom against frontier models: at ±2%,
claude-sonnet-5 and gemini-3.6-flash both still score 97/100 (against 99 and
98 at ±5%), while the untrained 8B falls to 3/100 and the trained 8B to
68/100 on those same instances. Tightening tolerance separates small models;
challenging a frontier model would need a structurally harder family (more
coupled parameters or competing objectives), not a finer bar.

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

## Second family: `sans_match` (2026-09-19)

Built after the fact to test whether the recipe generalises past one family,
and gated before training. Targets are two geometric widths — the beam at the
sample and the unscattered beam at the beamstop plane — matched within ±1.5%.
The tolerance was chosen by measurement, not after seeing a result: the
untrained 8B passes 45.3% at ±3%, 24.7% at ±1.5% and 18.0% at ±1%, and ±1.5%
keeps the ~5x margin over the 0.3% width noise that `guide_match` has.

Gate at the graded bar (n=150, 184 candidates): fixed designs, the baseline
and every other instance's solution as a lookup reach **12.7% (upper95
18.0%)** — best is another instance's hidden design, `{r_pin1: 0.013794,
r_pin2: 0.019028}`, at 19/150. The baseline never passes, no copy-type rules
apply, and the physics width-inversion reaches **3.3% (upper95 6.9%)**.
Verdict CLEAN, but by the narrowest margin of any family: **18.0% upper
against the 20% ceiling**.

> **Corrected 2026-09-22.** This note previously said 6.7% (upper95 11.1%)
> and 0.7% for the physics rule. Those values match no run on the DGX: the
> only `sans_match` gate on record (`runs/m8/sans_match_gate_dgx.log`,
> 2026-09-20 12:16) gives 12.7% / 18.0% / 3.3%, and the frozen evidence copy
> agrees. The earlier figures appear to predate the family's final
> tolerance. **Anything drafted from the old numbers understates
> `sans_match`'s shortcut ceiling by roughly 2x and should be re-checked.**

Held-out 300–599, ±1.5%, 10 turns, 0 errored:

| policy | passed | 95% CI |
|---|---|---|
| untrained Qwen3-8B | 67/300 (22.3%) | [17.8, 27.5] |
| untrained Qwen3-32B | 79/300 (26.3%) | [21.4, 31.7] |
| **GRPO-trained 8B** | **115/300 (38.3%)** | [32.8, 44.1] |

Paired: 70 trained-only vs 22 against the untrained 8B (p = 5.3e-7), and 59 vs
23 against the untrained **32B** (p = 8.7e-5) — the trained small model beats
the larger untrained one here, as it does on `guide_match`.

Training had not converged at 220 steps, so it continued to 340:

| checkpoint | held-out 300–599 (selection) | held-out 600–899 (unbiased) |
|---|---|---|
| untrained 8B | 67/300 (22.3%) | 78/300 (26.0%) |
| GRPO 220 steps | 115/300 (38.3%) | — |
| **GRPO 340 steps** | **153/300 (51.0%)** | **133/300 (44.3%)** |

As on `guide_match`, comparing checkpoints used the 300–599 slice, so the
chosen model was re-measured on 600–899, which played no part in the choice:
**26.0% → 44.3%**, paired 86 trained-only vs 31 (p = 3.7e-7). Quote the
unbiased pair as the second-family headline.

Honest comparison with the first family: the gain is **+18 points, not +65**,
and the model still uses most of its turn budget (median 10 on the unbiased
slice, 7 on the selection slice, against 5 for `guide_match`). Passes remain
instance-specific: 115 distinct designs among 133 passes, and on the selection
slice the most-reused design solves 5 of 300 instances (1.7%, against the
gate's 12.7% ceiling).

**What this supports:** the recipe transfers to a second, independently gated
family — not that it transfers with the same magnitude. On both families the
trained 8B also beats the untrained 32B.

## Third archetype: `tof_chopper` (2026-09-20)

The first family in the **time domain** and the first built on a third
instrument. A chopper pair monochromates a continuous beam: the phase
difference between the disks selects which velocity arrives in the open
window (lambda = 3956 * dt / L_ch, dt = phase / (360 * nu)), and the second
disk's opening angle with the frequency sets the spread. Targets are the mean
wavelength (±2%) and the spread (±5%) at the sample — per-observable bars,
because the two differ 15-fold in simulation noise (0.07% vs 1.1%).

Gate (n=150): fixed designs, the baseline and every other instance's solution
reach **2.7% (upper95 6.0%)**, the baseline never passes, and the physics
inversion rule reaches **0.0%** — the cleanest family in the project.

Held-out 300–599, 10 turns, 0 errored:

| policy | passed | 95% CI |
|---|---|---|
| untrained Qwen3-8B | 6/300 (2.0%) | [0.7, 4.3] |
| untrained Qwen3-32B | 36/300 (12.0%) | [8.6, 16.2] |
| **GRPO-trained 8B** | **140/300 (46.7%)** | [40.9, 52.5] |

Paired: 135 trained-only vs 1 against the untrained 8B (p = 3.1e-39) and 116
vs 12 against the **32B** (p = 1.6e-22). 138 distinct designs among 140
passes; no turn-1 passes; 124 of 140 solved on turn 4 or later.

Three things make this the strongest single result:

- **The largest relative gain (23x)** and the lowest starting point — episode
  collection found only 18 of 300 episodes passing, so GRPO bootstrapped from
  almost nothing. That is evidence the ladder's partial credit carries the
  learning when passes are rare, matching the sparse-reward ablation.
- **The first family where model scale clearly helps** (32B 12.0% vs 8B 2.0%,
  6x), so a capability axis exists here that the earlier families lacked —
  and training an 8B still beats the 32B four-fold.
- **No shortcut we can construct solves it**, including our own physics rule.

One honest cost: the trained policy reaches further and sometimes over-reaches.
28 of its 300 episodes end at level 2 (a design that runs but starves the
monitor below the statistics floor), where the untrained model never does —
it stays near the baseline and fails safely. Trading some validity for reach
is visible in the level histogram: trained {L2 28, L3 132, L4 140} against
untrained {L3 294, L4 6}.

## Fourth archetype: `bender` (2026-09-20/21)

A curved neutron guide. Curvature makes a geometric low-pass filter whose
cutoff is fixed by radius, channel width and coating:
lam_c = sqrt(2w/r) / (GAMMA * m), GAMMA = Qc / 4pi. Targets are the
transmitted beam's **centre of mass (±0.25%)** and **width (±1%)** in
wavelength. The tight bars are deliberate: at ±2%/±5% a single constant
design passed 4 of 8 probe instances. The incident spectrum varies per
instance (lam0 3.0–7.5 Å, spread drawn as a *fraction* of lam0 — drawn
independently it produced negative wavelength ranges Source_simple refuses
to run).

### v1 is retracted: a quarter of its instances were degenerate

v1 trained 17.3% → **47.0%** (paired 112 trained-only vs 23, p = 3.0e-15)
but did **not** beat the untrained 32B's 55.7% (36 vs 62, p = 0.011). The
diagnosis of *why* is the result worth keeping.

Three signatures were off. The trained policy used only **42 distinct
designs for 141 passes** (tof_chopper: 138 of 140), **43% of its passes came
from five designs**, and it produced **17 turn-1 passes** where every earlier
family had zero. One geometry passed 17 instances whose target means spanned
**4.2–8.2 Å** — nearly the whole held-out range, which no single cutoff can
match through the physics.

Cause: hidden designs were drawn uniformly from the parameter box, and for a
bender that is wrong. **24.7% of held-out instances got a hidden cutoff
below the incident band**, so the bender did nothing and the targets were
just the source spectrum's own mean and spread — any transparent design
solved them. Another 21.3% cut less than a quarter of the band.

**This is the degeneracy the probe suite exists to catch, caught late.** The
constant gate passed v1 at 10.7% because a *single* design must also match
the spread target, which varies with the incident band; the degenerate slice
was reachable by a *class* of designs, not by one. That is a real gap in the
methodology and belongs in the paper: a constant-policy gate bounds fixed
answers, not families of answers.

Fix: a `hidden_filter` family hook requiring the hidden design to cut
**25–85%** of the incident band (the upper bound keeps the monitor off the
statistics floor). Only bender's signature moves; the other three families
keep all 4,710 cached calibrations. v1 artifacts: `runs/m8/bender_v1/`.

### v2 (2026-09-21) — the family result

| probe | v1 | **v2** |
|---|---|---|
| best fixed design | 10.7% (upper95 15.8%) | **8.0% (upper95 12.6%)** |
| baseline | 0 | **0** |
| copy-type readout rules | none apply | **none apply** |
| analytic cutoff inversion (reference, not gated) | 6.7% (upper95 11.1%) | **12.7% (upper95 18.0%)** |

Held-out 300–599, 10 turns, 0 errored:

| policy | passed | 95% CI |
|---|---|---|
| untrained Qwen3-8B | 69/300 (23.0%) | [18.4, 28.2] |
| untrained Qwen3-32B | 131/300 (43.7%) | [38.0, 49.5] |
| **GRPO-trained 8B** | **227/300 (75.7%)** | [70.4, 80.4] |

Paired: **163 trained-only vs 5** against the untrained 8B (p = 5.8e-42) and
**125 vs 29** against the **32B** (p = 2.1e-15). Median turns-to-solve 4
(untrained 8B 6, 32B 5). So the trained 8B beats the untrained 32B here
after all — the v1 failure to do so was an artefact of the degenerate slice.

Scale gap on the repaired family: 1.9x (32B 43.7% vs 8B 23.0%), not v1's
3.2x. **Do not describe bender as the project's largest scale effect.**

**Design concentration needs a family-specific reading here, and this is
the subtle part.** The trained policy uses 63 distinct designs for 227
passes (28%), against guide_match's 98% and tof_chopper's 99% — on its face
the same signature that exposed v1. It is not, and the measurement that
separates them is the *physical* degree of freedom rather than the target:

| | v1 (degenerate) | **v2 (sound)** |
|---|---|---|
| most-reused design | 17 instances | 18 instances |
| span of target means it covers | ~the whole held-out range | 60% |
| hidden lam_c of *all* instances | — | 2.48–9.96 Å, sd 1.51 |
| hidden lam_c of the instances it solves | — | 5.04–6.57 Å, **sd 0.31** |

The v2 design has lam_c = 6.18 Å and the instances it solves cluster **5x
more tightly** around that value than the population does. It is solving
instances that genuinely need its cutoff. And it covers **18/300 = 6.0%**,
*below* the best fixed design the constant gate found (8.0%) — the trained
policy's most-reused answer underperforms the best constant.

The reason concentration is intrinsically low for this family: a bender's
two observables are both determined by the single quantity lam_c, so the
task has **one effective degree of freedom** where guide_match has two.
A one-dimensional answer drawn from a distribution with sd/mean ~29% means
any given design necessarily covers several instances. **Compare bender's
concentration against its own constant gate, not against guide_match.**

Honest residual: 18 turn-1 passes (untrained arms: 0). With a
one-dimensional answer whose population is concentrated, a learned prior
near the median sometimes lands without feedback. Worth stating; it is why
the "no turn-1 passes" signature is a claim about the other families.

## `sans_match` vs the untrained 32B — the last unpaired cell (2026-09-21)

Measured on the **unbiased** slice 600–899, the same slice and settings as
the other two arms (target fraction 0.85, 10 turns, temperature 0):

| policy | passed | 95% CI |
|---|---|---|
| untrained Qwen3-8B | 78/300 (26.0%) | [21.1, 31.4] |
| untrained Qwen3-32B | 87/300 (29.0%) | [23.9, 34.5] |
| **GRPO-trained 8B** | **133/300 (44.3%)** | [38.6, 50.2] |

Paired: **89 trained-only vs 43** against the 32B (p = 7.7e-5), and 86 vs 31
against the untrained 8B (p = 3.7e-7).

**This completes the claim on all four families**: a GRPO-trained 8B beats
an untrained 32B on guide_match (76.7% vs 11.3%), sans_match (44.3% vs
29.0%), tof_chopper (46.7% vs 12.0%) and bender (75.7% vs 43.7%), each
paired on one slice with exact McNemar.

Second finding from the same run: **scale buys nothing on `sans_match`** —
32B vs 8B is 37 vs 28 discordant, p = 0.32 (29.0% vs 26.0%). So sans_match
joins guide_match (11.3% vs 11.3% on the fresh slice) as a family where
model scale does not help, against tof_chopper (6x) and bender (1.9x) where
it does. Worth one sentence: the environment's families differ in whether
they reward scale, and the two that do not are the two oldest formulations.

## Joint training over four families — compression (2026-09-22)

Supersedes the framing in the three-family section below. Same 220-step
budget one specialist gets, now split four ways. Held-out 300–599:

| family | untrained | specialist | joint3 | **joint4** | joint4 vs specialist |
|---|---|---|---|---|---|
| guide_match | 11.3% | 76.7% | 67.7% | **63.7%** | 31 v 70, p = 1.3e-4 |
| sans_match | 22.3% | 38.3% | 24.0% | **43.3%** | 62 v 47, p = 0.18 (ns) |
| tof_chopper | 2.0% | 46.7% | 55.0% | **56.3%** | 41 v 12, p = 8.2e-5 |
| bender | 23.0% | 75.7% | — | **55.0%** | 26 v 88, p = 4.6e-9 |

Ordered by specialist performance, the sign pattern is exact:

| family | specialist | joint4 | Δ |
|---|---|---|---|
| sans_match | 38.3% | 43.3% | **+5.0** |
| tof_chopper | 46.7% | 56.3% | **+9.6** |
| bender | 75.7% | 55.0% | **−20.7** |
| guide_match | 76.7% | 63.7% | **−13.0** |

**The two families a specialist does worst on gain; the two it does best on
lose.** Joint training lands every family in a 43–64% band regardless of
where its specialist sits: the spread across families falls from **38.4
points to 20.4, a 47% compression**, while the joint policy keeps **92% of
mean specialist performance at a quarter of the compute** (220 steps against
4 × 220). One policy converges toward a common competence level rather than
specialising.

This rests on four families with a clean sign ordering, so it replaces the
sparsity-versus-weak-signal reading below, which rested on a contrast
between two and on a cell that has not replicated. **Do not write "pooling
erases learning on the weakest-signal family":** `sans_match` is the
*lowest*-specialist family, so compression predicts a gain, and joint4 shows
one (+5.0). Only joint3 showed a collapse.

`tof_chopper`'s gain replicates across both configurations — 55.0% and
56.3%, statistically indistinguishable (22 v 18, p = 0.64) — despite getting
a quarter of the budget rather than a third.

**RESOLVED 2026-09-23, and it overturns the framing above.** The
three-family replication at seed 20260922 did not replicate — it *degraded*:
guide_match 34.0%, sans_match 19.3%, **tof_chopper 2.3%** against an
untrained rate of 2.0%. Same families, same 8,592 states, same budget and
hyperparameters; only the seed differed.

So the recipe is **bimodal across seeds**: the identical configuration gave
55.0% and 2.3% on `tof_chopper`. Cause, from the step logs: through stage B
the replication ran at 2.1–2.5 of 8 groups carrying gradient signal (joint3:
4.5–4.9), so most groups had zero advantage and the policy drifted. It was
behind already — its stage-A checkpoint scores 11.0% on `tof_chopper`.

**Every per-family joint claim is therefore withdrawn, including the
compression framing above**, which was drafted from `joint4` alone before
this run landed. Only the 3-family configuration was run twice; `joint4` has
one seed and is not known to be the stable configuration.

What survives: a single pooled policy reached 63.7 / 43.3 / 56.3 / 55.0% on
the four families at a quarter of a specialist's compute, in one run. See
**`note/joint-training-summary-2026-09-23.md`** for the complete account and
a suggested paragraph — that note is authoritative for anything joint.

## Joint multi-family training (2026-09-21)

One policy, one LoRA, trained on the pooled states of all three gated
families at **the same 220-step budget a single specialist gets** — so it
sees a third as many steps per family. Evaluated on each family's held-out
300–599, against that family's own 220-step specialist:

| family | untrained 8B | specialist | joint | joint vs specialist (paired) |
|---|---|---|---|---|
| guide_match | 34/300 (11.3%) | 230 (76.7%) | **203 (67.7%)** | 40 vs 67, p = 0.012 |
| sans_match | 67/300 (22.3%) | 115 (38.3%) | **72 (24.0%)** | 21 vs 64, p = 3.3e-6 |
| tof_chopper | 6/300 (2.0%) | 140 (46.7%) | **165 (55.0%)** | 40 vs 15, p = 0.001 |

Joint vs untrained: guide_match 178-only vs 9 (p = 6.8e-42), tof_chopper
159-only vs 0 (p = 2.7e-48), sans_match **28 vs 23 (p = 0.58 — no learning
at all)**.

Two findings, and the second is the interesting one:

- **Pooling is compute-efficient in aggregate.** Mean pass rate 48.9%
  against the specialists' 53.9% — 91% of the performance for a third of
  the training compute (220 steps against 660, and that ignores the
  340-step continuation sans_match needed as a specialist).
- **Transfer is uneven, and it runs opposite to difficulty.**
  `tof_chopper` — the family that starts at 2.0%, where episode collection
  found only 18 of 300 passing and bootstrapping was hardest — is the one
  pooling *helps*, significantly (55.0% vs 46.7%). `sans_match`, the family
  whose specialist plateaued and needed a continuation to 340 steps, learns
  **nothing** when pooled. So the states of other families supply the early
  gradient a sparse family cannot generate for itself, while a family whose
  per-step signal is weak rather than sparse simply gets crowded out.

That pair is worth stating plainly: multi-task RL here is not a uniform
win or loss. It substitutes for missing exploration signal and it competes
for gradient budget, and which effect dominates depends on whether a
family's difficulty is *sparsity* (helped) or *weak per-step signal*
(hurt).

## Design concentration — the diagnostic the gate cannot replace (2026-09-21)

`benchmark/harness/design_concentration.py`, run on every family's passing
episodes. The constant-policy gate bounds what a single **fixed** design can
pass; it cannot bound what a **family** of designs can pass. Bender v1
passed the gate at 10.7% while a quarter of its instances fell to any
sufficiently transparent design — a class, not a point, so no candidate the
gate tried scored high.

| family / arm | passes | distinct designs | top-5 share | max reuse |
|---|---|---|---|---|
| guide_match, trained | 230 | **225 (98%)** | 4% | 2 |
| guide_match, untrained 8B | 34 | 29 (85%) | 29% | 2 |
| tof_chopper, trained | 140 | **138 (99%)** | 5% | 2 |
| tof_chopper, untrained 32B | 36 | 36 (100%) | 14% | 1 |
| sans_match, trained | 115 | 80 (70%) | 16% | 5 |
| **bender v1, trained (retracted)** | 141 | **42 (30%)** | **43%** | **17** |
| bender v1, untrained 32B | 167 | 120 (72%) | 18% | 8 |
| bender v2, trained | 227 | 63 (28%) | 32% | 18 |

Reading: the two families that carry the headline are essentially fully
instance-specific (98–99% distinct). `sans_match` sits in between at 70%
with a max reuse of 5 — worth stating, and consistent with it being the
family whose signal is weakest and the one that fails to learn under joint
training.

**bender v2's 28% is not the same failure as v1's 30%**, and the table
alone cannot tell them apart — see the `bender` section for the separating
measurement. In short: v2's most-reused design covers 6.0% of instances
against the constant gate's own 8.0%, and the instances it solves cluster
5x more tightly in the physical cutoff `lam_c` than the population does,
whereas v1's spanned nearly the whole target range while cutting none of
the band. **Read a family against its own gate, not against another
family** — concentration is bounded below by the task's effective degrees
of freedom, and bender has one where guide_match has two.

**Use it as a standing check, not a post-hoc one.** It costs nothing (it
reads the eval file that already exists) and it is the only probe here that
sees class-degeneracy. Artifacts: `runs/m8/concentration_*.json`.

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
6. **The constant-policy gate bounds fixed answers, not families of
   answers.** `bender` v1 passed it at 10.7% while 24.7% of its instances
   were solvable by *any* sufficiently transparent design — a class, not a
   single point, so no one candidate the gate tried scored high. It was
   caught by the trained policy's design concentration (42 distinct designs
   for 141 passes, one solving 17 instances across nearly the whole target
   range) rather than by the probe. **State this as a limitation of the
   methodology and report the diagnostic that actually worked**: the
   distribution of distinct passing designs per instance, which is now worth
   computing for every family, not just the suspect one.
7. **Caveat 2 is out of date** (written when guide_match stood alone). Four
   families are now gated: `guide_match`, `sans_match`, `tof_chopper`, and
   `bender` (v2). SANS *collimation* remains evaluation-only.
8. **A feedback-label bug affected every M8 number before 2026-09-16:** the
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

**Regenerate the whole four-family table from the frozen copies** — no
access to the DGX required, which is how a reader checks the paper's
numbers:

    python benchmark/harness/m8_table.py --root benchmark/evidence/m8_rl/eval

Every rate, exact Clopper–Pearson interval and paired McNemar p in §7 comes
out of that command. It covers all four families and pairs each trained arm
against **both** untrained arms.

Added 2026-09-21:
- `runs/m8/eval_bender_fresh_{untrained-8b,untrained-32b,trained-8b}.json` —
  fourth family (69/131/227 of 300)
- `runs/m8/match_gate_bender_n150.json` (fixed design 8.0%),
  `readout_probe_bender_0.85_n150.json` (analytic inversion 12.7%)
- `runs/m8/eval_sansmatch_unbiased_untrained-32b.json` — the last unpaired
  cell (87/300 on the unbiased slice 600–899)
- `runs/m8/eval_joint_{guide_match,sans_match,tof_chopper}.json`,
  `grpo_jointgrpo_{a,b}.log` — joint multi-family training (three families;
  **predates bender**)
- `runs/m8/concentration_*.json` — design-concentration probe, all families
- `benchmark/evidence/m8_rl/bender_v1_retracted/` — the retracted first
  bender, kept so the §8 limitation is auditable. **No v1 number is a
  result.**

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
