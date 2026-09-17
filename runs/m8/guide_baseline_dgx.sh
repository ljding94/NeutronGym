#!/usr/bin/env bash
# Guide headroom check before GRPO (2026-09-16): untrained 8B vs 32B on
# held-out guide, v3 targets, 0.85x, 10 turns, corrected feedback labels.
set -u
cd /netdisk/ldq/NeutronGym
RUN=/netdisk/ldq/mcstas-run.sh
CPUS=0-15,32-47,64-79,96-111
export NEUTRONGYM_VLLM_URL_8B=http://localhost:8137/v1 NEUTRONGYM_VLLM_URL_32B=http://localhost:8138/v1
done_() { echo "GUIDE_BASELINE_DONE $1 $(date +%T)"; exit "${2:-0}"; }
echo "== held-out targets 0-299 $(date +%T) =="
taskset -c $CPUS $RUN python benchmark/harness/precalibrate.py --family guide_divergence --split heldout --start 0 --n 300 --workers 48 | tail -1 || done_ PRECAL_FAILED 1
echo "== train targets 0-599 in background (for GRPO) $(date +%T) =="
taskset -c 48-63,80-95 $RUN python benchmark/harness/precalibrate.py --family guide_divergence --split train --start 0 --n 600 --workers 30 > runs/m8/guide_train_precal.log 2>&1 &
echo "== untrained 8B and 32B in parallel $(date +%T) =="
for arm in untrained-8b untrained-32b; do
  taskset -c 0-15 $RUN python -u benchmark/harness/m8_eval.py --target-fraction 0.85 --max-steps 10 --n 300 \
    --families guide_divergence --arms $arm --out runs/m8/eval_guide085v3_arm_$arm.json > runs/m8/eval_guide085v3_arm_$arm.log 2>&1 &
done
wait
for arm in untrained-8b untrained-32b; do grep -v "libmamba\|Waiting for\|Could not set" runs/m8/eval_guide085v3_arm_$arm.log | tail -2; done
tail -1 runs/m8/guide_train_precal.log
done_ OK 0
