#!/usr/bin/env bash
# Joint multi-family GRPO (2026-09-20): one policy trained on all three gated
# families, then evaluated on each against the three specialists. Tests whether
# the three results are separate overfits or one transferable design policy.
set -u
cd /netdisk/ldq/NeutronGym
RUN=/netdisk/ldq/mcstas-run.sh
SFT=/netdisk/ldq/sft-env/bin/python
BASE=/netdisk/ldq/hf/hub/models--Qwen--Qwen3-8B/snapshots/b968826d9c46dd6066d109eabc6255188de91218
TAG=jointgrpo
CKPT_A=/netdisk/ldq/ckpt/m8-$TAG-a
CKPT_B=/netdisk/ldq/ckpt/m8-$TAG-b
NAME=qwen3-8b-m8-$TAG
FAMS=guide_match,sans_match,tof_chopper
STATES=runs/m8/grpo_states_match.jsonl,runs/m8/grpo_states_sansmatch.jsonl,runs/m8/grpo_states_tof.jsonl
export NEUTRONGYM_VLLM_URL_8B=http://localhost:8137/v1 NEUTRONGYM_VLLM_URL_TRAINED=http://localhost:8139/v1
done_() { echo "JOINT_DONE $1 $(date +%T)"; exit "${2:-0}"; }

echo "== 1. free GPU 7, restart reward server $(date +%T) =="
pkill -f "vllm serve /netdisk/ldq/ckpt"
for i in $(seq 1 60); do [ $(nvidia-smi -i 7 --query-gpu=memory.used --format=csv,noheader,nounits) -lt 1000 ] && break; sleep 5; done
[ $(nvidia-smi -i 7 --query-gpu=memory.used --format=csv,noheader,nounits) -lt 1000 ] || done_ GPU7_BUSY 1
pkill -f "reward_server.py --port 8199"; sleep 3
(setsid nohup taskset -c 48-63,80-95 $RUN python benchmark/harness/reward_server.py --port 8199 --workers 32 > /netdisk/ldq/grpo/reward_server.log 2>&1 < /dev/null &)
for i in $(seq 1 90); do grep -q "reward server on" /netdisk/ldq/grpo/reward_server.log 2>/dev/null && break; sleep 2; done
grep -q "reward server on" /netdisk/ldq/grpo/reward_server.log || done_ REWARD_SERVER_FAILED 1

echo "== 2. stage A: 120 steps lr 1e-5 on pooled states $(date +%T) =="
CUDA_DEVICE_ORDER=PCI_BUS_ID CUDA_VISIBLE_DEVICES=7 HF_HOME=/netdisk/ldq/hf TMPDIR=/netdisk/ldq/tmp PYTORCH_CUDA_ALLOC_CONF=expandable_segments:True \
  $SFT -u benchmark/harness/m8_grpo.py --states $STATES --base $BASE --out $CKPT_A \
  --family $FAMS --steps 120 --states-per-step 8 --group 8 --lr 1e-5 --kl 0.02 \
  > runs/m8/grpo_${TAG}_a.log 2>&1 || { tail -12 runs/m8/grpo_${TAG}_a.log; done_ STAGE_A_FAILED 1; }
head -1 runs/m8/grpo_${TAG}_a.log

echo "== 3. stage B: 100 steps lr 3e-5 $(date +%T) =="
CUDA_DEVICE_ORDER=PCI_BUS_ID CUDA_VISIBLE_DEVICES=7 HF_HOME=/netdisk/ldq/hf TMPDIR=/netdisk/ldq/tmp PYTORCH_CUDA_ALLOC_CONF=expandable_segments:True \
  $SFT -u benchmark/harness/m8_grpo.py --states $STATES --base $BASE --out $CKPT_B \
  --family $FAMS --init-adapter $CKPT_A/adapter --start-step 120 --steps 100 \
  --states-per-step 8 --group 8 --lr 3e-5 --kl 0.01 \
  > runs/m8/grpo_${TAG}_b.log 2>&1 || { tail -12 runs/m8/grpo_${TAG}_b.log; done_ STAGE_B_FAILED 1; }
[ -f $CKPT_B/merged/config.json ] || done_ NO_MERGED 1

echo "== 4. serve $(date +%T) =="
(MODEL=$CKPT_B/merged GPU=7 PORT=8139 NAME=$NAME setsid nohup /netdisk/ldq/serve-m8-trained.sh > /netdisk/ldq/serve-m8-$TAG.log 2>&1 < /dev/null &)
for i in $(seq 1 90); do curl -s -m 5 localhost:8139/v1/models | grep -q $NAME && break; sleep 20; done
curl -s -m 5 localhost:8139/v1/models | grep -q $NAME || done_ SERVE_TIMEOUT 1

echo "== 5. evaluate on all three families, held-out 300-599 $(date +%T) =="
for fam in guide_match sans_match tof_chopper; do
  taskset -c 112-127 $RUN python -u benchmark/harness/m8_eval.py --target-fraction 0.85 --max-steps 10 \
    --n 300 --start-index 300 --families $fam --arms trained-8b --trained-model $NAME \
    --out runs/m8/eval_joint_${fam}.json > runs/m8/eval_joint_${fam}.log 2>&1
  grep -v "libmamba\|Waiting\|Could not" runs/m8/eval_joint_${fam}.log | tail -1
done
done_ OK 0
