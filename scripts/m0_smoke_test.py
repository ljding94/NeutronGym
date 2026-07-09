"""M0 smoke test (PLAN.md): verify the McStas toolchain end-to-end on this machine.

Three stages, each exercising a layer the MCP server will depend on:
  1. mcrun CLI on the shipped templateSANS example  -> binaries + compiler chain
  2. McStasScript load_data on the run output        -> data-object layer
  3. McStasScript-built minimal instrument, executed -> programmatic API (M1 path)

Acceptance: all stages pass, monitor intensities are nonzero, and a PNG renders.

Usage: conda run -n mcstas python scripts/m0_smoke_test.py
"""

import glob
import os
import subprocess
import sys

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
RUNS = os.path.join(REPO, "runs")
CONDA_PREFIX = os.environ.get("CONDA_PREFIX", "")


def fail(msg):
    print(f"FAIL: {msg}")
    sys.exit(1)


def find_mcstas_resources():
    candidates = glob.glob(os.path.join(CONDA_PREFIX, "share", "mcstas*", "resources"))
    if not candidates:
        fail(f"no mcstas resources dir under {CONDA_PREFIX}/share")
    return candidates[0]


def configure_mcstasscript(resources):
    from mcstasscript.interface import functions

    configurator = functions.Configurator()
    configurator.set_mcstas_path(resources)
    configurator.set_mcrun_path(os.path.join(CONDA_PREFIX, "bin"))


def stage1_mcrun_template_sans(resources):
    hits = glob.glob(
        os.path.join(resources, "examples", "**", "templateSANS.instr"), recursive=True
    )
    if not hits:
        fail(f"templateSANS.instr not found under {resources}/examples")
    instr = hits[0]
    outdir = os.path.join(RUNS, "m0_templateSANS")
    if os.path.exists(outdir):
        import shutil

        shutil.rmtree(outdir)
    build_dir = os.path.join(RUNS, "m0_build")
    os.makedirs(build_dir, exist_ok=True)
    print(f"stage 1: mcrun {os.path.relpath(instr, resources)} -n 1e6 lambda=6")
    # Passing at least one instrument parameter prevents the binary from
    # prompting interactively (mcreadparams) for all of them.
    proc = subprocess.run(
        ["mcrun", instr, "-n", "1e6", "-d", outdir, "lambda=6"],
        capture_output=True,
        text=True,
        timeout=600,
        cwd=build_dir,
    )
    if proc.returncode != 0:
        print(proc.stdout[-3000:])
        print(proc.stderr[-3000:])
        fail("mcrun exited nonzero")
    print("stage 1: OK")
    return outdir


def stage2_load_and_plot(outdir):
    import mcstasscript as ms

    data = ms.load_data(outdir)
    if not data:
        fail(f"McStasScript loaded no monitors from {outdir}")
    png = None
    for mon in data:
        i_sum = float(np.sum(mon.Intensity))
        e_sum = float(np.sum(mon.Error))
        n_sum = float(np.sum(mon.Ncount))
        print(
            f"stage 2: {mon.name}: I={i_sum:.4g} err={e_sum:.4g} events={n_sum:.4g} "
            f"shape={np.shape(mon.Intensity)}"
        )
        if i_sum <= 0 or not np.isfinite(i_sum):
            fail(f"monitor {mon.name} has non-positive or non-finite intensity")
        if png is None and np.ndim(mon.Intensity) == 2:
            png = os.path.join(RUNS, "m0_templateSANS_psd.png")
            plt.imshow(mon.Intensity, origin="lower", norm="log")
            plt.colorbar(label="Intensity [n/s]")
            plt.title(f"templateSANS: {mon.name} (1e6 rays)")
            plt.savefig(png, dpi=120, bbox_inches="tight")
            plt.close()
    if png is None or os.path.getsize(png) == 0:
        fail("no 2D monitor PNG rendered")
    print(f"stage 2: OK (PNG: {png})")


def stage3_programmatic_build():
    import mcstasscript as ms

    instr = ms.McStas_instr("m0_minimal")
    src = instr.add_component("source", "Source_simple")
    src.set_parameters(
        xwidth=0.02, yheight=0.02, dist=2, focus_xw=0.05, focus_yh=0.05,
        lambda0=5, dlambda=1,
    )
    psd = instr.add_component("psd", "PSD_monitor", AT=[0, 0, 2], RELATIVE="source")
    psd.set_parameters(nx=50, ny=50, xwidth=0.1, yheight=0.1, filename='"psd.dat"')
    outdir = os.path.join(RUNS, "m0_minimal")
    if os.path.exists(outdir):
        import shutil

        shutil.rmtree(outdir)
    instr.settings(ncount=1e5, output_path=outdir, suppress_output=True)
    print("stage 3: building + running source->PSD via McStasScript API")
    data = instr.backengine()
    if not data:
        fail("programmatic run returned no data")
    i_sum = float(np.sum(data[0].Intensity))
    print(f"stage 3: psd I={i_sum:.4g}")
    if i_sum <= 0 or not np.isfinite(i_sum):
        fail("programmatic PSD intensity non-positive")
    print("stage 3: OK")


def main():
    if not CONDA_PREFIX or "mcstas" not in CONDA_PREFIX:
        fail("run inside the mcstas conda env (conda run -n mcstas ...)")
    os.makedirs(RUNS, exist_ok=True)
    # Both mcrun and McStasScript compile into the CWD — keep artifacts out
    # of the repo root.
    build_dir = os.path.join(RUNS, "m0_build")
    os.makedirs(build_dir, exist_ok=True)
    os.chdir(build_dir)
    resources = find_mcstas_resources()
    print(f"mcstas resources: {resources}")
    configure_mcstasscript(resources)
    outdir = stage1_mcrun_template_sans(resources)
    stage2_load_and_plot(outdir)
    stage3_programmatic_build()
    print("\nM0 SMOKE TEST: ALL STAGES PASSED")


if __name__ == "__main__":
    main()
