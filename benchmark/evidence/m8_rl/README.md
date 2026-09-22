# M8 RL evidence (frozen copy, updated 2026-09-21)

Auditable copy of every file behind the M8 numbers. The narrative summary is
`note/m8-results-summary-2026-09-18.md`; the pre-registration is
`note/prereg-matched-compute-frontier-ablation-2026-09-18.md`.

- `eval/` — per-arm evaluation records (rows carry instance, best level,
  best reward, steps, passing action). Fresh-slice files are `*_fresh_*`
  (held-out 300–599, used for the headline); `*_arm_*` files are the
  selection slice (0–299).
- `gates/` — no-model probes: `match_gate_*` (fixed designs, baseline, and
  every other instance's solution as a lookup), `readout_probe_*` (copy-type
  rules gated, physics-model rules reported), `classical_*` (matched-compute
  search), `m8_table.json` (generated table with exact intervals).
- `training/` — GRPO step logs (reward, pass fraction, groups with signal,
  KL) and the pipeline logs for every run.
- `scripts/` — the exact pipelines and verification scripts that produced
  these files.
- `bender_v1_retracted/` — the **retracted** first version of the bender
  family, kept so the limitation in the paper is auditable. Its numbers must
  not be quoted as results; see below.

## Added 2026-09-21

**Fourth family, `bender` v2** — `eval/eval_bender_fresh_*.json` (held-out
300–599: untrained 8B 69/300, untrained 32B 131/300, GRPO 227/300),
`gates/match_gate_bender_n150.json` (best fixed design 8.0%, upper95 12.6%),
`gates/readout_probe_bender_0.85_n150.json` (no copy-type rule; analytic
cutoff inversion 12.7% as a reference arm),
`training/grpo_bendergrpo_{a,b}.log` (120 + 100 steps).

**Joint multi-family training** — `eval/eval_joint_*.json`, one policy over
the pooled states of guide_match, sans_match and tof_chopper at the same
220-step budget one specialist gets; `training/grpo_jointgrpo_{a,b}.log`.
This run **predates bender** and pools three families, not four.

**Design-concentration probe** — `gates/concentration_*.json` and
`scripts/design_concentration.py`. Distinct passing designs per pass, top-5
share, maximum reuse. The constant gate bounds a fixed *point*; this bounds
a *class*. Read each family against its own gate, not against another
family: bender's two observables are both set by one quantity (the cutoff
`lam_c`), so low concentration is intrinsic there, and its most-reused design
covers 6.0% of instances against the gate's own 8.0%.

**Why `bender_v1_retracted/` exists.** v1 passed the constant gate at 10.7%
while **24.7% of its held-out instances had a hidden cutoff below the
incident band** — the bender did nothing and the targets were just the source
spectrum's own mean and spread, so any transparent design solved them. Hidden
designs had been drawn uniformly from the parameter box; v2 adds a
`hidden_filter` requiring the hidden design to cut 25–85% of the band. The
failure was caught by design concentration (42 distinct designs for 141
passes), not by the gate, which is the point of the limitation stated in the
paper. **No v1 number is a result.**

Regenerate the headline table for **all four families** from these records,
without access to the machine that produced them:

  python benchmark/harness/m8_table.py --root benchmark/evidence/m8_rl/eval

Every rate, exact Clopper-Pearson interval and paired McNemar p in the
paper's main table comes out of that command. Each trained arm is paired
against both untrained arms, and the slice is recorded per family --
sans_match reports on 600-899 because 300-599 chose its checkpoint.
