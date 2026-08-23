# Instrument archetypes — canonical layouts, typical numbers, starting examples

Adapt a shipped example when one matches (`get_example` → `load_instr_file`).
"Start from" names below are shipped examples (`list_examples`).

## SANS (small-angle scattering)

source → λ-filter (velocity selector, Δλ/λ≈10%) → collimation (source
aperture ~3–5 cm, sample aperture ~1 cm, separation 2–20 m, often variable)
→ sample → evacuated flight tube → 2D PSD (~1 m², 1–20 m).
λ = 4.5–12 Å. Q range 10⁻³–0.5 Å⁻¹; L_coll ≈ L_det for optimal ΔQ.
Start from: `templateSANS` (pinhole, sphere sample), `ILL_H15_D11` (real, guide
feed), `ISIS_SANS2d` (ToF-SANS: white beam + time-resolved detector).

## Powder diffractometer (constant wavelength)

source → guide → vertically-focusing monochromator (PG(002) d=3.355 Å or
Ge; take-off 2θ_M 90–120° for resolution) → collimator (α ~ 10–30′) →
sample (cylinder ~1 cm) → collimator → detector bank 2θ = 5–160°.
λ 1.2–2.5 Å. Resolution: Caglioti; best Δd/d near focusing condition.
Start from: `PSI_DMC` (validated vs measurement), `ILL_D2B` (high-res).

## ToF powder / engineering diffractometer (pulsed source)

moderator → long guide (10–100 m) → frame-definition choppers → sample →
detector banks (backscattering bank = best Δd/d ≈ Δt/t).
Start from: `ISIS_IMAT` (imaging+diffraction hybrid).

## Reflectometer

source → coarse collimation → two slits defining θ and Δθ (~1–3% Δθ/θ) →
sample at grazing θ 0.3–5° → detector. Liquids need the beam deflected
downward onto a horizontal surface (supermirror deflector or inclined
guide). Monochromatic (rotate sample) or ToF (fixed geometry, λ-band).
Start from: `ISIS_CRISP` (ToF), `templateTOF` for the ToF pattern.

## Direct-geometry ToF spectrometer (chopper spectrometer)

pulsed source (or pulsing chopper) → guide → pulse-shaping chopper →
frame-overlap choppers → monochromating (Fermi/disk) chopper → sample
(L1 chopper–sample 1.5–3 m) → large detector array (L2 2.5–4 m).
Ei fixed by chopper phase: t = L/v(λ) — compute with resolution_calcs.py,
never scan. ΔE/E ≈ 1–5%; repetition-rate multiplication on long-pulse
sources. SPLIT 10–100 before the sample.
Start from: `ILL_IN5`, `ISIS_LET`, `SNS_ARCS`, `templateTOF`.

## Indirect-geometry / backscattering spectrometer

white beam → sample → analyzer crystals select fixed Ef → detectors.
Si(111) near-backscattering: Ef = 2.08 meV, µeV-class ΔE. Deep inelastic /
vibrational: PG analyzers + Be filter (TOSCA-like).
Start from: `SNS_BASIS` (complex; reader-fragile — build, don't import),
`ISIS_OSIRIS`.

## Triple-axis (TAS)

monochromator (θ_M) → sample (θ_S, 2θ_S) → analyzer (θ_A) → single detector.
Fixed kf typical (kf = 1.55, 2.662 Å⁻¹); collimators (α 20–60′) between all
stages; PG filter against λ/2. Point-by-point (Q, ω) scans.
Start from: `templateTAS`, `PSI/RITA-II` (multi-analyzer).

## Imaging / radiography

source → pinhole (D) → open flight (L) → sample plane → scintillator/PSD.
Resolution ≈ sample-detector gap × D/L; L/D 100–1000. No optics after the
pinhole. White or λ-selected (Bragg-edge imaging).
Start from: `ISIS_IMAT`, `RTP_NeutronRadiography`.

## Rules of thumb across archetypes

- Guide cross-sections 2–6 cm wide, m = 1.5–3 (m ≥ 4 only where angles
  demand it — cost and reflectivity fall-off).
- Curved guide loses line of sight after L ≈ √(8·w·R); R chosen so
  λ* = characteristic wavelength passes: λ* ≈ 6600·√(w/R)/m [Å, m units].
- Every λ-defining device leaks harmonics (λ/2 from monochromators, frame
  leakage in choppers) — add a filter or check the L_monitor for satellites.
- Sample-position beam sizes: 1×1 to 4×4 cm²; detectors: match the
  archetype's angular coverage, not "as big as possible".
