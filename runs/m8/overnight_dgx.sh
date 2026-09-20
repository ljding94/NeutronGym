#!/usr/bin/env bash
# Overnight strengthening runs (2026-09-18): OOD generalization, difficulty
# sweep, turn-budget curve. All against the reported seed-1 model.
set -u
cd /netdisk/ldq/NeutronGym
RUN=/netdisk/ldq/mcstas-run.sh
CKPT=/netdisk/ldq/ckpt/m8-match05grpoLR
NAME=qwen3-8b-m8-match05grpoLR
export NEUTRONGYM_VLLM_URL_8B=http://localhost:8137/v1 NEUTRONGYM_VLLM_URL_32B=http://localhost:8138/v1 NEUTRONGYM_VLLM_URL_TRAINED=http://localhost:8139/v1
done_() { echo "OVERNIGHT_DONE $1 $(date +%T)"; exit "${2:-0}"; }
ev() {  # ev <tag> <arm> <extra args...>
  local tag=$1 arm=$2; shift 2
  taskset -c 112-127 $RUN python -u benchmark/harness/m8_eval.py --target-fraction 0.85 \
    --families guide_match --arms $arm --trained-model $NAME "$@" \
    --out runs/m8/eval_${tag}_${arm}.json > runs/m8/eval_${tag}_${arm}.log 2>&1
  grep -v "libmamba\|Waiting for\|Could not set" runs/m8/eval_${tag}_${arm}.log | tail -1
}

echo "== 0. serve the reported model $(date +%T) =="
# every trained-model server runs from /netdisk/ldq/ckpt; the base
# 8B/32B servers (Qwen/Qwen3-*) are on other GPUs and are left alone
pkill -f "vllm serve /netdisk/ldq/ckpt"
for i in $(seq 1 60); do [ $(nvidia-smi -i 7 --query-gpu=memory.used --format=csv,noheader,nounits) -lt 1000 ] && break; sleep 5; done
(MODEL=$CKPT/merged GPU=7 PORT=8139 NAME=$NAME setsid nohup /netdisk/ldq/serve-m8-trained.sh > /netdisk/ldq/serve-m8-overnight.log 2>&1 < /dev/null &)
for i in $(seq 1 90); do curl -s -m 5 localhost:8139/v1/models | grep -q $NAME && break; sleep 20; done
curl -s -m 5 localhost:8139/v1/models | grep -q $NAME || done_ SERVE_TIMEOUT 1

echo "== 1. OOD targets + eval (n=150, ranges beyond train AND held-out) $(date +%T) =="
taskset -c 0-15,32-47,64-79,96-111 $RUN python benchmark/harness/precalibrate.py --family guide_match \
  --split ood --start 0 --n 150 --workers 48 | tail -1 || done_ OOD_PRECAL_FAILED 1
ev ood untrained-8b --split ood --n 150 --max-steps 10 &
ev ood trained-8b   --split ood --n 150 --max-steps 10 &
wait

echo "== 2. difficulty sweep on the fresh slice (tolerance 3% and 2%) $(date +%T) =="
for tol in 0.03 0.02; do
  tag="tol${tol#0.}"
  ev $tag untrained-8b --n 300 --start-index 300 --max-steps 10 --match-tolerance $tol &
  ev $tag trained-8b   --n 300 --start-index 300 --max-steps 10 --match-tolerance $tol &
  wait
done

echo "== 3. turn-budget curve (20 turns; every failure used all 10) $(date +%T) =="
ev turns20 untrained-8b --n 300 --start-index 300 --max-steps 20 &
ev turns20 trained-8b   --n 300 --start-index 300 --max-steps 20 &
wait
done_ OK 0
