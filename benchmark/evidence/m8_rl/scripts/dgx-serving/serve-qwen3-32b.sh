#!/usr/bin/env bash
# NeutronGym M6 open-weights arm — Qwen3-32B via vLLM.
# Usage: GPUS=1,2,3,4 PORT=8138 ./serve-qwen3-32b.sh
set -euo pipefail
# venv bin on PATH so torch-inductor can find ninja/cmake (host lacks them)
export PATH="/netdisk/ldq/vllm-env2/bin:$PATH"
# root FS is near-full; keep scratch and weights on /netdisk
export TMPDIR=/netdisk/ldq/tmp
export HF_HOME=/home/ldq/.cache/huggingface
export CUDA_DEVICE_ORDER=PCI_BUS_ID
export CUDA_VISIBLE_DEVICES="${GPUS:-1,2,3,4}"
# YaRN required: Qwen3-32B native max_position_embeddings is 40960, below the
# measured 67-77k peak episode context. 98304 matches the 32B arm exactly.
exec /netdisk/ldq/vllm-env2/bin/vllm serve Qwen/Qwen3-32B \
  --served-model-name qwen3-32b \
  --port "${PORT:-8138}" \
  --tensor-parallel-size 4 \
  --max-model-len 98304 \
  --hf-overrides "{\"rope_scaling\":{\"rope_type\":\"yarn\",\"factor\":4.0,\"original_max_position_embeddings\":40960}}" \
  --enable-auto-tool-choice --tool-call-parser hermes
