# NeutronGym — Implementation Plan

*(NeutronGym = the environment, headline artifact; McStasBench = its held-out benchmark slice; repo + local dir renamed 2026-07-26)*

**Created:** 2026-07-09 · **Living document** — check off items and revise dates as work proceeds.
Derived from `note/mcstas-mcp-feasibility-and-spec.md` (SPEC) and `note/scope-decision-2026-07-09.md` (benchmark = headline, agent = baseline). Re-anchored 2026-07-26: part-time effort → **ICLR 2027 full-paper deadline 2026-09-24 AoE** (abstract 2026-09-19).

**Status (updated 2026-07-26):** infrastructure phase (M0–M4) complete — server (22 tools), skill, optimization/baseline layer; all 4 de-risk gates passed. Benchmark harness validated end-to-end: pilot agent episodes 4/4 PASS, 14 T1 tasks self-validating, 2 T2 tasks calibrated (reward red-teamed), T3 defined. **Pivot 2026-07-26 (`note/neutrongym-vision-digest-2026-07-26.md` + same-day addendum): the environment is named NeutronGym (McStasBench = its held-out benchmark slice) and the committed venue is ICLR 2027 — abstract 2026-09-19, full paper 2026-09-24 AoE — with bench AND RL both in the paper** (60 days out; RL is planned content, not a stretch goal; hardware secured: 7×A100-40G). Next: M5 environment build by ~Aug 18 with the **reward-ladder API first (~Aug 10, RL critical path)**; M8 rejection sampling → filtered SFT Aug 11 – Sep 1, GRPO Sep 1 – 15, RL numbers frozen ~Sep 17; M6 eval matrix Aug 17 – Sep 5 (prereq: widen the OpenRouter privacy policy — only Google models route today); M7 writing Sep 1 – 24.

## Purpose (aligned 2026-07-24; environment-first evolution same day; named + venue-committed 2026-07-26 — see `note/scope-evolution-rl-env-2026-07-24.md`, `note/neutrongym-vision-digest-2026-07-26.md`)

**Headline artifact: NeutronGym — a fast, physically-verifiable, inverse-design
RL environment for LLM agents.** Neutron optics is the substrate, McStas the
simulation backend. Procedurally generated, densely rewarded (three-tier
ladder), trainable; **McStasBench**, the held-out benchmark slice, falls out of
it. Three questions, every milestone must serve one:

1. **Reproduction:** can an LLM agent reproduce a neutron instrument from its
   instrument-design paper, graded on simulation observables? (held-out
   benchmark slice — curated, non-generatable)
2. **Improvement:** can an LLM agent improve an instrument design against
   quantitative target specs, measured against classical baselines? (the
   trainable core — procedurally generated)
3. **Trainability:** does physics-verifiable reward train? Small-model delta
   (rejection sampling → filtered SFT → GRPO if signal); claim bar: 7B +
   training beats 7B baseline, approaching a larger untrained model.

Structural advantages to protect: seconds-per-rollout (template families +
binary cache), fully programmatic reward (no LLM judge anywhere), Liouville
bound as physics-native hack detection, open-source releasability.

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

### M3 — Skill (built 2026-07-24; informal acceptance pending, rigorous version = M5 ablation)

- [x] `mcstas-instrument-design` skill per SPEC §5: SKILL.md (96 lines) + 5 references (units/conventions, figures of merit with quadrature rules, 8 instrument archetypes each pointing at shipped starting examples, component guide with traps, verification checklist) + `resolution_calcs.py` (conversions, Bragg, chopper phasing/frame overlap, guide m, SANS Q — CLI + importable; physics pinned by 8 tests)
- [x] Canonical copy in `skills/` (benchmark-installable); local sessions load it via committed `.claude/skills/` symlink
- [x] Failure-transcript rules encoded (and regression-tested in `test_skill_encodes_observed_failures`): fix a seed for any comparison (M1 acceptance run never did), 1000-event statistics floor, `restore_neutron=1` on diagnostics, disclose source-brightness assumptions, compute chopper phases don't scan them
- [ ] Keep refining from future transcripts: every recurring agent mistake becomes a skill line (standing task through M5/M6)
- [ ] **Accept (user-driven):** on 3 informal dev tasks, agent-with-skill avoids the unit/statistics/phasing errors that agent-without-skill makes (eyeball comparison; the rigorous version is the M5 ablation).

### M4 — Optimization layer ✅ DONE 2026-07-24

