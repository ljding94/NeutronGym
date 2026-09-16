#!/usr/bin/env bash
# Passing-turn SFT of Qwen3-8B on SANS, redone on calibration-v3 targets at the
# 0.85x bar (2026-09-16). Old run (1.0x, v2 targets) is kept under its own names.
set -u
cd /Users/ldq/Work/NeutronGym
PY=/opt/homebrew/Caskroom/miniconda/base/envs/mcstas/bin/python
S="-o BatchMode=yes -o ConnectTimeout=20 -o ControlMaster=no -o ControlPath=none"
BASE=/netdisk/ldq/hf/hub/models--Qwen--Qwen3-8B/snapshots/b968826d9c46dd6066d109eabc6255188de91218
FRAC=0.85
MAX_STEPS=10    # with numeric SANS hints; 6 turns kept 2/300, 10 + hints kept 17/50 in the pilot
TAG=sans085v3h
CKPT=/netdisk/ldq/ckpt/m8-raft-$TAG-pass
NAME=qwen3-8b-m8raft-$TAG-pass
MIN_EPISODES=100
MAX_KEEP_RATE=0.80      # above this the bar is at ceiling for the 8B: nothing to learn
done_() { echo "SANS_V3_PIPELINE_DONE $1 $(date +%T)"; exit "${2:-0}"; }
export NEUTRONGYM_VLLM_URL_8B=http://localhost:8137/v1 NEUTRONGYM_VLLM_URL_32B=http://localhost:8138/v1 NEUTRONGYM_VLLM_URL_TRAINED=http://localhost:8139/v1

echo "== 0. tunnel supervisor (live ports only) $(date +%T) =="
pkill -f tunnel_supervisor.sh; pkill -f "ssh.*-L 8137:localhost:8137"; sleep 1
PORTS="8137 8138" nohup runs/m8/tunnel_supervisor.sh >> runs/m8/tunnel_supervisor.log 2>&1 < /dev/null &
for i in $(seq 1 30); do curl -s -m 5 http://localhost:8137/v1/models | grep -q qwen3-8b && break; sleep 5; done
curl -s -m 5 http://localhost:8137/v1/models | grep -q qwen3-8b || done_ TUNNEL_DOWN 1

echo "== 1. targets for train 0-299, then collection batch A in background $(date +%T) =="
$PY benchmark/harness/precalibrate.py --family sans_collimation --split train --start 0 --n 300 --workers 7 | tail -1 || done_ PRECAL_A_FAILED 1
$PY -u benchmark/harness/m8_collect.py --families sans_collimation --target-fraction $FRAC --max-steps $MAX_STEPS --n 300 --start 0 \
   --out runs/m8/raft_$TAG > runs/m8/raft_$TAG.log 2>&1 &
echo "== 2. targets for train 300-599 and held-out 150-299 $(date +%T) =="
while pgrep -f "precalibrate.py --family sans_collimation --split train --start 300" >/dev/null; do sleep 30; done   # a run from the previous launch may still be filling these
$PY benchmark/harness/precalibrate.py --family sans_collimation --split train --start 300 --n 300 --workers 6 | tail -1 || done_ PRECAL_B_FAILED 1
$PY -u benchmark/harness/m8_collect.py --families sans_collimation --target-fraction $FRAC --max-steps $MAX_STEPS --n 300 --start 300 \
   --out runs/m8/raft_${TAG}_b > runs/m8/raft_${TAG}_b.log 2>&1 &
$PY benchmark/harness/precalibrate.py --family sans_collimation --split heldout --start 0 --n 300 --workers 5 | tail -1 || done_ PRECAL_HELDOUT_FAILED 1

echo "== 3. wait for both collection batches $(date +%T) =="
for LOG in runs/m8/raft_$TAG.log runs/m8/raft_${TAG}_b.log; do
  until grep -qE "train examples|Traceback" $LOG 2>/dev/null; do sleep 60; done
  grep -q Traceback $LOG && { tail -20 $LOG; done_ "COLLECTION_FAILED_$(basename $LOG)" 1; }
  tail -5 $LOG
