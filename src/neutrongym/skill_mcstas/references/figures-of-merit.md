# Figures of merit — define before building, verify before claiming

## The non-negotiables when quoting any FOM

- I ± err (from `get_results`), events ≥ 1000, ncount, seed — all four.
- Same seed + same ncount for every comparison; re-verify accepted
  improvements at high ncount with a *fresh* seed (an optimum found with one
  seed can be noise).
- Resolution and intensity trade off; quote both, never one.

## Flux at sample

Integrated intensity [n/s] on a sample-sized monitor at the sample position,
divided by its area → n/s/cm². The monitor must match the sample size —
flux averaged over an oversized monitor understates what the sample sees.
Scales linearly with the assumed source brightness: state it.

## Brilliance transfer

Phase-space density delivered / phase-space density at the source, for a
stated (area × divergence × λ-band) acceptance. ≤ 1 by Liouville — measure
with matched monitors (same area/divergence window) at source exit and
delivery point. The honest guide-quality metric, immune to focusing tricks.

## Divergence

Monitor `beam_width` (dX, dY) is an RMS-like width; FWHM ≈ 2.355·dX for a
Gaussian beam — but guide beams are often *not* Gaussian (top-hat-ish or
double-peaked at the critical angle); check a Divergence_monitor when the
shape matters. Guide exit divergence ≲ 2·m·0.099°·λ[Å] (full width) —
anything larger leaks from somewhere.

## Wavelength / energy resolution

- Δλ/λ from L_monitor: FWHM / center. Velocity selector: Δλ/λ ~ 10%
  constant; chopper pair: Δλ/λ = (δt_pulse + δt_chop)·v/L (use
  `resolution_calcs.py`).
- ΔE/E = 2·Δv/v = 2·Δλ/λ (since E ∝ v²) — factor 2 forgotten routinely.
- Independent contributions add **in quadrature**: pulse width, chopper
  opening, flight-path uncertainty, sample thickness. Total² = Σ termᵢ².

## Q-resolution (SANS, diffraction)

Q = (4π/λ)·sin θ, with 2θ the scattering angle. ΔQ/Q combines Δθ (geometry:
source aperture, sample aperture, detector pixel over distances) and Δλ/λ
in quadrature. SANS accessible range: Q_min set by beamstop radius and
collimation; Q_max by detector half-width: Q ≈ 2π·r/(λ·L_det) small-angle.

## Diffraction resolution

Δd/d: CW machines — Caglioti U,V,W from mosaic + collimations vs 2θ; ToF —
Δd/d ≈ sqrt[(Δt/t)² + (cotθ·Δθ)²]; long flight path buys resolution.

## Signal/background

Only meaningful with a defined background: sample-out run, or off-peak
region of the same monitor. Compare integrals over stated windows, same
seed/ncount discipline as everything else.
