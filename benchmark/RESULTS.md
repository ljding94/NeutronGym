# NeutronGym — results of record

*Generated 2026-09-13 19:01 by `benchmark/harness/results_report.py` from the committed evidence in `benchmark/evidence/`. **These are the numbers the manuscript cites.** Regenerate after any re-run; diff the JSON to see what moved.*

**Validity rule:** INFRA (endpoint/provider/harness failures) and LEAK episodes are excluded from every rate and reported separately. This is not cosmetic — on 2026-09-10 an unnoticed dead SSH tunnel put 77 infra failures into the tables as capability zeros, which invalidated an entire model row until caught and re-run.

**Totals:** 225 valid matrix episodes · 28 valid held-out episodes · 0 infra-excluded · 0 leak-invalid (zero leaks across the whole campaign) · $122.16 OpenRouter spend.

### Table 1 — Main matrix (seen-tier scored set)

`main` = reference loop with MCP + skill · `oneshot` = plain-LLM, no tools · `noskill` = loop without the design skill · `claude_code` = production-harness comparison arm. Dev-split tasks are EXCLUDED so every arm is scored on the same 17 tasks. Rows with fewer than 5 valid episodes are omitted as uninformative — notably `openai/gpt-5.2-pro`, which ran a single episode (a PASS) before being cut on cost at $29.21/episode; n=1 is a footnote, not a 100% pass rate.

| model | arm | valid n | passes | pass rate | L4 | L3 | L2 | L1 | L0 | infra excl. |
|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| `anthropic_claude_haiku_4_5` | main | 16 | 4 | 0.25 | 4 | 2 | 2 | 4 | 0 | 0 |
| `anthropic_claude_haiku_4_5` | oneshot | 16 | 4 | 0.25 | 0 | 0 | 0 | 12 | 0 | 0 |
| `anthropic_claude_sonnet_5` | main | 16 | 5 | 0.31 | 1 | 0 | 0 | 9 | 1 | 0 |
| `anthropic_claude_sonnet_5` | oneshot | 16 | 7 | 0.44 | 0 | 1 | 2 | 6 | 0 | 0 |
| `google_gemini_3_5_flash_lite` | main | 16 | 2 | 0.12 | 1 | 0 | 1 | 5 | 7 | 0 |
| `google_gemini_3_5_flash_lite` | oneshot | 16 | 1 | 0.06 | 0 | 0 | 0 | 15 | 0 | 0 |
| `google_gemini_3_6_flash` | main | 16 | 4 | 0.25 | 0 | 1 | 0 | 1 | 10 | 0 |
| `google_gemini_3_6_flash` | oneshot | 16 | 6 | 0.38 | 1 | 1 | 2 | 6 | 0 | 0 |
| `meta_llama_llama_4_maverick` | main | 16 | 0 | 0.00 | 0 | 2 | 0 | 1 | 13 | 0 |
| `meta_llama_llama_4_maverick` | oneshot | 16 | 2 | 0.12 | 0 | 0 | 0 | 14 | 0 | 0 |
| `qwen3_32b` | main | 16 | 3 | 0.19 | 0 | 0 | 0 | 0 | 13 | 0 |
| `qwen3_32b` | oneshot | 16 | 0 | 0.00 | 0 | 0 | 0 | 0 | 16 | 0 |
| `qwen3_8b` | main | 16 | 1 | 0.06 | 0 | 1 | 1 | 0 | 13 | 0 |
| `qwen3_8b` | oneshot | 16 | 0 | 0.00 | 0 | 0 | 0 | 0 | 16 | 0 |


### Table 2 — Held-out final pass (once-only touch)

Instruments with no public `.instr` (BOYA, VENUS), authored for this benchmark and touched exactly once. The tool loop outscores one-shot here (paired, 7 models: 10/14 vs 2/14, Fisher p=0.006) — **but see the statistical note below: these instruments are significantly EASIER than the seen-tier set (loop arm 10/14 vs 20/113, p=0.0001), which is an unexcluded alternative explanation for the reversal.**

| model | arm | valid n | passes | pass rate | L4 | L3 | L2 | L1 | L0 | infra excl. |
|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| `anthropic_claude_haiku_4_5` | main | 2 | 2 | 1.00 | 0 | 0 | 0 | 0 | 0 | 0 |
| `anthropic_claude_haiku_4_5` | oneshot | 2 | 0 | 0.00 | 0 | 0 | 0 | 2 | 0 | 0 |
| `anthropic_claude_sonnet_5` | main | 2 | 2 | 1.00 | 0 | 0 | 0 | 0 | 0 | 0 |
| `anthropic_claude_sonnet_5` | oneshot | 2 | 1 | 0.50 | 1 | 0 | 0 | 0 | 0 | 0 |
| `google_gemini_3_5_flash_lite` | main | 2 | 1 | 0.50 | 0 | 0 | 0 | 0 | 1 | 0 |
| `google_gemini_3_5_flash_lite` | oneshot | 2 | 0 | 0.00 | 0 | 0 | 0 | 2 | 0 | 0 |
| `google_gemini_3_6_flash` | main | 2 | 2 | 1.00 | 0 | 0 | 0 | 0 | 0 | 0 |
| `google_gemini_3_6_flash` | oneshot | 2 | 1 | 0.50 | 1 | 0 | 0 | 0 | 0 | 0 |
| `meta_llama_llama_4_maverick` | main | 2 | 0 | 0.00 | 0 | 0 | 0 | 0 | 2 | 0 |
| `meta_llama_llama_4_maverick` | oneshot | 2 | 0 | 0.00 | 0 | 0 | 0 | 2 | 0 | 0 |
| `qwen3_32b` | main | 2 | 2 | 1.00 | 0 | 0 | 0 | 0 | 0 | 0 |
| `qwen3_32b` | oneshot | 2 | 0 | 0.00 | 0 | 0 | 0 | 0 | 2 | 0 |
| `qwen3_8b` | main | 2 | 1 | 0.50 | 0 | 0 | 0 | 0 | 1 | 0 |
| `qwen3_8b` | oneshot | 2 | 0 | 0.00 | 0 | 0 | 0 | 0 | 2 | 0 |


