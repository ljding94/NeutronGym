# McStasBench — Implementation Plan

**Created:** 2026-07-09 · **Living document** — check off items and revise dates as work proceeds.
Derived from `note/mcstas-mcp-feasibility-and-spec.md` (SPEC) and `note/scope-decision-2026-07-09.md` (benchmark = headline, agent = baseline). Assumes part-time effort, ~11 weeks → draft by mid-October 2026.

## Machine facts (verified 2026-07-09)

Apple Silicon Mac, 8 cores, 16 GB RAM. Miniconda at `/opt/homebrew/Caskroom/miniconda/base` (no `mcstas` env yet, no McStas binaries). `claude` CLI 2.1.195, `git`, `gh` all present. conda-forge ships `mcstas-core`/`mcstas-data` for osx-arm64. 8 cores is fine for design iteration at 1e6–1e7 rays; production 1e9 runs are hours-scale — acceptable for final validation passes, revisit HPC only if M6 wall-clock hurts.

## Milestones

### M0 — Environment + repo bootstrap ✅ DONE 2026-07-09

- [x] `git init`; add `.gitignore` (conda, `__pycache__`, McStas output dirs, `.DS_Store`)
- [x] `conda create -n mcstas -c conda-forge mcstas-core mcstas-data python=3.11 -y` → **McStas 3.7.12**, osx-arm64, works
- [x] `conda run -n mcstas pip install mcstasscript fastmcp pytest` → **McStasScript 0.0.84**
- [x] Smoke test (`scripts/m0_smoke_test.py`): templateSANS at 1e6 rays via `mcrun` + McStasScript data load + programmatic source→PSD build via the API; PSD PNG shows correct SANS ring pattern
- **Accepted:** all monitors nonzero-intensity; full run seconds-scale; PNG renders. **De-risk gate 1 passed** — no Docker fallback needed.
- Gotchas found (encode in M1 server): (1) running an `.instr` with zero CLI parameters makes the binary prompt interactively (`mcreadparams`) and hang — always pass at least one parameter or `-N`-style defaults; (2) `mcrun` compiles into the CWD — always run in a scratch/build dir, never the repo root.

### M1 — MVP MCP server (week of Jul 13)

Design is fully specified in `note/m1-server-design-2026-07-09.md` (grounded in the three 2026-07-09 studies — read it before implementing). Core decision: McStasScript for construction/introspection/validation/data-loading, **server-owned subprocess for execution** (backengine is sync, discards diagnostics, and silently returns `[]` on runtime failure).

- [x] Pre-M1 systematic study: McStas toolchain, McStasScript API (verified error behaviors), instrument papers + .instr pairs → `note/study-*-2026-07-09.md`
- [x] Env fix: `ncrystal` installed (PowderN/NCrystal instruments compile; PSI_DMC verified running)
- [ ] Package skeleton: `src/mcstas_mcp/{server,components,instruments,execution,results}.py`, pyproject, `.mcp.json`
- [ ] Component catalog: cached JSON from `ComponentReader.load_all_components()` (374 comps); `list_components`, `describe_component` (required = default None)
- [ ] Registry: declarative JSON spec per instrument (NOT dill, NOT .instr re-parsing); `create_instrument`, `add_component`, `set_parameters` with call-time validation (nearest-match errors, required-param checks, RELATIVE checks, isalpha-loophole closed)
- [ ] Execution: `run_simulation` via own subprocess (ANSI-stripped diagnostics, timeout, deterministic `-d`, all params explicit on CLI, `-c` on MPI toggle, seed≠0) — the 10 server-side rules in the design note
- [ ] Results: `get_results` summary stats parsed from mccode.sim `values:/statistics:/signal:` lines; `get_monitor_data` PNG via `make_sub_plot`
- [ ] Tests: pytest vs real install; 3–5 shipped examples graded against their `%Example:` expected values (mctest-style)
- [ ] Register in `.mcp.json`; drive manually from Claude Code
- **Accept:** from the single prompt "build a source → guide → PSD instrument and tell me the flux at the detector," the agent completes end-to-end with no human help. *Kill-list item 2.*

### M2 — Robustness (week of Jul 20)

- [ ] Async job manager (`run_simulation` → `job_id`; `job_status`; `get_results`) — subprocess + state file, no queue framework
- [ ] `validate_instrument` (translate + cc, no run) with full diagnostics passthrough
- [ ] `load_instr_file` / `export_instr_file` escape hatches
- [ ] `list_examples` / `get_example` over the shipped 297-instrument corpus
- [ ] Persistence in `~/.mcstas-mcp/projects/<name>/` (instr versions, seeds, ncount per run)
- [ ] Error-quality pass: every failure message names the next action
- **Accept:** kill and restart the server mid-session; instrument registry and job results survive.

### M3 — Skill (week of Jul 27, parallel with M2 tail)

