# Component guide — what to reach for, and the traps

Always `describe_component` before first use. Categories in
`list_components`: sources, optics, samples, monitors, misc, contrib, union.

## Sources

- `Source_simple` — flat spectrum, quick studies. `Source_Maxwell_3` /
  `Source_gen` — realistic moderator spectra (thermal/cold Maxwellians).
  `ESS_butterfly` — ESS long-pulse.
- **Trap:** `focus_xw/focus_yh/dist` is variance reduction — point it at
  the first real aperture exactly. Oversized wastes statistics; undersized
  fakes flux (Liouville violation downstream).
- **Trap:** flux/brightness params set the absolute scale — record what you
  assumed.

## Guides

- `Guide` — straight, simple, no gravity. `Guide_gravity` — use for
  length > 20 m or λ > 5 Å (and set `gravity=true` on the run).
  `Elliptic_guide_gravity` — ballistic/elliptic transport, fewer
  reflections. `Bender` / curved: kills line of sight (fast-neutron and
  gamma background).
- m-value: θc = m·0.099°·λ[Å]. m=1 Ni; m=2–3 workhorse; m=6+ only for
  extraction/high-angle sections. Reflectivity drops with m and angle —
  more m is not free flux.
- **Trap:** guide entrance must be ≥ the beam at that point, and the source
  focus must target the entrance, not the exit.

## Choppers

- `DiskChopper`: `nu` [Hz], `phase` [deg], slit angle `theta_0` [deg].
  Opening time δt = θ₀/(360·ν). Phase for wavelength λ at distance L:
  φ = 360·ν·L/v(λ) — use `resolution_calcs.py chopper`.
- `FermiChopper` for monochromating at higher resolution.
- **Trap:** frame overlap — slowest wanted neutron must beat the next
  pulse: λ_max = 3956/(L·ν_rep). Check it, add overlap choppers if not.
- **Trap:** `phase` vs `delay`: some components take seconds, some degrees.

## Monochromators / analyzers

- `Monochromator_flat` / `_curved` (vertical focusing buys flux).
  Bragg: λ = 2d·sin θ. d: PG(002) 3.355 Å, Ge(111) 3.266, Si(111) 3.135,
  Cu(220) 1.278. Mosaic in **arcmin** (typ. 25–50′).
- **Trap:** place downstream components in a frame ROTATED by 2θ_M; the
  crystal itself sits at θ_M. λ/2 harmonic rides along — filter (PG filter,
  Be) or accept and note it.

## Slits, collimators, filters

- `Slit` (aperture), `Collimator_linear` (divergence in **arcmin**),
  `Beamstop`. `Filter_gen` for transmission curves.
- **Trap:** a Slit with both radius and xwidth set errors; pick one shape.

## Samples

- `Sans_spheres` / sasmodels kernels (SANS), `PowderN` (needs .laz/.lau,
  NCrystal), `Incoherent`/`V_sample` (vanadium: isotropic scatterer for
  normalization/resolution), `Single_crystal`, `Phonon_simple`.
- Put `SPLIT 10–100` on the sample component of any scattering instrument —
  each incoming ray is re-scattered SPLIT times (variance reduction).
- **Trap:** samples scatter into 4π — set `focus`-type params of the sample
  (target detector/analyzer solid angle) or most rays are wasted.

## Monitors

- `PSD_monitor` (2D image), `L_monitor` (λ-spectrum), `E_monitor`,
  `TOF_monitor`, `Divergence_monitor`, `DivPos_monitor` (phase space).
  `Monitor_nD` is the swiss-army knife (options string) — prefer the simple
  ones unless you need its features.
- Diagnostic monitors mid-beam: `restore_neutron=1`, sized to the local
  beam, bins ~50–100 (more bins = fewer events/bin — statistics!).
- **Trap:** filename is per-monitor and must be unique, else outputs
  collide.

## Cross-cutting traps

- Component at identical z as its neighbor = overlapping volumes →
  undefined behavior. Leave gaps (≥ mm).
- Unrotated detector after a rotated Arm sees the wrong beam.
- Intensities dropping to zero after adding one component → its aperture,
  position, or rotation; binary-search with diagnostic monitors.
- 1e5 rays through a chopper cascade may leave < 1000 events at the
  detector — raise ncount or add SPLIT before concluding anything.
