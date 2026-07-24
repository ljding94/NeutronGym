"""T1 seen-tier task selection + authoring (M5).

Selection: machine-verified instruments from benchmark/inventory.json,
class/site-diverse, capped at TARGET_N. Authoring: for each selected
instrument the prompt (an NL spec sheet) is derived from the reference
.instr through the validated reader->spec converter — faithful by
construction, per the task-input policy for paywalled anchors
(note/study-paper-pairs-2026-07-24.md §B). The grading contract is derived
from the reference run's actual monitors (roles with adequate statistics).

Reader-incompatible instruments are reported for manual authoring, not
silently skipped. Every generated task must then pass
benchmark/validate_tasks.py (reference vs itself at a fresh seed).

Usage: conda run -n mcstas python benchmark/author_tasks.py
"""

import json
import os
import re
import tempfile

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TASK_DIR = os.path.join(REPO, "benchmark", "tasks", "T1")

os.environ.setdefault("MCSTAS_MCP_HOME", tempfile.mkdtemp(prefix="author_"))

from mcstas_mcp import registry, results  # noqa: E402
from mcstas_mcp.registry import SpecError  # noqa: E402
import grader  # noqa: E402  (benchmark/ on path when run from benchmark/)

TARGET = [
    # (name, class, split)
    ("templateSANS", "SANS", "dev"),
    ("templateTOF", "ToF-spectrometer", "dev"),
    ("PSI_DMC", "powder-diffractometer", "seen"),
    ("PSI_Focus", "ToF-spectrometer", "seen"),
    ("ILL_IN5", "ToF-spectrometer", "seen"),
    ("ILL_IN6", "ToF-spectrometer", "seen"),
    ("ILL_IN4", "ToF-spectrometer", "seen"),
    ("ILL_D2B", "powder-diffractometer", "seen"),
    ("ILL_H15_D11", "SANS", "seen"),
    ("ILL_H512_D22", "SANS", "seen"),
    ("ILL_IN13", "backscattering", "seen"),
    ("ILL_D4", "liquids-diffractometer", "seen"),
    ("ILL_H10_IN8", "TAS", "seen"),
    ("ILL_H53_IN14", "TAS", "seen"),
    ("SNS_BASIS", "backscattering", "seen"),
    ("ISIS_LET", "ToF-spectrometer", "seen"),
    ("ISIS_SANS2d", "SANS", "seen"),
    ("HZB_FLEX", "TAS", "seen"),
    ("HZB_NEAT", "ToF-spectrometer", "seen"),
    ("SESANS_Delft", "SESANS", "seen"),
    ("RTP_DIF", "powder-diffractometer", "seen"),
    ("ESS_IN5_reprate", "ToF-spectrometer", "seen"),
]

# provenance from note/study-instrument-papers-2026-07-09.md and
# note/study-paper-pairs-2026-07-24.md (access classes verified there)
PAPERS = {
    "PSI_DMC": {"cite": "Willendrup et al., Physica B 385-386, 1032 (2006)",
                "doi": "10.1016/j.physb.2006.05.329", "access": "spec-sheet"},
    "ILL_IN5": {"cite": "Ollivier & Mutka, JPSJ 80, SB003 (2011)",
                "doi": "10.1143/JPSJS.80SB.SB003", "access": "spec-sheet"},
    "ILL_IN6": {"cite": "Blanc, ILL Report 83BL21G (1983)", "doi": None,
                "access": "spec-sheet"},
    "ILL_IN4": {"cite": "Mutka, NIM A 338, 144 (1994)",
                "doi": "10.1016/0168-9002(94)91297-1", "access": "spec-sheet"},
    "ILL_D2B": {"cite": "Cussen, NIM A 554, 406 (2005)",
                "doi": "10.1016/j.nima.2005.08.018", "access": "spec-sheet"},
    "ILL_H15_D11": {"cite": "Lindner & Schweins, Neutron News 21(2) (2010)",
                    "doi": "10.1080/10448631003697985", "access": "spec-sheet"},
    "ILL_IN13": {"cite": "Natali et al., Neutron News 19(4) (2008)",
                 "doi": "10.1080/10448630802474083", "access": "spec-sheet"},
    "SNS_BASIS": {"cite": "Mamontov & Herwig, RSI 82, 085109 (2011)",
                  "doi": "10.1063/1.3626214", "access": "spec-sheet"},
    "ISIS_LET": {"cite": "Bewley et al., NIM A 637, 128 (2011)",
                 "doi": "10.1016/j.nima.2011.01.173", "access": "spec-sheet"},
    "ISIS_SANS2d": {"cite": "Heenan et al., Neutron News 22(2) (2011)",
                    "doi": "10.1080/10448632.2011.569531", "access": "spec-sheet"},
    "SESANS_Delft": {"cite": "Bouwman, J. Appl. Cryst. 54 (2021)",
                     "doi": "10.1107/S1600576720015496", "access": "oa-pdf"},
    "PSI_Focus": {"cite": "Janssen et al., Physica B 276-278, 89 (2000)",
                  "doi": "10.1016/S0921-4526(99)01253-3", "access": "spec-sheet"},
}

