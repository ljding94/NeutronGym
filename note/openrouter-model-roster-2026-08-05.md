# OpenRouter model roster + provider-pinning policy (2026-08-05)

**Status: measured reference for M6/M8 model selection; pinning policy
adopted same day.** Two probes bracket a guardrail change:

1. **Morning (Vertex-guardrailed key):** the policy pinned OpenRouter to
   **Google Vertex** — which serves several non-Google families, so the
   roster was already wider than the "only Google models route" assumption
   (spike note 2026-07-24).
2. **Afternoon (guardrail removed, user action):** the full catalog routes.
   Same-hour evidence that unpinned routing drifts: `claude-sonnet-5`
   served by Google in probe 1 came back via **Amazon Bedrock** unpinned in
   probe 2; llama-4-maverick moved Google → DigitalOcean.

**Policy (user decision 2026-08-05): every scored run pins ONE serving
provider per model, no fallbacks** (`provider: {order: [X],
allow_fallbacks: false}`) — Google/Vertex wherever it serves the model,
the model's first-party provider otherwise. Implemented in the reference
loop (`provider_pin`, harness `--provider`, default Google); the serving
provider is recorded per turn in every transcript, and `providers_seen`
in every episode meta makes drift auditable after the fact.

Probes are live 1-token completions under the project key — the only test
that reflects real routing (catalog/endpoint listings do not).

## Probe results (post-guardrail, 2026-08-05) — with the pin each model gets

| Model | Pin (scored runs) | Verified | $/M in/out |
|---|---|---|---|
| anthropic/claude-opus-5 | **Google** | ✅ (probe 1) | 5 / 25 |
| anthropic/claude-sonnet-5 | **Google** | ✅ pinned probe (unpinned drifts to Bedrock) | 2 / 10 |
| anthropic/claude-haiku-4.5 | **Google** | ✅ (probe 1) | 1 / 5 |
| google/gemini-3.6-flash | **Google** | ✅ both probes | 1.5 / 7.5 |
| google/gemini-3.1-pro-preview | **Google** | ✅ (probe 1) | 2 / 12 |
| google/gemini-3.5-flash-lite | **Google** | ✅ (probe 1) | 0.3 / 2.5 |
| meta-llama/llama-4-maverick | **Google** | ✅ pinned probe (unpinned → DigitalOcean) | 0.2 / 0.8 |
| meta-llama/llama-3.3-70b-instruct | **Google** | ✅ pinned probe (morning 429 resolved) | 0.1 / 0.32 |
| qwen/qwen3.8-max, 3.7-plus, 3.7-flash | Alibaba (first-party) | ✅ unpinned via Alibaba; pinned probe TODO at M6 freeze | — |
| openai/gpt-5.2-pro | OpenAI (first-party) | ✅ unpinned via OpenAI | — |
| mistralai/mistral-large-2512, small-2603 | Mistral (first-party) | ✅ unpinned via Mistral | — |
| deepseek/deepseek-v4-pro | pick at M6 freeze | ✅ unpinned via Cloudflare (reseller — prefer first-party if listed) | 0.44 / 0.87 |
| meta-llama/llama-3.1-8b-instruct | not on Vertex | ✅ unpinned via DeepInfra | 0.05 / 0.08 |

Catalog context: 339 models total on OpenRouter; the tool-capable set spans
openai (60), qwen (49), google (30), mistralai (18), anthropic (17),
deepseek (12), meta-llama (8). Everything probed routes now; the table's
"Pin" column is what scored runs must pass (`--provider`, default Google).

## Episode-cost anchors (reference loop, measured token profile)

The P1 loop shakedown consumed ~260k prompt + ~9k completion tokens.
Per-episode estimates at that profile:

| Model | ~$/episode |
|---|---|
| claude-opus-5 | ~1.55 |
| claude-sonnet-5 | ~0.61 |
| gemini-3.6-flash | ~0.46 (measured run) |
| claude-haiku-4.5 | ~0.30 |
| gemini-3.5-flash-lite | ~0.10 |
| llama-4-maverick | ~0.06 |

A 5-model × ~20-episode M6 API tier lands around **$30–60** — comfortably
inside the ~$200 OpenRouter budget.

## Implications (supersede earlier assumptions where noted)

1. **The reference loop's Claude arm is unblocked** — Claude routes via
   OpenRouter (`--model anthropic/claude-sonnet-5`, Google-pinned), no
   `ANTHROPIC_API_KEY` needed. Supersedes the loop-shipping caveat.
2. **The OpenRouter-privacy M6 prereq is CLOSED** (guardrail removed
   2026-08-05): full cross-vendor breadth available — Anthropic + Google +
   Meta on Vertex, plus OpenAI/Qwen-API/Mistral/DeepSeek via first-party
   pins if the matrix wants them.
3. **M6 candidate matrix (working set):** claude-sonnet-5 (main Claude
   arm) · gemini-3.6-flash (pilot-comparable) · gpt-5.2-pro (frontier
   breadth, first-party pin) · claude-haiku-4.5 + gemini-3.5-flash-lite +
   llama-4-maverick (small/cheap tier, all Google-pinned) ·
   claude-opus-5 reserved for the held-out final pass if budget allows.
   Subscription Claude Code stays the separate comparison arm ($0).
4. **Qwen API models route (Alibaba)** — useful for a Qwen-family API
   sanity arm, but the M8 training family still serves locally from the
   A100s via vLLM ($0; the untrained-baseline arms need local serving
   regardless). No plan change.
5. **Consistency rule:** never compare numbers across serving providers
   for the same model; `providers_seen` in each episode meta is the audit
   field. Provider pins get frozen with the M6 config; re-probe pinned
   routes at freeze (rosters and provider inventories drift).
