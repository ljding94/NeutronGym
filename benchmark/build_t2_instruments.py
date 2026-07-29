"""Build the T2 baseline instruments deterministically via the registry.

T2 tasks need references whose improvable knobs are instrument parameters
(mcrun can only scan/optimize those). Both current T2 tasks (guide_divergence,
sans_collimation) use instruments built here.

Outputs committed to benchmark/instruments/<name>/: the generated .instr +
spec.json (diffable provenance).

Usage: conda run -n mcstas python benchmark/build_t2_instruments.py
"""

import json
import os
import shutil
import tempfile

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(REPO, "benchmark", "instruments")

os.environ["MCSTAS_MCP_HOME"] = tempfile.mkdtemp(prefix="t2_build_")

from mcstas_mcp import examples, registry  # noqa: E402


def guide_divergence():
    """The M4 acceptance instrument, formalized: 10 m guide feeding a
    2x2 cm +-0.5 deg acceptance. Free knobs: entry/exit widths + m."""
    spec = registry.create("t2_guide_divergence",
                          "T2 baseline: guide feeding a divergence-limited target")
    registry.add_parameter(spec, "w_in", default=0.02, unit="m", comment="guide entry width/height")
    registry.add_parameter(spec, "w_out", default=0.02, unit="m", comment="guide exit width/height")
    registry.add_parameter(spec, "m_coat", default=2.0, unit="1", comment="supermirror m-value")
    registry.add_component(spec, "src", "Source_simple", at=[0, 0, 0], parameters={
        "xwidth": 0.1, "yheight": 0.1, "dist": 1.5, "focus_xw": "w_in",
        "focus_yh": "w_in", "lambda0": 5, "dlambda": 0.5})
    registry.add_component(spec, "guide", "Guide", at=[0, 0, 1.5], relative="src",
                           parameters={"w1": "w_in", "h1": "w_in", "w2": "w_out",
                                       "h2": "w_out", "l": 10, "m": "m_coat"})
    registry.add_component(spec, "divmon", "Divergence_monitor",
                           at=[0, 0, 10.05], relative="guide",
                           parameters={"xwidth": 0.02, "yheight": 0.02,
                                       "maxdiv_h": 0.5, "maxdiv_v": 0.5,
                                       "filename": "div.dat", "restore_neutron": 1})
    registry.add_component(spec, "psd", "PSD_monitor", at=[0, 0, 10.06],
                           relative="guide",
                           parameters={"nx": 60, "ny": 60, "xwidth": 0.03,
                                       "yheight": 0.03, "filename": "psd.dat",
                                       "restore_neutron": 1})
    return spec


def sans_collimation():
    """templateSANS with the collimation exposed as parameters: pinhole radii
    and collimation length become knobs. Flux/resolution trade-off."""
    src = examples.get_example("templateSANS")["path"]
    spec, warnings = registry.load_from_instr(src, name="t2_sans_collimation")
    assert not warnings, warnings
    registry.add_parameter(spec, "r_pin1", default=0.005, unit="m",
                           comment="first pinhole radius")
    registry.add_parameter(spec, "r_pin2", default=0.005, unit="m",
                           comment="sample pinhole radius")
    registry.set_parameters(spec, "coll1", {"radius": "r_pin1"})
    registry.set_parameters(spec, "coll2", {"radius": "r_pin2"})
    spec["description"] = "T2 baseline: templateSANS with parameterized collimation"
    registry.save(spec)
    return spec


def export(spec):
    d = os.path.join(OUT, spec["name"])
    os.makedirs(d, exist_ok=True)
    instr = registry.build_instr_file(spec)
    shutil.copy(instr, os.path.join(d, f"{spec['name']}.instr"))
    with open(os.path.join(d, "spec.json"), "w") as f:
        json.dump(spec, f, indent=2)
    print(f"built {spec['name']}: {len(spec['components'])} components, "
          f"params: {[p['name'] for p in spec['parameters']]}")


def main():
    os.makedirs(OUT, exist_ok=True)
    export(guide_divergence())
    export(sans_collimation())


if __name__ == "__main__":
    main()
