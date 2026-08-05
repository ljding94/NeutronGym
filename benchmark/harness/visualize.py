"""Shared instrument visualization — one component, both sides of the bench.

Consumers:
  (a) episode `artifacts/` bundles (run_episode.py): rendered POST-episode by
      the harness from the instrument the agent actually built — never from
      agent claims, zero agent turns spent on visualization;
  (b) `--refs` batch mode: the SAME visuals for every task's reference (and
      T2 baselines) into runs/refviz/<task>/, so episode review is always a
      side-by-side of agent artifact vs reference.

Visuals per instrument: the component-connection diagram PNG (McStasScript
show_diagram) and the real-scale 3D geometry trace (mcdisplay-webgl
--nobrowse — a directory re-servable offline). mcdisplay-matplotlib hardcopy
is NOT used: verified broken 2026-07-29 (SIGPIPE truncates the trace stream,
saves an empty plot). Examiner-side only — no sandbox implications.

Usage:
  conda run -n mcstas python benchmark/harness/visualize.py --refs \
      [--tasks T1_PSI_DMC,T2_guide_divergence] [--no-trace] [--rays 30]
"""

import argparse
import glob
import json
import os
import shutil
import subprocess
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))
import grader  # noqa: E402

REFVIZ = os.path.join(REPO, "runs", "refviz")
TRACE_TIMEOUT_S = 300


def diagram_png(instr_path: str, out_png: str) -> str | None:
    """Render the component-connection schematic; None if the McStasScript
    reader fails (known limitation on some complex instruments)."""
    import warnings

    import matplotlib

    matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    import mcstasscript as ms

    # show_diagram calls plt.show() internally — a harmless no-op under Agg
    warnings.filterwarnings("ignore", message=".*non-interactive.*")

    name = os.path.splitext(os.path.basename(instr_path))[0]
    build = os.path.join(os.path.dirname(os.path.abspath(out_png)), "_diagram_build")
    os.makedirs(build, exist_ok=True)
    prev = os.getcwd()
    try:
        os.chdir(build)  # McStasScript writes .instr copies + *_db/ into CWD
        instrument = ms.McStas_instr(f"{name}_diagram", input_path=".")
        instrument.settings(checks=False)  # structure only, not runnability
        ms.McStas_file(instr_path).add_to_instr(instrument)
        instrument.show_diagram()
        plt.gcf().savefig(out_png, dpi=150, bbox_inches="tight")
        plt.close("all")
        return out_png
    except Exception:
        return None
    finally:
        os.chdir(prev)
        shutil.rmtree(build, ignore_errors=True)


def webgl_trace(instr_path: str, params: dict, out_dir: str,
                rays: int = 30) -> str | None:
    """Real-scale 3D geometry trace via mcdisplay-webgl --nobrowse; the
    output directory is re-servable offline. None on failure."""
    from mcstas_mcp import config  # puts the conda env bin on PATH

    exe = shutil.which("mcdisplay-webgl") or os.path.join(
        config.ENV_BIN, "mcdisplay-webgl")
    build = os.path.join(os.path.dirname(os.path.abspath(out_dir)), "_trace_build")
    os.makedirs(build, exist_ok=True)
    cmd = [exe, os.path.abspath(instr_path), "-n", str(rays),
           "--dirname", os.path.abspath(out_dir), "--nobrowse"]
    if params:
        cmd += [f"{k}={v}" for k, v in params.items()]
    else:
        cmd.append("--default")  # never let mcreadparams prompt (M0 gotcha)
    try:
        proc = subprocess.run(cmd, cwd=build, capture_output=True, text=True,
                              stdin=subprocess.DEVNULL, timeout=TRACE_TIMEOUT_S)
        ok = proc.returncode == 0 and os.path.isdir(out_dir)
        return out_dir if ok else None
    except (subprocess.TimeoutExpired, OSError):
        return None
    finally:
        shutil.rmtree(build, ignore_errors=True)


def bundle(instr_path: str, params: dict, out_dir: str,
           trace: bool = True, rays: int = 30) -> dict:
    """Write the full visual bundle for one instrument into out_dir:
    the .instr copy, params.json, diagram.png, trace/. Never raises — a
    failed visual must not void a graded episode; failures land in the
    returned manifest."""
    os.makedirs(out_dir, exist_ok=True)
    manifest = {"instr": None, "params": None, "diagram": None, "trace": None,
                "errors": []}
    try:
        dest = os.path.join(out_dir, os.path.basename(instr_path))
        shutil.copy(instr_path, dest)
        manifest["instr"] = dest
    except OSError as e:
        manifest["errors"].append(f"instr copy failed: {e}")
        return manifest
    pj = os.path.join(out_dir, "params.json")
    with open(pj, "w") as f:
        json.dump(params or {}, f, indent=2, sort_keys=True)
    manifest["params"] = pj

    png = diagram_png(instr_path, os.path.join(out_dir, "diagram.png"))
    manifest["diagram"] = png
    if png is None:
        manifest["errors"].append("diagram: McStasScript reader failed "
                                  "(known limitation)")
    if trace:
        tdir = webgl_trace(instr_path, params or {},
                           os.path.join(out_dir, "trace"), rays=rays)
        manifest["trace"] = tdir
        if tdir is None:
            manifest["errors"].append("trace: mcdisplay-webgl failed")
    return manifest


def iter_ref_targets(only=None):
    """(task_id, instr_path, params) for every task with a reference
    instrument — T2 baselines included (their reference IS the baseline)."""
    for p in sorted(glob.glob(os.path.join(REPO, "benchmark", "tasks", "**",
                                           "*.json"), recursive=True)):
        if os.path.basename(p).startswith("_"):
            continue
        with open(p) as f:
            task = json.load(f)
        ref = (task.get("reference") or {}).get("instr")
        if not ref or (only and task["id"] not in only):
            continue
        yield task["id"], grader._resolve_instr(ref), (
            task["reference"].get("parameters") or {})


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--refs", action="store_true",
                    help="render reference visuals for all tasks into runs/refviz/")
    ap.add_argument("--tasks", default=None,
                    help="comma-separated task ids (default: all with a reference)")
    ap.add_argument("--no-trace", action="store_true",
                    help="diagram only (traces compile each instrument — slower)")
    ap.add_argument("--rays", type=int, default=30)
    args = ap.parse_args()
    if not args.refs:
        ap.error("only --refs batch mode has a CLI; bundles are built by "
                 "run_episode.py")

    only = set(args.tasks.split(",")) if args.tasks else None
    done = failed = 0
    for task_id, instr, params in iter_ref_targets(only):
        out = os.path.join(REFVIZ, task_id)
        m = bundle(instr, params, out, trace=not args.no_trace, rays=args.rays)
        status = "ok" if not m["errors"] else "; ".join(m["errors"])
        print(f"  {task_id:28} -> {os.path.relpath(out, REPO):34} [{status}]")
        done += 1
        failed += bool(m["errors"])
    print(f"{done} references rendered into {os.path.relpath(REFVIZ, REPO)}/"
          + (f" ({failed} with errors)" if failed else ""))
    print("thumbnails: regenerate the catalog with "
          "`python3 scripts/tasks_report.py`")


if __name__ == "__main__":
    main()
