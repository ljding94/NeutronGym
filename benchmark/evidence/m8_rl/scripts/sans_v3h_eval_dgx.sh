#!/usr/bin/env bash
# SANS v3h evaluation on the DGX (2026-09-16). Moved off the laptop after a
# laptop disconnect (16:09-17:46) broke the first run mid-arm. Runs the three
# arms in parallel (each model has its own GPU server), then combines them via
# m8_eval --reuse so the verdict code is unchanged, then the constant probe.
set -u
cd /netdisk/ldq/NeutronGym
RUN="/netdisk/ldq/mcstas-run.sh"
TAG=sans085v3h; FRAC=0.85; MAX_STEPS=10; N=300
NAME=qwen3-8b-m8raft-$TAG-pass
OUT=runs/m8; mkdir -p $OUT
export NEUTRONGYM_VLLM_URL_8B=http://localhost:8137/v1 NEUTRONGYM_VLLM_URL_32B=http://localhost:8138/v1 NEUTRONGYM_VLLM_URL_TRAINED=http://localhost:8139/v1
done_() { echo "SANS_V3H_EVAL_DONE $1 $(date +%T)"; exit "${2:-0}"; }
# CPUs on NUMA nodes 0/2/4/6, away from GPU 7's node and the vLLM GPUs
CPUS=(0-15 32-47 64-79)
echo "== arms in parallel $(date +%T) =="
i=0
for arm in untrained-8b trained-8b untrained-32b; do
  taskset -c ${CPUS[$i]} $RUN python -u benchmark/harness/m8_eval.py --target-fraction $FRAC --max-steps $MAX_STEPS \
    --n $N --families sans_collimation --arms $arm --trained-model $NAME \
    --out $OUT/eval_${TAG}_arm_${arm}.json > $OUT/eval_${TAG}_arm_${arm}.log 2>&1 &
  i=$((i+1))
done
wait
for arm in untrained-8b trained-8b untrained-32b; do
  tail -2 $OUT/eval_${TAG}_arm_${arm}.log
  [ -f $OUT/eval_${TAG}_arm_${arm}.json ] || done_ "ARM_FAILED_$arm" 1
done
echo "== combine $(date +%T) =="
$RUN python - <<PY || done_ COMBINE_FAILED 1
import json
arms = ["untrained-8b", "trained-8b", "untrained-32b"]
recs = {a: json.load(open(f"$OUT/eval_${TAG}_arm_{a}.json")) for a in arms}
base = dict(recs[arms[0]]); base["heldout"] = {}; base["arms"] = {}
for a in arms:
    r = recs[a]
    for k in ("target_fraction", "n_per_family", "max_steps", "split", "temperature"):
        assert r[k] == base[k], (a, k, r[k], base[k])
    base["heldout"][a] = r["heldout"][a]
    rows = r["heldout"][a]["sans_collimation"]["rows"]
    err = sum(x.get("error") is not None or x.get("best_level") is None and not x.get("skipped") for x in rows)
    print(a, "rows", len(rows), "errored", err)
    assert err == 0, f"{a} has {err} errored rows"
json.dump(base, open("$OUT/eval_${TAG}_arms_combined.json", "w"))
PY
$RUN python -u benchmark/harness/m8_eval.py --target-fraction $FRAC --max-steps $MAX_STEPS --n $N \
  --families sans_collimation --arms untrained-8b,trained-8b,untrained-32b --trained-model $NAME \
  --reuse $OUT/eval_${TAG}_arms_combined.json --reuse-arms untrained-8b,trained-8b,untrained-32b \
  --out $OUT/eval_${TAG}_n300_passing.json || done_ VERDICT_FAILED 1
echo "== constant probe at $FRAC $(date +%T) =="
taskset -c 0-15,32-47,64-79,96-111 $RUN python -u benchmark/harness/m8_constant_probe.py --family sans_collimation --grid \
  --target-fraction $FRAC --data /netdisk/ldq/m8data/train_$TAG.jsonl \
  --eval $OUT/eval_${TAG}_n300_passing.json --n $N --out $OUT/constant_probe_${TAG}_n300.json 2>&1 \
  | grep -E "^best single|^any of|^->" || done_ CONSTANT_PROBE_FAILED 1
done_ OK 0
