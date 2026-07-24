# McStasBench — Implementation Plan

**Created:** 2026-07-09 · **Living document** — check off items and revise dates as work proceeds.
Derived from `note/mcstas-mcp-feasibility-and-spec.md` (SPEC) and `note/scope-decision-2026-07-09.md` (benchmark = headline, agent = baseline). Assumes part-time effort, ~11 weeks → draft by mid-October 2026.

## Purpose (aligned 2026-07-24 — every milestone must serve one of these)

1. **Reproduction:** can an LLM agent reproduce a neutron instrument from its
   instrument-design paper, graded on simulation observables?
2. **Improvement:** can an LLM agent improve an instrument design against
   quantitative target specs, measured against classical baselines?

Both questions are answered on the same infrastructure (mcstas-mcp server +
design skill + grading harness). Purpose 1 is benchmark tier T1; purpose 2 is
tier T2 (+ open-design T3) — **neither tier is cuttable**.

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

### M1 — MVP MCP server ✅ DONE 2026-07-21

Design is fully specified in `note/m1-server-design-2026-07-09.md` (grounded in the three 2026-07-09 studies — read it before implementing). Core decision: McStasScript for construction/introspection/validation/data-loading, **server-owned subprocess for execution** (backengine is sync, discards diagnostics, and silently returns `[]` on runtime failure).

- [x] Pre-M1 systematic study: McStas toolchain, McStasScript API (verified error behaviors), instrument papers + .instr pairs → `note/study-*-2026-07-09.md`
- [x] Env fix: `ncrystal` installed (PowderN/NCrystal instruments compile; PSI_DMC verified running)
- [x] Package skeleton: `src/mcstas_mcp/{server,catalog,registry,execution,results,config}.py`, pyproject (editable install), `.mcp.json`
- [x] Component catalog: cached introspection over ComponentReader (374 comps); `list_components`, `describe_component` (required = default None), one-line docs from `%D`
- [x] Registry: declarative JSON spec per instrument; `create_instrument`, `add_parameter`, `add_component`, `set_parameters`, `get_instrument` with call-time validation (nearest-match errors, required-param warnings, RELATIVE checks, isalpha-loophole closed, string auto-quoting)
- [x] Execution: `run_simulation` via server-owned subprocess (ANSI-stripped diagnostics with translate/compile/run stage, timeout, deterministic `-d`, all params explicit, seed≠0, ncount cap 1e8); persistent job records
- [x] Results: `get_results` summary stats parsed from mccode.sim; `get_monitor_data` PNG (headless) or downsampled arrays; <1000-event monitors flagged
- [x] Tests: 28 passing (catalog/registry/execution/server-over-MCP + 2 shipped examples graded against `%Example:` values); rule 11 added (pint rejects McStas units — never pass unit= to McStasScript)
- [x] Registered in `.mcp.json`; verified over real stdio transport; human demo `scripts/m1_walkthrough.py` (build → validate → run → stats table → PNGs)
- [x] Adversarial review (fresh-context agent + author pass): 2 critical (silent wrong-physics values, MCP-stdin inheritance) + 6 major issues found and fixed; rules 12–15 added to design note; 54 tests passing incl. regression suite `tests/test_adversarial_review.py`
- [x] First acceptance attempt (2026-07-20) exposed a deployment bug: Claude Code launches the server without the conda env on PATH → first tool call died; the agent correctly self-diagnosed and patched `.mcp.json`. Fixed server-side (rule 16: self-locate from `sys.executable`); regression in `tests/test_deployment.py`; `.mcp.json` reverted to minimal form
- [x] **Accepted 2026-07-21 — de-risk gate 2 passed.** From the single acceptance prompt, the agent completed end-to-end with no help: 11 tool calls, built source→guide→PSD with a wavelength instrument parameter, ran 1e6 then 1e7 rays (consistency check), reported 1.2502e11 n/s ± 0.07% (6.6M events) → 8.33e9 n/s/cm², disclosed its source-brightness assumption. Every claim verified against mccode.sim on disk; diagram confirms topology. Notable agent behaviors for M3 skill/benchmark: per-area normalization unprompted, statistics floor respected, no seed fixed (worth a skill rule).

### M2 — Robustness ✅ DONE 2026-07-21

