"""Procedural instance generator — template families, nothing memorizable.

A family is ONE fixed-topology .instr whose geometry ("context") and
design knobs ("free parameters") are ALL instrument parameters, so each
family compiles exactly once and every instance is just a parameter
assignment (compile-once / sample-many; topology variation is slow-tier
only, out of M5 scope). Instances are drawn deterministically from
(family, split, index) — reproducible forever, infinite supply, and the
eval split lives in HELD-OUT parameter regimes: at least one context
parameter is sampled from an interval disjoint from the train range, so
memorizing train instances cannot cover evaluation.

Families descend from the two calibrated T2 baselines
(benchmark/instruments/), generalized: guide_divergence (deliver flux
through a guide, divergence-band constraint) and sans_collimation
(pinhole SANS, scattering flux vs collimation, beamstop + band
constraints — the red-teamed beamstop-leakage lesson baked in).
"""

import os
import random

# --------------------------------------------------------------------------------
# family definitions
# --------------------------------------------------------------------------------

GUIDE_INSTR = """\
DEFINE INSTRUMENT fam_guide_divergence(double src_wh=0.10, double L_in=1.5,
  double L_guide=10, double wl=5.0, double dwl=0.5, double det_wh=0.02,
  double w_in=0.02, double w_out=0.02, double m_coat=2.0)
TRACE
COMPONENT src = Source_simple(xwidth=src_wh, yheight=src_wh, dist=L_in,
  focus_xw=w_in, focus_yh=w_in, lambda0=wl, dlambda=dwl)
AT (0, 0, 0) ABSOLUTE

COMPONENT guide = Guide(w1=w_in, h1=w_in, w2=w_out, h2=w_out,
  l=L_guide, m=m_coat)
AT (0, 0, L_in) RELATIVE src

COMPONENT divmon = Divergence_monitor(filename="div.dat", xwidth=det_wh,
  yheight=det_wh, maxdiv_h=0.5, maxdiv_v=0.5, restore_neutron=1)
AT (0, 0, L_guide+0.05) RELATIVE guide

COMPONENT psd = PSD_monitor(nx=60, ny=60, filename="psd.dat",
  xwidth=1.5*det_wh, yheight=1.5*det_wh, restore_neutron=1)
AT (0, 0, L_guide+0.06) RELATIVE guide
END
"""

SANS_INSTR = """\
DEFINE INSTRUMENT fam_sans_collimation(double src_r=0.02, double L_coll=3.0,
  double wl=6.0, double r_sphere=100, double det_dist=3.0,
  double r_pin1=0.005, double r_pin2=0.005)
TRACE
COMPONENT arm = Arm()
AT (0, 0, 0) ABSOLUTE

COMPONENT source = Source_simple(radius=src_r, dist=3, focus_xw=0.01,
  focus_yh=0.01, lambda0=wl, dlambda=0.05*wl, flux=1e8)
AT (0, 0, 0) RELATIVE arm

COMPONENT coll1 = Slit(radius=r_pin1)
AT (0, 0, 3) RELATIVE arm

COMPONENT coll2 = Slit(radius=r_pin2)
AT (0, 0, 3+L_coll) RELATIVE arm

SPLIT 10 COMPONENT sample = Sans_spheres(R=r_sphere, Phi=0.001,
  Delta_rho=0.6, sigma_abs=0.5, xwidth=0.01, yheight=0.01, zdepth=0.005,
  target_index=2, focus_xw=0.6, focus_yh=0.6)
AT (0, 0, 0.2) RELATIVE coll2

COMPONENT STOP = Beamstop(radius=0.02)
AT (0, 0, det_dist-0.1) RELATIVE sample

COMPONENT detector = PSD_monitor(nx=128, ny=128, filename="PSD.dat",
  xmin=-0.3, xmax=0.3, ymin=-0.3, ymax=0.3, restore_neutron=1)
AT (0, 0, det_dist) RELATIVE sample

COMPONENT Ldetector = L_monitor(nL=200, filename="Ldet.dat", xmin=-0.3,
  xmax=0.3, ymin=-0.3, ymax=0.3, Lmin=0.8*wl, Lmax=1.2*wl,
  restore_neutron=1)
AT (0, 0, det_dist+0.01) RELATIVE sample
END
"""