#### Serving-route confounds in Table 2

*Graded episodes measured on a provider route this project has documented as broken. They are NOT re-run: unlike an INFRA episode, which never reached the model and so spent no held-out exposure, these were graded — re-running them would touch the once-only held-out axis twice.*

- **`meta-llama/llama-4-maverick` / main / m6_final** (2 episodes: T1_BOYA_CARR, T1_VENUS_SNS). Both held-out main-arm episodes ran 2026-09-10 09:48-09:53, BEFORE the provider pin moved from Google to DigitalOcean at 13:12 that day (commit 1aa5ee4). They were therefore measured on the Google route that pin_history already records as withdrawn and non-tool-calling. Both episodes show ~zero MCP calls (get_results x1, and none) and grade L0 - the exact signature pin_history attributes to the Google route, not to the model. The same model tool-calls correctly on DigitalOcean in the main matrix (Table 1). **Reported with this caveat rather than excluded or re-run. maverick's held-out main row is a serving-route artifact and must not be read as a capability measurement.**

### Table 3 — Failure kinds (valid failures only)

*`format` = never engaged the tools, or emitted no parseable file one-shot · `incomplete` = used the tools but finished no instrument · `physics` = built something that compiles, runs, or scores wrongly. The format/incomplete split matters: an episode with 50 validated tool calls and no finished instrument is not a protocol-mechanics failure.*

| set | format | incomplete | physics |
|---|---:|---:|---:|
| matrix | 38 | 51 | 96 |
| held-out | 5 | 3 | 8 |

### Table 4 — Contamination probes (per model, temperature 0, provider-pinned)

| model | references memorized | n probed |
|---|---|---:|
| `anthropic/claude-haiku-4.5` | none | 16 |
| `anthropic/claude-sonnet-5` | ILL_H10_IN8, templateSANS | 16 |
| `google/gemini-3.5-flash-lite` | none | 16 |
| `google/gemini-3.6-flash` | ILL_H10_IN8 | 16 |
| `meta-llama/llama-4-maverick` | probe incomplete | 0 |

### Statistical power — read this before quoting any arm-vs-arm difference

Two-sided Fisher exact on the pass counts. **At n=17 tasks per cell, seen-tier differences between arms are NOT resolvable**, and the one difference that is significant argues against the headline reading rather than for it.

| comparison | counts | p | reading |
|---|---|---:|---|
| seen: `anthropic_claude_haiku_4_5` loop vs one-shot | 4/16 vs 4/16 | 1.000 | not resolvable |
| seen: `anthropic_claude_sonnet_5` loop vs one-shot | 5/16 vs 7/16 | 0.716 | not resolvable |
| seen: `google_gemini_3_5_flash_lite` loop vs one-shot | 2/16 vs 1/16 | 1.000 | not resolvable |
| seen: `google_gemini_3_6_flash` loop vs one-shot | 4/16 vs 6/16 | 0.704 | not resolvable |
| held-out: loop vs one-shot (paired, 7 models) | 10/14 vs 2/14 | 0.006 | suggestive, n small, unadjusted |
| **held-out vs seen difficulty (loop arm)** | 10/14 vs 20/113 | **0.0001** | **held-out tasks are EASIER — a confound for the row above** |
| RL ladder: 8B vs 32B (loop) | 1/16 vs 3/16 | 0.600 | ordering holds as a strict pass-set superset; rate difference underpowered |

**What this means for the manuscript.** The defensible claims are the ones that do not rest on small pass-rate differences: zero reference leaks across the whole campaign; T2 unsolved by every model in every arm; the untrained open-weights floor and its strict-superset ordering; and the *distributional* failure structure of Table 3 (one-shot dies at L1, the loop dies at L0). Arm-vs-arm superiority on the seen tier is **not** supported, and the held-out reversal cannot be attributed to novelty while the difficulty difference is uncontrolled — controlling it needs held-out instruments matched to seen-tier complexity, which is future work, not a claim this data can carry.

### Table 5 — T2 classical baselines (1e+08 rays, fresh seed 20260823)

*Two independent baselines, not three: the mcrun optimizer arm was discarded during calibration (nelder-mead escaped parameter bounds), so `classical_best` IS the constraint-filtered random-search winner.* **No agent, in any arm, has yet beaten either T2 target — the improvement tier is unsolved across the whole matrix, which is the headroom the RL track targets.**

| task | initial FOM | classical best | improvement |
|---|---:|---:|---:|
| `T2_guide_divergence` | 0.000406812 | 0.00119898 | 2.95× (628.6σ) |
| `T2_sans_collimation` | 0.0445914 | 0.0657457 | 1.47× (510.3σ) |

---

Raw evidence per episode (report, transcript, built `.instr`, diagram): `benchmark/evidence/<set>/<task>__<arm>__<model>/`.
