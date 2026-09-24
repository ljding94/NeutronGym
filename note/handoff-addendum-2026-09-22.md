# Addendum to the 09-21 handoff — 2026-09-22

Read `note/handoff-to-writing-2026-09-21.md` first; this only says what
changed since. One correction that affects text already drafted, one
completed experiment that reframes the joint-training paragraph, and one run
still going that decides whether a sentence survives.

---

## 1. Correction — a gate number in the draft matches no run

`sans_match`'s degeneracy gate was recorded in the results note as **6.7%
(upper95 11.1%)** with the physics rule at 0.7%. Neither value matches any
run on the DGX.

| probe | recorded | **actual** |
|---|---|---|
| constant / lookup gate | 6.7% (upper95 11.1%) | **12.7% (upper95 18.0%)** |
| physics width-inversion | 0.7% | **3.3% (upper95 6.9%)** |

The only `sans_match` gate on record — `runs/m8/sans_match_gate_dgx.log`,
2026-09-20 12:16, 184 candidates — gives 12.7%, the best candidate being
another instance's hidden design (`r_pin1 0.013794, r_pin2 0.019028`) at
19/150. The frozen evidence copy agrees with the log, so the *evidence* was
right and the *prose* was stale; the old figures appear to predate the
family's final ±1.5% tolerance.

**Why it matters beyond the digits.** The family is still CLEAN, but by the
narrowest margin of any: **18.0% upper against the 20% ceiling**, where the
note implied 11.1%. Any sentence that leans on `sans_match` being comfortably
clear of the ceiling needs softening. One downstream number was also wrong
and is fixed: the most-reused-design comparison now reads "1.7% against the
gate's 12.7% ceiling", not 6.7%.

**Action: grep the draft for `6.7`** — note that `16.7%` (the sparse-reward
ablation) and `46.7%` (tof_chopper) are unrelated and correct.

This class of error would not have been caught by recomputing from
per-instance rows, because gate values do not come from eval rows.

---

## 2. New experiment — joint training over all four families

The 09-21 handoff flagged that the joint run pooled three families and
predated bender. That run is now done at four, same 220-step budget one
specialist gets (so a quarter of the steps per family, not a third).

Held-out 300–599, all arms:

| family | untrained | specialist | joint3 | **joint4** | joint4 vs specialist | joint4 vs joint3 |
|---|---|---|---|---|---|---|
| guide_match | 11.3% | 76.7% | 67.7% | **63.7%** | 31 v 70, p = 1.3e-4 | 4 v 16, p = 0.012 |
| sans_match | 22.3% | 38.3% | 24.0% | **43.3%** | 62 v 47, p = 0.18 (ns) | 106 v 48, p = 3.4e-6 |
| tof_chopper | 2.0% | 46.7% | 55.0% | **56.3%** | 41 v 12, p = 8.2e-5 | 22 v 18, p = 0.64 (ns) |
| bender | 23.0% | 75.7% | — | **55.0%** | 26 v 88, p = 4.6e-9 | — |

Evidence: `runs/m8/eval_joint4_*.json`, `grpo_joint4grpo_{a,b}.log`.
> **Corrected 2026-09-24 (caught by the revision session).** The `sans_match`
> specialist cell in the table above is the **220-step** checkpoint
> (`eval_sansmatch_fresh_trained-8b.json`, 38.3%), not the reported
> specialist, which is the **340-step** continuation: **51.0%** on 300–599
> (`eval_sansmatch_fresh_trained340.json`) and 44.3% on its unbiased slice
> 600–899. So joint4's 43.3% is **below** its specialist, not above, and any
> delta computed from 38.3% here is wrong. See
> `note/joint-training-summary-2026-09-23.md`, which is authoritative.


---

## 3. The joint paragraph should be reframed — compression, not sparsity

The current paragraph says pooling helps the family that cannot bootstrap
its own gradient and erases learning on the weakest-signal family. The
four-family run supports a simpler and better-evidenced claim. Order the
families by how well their **specialist** does:

| family | specialist | joint4 | Δ |
|---|---|---|---|
| sans_match | 38.3% | 43.3% | **+5.0** |
| tof_chopper | 46.7% | 56.3% | **+9.6** |
| bender | 75.7% | 55.0% | **−20.7** |
| guide_match | 76.7% | 63.7% | **−13.0** |

The sign ordering is perfect: **the two families a specialist does worst on
gain, the two it does best on lose.** Joint training lands every family in a
43–64% band regardless of where its specialist sits. The spread across
families falls from **38.4 points to 20.4 — a 47% compression** — while the
joint policy retains **92% of mean specialist performance at a quarter of
the compute** (220 steps against 4 × 220).

Read: one policy converges toward a common competence level rather than
specialising. That claim rests on four families with a clean sign ordering,
rather than on a contrast between two, and it does not depend on the one
unstable cell (below).

The aggregate efficiency figure survives unchanged in spirit: it was 91% at
a third of the compute over three families, and is **92% at a quarter** over
four.

**What to drop:** "pooling erases learning on the weakest-signal family."
`sans_match` is the *lowest*-specialist family, so compression predicts it
should gain — and it did in joint4 (+5.0). Only joint3 showed the collapse.

---

## 4. Running now — three-family replication at a different seed

`runs/m8/joint3_replicate_dgx.sh`, started 17:02, seed 20260922. Same
families, states, budget and hyperparameters as the original three-family
run; **only the seed differs**, so any difference in outcome is run-to-run
variance by construction.

**What it decides.** In joint3, `sans_match` reached 24.0% — 83% instance
agreement with the *untrained* model, i.e. it barely moved. In joint4 it
reached 43.3%. The two models' passing sets overlap at **Jaccard 0.13** (24
shared, 106 joint4-only, 48 joint3-only) despite near-identical training
data, which is what makes a single run untrustworthy here.

Ruled out already, so the replication is testing seed variance and nothing
else:

- **Not a data problem** — joint3's pool held exactly the same 2,852
  `sans_match` states as joint4's (both logs record the per-family counts).
- **Not a training anomaly** — the two runs' curves track within noise on
  reward, pass fraction, groups-with-signal, KL and validity (joint3 had one
  zero-signal step in 220; joint4 had none).
- **Not a shortcut** — joint4's `sans_match` passes are legitimate: its
  most-reused design covers only **13% of the target range** (bender v1's
  spanned nearly all of it) and reaches 10/300 = 3.3%, well under the gate's
  own 12.7%. It is less diverse than the specialist (40% vs 70% distinct
  designs), which is worth a caveat but is not class-degeneracy.

**Decision rule, so the paragraph can be finished either way:**

- If the replication's `sans_match` lands near **43%** → joint3's cell was
  the outlier, compression holds across both configurations, and §joint can
  state it without qualification.
- If it lands near **24%** → three-family pooling genuinely starves that
  family while four-family pooling does not. Compression still describes
  joint4, but the paragraph must say the per-family effect is
  configuration-dependent and report both runs.

`sans_match` is evaluated **first** so the deciding number arrives before the
other two families (~20:45; the full run ~22:00).

---

## 5. Unchanged from the 09-21 handoff

The four retractions, the four-family headline table, the
design-concentration probe as a methods contribution, and the limitation
that the constant gate bounds a fixed point rather than a class. Nothing in
sections 1–8 of that handoff is superseded except the joint-training
paragraph (§2 there) and the `sans_match` gate figure.

The headline per-family table is regenerable end to end without DGX access:

    python benchmark/harness/m8_table.py --root benchmark/evidence/m8_rl/eval
