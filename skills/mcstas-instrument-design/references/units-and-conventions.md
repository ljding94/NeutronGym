# Units and conventions (McStas 3.x)

## Units — always

| Quantity | Unit | Notes |
|---|---|---|
| length, positions | m | AT vectors, component dimensions (xwidth, yheight, l, …) |
| wavelength λ | Å | source lambda0/dlambda, L_monitor Lmin/Lmax |
| energy E | meV | E_monitor, spectrometer work |
| angle | deg | ROTATED, mosaic often **arcmin** — read the param doc |
| time | s | ToF monitors sometimes µs — read the param doc |
| frequency | Hz | choppers (`nu`) |
| intensity | n/s | integrated monitor `values:`; per-bin = n/s/bin |

Conversions (also in `scripts/resolution_calcs.py`):

- E[meV] = 81.81 / λ[Å]²    (λ = 9.045/√E)
- v[m/s] = 3956 / λ[Å]
- k[Å⁻¹] = 2π/λ
- 1 meV = 0.2418 THz = 8.066 cm⁻¹

Handy anchors: λ=1.8 Å ↔ 25.3 meV (thermal); λ=5 Å ↔ 3.27 meV (cold);
Si(111) backscattering: λ=6.271 Å, E=2.08 meV.

## Coordinate system

- **z = beam direction** (downstream positive), **y = up**, x completes
  right-handed (to the left looking downstream).
- `AT [x, y, z] RELATIVE comp` places the local origin in `comp`'s frame —
  including its rotation. Chain RELATIVE placements; ABSOLUTE only for the
  source.
- `ROTATED [rx, ry, rz] RELATIVE comp`: rotations applied about x, then y,
  then z (deg). A monochromator take-off of 2θ: rotate the crystal by θ
  about y, then place downstream components in a frame ROTATED 2θ.
- Component local origins differ: guides/collimators start at their entrance
  plane and extend +z by `l`; monitors/slits live in their plane; check the
  component doc when in doubt (`describe_component`).
- A component AT the same z as the previous one physically overlaps it —
  leave real gaps (guide exit → monitor a few cm downstream).

## Positioning idioms

- Use an `Arm` at the sample position as the anchor for everything
  downstream; rotate the Arm for scattering-angle geometry.
- Distances in expressions: define instrument parameters (`add_parameter`)
  and write `at=[0, 0, "L1"]` — expressions over parameters are validated.
- ROTATED is not inherited from AT RELATIVE alone; if you need the beam
  frame rotated, rotate explicitly.

## Intensity semantics

- McStas rays carry statistical weight; monitor intensity is **n/s at the
  nominal source power** — independent of ncount. More rays = smaller error,
  same mean.
- `events` (N) is the Monte-Carlo count — statistics quality, not physics.
- Absolute intensities are only as good as the source model's brightness
  parameters (`flux`, spectrum): quote them and say so.

## File-name parameters

String component parameters (reflectivity files, .laz/.lau powder files)
are auto-quoted by the server. Data files ship with McStas (resources
`data/` dir) — reference by bare name (`"Ni.rfl"`, `"Na2Ca3Al2F14.laz"`).
PowderN-class components need NCrystal (installed in this env).
