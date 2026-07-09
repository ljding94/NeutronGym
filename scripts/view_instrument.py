"""view_instrument.py — human-friendly 3D viewer for McStas instrument files.

Wraps mcdisplay-webgl with the sharp edges filed off:
  - accepts a .instr path OR a shipped example name (e.g. "templateSANS",
    "PSI_DMC" — resolved against the installed example library)
  - shows the instrument's parameters and defaults before launching
  - uses parameter defaults automatically; validates any param=value you
    pass against the real parameter list (instead of the C-level
    "unrecognized parameter" crash)
  - compiles inside runs/_view/ so no build artifacts litter the repo

Usage (inside the mcstas conda env):
    python scripts/view_instrument.py templateSANS
    python scripts/view_instrument.py templateSANS lambda=8
    python scripts/view_instrument.py runs/m0_build/m0_minimal.instr
    python scripts/view_instrument.py <file.instr> --rays 100 --no-browser
    python scripts/view_instrument.py PSI_DMC --diagram

Default mode is the interactive 3D geometry view (browser; its local server
stays up for --timeout seconds, default 600). With --diagram it instead
renders McStasScript's 2D component-connection schematic (beam order,
AT/ROTATED couplings) to a PNG and opens it — often the clearer view for
"is the setup right?" checks.
"""

import argparse
import glob
import os
import re
import subprocess
import sys
from datetime import datetime
from typing import NoReturn

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CONDA_PREFIX = os.environ.get("CONDA_PREFIX", "")


def die(msg) -> NoReturn:
    print(f"error: {msg}")
    sys.exit(1)


def resolve_instr(arg):
    if os.path.isfile(arg):
        return os.path.abspath(arg)
    examples = os.path.join(CONDA_PREFIX, "share", "mcstas", "resources", "examples")
    name = arg[:-6] if arg.endswith(".instr") else arg
    hits = glob.glob(os.path.join(examples, "**", f"{name}.instr"), recursive=True)
    if len(hits) == 1:
        return hits[0]
    if len(hits) > 1:
        listing = "\n  ".join(os.path.relpath(h, examples) for h in hits)
        die(f"'{arg}' matches several shipped examples:\n  {listing}\npass a full path instead")
    key = name.lower().replace("_", "")
    near = sorted(
        os.path.basename(p)[:-6]
        for p in glob.glob(os.path.join(examples, "**", "*.instr"), recursive=True)
        if key in os.path.basename(p).lower().replace("_", "")
    )
    hint = f" Did you mean: {', '.join(near[:5])}?" if near else ""
    die(f"'{arg}' is not a file and not a shipped example name.{hint}")


def read_parameters(instr_path):
    """Parse name=default pairs from the DEFINE INSTRUMENT(...) header."""
    with open(instr_path, errors="replace") as f:
        text = f.read()
    m = re.search(r"DEFINE\s+INSTRUMENT\s+\w+\s*\(", text)
    if not m:
        return []
    depth, i = 1, m.end()
    while i < len(text) and depth:
        depth += {"(": 1, ")": -1}.get(text[i], 0)
        i += 1
    params = []
    for part in text[m.end() : i - 1].split(","):
        part = part.strip()
        if not part:
            continue
        decl, _, default = part.partition("=")
        name = decl.split()[-1].strip("* ")  # drop 'double'/'int'/'string' type words
        params.append((name, default.strip() or None))
    return params


def make_diagram(instr_path, name, workdir):
    """Render McStasScript's component-connection diagram to PNG."""
    import matplotlib

    matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    import mcstasscript as ms

    os.chdir(workdir)  # McStasScript writes .instr copies and a *_db/ dir in CWD
    instrument = ms.McStas_instr(f"{name}_diagram", input_path=".")
    instrument.settings(checks=False)  # diagram needs structure only, not runnability
    try:
        ms.McStas_file(instr_path).add_to_instr(instrument)
    except Exception as e:
        die(
            f"could not parse this .instr for a diagram: {type(e).__name__}: {e}\n"
            "(the McStasScript reader chokes on some complex instruments — "
            "known limitation; the 3D view without --diagram still works)"
        )
    instrument.show_diagram()
    png = os.path.join(workdir, f"{name}_diagram.png")
    plt.gcf().savefig(png, dpi=150, bbox_inches="tight")
    return png


def main():
    ap = argparse.ArgumentParser(
        description="Human-friendly viewer for McStas instrument files (3D or diagram)."
    )
    ap.add_argument("instr", help=".instr path or shipped example name")
    ap.add_argument("params", nargs="*", help="param=value overrides (optional)")
    ap.add_argument("--diagram", action="store_true",
                    help="render the 2D component-connection schematic instead of the 3D view")
    ap.add_argument("--rays", type=int, default=50, help="trajectories to draw (default 50)")
    ap.add_argument("--no-browser", action="store_true", help="generate only, do not open viewer")
    ap.add_argument("--timeout", type=int, default=600, help="3D viewer server lifetime in s")
    args = ap.parse_args()

    if not CONDA_PREFIX or not os.path.isdir(os.path.join(CONDA_PREFIX, "share", "mcstas")):
        die("run inside the mcstas conda env:  conda activate mcstas")

    instr = resolve_instr(args.instr)
    params = read_parameters(instr)
    valid = {n for n, _ in params}

    overrides = {}
    for p in args.params:
        if "=" not in p:
            die(f"'{p}' is not a param=value pair")
        k, v = p.split("=", 1)
        if k not in valid:
            if not params:
                die(f"this instrument has NO parameters — run without '{p}'")
            die(f"unknown parameter '{k}'. Valid: {', '.join(n for n, _ in params)}")
        overrides[k] = v

    name = os.path.basename(instr)[:-6]
    print(f"instrument: {name}  ({instr})")
    if params:
        print("parameters:")
        for n, d in params:
            mark = f" -> {overrides[n]}" if n in overrides else ""
            print(f"  {n} = {d if d is not None else '(required)'}{mark}")
    else:
        print("parameters: none")

    workdir = os.path.join(REPO, "runs", "_view", f"{name}_{datetime.now():%Y%m%d_%H%M%S}")
    os.makedirs(workdir)

    if args.diagram:
        png = make_diagram(instr, name, workdir)
        print(f"\ndiagram written to {os.path.relpath(png, REPO)}")
        if not args.no_browser and sys.platform == "darwin":
            subprocess.run(["open", png])
        return

    cmd = ["mcdisplay-webgl", instr, "-n", str(args.rays),
           "--dirname", os.path.join(workdir, "trace"),
           "--timeout", str(args.timeout)]
    if args.no_browser:
        cmd.append("--nobrowse")
    if params and not overrides:
        cmd.append("--default")
    cmd += [f"{k}={v}" for k, v in overrides.items()]

    required_missing = [n for n, d in params if d is None and n not in overrides]
    if required_missing:
        die(f"parameters without defaults must be given: {', '.join(required_missing)}")

    print(f"\nlaunching viewer (build dir: {os.path.relpath(workdir, REPO)}) ...")
    proc = subprocess.run(cmd, cwd=workdir)
    if proc.returncode != 0:
        die("mcdisplay-webgl failed — see output above")
    if args.no_browser:
        print(f"view written to {os.path.join(workdir, 'trace')} (open index.html via a local server)")


if __name__ == "__main__":
    main()
