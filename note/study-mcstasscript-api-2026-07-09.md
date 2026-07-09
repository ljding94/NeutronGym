# Study: McStasScript 0.0.84 API — surface, error behavior, MCP-server gaps

**Date:** 2026-07-09 · Systematic study of the installed McStasScript package, conducted to ground the M1 server design (see `m1-server-design-2026-07-09.md`). All behavior verified by running code in this env, including deliberately-broken instruments.

## Headline findings that changed the plan

1. **Own the execution path.** `backengine()` is synchronous (no timeout/cancel/PID), discards mcrun's returncode and output, raises a generic `ValueError` on compile failure (real compiler message only printed to stdout with ANSI codes), and — worst — **returns `[]` with no exception when the simulation fails at runtime after creating the data folder**. The server must run mcrun via its own subprocess wrapper (captured output, timeout, deterministic `-d`), using McStasScript only up to `write_full_instrument()`.
2. **Introspection is programmatic after all** — not via the print-only helpers, but via `instr.component_reader.read_name(name)` → `ComponentInfo` with `parameter_names/defaults/types/units/comments`, where **`default is None` means required**. `load_all_components()` bulk-loads all 374. The server's catalog is a thin JSON wrapper + cache over this.
3. **Validation is two-phase and has a hole.** Name-level checks are immediate (good exceptions); required-params and RELATIVE-reference checks only fire at write time via `check_for_errors()`; and the variable-reference check **skips any string that isn't pure-alpha** (`str.isalpha()`), so `undeclared_var` or `2*foo` silently pass. Server must call `check_for_errors()` eagerly, add required-param completeness checks at tool-call time, and close the isalpha loophole.
4. **The `.instr` reader is too fragile for round-tripping complex instruments** (SNS_BASIS fails on param/DECLARE name shadowing; ISIS_OSIRIS on keyword-prefixed C lines in EXTEND). `load_instr_file` stays an escape hatch with explicit best-effort semantics; the registry's declarative spec is the persistence backbone.
5. **Env fix identified: `ncrystal` was missing** — PowderN/NCrystal_sample carry `DEPENDENCY @NCRYSTALFLAGS@` and compilation dies without `ncrystal-config`. (Installed via conda-forge on 2026-07-09, same day as this study.)

---

## 1. Package layout + entry points

`import mcstasscript as ms` exposes: `McStas_instr`, `McXtrace_instr`, `McStas_file`, `load_data`, `load_metadata`, `load_monitor`, `make_plot`, `make_sub_plot`, `make_animation`, `name_search`, `name_plot_options`, `Configurator`, `Cryostat`, `Diagnostics`, `has_component`, `has_parameter`, `all_parameters_set`.

| Module | Contents |
|---|---|
| `interface/instr.py` (3.4k loc) | `McCode_instr` base (inherits libpyvinyl `BaseCalculator`), `McStas_instr` — all construction/settings/backengine logic |
| `interface/functions.py` | `load_data`, `name_search`, `Configurator` (**writes into site-packages `configuration.yaml` — global mutable state, avoid**; PATH auto-detection works in this env) |
| `interface/plotter.py` | `make_plot`, `make_sub_plot` (headless PNG verified) |
| `interface/reader.py` + `instr_reader/` | `.instr` parser (heuristic, line-oriented) |
| `helper/mcstas_objects.py` | `Component` (frozen-attribute class), `DeclareVariable` |
| `helper/component_reader.py` | `ComponentReader`, `ComponentInfo` — .comp metadata parsing |
| `helper/managed_mcrun.py` | `ManagedMcrun` subprocess wrapper (discards process state) |
| `data/data.py` | `McStasData(Binned/Event)`, `McStasMetaData` |
| `tools/` | `instrument_checker`, cryostat builder |
| `instrument_diagram/`, `instrument_diagnostics/` | `show_diagram()`, beam diagnostics |

## 2. Construction API + verified error behavior

