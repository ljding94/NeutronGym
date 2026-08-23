"""Held-out T1 slice (M5.5 contamination item b): references WE author.

The held-out property: no public `.instr` of these instruments exists
anywhere (verified in note/study-paper-pairs-2026-07-24.md §D), so no
model's training data can contain the reference — the only way to pass is
to follow the spec sheet. Our references are *modeled after* the published
instrument descriptions (sources cited per task), deliberately simplified,
and labeled as NOT facility-validated: benchmark validity needs
self-consistency + the never-public property, not facility fidelity.
(Committing them publishes them from today — the per-model memorization
probe remains the operative test, as for every task.)

Instruments:
  BOYA  (CARR, arXiv:2501.01143) — multiplexing cold-neutron spectrometer;
        modeled as ONE elastic analyzing channel: cold guide -> PG(002)
        monochromator -> sample position -> PG(002) analyzer -> detector.
  VENUS (SNS BL-10, conceptual design 10.1016/j.phpro.2015.07.023) — 25 m
        TOF imaging beamline: moderator -> selectable pinhole aperture ->
        beam scraper -> 20 m sample plane -> 20x20 cm detector at 25 m.

Same pipeline as the seen tier: build via registry -> export committed
.instr -> reader-loadback -> spec-sheet prompt -> role-derived grading
contract -> fresh-seed self-validation.

Usage: conda run -n mcstas python benchmark/harness/build_heldout_tasks.py
"""

import json
import os
import sys
import tempfile

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))
os.environ.setdefault("MCSTAS_MCP_HOME", tempfile.mkdtemp(prefix="heldout_"))

import author_tasks  # noqa: E402
import grader  # noqa: E402
import validate_tasks  # noqa: E402

from mcstas_mcp import registry, results  # noqa: E402

OUT_INSTR = os.path.join(REPO, "benchmark", "instruments", "heldout")
TASK_DIR = os.path.join(REPO, "benchmark", "tasks", "T1")


def build_boya():
    """Single elastic channel of BOYA (CARR): kf = 1.55 A^-1 -> lambda
    4.05 A; PG(002) d = 3.355 A -> Bragg 37.14 deg."""
    s = registry.create("boya_channel",
                        "BOYA single-channel model (NeutronGym-authored)")
    registry.add_parameter(s, "lam", default=4.05, unit="AA",
                           comment="working wavelength (kf=1.55 inv-AA)")
    registry.add_parameter(s, "A1", default=37.14, unit="deg",
                           comment="PG(002) Bragg angle at lam")
    registry.add_parameter(s, "A2", default=74.28, unit="deg",
                           comment="monochromator take-off (2*A1)")
    registry.add_component(s, "src", "Source_simple", at=[0, 0, 0],
                           parameters={"xwidth": 0.03, "yheight": 0.12,
                                       "dist": 1.0, "focus_xw": 0.03,
                                       "focus_yh": 0.12, "lambda0": "lam",
                                       "dlambda": 0.4, "flux": 1e12})
    registry.add_component(s, "guide", "Guide", at=[0, 0, 1.0],
                           relative="src",
                           parameters={"w1": 0.03, "h1": 0.12, "w2": 0.03,
                                       "h2": 0.12, "l": 20.0, "m": 2.0})
    registry.add_component(s, "mono_arm", "Arm", at=[0, 0, 21.2],
                           relative="src")
    registry.add_component(s, "mono", "Monochromator_flat",
                           at=[0, 0, 0], relative="mono_arm",
                           rotated=[0, "A1", 0], rotated_relative="mono_arm",
                           parameters={"zwidth": 0.15, "yheight": 0.12,
                                       "mosaich": 40, "mosaicv": 40,
                                       "DM": 3.355})
    registry.add_component(s, "out_arm", "Arm", at=[0, 0, 0],
                           relative="mono_arm", rotated=[0, "A2", 0],
                           rotated_relative="mono_arm")
    registry.add_component(s, "mono_lam", "L_monitor",
                           at=[0, 0, 0.8], relative="out_arm",
                           parameters={"xwidth": 0.06, "yheight": 0.12,
                                       "nL": 200, "Lmin": "lam-1",
                                       "Lmax": "lam+1",
                                       "filename": "mono_lam.dat",
                                       "restore_neutron": 1})
    registry.add_component(s, "sample_psd", "PSD_monitor",
                           at=[0, 0, 1.5], relative="out_arm",
                           parameters={"xwidth": 0.05, "yheight": 0.10,
                                       "nx": 60, "ny": 60,
                                       "filename": "sample_psd.dat",
                                       "restore_neutron": 1})
    registry.add_component(s, "ana_arm", "Arm", at=[0, 0, 2.5],
                           relative="out_arm")
    registry.add_component(s, "ana", "Monochromator_flat",
                           at=[0, 0, 0], relative="ana_arm",
                           rotated=[0, "-A1", 0], rotated_relative="ana_arm",
                           parameters={"zwidth": 0.12, "yheight": 0.12,
                                       "mosaich": 40, "mosaicv": 40,
                                       "DM": 3.355})
    registry.add_component(s, "det_arm", "Arm", at=[0, 0, 0],
                           relative="ana_arm", rotated=[0, "-A2", 0],
                           rotated_relative="ana_arm")
    registry.add_component(s, "det", "PSD_monitor", at=[0, 0, 0.8],
                           relative="det_arm",
                           parameters={"xwidth": 0.06, "yheight": 0.12,
                                       "nx": 60, "ny": 60,
                                       "filename": "det.dat",
                                       "restore_neutron": 1})
    return s


