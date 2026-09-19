#!/usr/bin/env bash
# Unbiased number for the CHOSEN guide_match checkpoint (2026-09-17).
# 120/220/340-step checkpoints were compared on held-out 0-299, so that slice
# can no longer give an unbiased estimate for the winner. Instances 300-599 of
# the same held-out split played no part in the choice.
set -u
cd /netdisk/ldq/NeutronGym
RUN=/netdisk/ldq/mcstas-run.sh
CKPT=/netdisk/ldq/ckpt/m8-match05grpoLR      # the 220-step model
NAME=qwen3-8b-m8-match05grpoLR
export NEUTRONGYM_VLLM_URL_8B=http://localhost:8137/v1 NEUTRONGYM_VLLM_URL_32B=http://localhost:8138/v1 NEUTRONGYM_VLLM_URL_TRAINED=http://localhost:8139/v1
done_() { echo "MATCH_FRESH_DONE $1 $(date +%T)"; exit "${2:-0}"; }

echo "== 1. targets for held-out 300-599 $(date +%T) =="
taskset -c 0-15,32-47,64-79,96-111 $RUN python benchmark/harness/precalibrate.py --family guide_match \
  --split heldout --start 300 --n 300 --workers 48 | tail -1 || done_ PRECAL_FAILED 1

echo "== 2. serve the 220-step checkpoint $(date +%T) =="
for n in qwen3-8b-m8-match05grpo340 qwen3-8b-m8-match05grpoLR; do pkill -f "served-model-name $n"; done
for i in $(seq 1 60); do [ $(nvidia-smi -i 7 --query-gpu=memory.used --format=csv,noheader,nounits) -lt 1000 ] && break; sleep 5; done
(MODEL=$CKPT/merged GPU=7 PORT=8139 NAME=$NAME setsid nohup /netdisk/ldq/serve-m8-trained.sh > /netdisk/ldq/serve-m8-fresh.log 2>&1 < /dev/null &)
for i in $(seq 1 90); do curl -s -m 5 localhost:8139/v1/models | grep -q $NAME && break
  grep -q "Engine core initialization failed" /netdisk/ldq/serve-m8-fresh.log && done_ SERVE_FAILED 1; sleep 20; done
curl -s -m 5 localhost:8139/v1/models | grep -q $NAME || done_ SERVE_TIMEOUT 1

echo "== 3. fresh-slice eval, both arms, instances 300-599 $(date +%T) =="
for arm in untrained-8b trained-8b; do
  taskset -c 112-127 $RUN python -u benchmark/harness/m8_eval.py --target-fraction 0.85 --max-steps 10 \
    --n 300 --start-index 300 --families guide_match --arms $arm --trained-model $NAME \
    --out runs/m8/eval_match_fresh_$arm.json > runs/m8/eval_match_fresh_$arm.log 2>&1 &
done
wait
for arm in untrained-8b trained-8b; do grep -v "libmamba\|Waiting for\|Could not set" runs/m8/eval_match_fresh_$arm.log | tail -2; done
done_ OK 0
