#!/usr/bin/env bash
# Run a command inside the CPU-only NeutronGym/McStas env on neutrons-dgx01.
#   cd /netdisk/ldq/NeutronGym && /netdisk/ldq/mcstas-run.sh python scripts/m0_smoke_test.py
# Activation is REQUIRED: mcstas-core_activate.sh sets $MCSTAS, and NCrystal's
# shared-library lookup fails without it. Always go through this wrapper.
export MAMBA_ROOT_PREFIX=/netdisk/ldq/mamba-root
export TMPDIR=/netdisk/ldq/tmp
export MCSTAS_MCP_HOME=/netdisk/ldq/mcstas-mcp-home   # isolated caches
export CUDA_VISIBLE_DEVICES=""                         # CPU-only by construction
export OMP_NUM_THREADS=1 OPENBLAS_NUM_THREADS=1 MKL_NUM_THREADS=1
exec /netdisk/ldq/bin/micromamba run -p /netdisk/ldq/mcstas-env "$@"