ROLE_OBSERVABLES = {
    "2d": {"intensity": {"rtol": 0.2, "nsigma": 3},
           "beam_width_x": {"rtol": 0.3}, "beam_width_y": {"rtol": 0.3}},
    "wavelength": {"intensity": {"rtol": 0.25, "nsigma": 3},
                   "center_of_mass": {"rtol": 0.05}, "fwhm": {"rtol": 0.5}},
    "energy": {"intensity": {"rtol": 0.25, "nsigma": 3},
               "center_of_mass": {"rtol": 0.05}, "fwhm": {"rtol": 0.5}},
    "tof": {"intensity": {"rtol": 0.25, "nsigma": 3},
            "center_of_mass": {"rtol": 0.05}},
}
ROLE_HINT = {
    "2d": "a 2D position-sensitive monitor",
    "wavelength": "a wavelength monitor",
    "energy": "an energy monitor",
    "tof": "a time-of-flight monitor",
}


def fmt_val(v):
    if isinstance(v, str):
        return v
    if isinstance(v, (int, float)):
        return f"{v:g}"
    return str(v)  # array initializers etc. from the loader


def render_prompt(name, klass, spec, roles, params):
    lines = [
        f"Rebuild the following {klass.replace('-', ' ')} in McStas from its "
        "specification, run it, and report the integrated intensity (with "
        "error and event count) on every monitor.",
        "",
        "Instrument parameters (defaults in parentheses):",
    ]
    for p in spec["parameters"]:
        unit = f" {p['unit']}" if p.get("unit") else ""
        com = f" — {p['comment']}" if p.get("comment") else ""
        lines.append(f"- {p['name']} ({fmt_val(p['default'])}{unit}){com}")
    if spec.get("declares") or spec.get("raw_declares") or spec.get("initialize"):
        lines.append("")
        lines.append("Constants and derived quantities computed at startup "
                     "(implement equivalent logic):")
        for d in spec.get("declares", []):
            if d.get("value") not in (None, ""):
                lines.append(f"- {d['name']} = {fmt_val(d['value'])}")
        for rd in spec.get("raw_declares", []):
            lines.append(f"- declaration: `{rd.strip()}`")
        if spec.get("initialize"):
            lines.append("```c")
            lines.append(spec["initialize"].strip())
            lines.append("```")
    lines.append("")
    lines.append("Beamline, in order (positions in meters; AT [x,y,z] "
                 "RELATIVE to the named component; parameters as given):")
    for c in spec["components"]:
        rel = f" RELATIVE {c['relative']}" if c["relative"] else " ABSOLUTE"
        at = "[" + ", ".join(fmt_val(x) for x in c["at"]) + "]"
        seg = f"- {c['name']}: {c['component']} AT {at}{rel}"
        if c.get("rotated"):
            seg += (" ROTATED [" + ", ".join(fmt_val(x) for x in c["rotated"])
                    + "]" + (f" RELATIVE {c['rotated_relative']}"
                             if c.get("rotated_relative") else ""))
        extras = []
        if c.get("when"):
            extras.append(f"WHEN {c['when']}")
        if c.get("split"):
            extras.append(f"SPLIT {c['split']}")
        if c.get("group"):
            extras.append(f"GROUP {c['group']}")
        if extras:
            seg += "  (" + ", ".join(extras) + ")"
        lines.append(seg)
        if c["parameters"]:
            ps = ", ".join(f"{k}={fmt_val(v)}" for k, v in c["parameters"].items())
            lines.append(f"    parameters: {ps}")
        if c.get("extend"):
            lines.append("    EXTEND: ```c")
            lines.append("    " + c["extend"].strip())
            lines.append("    ```")
    lines.append("")
    need = [ROLE_HINT[r] for r in roles if r in ROLE_HINT]
    if need:
        lines.append("Your instrument must include at least: "
                     + "; ".join(need) + ".")
    if params:
        ps = ", ".join(f"{k}={v}" for k, v in params.items())
        lines.append(f"Run with: {ps}.")
    return "\n".join(lines)