def build_venus():
    """VENUS (SNS BL-10) TOF imaging line, conceptual-design geometry:
    selectable pinhole at 1 m, scraper at 10 m, sample plane 20 m,
    20x20 cm detector at 25 m."""
    s = registry.create("venus_imaging",
                        "VENUS imaging beamline model (NeutronGym-authored)")
    registry.add_parameter(s, "lam", default=3.5, unit="AA",
                           comment="band center (bandwidth-chopper limited)")
    registry.add_parameter(s, "dlam", default=1.3, unit="AA",
                           comment="half band width")
    registry.add_parameter(s, "ap_D", default=0.016, unit="m",
                           comment="selectable pinhole aperture diameter")
    registry.add_component(s, "src", "Source_simple", at=[0, 0, 0],
                           parameters={"xwidth": 0.10, "yheight": 0.12,
                                       "dist": 1.0, "focus_xw": "ap_D",
                                       "focus_yh": "ap_D", "lambda0": "lam",
                                       "dlambda": "dlam", "flux": 1e13})
    registry.add_component(s, "aperture", "Slit", at=[0, 0, 1.0],
                           relative="src",
                           parameters={"radius": "0.5*ap_D"})
    registry.add_component(s, "scraper", "Slit", at=[0, 0, 10.0],
                           relative="src",
                           parameters={"xwidth": 0.30, "yheight": 0.30})
    registry.add_component(s, "fov_mon", "PSD_monitor", at=[0, 0, 20.0],
                           relative="src",
                           parameters={"xwidth": 0.28, "yheight": 0.28,
                                       "nx": 128, "ny": 128,
                                       "filename": "fov.dat",
                                       "restore_neutron": 1})
    registry.add_component(s, "det", "PSD_monitor", at=[0, 0, 25.0],
                           relative="src",
                           parameters={"xwidth": 0.20, "yheight": 0.20,
                                       "nx": 256, "ny": 256,
                                       "filename": "det.dat",
                                       "restore_neutron": 1})
    registry.add_component(s, "det_lam", "L_monitor", at=[0, 0, 25.01],
                           relative="src",
                           parameters={"xwidth": 0.20, "yheight": 0.20,
                                       "nL": 200, "Lmin": "lam-2*dlam",
                                       "Lmax": "lam+2*dlam",
                                       "filename": "det_lam.dat",
                                       "restore_neutron": 1})
    return s


