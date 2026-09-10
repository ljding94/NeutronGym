# vLLM serving runbook — M6 open-weights arms + M8 stack (2026-09-07)

## ⚠ HOST RECONNAISSANCE 2026-09-08 (peer session, neutrons-dgx01) — supersedes assumptions below

Findings from the actual box (8× A100-SXM4-40GB):
- **vLLM not installed, nothing of ours running.** Root FS (/dev/md0,
  1.8T) is **100% full** and /home lives on it → env install AND weight
  downloads must go to **/netdisk** (94T free): set `HF_HOME` and the
  venv/conda prefix there deliberately.
- **Port 8000 is TAKEN** by another user's uvicorn app (answers /, 404s
  /v1/models) — serve on a free port (e.g. 8010/8011) or tunnels will
  "connect" and then fail confusingly at the first call.
- **Only GPUs 0–1 are free**; 2–7 are held by other users' week-long
  jobs. Consequences:
  - `qwen3-8b`: **runnable on one free card** (~30.6 GB at 96k) — but
    the weights are ABSENT from the host cache and need a ~16 GB
    download (to /netdisk).
  - `qwen3-32b` (weights present, 68 G): **NOT runnable today** — TP4
    needs 4 free cards; TP2 leaves only ~4–6 GB/card for KV (~16k-order
    context vs the measured ~77k peak → silent truncation masquerading
    as capability failure); FP8 would fit but changes the model, which
    is unacceptable for an untrained RL-claim baseline.
  - **Option to evaluate: Qwen3.6-27B is already complete on the host
    cache.** As the "larger untrained comparator" it may fit at TP2 on
    the two free cards (≈54 GB bf16 weights over two 36 GB budgets
    leaves ~18 GB total for KV — plausibly enough for the 77k ceiling,
    but VERIFY against the model's actual KV-per-token from its config
    before promising it). Substituting 27B for 32B is a config/PLAN
    change the user must approve; the plan's bar says "a larger
    untrained comparator", not 32B specifically.

Near-term realistic shape: **8B-only first** (baseline REQUIRED for the
RL claim + M8), 27B-as-comparator if it verifies, 32B deferred until
four cards free up. All host changes (env to /netdisk, weight download,
serving) are the user's call on a shared institutional machine.


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
# 8B: single GPU. 96k, NOT 128k — 131072 does not fit at TP1 on a 40 GB
# card (arithmetic in note 1). Use TP2 if you want the full 128k here.
# YaRN REQUIRED (host verification 2026-09-08): Qwen3-8B's native
# max_position_embeddings is 40960 — vLLM REFUSES a larger --max-model-len
# without explicit rope scaling. Port must NOT be 8000 (taken on the host).
vllm serve <hf-path-qwen3-8b>  --served-model-name qwen3-8b  --port 8010 \
    --tensor-parallel-size 1 --max-model-len 98304 \
    --hf-overrides '{"rope_scaling": {"rope_type": "yarn", "factor": 4.0, "original_max_position_embeddings": 40960}}' \
    --enable-auto-tool-choice --tool-call-parser hermes
# MEASUREMENT-CONFIG NOTE: YaRN is a documented Qwen3 long-context mode
# but it ALTERS the model versus stock (and static YaRN affects short
# requests too). It is pinned in m6_config.json for the qwen3-8b arm and
# MUST be stated in M7. The alternative — native 40960 — would silently
# truncate long episodes into fake capability failures.

# 32B: TP4 (TP7 impossible — TP must divide attention heads; TP2 on 40 GB
# cards is tight once KV cache is added).
# CORRECTED 2026-09-10: Qwen3-32B's native max_position_embeddings is ALSO
# 40960 — YaRN required here too, and it serves at 98304 (NOT 131072):
# identical context budgets across both open-weights arms RETIRES the
# note-1b comparability caveat. Live ports: 8B on 8137 (GPU 0), 32B on
# 8138 (GPUs 1-4); env at /netdisk/ldq/vllm-env2 (uv CPython 3.12.14 —
# the system python lacked dev headers and Triton could not compile).
vllm serve <hf-path-qwen3-32b> --served-model-name qwen3-32b --port 8138 \
    --tensor-parallel-size 4 --max-model-len 98304 \
    --hf-overrides '{"rope_scaling": {"rope_type": "yarn", "factor": 4.0, "original_max_position_embeddings": 40960}}' \
    --enable-auto-tool-choice --tool-call-parser hermes
```

Key choices (decide deliberately, they touch measurement validity):

1. **Context length — now measured, not guessed** (peer session, 185 M6
   evidence episodes). Worst observed loop episode: `T1_HZB_NEAT` /
   sonnet-5, 50 turns, 235,318 bytes of final conversation +
   ~10k tokens of static prefix (skill + 22 tool schemas, resent every
   turn) → **peak single-request context ≈ 67–77k tokens**. Cross-check:
   the reconstructed per-turn sum matches that episode's recorded
   3,827,672 cumulative prompt tokens, so the profile is sound.
   Consequences:
   - Qwen3's native 32k **WILL** overrun — confirmed, not hypothetical.
   - 64k is **marginal** (clips the worst episodes); do not use it.
   - **96k is the sweet spot**: covers the measured worst case with
     headroom, and fits at TP1 on one 40 GB card.
   - KV-cache arithmetic (why 128k does not fit the 8B at TP1): Qwen3-8B
     is 36 layers × 8 KV heads × 128 head_dim, i.e. ~144 KiB/token.
     At 131072 tokens that is ~18.9 GB of KV on top of ~16.4 GB of bf16
     weights = ~35.3 GB, which will not allocate under vLLM's default
     0.90 memory utilization on a 40 GB A100. At 98304 it is ~14.2 GB
     KV → ~30.6 GB total, comfortable. The 32B at TP4 is unaffected
     (~16 GB weights + ~8.4 GB KV per card at 128k = ~24.4 GB) — keep
     131072 there.
   Overruns that still occur are recorded as infra-errors in the episode
   meta (`error` field) — M7 must report them separately from capability
   failures.
1b. **M7 comparability flag (peer review)**: serving 8B@96k and 32B@128k
   means the two open-weights arms do NOT have identical context budgets.
   At the measured 77k ceiling neither should clip, so in practice this
   should not bite — but if ANY qwen episode records a context overrun in
   its meta `error` field, the arms are not strictly comparable and M7
   must state that rather than quietly averaging. (Per-request usage is
   now recorded on every assistant transcript event, so peak context is
   directly observable — no reconstruction needed for the qwen arms.)
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
