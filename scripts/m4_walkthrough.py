"""M4 walkthrough — human-verifiable demo of the optimization layer.

The guide_bot-style task from PLAN M4 acceptance: a 10 m m=2 guide feeding a
2x2 cm target with a +-0.5 deg divergence window; free knob = guide width.
Shows the classical scan curve, then mcrun --optimize finding the optimum,
then the mandatory high-ncount fresh-seed re-verification. Artifacts in
runs/m4_demo/ (scan_curve.png is the one to look at).

Usage: conda run -n mcstas python scripts/m4_walkthrough.py
"""

import os
import shutil

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt  # noqa: E402

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DEMO = os.path.join(REPO, "runs", "m4_demo")
shutil.rmtree(DEMO, ignore_errors=True)
os.makedirs(DEMO)
os.environ["MCSTAS_MCP_HOME"] = os.path.join(DEMO, "home")

from mcstas_mcp import execution, optimization, registry  # noqa: E402


def step(title):
    print(f"\n=== {title} " + "=" * max(0, 60 - len(title)))


step("0. the task (guide_bot-style, first T2 prototype)")
print("maximize flux into 2x2 cm within +-0.5 deg at lambda = 5+-0.5 A,\n"
      "10 m m=2 straight guide; free knob: guide width/height 'gws'")

spec = registry.create("guide_opt", "M4 demo: guide width optimization")
registry.add_parameter(spec, "gws", default=0.02, unit="m", comment="guide width")
registry.add_component(spec, "src", "Source_simple", at=[0, 0, 0], parameters={
    "xwidth": 0.1, "yheight": 0.1, "dist": 1.5, "focus_xw": "gws",
    "focus_yh": "gws", "lambda0": 5, "dlambda": 0.5})
registry.add_component(spec, "guide", "Guide", at=[0, 0, 1.5], relative="src",
                       parameters={"w1": "gws", "h1": "gws", "w2": "gws",
                                   "h2": "gws", "l": 10, "m": 2})
registry.add_component(spec, "divmon", "Divergence_monitor",
                       at=[0, 0, 10.05], relative="guide",
                       parameters={"xwidth": 0.02, "yheight": 0.02,
                                   "maxdiv_h": 0.5, "maxdiv_v": 0.5,
                                   "filename": "div.dat", "restore_neutron": 1})

step("1. classical scan: FOM vs guide width (7 points, 1e5 rays, seed 11)")
scan = optimization.run_scan(spec, "gws", 0.01, 0.09, numpoints=7,
                             ncount=1e5, seed=11, wait=None, timeout=1200)
sres = optimization.scan_results(scan["job_id"])
div = next(m for m in sres["monitors"] if m["monitor"] == "divmon")
for w, i, e in zip(sres["scanned"]["gws"], div["I"], div["err"]):
    bar = "#" * int(60 * i / max(div["I"]))
    print(f"  gws={w:6.4f}  I={i:10.4g} ± {e:8.2g}  {bar}")
scan_best_i = max(div["I"])
scan_best_w = sres["scanned"]["gws"][div["I"].index(scan_best_i)]
print(f"scan best: gws={scan_best_w:.4f} -> FOM={scan_best_i:.4g}")

step("2. mcrun --optimize (nelder-mead, 40 iterations max, same seed)")
opt = optimization.run_optimize(spec, {"gws": [0.01, 0.03, 0.09]},
                                monitor="divmon", method="nelder-mead",
                                maxiter=40, ncount=1e5, seed=11,
                                wait=None, timeout=1500)
ores = optimization.optimize_results(opt["job_id"])
best = ores["best"]
print(f"optimizer: {ores['iterations']} iterations -> "
      f"gws={best['parameters']['gws']:.4f}, FOM={best['fom']:.4g} ± {best['fom_err']:.2g}")
print(f"vs scan best {scan_best_i:.4g}: "
      f"{'>= within errors OK' if best['fom'] >= scan_best_i - 3*(best['fom_err'] or 0) else 'WORSE — investigate'}")

step("3. re-verify optimum: high ncount, FRESH seed (skill rule)")
ver = execution.run_spec(registry.load("guide_opt"), ncount=1e6,
                         parameters={"gws": best["parameters"]["gws"]},
                         seed=99, wait=None)
vres = execution.job_results(ver["job_id"])
vm = vres["monitors"][0]
print(f"verified FOM at optimum: {vm['intensity']:.4g} ± {vm['intensity_err']:.2g} "
      f"({vm['events']:.0f} events, rel_err {vm['relative_err']:.2%})")

step("4. scan curve plot -> runs/m4_demo/scan_curve.png")
fig, ax = plt.subplots(figsize=(7, 4.5))
ax.errorbar(sres["scanned"]["gws"], div["I"], yerr=div["err"],
            fmt="o-", capsize=3, label="classical scan (7 pts)")
ax.axvline(best["parameters"]["gws"], color="tab:red", ls="--",
           label=f"optimizer: gws={best['parameters']['gws']:.4f}")
ax.errorbar([best["parameters"]["gws"]], [vm["intensity"]],
            yerr=[vm["intensity_err"]], fmt="s", color="tab:green",
            ms=9, label="re-verified @1e6, fresh seed")
ax.set_xlabel("guide width gws [m]")
ax.set_ylabel("flux into 2x2 cm, ±0.5° [n/s]")
ax.set_title("M4 acceptance: guide width optimization (guide_bot-style)")
ax.legend()
fig.tight_layout()
fig.savefig(os.path.join(DEMO, "scan_curve.png"), dpi=120)
print("saved. Baseline CLI equivalent:")
print("  mcstas-baseline optimize guide_opt --free gws=0.01,0.03,0.09 "
      "--monitor divmon --method nelder-mead --ncount 1e5 --seed 11")
