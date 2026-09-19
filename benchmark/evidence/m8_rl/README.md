# M8 RL evidence (frozen copy, 2026-09-19)

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

Regenerate the headline table from these records with:
  python benchmark/harness/m8_table.py
