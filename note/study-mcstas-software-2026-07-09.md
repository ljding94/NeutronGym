# Study: McStas 3.7.12 software (installed toolchain)

**Date:** 2026-07-09 · Systematic study of the conda-installed McStas toolchain, conducted to ground the M1 server design (see `m1-server-design-2026-07-09.md`). All facts verified by running the tools on this machine.

## Headline findings that changed the plan

1. **`mcrun` has a built-in scipy optimizer** (`--optimize`, 14 methods incl. powell/nelder-mead, `--optimize-eval` expressions over detector stats like `d.intensity/d.dX`, `--optimize-monitor`). M4's `optimize` tool should wrap this instead of reimplementing a scipy loop.
2. **Every shipped example carries a `%Example:` self-test line with an expected detector value** (e.g. templateSANS: `%Example: lambda=6 Detector: detector_I=0.0445623`), validated by the `mctest` harness. This is free, curated ground truth for benchmark T1 grading and for server integration tests.
3. **The `values: I I_err N` line in mccode.sim** gives integrated intensity/error/events per monitor without touching data arrays — plus `statistics: X0/dX[/Y0/dY]` (beam center/width) and `signal: Min/Max/Mean`. `get_results` summary statistics are nearly free.
4. **MPI works** (Homebrew Open MPI 5.0.7 on PATH in-env, verified 2-node run) but toggling MPI on/off against a cached binary fails with exit 255 — always pass `-c` when the MPI setting changes.
5. **Name collision warning:** the env's `mcp` binary is the (broken) Python Model Context Protocol CLI, not a McStas tool.

---

## 1. CLI tool inventory

### Binaries in `$CONDA_PREFIX/bin` (mc*)

| Tool | Purpose |
|---|---|
| `mcrun` | **Primary driver**: compiles .instr → C → binary, runs simulations, parameter scans, and optimizations. The tool an MCP wrapper should shell out to. |
| `mcstas` | The core code generator (cogen): translates `.instr` → C (invoked by mcrun). |
| `mcstas-pygen` | Alternative compiler emitting Python instead of C. |
| `mcplot`, `mcplot-pyqtgraph/-matplotlib/-html/-matlab` | Plot dataset dirs (mccode.sim/.dat) with different backends. |
| `mcdisplay`, `mcdisplay-webgl/-webgl-classic/-pyqtgraph/-matplotlib/-matlab/-cad/-mantid` | Instrument geometry / ray-trace visualization backends (`-cad` exports CAD; `-mantid` produces Mantid IDF). |
| `mcdoc` | Doc generator/browser: builds HTML/`--md`/`--tex` docpages for every component and instrument, with searchterm filter — a ready-made introspection backend. |
| `mcgui` | PyQt GUI front-end (irrelevant for a wrapper). |
| `mcresplot` | Plots TAS resolution functions. |
| `mcsplit` | Splits an mcstas-generated C file (build tooling). |
| `mctest` / `mcviewtest` | Run the example self-test suite / view results (uses `%Example:` lines). |
| `mcstas_errmsg`, `mcstas-acc_gpu_bind`, `mcstas-jupylab`, `mcstas-postinst` | Misc helpers. |
| `mcp` | **Not McStas** — broken Python MCP-package CLI in this env. Do not confuse. |

### `mcrun` flags relevant to a wrapper

- **Core run control**: `-n/--ncount`, `-d/--dir DIR` (unspecified → `INSTRUMENT_TIMESTAMP`; also `--dirprefix/--dirsuffix`, `-a/--append`), `-s/--seed` (**must be != 0**), `-g/--gravitation`, `-t/--trace`/`--no-trace`, `-y/--yes`, `--no-output-files`, `--bufsiz`.
- **Compilation**: `-c/--force-compile`, `-I dir`, `--D1/--D2/--D3`, `--embed` (copy .instr into output dir), `--no-cflags`, `--showcfg=ITEM` (`bindir|libdir|resourcedir|tooldir` — lets a wrapper self-locate resources).
- **Parameters**: positional `param=value`, ranges `param=min,max`, `-p FILE`, `--list-parameters`, `-i/--info`.
- **Scans**: `-N NP/--numpoints`, `-L/--list` (fixed points), `-M/--multi` (multi-dim), `--scan_split=n`, `--seeds=SEEDS`, `--optimise-file=FILE` (rename scan result, default `mccode.dat`).
- **Built-in optimizer** (scipy): `--optimize` (maximize monitors over params given as `param=min,guess,max`), `--optimize-maxiter` (default 1000), `--optimize-tol`, `--optimize-method` ∈ {powell (default), nelder-mead, cg, bfgs, newton-cg, l-bfgs-b, tnc, cobyla, slsqp, trust-constr, dogleg, trust-ncg, trust-exact, trust-krylov}, `--optimize-eval` (expression over detector struct: `d.intensity`, `d.error`, `d.values`, `d.X0/d.Y0`, `d.dX/d.dY`), `--optimize-minimize`, `--optimize-monitor=NAME`.
- **Parallel**: `--mpi=NB_CPU`, `--machines`, `--openacc`, `--funnel`, GPU tuning flags.
- **Format**: `--format=McCode|NeXus` — but this build is text-only (NeXus needs an HDF5-enabled build). `--IDF` for Mantid.
- **Metadata**: `--meta-list/--meta-defined/--meta-type/--meta-data` on a compiled instrument.