def monitor_roles(name):
    """Gradable roles from the inventory run outputs (events >= 1000)."""
    base = os.path.join(REPO, "runs", "inventory", name.replace("-", "_"))
    runs = sorted(
        (d for d in os.listdir(base) if os.path.isdir(os.path.join(base, d))),
        key=lambda d: os.path.getmtime(os.path.join(base, d))) if os.path.isdir(base) else []
    if not runs:
        return []
    summary = results.summarize(os.path.join(base, runs[-1]))
    roles = []
    for role in ("2d", "wavelength", "energy", "tof"):
        m = grader.match_monitor(summary, role)
        if m and (m.get("events") or 0) >= 1000:
            roles.append(role)
    return roles


def main():
    os.makedirs(TASK_DIR, exist_ok=True)
    inv = {e["name"]: e for e in json.load(
        open(os.path.join(REPO, "benchmark", "inventory.json")))["instruments"]}
    manual, written = [], []
    for name, klass, split in TARGET:
        e = inv.get(name)
        if not e or e["status"] != "ok":
            manual.append((name, f"not runnable ({e['status'] if e else 'absent'})"))
            continue
        reg_name = "t1_" + re.sub(r"\W", "_", name).lower()
        try:
            if registry.exists(reg_name):
                spec = registry.load(reg_name)
            else:
                spec, warnings = registry.load_from_instr(e["path"], name=reg_name)
                if warnings:
                    manual.append((name, f"import warnings: {warnings[:2]}"))
                    continue
        except SpecError as ex:
            manual.append((name, f"reader: {str(ex)[:120]}"))
            continue
        roles = monitor_roles(name)
        if not roles:
            manual.append((name, "no gradable monitor roles at curation stats"))
            continue
        params = e.get("example_params", {})
        task = {
            "id": f"T1_{name.replace('-', '_')}",
            "tier": "T1",
            "split": split,
            "kind": "reproduce",
            "class": klass,
            "paper": PAPERS.get(name),
            "prompt": render_prompt(name, klass, spec, roles, params),
            "reference": {"instr": f"shipped:{name}", "parameters": params},
            "protocol": {"ncount": 1e6, "seed": 1234, "timeout": 900},
            "grading": {
                "min_events": 1000,
                "monitors": [{"role": r, "observables": ROLE_OBSERVABLES[r]}
                             for r in roles],
            },
            "notes": f"Auto-authored from the reference via reader->spec "
                     f"converter on 2026-07-24; %Example check: "
                     f"{e.get('example_check', {}).get('agree')}",
        }
        out = os.path.join(TASK_DIR, task["id"] + ".json")
        with open(out, "w") as f:
            json.dump(task, f, indent=2)
        written.append((task["id"], split, klass, len(spec["components"]), roles))
    print(f"authored {len(written)} tasks -> {TASK_DIR}")
    for t in written:
        print(f"  {t[0]:28} {t[1]:5} {t[2]:24} {t[3]:3} comps  roles={','.join(t[4])}")
    if manual:
        print("\nneeds manual authoring:")
        for n, why in manual:
            print(f"  {n:22} {why}")


if __name__ == "__main__":
    main()
