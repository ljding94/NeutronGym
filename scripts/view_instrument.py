#!/usr/bin/env python3
"""view_instrument.py — human-friendly viewer for McStas instrument files.

Usage:
  python3 scripts/view_instrument.py <file.instr | example-name> [param=value ...] [options]

Examples:
  python3 scripts/view_instrument.py runs/m0_build/m0_minimal.instr   # summary + 3D view
  python3 scripts/view_instrument.py templateSANS                     # finds shipped example
  python3 scripts/view_instrument.py templateSANS lambda=8 --rays 100
  python3 scripts/view_instrument.py PSI_DMC --info                   # summary only, no browser

What it does:
  1. Locates the .instr file (a path, or a name searched in the shipped examples).
  2. Prints a summary: instrument parameters with defaults, and the component list.
  3. Launches the interactive 3D viewer (mcdisplay-webgl) in your browser.

It also works around the McStas gotchas so you don't have to remember them:
  - params you pass are validated against the instrument's actual parameters
    (passing an unknown one, e.g. lambda=6 to a parameter-less instrument,
    makes the binary die with 'unrecognized parameter');
  - if the instrument HAS parameters and you pass none, it adds --default
    (otherwise the binary prompts interactively and appears to hang);
  - compilation runs in runs/view/<name>/, never your current directory.

Run from anywhere; if the mcstas conda env is not active it re-executes itself
inside it.
"""

import os
import re
import shutil
import subprocess
import sys
from typing import NoReturn

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

if shutil.which("mcdisplay-webgl") is None:
    os.execvp(
        "conda",
        ["conda", "run", "-n", "mcstas", "--no-capture-output",
         "python3", os.path.abspath(__file__), *sys.argv[1:]],
    )


def die(msg) -> NoReturn:
    print(f"error: {msg}")
    sys.exit(1)


def resources_dir():
    out = subprocess.run(
        ["mcrun", "--showcfg=resourcedir"], capture_output=True, text=True
    )
    return out.stdout.strip()


def locate_instr(arg):
    if os.path.isfile(arg):
        return os.path.abspath(arg)
    pattern = arg.lower().removesuffix(".instr")
    hits = []
    for root, _dirs, files in os.walk(os.path.join(resources_dir(), "examples")):
        for f in files:
            if f.endswith(".instr") and pattern in f.lower():
                hits.append(os.path.join(root, f))
    exact = [h for h in hits if os.path.basename(h).lower() == pattern + ".instr"]
    if exact:
        return exact[0]
    if len(hits) == 1:
        return hits[0]
    if not hits:
        die(f"'{arg}' is neither a file nor matches any shipped example")
    print(f"'{arg}' matches {len(hits)} shipped examples — be more specific:")
    for h in sorted(hits):
        print(f"  {os.path.basename(h)[:-6]:40s} ({os.path.relpath(h, resources_dir())})")
    sys.exit(1)


def strip_comments(text):
    text = re.sub(r"/\*.*?\*/", " ", text, flags=re.S)
    return re.sub(r"//[^\n]*", " ", text)


def split_top_commas(s):
    parts, depth, buf = [], 0, ""
    for c in s:
        if c in "([":
            depth += 1
        elif c in ")]":
            depth -= 1
        if c == "," and depth == 0:
            parts.append(buf)
            buf = ""
        else:
            buf += c
    if buf.strip():
        parts.append(buf)
    return parts


def parse_instrument(path):
    """Return (name, [(param, default|None)], [(comp, type, placement)])."""
    text = strip_comments(open(path, errors="replace").read())
    m = re.search(r"DEFINE\s+INSTRUMENT\s+(\w+)\s*\(", text)
    if not m:
        die(f"no DEFINE INSTRUMENT found in {path}")
    name = m.group(1)
    i, depth, buf = m.end(), 1, ""
    while i < len(text) and depth:
        c = text[i]
        depth += c == "("
        depth -= c == ")"
        if depth:
            buf += c
        i += 1
    params = []
    for part in split_top_commas(buf):
        part = part.strip()
        if not part:
            continue
        pm = re.match(r"(?:(?:double|int|string|char)\s*\*?\s+)?(\w+)\s*(?:=\s*(.+))?$", part)
        if pm:
            params.append((pm.group(1), (pm.group(2) or "").strip() or None))
    comps = []
    for cm in re.finditer(r"^\s*COMPONENT\s+(\w+)\s*=\s*(\w+)\s*\(", text, re.M):
        tail = text[cm.end():cm.end() + 400]
        at = re.search(r"AT\s*\(([^)]*)\)\s*(RELATIVE\s+\w+|ABSOLUTE)?", tail)
        placement = ""
        if at:
            placement = f"AT ({at.group(1).strip()}) {at.group(2) or ''}".strip()
        comps.append((cm.group(1), cm.group(2), placement))
    return name, params, comps


def main():
    args = sys.argv[1:]
    if not args or args[0] in ("-h", "--help"):
        print(__doc__)
        sys.exit(0)
    info_only = "--info" in args
    nobrowse = "--nobrowse" in args
    rays = None
    if "--rays" in args:
        rays = args[args.index("--rays") + 1]
        args.remove(rays)
    args = [a for a in args if a not in ("--info", "--nobrowse", "--rays")]
    target, user_params = args[0], args[1:]

    instr = locate_instr(target)
    name, params, comps = parse_instrument(instr)

    print(f"\nInstrument: {name}")
    print(f"File:       {instr}\n")
    if params:
        print(f"Parameters ({len(params)}):")
        for p, d in params:
            print(f"  {p:24s} default = {d if d is not None else '(REQUIRED)'}")
    else:
        print("Parameters: none (everything is hardcoded in the components)")
    print(f"\nComponents ({len(comps)}):")
    for cname, ctype, placement in comps:
        print(f"  {cname:24s} {ctype:28s} {placement}")

    valid = {p for p, _ in params}
    for up in user_params:
        if "=" not in up:
            die(f"'{up}' is not a param=value setting")
        key = up.split("=", 1)[0]
        if key not in valid:
            msg = f"'{key}' is not a parameter of {name}."
            msg += (
                f" Available: {', '.join(sorted(valid))}" if valid
                else " This instrument takes no parameters — pass none."
            )
            die(msg)

    if info_only:
        return
    required_unset = [p for p, d in params if d is None
                      and not any(u.startswith(p + "=") for u in user_params)]
    if required_unset:
        die(f"these parameters have no default, set them: {', '.join(required_unset)}")

    workdir = os.path.join(REPO, "runs", "view", name)
    os.makedirs(workdir, exist_ok=True)
    cmd = ["mcdisplay-webgl", instr]
    if rays:
        cmd += ["-n", rays]
    if nobrowse:
        cmd.append("--nobrowse")
    cmd += user_params
    if params and not user_params:
        cmd.append("--default")
    print(f"\nLaunching 3D viewer (serves ~5 min; Ctrl-C to stop early)...")
    print(f"  {' '.join(cmd)}\n")
    sys.exit(subprocess.run(cmd, cwd=workdir).returncode)


if __name__ == "__main__":
    main()