Constructor: `McStas_instr(name, input_path=".", output_path="<name>_data", package_path, executable_path, ncount, mpi, seed, force_compile, gravity, checks, NeXus, openacc, …)`.
- `input_path` = work dir: `.instr`/`.c`/`.out` land here (mcrun runs with `cwd=input_path`); local `.comp` files here **override** installed components; must pre-exist.
- **Side effect**: constructing an instrument immediately creates `<name>_db/` in input_path.
- `output_path` resolves relative to process CWD at backengine time, not input_path.

`add_component(name, component_name, before/after, AT, AT_RELATIVE, ROTATED, ROTATED_RELATIVE, RELATIVE, WHEN, EXTEND, GROUP, JUMP, SPLIT, c_code_before/after)` — all keywords render correctly into the generated file. Params set as attributes (`comp.nx = 100`) or `comp.set_parameters(...)`; values may be numbers, strings (C expressions/var names — `'"file.dat"'` double-quoting for string literals, unvalidated), or Parameter/DeclareVariable objects. Also `copy_component`, `remove_component`, `move_component`, `get_component`, `add_parameter`, `add_declare_var`, `append_initialize`, `run_to`/`run_from` (MCPL splitting).

### Error behavior (all triggered live, verbatim)

```
[unknown component type]            NameError: No component named NotAComponent in McStas installation...   # immediate
[duplicate instance name]           NameError: Component name "source" used twice...                        # immediate
[unknown param via attribute]       AttributeError: No parameter called 'not_a_param' in component named source of component type Source_simple.  # immediate
[unknown param via set_parameters]  NameError: Unknown parameters: ['not_a_param']                          # immediate
[unknown instr param]               KeyError: "Unknown parameters: ['nope']"                                # different type!
[missing required param]            NameError: Required parameter named w1 in component named guide not set.  # only at write/backengine time
[RELATIVE to unknown comp]          McStasError: Component 'psd' referenced unknown component named 'ghost'.  # at write time (checks=True default)
[pure-alpha undeclared var]         McStasError: Variable not recognized. Unrecognized variable 'undeclaredvar'...  # at write time
[undeclared var WITH underscore]    NO ERROR — check_parameters() skips strings failing str.isalpha()      # LOOPHOLE
[unset instrument parameter]        RuntimeError: Parameter value not set for parameter: 'wl'...            # at backengine
```

Pre-flight validation exists: `instr.check_for_errors()` / `has_errors()`; `McStasError` at `mcstasscript.helper.exceptions`. Plus `tools.instrument_checker.{has_component, has_parameter, all_parameters_set}`.

## 3. Component introspection (the server's catalog source)

`ComponentReader` scans a hardcoded whitelist (`sources, optics, samples, monitors, misc, contrib, obsolete, union, astrox, sasmodels`) + input_path (overrides). Counts here: 374 total (contrib 96, sasmodels 94, optics 45, monitors 43, union 40, obsolete 15, misc 14, samples 14, sources 13).

```python
info = instr.component_reader.read_name("PSD_monitor")   # ComponentInfo
info.name, info.category
info.parameter_names        # ordered
info.parameter_defaults     # dict; None == REQUIRED  (Guide → ['w1','h1','l'])
info.parameter_types        # {'nx': 'int', 'filename': 'string', ...}
info.parameter_units, info.parameter_comments
instr.component_reader.load_all_components()             # bulk {name: ComponentInfo}
```

All shipped helpers (`available_components`, `component_help`, `show_components`) are print-only (return None) — wrap ComponentInfo into JSON instead, and cache (parsing 374 files is per-call file I/O).

## 4. Execution semantics

`settings(ncount, mpi, seed, force_compile, output_path, increment_folder_name, custom_flags, suppress_output, gravity, checks, NeXus, openacc, …)`; defaults: ncount 1e6, force_compile True, increment_folder_name True, checks True.

