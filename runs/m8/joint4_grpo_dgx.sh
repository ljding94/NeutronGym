#!/usr/bin/env bash
# Joint multi-family GRPO over ALL FOUR gated families (2026-09-22).
#
# The three-family run (TAG=jointgrpo) predates bender, so the paper has to
# scope its joint claim to three. This repeats it over four at the SAME
# 220-step budget one specialist gets -- a quarter of the steps per family
# rather than a third -- so both comparisons stay clean:
#   vs the specialists   (same total budget, more families)
#   vs the 3-family run  (same total budget, one more family)
#
# Writes to its own TAG and its own eval files, so the three-family evidence
# already in the paper is never overwritten.
#
# Evaluates every family on held-out 300-599, matching the three-family run.
# That slice is unbiased for a joint model: the step-220 checkpoint is taken
# without per-family selection, so no slice chose it. (sans_match's own
# SPECIALIST is reported on 600-899 for a different reason -- selection over
# its checkpoint curve used 300-599.)
#
# Launch detached, or it dies with its ssh connection:
#   ssh Neutron 'cd /netdisk/ldq/NeutronGym && setsid nohup bash \
#     runs/m8/joint4_grpo_dgx.sh > runs/m8/joint4.log 2>&1 < /dev/null &'
set -u
cd /netdisk/ldq/NeutronGym
RUN=/netdisk/ldq/mcstas-run.sh
SFT=/netdisk/ldq/sft-env/bin/python
BASE=/netdisk/ldq/hf/hub/models--Qwen--Qwen3-8B/snapshots/b968826d9c46dd6066d109eabc6255188de91218
TAG=joint4grpo
CKPT_A=/netdisk/ldq/ckpt/m8-$TAG-a
CKPT_B=/netdisk/ldq/ckpt/m8-$TAG-b
NAME=qwen3-8b-m8-$TAG
FAMS=guide_match,sans_match,tof_chopper,bender
STATES=runs/m8/grpo_states_match.jsonl,runs/m8/grpo_states_sansmatch.jsonl,runs/m8/grpo_states_tof.jsonl,runs/m8/grpo_states_bender.jsonl
export NEUTRONGYM_VLLM_URL_8B=http://localhost:8137/v1 NEUTRONGYM_VLLM_URL_TRAINED=http://localhost:8139/v1
done_() { echo "JOINT4_DONE $1 $(date +%T)"; exit "${2:-0}"; }

echo "== 0. preflight $(date +%T) =="
# bender's states must come from the REPAIRED family. v1's were collected
# before the hidden_filter existed, and training on them would reproduce the
# retracted result.
$RUN python - <<'PY' || done_ BENDER_STATES_STALE 1
import json
from neutrongym import generate
sig = generate.family_signature("bender")
ep = json.loads(open("runs/m8/grpo_states_bender.jsonl").readline())
assert ep["family"] == "bender" and ep["split"] == "train", ep
# the v2 filter is what makes the family sound; assert it is registered
assert generate.FAMILIES["bender"].get("hidden_filter") == "bender_hidden_ok"
print(f"  bender signature {sig}, states look like v2")
PY
for f in ${STATES//,/ }; do
  n=$(wc -l < "$f")
  [ "$n" -eq 300 ] || done_ "STATES_WRONG_LENGTH_${f}_${n}" 1
  echo "  $f: $n episodes"
done

echo "== 1. free GPU 7, restart reward server $(date +%T) =="
# kills only OUR trained-checkpoint servers; the base 8B/32B servers and any
# other user's job on this box are left alone
pkill -f "vllm serve /netdisk/ldq/ckpt"
for i in $(seq 1 60); do [ $(nvidia-smi -i 7 --query-gpu=memory.used --format=csv,noheader,nounits) -lt 2000 ] && break; sleep 5; done
[ $(nvidia-smi -i 7 --query-gpu=memory.used --format=csv,noheader,nounits) -lt 2000 ] || done_ GPU7_BUSY 1
pkill -f "reward_server.py --port 8199"; sleep 3
rm -f /netdisk/ldq/grpo/reward_server.log
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
$RUN python benchmark/harness/m8_serve_check.py --family bender --name $NAME 2>&1 | grep -v libmamba | tail -3 | grep -q PASS || done_ SERVE_CHECK_FAILED 1

echo "== 5. evaluate on all four families, held-out 300-599 $(date +%T) =="
for fam in guide_match sans_match tof_chopper bender; do
  taskset -c 112-127 $RUN python -u benchmark/harness/m8_eval.py --target-fraction 0.85 --max-steps 10 \
    --n 300 --start-index 300 --families $fam --arms trained-8b --trained-model $NAME \
    --out runs/m8/eval_joint4_${fam}.json > runs/m8/eval_joint4_${fam}.log 2>&1
  [ -s runs/m8/eval_joint4_${fam}.json ] || done_ "EVAL_FAILED_${fam}" 1
  grep -v "libmamba\|Waiting\|Could not" runs/m8/eval_joint4_${fam}.log | tail -1
done
done_ OK 0
