#!/usr/bin/env bash
set -u
cd /netdisk/ldq/NeutronGym
RUN=/netdisk/ldq/mcstas-run.sh
export NEUTRONGYM_VLLM_URL_8B=http://localhost:8137/v1 NEUTRONGYM_VLLM_URL_32B=http://localhost:8138/v1
echo "== baselines guide_match n=300, 10 turns $(date +%T) =="
for arm in untrained-8b untrained-32b; do
  taskset -c 112-127 $RUN python -u benchmark/harness/m8_eval.py --target-fraction 0.85 --max-steps 10 --n 300 \
    --families guide_match --arms $arm --out runs/m8/eval_match_arm_$arm.json > runs/m8/eval_match_arm_$arm.log 2>&1 &
done
taskset -c 0-15,32-47,64-79 $RUN python benchmark/harness/readout_probe.py --family guide_match --n 300 --workers 48 \
  --out runs/m8/readout_probe_guide_match_n300.json > runs/m8/readout_probe_guide_match_n300.log 2>&1 &
wait
for arm in untrained-8b untrained-32b; do grep -v "libmamba\|Waiting for\|Could not set" runs/m8/eval_match_arm_$arm.log | tail -2; done
grep -v libmamba runs/m8/readout_probe_guide_match_n300.log | head -3
echo "MATCH_BASELINES_DONE $(date +%T)"
