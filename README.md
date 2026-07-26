# NeutronGym

**An executable, physically-verifiable RL environment for neutron instrument
design with LLM agents.** Agents design real neutron instruments — SANS
machines, spectrometers, diffractometers — against [McStas](https://www.mcstas.org/)
Monte-Carlo ray-tracing as the simulation backend. Every reward is computed
from physics observables (flux, beam statistics, resolution); there is no LLM
judge anywhere in the loop.

**McStasBench** is NeutronGym's held-out benchmark slice: curated
paper↔instrument reproduction tasks (T1), calibrated improve-to-target-spec
tasks (T2), and open-design briefs (T3), with contamination controls
(memorization probes, held-out 2024–26 instruments) and a red-teamed reward.

## What's inside

| Piece | Where | What |
|---|---|---|
| `mcstas-mcp` server | `src/mcstas_mcp/` | 22 MCP tools: component introspection, validated instrument construction, async simulation, scans, classical optimization |
| Design skill | `skills/mcstas-instrument-design/` | the physics judgment layer: workflow, units, figures of merit, archetypes, verification checklist |
| Benchmark + env harness | `benchmark/` | tasks, observable-based grader, memorization probe, episode runner, classical-baseline calibration |
| Dashboards | `progress.html`, `guide.html`, `benchmark/tasks.html` | generated, never hand-edited: progress, how-the-agent-works, task catalog |

## Quick start (this machine's setup)

```bash
conda create -n mcstas -c conda-forge mcstas-core mcstas-data ncrystal python=3.11
conda run -n mcstas pip install -e ".[dev]" && pip install ply
conda run -n mcstas python -m pytest tests -q          # ~100 tests
conda run -n mcstas python scripts/m1_walkthrough.py   # server demo, end to end
conda run -n mcstas python benchmark/run_episode.py benchmark/tasks/T1/T1_PSI_DMC.json
```

The last command runs a real headless agent episode: an LLM rebuilds a
validated model of PSI's DMC powder diffractometer from a natural-language
spec and is graded against the hidden reference on simulation observables.

## Status

Infrastructure and benchmark harness complete and self-validated; environment
build (executor, procedural generation, reward-ladder API) in progress.
See `progress.html` for the live plan and `PLAN.md` for the source of truth.
Target: ICLR 2027.
