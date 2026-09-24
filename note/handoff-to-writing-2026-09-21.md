# Handoff to the writing session — 2026-09-21

Everything that changed on 2026-09-20/21. Read this **before** drafting from
`note/m8-results-summary-2026-09-18.md`; that note is still authoritative and
has been updated, but this file says what is *new*, what is *retracted*, and
what must not be repeated from earlier drafts.

Short version: two new results (a fourth trained family and a joint
multi-family run), one new methodological probe, and **four claims retracted**.

---

## 1. New result — fourth archetype, `bender`

A curved neutron guide: curvature makes a geometric low-pass filter whose
cutoff is set by radius, channel width and coating,
`lam_c = sqrt(2w/r) / (Qc/4pi * m)`. Targets are the transmitted beam's
**centre of mass (±0.25%)** and **width (±1%)** in wavelength. Fourth
instrument, still a target-matching family.

Held-out 300–599, 10 turns, 0 errored:

| policy | passed | 95% CI (Clopper–Pearson) |
|---|---|---|
| untrained Qwen3-8B | 69/300 (23.0%) | [18.4, 28.2] |
| untrained Qwen3-32B | 131/300 (43.7%) | [38.0, 49.5] |
| **GRPO-trained 8B** | **227/300 (75.7%)** | [70.4, 80.4] |

Paired (exact McNemar): **163 trained-only vs 5** against the untrained 8B
(p = 5.8e-42); **125 vs 29** against the **32B** (p = 2.1e-15). Median
turns-to-solve 4, against 6 (untrained 8B) and 5 (32B).

Degeneracy probes: best fixed design **8.0%** (upper95 12.6%), baseline
never passes, **no copy-type readout rule applies**, analytic cutoff
inversion **12.7%** (upper95 18.0%, reference arm — not gated).

**Headline consequence:** the project now has **four gated families, two
task formulations, four instruments** (guide, SANS, chopper pair, bender).
The old caveat "one family carries the design claim" is dead — update any
draft text that still says it.

---

## 2. New result — joint multi-family training

One policy, one LoRA, trained on the **pooled** states of the three
previously gated families, at **the same 220-step budget one specialist
gets** — so a third as many steps per family. Evaluated on each family's
held-out 300–599 against that family's own 220-step specialist:

| family | untrained 8B | specialist | **joint** | joint vs specialist (paired) |
|---|---|---|---|---|
| guide_match | 11.3% | 76.7% | **67.7%** | 40 vs 67, p = 0.012 |
| sans_match | 22.3% | 38.3% | **24.0%** | 21 vs 64, p = 3.3e-6 |
| tof_chopper | 2.0% | 46.7% | **55.0%** | 40 vs 15, **p = 0.001 (joint wins)** |

> **Corrected 2026-09-24 (caught by the revision session).** The `sans_match`
> specialist cell in the table above is the **220-step** checkpoint
> (`eval_sansmatch_fresh_trained-8b.json`, 38.3%), not the reported
> specialist, which is the **340-step** continuation: **51.0%** on 300–599
> (`eval_sansmatch_fresh_trained340.json`) and 44.3% on its unbiased slice
> 600–899. So joint4's 43.3% is **below** its specialist, not above, and any
> delta computed from 38.3% here is wrong. See
> `note/joint-training-summary-2026-09-23.md`, which is authoritative.

Joint vs untrained: guide_match 178-only vs 9 (p = 6.8e-42), tof_chopper
159-only vs 0 (p = 2.7e-48), sans_match **28 vs 23 (p = 0.58 — no learning
at all)**.

Two claims, and the second is the interesting one:

1. **Aggregate efficiency.** Mean 48.9% against the specialists' 53.9% —
   **91% of the performance for a third of the training compute** (220 steps
   against 660, ignoring the 340-step continuation sans_match needed).
2. **Transfer runs opposite to difficulty.** `tof_chopper` — the family that
   starts at 2.0%, where episode collection found only 18 of 300 passing and
   bootstrapping was hardest — is the one pooling **helps**, significantly.
   `sans_match`, whose specialist plateaued and needed a continuation, learns
   **nothing** pooled. Reading: other families' states supply the early
   gradient a *sparse* family cannot generate for itself, while a family
   whose per-step signal is *weak* is simply crowded out of the budget.

That framing — multi-task RL substitutes for missing exploration signal and
competes for gradient budget, and which dominates depends on whether a
family's difficulty is sparsity or weak signal — is more defensible than
"joint training works", and it is supported by all three arms.

**Caveat for the paper:** this run predates bender, so it pools three
families, not four. Either scope the claim to three or wait for a rerun.

---

## 3. New methodology — the design-concentration probe

`benchmark/harness/design_concentration.py`. This is a genuine contribution
and belongs in the methods section, because it found a flaw the existing
probe suite structurally could not.

**The gap:** the constant-policy gate bounds what a single **fixed** design
can pass. It cannot bound what a **family** of designs can pass. `bender` v1
passed the gate at 10.7% while a quarter of its instances fell to *any*
sufficiently transparent design — a class, not a point, so no candidate the
gate tried ever scored high.

**The probe:** for every passing episode, record the design that passed, then
report distinct designs per pass, the top-5 share, the maximum reuse, and how
much of the target range the most-reused design covers.

| family / arm | passes | distinct | top-5 | max reuse |
|---|---|---|---|---|
| guide_match, trained | 230 | **225 (98%)** | 4% | 2 |
| guide_match, untrained 8B | 34 | 29 (85%) | 29% | 2 |
| tof_chopper, trained | 140 | **138 (99%)** | 5% | 2 |
| tof_chopper, untrained 32B | 36 | 36 (100%) | 14% | 1 |
| sans_match, trained | 115 | 80 (70%) | 16% | 5 |
| bender v1, trained *(retracted family)* | 141 | **42 (30%)** | 43% | 17 |
| bender v2, trained | 227 | 63 (28%) | 32% | 18 |