### MPI status: usable, with one gotcha

- `mpirun` = `/opt/homebrew/bin/mpirun` (Open MPI 5.0.7, Homebrew), visible in-env; `mpicc` present. Verified 2-node run: compiled with mpicc, output merged correctly (`Nodes: 2` in mccode.sim, single .dat set).
- **Gotcha**: `--mpi=2` with a cached non-MPI `.out` binary fails (exit 255, stale binary reused). Pass `-c` whenever toggling MPI. Harmless linker warnings on macOS (missing fftw dir, dylib version mismatch).

---

## 2. Component library census

### Organization under `$CONDA_PREFIX/share/mcstas/resources`

| Dir | .comp files | Content |
|---|---|---|
| `sources/` | 13 | Source_simple, Source_Maxwell_3, ESS_butterfly, … |
| `optics/` | 45 | guides, monochromators, choppers, slits, benders, polarizers |
| `samples/` | 14 | Vanadium, PowderN, SANS spheres, single crystal, … |
| `monitors/` | 43 | PSD, wavelength/energy/ToF monitors, Monitor_nD |
| `misc/` | 14 | Progress_bar, MCPL I/O, Vitess compat |
| `contrib/` | 96 | user-contributed |
| `union/` | 40 | Union geometry/physics subsystem |
| `sasmodels/` | 94 | auto-generated SasView SAS kernels |
| `obsolete/` | 15 | deprecated |
| **Total** | **374** | |

Non-component dirs: `data/` (cross sections, reflectivities, .lau/.laz tables referenced by string params), `examples/`, `share/` (C runtime), `NOMENCLATURE.md` (canonical parameter naming: `xwidth`, `yheight`, `zdepth`, `radius`, `thickness`, …).

### Component file anatomy (verified on Source_simple, PSD_monitor, Guide_gravity)

- **Doc sections**: `%I` (Written by/Date/Origin), `%D` (description + an `Example:` instantiation line), optional `%VALIDATION`, `%P` (parameter docs), `%L` links, `%E` end.
- **`%P` line pattern**: `* name: [unit] Description` — unit always in brackets; `[1]` = dimensionless; `[str]`/`[string]` for strings.
- **Declaration grammar**: `DEFINE COMPONENT <Name>` + `SETTING PARAMETERS (int nx=90, string filename=0, xmin=-0.05, …)` (untyped = double). **Required params have no default** (Guide_gravity: `w1, h1, l`). `DEFINITION PARAMETERS` is nearly extinct in 3.x — only 3 of 374 components still use it. So `SETTING PARAMETERS` is the parameter source of truth; `%P` supplies units/docs.
- Monitors emit data via `DETECTOR_OUT_1D/2D(...)` — the origin of all .dat metadata.

---

## 3. Example corpus census + in-file paper citations

**297 `.instr` files** under `resources/examples/`, one dir per example (`<name>.instr` + README, sometimes extra .comp/data files).

- **Facility dirs**: ILL **41**, ISIS 14, ESS 11, Mantid 11, SNS 7, Risoe 7, PSI 5, TRIGA 4, BNL 3, DTU 3, FZ_Juelich 3, NCrystal 3, SINE2020 3, TUDelft 3, HZB 2, Necsa 2, HighNESS 1, LLB 1
- **Pedagogical**: Templates 20, elearning 5, Tools 7
- **Test suites**: Tests_union 33, Tests_samples 24, Tests_optics 22, Tests_polarization 20, Tests_grammar 8, others ~20
- **Union demos/validation**: 14

Header format: `%Identification` (Written by / Date / Origin / `%INSTRUMENT_SITE:`), `%Description`, `%Parameters` (with units), `%Link`, and a **`%Example:` self-test line with expected detector value** (validated by `mctest`).

**~32 of 297** files carry a paper-like citation in the header. Best real-facility pairs (citations verbatim from files; DOI verification in `study-instrument-papers-2026-07-09.md`):

