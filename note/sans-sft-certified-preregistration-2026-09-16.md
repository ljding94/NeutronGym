# Pre-registration: RAFT SFT on the certified SANS family (2026-09-16)

**Status: WRITTEN BEFORE ANY DATA. Not started.** Nothing below may change
after collection begins. Deviations get appended as dated addenda, never edited
in place.

## Why this run exists

Every M8 result so far ran on the pre-repair families, and on those families a
single fixed action passed ~50% of held-out instances. The only positive-looking
gain (guide passing-turn SFT, 40.3% → 52.3%) was that fixed action. The repaired
SANS family is **certified**: the best fixed action passes 20/150 held-out
instances at 0.85× (13.3%, one-sided 95% upper bound 18.8%). A gain there cannot
be a lookup of one answer, so this run is the paper's only possible *valid*
positive training result. The method was chosen after seeing the guide results,
so the paper reports it as a **confirmatory test on the repaired family of a
method selected post hoc**.

## Frozen environment

- Family `sans_collimation`, signature **`8f8f7e3b09`**; record the git commit at launch.
- Bar **0.85×** (released `TARGET_FRACTION`), calibration v2 (protocol seed, common random numbers).
- No change to `src/neutrongym/` families, reward or calibration during the run. A change aborts the run.

## Data (self-generated RAFT)

- Policy: untrained Qwen3-8B, identical serving to all M8 runs (vLLM, YaRN 4.0, 98,304 ctx, non-thinking).
- Train split instances **0–599**, `m8_collect.py --families sans_collimation --target-fraction 0.85 --temperature 0.7 --max-steps 6`.
- Keep an episode iff it reaches **strict L4**.
- **Training rows: passing turn only** (`m8_train.py --turns passing`): one pair per kept episode.
- **Feasibility gate:** ≥ 100 kept episodes. Otherwise extend to instances 600–1199 once; if still < 100, stop and report "insufficient self-generated data".

## Training (identical to prior M8 runs)

LoRA r=32, α=64, all seven projections, lr 1e-4, 2 epochs, **16,000 tokens/step**
(explicit; the script default is now 32k), seed 20260913, max_len 8192.
Chat-template identity check against the live base server must pass.
Merge, then serve with the base 8B's exact vLLM flags. `m8_serve_check.py` must PASS.

## Evaluation

- **Held-out instances 0–299** at 0.85×, temperature 0, 6 steps.
- Arms: untrained 8B, trained 8B, untrained 32B (all run fresh).
- **No-model constant probe on the same 300 instances**, using the gate's full candidate set (grid + baseline + classical optima + refinement).

## Endpoints and decision rules (fixed now)

**Primary:** pass rate, trained vs untrained 8B, paired McNemar, two-sided α = 0.05.

The result is reported as **"SFT improved design on a certified family"** only if ALL hold:
1. trained − untrained 8B ≥ **+10 points**, McNemar p < 0.05;
2. trained pass rate exceeds the **best single fixed action's** pass rate on the same 300 instances, paired McNemar p < 0.05;
3. the trained model does not submit one identical action on every step in more than half of its episodes. This mirrors the guide ablation's failure signature and is checked with `m8_mechanism.py`.

Other readings, all reported:
- **Improved but not beyond a constant:** (1) holds but (2) fails.
- **No effect:** p ≥ 0.05 in either direction.
- **Regression:** untrained significantly better.

**Secondary (descriptive, no claims):** level migration (Cochran–Armitage), steps to success, FOM/target distribution, gap closed to the untrained 32B.

## Reporting commitment

Whatever the outcome, it goes into §7 of the manuscript with these rules. A
positive result is described as confirmatory on the repaired family; a null or
regression is described with the same weight.

## Compute and conflicts

- Local Mac (CPU): calibration of ~540 more train and ~150 more held-out SANS instances, RAFT collection, eval driver, constant probe. **This competes with the guide recertification (~14 h wall last time) for the same CPU.** Run one at a time.
- DGX: base 8B/32B already served on GPUs 0–4; LoRA + serving of the trained model on GPU 7 (free as of 2026-09-16). The local SSH tunnel must be up (currently down).

## Addendum 2026-09-16 (before any data): execution

- **Runs on the remote DGX, launched and monitored by the other session** (user decision). The "Compute and conflicts" section above assumed local CPU for calibration, collection and the probe; that no longer applies. The protocol, endpoints and decision rules are unchanged.
- The executing session should record the git commit, confirm family signature `8f8f7e3b09` at launch, and preserve evidence under `benchmark/evidence/m8/` as prior runs did.

## Addendum 2026-09-17 (written after the run, by the drafting session): what actually ran

The executing session ran this experiment as **"SANS v3h"** (commits 6bdf7c2 → 86c4fa8), with protocol changes that were not recorded here before data collection:
- calibration **v3** targets instead of v2;
- **numeric pinhole-limit hints** added to rejection feedback (315e26d);
- **10 turns** instead of 6;
- collection kept **146/600** episodes.

The endpoints were evaluated anyway:
- **untrained 8B 8.3% (25/300) → passing-turn SFT 1.0% (3/300)**; McNemar untrained-only 23 vs trained-only 1, p = 3e-6; Cochran–Armitage z = −3.21, p = 0.0013;
- untrained 32B 8.7%;
- best single fixed action 4.7%.

Under the rules above the reading is **Regression**; criterion 1 fails. Because the protocol deviated before data, the paper must describe this run as **exploratory, not pre-registered**. The report must also state that the pre-registered protocol as written was never executed.

The subsequent **guide GRPO run (7320ad1) was not pre-registered at all**. It is exploratory.
