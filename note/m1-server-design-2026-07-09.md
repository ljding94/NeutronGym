# M1 design: `mcstas-mcp` MVP server

**Date:** 2026-07-09 · Synthesis of the three studies (`study-mcstas-software-*`, `study-mcstasscript-api-*`, `study-instrument-papers-*`) into the concrete M1 design. Supersedes the sketch in the SPEC §4/§6 where they differ.

## Architecture decision: split construction from execution

McStasScript is kept for what it does well and **bypassed for what it does badly**:

- **Use McStasScript for**: component catalog (`ComponentReader.read_name()` → names/types/defaults/units/docs; `default is None` = required), instrument construction (`McStas_instr`, `add_component` — good immediate name-level errors), pre-flight validation (`check_for_errors()`), `.instr` generation (`write_full_instrument()`), data loading (`ms.load_data()`, `metadata.total_I/E/N`), PNG rendering (`make_sub_plot(filename=…)`, headless-verified).
- **Server owns execution**: run `mcrun` via its own `subprocess` wrapper — captured stdout/stderr (ANSI-stripped), returncode, timeout/kill, deterministic `-d` output dir, `cwd` = per-instrument workdir. Rationale (verified): `backengine()` has no timeout, discards mcrun output/returncode, raises a generic ValueError on compile failure with the real compiler message only on stdout, and **silently returns `[]` on runtime failure** — unusable for agent-grade diagnostics.

## Server-side rules distilled from verified gotchas

| # | Rule | Source finding |
|---|---|---|
| 1 | Always run mcrun with `cwd` = per-instrument workdir; never repo/server CWD | mcrun + McStasScript compile into CWD; `McStas_instr()` also creates `<name>_db/` there |
| 2 | Always pass every instrument parameter explicitly on the mcrun command line | zero-param invocation → interactive `mcreadparams` prompt hangs the run (M0 gotcha) |
| 3 | Always `-c` (force recompile) when the MPI setting changes; cache binary otherwise | stale non-MPI binary + `--mpi` → exit 255 |
| 4 | Reject `seed=0`; treat seed as part of the run record | mcrun requires seed ≠ 0; reproducibility for benchmark |
| 5 | Validate required params at `add_component`/`validate` time from `parameter_defaults[p] is None` | McStasScript defers this to write time |
| 6 | Close the `isalpha()` loophole: check identifiers in param values against declared vars/params, including underscored names | `undeclared_var` passes McStasScript's check silently |
| 7 | Treat "no data written" AND "empty results list" as failures with diagnostics | backengine's silent `[]` mode |
| 8 | Deterministic output dirs: server names them (`runs/<instrument>/<job_id>`), no reliance on `increment_folder_name` | default silently renames to `_0`, `_1`, … |
| 9 | Never touch `Configurator` (mutates site-packages YAML); rely on PATH auto-detection | verified auto-detection works in-env |
| 10 | Error messages must name the next action (nearest-match suggestions for unknown components/params) | spec §4.2 requirement, now concretely implementable |

## Registry & persistence

Server-owned **declarative JSON spec** per instrument (name, parameters, declares, ordered components with type/AT/ROTATED/RELATIVE/params/EXTEND-WHEN-GROUP-SPLIT strings) as the source of truth; McStasScript objects are rebuilt from the spec on demand. Not dill dumps (version-fragile), not `.instr` re-parsing (reader verified fragile: SNS_BASIS, ISIS_OSIRIS fail). Registry dir: `~/.mcstas-mcp/projects/<project>/instruments/*.json` + exported `.instr` alongside. `load_instr_file` (M2) is explicitly best-effort with the known failure classes in its error message.

## MVP tool set (8 tools, revised signatures)

