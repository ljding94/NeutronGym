#!/usr/bin/env bash
# M8 step-level GRPO on guide_match (target matching), end to end on the DGX (2026-09-17).
set -u
cd /netdisk/ldq/NeutronGym
RUN=/netdisk/ldq/mcstas-run.sh
SFT=/netdisk/ldq/sft-env/bin/python
BASE=/netdisk/ldq/hf/hub/models--Qwen--Qwen3-8B/snapshots/b968826d9c46dd6066d109eabc6255188de91218
TAG=match05grpo
CKPT=/netdisk/ldq/ckpt/m8-$TAG
NAME=qwen3-8b-m8-$TAG
CAL=/netdisk/ldq/mcstas-mcp-home/families/guide_match/calibration_v3/29acec482e
export NEUTRONGYM_VLLM_URL_8B=http://localhost:8137/v1 NEUTRONGYM_VLLM_URL_32B=http://localhost:8138/v1 NEUTRONGYM_VLLM_URL_TRAINED=http://localhost:8139/v1
done_() { echo "MATCH_GRPO_DONE $1 $(date +%T)"; exit "${2:-0}"; }

echo "== 1. wait for guide train targets 0-299 $(date +%T) =="
until [ $(ls $CAL 2>/dev/null | grep -c "train-000[0-2]") -ge 300 ]; do sleep 60; done

echo "== 2. collect all untrained-8B episodes, train 0-299 $(date +%T) =="
taskset -c 112-127 $RUN python -u benchmark/harness/m8_states.py --family guide_match --start 0 --n 300 \
  --workers 8 --out runs/m8/grpo_states_match.jsonl 2>&1 | grep -v libmamba | tail -3 || done_ STATES_FAILED 1

echo "== 3. reward server + GRPO on GPU 7 $(date +%T) =="
pkill -f "reward_server.py --port 8199"; sleep 2
(setsid nohup taskset -c 48-63,80-95 $RUN python benchmark/harness/reward_server.py --port 8199 --workers 32 > /netdisk/ldq/grpo/reward_server.log 2>&1 < /dev/null &)
for i in $(seq 1 90); do grep -q "reward server on" /netdisk/ldq/grpo/reward_server.log 2>/dev/null && break; sleep 2; done
grep -q "reward server on" /netdisk/ldq/grpo/reward_server.log || done_ REWARD_SERVER_FAILED 1
pkill -f "served-model-name qwen3-8b-m8-guide085grpo"; for i in $(seq 1 60); do [ $(nvidia-smi -i 7 --query-gpu=memory.used --format=csv,noheader,nounits) -lt 1000 ] && break; sleep 5; done
[ $(nvidia-smi -i 7 --query-gpu=memory.used --format=csv,noheader,nounits) -lt 1000 ] || done_ GPU7_BUSY 1
[ -e $CKPT ] && done_ CKPT_EXISTS 1
CUDA_DEVICE_ORDER=PCI_BUS_ID CUDA_VISIBLE_DEVICES=7 HF_HOME=/netdisk/ldq/hf TMPDIR=/netdisk/ldq/tmp PYTORCH_CUDA_ALLOC_CONF=expandable_segments:True \
  $SFT -u benchmark/harness/m8_grpo.py --states runs/m8/grpo_states_match.jsonl --base $BASE --out $CKPT \
  --family guide_match --steps 120 --states-per-step 8 --group 8 --lr 1e-5 --kl 0.02 > runs/m8/grpo_$TAG.log 2>&1 \
  || { tail -20 runs/m8/grpo_$TAG.log; done_ TRAINING_FAILED 1; }
grep -E "^\{" runs/m8/grpo_$TAG.log | tail -3
[ -f $CKPT/merged/config.json ] || done_ NO_MERGED 1

echo "== 4. serve on GPU 7 :8139 $(date +%T) =="
ss -ltn | grep -q ":8139 " && done_ PORT_8139_BUSY 1
(MODEL=$CKPT/merged GPU=7 PORT=8139 NAME=$NAME setsid nohup /netdisk/ldq/serve-m8-trained.sh > /netdisk/ldq/serve-m8-$TAG.log 2>&1 < /dev/null &)
for i in $(seq 1 90); do curl -s -m 5 localhost:8139/v1/models | grep -q $NAME && break
  grep -q "Engine core initialization failed" /netdisk/ldq/serve-m8-$TAG.log && done_ SERVE_FAILED 1; sleep 20; done
curl -s -m 5 localhost:8139/v1/models | grep -q $NAME || done_ SERVE_TIMEOUT 1
$RUN python benchmark/harness/m8_serve_check.py --family guide_match --name $NAME 2>&1 | grep -v libmamba | tail -3 | grep -q PASS || done_ SERVE_CHECK_FAILED 1

echo "== 5. wait for baseline arms, then evaluate trained arm $(date +%T) =="
until grep -q "MATCH_BASELINES_DONE" runs/m8/match_baselines_dgx.log; do sleep 60; done
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
    err = sum(x.get("error") is not None for x in rows)
    print(a, "rows", len(rows), "errored", err); assert err == 0
    base["heldout"][a] = r["heldout"][a]
json.dump(base, open("runs/m8/eval_${TAG}_arms_combined.json", "w"))
PY
$RUN python -u benchmark/harness/m8_eval.py --target-fraction 0.85 --max-steps 10 --n 300 --families guide_match \
  --arms untrained-8b,trained-8b,untrained-32b --trained-model $NAME \
  --reuse runs/m8/eval_${TAG}_arms_combined.json --reuse-arms untrained-8b,trained-8b,untrained-32b \
  --out runs/m8/eval_${TAG}_n300.json 2>&1 | grep -v libmamba | tail -9 || done_ VERDICT_FAILED 1
done_ OK 0
