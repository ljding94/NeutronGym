# McStas MCP Server + Agent Skill: Feasibility Analysis, Implementation Plan, and SPEC

**Date:** 2026-07-08 · **Verdict: Highly feasible.** No existing McStas MCP server was found (as of July 2026), so this would be novel — both as a tool and as a publishable AI-for-science result.

> **Scope update (2026-07-09):** see `scope-decision-2026-07-09.md` — McStasBench (the benchmark) is the paper and headline contribution; the MCP server + skill below are the McStasAgent baseline shipped inside it. That note supersedes the framing of §7 where they differ.

---

## 1. Why McStas is unusually well-suited to agent control

McStas is a better fit for LLM agents than most scientific simulation codes, for structural reasons:

1. **Text-based DSL.** Instruments are `.instr` files in a C-like meta-language — exactly what LLMs are good at reading and writing. No GUI required; `mcstas`/`mcrun` are fully headless CLI tools.
2. **Mature Python API already exists.** [McStasScript](https://github.com/PaNOSC-ViNYL/McStasScript) (ESS DMSC, pip-installable) provides programmatic instrument construction, component introspection, simulation execution, and data objects (numpy arrays). An MCP server is largely a thin wrapper over it — you are not writing a McStas interface from scratch.
3. **Self-describing component library.** 280 components (counted in the current McCode repo) (sources, guides, choppers, monochromators, samples, detectors) carry machine-readable parameter definitions with units, defaults, and docstrings. This enables introspection tools that ground the agent and suppress hallucinated parameters.
4. **Fast, tunable feedback loop.** `ncount` scales runtime from seconds (1e5 rays, debugging) to hours (1e9, production). An agent can iterate cheaply and only pay for precision at the end. Monitor output is plain-text arrays — directly analyzable, or renderable to PNG for multimodal inspection.
5. **Optimization is a known workflow.** `mcrun` has built-in parameter scans and a nonlinear optimizer; [guide_bot](https://github.com/mads-bertelsen/guide_bot) proved a decade ago that automated McStas-driven guide design works. The agent replaces the rigid input-file front end with natural-language goal specification.

### Validated hands-on (this session)

I installed McStasScript from PyPI and exercised the full non-execution pipeline against the McCode component library (sparse-cloned from GitHub):

- `pip install mcstasscript` — works cleanly.
- Component discovery: 115 components auto-parsed from 4 category dirs (sources/optics/samples/monitors).
- Introspection: `component_help("Guide_gravity")` returns every parameter with units, defaults, required-flags, and descriptions.
- Instrument assembly: source → guide → PSD monitor built via API; valid `.instr` file generated.

The only step not testable in this sandbox was binary execution (no conda/root here). That step is routine on a real machine: `conda install -c conda-forge mcstas-core mcstas-data` (Linux x64/aarch64, macOS, Windows; current v3.6.x), or the official Docker image. **This is the single environmental prerequisite and it is low-risk.**

### Prior art / novelty check

- No McStas MCP server exists (searched July 2026).
- No published LLM-agent-designs-neutron-instrument paper found. Closest analogs: [MooseAgent](https://arxiv.org/pdf/2504.08621) (LLM multi-agent for MOOSE FEM simulation), LLM agents for CFD, and ML-on-McStas-virtual-experiment work ([arXiv 2501.06054](https://arxiv.org/pdf/2501.06054)). guide_bot is the non-LLM predecessor for automated design.
- The 2026 best-practice pattern is exactly what you'd build: one MCP server per external system + a thin skill teaching the agent how to use it well.

---

## 2. Risks and mitigations

| Risk | Severity | Mitigation |
|---|---|---|
| Agent hallucinates component names/params | High | Introspection tools (`list_components`, `describe_component`) + server-side validation before compile; McStasScript already errors on unknown components/params |
| Long runtimes block the agent loop | Medium | Async job model (`start_simulation` → `job_status` → `get_results`); default low `ncount`; MPI flag for production runs |
| Compile errors from raw `.instr` edits | Medium | Prefer structured build tools; surface full `mcstas`/`cc` diagnostics back to agent; keep raw-file escape hatch |
| Physically valid but nonsensical designs (wrong units, unphased choppers, m=7 everywhere) | High | This is the *skill's* job: units conventions, FOM definitions, design recipes, sanity checklist |
| Weak statistics misread as signal | Medium | Return intensity **and** error + event count per monitor; skill rule: never compare designs below N events |
| McStas 2.x vs 3.x syntax differences | Low | Target 3.x only; McStasScript handles version quirks |
| State loss between agent turns | Low | Server persists instrument registry + project dir on disk |

---

## 3. Recommended architecture

```
┌────────────┐   MCP (stdio)   ┌──────────────────────────┐
│ Claude /    │◄──────────────►│ mcstas-mcp (Python,       │
│ agent + skill│                │ FastMCP)                  │
└────────────┘                 │  ├─ McStasScript wrapper  │
                               │  ├─ instrument registry   │
                               │  ├─ async job manager     │
                               │  └─ plot renderer (PNG)   │
                               └─────────┬────────────────┘
                                         │ subprocess
                               ┌─────────▼────────────────┐
                               │ mcstas / mcrun / cc (MPI) │
                               │ conda-forge or Docker     │
                               └──────────────────────────┘
```

**Key design decision — structured tools vs. raw `.instr` generation.** Two viable modes:

- **(A) Raw mode:** agent writes the whole `.instr` file as text; server compiles/runs. Leverages LLM code fluency, minimal tool surface, but errors surface only at compile time.
- **(B) Structured mode:** agent calls `add_component`/`set_parameters`; server validates each step against component metadata immediately.

**Recommendation: B as primary, A as escape hatch** (`load_instr_file` / `export_instr_file`). Immediate validation catches hallucination at the cheapest point, and the export tool preserves interoperability with human workflows and the existing McStas example corpus (297 shipped example instruments, counted in the current McCode repo — valuable few-shot material).

---

## 4. SPEC — `mcstas-mcp` server

Python ≥3.10, `fastmcp`, `mcstasscript`, `numpy`, `matplotlib`. Requires a McStas 3.x installation (auto-detected via `mcstas-config` / configurable). Transport: stdio (local). All tools return structured JSON; errors return McStas/compiler diagnostics verbatim.

### 4.1 Discovery tools

```
list_components(category?: str, search?: str) -> [{name, category, one_line_doc}]
describe_component(name: str) -> {name, category, description,
    parameters: [{name, type, unit, default, required, doc}], links}
list_examples(search?: str) -> [{name, description, path}]   # shipped .instr corpus
get_example(name: str) -> {source: str}                       # few-shot material
```

### 4.2 Instrument construction tools

```
create_instrument(name: str, description?: str) -> {instrument_id}
add_parameter(instrument_id, name, type="double", default?, unit?, comment?)
add_component(instrument_id, name, component: str,
    at: [x,y,z], relative?: str, rotated?: [rx,ry,rz],
    parameters?: {param: value}, after?: str) -> {ok, validation_warnings}
set_parameters(instrument_id, component_name, parameters: {..}) -> {ok}
remove_component(instrument_id, component_name)
get_instrument(instrument_id) -> {components: [...], parameters: [...],
    instr_source: str, geometry_summary: [{name, position_abs, rotation_abs}]}
load_instr_file(path|source) -> {instrument_id, warnings}     # escape hatch in
export_instr_file(instrument_id, path) -> {path}              # escape hatch out
validate_instrument(instrument_id) -> {ok, compile_output}    # mcstas translate + cc, no run
```

Every mutating call re-validates against component metadata: unknown component → error listing nearest matches; missing required params → error naming them; wrong RELATIVE reference → error. **Fail at tool-call time, not compile time.**

### 4.3 Execution tools (async)

```
run_simulation(instrument_id, ncount=1e6, parameters?: {..}, seed?,
    mpi?: int, gravity?: bool) -> {job_id}
job_status(job_id) -> {state: queued|running|done|failed, elapsed, log_tail}
get_results(job_id) -> [{monitor, dims, intensity_sum, err_sum, events,
    peak:{value,pos}, center_of_mass, fwhm?, metadata}]      # stats, not raw dumps
get_monitor_data(job_id, monitor, format="array"|"png") -> array | image
scan_parameter(instrument_id, parameter, values: [..] | {min,max,n},
    ncount, fom?: {monitor, metric}) -> {job_id}              # wraps mcrun -N
optimize(instrument_id, free_parameters: [{name,min,max}],
    fom: {monitor, metric: intensity|peak|fwhm|custom_expr},
    method="nelder-mead", ncount, max_iter) -> {job_id}       # scipy loop over runs
```

Design notes: `get_results` returns *summary statistics* — raw 100×100 arrays waste context; the agent asks for a PNG (multimodal inspection) or specific slices when needed. `optimize` exists because pure agent-loop optimization burns tokens on what scipy does for free; the agent chooses parametrization and FOM, the server grinds.

### 4.4 Project/state

Instruments and job outputs persist in a project directory (`~/.mcstas-mcp/projects/<name>/`), so a design session survives restarts and results remain diffable/reproducible (seed, ncount, and git-style history of `.instr` versions recorded per run).

---

## 5. SPEC — `mcstas-instrument-design` skill

The MCP gives capability; the skill gives judgment. Structure:

```
mcstas-instrument-design/
├── SKILL.md                    # workflow + guardrails (< 200 lines)
├── references/
│   ├── units-and-conventions.md   # meters, Å, meV, coordinate system (z=beam),
│   │                              # AT/ROTATED/RELATIVE semantics, filename quoting
│   ├── figures-of-merit.md        # flux@sample, brilliance transfer, divergence,
│   │                              # ΔE/E, Δλ/λ, Q-resolution, signal/background
│   ├── instrument-archetypes.md   # canonical layouts + typical parameter ranges:
│   │                              # SANS, powder diffractometer, reflectometer,
│   │                              # TAS, direct/indirect ToF spectrometer, imaging
│   ├── component-guide.md         # which component to reach for and when;
│   │                              # common pitfalls (m-value economics, chopper
│   │                              # phasing math, gravity, focusing conventions)
│   └── verification-checklist.md
└── scripts/
    └── resolution_calcs.py        # chopper phase/frame calcs, guide m estimator
```

**SKILL.md core workflow it must encode:**

1. Clarify design goal as quantitative FOM + constraints (source type, total length, budgetary proxies like m-coating and guide length) *before* building.
2. Build moderator → optics → sample → detector, adding a diagnostic monitor after each stage.
3. Iterate at `ncount=1e5–1e6`; production-validate at `≥1e8`. Never compare designs whose monitors hold <1000 events.
4. Check the checklist every iteration: beam actually reaches sample (transmission chain), intensities in n/s not counts, divergence within spec, no negative/NaN monitors, resolution from time/angular contributions adds in quadrature.
5. Use `scan_parameter`/`optimize` for continuous knobs; reserve agent reasoning for topology changes (add a bender? curved vs straight guide? chopper count?).
6. Ground everything: `describe_component` before first use of any component; start from a shipped example (`get_example`) when an archetype matches.

---

## 6. Implementation plan

**Phase 0 — Environment (½ day).** `conda create -n mcstas -c conda-forge mcstas-core mcstas-data python=3.11; pip install mcstasscript fastmcp`. Smoke-test: run a shipped SANS example via McStasScript. (On your Mac, or Docker `mccode/mcstas` for reproducibility.)

**Phase 1 — MVP server (2–3 days).** Eight tools: `list_components`, `describe_component`, `create_instrument`, `add_component`, `set_parameters`, `run_simulation` (synchronous, capped ncount), `get_results`, `get_monitor_data(png)`. Register with Claude Code/Cowork; manually drive the source→guide→PSD test. *Milestone: agent builds and runs an instrument end-to-end from one prompt.*

**Phase 2 — Robustness (3–5 days).** Async jobs, `validate_instrument` with full diagnostics passthrough, `load/export_instr_file`, example-corpus tools, project persistence, error-message quality pass (every failure must tell the agent what to do next).

**Phase 3 — Skill (2 days, parallel with 2).** Write SKILL.md + references above; refine by watching failure transcripts — every recurring agent mistake becomes a line in the skill.

**Phase 4 — Optimization layer (1 week).** `scan_parameter`, `optimize`, FWHM/CoM analysis in `get_results`. Reproduce a guide_bot-style task (maximize brilliance transfer into 2×2 cm², ±0.5° over given λ-band) as the acceptance test.

**Phase 5 — Evaluation & write-up (ongoing).** Benchmark suite with three tiers:
- **Reproduce:** rebuild N shipped example instruments from natural-language descriptions; metric = compile rate + monitor agreement vs reference.
- **Optimize:** fixed-topology parameter tuning vs known optima (guide_bot comparisons); metric = FOM ratio, wall-clock, token cost.
- **Design:** open-ended ("design a compact SANS for a CANS source, 5 m envelope, ≤10% Δλ/λ"); metric = expert rubric + FOM.

This tiered eval is itself the skeleton of a publishable paper (direct analog: MooseAgent's evaluation design).

**Total: ~3 weeks part-time to a system worth evaluating seriously.**

---

## 7. Research program (converged 2026-07-08)

**Paper skeleton:** system (MCP + skill) + benchmark + case study + ablations.

### Study A — McStasBench: paper → instrument reproduction (core contribution)
Curate ~20–30 pairs of (paper/technical report, reference `.instr`, reference monitor outputs). Ground truth base: 80 of 297 shipped McStas examples model real facility instruments (ILL/PSI/SNS/ISIS/ESS/FRM-II/HZB/NIST); 19 cite literature in-file. Grade on **simulation observables** (flux spectrum at sample, beam profile, resolution function) with tolerance tiers — no LLM judge needed. Report pipeline-decomposed metrics: spec-extraction accuracy vs build accuracy.

**Contamination controls (mandatory):** (a) "seen" vs "held-out" tiers — held-out = recent (2024–26) instruments with no public `.instr`; (b) memorization probe — model must fail to emit the instrument file *without tools* for a pair to count as unseen; (c) perturbed variants where memorization hurts.

### Study B — Autonomous design improvement (case study)
Agent receives a published design + goal; measures FOM improvement under fixed turn/compute budget vs (i) published design, (ii) classical optimizer (guide_bot/scipy) on same parametrization, (iii) random search. Framing: **classical tools tune parameters; can the agent choose topology and parametrization?** Guardrails: server-enforced physical/cost constraints (m-value caps, coating cost model), frozen FOM/monitor definitions, improvements accepted only if > k·σ re-verified at high ncount with fresh seeds, expert adjudication for claims against published designs. Precondition per instrument: reproduce the *published* FOM first; drop instruments where the baseline can't be reproduced.

### Study C — Ablations
Structured tools vs raw `.instr` writing; ± component introspection; ± skill; ± vision (monitor PNGs). Plus cost/scaling curves: FOM and success rate vs token budget and ncount.

**Follow-up papers (out of scope):** instrument-fault diagnosis from monitor outputs; closed-loop virtual experiments (design → measure known S(Q) → recoverability).

### Evaluation harness — one scaffold, three model tiers
Headless Claude Code (`claude -p`, JSON transcripts) as the universal scaffold; MCP server in `.mcp.json`, skill installed. Model tiers:

| Tier | Route | Cost |
|---|---|---|
| Claude (frontier) | subscription auth | $0 (weekly caps → spread final pass over days) |
| Mid + small models | `ANTHROPIC_BASE_URL=https://openrouter.ai/api` (native Claude Code support) | ~$50–200 total |
| Open-weights (optional) | vLLM on 7×A100-40G behind LiteLLM Anthropic-proxy | $0 |

Identical scaffold across tiers kills the scaffold confound. Log per-episode tokens/tool-calls/wall-clock from transcripts; convert to list-price dollar-equivalents for cost curves. Protocol: pass@1, fixed seeds, hard 50-turn cap, 5-task dev split for all debugging, held-out set touched once. Distinguish *format failures* (tool-calling mechanics) from *physics failures* in the error taxonomy. Pin model IDs + Claude Code version; run each model's pass in a tight window.

**Budget estimate:** ~1–3M billed tokens/episode with prompt caching and summary-stat tool returns → full program ≈ $100–300 on OpenRouter + $0 Claude + $0 local. Apply for Anthropic/OpenAI academic API credit programs in parallel.

### De-risk kill list (weeks 1–2, in order)
1. conda-forge McStas install + shipped example runs via McStasScript (hours).
2. MVP MCP server; Claude Code builds/runs source→guide→PSD from one prompt (days).
3. OpenRouter-backend spike: same task, 2 non-Claude models — verifies tool-calling fidelity through the alternate backend (1 day).
4. Pilot-grade 3 reproduction tasks including one memorization probe and one underspecified paper — validates the grading rubric before curating 30 (week 2).

Anything fatal to the plan surfaces within two weeks.

**Honest timeline:** infra ~3 weeks (Phases 0–4) + curation 2–3 weeks + pilot/rubric 1–2 weeks + full runs & analysis 2 weeks + writing 2 weeks ≈ **2.5–3 months part-time**.

## 8. Open questions to decide early

1. **Compute target:** laptop-scale (fine for design iteration at 1e6–1e7 rays) vs HPC dispatch (add a SLURM submit path to the job manager later — the async API already accommodates it).
2. **McXtrace too?** McStasScript supports both; ~90% of the server generalizes to X-ray instruments for free. Worth keeping in scope for the design, out of scope for the MVP.
3. **Union components / NCrystal samples:** powerful but complex; defer past MVP, but the introspection layer already exposes them.
4. **Multi-agent split (builder/runner/critic à la MooseAgent):** don't start there — one agent + good skill + validating tools first; add a critic sub-agent only if eval shows unforced physics errors persisting.

---

## Sources

- [McStas homepage](https://www.mcstas.org/) · [About](http://www.mcstas.org/about/)
- [McStasScript (GitHub, PaNOSC-ViNYL)](https://github.com/PaNOSC-ViNYL/McStasScript) · [Docs](https://mads-bertelsen.github.io/) · [conda-forge](https://anaconda.org/conda-forge/mcstasscript)
- [mcstas on conda-forge](https://anaconda.org/conda-forge/mcstas) · [mcstas-suite feedstock](https://github.com/conda-forge/mcstas-suite-feedstock)
- [mcrun manpage (scans + optimizer)](https://manpages.ubuntu.com/manpages/plucky/man1/mcrun.1.html)
- [guide_bot (GitHub)](https://github.com/mads-bertelsen/guide_bot) · [guide-bot on PyPI (Python rewrite)](https://pypi.org/project/guide-bot/)
- [SANS in McStas: high-throughput virtual experiments (arXiv 2501.06054)](https://arxiv.org/pdf/2501.06054)
- [MooseAgent: LLM multi-agent for MOOSE simulation (arXiv 2504.08621)](https://arxiv.org/pdf/2504.08621)
- [McStasToX (data export)](https://github.com/mccode-dev/McStasToX)
- [Claude skills + MCP integration pattern](https://claude.com/blog/extending-claude-capabilities-with-skills-mcp-servers)