HELDOUT = [
    {"build": build_boya, "name": "boya_channel", "id": "T1_BOYA_CARR",
     "params": {"lam": 4.05, "A1": 37.14, "A2": 74.28},
     "klass": "multiplexing-spectrometer-channel",
     "probe_hint": "the BOYA multiplexing cold-neutron spectrometer at the "
                   "China Advanced Research Reactor (CARR)",
     "paper": {"cite": "Boya multiplexing cold neutron spectrometer at "
                       "CARR (2025)", "doi": None,
               "access": "OA preprint arXiv:2501.01143"},
     "provenance": "Reference authored for NeutronGym 2026-08-23 as a "
                   "SINGLE-CHANNEL simplification of the published design "
                   "(34-channel Rowland-focusing analyzer bank reduced to "
                   "one elastic channel); no public .instr of BOYA existed "
                   "at authoring (held-out re-check 2026-07-24). NOT a "
                   "facility-validated model."},
    {"build": build_venus, "name": "venus_imaging", "id": "T1_VENUS_SNS",
     "params": {"lam": 3.5, "dlam": 1.3, "ap_D": 0.016},
     "klass": "tof-imaging-beamline",
     "probe_hint": "the VENUS time-of-flight neutron imaging beamline at "
                   "the SNS (BL-10)",
     "paper": {"cite": "Overview of the conceptual design of the future "
                       "VENUS neutron imaging beam line at SNS, Physics "
                       "Procedia 69 (2015)",
               "doi": "10.1016/j.phpro.2015.07.023", "access": "OA"},
     "provenance": "Reference authored for NeutronGym 2026-08-23 from the "
                   "published conceptual-design geometry (choppers modeled "
                   "as the delivered wavelength band; He flight tubes "
                   "omitted); no public .instr of VENUS existed at "
                   "authoring (held-out re-check 2026-07-24). NOT a "
                   "facility-validated model."},
]


def main():
    os.makedirs(OUT_INSTR, exist_ok=True)
    for h in HELDOUT:
        spec = h["build"]()
        dest_dir = os.path.join(OUT_INSTR, h["name"])
        os.makedirs(dest_dir, exist_ok=True)
        instr_path = registry.export_instr(
            spec, dest=os.path.join(dest_dir, h["name"] + ".instr"))
        rel = os.path.relpath(instr_path, REPO)

        # curation run at protocol stats -> gradable roles (same bar as
        # seen). All params explicit — M0 gotcha: a param-less CLI makes
        # the binary prompt interactively and die
        run = grader.run_protocol(instr_path, h["params"],
                                  {"ncount": 1e6, "seed": 1234,
                                   "timeout": 900},
                                  os.path.join(REPO, "runs", "heldout",
                                               h["name"]), "curate")
        if not run.get("ok"):
            print(f"FAIL {h['id']}: curation run failed: "
                  f"{(run.get('diagnostics') or [])[-4:]}")
            continue
        roles = []
        for role in ("2d", "wavelength", "energy", "tof"):
            m = grader.match_monitor(run, role)
            if m and (m.get("events") or 0) >= 1000:
                roles.append(role)
        if not roles:
            print(f"FAIL {h['id']}: no gradable roles "
                  f"(events: {[(m['component'], m['events']) for m in run['monitors']]})")
            continue

        # reader-loadback so the prompt rides the same spec pipeline
        loaded, warnings = registry.load_from_instr(
            instr_path, name=h["name"] + "_loadback")
        task = {
            "id": h["id"], "tier": "T1", "split": "heldout",
            "kind": "reproduce", "class": h["klass"], "paper": h["paper"],
            "prompt": author_tasks.render_prompt(
                h["name"], h["klass"], loaded, roles, h["params"]),
            "reference": {"instr": rel, "parameters": h["params"]},
            "protocol": {"ncount": 1e6, "seed": 1234, "timeout": 900},
            "grading": {"min_events": 1000,
                        "monitors": [{"role": r,
                                      "observables":
                                      author_tasks.ROLE_OBSERVABLES[r]}
                                     for r in roles]},
            "probe_hint": h["probe_hint"],
            "provenance": h["provenance"],
            "notes": "Held-out tier (M5.5): reference never public; see "
                     "provenance.",
        }
        out = os.path.join(TASK_DIR, task["id"] + ".json")
        with open(out, "w") as f:
            json.dump(task, f, indent=1)

        val = validate_tasks.validate(out)
        status = "OK  " if val["pass"] else "FAIL-VALIDATE"
        print(f"{status} {task['id']:16} roles={','.join(roles)} "
              f"self-validation={val.get('checks')} "
              f"monitors={[(m['component'], int(m['events'])) for m in run['monitors']]}")


if __name__ == "__main__":
    main()