done
mkdir -p runs/m8/raft_${TAG}_all
cat runs/m8/raft_$TAG/train.jsonl runs/m8/raft_${TAG}_b/train.jsonl > runs/m8/raft_${TAG}_all/train.jsonl
$PY -c "
import json
a=json.load(open('runs/m8/raft_$TAG/manifest.json')); b=json.load(open('runs/m8/raft_${TAG}_b/manifest.json'))
ids=[json.loads(l)['instance_id'] for l in open('runs/m8/raft_${TAG}_all/train.jsonl') if l.strip()]
assert len(ids)==len(set(ids)), 'duplicate instances across batches'
n_inst=sum(m['families']['sans_collimation']['instances'] for m in (a,b))
rate=len(ids)/n_inst
json.dump({'batches':[a,b],'train_examples':len(ids),'instances':n_inst,'keep_rate':rate,
           'target_fraction':$FRAC,'calibration':'v3','families_used':['sans_collimation']},
          open('runs/m8/raft_${TAG}_all/manifest.json','w'), indent=1)
print(f'combined SANS episodes: {len(ids)} of {n_inst} instances, keep rate {rate:.3f}')
assert rate <= $MAX_KEEP_RATE, f'keep rate {rate:.2f} > $MAX_KEEP_RATE: bar at ceiling for the untrained 8B'
" || done_ MERGE_OR_CEILING_FAILED 1
n=$(grep -c . runs/m8/raft_${TAG}_all/train.jsonl)
[ "$n" -ge "$MIN_EPISODES" ] || done_ "TOO_FEW_EPISODES_${n}_NEED_${MIN_EPISODES}" 1

echo "== 4. copy + dry run + train on GPU 7 $(date +%T) =="
scp $S benchmark/harness/m8_train.py Neutron:/netdisk/ldq/m8_train.py || done_ SCP_FAILED 1
scp $S runs/m8/raft_${TAG}_all/train.jsonl Neutron:/netdisk/ldq/m8data/train_$TAG.jsonl || done_ SCP_FAILED 1
ssh $S Neutron "bash -s" <<REMOTE || done_ REMOTE_TRAIN_LAUNCH_FAILED 1
set -e
cd /netdisk/ldq
HF_HOME=/netdisk/ldq/hf TMPDIR=/netdisk/ldq/tmp /netdisk/ldq/sft-env/bin/python m8_train.py \
  --data m8data/train_$TAG.jsonl --base $BASE --out /netdisk/ldq/ckpt/m8-dryrun-$TAG \
  --turns passing --check-server http://localhost:8137/v1 --dry-run 2>&1 | grep -v -i warn | tail -2
used=\$(nvidia-smi -i 7 --query-gpu=memory.used --format=csv,noheader,nounits)
[ "\$used" -lt 1000 ] || { echo "GPU 7 busy"; exit 1; }
[ -e $CKPT ] && { echo "checkpoint dir exists"; exit 1; }
nohup env CUDA_DEVICE_ORDER=PCI_BUS_ID CUDA_VISIBLE_DEVICES=7 HF_HOME=/netdisk/ldq/hf TMPDIR=/netdisk/ldq/tmp \
  /netdisk/ldq/sft-env/bin/python -u m8_train.py --data m8data/train_$TAG.jsonl --base $BASE --out $CKPT \
  --turns passing --epochs 2 --tokens-per-step 16000 --rank 32 --alpha 64 --lr 1e-4 \
  --check-server http://localhost:8137/v1 > /netdisk/ldq/m8-train-$TAG.log 2>&1 < /dev/null &
echo "training launched"
REMOTE

echo "== 5. wait for merge, then serve on GPU 7 :8139 $(date +%T) =="
deadline=$(( $(date +%s) + 3600 ))
until ssh $S Neutron "grep -qE '^-> merged|Traceback|MISMATCH|out of memory' /netdisk/ldq/m8-train-$TAG.log"; do
  [ $(date +%s) -gt $deadline ] && done_ TRAINING_TIMEOUT 1; sleep 30