These tools are double-duty: agent capability AND the **classical baselines
that purpose-2 (T2) tasks are measured against** — a T2 score is only
meaningful relative to what `mcrun --optimize` achieves on the same
parametrization under the same compute.

- [x] `scan_parameter` wraps `mcrun -N` — instrument-parameter-only (actionable error otherwise), mccode.dat parsed with positional column keying (component names can repeat)
- [x] `optimize` wraps `mcrun --optimize` (scipy: 10 validated methods, monitor-intensity FOM or eval expressions, minimize flag, maxiter cap 500); results = best params + FOM ± err + downsampled history + scipy log tail + "re-verify fresh-seed" next_step
- [x] FWHM/CoM for 1D monitors in `get_results` (interpolated half-max crossings; synthetic-profile pinned)
- [x] Baseline-runner: `mcstas-baseline scan|optimize <instrument> ...` console script prints results JSON — the harness's classical T2 baselines without MCP
- [x] **Accepted:** guide_bot-style task (flux into 2×2 cm², ±0.5°, λ=5±0.5 Å, 10 m m=2 guide, free width) — optimizer reaches the classical scan's best FOM within statistics in 11 iterations; optimum re-verified at 1e6 with a fresh seed. *First T2 prototype; 88 tests passing.*
- Human demo: `conda run -n mcstas python scripts/m4_walkthrough.py` → `runs/m4_demo/scan_curve.png` (scan curve + optimizer line + re-verified point; FOM plateau above guide-acceptance matching is the expected physics)

### M5 — Environment + benchmark curation (late Jul – **Aug 18 hard**, ICLR-anchored) ← headline contribution

Environment work (new, from the 2026-07-24 scope evolution — mostly wraps
existing machinery). **Ordering within M5: the reward-ladder API + env
executor land first (~Aug 10) — they are the M8 critical path** (rejection
sampling cannot start without them); curation items can trail to Aug 18:

