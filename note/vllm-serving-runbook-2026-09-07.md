# vLLM serving runbook — M6 open-weights arms + M8 stack (2026-09-07)

**Status: runbook for the user** (the A100 host is user-operated by
standing decision; a peer session correctly declined to touch it and its
serving review is folded in here). Laptop side is fully plumbed
(`db88a35` + per-model endpoint fix): the moment an endpoint env var is
set, the matrix picks that model up at $0.

## The contract (must match `benchmark/m6_config.json`)

Two models, REQUIRED (they are the RL-claim baselines):

| Model id (= `--served-model-name`, exact) | Endpoint env var on the laptop |
|---|---|
| `qwen3-8b`  (untrained Qwen-family 7–8B — the M8 training family) | `NEUTRONGYM_VLLM_URL_8B` |
| `qwen3-32b` (larger untrained comparator) | `NEUTRONGYM_VLLM_URL_32B` |

Per-model env vars (peer-review fix): `vllm serve` is one model per
process, so serve on **different ports concurrently** or **sequentially**
— either works; an unset var simply skips that model. Never point both
vars at one server.

## Serving commands (adjust HF paths to what's on the host)

```bash
# 8B: single GPU
vllm serve <hf-path-qwen3-8b>  --served-model-name qwen3-8b  --port 8000 \
    --tensor-parallel-size 1 --max-model-len 131072 \
    --enable-auto-tool-choice --tool-call-parser hermes

# 32B: TP4 (TP7 impossible — TP must divide attention heads; TP2 on 40 GB
# cards is tight once KV cache is added)
vllm serve <hf-path-qwen3-32b> --served-model-name qwen3-32b --port 8001 \
    --tensor-parallel-size 4 --max-model-len 131072 \
    --enable-auto-tool-choice --tool-call-parser hermes
```

Key choices (decide deliberately, they touch measurement validity):

1. **Context length**: the protocol is 50 turns with MCP tool results
   accumulating in-context; Qwen3's native 32k WILL overrun on long
   episodes and the failures would masquerade as capability differences.
   Serve with `--max-model-len` ≥ 128k (YaRN rope scaling). Overruns that
   still occur are recorded as infra-errors in the episode meta (`error`
   field) — M7 must report them separately from capability failures.
2. **Thinking mode**: the protocol pins temperature 0.0, where Qwen3
   thinking mode is prone to repetition loops (the loop's empty-turn
   nudge does not fix a repetition loop). **Baseline arms serve in
   NON-thinking mode** (chat-template default / `enable_thinking=false`);
   if thinking is ever enabled, `--reasoning-parser` is needed so
   tool-calls still parse. Record whichever is served — it is part of the
   measurement config.
3. Tool calling must be on (the reference loop drives MCP tools through
   OpenAI tool-calls): `--enable-auto-tool-choice --tool-call-parser
   hermes` (verify the parser name against the installed vLLM).

## From the laptop, once serving

```bash
# reachability (SSH tunnel fine: ssh -L 8000:localhost:8000 <host>)
export NEUTRONGYM_VLLM_URL_8B=http://localhost:8000/v1
curl -s $NEUTRONGYM_VLLM_URL_8B/models

# shakedown first (P1/P3 — closes the last scaffold-decision shakedown gap)
ENV_PY=/opt/homebrew/Caskroom/miniconda/base/envs/mcstas/bin/python
$ENV_PY benchmark/harness/run_episode.py P1_sans_reproduce \
    --model qwen3-8b --base-url $NEUTRONGYM_VLLM_URL_8B --provider ''

# then the arms (34 episodes per model, $0)
$ENV_PY benchmark/harness/run_matrix.py --priority 1 --model qwen3-8b
$ENV_PY benchmark/harness/run_matrix.py --priority 3 --model qwen3-8b
```

## Why this is urgent (Sep 7)

These arms feed the Sep 8–10 held-out final pass, and the same stack is
the prerequisite for ANY M8 content — no SFT/GRPO has run; 17 days to the
paper deadline, degradation ladder GRPO → SFT-only → env+eval-only.