- [x] Async job manager: `run_simulation(wait_s)` hybrid — short runs return finished, long ones return `running` for `job_status` polling (elapsed + log tail); `cancel_job` kills the process group; per-job log files; jobs recover after server death via pid + on-disk reconciliation (orphans past 1.5× timeout get killed)
- [x] Deferred M1-review items: string/int-typed instrument parameters; declares + WHEN/EXTEND/GROUP/SPLIT in spec and tools; cross-process file locking (fcntl) with atomic writes; binary caching via comment-insensitive `.instr` sha (validate compiles once, parameter iterations never recompile)
- [x] `validate_instrument`: required-param audit + translate/compile/1-ray probe with staged diagnostics
- [x] `load_instr_file` / `export_instr_file` escape hatches — the loader converts reader output into a full spec (params/declares/INITIALIZE/WHEN/EXTEND/GROUP/SPLIT; bare `SPLIT` = McStas default 10), fidelity-tested: imported templateSANS reproduces the direct-file run at the same seed; failure messages name the reader's known failure classes
- [x] `list_examples` / `get_example` over the 297-example corpus, exposing `%Example:` ground-truth lines
- [x] Persistence: per-run `.instr` snapshots + seed/ncount/params in every job record. **Decision:** no `projects/` level — flat `~/.mcstas-mcp/`, benchmark episodes isolate via `MCSTAS_MCP_HOME`
- [x] Error-quality pass: every failure names the next action (poll `job_status`, fix with `set_parameters`, reader failure classes, nearest-match suggestions)
- [x] **Accepted:** kill and restart the server mid-run over real stdio — registry and job results survive (`test_restart_survival_acceptance`). Suite: 72 tests passing.
- Human demo: `conda run -n mcstas python scripts/m2_walkthrough.py` (import shipped example → validate → async poll → caching → persistence map; detector PNG at λ=8 vs λ=6 shows correct ring scaling)

### M3 — Skill (week of Jul 27)

- [x] `mcstas-instrument-design` skill per SPEC §5: SKILL.md (96 lines) + 5 references (units/conventions, figures of merit with quadrature rules, 8 instrument archetypes each pointing at shipped starting examples, component guide with traps, verification checklist) + `resolution_calcs.py` (conversions, Bragg, chopper phasing/frame overlap, guide m, SANS Q — CLI + importable; physics pinned by 8 tests)
- [x] Canonical copy in `skills/` (benchmark-installable); local sessions load it via committed `.claude/skills/` symlink
- [x] Failure-transcript rules encoded (and regression-tested in `test_skill_encodes_observed_failures`): fix a seed for any comparison (M1 acceptance run never did), 1000-event statistics floor, `restore_neutron=1` on diagnostics, disclose source-brightness assumptions, compute chopper phases don't scan them
- [ ] Keep refining from future transcripts: every recurring agent mistake becomes a skill line (standing task through M5/M6)
- [ ] **Accept (user-driven):** on 3 informal dev tasks, agent-with-skill avoids the unit/statistics/phasing errors that agent-without-skill makes (eyeball comparison; the rigorous version is the M5 ablation).

### M4 — Optimization layer (weeks of Aug 3–10)

These tools are double-duty: agent capability AND the **classical baselines
that purpose-2 (T2) tasks are measured against** — a T2 score is only
meaningful relative to what `mcrun --optimize` achieves on the same
parametrization under the same compute.

- [ ] `scan_parameter` wraps `mcrun -N` (parse `mccode.dat`; key yvars columns by position — component names can repeat)
- [ ] `optimize` wraps `mcrun --optimize` — mcrun has a built-in scipy optimizer (14 methods, `--optimize-eval` FOM expressions, `--optimize-monitor`); no hand-rolled loop needed
- [ ] FWHM/CoM in `get_results`
- [ ] Baseline-runner mode: the same scan/optimize machinery invocable headlessly by the benchmark harness (not only via MCP) for T2 baseline curves
- **Accept:** reproduce a guide_bot-style task — maximize brilliance transfer into 2×2 cm², ±0.5°, given λ-band — and match the classical optimizer's FOM within noise. *This doubles as the first T2 task prototype.*

### M5 — Benchmark curation (weeks of Aug 10 – Sep 4) ← headline contribution

- [ ] **Pilot first (kill-list item 4):** 3 reproduction tasks — one memorization probe, one underspecified paper — to validate the grading rubric *before* curating at scale
- [ ] Task inventory — head start from the 2026-07-09 studies (`note/m1-server-design-2026-07-09.md` §Benchmark spillover): 15-instrument seen-tier shortlist with verified DOIs, 9 held-out candidates (2024–26, no public .instr, per-instrument contamination evidence), 5 paper-but-no-model instruments for T3; select 20–30 (paper, reference `.instr`, reference monitor outputs) triples
- [ ] T1 grading skeleton: shipped `%Example:` lines carry expected detector values (`mctest` mechanism) — free ground truth for integrated-intensity checks
- [ ] Tier structure (maps 1:1 to the Purpose questions):
  - **T1 reproduce (purpose 1):** task input = the instrument paper (or excerpts/tech report), agent rebuilds the instrument, graded on simulation observables vs the reference `.instr` outputs. An NL-spec-sheet variant per task acts as the pipeline-decomposition control (separates "couldn't extract the spec from the paper" from "couldn't build the instrument")
  - **T2 improve to target specs (purpose 2):** task input = a working baseline instrument + quantitative targets (e.g. "≥20% more flux at sample, same Δλ/λ, same envelope"); metrics = FOM vs three baselines under matched compute — the published/baseline design, classical optimization (`mcrun --optimize` on the same parametrization, the M4 tools), and random search; success = target met AND improvement > 3σ re-verified at high ncount with a fresh seed
  - **T3 open design:** goal-only specification; expert rubric + FOM
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
- [ ] T2 analysis is core (purpose 2) and **not cuttable**. Only the stretch layer of SPEC Study B — expert-adjudicated claims that an agent design *beats the published instrument* — is cut-if-behind (that claim standard is a follow-up paper on its own)
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
