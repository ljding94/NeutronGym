"""M1 walkthrough — human-verifiable demo of the mcstas-mcp server internals.

Exercises the same code paths the MCP tools use (catalog -> registry ->
execution -> results) to build and run a small guide instrument, then leaves
artifacts a human can inspect:

    runs/m1_demo/home/instruments/demo_guide/demo_guide.instr   the instrument
    runs/m1_demo/*.png                                          monitor plots

Follow-up inspection commands are printed at the end (3D view + diagram).

Usage: conda run -n mcstas python scripts/m1_walkthrough.py
"""

import os
import shutil
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DEMO = os.path.join(REPO, "runs", "m1_demo")

# repo-local, disposable server home so the demo never touches ~/.mcstas-mcp
shutil.rmtree(DEMO, ignore_errors=True)
os.makedirs(DEMO)
os.environ["MCSTAS_MCP_HOME"] = os.path.join(DEMO, "home")

from mcstas_mcp import catalog, execution, registry, results  # noqa: E402


def step(title):
    print(f"\n=== {title} " + "=" * max(0, 60 - len(title)))


step("1. catalog: discover and introspect components")
guides = catalog.list_components(category="optics", search="guide")
print(f"library has {len(catalog.component_names())} components; "
      f"{len(guides)} optics match 'guide', e.g. "
      + ", ".join(c["name"] for c in guides[:4]))
info = catalog.describe("Guide")
print(f"Guide: {info['description'][:90]}")
print("required params:", catalog.required_params("Guide"))

step("2. registry: build source -> guide -> monitors with validation")
spec = registry.create("demo_guide", "M1 demo: cold source, 10 m guide, PSD + wavelength monitor")
registry.add_parameter(spec, "wl", default=5.0, unit="AA", comment="central wavelength")

registry.add_component(spec, "source", "Source_simple", at=[0, 0, 0], parameters={
    "xwidth": 0.05, "yheight": 0.05, "dist": 1.5, "focus_xw": 0.03, "focus_yh": 0.05,
    "lambda0": "wl", "dlambda": "0.5*wl"})
warnings = registry.add_component(spec, "guide", "Guide", at=[0, 0, 1.5], relative="source")
print("expected warning ->", warnings[0])
registry.set_parameters(spec, "guide", {
    "w1": 0.03, "h1": 0.05, "w2": 0.02, "h2": 0.04, "l": 10.0, "m": 2.0})
registry.add_component(spec, "psd", "PSD_monitor", at=[0, 0, 10.1], relative="guide",
                       parameters={"nx": 60, "ny": 60, "xwidth": 0.04, "yheight": 0.06,
                                   "filename": "psd.dat"})
registry.add_component(spec, "lmon", "L_monitor", at=[0, 0, 10.15], relative="guide",
                       parameters={"nL": 80, "Lmin": 1.0, "Lmax": 10.0,
                                   "xwidth": 0.04, "yheight": 0.06,
                                   "filename": "lmon.dat"})

print("\nvalidation demo — three mistakes the server catches immediately:")
for fail in (
    lambda: registry.add_component(spec, "psd2", "PSD_monitr", at=[0, 0, 11]),
    lambda: registry.set_parameters(spec, "psd", {"xwidht": 0.1}),
    lambda: registry.set_parameters(spec, "source", {"lambda0": "undeclared_var"}),
):
    try:
        fail()
    except registry.SpecError as e:
        print("  caught:", str(e).split(".")[0])

step("3. execution: compile + run (ncount=1e6, seed=42)")
job = execution.run_spec(registry.load("demo_guide"), ncount=1e6, seed=42)
if not job["ok"]:
    print("RUN FAILED:", job.get("stage"), *job.get("diagnostics", [])[-10:], sep="\n  ")
    sys.exit(1)
print(f"job {job['job_id']} ok in {job['elapsed_s']}s")

step("4. results: summary statistics (what the agent sees)")
summary = execution.job_results(job["job_id"])
print(f"{'monitor':10} {'I [n/s]':>12} {'rel_err':>8} {'events':>10}  beam center/width")
for m in summary["monitors"]:
    bc = m["beam_center"]; bw = m["beam_width"]
    geo = " ".join(f"{k}={v:.3g}" for k, v in {**bc, **bw}.items())
    print(f"{m['component']:10} {m['intensity']:12.4g} {m['relative_err']:8.2%} "
          f"{m['events']:10.0f}  {geo}")
if summary["low_statistics"]:
    print("low statistics (<1000 events):", summary["low_statistics"])

step("5. rendering: monitor PNGs for human inspection")
for mon in ("psd", "lmon"):
    png = results.monitor_png(job["output_dir"], mon)
    dest = os.path.join(DEMO, f"{mon}.png")
    shutil.copy(png, dest)
    print("wrote", os.path.relpath(dest, REPO))

instr_file = os.path.join(registry.workdir("demo_guide"), "demo_guide.instr")
print(f"""
all good — inspect the demo instrument yourself:
  open {os.path.relpath(DEMO, REPO)}/psd.png {os.path.relpath(DEMO, REPO)}/lmon.png
  python scripts/view_instrument.py {os.path.relpath(instr_file, REPO)}            # 3D geometry
  python scripts/view_instrument.py {os.path.relpath(instr_file, REPO)} --diagram  # schematic
""")