- [x] **Fast-tier rollout timing measured 2026-07-24** (`benchmark/measure_fast_tier.py`): mcrun-path rollouts are ~2.4 s FLAT regardless of ncount (wrapper+rebuild overhead, not physics); **direct binary execution = ~0.04 s/rollout at 1e5 rays** (validated output) → ~25 rollouts/s/core. Env throughput will never bound RL training
- [ ] Env executor: fast-tier `step()` runs the compiled binary directly (mcrun compiles once per template family); MCP path keeps the wrapper for interactive use
- [ ] Procedural instance generator: parameterized template families per archetype (compile-once/sample-many for the fast tier; topology variation = slow tier only); nothing memorizable; held-out parameter regimes for the eval split
- [ ] Reward-ladder API wrapping existing tiers: static (registry validation + `validate_instrument`, free) → cheap dynamic (truncated-ncount run, staged diagnostics) → terminal (full-protocol run through `benchmark/grader.py`); reward computed at env-controlled protocol (never agent-chosen ncount). Every graded episode also reports **deepest-level-reached** (L1 syntax/compile → L2 runtime → L3 structural → L4 scientific — the ladder presented as the vision note's hierarchy) as a first-class harness field: raw material for the M7 failure taxonomy and M8 "why training improves" attribution
- [ ] Multi-objective target-spec format (flux + resolution + geometry constraints jointly — single-metric gaming fails by construction)
- [ ] Anti-hacking checks: Liouville/brilliance-transfer ≤ 1 (matched phase-space monitors), degenerate-config detectors; then **red-team our own reward and write up what broke** (paper section)
- [ ] Packaging: pip-installable env + Docker with McStas baked in; one-command eval slice

Benchmark slice (curation items, as before):

- [x] **Pilot (kill-list item 4) — de-risk gate 4 passed 2026-07-24 (mechanics):** `benchmark/` harness with grader (observable-based, role-matched monitors, statistics-aware tolerances, hard-failure gating), memorization probe (live-validated: real file ≈1.0 → contaminated; flash-lite from memory 0.07 → unseen), 3 pilot tasks (P1 full-spec SANS, P2 probe, P3 underspecified). Validated: reference passes at fresh seed; λ=8 and R=30 frauds fail with localized blame; underspecified choices not punished. See `note/spike-and-pilot-2026-07-24.md`
- [x] **Pilot agent episodes complete 2026-07-24 — 4/4 PASS** (P1/P3 × claude-fable-5+skill / gemini-3.6-flash) via `benchmark/run_episode.py`; grader needed zero manual overrides; failure-taxonomy entries from two infra-failed attempts (stream-drop → M6 retry policy; harness path bug → fixed). Cost anchor $2.5–3.9/episode → M6 budget needs revisiting. See `note/pilot-episodes-and-fast-tier-2026-07-24.md`
- [x] **Task inventory verified 2026-07-24** (`benchmark/build_inventory.py` → `inventory.json` + `note/study-paper-pairs-2026-07-24.md`): 30/35 shipped candidates machine-runnable here (25 agree with `%Example` ground truth; 5 triaged with causes), ~12 new external pairs found (CNCS/MARI/MAPS/BIFROST-26-instr/CSPEC/T-REX/PANDA/HEIMDAL…), OA status of all anchors checked → **pool ≈ 40 candidates**. Policy set: paper-PDF task input only for OA anchors (3/12!), spec-sheet input for paywalled ones; contamination re-sweep mandatory before split freeze (2026 model-publication burst; PIK caveated; PaNRAID watch)
- [x] **T1 seen-tier tasks authored + validated 2026-07-24**: 14 tasks (2 dev + 12 seen; 6 instrument classes, 4–102 components) generated by `benchmark/author_tasks.py` — prompts are NL spec sheets derived from the reference via the reader→spec converter (params/declared constants/INITIALIZE logic/beam-order walk/monitor contract), grading contracts auto-derived from reference monitor roles; **14/14 self-validate** (`validate_tasks.py`: reference passes own task at fresh seed). Registry gained `RELATIVE PREVIOUS` resolution + raw-declare passthrough en route
- [x] **T2 improvement tasks calibrated + self-validated 2026-07-24** (2 tasks: guide-divergence, SANS-collimation): parameterized baseline instruments committed (`benchmark/instruments/`), targets = 80% of the **fresh-seed re-verified**, constraint-filtered classical best (`calibrate_t2.py`), multi-objective contracts, `grade_improvement` in the grader; curation gate extended (`validate_tasks.py`): classical-best must PASS at a fresh seed, unimproved baseline must FAIL — both tasks pass. **Calibration doubled as reward red-teaming: three real exploits caught before any agent saw a task** — (a) beamstop-leakage hack: unconstrained flux maximization found 672 n/s of direct beam; max-only resolution constraints have the wrong sign for leakage (it *shrinks* dX) → pattern-integrity BAND constraints; (b) `mcrun --optimize` nelder-mead ignores parameter bounds (escaped to w=2.1 m) → classical baselines are bounds+constraint-filtered ensembles; (c) **winner's curse**: the selected best over 30 noisy evaluations is biased upward — at a fresh seed it fell below its own target, and the guard honestly rejected the original guide task (baseline sat on the FOM plateau; rebased to w=0.012/m=1.5 for 2.9× verified headroom). The benchmark now obeys the same fresh-seed re-verification discipline the skill imposes on agents. All three findings are paper material (red-team-the-reward section)
- [x] T3 open-design task definitions authored (compact-SANS, ballistic-guide-with-coating-budget): automatic floors machine-checkable; expert rubric explicitly PENDING (M6/M7) — not in the scored set
- [ ] Grow T1 to 20+: reader-shadowing workaround for 6 blocked instruments (PSI_Focus/IN4/IN13/D4/H53_IN14/BASIS — DEFINE-param-shadows-DECLARE class), higher-stat protocols for IN5/LET roles, vet + admit external pairs (CNCS/MARI/MAPS/BIFROST), author held-out tier tasks (BOYA has OA preprint); grow T2 with chopper-phasing and monochromator tasks
- [ ] T1 grading skeleton: shipped `%Example:` lines carry expected detector values (`mctest` mechanism) — free ground truth for integrated-intensity checks
- [ ] Tier structure (maps 1:1 to the Purpose questions):
  - **T1 reproduce (purpose 1):** task input = the instrument paper (or excerpts/tech report), agent rebuilds the instrument, graded on simulation observables vs the reference `.instr` outputs. An NL-spec-sheet variant per task acts as the pipeline-decomposition control (separates "couldn't extract the spec from the paper" from "couldn't build the instrument")
  - **T2 improve to target specs (purpose 2):** task input = a working baseline instrument + quantitative targets (e.g. "≥20% more flux at sample, same Δλ/λ, same envelope"); metrics = FOM vs three baselines under matched compute — the published/baseline design, classical optimization (`mcrun --optimize` on the same parametrization, the M4 tools), and random search; success = target met AND improvement > 3σ re-verified at high ncount with a fresh seed
  - **T3 open design:** goal-only specification; expert rubric + FOM
- [ ] Contamination controls: seen/held-out split (held-out = 2024–26 instruments with no public `.instr`); memorization probe per task; perturbed variants
- [ ] Grading harness: observable-based (flux spectrum at sample, beam profile, resolution function) with tolerance tiers; fully headless, no LLM judge for T1/T2
- **Accept:** every task graded automatically from a transcript directory; a deliberately-wrong `.instr` fails and the reference passes.

### M6 — Evaluation runs (Aug 17 – Sep 5, overlaps M5 tail; ICLR-anchored)

- [x] **OpenRouter spike — de-risk gate 3 passed 2026-07-24**: gemini-3.6-flash + gemini-3.5-flash-lite both completed the one-prompt task through the identical scaffold (9–12 MCP calls, zero tool-format errors, jobs verified on disk, ~$0.5–0.8/episode). Harness lessons + **M6 prerequisite: widen OpenRouter privacy policy** (only Google models route today) in `note/spike-and-pilot-2026-07-24.md`
- [ ] Scaffold: headless `claude -p` + `.mcp.json` + skill installed; JSON transcripts; pin model IDs + Claude Code version; log tokens/tool-calls/wall-clock per episode
- [ ] Protocol: pass@1, fixed seeds, 50-turn cap, 5-task dev split for all debugging, held-out set touched once
- [ ] Baseline arms: **plain-LLM** (no tools — one-shot `.instr` generation from the task prompt, graded by the same harness) anchors the value of the env infrastructure; agent arms form the **±MCP × ±skill 2×2 grid** feeding the M7 ablation table
- [ ] Model tiers: Claude via subscription ($0, spread over days under weekly caps); mid/small via OpenRouter (~$50–200); open-weights via vLLM+LiteLLM optional
- **Accept:** full run matrix complete within the $100–300 budget; transcripts reproducible from pinned config.

### M7 — Analysis + ICLR paper writing (Sep 1–24, overlaps M6 tail; Figure 1 + skeleton by ~Sep 5, abstract locked Sep 19)

The paper is the **NeutronGym environment paper with the trainability result
as a co-equal contribution** (env + reward ladder + procedural generation +
frontier-model numbers on the McStasBench held-out slice + red-team-the-reward
section + **RL: filtered SFT + GRPO on a 7–8B model — planned content, in the
paper by default**). RL numbers freeze ~Sep 17 (two days before abstract
lock); if training signal genuinely fails, the paper degrades gracefully —
GRPO → SFT-only → env+eval-only — and a no-signal outcome is itself reported
honestly, but the plan is the full result. **Figure-1-first discipline:**
draw the full loop (NL requirement → agent → MCP + skill → NeutronGym →
simulation → evaluation → reward → improved agent) before writing text; the
paper should be understandable from Figure 1 alone. Claim wording: "first
executable environment for *neutron instrument design*" — never the
unqualified "first executable scientific environment" (MDGYM et al. exist).
**Re-check prior art immediately before writing the positioning section**
(this space produces papers monthly; last check 2026-07-09).

- [ ] Failure taxonomy: format failures (tool-calling mechanics) vs physics failures, organized by the harness's deepest-level-reached field (L1–L4)
- [ ] Ablations (SPEC §7 Study C): structured vs raw `.instr`, ±introspection, ±skill, ±vision; cost/scaling curves vs token budget and ncount
- [ ] T2 analysis is core (purpose 2) and **not cuttable**. Only the stretch layer of SPEC Study B — expert-adjudicated claims that an agent design *beats the published instrument* — is cut-if-behind (that claim standard is a follow-up paper on its own)
- [ ] Paper draft: lead with benchmark + failure-mode analysis; system description as means, not claim
- **Target venue (committed 2026-07-26): ICLR 2027** — abstract **2026-09-19**, full paper **2026-09-24 AoE** (web-verified 2026-07-26; re-confirm on the official CFP at M7 start). Fallback: NeurIPS 2027 Datasets & Benchmarks (~May 2027) if ICLR slips or rejects. Cuttable-if-behind, in order: T3 from the scored set → GRPO (drop to SFT-only) → T1 growth beyond the current 14; **not cuttable:** contamination controls, red-team-the-reward, T2 analysis, the SFT trainability result (bench + RL are both in the paper — user decision 2026-07-26).

### M8 — RL track, in the ICLR paper (SFT Aug 11 – Sep 1 · GRPO Sep 1 – 15 · results frozen ~Sep 17)

Purpose-3 machinery, now a **co-equal paper contribution** (user decision
2026-07-26 — bench + RL both in the ICLR paper). Recorded defaults from the
2026-07-24 scope evolution; hardware (verified 2026-07-26): **7×A100-40G**
(~3 generation / ~4 training; McStas rollouts are CPU-bound — no GPU
contention; 7–8B + LoRA fits comfortably).

- [ ] **Rejection sampling FIRST**: strong-model trajectories → filter by programmatic reward (physics-passing only) → filtered SFT on Qwen-family 7–8B → iterate. If filtered SFT shows no delta, the reward signal has a problem — learned in weeks, not months. **Starts the day the M5 reward API lands (~Aug 11), parallel with M6 (rollouts are CPU-bound, M6 episodes are API-bound — no contention). SFT delta = the GRPO go/no-go gate, decided ~Sep 1**
- [ ] Rollout logging captures per-level outcomes (deepest-level-reached) from the first trajectory — the M7/M8 attribution analysis cannot be reconstructed after the fact
- [ ] Agentic-RL framework: survey + capability test of current tooling before committing (churns fast; do not write the loop by hand)
- [ ] GRPO (critic-free), gated on the Sep 1 SFT signal: LoRA r=32–64 on ALL linear projections (attention-only low-rank is where "LoRA underperforms" comes from; justification: RL post-training sharpens existing capability — the regime where LoRA tracks full FT); ~2,500 generations/gradient step (8 samples × 32 prompts × ~10 turns). **Window Sep 1 – 15** — days per run on 7×A100 → 2–3 real runs fit
- [ ] Budget: 2–3 real GRPO runs total; one headline result + ablations that reuse rollouts; **all RL numbers destined for the paper frozen ~Sep 17** (fresh-seed re-verified on held-out tasks, same discipline as T2)
- [ ] **Analysis bar: explain *why* training helps, not just that it helps** — level-resolved attribution: do failures migrate from L1/L2 (syntax/mechanics) to L4 (science)? do L4 pass rates move on held-out families? Mechanics-only gains are reported as such
- **Claim bar:** the delta validates the environment (7B+training > 7B baseline, approaching a larger untrained model on held-out tasks) — not a frontier agent. **Paper target: SFT delta + one GRPO headline run; SFT-only is the fallback, not the plan.**

## De-risk gates (fatal-flaw checks, in order — from SPEC §7)

| # | Gate | When | Kills the plan if |
|---|---|---|---|
| 1 | conda McStas + example runs on this Mac | M0 | osx-arm64 binaries broken AND Docker unusable |
| 2 | Agent builds/runs instrument from one prompt | M1 | validation-at-call-time can't be made reliable |
| 3 | OpenRouter backend spike, 2 non-Claude models | **de-risk gate 3 passed** 2026-07-24 | tool-calling fidelity too poor → single-model paper only |
| 4 | 3 pilot tasks incl. memorization probe | **de-risk gate 4 passed** 2026-07-24 (mechanics; agent episodes next) | grading rubric can't separate memorization from capability |

## Standing decisions (defaults from SPEC §8 — change only with a dated note)

- Laptop-scale compute for env/eval; **7×A100-40G** (verified 2026-07-26) reserved for the M8 RL track; SLURM path deferred (async API already accommodates it)
- McStas only; McXtrace kept in the design, out of scope for MVP
- Union components / NCrystal deferred past MVP (introspection exposes them anyway)
- Single agent + skill + validating tools; no multi-agent split unless evals show persistent unforced physics errors
- Environment-first framing (2026-07-24): benchmark = held-out slice; T1 paper-reproduction stays curated and uncuttable; RL track (M8) in scope as the paper-strengthening trainability result
- **This work is McStas only** (user decision 2026-07-24): no autoMartiniAgent suite, no cross-project framework paper — one substrate, done deeply
- **Naming (2026-07-26): NeutronGym** = the environment (paper headline artifact + released package); **McStasBench** = its held-out benchmark slice; repo name unchanged
- **Venue (2026-07-26): ICLR 2027 committed** (full paper 2026-09-24 AoE); NeurIPS 2027 D&B is the fallback, not a co-target
- **RL in the paper (2026-07-26, supersedes same-day include-if-signal policy): bench + RL are both ICLR paper content.** SFT Aug 11 – Sep 1 → GRPO go/no-go Sep 1 → GRPO Sep 1 – 15 → RL numbers frozen ~Sep 17. Degradation ladder if signal fails (reported honestly): GRPO → SFT-only → env+eval-only
