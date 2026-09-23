# Joint multi-family training — complete summary (2026-09-23)

Everything we know about the joint experiment, now that it is finished and
stopped. Supersedes the joint sections of
`note/handoff-to-writing-2026-09-21.md` (§2) and
`note/handoff-addendum-2026-09-22.md` (§3), **both of which state conclusions
that a later run contradicted.**

---

## 1. What the experiment is — and one thing to get right

**It is not a second stage on top of the per-family models.** Joint training
starts from the **same Qwen3-8B base** the specialists start from
(`--base <Qwen3-8B>`, no adapter loaded); only its own stage B initialises
from its own stage A. Nothing in it is warm-started from a specialist
checkpoint. If the draft says "a further stage after the individual RL runs",
that is wrong and a reviewer who reads the script will catch it.

The setup: pool the recorded decision points (states) from several families
into one training set, train **one** LoRA on the mixture, evaluate that
single model on each family's held-out instances.

The budget is the sharp part: **220 steps total, exactly what one specialist
gets**, split across 3 or 4 families. So it is compute-matched — one policy
and one training run against four policies and four training runs.

The question it answers: are the Table 1 results four narrow tricks, or one
transferable design habit?

---

## 2. Every run, with results

Held-out 300–599 for all families, 10 turns, temperature 0.

| run | families pooled | seed | guide_match | sans_match | tof_chopper | bender |
|---|---|---|---|---|---|---|
| *specialists (Table 1)* | — | — | 76.7% | 38.3%¹ | 46.7% | 75.7% |
| **joint3** (09-21) | 3 | default | 67.7% | 24.0% | 55.0% | — |
| **joint4** (09-22) | 4 | default | 63.7% | 43.3% | 56.3% | 55.0% |
| **joint3rep** (09-22) | 3 | 20260922 | **34.0%** | **19.3%** | **2.3%** | — |
| *untrained 8B* | — | — | 11.3% | 22.3% | 2.0% | 23.0% |

¹ `sans_match`'s specialist is 44.3% on its own unbiased slice 600–899;
38.3% is the same checkpoint on 300–599, which is the slice the joint runs
use, so it is the like-for-like comparison here.

`joint3` and `joint4` were only 3 and 4 families because bender did not exist
when `joint3` ran. `joint3rep` is an exact re-run of `joint3` — same
families, same 8,592 states, same budget, same hyperparameters — **with only
the seed changed.**

---

## 3. The recipe is bimodal across seeds

The headline fact, and the reason nothing below is claimed strongly:

> On `tof_chopper`, two runs of the *identical* configuration differing only
> in seed produced **55.0%** and **2.3%**. The untrained model scores 2.0%.

`joint3rep` did not merely underperform — it ended at the untrained rate on
one family and roughly halved on another. Diagnosis from the step logs:

| | joint3 | joint3rep |
|---|---|---|
| groups with signal, steps 1–120 | 4.4 → 4.0 / 8 | 4.4 → 4.7 / 8 |
| groups with signal, steps 121–220 | 4.5 → 4.9 / 8 | **2.5 → 2.1 / 8** |
| `pass_frac`, steps 171–220 | 0.067 | 0.019 |

Through the whole second stage the replication ran with most sampled groups
carrying no gradient at all (identical rewards within a group ⇒ zero
advantage), so the policy drifted rather than learned. Reward stayed flat
near 0.77–0.83, so this does **not** show up as a loud divergence — it is a
quiet failure that only the held-out evaluation reveals.

It was already behind before that. The replication's **stage-A checkpoint**
(120 steps) scores **11.0%** on `tof_chopper`, against a final `joint3` of
55.0%. So the two runs diverged early and stage B finished the job
(11.0% → 2.3%). Evidence: `runs/m8/eval_joint3repA_tof_chopper.json`.

**Caveat on scope:** only the 3-family configuration was run twice. `joint4`
has a single seed and was never re-run, so there is **no evidence that
4-family pooling is the stable configuration** — it simply has not been
tested. Do not imply otherwise.

---

## 4. What can and cannot be said

**Supported:**

- A single policy trained on the pooled states of four families at one
  specialist's budget reached **63.7 / 43.3 / 56.3 / 55.0%** on
  guide_match / sans_match / tof_chopper / bender — useful performance on
  every family from one training run at a quarter of the compute (220 steps
  against 4 × 220). *One run.*
- The same recipe is **unstable across seeds**: in the only configuration run
  twice, one run reached 55.0% on `tof_chopper` and the other 2.3%.
- The failure mode is identifiable from training telemetry alone —
  groups-carrying-signal collapsing to ~2/8 — which is worth stating because
  it means the failure is detectable without a held-out evaluation.

**Not supported — do not write these:**

- ❌ *"Pooling erases learning on the weakest-signal family."* From `joint3`
  alone. `sans_match` gained in `joint4` (+5.0 over its specialist).
- ❌ *"Pooling helps the family that cannot bootstrap its own gradient."*
  `tof_chopper` gained in `joint3` and `joint4` and collapsed to untrained in
  `joint3rep`.
- ❌ **The compression framing** (low-specialist families gain, high ones
  lose, 47% range compression). This was drafted on 09-22 from `joint4`
  alone, before the replication landed. It is n = 1 on a bimodal method.
- ❌ Any per-family joint number presented as characteristic of joint
  training rather than of one run.

---

## 5. Suggested paragraph

> We also asked whether one policy can cover all four families. Pooling the
> recorded decision points from every family and training a single LoRA from
> the base model at one specialist's budget (220 steps, split four ways)
> yields a policy scoring 63.7 / 43.3 / 56.3 / 55.0% on the four held-out
> families — useful competence everywhere from a quarter of the total
> compute. We caution against reading the per-family pattern: repeating the
> three-family configuration with a different seed produced a policy that
> scored 2.3% on `tof_chopper`, at the untrained rate of 2.0%, against 55.0%
> for the first run. Its training telemetry shows the cause — through the
> second stage, fewer than a quarter of sampled groups carried any gradient
> signal — so the instability is detectable during training, but it makes
> single-run per-family comparisons unreliable. We report the joint result as
> evidence that the environment admits a shared policy, not as a
> characterisation of multi-task transfer.

That paragraph is honest, it uses the strongest legitimate reading, and the
instability is a genuine negative result about multi-task RL in this setting
rather than a hole.

---

## 6. Evidence

- `runs/m8/eval_joint_{guide_match,sans_match,tof_chopper}.json` — joint3
- `runs/m8/eval_joint4_{guide_match,sans_match,tof_chopper,bender}.json` — joint4
- `runs/m8/eval_joint3rep_{guide_match,sans_match,tof_chopper}.json` — joint3rep
- `runs/m8/eval_joint3repA_tof_chopper.json` — joint3rep's stage-A checkpoint
- `runs/m8/grpo_{jointgrpo,joint4grpo,joint3rep}_{a,b}.log` — step telemetry
- `runs/m8/joint4_grpo_dgx.sh`, `joint3_replicate_dgx.sh` — the pipelines

Frozen copies under `benchmark/evidence/m8_rl/`. Table 1 and the probe suite
are separate experiments and are unaffected by anything here.
