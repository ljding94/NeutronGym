"""M5: empirical fast-tier rollout timing — the number the RL plan rests on.

Measures the template-family regime the environment will use:
  - one-time template compile (topology fixed)
  - then N parameter-instance rollouts at several ncount values, reusing the
    cached binary (parameters ride the mcrun CLI)

A 'rollout' here = one full env step's simulation cost: run + mccode.sim
parse (reward-ladder terminal stage at truncated ncount). Static-tier checks
are pure Python (<1 ms) and cheap-dynamic = the same run at the lowest
ncount, so this table bounds the whole ladder.

Usage: conda run -n mcstas python benchmark/harness/measure_fast_tier.py [rollouts_per_ncount]
"""

import os
import statistics
import sys
import tempfile
import time

os.environ.setdefault("MCSTAS_MCP_HOME", tempfile.mkdtemp(prefix="fast_tier_"))

from mcstas_mcp import execution, registry, results  # noqa: E402


def build_template():
    spec = registry.create("fast_tier_probe", "timing template: src->guide->div monitor")
    registry.add_parameter(spec, "gws", default=0.03)
    registry.add_parameter(spec, "wl", default=5.0)
    registry.add_component(spec, "src", "Source_simple", at=[0, 0, 0], parameters={
        "xwidth": 0.1, "yheight": 0.1, "dist": 1.5, "focus_xw": "gws",
        "focus_yh": "gws", "lambda0": "wl", "dlambda": "0.5*wl"})
    registry.add_component(spec, "guide", "Guide", at=[0, 0, 1.5], relative="src",
                           parameters={"w1": "gws", "h1": "gws", "w2": "gws",
                                       "h2": "gws", "l": 10, "m": 2})
    registry.add_component(spec, "mon", "Divergence_monitor", at=[0, 0, 10.05],
                           relative="guide",
                           parameters={"xwidth": 0.02, "yheight": 0.02,
                                       "maxdiv_h": 0.5, "maxdiv_v": 0.5,
                                       "filename": "d.dat", "restore_neutron": 1})
    return spec


def main():
    n_rollouts = int(sys.argv[1]) if len(sys.argv) > 1 else 8
    spec = build_template()

    t0 = time.time()
    job = execution.run_spec(spec, ncount=1e3, seed=1, wait=None)
    compile_s = time.time() - t0
    assert job["ok"] and job["compiled"]
    print(f"template compile + first run: {compile_s:.2f} s (one-time per family)\n")

    print(f"{'ncount':>8} {'median':>9} {'p90':>9} {'min':>9}   (s/rollout, n={n_rollouts})")
    for ncount in (1e3, 1e4, 1e5, 1e6):
        times = []
        for i in range(n_rollouts):
            gws = 0.02 + 0.005 * (i % 5)  # vary parameters like the env will
            wl = 4.0 + 0.25 * i
            t0 = time.time()
            j = execution.run_spec(registry.load("fast_tier_probe"), ncount=ncount,
                                   parameters={"gws": gws, "wl": wl},
                                   seed=100 + i, wait=None)
            assert j["ok"] and not j["compiled"], "cache miss mid-family!"
            results.summarize(j["output_dir"])  # include reward-parse cost
            times.append(time.time() - t0)
        med = statistics.median(times)
        p90 = sorted(times)[max(0, int(0.9 * len(times)) - 1)]
        print(f"{ncount:8.0e} {med:9.3f} {p90:9.3f} {min(times):9.3f}")
    print("\nfast-tier verdict: rollout < 1 s requires ncount <= ~1e5 with the "
          "cached template binary; static+cheap tiers are bounded by the "
          "smallest ncount row.")


if __name__ == "__main__":
    main()
