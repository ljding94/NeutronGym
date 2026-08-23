# Verification checklist — run per iteration, all of it before any claim

## Before running

1. `validate_instrument` passes (translate + compile + 1 ray).
2. Every component was added after `describe_component`; no guessed params.
3. Beam order sane: source first; each component downstream (+z) of the
   previous; gaps between adjacent components; monitors `restore_neutron=1`.
4. Seed fixed and recorded if this run will be compared or quoted.

## After every run (`get_results`)

5. **Transmission chain:** every diagnostic monitor has intensity > 0, and
   intensity is non-increasing downstream (SPLIT-weighted stages excepted).
   A stage-to-stage drop > 10× that you cannot explain by design (a slit
   cutting the beam, a λ-filter) is a geometry bug — fix before continuing.
6. **Statistics:** `low_statistics` empty for every monitor you will cite;
   events ≥ 1000; rel_err ≤ 5% for iteration decisions, ≤ 1% for final
   numbers.
7. **Band check:** L/E-monitor center and width match the intended λ/E
   band; satellites (λ/2, frame leakage) visible? Explain or filter them.
8. **Geometry check:** beam_center (X0, Y0) ≈ 0 unless deflected by design;
   beam_width consistent with the local aperture/guide cross-section;
   nothing NaN or negative.
9. **Physics bounds:** flux at sample below source phase-space allowance
   (brilliance transfer ≤ 1); resolution consistent with the quadrature
   estimate from `resolution_calcs.py` (within ~2× — if far off, find why).

## Before reporting / accepting a design

10. Production run at `ncount ≥ 1e8` (or every quoted monitor rel_err < 1%),
    fresh seed, results consistent with the iteration runs within errors.
11. Quote: I ± err [n/s], per-cm² where "flux", events, ncount, seed, and
    every assumption (source brightness model, coating budget, ...).
12. For claimed improvements: same-seed same-ncount comparison AND
    fresh-seed re-verification both favor the new design by > 3× the
    combined error.
13. Instrument file exported / instrument_id given, so the result is
    reproducible by others.

## Failure triage order

geometry (positions/rotations) → apertures & focusing → wavelength band →
statistics → component physics params. Diagnostic monitors localize the
stage; `get_monitor_data` PNGs show *how* it's wrong (clipped? off-center?
wrong band?), not just that it is.