- [ ] `mcstas-instrument-design` skill per SPEC §5: SKILL.md (<200 lines) + 5 reference files + `resolution_calcs.py`
- [ ] Refine from failure transcripts: every recurring agent mistake becomes a skill line
- **Accept:** on 3 informal dev tasks, agent-with-skill avoids the unit/statistics/phasing errors that agent-without-skill makes (eyeball comparison; the rigorous version is the M5 ablation).

### M4 — Optimization layer (weeks of Aug 3–10)

- [ ] `scan_parameter` wraps `mcrun -N` (parse `mccode.dat`; key yvars columns by position — component names can repeat)
- [ ] `optimize` wraps `mcrun --optimize` — mcrun has a built-in scipy optimizer (14 methods, `--optimize-eval` FOM expressions, `--optimize-monitor`); no hand-rolled loop needed
- [ ] FWHM/CoM in `get_results`
- **Accept:** reproduce a guide_bot-style task — maximize brilliance transfer into 2×2 cm², ±0.5°, given λ-band — and match the classical optimizer's FOM within noise.

### M5 — Benchmark curation (weeks of Aug 10 – Sep 4) ← headline contribution

- [ ] **Pilot first (kill-list item 4):** 3 reproduction tasks — one memorization probe, one underspecified paper — to validate the grading rubric *before* curating at scale
- [ ] Task inventory — head start from the 2026-07-09 studies (`note/m1-server-design-2026-07-09.md` §Benchmark spillover): 15-instrument seen-tier shortlist with verified DOIs, 9 held-out candidates (2024–26, no public .instr, per-instrument contamination evidence), 5 paper-but-no-model instruments for T3; select 20–30 (paper, reference `.instr`, reference monitor outputs) triples
- [ ] T1 grading skeleton: shipped `%Example:` lines carry expected detector values (`mctest` mechanism) — free ground truth for integrated-intensity checks
- [ ] Tier structure: T1 reproduce (from NL description), T2 optimize (fixed topology vs known optima), T3 open design (expert rubric + FOM)
- [ ] Contamination controls: seen/held-out split (held-out = 2024–26 instruments with no public `.instr`); memorization probe per task; perturbed variants
- [ ] Grading harness: observable-based (flux spectrum at sample, beam profile, resolution function) with tolerance tiers; fully headless, no LLM judge for T1/T2
- **Accept:** every task graded automatically from a transcript directory; a deliberately-wrong `.instr` fails and the reference passes.

### M6 — Evaluation runs (weeks of Sep 7–18)

- [ ] **OpenRouter spike early** (kill-list item 3, actually run it in week of Jul 20): same MVP task via `ANTHROPIC_BASE_URL` with 2 non-Claude models — verifies tool-calling fidelity through the alternate backend
- [ ] Scaffold: headless `claude -p` + `.mcp.json` + skill installed; JSON transcripts; pin model IDs + Claude Code version; log tokens/tool-calls/wall-clock per episode
- [ ] Protocol: pass@1, fixed seeds, 50-turn cap, 5-task dev split for all debugging, held-out set touched once
- [ ] Model tiers: Claude via subscription ($0, spread over days under weekly caps); mid/small via OpenRouter (~$50–200); open-weights via vLLM+LiteLLM optional
- **Accept:** full run matrix complete within the $100–300 budget; transcripts reproducible from pinned config.

### M7 — Analysis + writing (weeks of Sep 21 – Oct 9)

- [ ] Failure taxonomy: format failures (tool-calling mechanics) vs physics failures
- [ ] Ablations (SPEC §7 Study C): structured vs raw `.instr`, ±introspection, ±skill, ±vision; cost/scaling curves vs token budget and ncount
- [ ] Study B case study if time permits (autonomous design improvement) — **cut first if behind schedule**; it's a follow-up paper on its own
- [ ] Paper draft: lead with benchmark + failure-mode analysis; system description as means, not claim
- **Target venue:** NeurIPS 2027 Datasets & Benchmarks (deadline ~May 2027 — comfortable). ICLR 2027 (~late Sep 2026) is too tight for a part-time schedule ending mid-October; verify actual deadlines when M5 starts and re-decide.

## De-risk gates (fatal-flaw checks, in order — from SPEC §7)

| # | Gate | When | Kills the plan if |
|---|---|---|---|
| 1 | conda McStas + example runs on this Mac | M0 | osx-arm64 binaries broken AND Docker unusable |
| 2 | Agent builds/runs instrument from one prompt | M1 | validation-at-call-time can't be made reliable |
| 3 | OpenRouter backend spike, 2 non-Claude models | week of Jul 20 | tool-calling fidelity too poor → single-model paper only |
| 4 | 3 pilot tasks incl. memorization probe | M5 start | grading rubric can't separate memorization from capability |

## Standing decisions (defaults from SPEC §8 — change only with a dated note)

- Laptop-scale compute; SLURM path deferred (async API already accommodates it)
- McStas only; McXtrace kept in the design, out of scope for MVP
- Union components / NCrystal deferred past MVP (introspection exposes them anyway)
- Single agent + skill + validating tools; no multi-agent split unless evals show persistent unforced physics errors