FAMILIES = {
    "guide_divergence": {
        "instr_name": "fam_guide_divergence",
        "instr": GUIDE_INSTR,
        "description": ("Deliver neutrons through a straight supermirror "
                        "guide onto a small divergence monitor; maximize "
                        "delivered intensity without blowing up the beam "
                        "divergence."),
        # context: sampled per instance; heldout ranges DISJOINT where given
        "context": {
            "src_wh":  {"train": (0.06, 0.14), "heldout": (0.06, 0.14)},
            "L_in":    {"train": (1.0, 2.0),   "heldout": (1.0, 2.0)},
            "L_guide": {"train": (6.0, 12.0),  "heldout": (12.5, 16.0)},
            "wl":      {"train": (3.0, 8.0),   "heldout": (3.0, 8.0)},
            "dwl":     {"train": (0.3, 1.0),   "heldout": (0.3, 1.0)},
            "det_wh":  {"train": (0.015, 0.03), "heldout": (0.015, 0.03)},
        },
        "free_parameters": {"w_in": (0.01, 0.09), "w_out": (0.01, 0.09),
                            "m_coat": (1.0, 3.0)},
        "baseline": {"w_in": 0.012, "w_out": 0.012, "m_coat": 1.5},
        "fom": {"monitor": "divmon", "metric": "intensity", "maximize": True},
        # band constraints vs baseline observables (anti-hacking: max-only
        # constraints have the wrong sign for leakage-class exploits)
        "constraints": [
            {"monitor": "divmon", "observable": "beam_width_x",
             "band": (0.3, 3.0)},
            {"monitor": "psd", "observable": "beam_width_x",
             "band": (0.3, 3.0)},
        ],
    },
    "sans_collimation": {
        "instr_name": "fam_sans_collimation",
        "instr": SANS_INSTR,
        "description": ("Pinhole SANS: choose the two collimation pinhole "
                        "radii to maximize scattered intensity on the "
                        "detector while keeping the direct beam on the "
                        "beamstop and the scattering pattern intact."),
        "context": {
            "src_r":    {"train": (0.015, 0.03), "heldout": (0.015, 0.03)},
            "L_coll":   {"train": (2.0, 4.0),    "heldout": (4.5, 6.0)},
            "wl":       {"train": (4.0, 8.0),    "heldout": (4.0, 8.0)},
            "r_sphere": {"train": (50.0, 150.0), "heldout": (50.0, 150.0)},
            "det_dist": {"train": (2.5, 3.5),    "heldout": (2.5, 3.5)},
        },
        "free_parameters": {"r_pin1": (0.001, 0.02), "r_pin2": (0.001, 0.02)},
        "baseline": {"r_pin1": 0.005, "r_pin2": 0.005},
        "fom": {"monitor": "detector", "metric": "intensity",
                "maximize": True},
        "constraints": [
            # pattern integrity: scattering ring width must stay in band —
            # beam leakage past the stop SHRINKS apparent width (the caught
            # exploit), a wider band only means more smearing
            {"monitor": "detector", "observable": "beam_width_x",
             "band": (0.4, 2.5)},
            # wavelength band integrity at the detector
            {"monitor": "Ldetector", "observable": "center_of_mass",
             "band": (0.9, 1.1)},
        ],
    },
}

PROTOCOL = {"ncount_cheap": 1e4, "ncount": 1e5, "statistics_floor": 500}


def family_instr(family: str, workdir: str) -> str:
    """Materialize the family's .instr (stable content — the executor's
    comment-stripped sha keeps the compiled binary cached across calls)."""
    fam = FAMILIES[family]
    d = os.path.join(workdir, family)
    os.makedirs(d, exist_ok=True)
    path = os.path.join(d, fam["instr_name"] + ".instr")
    if not os.path.isfile(path) or open(path).read() != fam["instr"]:
        with open(path, "w") as f:
            f.write(fam["instr"])
    return path


def instance(family: str, split: str, index: int) -> dict:
    """Deterministic instance: (family, split, index) -> the same task
    forever. split: 'train' | 'heldout' (held-out parameter regimes)."""
    fam = FAMILIES[family]
    if split not in ("train", "heldout"):
        raise ValueError(f"split must be train|heldout (got {split!r})")
    rng = random.Random(f"{family}/{split}/{index}")
    context = {k: round(rng.uniform(*rr[split]), 6)
               for k, rr in fam["context"].items()}
    return {
        "id": f"{family}-{split}-{index:06d}",
        "family": family,
        "split": split,
        "index": index,
        "description": fam["description"],
        "context": context,
        "free_parameters": {k: list(v)
                            for k, v in fam["free_parameters"].items()},
        "baseline": dict(fam["baseline"]),
        "fom": dict(fam["fom"]),
        "constraints": [dict(c) for c in fam["constraints"]],
        "target_ratio": 1.0,  # L4 pass bar: fom >= target_ratio * baseline fom
        # rng is seeded from a string (deterministic across processes);
        # never use hash() here — string hashing is per-process randomized
        "protocol": {**PROTOCOL, "seed": 1 + rng.randrange(2**31 - 1)},
    }


def render_prompt(inst: dict, baseline_obs: dict | None = None) -> str:
    """Natural-language task sheet for LLM agents (reference loop / SFT)."""
    lines = [
        f"Instrument design task {inst['id']}.",
        "",
        inst["description"],
        "",
        "Fixed geometry for this instance (instrument parameters you must "
        "pass but may NOT change):",
    ]
    lines += [f"  {k} = {v}" for k, v in inst["context"].items()]
    lines += ["", "Design parameters you control (bounds inclusive):"]
    lines += [f"  {k} in [{lo}, {hi}]  (baseline {inst['baseline'][k]})"
              for k, (lo, hi) in inst["free_parameters"].items()]
    f = inst["fom"]
    lines += ["", f"Figure of merit: {f['metric']} on monitor "
              f"'{f['monitor']}' "
              f"({'maximize' if f['maximize'] else 'minimize'})."]
    base_fom = (baseline_obs or {}).get("fom")
    tr = inst.get("target_ratio", 1.0)
    if base_fom:
        lines += [f"Baseline FOM at the evaluation protocol: {base_fom:.6g}"]
        if inst.get("target_calibrated"):
            # the model must know the bar it is held to: the target is
            # 80% of a constraint-filtered, fresh-seed-verified classical
            # optimum, not merely "beat the baseline" (2026-09-13)
            lines += [f"TARGET TO BEAT: {base_fom * tr:.6g} "
                      f"({tr:.2f}x the baseline). This target is 80% of what "
                      f"a classical constraint-filtered random search "
                      f"achieves on this instance, so beating the baseline "
                      f"alone is NOT sufficient."]
        else:
            lines += ["Target: beat the baseline configuration."]
    lines += ["", "Constraints (checked against the baseline's pattern — "
              "stay within band):"]
    lines += [f"  {c['monitor']}.{c['observable']} within "
              f"[{c['band'][0]}x, {c['band'][1]}x] of baseline"
              for c in inst["constraints"]]
    lines += ["", "Evaluation runs at a fixed protocol (ncount "
              f"{inst['protocol']['ncount']:g}, env-controlled seed); "
              "iterate however you like, the graded run is the env's."]
    return "\n".join(lines)
