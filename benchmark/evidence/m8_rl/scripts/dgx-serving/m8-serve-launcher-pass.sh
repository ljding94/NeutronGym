#!/usr/bin/env bash
# Serve the passing-turn ablation checkpoint once its merge finishes, with
# the base 8B's exact flags. Guarded by the port and GPU 7 occupancy, not by
# process names (exec -a hid a launcher from pgrep on 2026-09-13).
TRAIN_LOG=/netdisk/ldq/m8-train-guide1x-pass.log
MERGED=/netdisk/ldq/ckpt/m8-raft-guide1x-pass/merged
deadline=$(( $(date +%s) + 3600 ))
echo "launcher waiting for ablation merge $(date)"
until grep -q "^-> merged" "$TRAIN_LOG" 2>/dev/null; do
  if grep -qE "Traceback|out of memory|Killed|MISMATCH" "$TRAIN_LOG" 2>/dev/null; then
    echo "ABLATION_TRAINING_FAILED_NOT_SERVING"; exit 1; fi
  if [ "$(date +%s)" -gt "$deadline" ]; then echo "ABLATION_TIMEOUT_NOT_SERVING"; exit 1; fi
  sleep 20
done
until [ "$(nvidia-smi -i 7 --query-gpu=memory.used --format=csv,noheader,nounits)" -lt 1000 ]; do
  if [ "$(date +%s)" -gt "$deadline" ]; then echo "ABLATION_GPU7_NEVER_FREED_NOT_SERVING"; exit 1; fi
  sleep 10
done
if ss -ltn | grep -q ":8139 "; then echo "ABLATION_PORT_8139_BUSY_NOT_SERVING"; exit 1; fi
if [ ! -f "$MERGED/config.json" ]; then echo "ABLATION_NO_MERGED_CONFIG"; exit 1; fi
echo "ABLATION_LAUNCHING_VLLM $(date)"
MODEL="$MERGED" GPU=7 PORT=8139 NAME=qwen3-8b-m8raft-pass exec /netdisk/ldq/serve-m8-trained.sh
