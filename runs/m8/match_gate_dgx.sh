#!/usr/bin/env bash
# guide_match: targets -> constant gate -> readout/formula gate -> baselines (2026-09-17)
set -u
cd /netdisk/ldq/NeutronGym
RUN=/netdisk/ldq/mcstas-run.sh
CPUS=0-15,32-47,64-79,96-111
export NEUTRONGYM_VLLM_URL_8B=http://localhost:8137/v1 NEUTRONGYM_VLLM_URL_32B=http://localhost:8138/v1
done_() { echo "MATCH_GATE_DONE $1 $(date +%T)"; exit "${2:-0}"; }
echo "== 1. targets $(date +%T) =="
for split in heldout train; do
  taskset -c $CPUS $RUN python benchmark/harness/precalibrate.py --family guide_match --split $split --start 0 --n 300 --workers 48 | tail -1 || done_ PRECAL_${split}_FAILED 1
done
echo "== 2. constant + lookup gate $(date +%T) =="
taskset -c $CPUS $RUN python benchmark/harness/match_gate.py --family guide_match --n 150 --workers 48 || done_ CONSTANT_GATE_FAILED 1
echo "== 3. readout / formula gate $(date +%T) =="
taskset -c $CPUS $RUN python benchmark/harness/readout_probe.py --family guide_match --n 150 --workers 48 --target-fraction 0.85 || done_ READOUT_GATE_FAILED 1
python3 - <<'PY' || done_ "GATE_NOT_CLEAN_skipping_baselines" 0
import json
c = json.load(open("runs/m8/match_gate_guide_match_n150.json"))["verdict"]
r = json.load(open("runs/m8/readout_probe_guide_match_0.85_n150.json"))["verdict"]
print("constant ok", c["ok"], c["best_pass_rate"], "| readout ok", r["ok"], r["best_pass_rate"])
assert c["best_pass_rate"] <= 0.20 and r["best_pass_rate"] <= 0.20
PY
echo "== 4. untrained 8B and 32B baselines $(date +%T) =="
for arm in untrained-8b untrained-32b; do
  taskset -c 112-127 $RUN python -u benchmark/harness/m8_eval.py --target-fraction 0.85 --max-steps 10 --n 300 \
    --families guide_match --arms $arm --out runs/m8/eval_match_arm_$arm.json > runs/m8/eval_match_arm_$arm.log 2>&1 &
done
wait
for arm in untrained-8b untrained-32b; do grep -v "libmamba\|Waiting for\|Could not set" runs/m8/eval_match_arm_$arm.log | tail -2; done
done_ OK 0
