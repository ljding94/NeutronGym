# Prior-art re-check (2026-09-20)

Supersedes `note/prior-art-recheck-2026-09-13.md` for anything it contradicts.
Run because the area publishes monthly and the previous check predates every
RL result we now report.

## What is unchanged

No LLM-agent work on **McStas or McXtrace** instrument design has appeared.
The neutron-simulation literature that surfaces is the usual instrument and
optimisation work (SANS throughput optimisation, GP-SANS ray-tracing, cold
triple-axis beamline design, McStas/Mantid integration) with no agent or
language-model component. The narrow claim still holds as worded in SCOPE.md:
*"the first executable environment for neutron instrument design"* — dated,
and not the unqualified "first scientific environment".

## What changed, and what it costs us

**1. `Reinforcement Learning with Verifiable Physics` (RLVP), arXiv 2607.10474.**
Post-trains LLMs for PDE-solver code generation with GRPO, using *hard
execution-validity gates plus continuous function-space rewards* against
hidden references. That is structurally the same recipe as our ladder: gate
validity, then score continuously. **Implication:** "GRPO on a
physics-verifiable reward" is no longer novel in itself and should not be
claimed as such. Our separable contributions are (a) the environment and
benchmark in a domain with a real Monte-Carlo simulator in the loop, (b) the
multi-turn *design* loop — the agent proposes, measures and revises, rather
than emitting one artefact to be verified, and (c) the shortcut-probe
methodology and what it exposed.

**2. `GenEnv: Difficulty-Aligned Co-Evolution Between LLM Agents and
Environment Simulators`, arXiv 2512.19682.** Difficulty alignment between
agent and environment. Related to our tolerance knob and the
measure-then-choose procedure used to set `sans_match` at ±1.5%. Cite
alongside RLVE rather than presenting difficulty calibration as new.

**3. LLM-simulated environments are a growing cluster** — EnvSimBench
(arXiv 2605.07247) formalises simulation *fidelity* as a measurable capability
and documents hallucination and silent state drift in LLM-simulated feedback;
EnvScaler and Qwen-AgentWorld synthesise tool environments with LLMs.
**Implication:** a useful contrast to draw explicitly. Our feedback comes from
a Monte-Carlo ray tracer, so reward cannot hallucinate; the failure modes we
had to engineer against were *degenerate tasks and mis-graded targets*, not
fabricated observations. That is a sharper framing than "we built an
environment".

**4. AP-GRPO** (absolute-preserving GRPO for physics-grounded sparse reward)
targets the same sparse-reward failure our ablation measured (0.33 of 8 groups
per step carried signal with pass/fail-only reward). Worth one sentence as
independent corroboration that shaping matters in this regime.

## Suggested wording changes

- Drop any phrasing implying novelty for "verifiable physics reward + RL";
  claim the environment, the benchmark, the design loop and the probe suite.
- When contrasting with LLM-simulated environments, say what it buys:
  non-hallucinable reward, at the cost of needing a real simulator and the
  task-degeneracy work this project spent most of its effort on.

Sources: [RLVP](https://arxiv.org/html/2607.10474v1) ·
[GenEnv](https://arxiv.org/pdf/2512.19682) ·
[EnvSimBench](https://arxiv.org/abs/2605.07247) ·
[SANS in McStas](https://arxiv.org/pdf/2501.06054) ·
[GP-SANS ray-tracing](https://arxiv.org/pdf/2404.08890)
