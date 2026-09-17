# M8 result: step-level GRPO on the guide family (2026-09-17)

Status: final numbers for the guide GRPO run, with every baseline needed to
read them honestly. Evidence files are under `runs/m8/` (listed at the end).

## Headline

On 300 held-out guide instances (calibration-v3 targets, pass = FOM > 0.85 x
the per-instance classical optimum, 10 turns, temperature 0, corrected
feedback labels, 0 errored episodes in every arm):

| policy | passed | note |
|---|---|---|
| untrained Qwen3-8B | 54/300 (18.0%) | 73 instances never got a design past the spec checks |
| untrained Qwen3-32B | 232/300 (77.3%) | |
| best single fixed design (no model) | 10.7% (upper95 15.7%) | n=150, calibration-v3 diagnostic |
| best readout rule (no model) | 289/300 (96.3%) | `w_out`, `m_coat` at the limits stated in the prompt, `w_in = 0.06` |
| **GRPO-trained Qwen3-8B** | **296/300 (98.7%)** | |

Trained vs untrained 8B, paired on the same instances: 242 trained-only
passes, 0 untrained-only (McNemar p ~ 0); Cochran–Armitage z = 14.6.

## What the claim can and cannot be

**Supported:** from environment reward alone, step-level GRPO teaches the 8B
the physics-correct strategy for this family — push the coating and exit
width to the instance's stated limits and pick the entrance width — which the
untrained 8B applies 18% of the time and the untrained 32B 77%. The trained 8B
matches or slightly exceeds the best hand-coded rule (98.7% vs 96.3%); the
difference comes from choosing `w_in` per instance (0.045–0.07) instead of a
constant.

**Not supported:** general instrument-design skill. The guide family as
specified is solvable by a two-line readout rule, so a high pass rate here
cannot distinguish design reasoning from that rule.

Any table reporting this result should include the fixed-design and
readout-rule rows above.

## Verification that the result is not an exploit

- 240/300 solved on the first turn; 296 distinct passing designs (not a constant).
- Passing designs sit on each instance's own limits: `w_out` / beam-size
  limit median 1.000 (min 0.983); `m_coat` / coating limit median 0.998 (min 0.978).
- FOM / target median 1.115, max 1.209 — at most ~3% above the calibrated
  classical optimum on 9 instances, i.e. within calibration error.
- Re-simulated at 3 fresh seeds with 10x rays (60 instances): 176/180 still
  pass; median 0.974 of the calibrated optimum.
- Its 4 failures are also untrained-8B failures.

## Training setup

- Data: every episode of the untrained 8B on guide train instances 0–299
  (0.85x, 10 turns, T = 0.7), failures included: 300 episodes → 2,792
  decision states (48 episodes passed).
- Update: for each state, sample 8 completions (T = 1.0), score each action
  with the environment reward (0.75 + 0.25 x min(FOM/target, 2) when valid,
  lower levels 0–0.5), group-normalised advantage, k3 KL to the frozen base
  (beta 0.02), LoRA r = 32 on all projections, lr 1e-5, 120 steps x 8 states,
  gradient clip 1.0. 117 minutes on one A100-40GB.
- Training trend (10-step windows): mean reward 0.51 → 1.04; valid actions
  61% → 97%; sampled actions passing 0.3% → ~94%; KL/token rose to ~1.3–1.9.

## Context: SFT regressed three times first

| run | untrained 8B | trained 8B |
|---|---|---|
| guide, per-turn RAFT SFT (1.0x, v2 targets) | 40.3% | 31.3% (p = 0.0013) |
| SANS, passing-turn SFT (1.0x, v2 targets) | 31.0% | 27.0% (p = 0.004) |
| SANS, passing-turn SFT (0.85x, v3 targets, hints) | 8.3% | 1.0% (p = 3e-6) |

Traced mechanism for the last run: SFT on the model's own successful turns
learned "keep r_pin1, snap r_pin2 to the hinted limit" and resubmitted one
design for every remaining turn from states it never trained on (r_pin1 at
its minimum). GRPO trains on decisions from all states, failures included.