`backengine()` (verified): fully synchronous `subprocess.run(shell=True)`, no timeout; writes `.instr`, checks instrument params set, runs `mcrun -c -n <ncount> [--mpi] [--seed] -d <output_path> name.instr par=val…` with `cwd=input_path`. Timings: 1.7 s compile+run trivial instrument; 0.6 s re-run with `force_compile=False` (per-run parameter changes need no rebuild — passed on CLI).

Return/error semantics:
- Success → `list[McStasDataBinned]`.
- Compile error → `ValueError: Simulation failed and no data was written to disk` + UserWarning; **the actual compiler message is only on stdout (ANSI-colored)**; ManagedMcrun discards returncode/output.
- Runtime failure after data dir created → **no exception, returns `[]`** (the dangerous silent mode).
- `increment_folder_name=True` (default) silently renames output to `<dir>_0`, `_1`…; with False, existing dir → NameError. Actual dir only knowable via `ManagedMcrun.data_folder_name` or the data objects.

Persistence: `instr.dump(file)`/`McStas_instr.from_dump(file)` (dill) round-trips — works but version-fragile.

## 5. Data objects

`McStasDataBinned`: `.name`, `.Intensity/.Error/.Ncount` (numpy; 1D `(n,)` + `.xaxis`, 2D `(ny, nx)`; also 0D and Event variants with `make_1d/make_2d`). `.metadata`: `dimension`, `limits`, `xlabel/ylabel/zlabel/title`, `component_name`, `filename`, **`total_I/total_E/total_N`** (from the `values:` line), `parameters` (run's instrument params), and raw `info` dict with `statistics` (`'X0=…; dX=…;'`) and `signal` (`'Min/Max/Mean'`) as **unparsed strings** — parse these for summary stats.

`ms.load_data(dir)` reloads any output folder via mccode.sim. `ms.name_search(name, data)` fetches one monitor. `make_plot/make_sub_plot(data, filename=png)` save PNGs headlessly with Agg — verified.

## 6. Reading existing .instr files (escape-hatch quality)

Line-oriented heuristic parser. Verified results:

| Instrument | Features | Result |
|---|---|---|
| templateSANS | 10 comps, SPLIT | clean round-trip (incl. `write_python_file()`) |
| PSI_DMC | 32 comps, DECLARE×24, SPLIT×4 | clean structural round-trip |
| elearning/SANSsimple | EXTEND, GROUP, WHEN, SPLIT | round-trips correctly |
| SNS_BASIS | 154 comps | **FAILS**: DEFINE param also declared in DECLARE (legal McStas, rejected namespace check) |
| ISIS_OSIRIS | 100 comps, EXTEND×43, GROUP×40 | **FAILS**: `IndexError` — EXTEND line `groupNumber=0;` prefix-matches the GROUP keyword |

Failure classes: (a) parameter/DECLARE name shadowing, (b) keyword-prefixed identifiers in embedded C, (c) any deviation from expected line formatting.

## 7. Gaps the MCP server must fill

1. **Async job management** — own process supervision, job IDs, status polling, timeout/kill.
2. **Structured error capture** — run mcrun directly; strip ANSI; return compiler/runtime diagnostics + returncode as data; treat empty results as failure.
3. **Validation at call time** — required-param completeness from `parameter_defaults[p] is None` at add/validate time; type checks; string-quoting check; close the isalpha loophole; expose `check_for_errors()` as a validate tool.
4. **Workspace hygiene** — per-instrument dir as `input_path`; absolute deterministic output dirs; expect the `<name>_db/` side-effect dir.
5. **Registry/persistence** — server-owned declarative JSON spec, rebuild objects on load (preferred over dill dumps and over the fragile reader).
6. **Summary statistics** — from arrays + `total_I/E/N` + parsed `statistics`/`signal` strings.
7. **Catalog as JSON + cache** — wrap ComponentReader.
8. **Env quirks** — ncrystal-config (fixed); Configurator mutates site-packages (avoid); seed ≠ 0; MPI needs `-c` on toggle.
