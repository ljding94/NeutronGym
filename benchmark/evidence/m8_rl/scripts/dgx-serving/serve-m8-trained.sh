#!/usr/bin/env bash
# M8: serve the merged RAFT checkpoint with EXACTLY the flags of
# serve-qwen3-8b.sh (same vLLM env, YaRN factor 4.0, 98304 ctx, same parsers),
# so trained-vs-untrained is a weights-only difference.
# Usage: MODEL=/netdisk/ldq/ckpt/m8-raft/merged GPU=7 PORT=8139 ./serve-m8-trained.sh
set -euo pipefail
: "${MODEL:?set MODEL to the merged checkpoint directory}"
export PATH="/netdisk/ldq/vllm-env2/bin:$PATH"
export TMPDIR=/netdisk/ldq/tmp
export HF_HOME=/netdisk/ldq/hf
export CUDA_DEVICE_ORDER=PCI_BUS_ID
export CUDA_VISIBLE_DEVICES="${GPU:-7}"
exec /netdisk/ldq/vllm-env2/bin/vllm serve "$MODEL" \
  --served-model-name "${NAME:-qwen3-8b-m8raft}" \
  --port "${PORT:-8139}" \
  --tensor-parallel-size 1 \
  --max-model-len 98304 \
  --hf-overrides "{\"rope_scaling\":{\"rope_type\":\"yarn\",\"factor\":4.0,\"original_max_position_embeddings\":40960}}" \
  --enable-auto-tool-choice --tool-call-parser hermes