## Degeneracy findings that came out of this

- Readout-rule probe (new gate component, `hacks.readout_rules`), held-out
  n=150, best rule pass share: guide 98.0% at 0.85x, 87.3% at 0.9x, 57.3% at
  0.95x; SANS 56.0%, 50.0%, 39.3%. A stricter bar does not remove it.
- Removing the guide's hard limits makes its optimum interior, but one fixed
  design then reaches 0.85 of the optimum on 16/16 instances: guide flux is
  intrinsically flat in these parameters.
- Two feedback bugs were fixed before this run (and affect every earlier M8
  number): the per-turn FOM was reported as a fraction of the target but
  labelled "vs baseline", and exact resubmissions were not flagged.

## Evidence

- `runs/m8/eval_guide085grpo_n300.json` — combined three-arm eval + verdict
- `runs/m8/eval_guide085v3_arm_untrained-{8b,32b}.json`, `runs/m8/eval_guide085grpo_arm_trained-8b.json`
- `runs/m8/grpo_guide085grpo.log`, `runs/m8/guide_grpo_dgx.log` — training log and pipeline
- `runs/m8/guide_rule_policy_heldout.json` — readout rule, 300 held-out instances
- `runs/m8/readout_probe_{guide_divergence,sans_collimation}_0.85_n150.json` (+ 0.9, 0.95 on the DGX)
- `runs/m8/grpo_verify/` — verification, rule-policy and limit-free prototype scripts
- Checkpoint: `/netdisk/ldq/ckpt/m8-guide085grpo/merged` (served as `qwen3-8b-m8-guide085grpo` on DGX :8139)

## Addendum: guide_match — GRPO learns iterative design (2026-09-17)

The guide family above is solvable by a readout rule, so a second family was
built specifically to resist that: `guide_match`. Each instance states a
target spot size and divergence at the sample, taken from the simulated beam
of a hidden per-instance design; a pass needs both within +/-5%.

Gate (held-out n=150): fixed designs, the baseline, every other instance's
hidden design (lookup) and a refinement round — **best 4.0% (upper95 7.7%),
baseline 0 -> clean**. A one-shot geometric-optics inversion passes 31.3%; it
is reported as a reference arm rather than gated, since every physically
sensible few-parameter task admits such an estimate.

Held-out n=300, +/-5%, 10 turns, 0 errored episodes:

| policy | passed |
|---|---|
| untrained Qwen3-8B | 42/300 (14.0%) |
| untrained Qwen3-32B | 33/300 (11.0%) |
| one-shot physics formula (reference) | 94/300 (31.3%) |
| best fixed design / lookup (n=150) | 4.0% |
| **GRPO-trained Qwen3-8B (120 steps)** | **150/300 (50.0%)** |

Paired vs untrained 8B: 122 trained-only, 14 untrained-only (McNemar p ~ 0);
Cochran-Armitage z = 9.45.

What makes this different from the guide result:

- **No policy solves an instance on the first turn** (0/300 for every arm), so
  passes require measuring and adjusting. The trained model solves 118 of its
  150 on turn 4 or later.
- **It is not the physics formula:** on the same instances, trained-only 97,
  formula-only 41, both 53.
- **It is instance-specific:** 132 distinct passing designs out of 150.
- **Scale does not substitute:** the untrained 32B (11.0%) is no better than
  the untrained 8B (14.0%).
- Caveat: re-simulated at fresh seeds, 49/58 passing designs still match, so
  ~16% sit close enough to the tolerance edge to flip.

Training: 300 train instances -> 2,807 states, 120 steps x 8 states x 8
samples, 104 min on one A100-40GB; mean reward 0.79 -> 0.88, sampled-action
pass 1.6% -> ~12%, KL/token 0.78 at the end and still rising — the run had not
converged, so a longer continuation is under way from `adapter_step120`.
