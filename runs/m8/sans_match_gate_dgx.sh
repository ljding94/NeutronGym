#!/usr/bin/env bash
# Gate the second matching family, then baselines if it is clean (2026-09-18).
set -u
cd /netdisk/ldq/NeutronGym
RUN=/netdisk/ldq/mcstas-run.sh
export NEUTRONGYM_VLLM_URL_8B=http://localhost:8137/v1 NEUTRONGYM_VLLM_URL_32B=http://localhost:8138/v1
done_() { echo "SANS_MATCH_GATE_DONE $1 $(date +%T)"; exit "${2:-0}"; }
CPUS=16-31,48-63,80-95            # leave 0-15 and 112-127 to the overnight evals

echo "== 1. targets, held-out 0-299 and train 0-299 $(date +%T) =="
for split in heldout train; do
  taskset -c $CPUS $RUN python benchmark/harness/precalibrate.py --family sans_match \
    --split $split --start 0 --n 300 --workers 24 | tail -1 || done_ PRECAL_${split}_FAILED 1
done

echo "== 2. constant + lookup gate (n=150) $(date +%T) =="
taskset -c $CPUS $RUN python benchmark/harness/match_gate.py --family sans_match --n 150 --workers 24 \
  || done_ CONSTANT_GATE_FAILED 1

echo "== 3. physics-rule reference (n=150) $(date +%T) =="
taskset -c $CPUS $RUN python benchmark/harness/readout_probe.py --family sans_match --n 150 --workers 24 \
  --target-fraction 0.85 || done_ READOUT_FAILED 1

$RUN python - <<'PY' || done_ GATE_NOT_CLEAN 0
import json
c = json.load(open("runs/m8/match_gate_sans_match_n150.json"))["verdict"]
print("constant/lookup gate ok:", c["ok"], c["best_pass_rate"], "upper", c["best_pass_rate_upper"])
assert c["ok"], "not clean on copy-type shortcuts"
PY

echo "== 4. untrained baselines, held-out 0-299 $(date +%T) =="
for arm in untrained-8b untrained-32b; do
  taskset -c 16-31 $RUN python -u benchmark/harness/m8_eval.py --target-fraction 0.85 --max-steps 10 \
    --n 300 --families sans_match --arms $arm --out runs/m8/eval_sansmatch_$arm.json \
    > runs/m8/eval_sansmatch_$arm.log 2>&1 &
done
wait
for arm in untrained-8b untrained-32b; do grep -v "libmamba\|Waiting for\|Could not set" runs/m8/eval_sansmatch_$arm.log | tail -1; done
done_ OK 0
