"""T1 curation: machine-verify candidate reference instruments in THIS env.

A (paper, .instr) pair is only curatable if the reference actually compiles,
runs, and produces stable observables here. For every candidate shipped
example this sweep records: %Example ground-truth line, run outcome at the
curation protocol (ncount 1e5, fixed seed), runtime, monitor census, and —
where a %Example expectation exists — agreement with it.

Output: benchmark/inventory.json (the curation registry seed).

Usage: conda run -n mcstas python benchmark/build_inventory.py [ncount]
"""

import json
import os
import re
import sys
import time

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

from mcstas_mcp import examples, execution, results  # noqa: E402

# Real-facility shipped examples from the 2026-07-09 studies (+ anchors)
CANDIDATES = [
    "templateSANS", "templateTOF", "templateDIFF",
    "PSI_DMC", "PSI_DMC_simple", "PSI_Focus", "RITA-II",
    "ILL_IN5", "ILL_IN6", "ILL_H15_IN6", "ILL_IN4", "ILL_D2B",
    "ILL_H15_D11", "ILL_H512_D22", "ILL_IN13", "ILL_D4",
    "ILL_H10_IN8", "ILL_H53_IN14",
    "SNS_ARCS", "SNS_BASIS",
    "ISIS_LET", "ISIS_MERLIN", "ISIS_OSIRIS", "ISIS_SANS2d",
    "ISIS_CRISP", "ISIS_IMAT", "ISIS_TOSCA_preupgrade",
    "HZB_FLEX", "HZB_NEAT",
    "SESANS_Delft", "SEMSANS_Delft",
    "RTP_SANS", "RTP_DIF", "WOFSANS", "ESS_IN5_reprate",
]

EXAMPLE_RE = re.compile(
    r"%Example:\s*(.*?)\s*Detector:\s*(\w+)_I=([-\d.eE+]+)")


def define_defaults(path):
    """Parameter defaults from the DEFINE INSTRUMENT(...) header.
    Returns (defaults, missing_required)."""
    with open(path, errors="replace") as f:
        text = f.read()
    m = re.search(r"DEFINE\s+INSTRUMENT\s+[\w-]+\s*\(", text)
    if not m:
        return {}, []
    depth, i = 1, m.end()
    while i < len(text) and depth:
        depth += {"(": 1, ")": -1}.get(text[i], 0)
        i += 1
    defaults, missing = {}, []
    for part in text[m.end():i - 1].split(","):
        part = part.split("//")[0].strip()
        if not part:
            continue
        decl, _, default = part.partition("=")
        name = decl.split()[-1].strip("* ")
        if default.strip():
            defaults[name] = default.strip().strip('"')
        else:
            missing.append(name)
    return defaults, missing


def example_params(path):
    with open(path, errors="replace") as f:
        text = f.read(12000)
    m = EXAMPLE_RE.search(text)
    if not m:
        # no %Example: run at the instrument's own defaults (passing them
        # explicitly — zero CLI params triggers the interactive prompt)
        defaults, missing = define_defaults(path)
        return defaults, None, None, missing
    params = dict(re.findall(r"([\w.]+)=(\S+)", m.group(1)))
    # %Example lines list only the non-default params — merge the defaults
    defaults, missing = define_defaults(path)
    merged = {**defaults, **params}
    return merged, m.group(2), float(m.group(3)), missing


def sweep_one(name, ncount, workdir):
    try:
        meta = examples.get_example(name)
    except KeyError as e:
        return {"name": name, "status": "not-found", "error": str(e)}
    path = meta["path"]
    params, mon_name, expected, missing = example_params(path)
    entry = {
        "name": name, "site": meta["site"], "path": path,
        "example_line": meta["example_line"],
        "example_params": params,
    }
    if missing:
        entry.update({"status": "needs-params",
                      "error": f"required params without defaults: {missing}"})
        return entry
    t0 = time.time()
    try:
        job = execution.run_instr_file(
            path, params, ncount=ncount, seed=1,
            workdir=os.path.join(workdir, name.replace("-", "_")),
            timeout=420, wait=None, job_prefix="inv")
    except Exception as e:
        entry.update({"status": "launch-error", "error": str(e)[:300]})
        return entry
    entry["runtime_s"] = round(time.time() - t0, 1)
    if not job.get("ok"):
        entry.update({"status": f"failed-{job.get('stage', '?')}",
                      "error": " | ".join((job.get("diagnostics") or [])[-3:])[:400]})
        return entry
    try:
        summary = results.summarize(job["output_dir"])
    except Exception as e:
        entry.update({"status": "parse-error", "error": str(e)[:200]})
        return entry
    mons = summary["monitors"]
    entry.update({
        "status": "ok",
        "n_monitors": len(mons),
        "max_events": max((m["events"] for m in mons), default=0),
        "monitors_low_stats": len(summary["low_statistics"]),
    })
    if expected is not None and mon_name:
        got = next((m for m in mons if m["component"] == mon_name), None)
        if got:
            tol = max(0.25 * abs(expected), 5 * (got["intensity_err"] or 0))
            entry["example_check"] = {
                "monitor": mon_name, "expected": expected,
                "got": got["intensity"],
                "agree": abs(got["intensity"] - expected) <= tol,
            }
    return entry


def main():
    ncount = float(sys.argv[1]) if len(sys.argv) > 1 else 1e5
    workdir = os.path.join(REPO, "runs", "inventory")
    out_path = os.path.join(REPO, "benchmark", "inventory.json")
    inventory = []
    for name in CANDIDATES:
        print(f"--- {name}", flush=True)
        entry = sweep_one(name, ncount, workdir)
        print(f"    {entry['status']}"
              + (f"  {entry.get('runtime_s')}s, {entry.get('n_monitors')} monitors,"
                 f" example_agree={entry.get('example_check', {}).get('agree')}"
                 if entry["status"] == "ok" else f"  {entry.get('error', '')[:120]}"),
              flush=True)
        inventory.append(entry)
    with open(out_path, "w") as f:
        json.dump({"protocol": {"ncount": ncount, "seed": 1},
                   "generated": time.strftime("%Y-%m-%d %H:%M"),
                   "instruments": inventory}, f, indent=2)
    ok = sum(1 for e in inventory if e["status"] == "ok")
    print(f"\n{ok}/{len(inventory)} runnable -> {out_path}")


if __name__ == "__main__":
    main()
