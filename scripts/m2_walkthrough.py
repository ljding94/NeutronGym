"""M2 walkthrough — human-verifiable demo of the robustness layer.

Shows: the .instr escape hatch (import shipped templateSANS), 1-ray
validation, the async job model (submit -> poll -> results), binary caching
(second run skips the recompile), and where state lives so it survives
server restarts. Artifacts land in runs/m2_demo/.

Usage: conda run -n mcstas python scripts/m2_walkthrough.py
"""

import os
import shutil
import time

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DEMO = os.path.join(REPO, "runs", "m2_demo")
shutil.rmtree(DEMO, ignore_errors=True)
os.makedirs(DEMO)
os.environ["MCSTAS_MCP_HOME"] = os.path.join(DEMO, "home")

from mcstas_mcp import examples, execution, registry, results  # noqa: E402
from mcstas_mcp.config import resources_dir  # noqa: E402


def step(title):
    print(f"\n=== {title} " + "=" * max(0, 60 - len(title)))


step("1. examples corpus: find few-shot material")
sans_examples = examples.list_examples(search="SANS")
print(f"{len(examples.list_examples())} shipped examples; {len(sans_examples)} match 'SANS'")
meta = examples._meta("templateSANS")
print(f"templateSANS ground truth line: %Example: {meta['example_line']}")

step("2. escape hatch: import the shipped templateSANS .instr")
path = os.path.join(resources_dir(), "examples", "Templates", "templateSANS",
                    "templateSANS.instr")
spec, warnings = registry.load_from_instr(path, name="sans_demo")
print(f"imported {len(spec['components'])} components, "
      f"{len(spec['parameters'])} parameters, warnings: {warnings or 'none'}")
split_comps = [c['name'] for c in spec['components'] if c['split']]
print(f"SPLIT preserved on: {split_comps}")

step("3. validate: translate + compile + 1 ray before spending ncount")
val = execution.run_spec(spec, ncount=1, timeout=180, wait=None,
                         job_prefix="sans_demo_validate")
print(f"validation {'PASSED' if val['ok'] else 'FAILED: ' + str(val.get('diagnostics'))}")

step("4. async job: submit, poll, fetch results")
job = execution.run_spec(registry.load("sans_demo"), ncount=1e6, seed=42, wait=0)
print(f"submitted {job['job_id']} -> state={job['state']}")
while True:
    st = execution.job_status(job["job_id"])
    print(f"  poll: state={st['state']} elapsed={st.get('elapsed_s')}s")
    if st["state"] != "running":
        break
    time.sleep(1)
res = execution.job_results(job["job_id"])
print(f"{'monitor':10} {'I [n/s]':>12} {'rel_err':>8} {'events':>10}")
for m in res["monitors"]:
    print(f"{m['component']:10} {m['intensity']:12.4g} "
          f"{(m['relative_err'] or 0):8.2%} {m['events']:10.0f}")

step("5. binary caching: rerun at lambda=8 — no recompile")
job2 = execution.run_spec(registry.load("sans_demo"), ncount=1e6,
                          parameters={"lambda": 8}, seed=42, wait=None)
print(f"the 1-ray validation compiled the binary once; both physics runs "
      f"reused it (compiled: {job['compiled']}, {job2['compiled']}) — parameter "
      "changes ride the CLI, so iteration never pays the ~2 s recompile")

step("6. persistence: what survives a server restart")
home = os.environ["MCSTAS_MCP_HOME"]
print(f"registry:  {home}/instruments/sans_demo/spec.json")
print(f"jobs:      {home}/jobs.json  (records incl. seed/ncount/params per run)")
print(f"snapshots: per-run .instr copies next to each job's log")
print("kill the server at any point — a fresh one reconciles running jobs "
      "from pid + on-disk output (tested in test_restart_survival_acceptance)")

png = results.monitor_png(job2["output_dir"], "detector")
dest = os.path.join(DEMO, "detector_lambda8.png")
shutil.copy(png, dest)
print(f"\ndetector image (lambda=8 A): {os.path.relpath(dest, REPO)}")
print("compare with runs/m0_templateSANS_psd.png (lambda=6) — rings shift with wavelength")
