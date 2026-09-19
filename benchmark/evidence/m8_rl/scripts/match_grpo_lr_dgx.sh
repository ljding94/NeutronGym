#!/usr/bin/env bash
# guide_match GRPO continuation at 3x lr, weaker KL: steps 121-220 (2026-09-17).
# Steps 121-180 at lr 1e-5 plateaued (reward ~0.85, sampled pass 6-8%, KL flat), so
# this separates a learning-rate plateau from the task ceiling.
set -u
cd /netdisk/ldq/NeutronGym
RUN=/netdisk/ldq/mcstas-run.sh
SFT=/netdisk/ldq/sft-env/bin/python
BASE=/netdisk/ldq/hf/hub/models--Qwen--Qwen3-8B/snapshots/b968826d9c46dd6066d109eabc6255188de91218
PREV=/netdisk/ldq/ckpt/m8-match05grpo
TAG=match05grpoLR
CKPT=/netdisk/ldq/ckpt/m8-$TAG
NAME=qwen3-8b-m8-$TAG
export NEUTRONGYM_VLLM_URL_8B=http://localhost:8137/v1 NEUTRONGYM_VLLM_URL_32B=http://localhost:8138/v1 NEUTRONGYM_VLLM_URL_TRAINED=http://localhost:8139/v1
done_() { echo "MATCH_RESUME_DONE $1 $(date +%T)"; exit "${2:-0}"; }
[ -d $PREV/adapter_step120 ] || done_ NO_STEP120_ADAPTER 1
[ -e $CKPT ] && done_ CKPT_EXISTS 1

echo "== 1. free GPU 7 (stop the 120-step model server) $(date +%T) =="
for n in qwen3-8b-m8-match05grpo qwen3-8b-m8-match05grpo360; do pkill -f "served-model-name $n"; done
for i in $(seq 1 60); do [ $(nvidia-smi -i 7 --query-gpu=memory.used --format=csv,noheader,nounits) -lt 1000 ] && break; sleep 5; done
[ $(nvidia-smi -i 7 --query-gpu=memory.used --format=csv,noheader,nounits) -lt 1000 ] || done_ GPU7_BUSY 1

echo "== 2. reward server $(date +%T) =="
pgrep -f "reward_server.py --port 8199" > /dev/null || \
  (setsid nohup taskset -c 48-63,80-95 $RUN python benchmark/harness/reward_server.py --port 8199 --workers 32 > /netdisk/ldq/grpo/reward_server.log 2>&1 < /dev/null &)
for i in $(seq 1 90); do grep -q "reward server on" /netdisk/ldq/grpo/reward_server.log 2>/dev/null && break; sleep 2; done

echo "== 3. GRPO steps 121-360 $(date +%T) =="
CUDA_DEVICE_ORDER=PCI_BUS_ID CUDA_VISIBLE_DEVICES=7 HF_HOME=/netdisk/ldq/hf TMPDIR=/netdisk/ldq/tmp PYTORCH_CUDA_ALLOC_CONF=expandable_segments:True \
  $SFT -u benchmark/harness/m8_grpo.py --states runs/m8/grpo_states_match.jsonl --base $BASE --out $CKPT \
  --family guide_match --init-adapter $PREV/adapter_step120 --start-step 120 --steps 100 \
  --states-per-step 8 --group 8 --lr 3e-5 --kl 0.01 --save-every 25 > runs/m8/grpo_$TAG.log 2>&1 \
  || { tail -20 runs/m8/grpo_$TAG.log; done_ TRAINING_FAILED 1; }
grep -E "^\{" runs/m8/grpo_$TAG.log | tail -2
[ -f $CKPT/merged/config.json ] || done_ NO_MERGED 1

echo "== 4. serve + check $(date +%T) =="
ss -ltn | grep -q ":8139 " && done_ PORT_8139_BUSY 1
(MODEL=$CKPT/merged GPU=7 PORT=8139 NAME=$NAME setsid nohup /netdisk/ldq/serve-m8-trained.sh > /netdisk/ldq/serve-m8-$TAG.log 2>&1 < /dev/null &)
for i in $(seq 1 90); do curl -s -m 5 localhost:8139/v1/models | grep -q $NAME && break
  grep -q "Engine core initialization failed" /netdisk/ldq/serve-m8-$TAG.log && done_ SERVE_FAILED 1; sleep 20; done
curl -s -m 5 localhost:8139/v1/models | grep -q $NAME || done_ SERVE_TIMEOUT 1
$RUN python benchmark/harness/m8_serve_check.py --family guide_match --name $NAME 2>&1 | grep -v libmamba | tail -3 | grep -q PASS || done_ SERVE_CHECK_FAILED 1

echo "== 5. eval n=300 $(date +%T) =="
taskset -c 112-127 $RUN python -u benchmark/harness/m8_eval.py --target-fraction 0.85 --max-steps 10 --n 300 \
  --families guide_match --arms trained-8b --trained-model $NAME --out runs/m8/eval_${TAG}_arm_trained-8b.json 2>&1 \
  | grep -v libmamba | tail -2 || done_ EVAL_FAILED 1
$RUN python - <<PY || done_ COMBINE_FAILED 1
import json
arms = {"untrained-8b": "runs/m8/eval_match_arm_untrained-8b.json",
        "untrained-32b": "runs/m8/eval_match_arm_untrained-32b.json",
        "trained-8b": "runs/m8/eval_${TAG}_arm_trained-8b.json"}
recs = {a: json.load(open(p)) for a, p in arms.items()}
base = dict(recs["trained-8b"]); base["heldout"], base["arms"] = {}, {}
for a, r in recs.items():
    for k in ("target_fraction", "n_per_family", "max_steps", "split", "temperature"):
        assert r[k] == base[k], (a, k)
    rows = r["heldout"][a]["guide_match"]["rows"]
    assert sum(x.get("error") is not None for x in rows) == 0
    print(a, "rows", len(rows))
    base["heldout"][a] = r["heldout"][a]
json.dump(base, open("runs/m8/eval_${TAG}_arms_combined.json", "w"))
PY
$RUN python -u benchmark/harness/m8_eval.py --target-fraction 0.85 --max-steps 10 --n 300 --families guide_match \
  --arms untrained-8b,trained-8b,untrained-32b --trained-model $NAME \
  --reuse runs/m8/eval_${TAG}_arms_combined.json --reuse-arms untrained-8b,trained-8b,untrained-32b \
  --out runs/m8/eval_${TAG}_n300.json 2>&1 | grep -v libmamba | tail -9 || done_ VERDICT_FAILED 1
done_ OK 0