**Do not read this table as "bender v2 is still broken".** Concentration is
bounded below by a task's effective degrees of freedom, and the separating
measurement is the *physical* DOF rather than the target:

- bender v2's most-reused design has `lam_c = 6.18 Å`; the 18 instances it
  solves cluster at **sd 0.31 against the population's 1.51 — five times
  tighter**. It solves instances that genuinely need its cutoff.
- It covers **18/300 = 6.0%**, *below* the 8.0% the constant gate's best
  fixed design reaches. The policy's most-reused answer underperforms the
  best constant.
- A bender's two observables are **both** determined by `lam_c`, so the
  family has **one effective degree of freedom** where guide_match has two.
  Low concentration is intrinsic there.

**Rule to state:** compare a family's concentration against **its own
constant gate**, not against another family. Low concentration is a flag to
investigate, not a verdict. What condemned v1 was not the 30% but that one
design spanned nearly the whole target range while cutting none of the band.

---

## 4. Retractions — claims that must NOT appear

These were stated during the day and are now known to be wrong. If any
survives into a draft it will be a correctness error.

1. **`bender` v1's numbers (17.3% → 47.0%) are void.** The family was
   degenerate: **24.7% of held-out instances had a hidden cutoff below the
   incident band**, so the bender did nothing and the targets were just the
   source spectrum's own mean and spread. Another 21.3% cut less than a
   quarter of the band. Hidden designs had been drawn uniformly from the
   parameter box. Fixed by a `hidden_filter` hook requiring the hidden
   design to cut **25–85%** of the band. Use v2 numbers only.
2. **"bender shows the largest scale effect in the project (3.2x)."** False.
   On the repaired family the 32B advantage is **1.9x** (43.7% vs 23.0%).
   Part of what looked like the 32B's physics was it exploiting the
   degenerate slice — its v1 concentration was 72% distinct with max reuse
   8, worse than any clean family. *Note:* v1 and v2 are different task
   distributions, not a subset relation — the new signature re-drew every
   instance's hidden design.
3. **"A closed-form inversion barely works on bender (6.7%)."** False on the
   repaired family: **12.7%** (upper95 18.0%), which makes it the
   **strongest** physics rule in the project, not the weakest (tof_chopper's
   scored 0.0%, guide_match's 6/16 at ±5%). Still under the 20% ceiling and
   still a reference arm, not a gate. The v1 framing that the environment
   rewards iteration because one-shot physics fails by 8x is **not
   supported** — make that argument from the multi-turn evidence instead.
4. **"No turn-1 passes" must be scoped.** bender v2's trained policy has
   **18 turn-1 passes** (both untrained arms: 0). With a one-dimensional
   answer drawn from a concentrated distribution, a learned prior near the
   median sometimes lands without feedback. State the signature as a claim
   about guide_match, sans_match and tof_chopper.

---

## 5. New caveats for the paper's limitations section

Added to `note/m8-results-summary-2026-09-18.md` §Caveats as items 6–7:

- **The constant-policy gate bounds fixed answers, not families of answers.**
  bender v1 passed at 10.7% while a quarter of its instances fell to a class
  of designs. It was caught by the trained policy's design concentration, not
  by the probe. Report this as a limitation of the methodology *and* report
  the diagnostic that worked.
- **The old caveat 2 ("one family carries the design claim") is out of date.**
  Four families are now gated. SANS *collimation* remains evaluation-only.

---

## 6. Infrastructure (reproducibility footnote at most)

Three fixes, worth a sentence only if the paper claims reproducible runs:

- **Atomic calibration cache.** `open(path, "w")` truncates before writing, so
  a concurrent reader saw a zero-byte file and a reward-server worker died of
  `JSONDecodeError`, taking a training run with it. Now temp-file +
  `os.replace`, with unreadable treated as absent (the entry is a pure
  function of the instance, so recomputing is always correct).
- **Reward-server fault isolation.** One bad item can no longer forfeit a run:
  `score_item` never raises, `post_json` retries, and the failure rate is
  surfaced as `err_frac` in the step line rather than silently scored 0.
- **Episode dedup.** `load_states` drops duplicate `(family, index)` episodes,
  which would otherwise be sampled at double weight.

---

## 7. Evidence

- Results note (authoritative): `note/m8-results-summary-2026-09-18.md`
- bender v2: `runs/m8/eval_bender_fresh_{untrained-8b,untrained-32b,trained-8b}.json`,
  `runs/m8/match_gate_bender_n150.json`,
  `runs/m8/readout_probe_bender_0.85_n150.json`
- bender v1 (retracted, kept for the degeneracy analysis): `runs/m8/bender_v1/`
- Joint: `runs/m8/eval_joint_{guide_match,sans_match,tof_chopper}.json`
- Concentration: `runs/m8/concentration_*.json`
- Prior art: `note/prior-art-recheck-2026-09-20.md` — **RLVP (arXiv 2607.10474)
  is structurally our recipe**; claim the environment, the multi-turn design
  loop and the probe methodology, not "GRPO on a verifiable physics reward".

---

## 8. Open items

1. **Joint training excludes bender** — its inputs changed today. Scope the
   joint claim to three families, or rerun over four.
2. **`sans_match` sits at 70% concentration with max reuse 5**, between the
   clean families and bender. Not explained. Consistent with it being the
   weakest-signal family and the one that fails under joint training, but
   that connection is a conjecture, not a measurement.
3. **bender's 18 turn-1 passes** are unexplained beyond the 1-DOF argument.
