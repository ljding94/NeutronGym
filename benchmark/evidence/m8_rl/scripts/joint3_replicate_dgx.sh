#!/usr/bin/env bash
# Replication of the THREE-family joint run at a different seed (2026-09-22).
#
# Why: the paper says pooling "erases learning on the weakest-signal family",
# and that rests on one run. In it sans_match reached 24.0% -- 83% instance
# agreement with the UNTRAINED model, i.e. it barely moved. The four-family
# run then reached 43.3% on the same family with LESS budget per family, and
# the two models' passing sets overlap at Jaccard 0.13 despite near-identical
# training data. Either three-family pooling really does starve sans_match
# (and four-family pooling rescues it), or the outcome is unstable across
# runs and the written claim is over-read from n=1.
#
# Same families, same states, same budget and hyperparameters as jointgrpo.
# ONLY the seed differs, so a difference in outcome is run-to-run variance by
# construction.
#
# Waits for the four-family run to finish before touching GPU 7 -- its eval
# loop is still being served from there.
#
# Launch detached:
#   ssh Neutron 'cd /netdisk/ldq/NeutronGym && setsid nohup bash \
#     runs/m8/joint3_replicate_dgx.sh > runs/m8/joint3rep.log 2>&1 < /dev/null &'
set -u
cd /netdisk/ldq/NeutronGym
RUN=/netdisk/ldq/mcstas-run.sh
SFT=/netdisk/ldq/sft-env/bin/python
BASE=/netdisk/ldq/hf/hub/models--Qwen--Qwen3-8B/snapshots/b968826d9c46dd6066d109eabc6255188de91218
TAG=joint3rep
SEED=20260922
CKPT_A=/netdisk/ldq/ckpt/m8-$TAG-a
CKPT_B=/netdisk/ldq/ckpt/m8-$TAG-b
NAME=qwen3-8b-m8-$TAG
FAMS=guide_match,sans_match,tof_chopper
STATES=runs/m8/grpo_states_match.jsonl,runs/m8/grpo_states_sansmatch.jsonl,runs/m8/grpo_states_tof.jsonl
export NEUTRONGYM_VLLM_URL_8B=http://localhost:8137/v1 NEUTRONGYM_VLLM_URL_TRAINED=http://localhost:8139/v1
done_() { echo "JOINT3REP_DONE $1 $(date +%T)"; exit "${2:-0}"; }

echo "== 0. wait for the four-family run to release GPU 7 $(date +%T) =="
# up to 6 h; its eval loop is served from GPU 7, so killing that server early
# would corrupt the run already in flight
for i in $(seq 1 2160); do
  grep -aq "JOINT4_DONE" runs/m8/joint4.log 2>/dev/null && break
  pgrep -f "bash runs/m8/joint4_grpo_dgx.sh" > /dev/null || break   # died, don't wait forever
  sleep 10
done
grep -aq "JOINT4_DONE" runs/m8/joint4.log 2>/dev/null \
  && echo "   four-family run finished: $(grep -a JOINT4_DONE runs/m8/joint4.log | tail -1)" \
  || echo "   four-family run no longer present; proceeding"

echo "== 1. free GPU 7, restart reward server $(date +%T) =="
pkill -f "vllm serve /netdisk/ldq/ckpt"
for i in $(seq 1 60); do [ $(nvidia-smi -i 7 --query-gpu=memory.used --format=csv,noheader,nounits) -lt 2000 ] && break; sleep 5; done
[ $(nvidia-smi -i 7 --query-gpu=memory.used --format=csv,noheader,nounits) -lt 2000 ] || done_ GPU7_BUSY 1
pkill -f "reward_server.py --port 8199"; sleep 3
rm -f /netdisk/ldq/grpo/reward_server.log
(setsid nohup taskset -c 48-63,80-95 $RUN python benchmark/harness/reward_server.py --port 8199 --workers 32 > /netdisk/ldq/grpo/reward_server.log 2>&1 < /dev/null &)
for i in $(seq 1 90); do grep -q "reward server on" /netdisk/ldq/grpo/reward_server.log 2>/dev/null && break; sleep 2; done
grep -q "reward server on" /netdisk/ldq/grpo/reward_server.log || done_ REWARD_SERVER_FAILED 1

echo "== 2. stage A: 120 steps lr 1e-5, seed $SEED $(date +%T) =="
CUDA_DEVICE_ORDER=PCI_BUS_ID CUDA_VISIBLE_DEVICES=7 HF_HOME=/netdisk/ldq/hf TMPDIR=/netdisk/ldq/tmp PYTORCH_CUDA_ALLOC_CONF=expandable_segments:True \
  $SFT -u benchmark/harness/m8_grpo.py --states $STATES --base $BASE --out $CKPT_A \
  --family $FAMS --steps 120 --states-per-step 8 --group 8 --lr 1e-5 --kl 0.02 --seed $SEED \
  > runs/m8/grpo_${TAG}_a.log 2>&1 || { tail -12 runs/m8/grpo_${TAG}_a.log; done_ STAGE_A_FAILED 1; }
head -1 runs/m8/grpo_${TAG}_a.log

echo "== 3. stage B: 100 steps lr 3e-5, seed $SEED $(date +%T) =="
CUDA_DEVICE_ORDER=PCI_BUS_ID CUDA_VISIBLE_DEVICES=7 HF_HOME=/netdisk/ldq/hf TMPDIR=/netdisk/ldq/tmp PYTORCH_CUDA_ALLOC_CONF=expandable_segments:True \
  $SFT -u benchmark/harness/m8_grpo.py --states $STATES --base $BASE --out $CKPT_B \
  --family $FAMS --init-adapter $CKPT_A/adapter --start-step 120 --steps 100 \
  --states-per-step 8 --group 8 --lr 3e-5 --kl 0.01 --seed $SEED \
  > runs/m8/grpo_${TAG}_b.log 2>&1 || { tail -12 runs/m8/grpo_${TAG}_b.log; done_ STAGE_B_FAILED 1; }
[ -f $CKPT_B/merged/config.json ] || done_ NO_MERGED 1

echo "== 4. serve $(date +%T) =="
(MODEL=$CKPT_B/merged GPU=7 PORT=8139 NAME=$NAME setsid nohup /netdisk/ldq/serve-m8-trained.sh > /netdisk/ldq/serve-m8-$TAG.log 2>&1 < /dev/null &)
for i in $(seq 1 90); do curl -s -m 5 localhost:8139/v1/models | grep -q $NAME && break; sleep 20; done
curl -s -m 5 localhost:8139/v1/models | grep -q $NAME || done_ SERVE_TIMEOUT 1

# sans_match FIRST: it is the family the claim is about, so the decisive
# number lands ~40 min before the other two rather than last.
echo "== 5. evaluate, sans_match first $(date +%T) =="
for fam in sans_match tof_chopper guide_match; do
  taskset -c 112-127 $RUN python -u benchmark/harness/m8_eval.py --target-fraction 0.85 --max-steps 10 \
    --n 300 --start-index 300 --families $fam --arms trained-8b --trained-model $NAME \
    --out runs/m8/eval_joint3rep_${fam}.json > runs/m8/eval_joint3rep_${fam}.log 2>&1
  [ -s runs/m8/eval_joint3rep_${fam}.json ] || done_ "EVAL_FAILED_${fam}" 1
  grep -v "libmamba\|Waiting\|Could not" runs/m8/eval_joint3rep_${fam}.log | tail -1
done
done_ OK 0
