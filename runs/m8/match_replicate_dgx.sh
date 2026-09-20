#!/usr/bin/env bash
# Replication of the guide_match GRPO result with a different sampling seed
# (2026-09-18). Same two-stage recipe that produced 76.7%: fresh states from
# the untrained 8B, 120 steps at lr 1e-5, then 100 steps at lr 3e-5 from that
# adapter. Evaluated directly on the fresh slice (held-out 300-599).
set -u
cd /netdisk/ldq/NeutronGym
RUN=/netdisk/ldq/mcstas-run.sh
SFT=/netdisk/ldq/sft-env/bin/python
BASE=/netdisk/ldq/hf/hub/models--Qwen--Qwen3-8B/snapshots/b968826d9c46dd6066d109eabc6255188de91218
TAG=matchrep2
CKPT_A=/netdisk/ldq/ckpt/m8-$TAG-a
CKPT_B=/netdisk/ldq/ckpt/m8-$TAG-b
NAME=qwen3-8b-m8-$TAG
SEED=20260918
export NEUTRONGYM_VLLM_URL_8B=http://localhost:8137/v1 NEUTRONGYM_VLLM_URL_32B=http://localhost:8138/v1 NEUTRONGYM_VLLM_URL_TRAINED=http://localhost:8139/v1
done_() { echo "MATCH_REP_DONE $1 $(date +%T)"; exit "${2:-0}"; }

echo "== 1. fresh states from the untrained 8B $(date +%T) =="
taskset -c 112-127 $RUN python -u benchmark/harness/m8_states.py --family guide_match --start 0 --n 300 \
  --workers 8 --out runs/m8/grpo_states_match_rep2.jsonl 2>&1 | grep -v libmamba | tail -2 || done_ STATES_FAILED 1

echo "== 2. free GPU 7 and start the reward server $(date +%T) =="
# every trained-model server runs from /netdisk/ldq/ckpt; the base
# 8B/32B servers (Qwen/Qwen3-*) are on other GPUs and are left alone
pkill -f "vllm serve /netdisk/ldq/ckpt"
for i in $(seq 1 60); do [ $(nvidia-smi -i 7 --query-gpu=memory.used --format=csv,noheader,nounits) -lt 1000 ] && break; sleep 5; done
[ $(nvidia-smi -i 7 --query-gpu=memory.used --format=csv,noheader,nounits) -lt 1000 ] || done_ GPU7_BUSY 1
pgrep -f "reward_server.py --port 8199" > /dev/null || \
  (setsid nohup taskset -c 48-63,80-95 $RUN python benchmark/harness/reward_server.py --port 8199 --workers 32 > /netdisk/ldq/grpo/reward_server.log 2>&1 < /dev/null &)
for i in $(seq 1 90); do grep -q "reward server on" /netdisk/ldq/grpo/reward_server.log 2>/dev/null && break; sleep 2; done

echo "== 3. stage A: 120 steps at lr 1e-5 (seed $SEED) $(date +%T) =="
CUDA_DEVICE_ORDER=PCI_BUS_ID CUDA_VISIBLE_DEVICES=7 HF_HOME=/netdisk/ldq/hf TMPDIR=/netdisk/ldq/tmp PYTORCH_CUDA_ALLOC_CONF=expandable_segments:True \
  $SFT -u benchmark/harness/m8_grpo.py --states runs/m8/grpo_states_match_rep2.jsonl --base $BASE --out $CKPT_A \
  --family guide_match --steps 120 --states-per-step 8 --group 8 --lr 1e-5 --kl 0.02 --seed $SEED \
  > runs/m8/grpo_${TAG}_a.log 2>&1 || { tail -15 runs/m8/grpo_${TAG}_a.log; done_ STAGE_A_FAILED 1; }
[ -d $CKPT_A/adapter ] || done_ NO_STAGE_A_ADAPTER 1

echo "== 4. stage B: 100 steps at lr 3e-5 $(date +%T) =="
CUDA_DEVICE_ORDER=PCI_BUS_ID CUDA_VISIBLE_DEVICES=7 HF_HOME=/netdisk/ldq/hf TMPDIR=/netdisk/ldq/tmp PYTORCH_CUDA_ALLOC_CONF=expandable_segments:True \
  $SFT -u benchmark/harness/m8_grpo.py --states runs/m8/grpo_states_match_rep2.jsonl --base $BASE --out $CKPT_B \
  --family guide_match --init-adapter $CKPT_A/adapter --start-step 120 --steps 100 \
  --states-per-step 8 --group 8 --lr 3e-5 --kl 0.01 --seed $SEED \
  > runs/m8/grpo_${TAG}_b.log 2>&1 || { tail -15 runs/m8/grpo_${TAG}_b.log; done_ STAGE_B_FAILED 1; }
[ -f $CKPT_B/merged/config.json ] || done_ NO_MERGED 1

echo "== 5. serve + check $(date +%T) =="
(MODEL=$CKPT_B/merged GPU=7 PORT=8139 NAME=$NAME setsid nohup /netdisk/ldq/serve-m8-trained.sh > /netdisk/ldq/serve-m8-$TAG.log 2>&1 < /dev/null &)
for i in $(seq 1 90); do curl -s -m 5 localhost:8139/v1/models | grep -q $NAME && break
  grep -q "Engine core initialization failed" /netdisk/ldq/serve-m8-$TAG.log && done_ SERVE_FAILED 1; sleep 20; done
curl -s -m 5 localhost:8139/v1/models | grep -q $NAME || done_ SERVE_TIMEOUT 1
$RUN python benchmark/harness/m8_serve_check.py --family guide_match --name $NAME 2>&1 | grep -v libmamba | tail -3 | grep -q PASS || done_ SERVE_CHECK_FAILED 1

echo "== 6. eval on the FRESH slice (held-out 300-599) $(date +%T) =="
taskset -c 112-127 $RUN python -u benchmark/harness/m8_eval.py --target-fraction 0.85 --max-steps 10 --n 300 \
  --start-index 300 --families guide_match --arms trained-8b --trained-model $NAME \
  --out runs/m8/eval_match_fresh_rep2.json 2>&1 | grep -v libmamba | tail -2 || done_ EVAL_FAILED 1
done_ OK 0
