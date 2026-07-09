# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project status

**M0 complete (2026-07-09): environment bootstrapped, smoke test passing.** Git repo initialized; next milestone is M1 (MVP MCP server). Sources of truth, in order:

- `note/mcstas-mcp-feasibility-and-spec.md` (2026-07-08) — feasibility analysis, full SPEC, research plan.
- `note/scope-decision-2026-07-09.md` — scope/framing decision; supersedes §7 framing in the spec where they differ.
- `PLAN.md` — the concrete implementation plan: milestones M0–M7 with acceptance criteria, de-risk gates, and standing decisions. This is the living document; check items off there.
- `note/m1-server-design-2026-07-09.md` — the M1 server design (construction-vs-execution split, 10 server-side rules, revised tool signatures, benchmark spillover). Grounded in three systematic studies: `note/study-mcstas-software-*.md`, `note/study-mcstasscript-api-*.md`, `note/study-instrument-papers-*.md` — consult these before touching McStas/McStasScript integration code; they contain verified error behaviors and gotchas.
- `progress.html` — human-friendly dashboard generated from PLAN.md. **After editing PLAN.md, regenerate it**: `python3 scripts/progress_report.py` (stdlib only, any python). Never edit progress.html by hand.

Read these before doing any design or implementation work here; keep them updated when decisions change.

## What this project is

A research program around LLM agents designing neutron instruments with [McStas](https://www.mcstas.org/) (a Monte Carlo ray-tracing simulator whose instruments are text-based `.instr` files).

**Framing (decided 2026-07-09): McStasBench is the paper and headline contribution — a benchmark evaluating LLM agents on McStas tasks (NeurIPS D&B / ICLR class). McStasAgent is the reference baseline shipped inside it**, not a standalone contribution: an agent-tooling paper alone reads as commoditized engineering, and the agent needs the benchmark to prove anything. The agent's failure modes on the benchmark are the analysis section of the paper.

Deliverables:

1. **McStasBench** (headline) — tiered evaluation (reproduce / optimize / open-ended design) run through headless Claude Code as a universal scaffold across model tiers (spec §7). Benchmark quality bars are load-bearing: contamination controls, difficulty tiers, pipeline-decomposed metrics.
2. **`mcstas-mcp` server** (baseline tooling) — Python (FastMCP) wrapping [McStasScript](https://github.com/PaNOSC-ViNYL/McStasScript): component discovery/introspection, structured instrument construction, async simulation jobs, parameter scans, scipy-driven optimization. Tool signatures in spec §4.
3. **`mcstas-instrument-design` skill** (baseline tooling) — the judgment layer: units/conventions, figures of merit, instrument archetypes, verification checklist (spec §5).

## Key architecture decisions (from the spec — don't relitigate without reason)

- **Structured construction is primary, raw `.instr` text is the escape hatch.** Every mutating tool call validates immediately against component metadata — fail at tool-call time, not compile time.
- **Async job model** for simulations (`run_simulation` → `job_status` → `get_results`); iterate at low `ncount` (1e5–1e6), production-validate at ≥1e8. Never compare designs on monitors with <1000 events.
- **`get_results` returns summary statistics** (intensity, error, event counts, peak, CoM, FWHM), not raw arrays; agents request PNGs or slices when needed.
- **Server-side scipy optimization** for continuous parameters; agent reasoning reserved for topology choices.
- Target **McStas 3.x only**; state persists in `~/.mcstas-mcp/projects/<name>/`.
- Benchmark requires **contamination controls**: held-out recent instruments, memorization probes, perturbed variants (§7, Study A).

## Environment (working, verified 2026-07-09)

Conda env `mcstas`: McStas 3.7.12 (conda-forge, osx-arm64) + McStasScript 0.0.84 + NCrystal 4.4.6 (required by PowderN-class components) + fastmcp + pytest + ply (needed by `mcdisplay-*` visualization). MPI available (Homebrew Open MPI on PATH). Instrument visualization — use the friendly wrapper: `conda run -n mcstas python scripts/view_instrument.py <file.instr | shipped-example-name> [param=value ...]` (validates parameters, uses defaults automatically, keeps build artifacts in `runs/_view/`, opens browser). Add `--diagram` for the 2D component-connection schematic PNG (McStasScript `show_diagram`; falls back gracefully where the .instr reader fails). Raw tool underneath: `mcdisplay-webgl`. This conda-forge install is the single McStas on this machine — do NOT add the official macOS app bundle alongside it (duplicate toolchains/PATH conflicts). Run anything McStas-related via `conda run -n mcstas ...`. Smoke test: `conda run -n mcstas python scripts/m0_smoke_test.py` (three stages: mcrun CLI → McStasScript data load → programmatic build; also documents two mcrun gotchas — interactive parameter prompting and CWD compilation). Simulation outputs go in `runs/` (gitignored).

## Where to start

Follow `PLAN.md` — milestones M0–M7 with acceptance criteria and dates. Ordering: environment smoke test → 8-tool MVP server → robustness/async → skill → optimization layer → benchmark curation → evaluation runs → analysis/writing. The server and skill are built first even though the benchmark is the headline — the benchmark harness runs on them. When beginning implementation, initialize git first (M0 item 1).
