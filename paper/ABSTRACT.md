# Abstract — DRAFT for review (ICLR 2027 abstract deadline 2026-09-18)

*Draft 2, 2026-09-15 (numbers verified against the record). Not submitted. Wording follows `SCOPE.md` (updated
2026-09-15): no "RL environment" in the headline; "first" kept narrow and
dated; the post-hoc M8 number is never presented as a trainability result.
Every number cites `benchmark/RESULTS.md` or `benchmark/evidence/m8/`.*

---

**NeutronGym: An Executable, Physically Verifiable Environment and Benchmark
for LLM Agents Designing Neutron Instruments**

Scientific-instrument design is a natural test of whether language-model
agents can do physics rather than recall it, but it needs grading that cannot
be argued with. We introduce NeutronGym, to our knowledge the first executable
environment for neutron instrument design: agents build instruments through
validating tools, a Monte Carlo ray tracer (McStas) simulates them, and a
level-resolved ladder grades syntax, runtime, structure and science with no
LLM judge. Procedural families supply unlimited instances with held-out
parameter regimes, and its benchmark slice, McStasBench, adds reproduction
tasks from published instruments, including two never made public.

Across 253 scored episodes from eight models, frontier to open-weights, no
agent leaked a reference and none solved an improvement task that classical
search solves. Failures split almost evenly between never finishing an
instrument and building the wrong physics.

We also report what the environment revealed about itself. Rejection-sampling
fine-tuning of Qwen3-8B on its own successes, pre-registered, made it worse
(40.3% → 31.3% on 300 paired held-out instances, p = 0.0013): per-turn
imitation cloned exploration moves instead of the decisive one. A post-hoc
variant matched Qwen3-32B, but a no-model constant action passes the same
instances at the same rate. The same constant-policy probe exposed a reward
hole in a second family. We release the environment with that probe as a
mandatory acceptance gate, and argue every executable science environment
needs one before its pass rates are read as capability.

---

## Numbers to verify against the record before submission

| Claim in the abstract | Source | Value |
|---|---|---|
| scored episodes | `benchmark/RESULTS.md` totals | 225 matrix + 28 held-out = 253 — VERIFIED |
| models | `benchmark/results/m6_results.json` matrix rows | 8 (7 on held-out; incl. a one-episode gpt-5.2-pro probe) — VERIFIED |
| zero reference leaks | RESULTS.md totals | 0 leak-invalid — VERIFIED |
| improvement tier unsolved | RESULTS.md Table 5 note | no agent beat either T2 target — VERIFIED |
| failures split evenly | m6_results.json failure_kinds (matrix) | physics 96 vs never-finished 89 (format 38 + incomplete 51) — VERIFIED; the earlier "concentrate at construction" wording was FALSE and removed |
| 40.3% → 31.3%, p | `evidence/m8/eval_guide_1x_n300.json` | McNemar p=0.0013 — VERIFIED |
| post-hoc ≈ 32B | `evidence/m8/ablation_readout.json` | 52.3% vs 54.7%, p=0.49 — VERIFIED |
| constant matches | `evidence/m8/constant_probe_guide_1x_n300.json` | 50.3–52.3%, 98% agreement — VERIFIED |
| probe is an acceptance gate | `evidence/env_acceptance/acceptance.json` | guide FAILS (best constant 36% of 25 held-out), SANS passes (12%) |
| second family reward hole | `note/sans-direct-beam-exploit-2026-09-13.md` | SANS direct beam, fixed |

## Open questions for the author

- The failure-split sentence replaced a false claim; it can be cut to save
  words without losing anything the paper relies on.
- The held-out loop-vs-one-shot result (10/14 vs 2/14, p=0.006) is left out on
  purpose: held-out instruments are significantly easier (p=0.0001). Confirm.
- Title: keep "Environment and Benchmark", or lead with the methods finding?
