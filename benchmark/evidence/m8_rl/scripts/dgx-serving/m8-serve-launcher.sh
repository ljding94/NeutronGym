#!/usr/bin/env bash
# Wait for the M8 merge to finish and GPU 7 to free, then serve the merged
# checkpoint with the base 8B's exact vLLM flags. Gives up after 60 min so a
# failed training run never leaves a stray server.
TRAIN_LOG=/netdisk/ldq/m8-train-guide1x.log
MERGED=/netdisk/ldq/ckpt/m8-raft-guide1x/merged
deadline=$(( $(date +%s) + 3600 ))
echo "launcher waiting for merge $(date)"
until grep -q "^-> merged" "$TRAIN_LOG" 2>/dev/null; do
  if grep -qE "Traceback|out of memory|Killed" "$TRAIN_LOG" 2>/dev/null; then
    echo "TRAINING_FAILED_NOT_SERVING"; exit 1; fi
  if [ "$(date +%s)" -gt "$deadline" ]; then echo "TIMEOUT_NOT_SERVING"; exit 1; fi
  sleep 20
done
echo "merge done, waiting for GPU 7 to free $(date)"
until [ "$(nvidia-smi -i 7 --query-gpu=memory.used --format=csv,noheader,nounits)" -lt 1000 ]; do
  if [ "$(date +%s)" -gt "$deadline" ]; then echo "GPU7_NEVER_FREED_NOT_SERVING"; exit 1; fi
  sleep 10
done
if [ ! -f "$MERGED/config.json" ]; then echo "NO_MERGED_CONFIG"; exit 1; fi
echo "LAUNCHING_VLLM $(date)"
MODEL="$MERGED" GPU=7 PORT=8139 NAME=qwen3-8b-m8raft exec /netdisk/ldq/serve-m8-trained.sh
