#!/usr/bin/env bash
# Second-family run: gate at the graded tolerance (1.5%), baselines, GRPO, eval.
set -u
cd /netdisk/ldq/NeutronGym
RUN=/netdisk/ldq/mcstas-run.sh
SFT=/netdisk/ldq/sft-env/bin/python
BASE=/netdisk/ldq/hf/hub/models--Qwen--Qwen3-8B/snapshots/b968826d9c46dd6066d109eabc6255188de91218
TAG=bendergrpo
CKPT_A=/netdisk/ldq/ckpt/m8-$TAG-a
CKPT_B=/netdisk/ldq/ckpt/m8-$TAG-b
NAME=qwen3-8b-m8-$TAG
CPUS=0-15,32-47,64-79,96-111
export NEUTRONGYM_VLLM_URL_8B=http://localhost:8137/v1 NEUTRONGYM_VLLM_URL_32B=http://localhost:8138/v1 NEUTRONGYM_VLLM_URL_TRAINED=http://localhost:8139/v1
done_() { echo "BENDER_GRPO_DONE $1 $(date +%T)"; exit "${2:-0}"; }
# Each prep stage skips work whose artifact already exists, so a killed run
# resumes instead of recomputing. The first bender prep died mid-step-3 when
# its ssh connection dropped (2026-09-20), and SKIP_PREP=1 would then have
# skipped the states file it never wrote. Launch detached: setsid nohup.
have() { [ -s "$1" ]; }

if [ "${SKIP_PREP:-0}" = "1" ]; then echo "== skipping steps 1-4 (already done) =="; else
echo "== 1. targets at the new tolerance $(date +%T) =="
for split in heldout train; do
  taskset -c $CPUS $RUN python benchmark/harness/precalibrate.py --family bender \
    --split $split --start 0 --n 600 --workers 48 | tail -1 || done_ PRECAL_FAILED 1
done

echo "== 2. gate at the graded 1.5% bar $(date +%T) =="
if have runs/m8/match_gate_bender_n150.json && have runs/m8/readout_probe_bender_0.85_n150.json; then
  echo "   both probe artifacts present -- skipping"
else
taskset -c $CPUS $RUN python benchmark/harness/match_gate.py --family bender --n 150 --workers 48 || done_ GATE_FAILED 1
taskset -c $CPUS $RUN python benchmark/harness/readout_probe.py --family bender --n 150 --workers 48 --target-fraction 0.85 || done_ READOUT_FAILED 1
$RUN python - <<'PY' || done_ GATE_NOT_CLEAN 1
import json
v = json.load(open("runs/m8/match_gate_bender_n150.json"))["verdict"]
print("gate:", v["best_pass_rate"], "upper", v["best_pass_rate_upper"], "ok", v["ok"])
assert v["ok"]
PY
fi

echo "== 3. baselines, held-out 300-599 $(date +%T) =="
for arm in untrained-8b untrained-32b; do
  have runs/m8/eval_bender_fresh_$arm.json && { echo "   $arm eval present -- skipping"; continue; }
  taskset -c 112-127 $RUN python -u benchmark/harness/m8_eval.py --target-fraction 0.85 --max-steps 10 \
    --n 300 --start-index 300 --families bender --arms $arm \
    --out runs/m8/eval_bender_fresh_$arm.json > runs/m8/eval_bender_fresh_$arm.log 2>&1 &
done
echo "== 4. states from the untrained 8B, train 0-299 $(date +%T) =="
if have runs/m8/grpo_states_bender.jsonl; then echo "   states present -- skipping"; else
taskset -c 16-31 $RUN python -u benchmark/harness/m8_states.py --family bender --start 0 --n 300 \
  --workers 8 --out runs/m8/grpo_states_bender.jsonl 2>&1 | grep -v libmamba | tail -2 || done_ STATES_FAILED 1
fi
wait
for arm in untrained-8b untrained-32b; do
  have runs/m8/eval_bender_fresh_$arm.json || done_ "BASELINE_FAILED_$arm" 1
  grep -v "libmamba\|Waiting\|Could not" runs/m8/eval_bender_fresh_$arm.log 2>/dev/null | tail -1