| Path (under examples/) | Facility | Class | Cited reference (verbatim, abridged) |
|---|---|---|---|
| ILL/ILL_D2B | ILL | High-res powder diffr. | Caglioti et al., NIM 3 (1958) 223; Cussen, NIM A 554 (2005) 406 |
| ILL/ILL_IN4 | ILL | Thermal ToF spectrometer | H. Mutka, NIM A 338 (1994) 144 |
| ILL/ILL_IN6 (+H15_IN6) | ILL | Cold ToF spectrometer | Scherm et al., ILL Report 76S235 (1976); Blanc, ILL Report 83BL21G (1983) |
| PSI/PSI_DMC (+simple, source) | PSI/SINQ | Powder diffractometer | Willendrup et al., Physica B 386 (2006) 1032 (validation vs real DMC data) |
| PSI/RITA-II | PSI/SINQ | TAS (multi-analyzer) | Udby et al., NIM 634 (2011) s138 (verified vs measured data) |
| ISIS/ISIS_IMAT | ISIS | Imaging+diffraction | Burca et al., JINST 8 (2013) P10001 |
| SNS/SNS_BASIS | SNS | Backscattering | Mamontov & Herwig; DOIs 10.1051/epjconf/20158303015, 10.1063/1.4961569 |
| TUDelft/SESANS_Delft | TU Delft | SESANS | J. Appl. Cryst. 54 (2021) 10.1107/S1600576720015496; RSI 76 (2005) |
| TUDelft/SEMSANS_Delft | TU Delft | SEMSANS | same JAC 2021 + Physica B 406 (2011) |
| HighNESS/WOFSANS | ESS | SANS w/ Wolter optics | Santoro et al., Nucl. Sci. Eng. 2023, 10.1080/00295639.2023.2204184 |
| TRIGA/RTP_{SANS,DIF,Laue,NeutronRadiography} | TRIGA Malaysia | 4 classes | Sufi et al., J. Appl. Cryst. 30 (1997) 884 |
| Tests_polarization/He3_spin_filter | (NIST physics) | He-3 spin filter | Batz et al., J. Res. NIST 110 (2005), 10.6028/jres.110.042 |
| Tests_samples/GISANS_tests | (ILL D22) | GISANS | Hellsing et al., APL 100 (2012) 221601 |

Real-instrument models **without** in-header citations (still benchmark-worthy): SNS_ARCS, ISIS_LET, ISIS_MERLIN, ISIS_OSIRIS, ISIS_SANS2d, ISIS_CRISP, ISIS_TOSCA_preupgrade, ESS_Testbeamline_HZB_V20, HZB_FLEX, HZB_NEAT, and the ILL guide-hall suite (ILL_H512_D22, ILL_H53_IN14, ILL_H10_IN8, ILL_H13_IN20, ILL_H16_IN5, ILL_H22_VIVALDI, ILL_H143_LADI, ILL_D4, ILL_SALSA, ILL_Lagrange, ILL_BRISP, ILL_IN13).

Caution: ESS_KVASIR has **no** citation (BIFROST-derived NBI model) — easy to mis-attribute.

---

## 4. Output data format (from `runs/m0_templateSANS/`)

### `mccode.sim` (dataset index — parse this first in `get_results`)

Three block types:
1. `begin instrument` — name, source path, `Parameters: lambda(double) …`
2. `begin simulation` — Format, Creator (`3.7.12, git`), `Ncount`, `Trace`, `Gravitation`, **`Seed`**, `Directory`, one `Param: name=value` per parameter; `Nodes: N` when MPI.
3. One `begin data … end data` per monitor output file:
   - `type: array_1d(1000)` / `array_2d(128, 128)`; `component:` instance name; `position: x y z` (absolute, m)
   - `statistics: X0=…; dX=…; [Y0=…; dY=…;]` (1st/2nd moments), `signal: Min/Max/Mean`
   - **`values: I I_err N`** (integrated) — the key line for grading
   - axes: `xvar/yvar/zvar`, `xlabel/ylabel/zlabel`, `xlimits`/`xylimits`, `variables:` column spec

One component can emit multiple data blocks (e.g. PSD_monitor_rad → psd2.dat + psd2_av.dat).

### `.dat` files

- **1D**: `#`-header (full simulation block + data block metadata), then rows in `variables` order: bin-center, I, I_err, N.
- **2D**: same header, then **three matrix blocks** (`# Data`, `# Errors`, `# Events` markers), each ny rows × nx values.
- mcrun stdout also prints per-monitor `Detector: <name>_I=… <name>_ERR=… <name>_N=… "file.dat"` lines — greppable without file I/O.

---

## 5. Scans (`mcrun -N`)

Verified with `mcrun templateSANS.instr -n 10000 -N 3 -d scan_test lambda=5,7`:

- Scanned params use `param=min,max` + `-N <numpoints>` (linear); `-L` fixed list; `-M` multi-dim; `--seeds` seed scans.
- Output dir: numbered subdirs `0/ 1/ 2/` (each a complete single-run dataset) + top-level `mccode.dat` + scan-level `mccode.sim`.
- `mccode.dat`: `#` header (`Numpoints`, `xvars`, `yvars`, `variables`) then one row per point: param value(s), then `<component>_I <component>_ERR` **per output file**. Component names repeat if one component writes several files — **key columns by position within `yvars`, not by name**. Integrated N is not in mccode.dat (get it from point subdirs).
- The optimizer reuses this machinery (`param=min,guess,max`, iterations to the same mccode.dat-style file).