done
ssh $S Neutron "grep -qE 'Traceback|MISMATCH|out of memory' /netdisk/ldq/m8-train-$TAG.log" && { ssh $S Neutron "tail -20 /netdisk/ldq/m8-train-$TAG.log"; done_ TRAINING_FAILED 1; }
ssh $S Neutron "grep -E '^\{.step|^-> |data:' /netdisk/ldq/m8-train-$TAG.log | tail -4"
mkdir -p runs/m8/train/$TAG
scp $S Neutron:$CKPT/train_log.jsonl Neutron:$CKPT/data_stats.json Neutron:/netdisk/ldq/m8-train-$TAG.log runs/m8/train/$TAG/
ssh $S Neutron "bash -s" <<REMOTE || done_ SERVE_LAUNCH_FAILED 1
for i in \$(seq 1 60); do [ \$(nvidia-smi -i 7 --query-gpu=memory.used --format=csv,noheader,nounits) -lt 1000 ] && break; sleep 5; done
ss -ltn | grep -q ":8139 " && { echo "port 8139 busy"; exit 1; }
[ -f $CKPT/merged/config.json ] || { echo "no merged config"; exit 1; }
MODEL=$CKPT/merged GPU=7 PORT=8139 NAME=$NAME nohup /netdisk/ldq/serve-m8-trained.sh > /netdisk/ldq/serve-m8-$TAG.log 2>&1 < /dev/null &
echo "vllm launching"
REMOTE
deadline=$(( $(date +%s) + 1800 ))
until ssh $S Neutron "curl -s -m 5 http://localhost:8139/v1/models | grep -q $NAME"; do
  ssh $S Neutron "grep -q 'Engine core initialization failed' /netdisk/ldq/serve-m8-$TAG.log" && done_ SERVE_FAILED 1
  [ $(date +%s) -gt $deadline ] && done_ SERVE_TIMEOUT 1; sleep 20
done
echo "trained SANS model serving on DGX :8139"

echo "== 6. tunnel supervisor now also watches 8139 =="
pkill -f tunnel_supervisor.sh; sleep 1
PORTS="8137 8138 8139" nohup runs/m8/tunnel_supervisor.sh >> runs/m8/tunnel_supervisor.log 2>&1 < /dev/null &
for i in $(seq 1 30); do curl -s -m 5 http://localhost:8139/v1/models | grep -q $NAME && break; sleep 5; done
curl -s -m 5 http://localhost:8139/v1/models | grep -q $NAME || done_ LOCAL_8139_UNREACHABLE 1

echo "== 7. serve check =="
$PY benchmark/harness/m8_serve_check.py --family sans_collimation --name $NAME || done_ SERVE_CHECK_FAILED 1

echo "== 8. eval n=300 held-out SANS at $FRAC, three arms fresh $(date +%T) =="
restarts_before=$(grep -c "re)started" runs/m8/tunnel_supervisor.log)
$PY -u benchmark/harness/m8_eval.py --target-fraction $FRAC --max-steps $MAX_STEPS --n 300 --families sans_collimation \
  --trained-model $NAME --out runs/m8/eval_${TAG}_n300_passing.json || done_ EVAL_FAILED 1
echo "tunnel restarts during eval: $(( $(grep -c "re)started" runs/m8/tunnel_supervisor.log) - restarts_before ))"

echo "== 9. no-model constant probe on the same 300 instances =="
$PY -u benchmark/harness/m8_constant_probe.py --family sans_collimation --grid \
  --data runs/m8/raft_${TAG}_all/train.jsonl --eval runs/m8/eval_${TAG}_n300_passing.json --n 300 \
  --out runs/m8/constant_probe_${TAG}_n300.json 2>&1 | grep -E "^best single|^any of|^->" || done_ CONSTANT_PROBE_FAILED 1
done_ OK 0
