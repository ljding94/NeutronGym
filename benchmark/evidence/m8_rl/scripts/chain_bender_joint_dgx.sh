#!/usr/bin/env bash
# Run the bender pipeline, then the joint multi-family pipeline (2026-09-20).
#
# Serial, not parallel, for two reasons: both train on GPU 7, and each one
# restarts the shared reward server on :8199 -- run together, whichever
# starts second kills the first one's server mid-step.
#
# Launch detached, or it dies with its ssh connection the way the first
# bender prep did:
#   ssh Neutron 'cd /netdisk/ldq/NeutronGym && setsid nohup bash \
#     runs/m8/chain_bender_joint_dgx.sh > runs/m8/chain.log 2>&1 < /dev/null &'
set -u
cd /netdisk/ldq/NeutronGym

echo "=== CHAIN START $(date +%F' '%T) ==="
bash runs/m8/bender_grpo_dgx.sh 2>&1 | grep -v libmamba
echo "=== bender finished $(date +%T), starting joint ==="
bash runs/m8/joint_grpo_dgx.sh 2>&1 | grep -v libmamba
echo "=== CHAIN DONE $(date +%F' '%T) ==="