1. `list_components(category?, search?)` → `[{name, category, one_line_doc}]` — from a cached catalog built once via `ComponentReader.load_all_components()` (374 components; include contrib/union/sasmodels, tag `obsolete`).
2. `describe_component(name)` → `{name, category, description, parameters: [{name, type, unit, default, required, doc}]}` — required = `default is None`.
3. `create_instrument(name, parameters?)` → `{instrument_id}` — creates workdir, registry entry.
4. `add_component(instrument_id, name, component, at, relative?, rotated?, parameters?, after?)` → `{ok, warnings}` — validates: component exists (else nearest matches), instance name unique, params known (else nearest matches), required params present-or-flagged, RELATIVE target exists, identifier references resolve (rule 6).
5. `set_parameters(instrument_id, component_name, parameters)` → `{ok, warnings}` — same validation.
6. `run_simulation(instrument_id, ncount=1e6, parameters?, seed?, mpi?, gravity?)` → synchronous for MVP, hard ncount cap 1e8, timeout (default 600 s); returns `{ok, output_dir, stdout_tail}` or `{ok: false, stage: translate|compile|run, diagnostics}` with ANSI-stripped compiler/runtime errors.
7. `get_results(job_or_instrument_id)` → per monitor: `{name, component, dims, position, total_I, total_E, total_N, stats: {X0, dX, Y0?, dY?}, signal: {min, max, mean}, xlabel, ylabel, limits}` — parsed from mccode.sim `values:`/`statistics:`/`signal:` lines; **never raw arrays**.
8. `get_monitor_data(id, monitor, format="png"|"array_summary")` — PNG via `make_sub_plot(filename=…)`; array_summary returns downsampled slices, capped size.

M2 additions unchanged (async jobs, validate_instrument, load/export escape hatches, examples corpus tools, persistence polish). **M4 change**: `optimize` wraps `mcrun --optimize` (built-in scipy: 14 methods, `--optimize-eval` FOM expressions, `--optimize-monitor`) instead of a hand-rolled scipy loop; `scan_parameter` wraps `mcrun -N` and parses `mccode.dat` (keying yvars columns by position — component names can repeat).

## Test strategy

- pytest against the real McStas install; `@pytest.mark.slow` for anything that compiles/runs.
- Compile-only integration tests over a fixed shipped-example set (fast: `mcrun --info` / translate+cc without run, or ncount=1e3 runs).
- **`%Example:` lines as ground truth**: every shipped example carries `%Example: <params> Detector: <mon>_I=<value>` — server integration tests replicate `mctest` on 3–5 examples and compare integrated I within statistical tolerance. Same mechanism later drives benchmark T1 grading.
- The M1 acceptance test stays: one prompt → source → guide → PSD → flux reported, driven through Claude Code with the server in `.mcp.json`.

## Environment deltas applied 2026-07-09

- `ncrystal` installed into the `mcstas` env (conda-forge) — PowderN/NCrystal_sample carry `DEPENDENCY @NCRYSTALFLAGS@` and need `ncrystal-config` at compile time; without it PSI_DMC-class instruments (prime benchmark tasks) fail.
- Name collision noted: the env's `mcp` binary is the unrelated (and broken) Python MCP CLI, not a McStas tool; our server entry point will be `mcstas-mcp`.

## Benchmark spillover (feeds M5, recorded here so it isn't lost)

- **T1 seen-tier shortlist** (shipped, real instrument, paper-anchored): PSI_DMC, SNS_BASIS, SNS_ARCS, ILL_IN5, ILL_IN6, ILL_H15_D11, ILL_IN13, PSI_Focus, ISIS_LET, ISIS_SANS2d, ILL_D2B, ISIS_IMAT, RITA-II, SESANS_Delft, templateSANS (sanity anchor).
- **Held-out shortlist** (2024–26 paper, no public .instr, per-instrument evidence): BOYA, CSNS cold DGS, CSNS EMD, PIONEER, FRM-II MUSHROOM-type, Jülich HBS diffractometer, POLANO, PIK suite, VENUS. Re-verify ESS GitLab before use.
- **Instruments with papers but no public model** (T3 open-design candidates): IN16, TOFTOF, NG7 SANS, CANDOR, DREAM.
- `%Example:` expected values + `mctest` = the T1 grading harness skeleton.
- Novelty check passed (no LLM+McStas prior work as of 2026-07); methodology citations verified in `study-instrument-papers-2026-07-09.md` §3.
