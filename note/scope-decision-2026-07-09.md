# Scope decision: McStasBench is the paper, McStasAgent is the baseline inside it

**Date:** 2026-07-09 · **Status: decided.** Supersedes any ambiguity in the framing of `mcstas-mcp-feasibility-and-spec.md` §7; the SPEC itself (§4–6) is unchanged.

## The question

Is the contribution (a) **McStasAgent** — an MCP server + agent skill for McStas instrument design — or (b) **McStasBench** — a benchmark evaluating LLM agents on McStas tasks?

## Landscape (checked 2026-07-09)

No McStas+LLM paper exists yet, but "agent for simulator X" is now a well-trodden pattern (El Agente for quantum chemistry, OpenFOAM/CFD agents, SIGA), while simulation-agent benchmarks are an active, well-received genre (MDGYM for molecular dynamics, SciReplicate-Bench, ScienceAgentBench).

## Evaluation

### McStasAgent alone (MCP + skill)

- **Pros:** first-mover for neutron scattering; fast to build; immediately useful to facility user programs (ORNL/SNS, ESS, ILL); low risk.
- **Cons:** at an AI venue it reads as engineering, not research — the pattern is commoditized and reviewers will ask "what did we learn?" Credible claims of "it works" require systematic evaluation anyway, so half a benchmark gets built regardless. Realistic venues: J. Appl. Cryst., JOSS, NOBUGS — fine, but not hardcore AI.

### McStasBench alone

- **Pros:** benchmarks are the recognized contribution format (NeurIPS Datasets & Benchmarks, ICLR); durable and highly citable; MDGYM proves the template for a simulator domain and no neutron equivalent exists. McStas is unusually well-suited: quantitative ground truth (detector spectra, flux, resolution functions) and a natural difficulty ladder (single component → full instrument file → geometry via Union → parameter optimization against target spectra).
- **Cons:** 3–5× the work (task curation, ground-truth validation, rubrics, multi-model runs); contamination risk since McStas examples are on GitHub and in training data (needs perturbed/novel tasks); ongoing maintenance expectation; neutron community is smaller than MD/chemistry so uptake may be modest.

## Decision

**The binary is false — do both in one paper, framed as McStasBench.** Benchmark papers are expected to ship a reference agent as the baseline (MDGYM does exactly this), and the agent needs the benchmark to prove anything.

- **McStasBench** = the paper and headline contribution (benchmark venue: NeurIPS D&B / ICLR class).
- **McStasAgent** = the baseline system + released tooling *inside* the paper: the `mcstas-mcp` server and `mcstas-instrument-design` skill from the SPEC.
- The agent's failure modes on the benchmark become the analysis section — the part reviewers actually value.

## Implications for the plan

1. Repo/project name stays **McStasBench**; the server + skill artifacts keep their own names (`mcstas-mcp`, `mcstas-instrument-design`) and can be released under a McStasAgent umbrella name.
2. Implementation ordering in the SPEC (§6) is unchanged — the server and skill are still built first, because the benchmark harness runs on them.
3. Benchmark quality bars from §7 are now load-bearing for the headline contribution, not optional rigor: contamination controls, difficulty tiers, pipeline-decomposed metrics, and the multi-model/one-scaffold protocol.
4. Writing framing: lead with the benchmark and the analysis of agent failure modes; the system description is a means, not the claim.

## Sources

From the scoping discussion (look up before citing in the paper): MDGYM · El Agente · SIGA · [SciReplicate-Bench (arXiv 2504.00255)](https://arxiv.org/abs/2504.00255) · [ScienceAgentBench (arXiv 2410.05080)](https://arxiv.org/abs/2410.05080)
