---
name: mcstas-instrument-design
description: Neutron instrument design/simulation with the mcstas MCP tools. Use whenever building, modifying, optimizing, or evaluating a McStas instrument — covers units, workflow, statistics discipline, figures of merit, instrument archetypes, and verification.
---

# McStas instrument design

The mcstas MCP server validates syntax; this skill is the physics judgment.
References: `references/units-and-conventions.md`, `figures-of-merit.md`,
`instrument-archetypes.md`, `component-guide.md`, `verification-checklist.md`.
Calculator: `scripts/resolution_calcs.py` (chopper phasing, Bragg angles,
unit conversions — run it, don't do these by hand).

## Core workflow

1. **Pin the goal as numbers first.** Before any tool call, restate the task
   as a quantitative figure of merit + constraints (e.g. "maximize flux on a
   1×1 cm sample at λ = 5 ± 0.5 Å within 30 m"). If the request is
   qualitative, propose the FOM and say so. Disclose every assumption you
   must invent (source brightness, coating budget) — absolute intensities
   scale with them.
2. **Start from an archetype.** Match the task to an archetype
   (`instrument-archetypes.md`), and check `list_examples` /`get_example`
   for a shipped instrument of that class — adapting a working example beats
   building blind. `load_instr_file` imports it.
3. **`describe_component` before first use of any component type.** Never
   guess parameter names, units, or defaults.
4. **Build in beam order** — source → optics → sample → monitors — adding a
   diagnostic monitor after each stage (wavelength or PSD, sized to the
   expected beam, `restore_neutron=1`). Remove or keep them; they are how
   you debug geometry.
5. **`validate_instrument` before spending rays.** Then iterate at
   `ncount=1e5–1e6` (seconds); production-validate the final answer at
   `ncount≥1e8` or until every quoted monitor has rel_err < 1%.
6. **Fix the seed.** Any comparison between two designs or parameter values
   uses the same `seed` and the same `ncount`; any quoted result records
   both. Without this, nothing is reproducible or comparable.
7. **Check the transmission chain every iteration** (see
   `verification-checklist.md`): every stage monitor must show signal;
   an unexplained >10× intensity drop between stages is a geometry bug
   (positions, apertures, focusing), not physics.
8. **Optimize with the right tool.** Continuous knobs → `scan_parameter` /
   server-side optimization when available; agent reasoning is for topology
   (add a bender? curved vs straight? chopper count?). Accept an improvement
   only if it survives re-verification at high ncount with a fresh seed.

## Hard rules

- **Statistics floor:** never compare designs or quote a number from a
  monitor with < 1000 events (`low_statistics` in `get_results` flags
  these). Rel_err > 5% → run more rays before deciding anything.
- **Report I ± err, events, ncount, seed** for every quoted intensity, and
  normalize per cm² when calling it "flux".
- **Units:** lengths m, wavelength Å, energy meV, angles deg, time s;
  z is the beam axis, y is up. E[meV] = 81.81/λ², v[m/s] = 3956/λ[Å].
- **Intermediate monitors get `restore_neutron=1`** so they cannot perturb
  downstream physics.
- **Monitor sizing:** a monitor smaller than the beam under-reports; check
  `beam_width` (dX/dY) against the monitor's xwidth/yheight and against
  aperture sizes.
- **Source focusing is variance reduction, not physics:** `focus_xw/yh`
  must match the first real aperture — larger wastes rays, smaller is a
  simulation lie that inflates apparent flux.
- **Gravity matters** for cold neutrons and long instruments: λ > 5 Å or
  length > 20 m → `gravity=true` (and prefer Guide_gravity).
- **Brilliance transfer ≤ 1 always** (Liouville). A "gain" above the source
  phase-space density is a bug, usually focusing-parameter fraud (see rule
  above).
- **Long runs:** submit with `wait_s=0` and poll `job_status`; don't sit in
  a blocking call. Cancel jobs you no longer need.
- **Chopper phasing is arithmetic, not tuning:** compute phase from flight
  path and wavelength with `resolution_calcs.py chopper`; never hand-scan a
  chopper phase to find the beam.

## When results look wrong

Zero intensity everywhere → source focusing/aperture chain (step 7).
Intensity but wrong λ-band → check source lambda0/dlambda vs monitor Lmin/Lmax.
Beam off-center (X0, Y0 ≉ 0) → component mis-positioned or rotation error.
NaN/negative → report as a bug, don't paper over.
Numbers plausible but unverifiable → you skipped seed/ncount recording (rule 6).
