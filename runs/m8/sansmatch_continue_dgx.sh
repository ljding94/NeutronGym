#!/usr/bin/env bash
# sans_match GRPO continuation: steps 221-340 at lr 3e-5 (2026-09-19).
# First run reached 38.3% from 22.3% but the curve had not converged.
set -u
cd /netdisk/ldq/NeutronGym
RUN=/netdisk/ldq/mcstas-run.sh
SFT=/netdisk/ldq/sft-env/bin/python
BASE=/netdisk/ldq/hf/hub/models--Qwen--Qwen3-8B/snapshots/b968826d9c46dd6066d109eabc6255188de91218
PREV=/netdisk/ldq/ckpt/m8-sansmatchgrpo-b
TAG=sansmatchgrpo340
CKPT=/netdisk/ldq/ckpt/m8-$TAG
NAME=qwen3-8b-m8-$TAG
export NEUTRONGYM_VLLM_URL_8B=http://localhost:8137/v1 NEUTRONGYM_VLLM_URL_TRAINED=http://localhost:8139/v1
done_() { echo "SANSMATCH_CONT_DONE $1 $(date +%T)"; exit "${2:-0}"; }
[ -d $PREV/adapter ] || done_ NO_PREV_ADAPTER 1

echo "== 1. free GPU 7 and restart the reward server $(date +%T) =="
for n in qwen3-8b-m8-sansmatchgrpo qwen3-8b-m8-match05grpoLR; do pkill -f "served-model-name $n"; done
for i in $(seq 1 60); do [ $(nvidia-smi -i 7 --query-gpu=memory.used --format=csv,noheader,nounits) -lt 1000 ] && break; sleep 5; done
pkill -f "reward_server.py --port 8199"; sleep 3
(setsid nohup taskset -c 48-63,80-95 $RUN python benchmark/harness/reward_server.py --port 8199 --workers 32 > /netdisk/ldq/grpo/reward_server.log 2>&1 < /dev/null &)
for i in $(seq 1 90); do grep -q "reward server on" /netdisk/ldq/grpo/reward_server.log 2>/dev/null && break; sleep 2; done

echo "== 2. steps 221-340 $(date +%T) =="
CUDA_DEVICE_ORDER=PCI_BUS_ID CUDA_VISIBLE_DEVICES=7 HF_HOME=/netdisk/ldq/hf TMPDIR=/netdisk/ldq/tmp PYTORCH_CUDA_ALLOC_CONF=expandable_segments:True \
  $SFT -u benchmark/harness/m8_grpo.py --states runs/m8/grpo_states_sansmatch.jsonl --base $BASE --out $CKPT \
  --family sans_match --init-adapter $PREV/adapter --start-step 220 --steps 120 \
  --states-per-step 8 --group 8 --lr 3e-5 --kl 0.01 --save-every 40 \
  > runs/m8/grpo_${TAG}.log 2>&1 || { tail -12 runs/m8/grpo_${TAG}.log; done_ TRAINING_FAILED 1; }
[ -f $CKPT/merged/config.json ] || done_ NO_MERGED 1

echo "== 3. serve + eval, held-out 300-599 $(date +%T) =="
(MODEL=$CKPT/merged GPU=7 PORT=8139 NAME=$NAME setsid nohup /netdisk/ldq/serve-m8-trained.sh > /netdisk/ldq/serve-m8-$TAG.log 2>&1 < /dev/null &)
for i in $(seq 1 90); do curl -s -m 5 localhost:8139/v1/models | grep -q $NAME && break; sleep 20; done
curl -s -m 5 localhost:8139/v1/models | grep -q $NAME || done_ SERVE_TIMEOUT 1
taskset -c 112-127 $RUN python -u benchmark/harness/m8_eval.py --target-fraction 0.85 --max-steps 10 --n 300 \
  --start-index 300 --families sans_match --arms trained-8b --trained-model $NAME \
  --out runs/m8/eval_sansmatch_fresh_trained340.json 2>&1 | grep -v libmamba | tail -2 || done_ EVAL_FAILED 1
done_ OK 0