done

fi
echo "== 5. GRPO 120 @1e-5 then 100 @3e-5 $(date +%T) =="
# every trained-model server runs from /netdisk/ldq/ckpt; the base
# 8B/32B servers (Qwen/Qwen3-*) are on other GPUs and are left alone
pkill -f "vllm serve /netdisk/ldq/ckpt"
for i in $(seq 1 60); do [ $(nvidia-smi -i 7 --query-gpu=memory.used --format=csv,noheader,nounits) -lt 1000 ] && break; sleep 5; done
# ALWAYS restart: a server started before this family existed has workers
# whose generate module lacks it, and the request dies as RemoteDisconnected
pkill -f "reward_server.py --port 8199"; sleep 3
rm -f /netdisk/ldq/grpo/reward_server.log
(setsid nohup taskset -c 48-63,80-95 $RUN python benchmark/harness/reward_server.py --port 8199 --workers 32 > /netdisk/ldq/grpo/reward_server.log 2>&1 < /dev/null &)
for i in $(seq 1 90); do grep -q "reward server on" /netdisk/ldq/grpo/reward_server.log 2>/dev/null && break; sleep 2; done
CUDA_DEVICE_ORDER=PCI_BUS_ID CUDA_VISIBLE_DEVICES=7 HF_HOME=/netdisk/ldq/hf TMPDIR=/netdisk/ldq/tmp PYTORCH_CUDA_ALLOC_CONF=expandable_segments:True \
  $SFT -u benchmark/harness/m8_grpo.py --states runs/m8/grpo_states_bender.jsonl --base $BASE --out $CKPT_A \
  --family bender --steps 120 --states-per-step 8 --group 8 --lr 1e-5 --kl 0.02 \
  > runs/m8/grpo_${TAG}_a.log 2>&1 || { tail -12 runs/m8/grpo_${TAG}_a.log; done_ STAGE_A_FAILED 1; }
CUDA_DEVICE_ORDER=PCI_BUS_ID CUDA_VISIBLE_DEVICES=7 HF_HOME=/netdisk/ldq/hf TMPDIR=/netdisk/ldq/tmp PYTORCH_CUDA_ALLOC_CONF=expandable_segments:True \
  $SFT -u benchmark/harness/m8_grpo.py --states runs/m8/grpo_states_bender.jsonl --base $BASE --out $CKPT_B \
  --family bender --init-adapter $CKPT_A/adapter --start-step 120 --steps 100 \
  --states-per-step 8 --group 8 --lr 3e-5 --kl 0.01 \
  > runs/m8/grpo_${TAG}_b.log 2>&1 || { tail -12 runs/m8/grpo_${TAG}_b.log; done_ STAGE_B_FAILED 1; }
[ -f $CKPT_B/merged/config.json ] || done_ NO_MERGED 1

echo "== 6. serve + eval on held-out 300-599 $(date +%T) =="
(MODEL=$CKPT_B/merged GPU=7 PORT=8139 NAME=$NAME setsid nohup /netdisk/ldq/serve-m8-trained.sh > /netdisk/ldq/serve-m8-$TAG.log 2>&1 < /dev/null &)
for i in $(seq 1 90); do curl -s -m 5 localhost:8139/v1/models | grep -q $NAME && break; sleep 20; done
curl -s -m 5 localhost:8139/v1/models | grep -q $NAME || done_ SERVE_TIMEOUT 1
$RUN python benchmark/harness/m8_serve_check.py --family bender --name $NAME 2>&1 | grep -v libmamba | tail -3 | grep -q PASS || done_ SERVE_CHECK_FAILED 1
taskset -c 112-127 $RUN python -u benchmark/harness/m8_eval.py --target-fraction 0.85 --max-steps 10 --n 300 \
  --start-index 300 --families bender --arms trained-8b --trained-model $NAME \
  --out runs/m8/eval_bender_fresh_trained-8b.json 2>&1 | grep -v libmamba | tail -2 || done_ EVAL_FAILED 1
done_ OK 0
