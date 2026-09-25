# Two revision experiments — results (2026-09-24)

Both requested before the deadline; both done, on the DGX, no GPU.
**One of them changes a headline claim.** Read §1 before touching Table 2.

---

## 1. Physics-inversion start + local search at the agent's budget

**The arm:** seed coordinate descent from the strongest closed-form optics
inversion instead of from the baseline, then spend the agent's own
**10-simulation budget** refining. The seed is the exact rule the readout
probe ranked best for `guide_match` — `kdiv = 0.577, w_in = 0.07`, which
scores 83/300 (27.7%) as a one-shot answer. Same slice (held-out 300–599),
same budget, same environment scoring as every other row.

Sanity checks, so a strong result cannot be an artefact: the formula produced
an in-range solution on **300/300** instances (no silent fallback to the
baseline), and **every** instance used its full 10 simulations.

### Result

| arm | pass rate |
|---|---|
| classical search from the baseline, 10 sims | 13.3% |
| hand-coded physics rule, one shot | 27.7% |
| **physics-seeded search, 10 sims** | **243/300 = 81.0%** |
| GRPO-trained 8B, 10 turns | 230/300 = 76.7% |

**Paired on the same 300 instances: 59 physics-only vs 46 GRPO-only, exact
McNemar p = 0.24.** The 4.3-point gap is **not significant** — the two are
statistically tied.

### Control: does the *physics* matter, or just "not the baseline"? (added 2026-09-24)

Every local-search arm started from the same baseline design, and that
baseline is deliberately undersized (the T2 calibration placed it 3–4x below
the FOM plateau). So the 13.3% → 81.0% jump was ambiguous between "the
closed form carries information" and "anything beats that start". The
control runs the identical optimizer and budget from a **random valid
design**:

| arm, guide match, 10 sims | pass rate |
|---|---|
| coordinate descent, **baseline** start | 40/300 (13.3%) |
| coordinate descent, **random** start | **43/300 (14.3%)** |
| coordinate descent, **physics** start | 243/300 (81.0%) |
| GRPO-trained 8B | 230/300 (76.7%) |

Paired: random vs baseline start **34 vs 31, p = 0.80** (indistinguishable —
the baseline is not an unfairly weak start); physics vs random start
**204 vs 4, p = 3.8e-55**; random start vs GRPO **8 vs 195, p = 1.0e-47**.

**This settles it in the paper's favour.** The 13.3% row is a fair
characterisation of unseeded local search at ten simulations, and the entire
81.0% comes from the closed-form inversion rather than from the choice of
start. So the matched-budget claim survives with one qualifier: RL beats
classical search that is *not handed the physics*, decisively (76.7% vs
14.3%), and is statistically indistinguishable from search that *is*
(81.0%, p = 0.24).

Evidence: `runs/m8/classical_guide_match_randomstart_budget10_from300_n300.json`.

**A bug worth recording.** The control's first run reported 243/300 —
byte-identical to the physics arm, 0 discordant, p = 1. That was a defect,
not a result: the edit adding the control replaced both occurrences of the
physics guard, so the control silently ran the physics seed. Two different
starts agreeing on all 300 instances is a bug signature. Fixed, with
regressions in `tests/test_classical_baseline.py`.

### What this does to the paper

The pre-stated criterion was that landing near 76.7% leaves the paper intact
because the text already frames the RL gain as search efficiency rather than
design skill. It landed at 81.0%, tied. So the paper survives — **but the
matched-budget claim must be rewritten, because it is currently false as
stated.**

`\quad Classical search, agent's simulation budget & 13.3\%` is true only of
a search started from the baseline. Handed the closed form, the same
optimizer at the same budget **matches the trained model**. What cannot be
said any more:

- ❌ "RL beats hand-coded physics *and* classical search at matched budget."
- ❌ Any wording implying the 13.3% row bounds what classical methods can do
  at 10 simulations.

What is still true, and is a better sentence anyway (**the control above
confirms the first clause is a real contrast, not an artefact of the
starting point**):

> At the same simulation budget the trained 8B beats classical search
> decisively when that search is not handed the physics (76.7% vs 14.3%
> from a random start, 13.3% from the baseline), and is statistically
> indistinguishable from a search seeded with the closed-form inversion
> (81.0%, p = 0.24). What RL recovers from reward alone is what the
> physicist supplies by hand.

That keeps the contribution (learning from execution feedback, no formula
supplied) and drops a claim a reviewer would have broken. It also strengthens
§related engagement with OptiAgent-style optimizer baselines.

### Suggested Table 2 row

Insert in *"Something simpler did it"*, after the hand-coded physics rule:

```latex
\quad Physics inversion + search, agent's budget & guide match & 81.0\% & 76.7\% RL & $+4$ \\
```

with a footnote that the difference is not significant (59 vs 46 discordant,
$p = 0.24$), so the row reads as a tie rather than a defeat.

Evidence: `runs/m8/classical_guide_match_physics_budget10_from300_n300.json`.

---

## 2. Full fresh-seed re-grade of all 230 passes

**The arm:** re-simulate every passing design at **3 fresh Monte-Carlo
seeds** (90001 / 91001 / 92001, all distinct from the protocol seed), using
the family's own per-observable tolerances. Both sides are re-run: the target
is the hidden design's *measured* observables, not a constant, so re-grading
against the old target would charge the agent for noise in the target. All
690 checks were usable (no starved monitors).

This supersedes the sampled figure — 72 of 82, from 60 instances of the
**selection** slice at 2 seeds. It now covers all 230 passes of the
**reported** slice.

### Result

| measure | value |
|---|---|
| holds per check | 605/690 = **87.7%** |
| **holds at all three seeds** | 184/230 = **80.0%** |
| fails at exactly one seed | 19/230 = 8.3% |
| fails at two seeds | 15/230 = 6.5% |
| fails at all three | 12/230 = **5.2%** |

Worst miss across seeds, as a fraction of tolerance (1.0 = exactly at the
bar): median 0.79, p75 0.98, p90 1.11. **33 designs miss by ≤20% of the
tolerance** — the failures are concentrated right at the boundary, not
scattered.

### What this does to the paper

The current sentence — *"re-simulating 82 of a checkpoint's passing designs
at fresh seeds kept 72, so about 12% of passes fail at another seed"* — turns
out to be **right per check** (12.3% of checks fail) but **optimistic as a
per-design claim**: on the strict reading, 20% of passes fail at at least one
of three fresh seeds, and only 5.2% fail at all three.

Suggested replacement:

> Re-simulating all 230 passing designs at three fresh seeds, 80.0% match at
> every seed and 87.7% of individual re-runs hold; 5.2% fail at all three.
> The failures cluster at the boundary — median worst miss 0.79 of the
> tolerance, and 33 designs miss by no more than a fifth of it — so this is
> tolerance-edge fragility rather than wrong designs.

That is stronger than the current text in one way and weaker in another: it
is a measured number over every pass instead of a sample of 82, and it is
honest that the strict figure is 20%, not 12%. Reporting both readings is
better than picking one, and the 5.2%-fail-everywhere figure is the one that
bounds "wrong design" as opposed to "noisy boundary".

Evidence: `runs/m8/fresh_seed_regrade_guide_match.json`.

---

## Reproduce

    python benchmark/harness/classical_baseline.py --family guide_match \
      --split heldout --start 300 --n 300 --budget 10 \
      --methods physics-coordinate --workers 48

    python benchmark/harness/fresh_seed_regrade.py \
      --eval runs/m8/eval_match_fresh_trained-8b.json --arm trained-8b \
      --family guide_match --split heldout --seeds 3 --workers 30
