# NeutronGym — results of record

*Generated 2026-09-10 15:40 by `benchmark/harness/results_report.py` from the committed evidence in `benchmark/evidence/`. **These are the numbers the manuscript cites.** Regenerate after any re-run; diff the JSON to see what moved.*

**Validity rule:** INFRA (endpoint/provider/harness failures) and LEAK episodes are excluded from every rate and reported separately. This is not cosmetic — on 2026-09-10 an unnoticed dead SSH tunnel put 77 infra failures into the tables as capability zeros, which invalidated an entire model row until caught and re-run.

**Totals:** 253 valid matrix episodes · 18 valid held-out episodes · 10 infra-excluded · 0 leak-invalid (zero leaks across the whole campaign) · $121.30 OpenRouter spend.

### Table 1 — Main matrix (seen-tier scored set)

`main` = reference loop with MCP + skill · `oneshot` = plain-LLM, no tools · `noskill` = loop without the design skill · `claude_code` = production-harness comparison arm.

| model | arm | valid n | passes | pass rate | L4 | L3 | L2 | L1 | L0 | infra excl. |
|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| `anthropic_claude_haiku_4_5` | main | 17 | 4 | 0.24 | 5 | 2 | 2 | 4 | 0 | 0 |
| `anthropic_claude_haiku_4_5` | oneshot | 17 | 4 | 0.24 | 0 | 0 | 0 | 13 | 0 | 0 |
| `anthropic_claude_sonnet_5` | main | 17 | 5 | 0.29 | 2 | 0 | 0 | 9 | 1 | 0 |
| `anthropic_claude_sonnet_5` | oneshot | 17 | 7 | 0.41 | 0 | 1 | 2 | 6 | 1 | 0 |
| `google_gemini_3_5_flash_lite` | main | 17 | 2 | 0.12 | 1 | 0 | 1 | 5 | 8 | 0 |
| `google_gemini_3_5_flash_lite` | oneshot | 17 | 1 | 0.06 | 0 | 0 | 0 | 16 | 0 | 0 |
| `google_gemini_3_6_flash` | main | 21 | 6 | 0.29 | 1 | 2 | 0 | 2 | 10 | 0 |
| `google_gemini_3_6_flash` | noskill | 5 | 3 | 0.60 | 1 | 0 | 0 | 1 | 0 | 0 |
| `google_gemini_3_6_flash` | oneshot | 17 | 6 | 0.35 | 1 | 1 | 2 | 7 | 0 | 0 |
| `meta_llama_llama_4_maverick` | main | 17 | 0 | 0.00 | 0 | 0 | 0 | 0 | 17 | 0 |
| `meta_llama_llama_4_maverick` | oneshot | 17 | 2 | 0.12 | 0 | 0 | 0 | 15 | 0 | 0 |
| `openai_gpt_5_2_pro` | main | 1 | 1 | 1.00 | 0 | 0 | 0 | 0 | 0 | 0 |
| `qwen3_32b` | main | 17 | 3 | 0.18 | 0 | 1 | 0 | 0 | 13 | 0 |
| `qwen3_32b` | oneshot | 17 | 0 | 0.00 | 0 | 0 | 0 | 0 | 17 | 0 |
| `qwen3_8b` | main | 17 | 1 | 0.06 | 0 | 2 | 1 | 0 | 13 | 0 |
| `qwen3_8b` | oneshot | 17 | 0 | 0.00 | 0 | 0 | 0 | 0 | 17 | 0 |
| `subscription` | claude_code | 5 | 2 | 0.40 | 1 | 1 | 0 | 1 | 0 | 0 |


### Table 2 — Held-out final pass (once-only touch)

Instruments with no public `.instr` (BOYA, VENUS), authored for this benchmark. **The seen-tier one-shot advantage reverses here: the tool loop dominates.**

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
| `meta_llama_llama_4_maverick` | oneshot | 0 | 0 | — | 0 | 0 | 0 | 0 | 0 | 2 |
| `qwen3_32b` | main | 0 | 0 | — | 0 | 0 | 0 | 0 | 0 | 2 |
| `qwen3_32b` | oneshot | 0 | 0 | — | 0 | 0 | 0 | 0 | 0 | 2 |
| `qwen3_8b` | main | 0 | 0 | — | 0 | 0 | 0 | 0 | 0 | 2 |
| `qwen3_8b` | oneshot | 0 | 0 | — | 0 | 0 | 0 | 0 | 0 | 2 |


### Table 3 — Failure kinds (valid failures only)

| set | format (tool/protocol mechanics) | physics (wrong instrument) |
|---|---:|---:|
| matrix | 97 | 109 |
| held-out | 3 | 6 |

### Table 4 — Contamination probes (per model, temperature 0, provider-pinned)

| model | references memorized | n probed |
|---|---|---:|
| `anthropic/claude-haiku-4.5` | none | 16 |
| `anthropic/claude-sonnet-5` | ILL_H10_IN8, templateSANS | 16 |
| `google/gemini-3.5-flash-lite` | none | 16 |
| `google/gemini-3.6-flash` | ILL_H10_IN8 | 16 |
| `meta-llama/llama-4-maverick` | probe incomplete | 0 |

### Table 5 — T2 classical baselines (1e+08 rays, fresh seed 20260823)

*Two independent baselines, not three: the mcrun optimizer arm was discarded during calibration (nelder-mead escaped parameter bounds), so `classical_best` IS the constraint-filtered random-search winner.* **No agent, in any arm, has yet beaten either T2 target — the improvement tier is unsolved across the whole matrix, which is the headroom the RL track targets.**

| task | initial FOM | classical best | improvement |
|---|---:|---:|---:|
| `T2_guide_divergence` | 0.000406812 | 0.00119898 | 2.95× (628.6σ) |
| `T2_sans_collimation` | 0.0445914 | 0.0657457 | 1.47× (510.3σ) |

---

Raw evidence per episode (report, transcript, built `.instr`, diagram): `benchmark/evidence/<set>/<task>__<arm>__<model>/`.
