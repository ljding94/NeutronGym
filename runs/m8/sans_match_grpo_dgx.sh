#!/usr/bin/env bash
# Second-family run: gate at the graded tolerance (1.5%), baselines, GRPO, eval.
set -u
cd /netdisk/ldq/NeutronGym
RUN=/netdisk/ldq/mcstas-run.sh
SFT=/netdisk/ldq/sft-env/bin/python
BASE=/netdisk/ldq/hf/hub/models--Qwen--Qwen3-8B/snapshots/b968826d9c46dd6066d109eabc6255188de91218
TAG=sansmatchgrpo
CKPT_A=/netdisk/ldq/ckpt/m8-$TAG-a
CKPT_B=/netdisk/ldq/ckpt/m8-$TAG-b
NAME=qwen3-8b-m8-$TAG
CPUS=0-15,32-47,64-79,96-111
export NEUTRONGYM_VLLM_URL_8B=http://localhost:8137/v1 NEUTRONGYM_VLLM_URL_32B=http://localhost:8138/v1 NEUTRONGYM_VLLM_URL_TRAINED=http://localhost:8139/v1
done_() { echo "SANSMATCH_GRPO_DONE $1 $(date +%T)"; exit "${2:-0}"; }

echo "== 1. targets at the new tolerance $(date +%T) =="
for split in heldout train; do
  taskset -c $CPUS $RUN python benchmark/harness/precalibrate.py --family sans_match \
    --split $split --start 0 --n 600 --workers 48 | tail -1 || done_ PRECAL_FAILED 1
done

echo "== 2. gate at the graded 1.5% bar $(date +%T) =="
taskset -c $CPUS $RUN python benchmark/harness/match_gate.py --family sans_match --n 150 --workers 48 || done_ GATE_FAILED 1
taskset -c $CPUS $RUN python benchmark/harness/readout_probe.py --family sans_match --n 150 --workers 48 --target-fraction 0.85 || done_ READOUT_FAILED 1
$RUN python - <<'PY' || done_ GATE_NOT_CLEAN 1
import json
v = json.load(open("runs/m8/match_gate_sans_match_n150.json"))["verdict"]
print("gate:", v["best_pass_rate"], "upper", v["best_pass_rate_upper"], "ok", v["ok"])
assert v["ok"]
PY

echo "== 3. baselines, held-out 300-599 $(date +%T) =="
for arm in untrained-8b untrained-32b; do
  taskset -c 112-127 $RUN python -u benchmark/harness/m8_eval.py --target-fraction 0.85 --max-steps 10 \
    --n 300 --start-index 300 --families sans_match --arms $arm \
    --out runs/m8/eval_sansmatch_fresh_$arm.json > runs/m8/eval_sansmatch_fresh_$arm.log 2>&1 &
done
echo "== 4. states from the untrained 8B, train 0-299 $(date +%T) =="
taskset -c 16-31 $RUN python -u benchmark/harness/m8_states.py --family sans_match --start 0 --n 300 \
  --workers 8 --out runs/m8/grpo_states_sansmatch.jsonl 2>&1 | grep -v libmamba | tail -2 || done_ STATES_FAILED 1
wait
for arm in untrained-8b untrained-32b; do grep -v "libmamba\|Waiting\|Could not" runs/m8/eval_sansmatch_fresh_$arm.log | tail -1; done

echo "== 5. GRPO 120 @1e-5 then 100 @3e-5 $(date +%T) =="
for n in qwen3-8b-m8-match05grpoLR qwen3-8b-m8-matchsparse qwen3-8b-m8-matchrep2; do pkill -f "served-model-name $n"; done
for i in $(seq 1 60); do [ $(nvidia-smi -i 7 --query-gpu=memory.used --format=csv,noheader,nounits) -lt 1000 ] && break; sleep 5; done
pgrep -f "reward_server.py --port 8199" > /dev/null || \
  (setsid nohup taskset -c 48-63,80-95 $RUN python benchmark/harness/reward_server.py --port 8199 --workers 32 > /netdisk/ldq/grpo/reward_server.log 2>&1 < /dev/null &)
for i in $(seq 1 90); do grep -q "reward server on" /netdisk/ldq/grpo/reward_server.log 2>/dev/null && break; sleep 2; done
CUDA_DEVICE_ORDER=PCI_BUS_ID CUDA_VISIBLE_DEVICES=7 HF_HOME=/netdisk/ldq/hf TMPDIR=/netdisk/ldq/tmp PYTORCH_CUDA_ALLOC_CONF=expandable_segments:True \
  $SFT -u benchmark/harness/m8_grpo.py --states runs/m8/grpo_states_sansmatch.jsonl --base $BASE --out $CKPT_A \
  --family sans_match --steps 120 --states-per-step 8 --group 8 --lr 1e-5 --kl 0.02 \
  > runs/m8/grpo_${TAG}_a.log 2>&1 || { tail -12 runs/m8/grpo_${TAG}_a.log; done_ STAGE_A_FAILED 1; }
CUDA_DEVICE_ORDER=PCI_BUS_ID CUDA_VISIBLE_DEVICES=7 HF_HOME=/netdisk/ldq/hf TMPDIR=/netdisk/ldq/tmp PYTORCH_CUDA_ALLOC_CONF=expandable_segments:True \
  $SFT -u benchmark/harness/m8_grpo.py --states runs/m8/grpo_states_sansmatch.jsonl --base $BASE --out $CKPT_B \
  --family sans_match --init-adapter $CKPT_A/adapter --start-step 120 --steps 100 \
  --states-per-step 8 --group 8 --lr 3e-5 --kl 0.01 \
  > runs/m8/grpo_${TAG}_b.log 2>&1 || { tail -12 runs/m8/grpo_${TAG}_b.log; done_ STAGE_B_FAILED 1; }
[ -f $CKPT_B/merged/config.json ] || done_ NO_MERGED 1

echo "== 6. serve + eval on held-out 300-599 $(date +%T) =="
(MODEL=$CKPT_B/merged GPU=7 PORT=8139 NAME=$NAME setsid nohup /netdisk/ldq/serve-m8-trained.sh > /netdisk/ldq/serve-m8-$TAG.log 2>&1 < /dev/null &)
for i in $(seq 1 90); do curl -s -m 5 localhost:8139/v1/models | grep -q $NAME && break; sleep 20; done
curl -s -m 5 localhost:8139/v1/models | grep -q $NAME || done_ SERVE_TIMEOUT 1
$RUN python benchmark/harness/m8_serve_check.py --family sans_match --name $NAME 2>&1 | grep -v libmamba | tail -3 | grep -q PASS || done_ SERVE_CHECK_FAILED 1
taskset -c 112-127 $RUN python -u benchmark/harness/m8_eval.py --target-fraction 0.85 --max-steps 10 --n 300 \
  --start-index 300 --families sans_match --arms trained-8b --trained-model $NAME \
  --out runs/m8/eval_sansmatch_fresh_trained-8b.json 2>&1 | grep -v libmamba | tail -2 || done_ EVAL_FAILED 1
done_ OK 0
