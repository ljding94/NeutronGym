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

import hashlib
import json
import math
import os
import random

# --------------------------------------------------------------------------------
# family definitions
# --------------------------------------------------------------------------------

GUIDE_INSTR = """\
DEFINE INSTRUMENT fam_guide_divergence(double src_wh=0.10, double L_in=1.5,
  double L_guide=10, double wl=5.0, double dwl=0.5, double det_wh=0.02,
  double w_in=0.02, double w_out=0.02, double m_coat=2.0, double div_max=0.5)
TRACE
COMPONENT src = Source_simple(xwidth=src_wh, yheight=src_wh, dist=L_in,
  focus_xw=w_in, focus_yh=w_in, lambda0=wl, dlambda=dwl)
AT (0, 0, 0) ABSOLUTE

COMPONENT guide = Guide(w1=w_in, h1=w_in, w2=w_out, h2=w_out,
  l=L_guide, m=m_coat)
AT (0, 0, L_in) RELATIVE src

COMPONENT divmon = Divergence_monitor(filename="div.dat", xwidth=det_wh,
  yheight=det_wh, maxdiv_h=div_max, maxdiv_v=div_max, restore_neutron=1)
AT (0, 0, L_guide+0.05) RELATIVE guide

COMPONENT psd = PSD_monitor(nx=60, ny=60, filename="psd.dat",
  xwidth=1.5*det_wh, yheight=1.5*det_wh, restore_neutron=1)
AT (0, 0, L_guide+0.06) RELATIVE guide
END
"""

SANS_INSTR = """\
DEFINE INSTRUMENT fam_sans_collimation(double src_r=0.02, double L_coll=3.0,
  double wl=6.0, double r_sphere=100, double det_dist=3.0, double stop_r=0.02,
  double sample_wh=0.01, double r_pin1=0.005, double r_pin2=0.005)
TRACE
COMPONENT arm = Arm()
AT (0, 0, 0) ABSOLUTE

COMPONENT source = Source_simple(radius=src_r, dist=3, focus_xw=0.045,
  focus_yh=0.045, lambda0=wl, dlambda=0.05*wl, flux=1e8)
AT (0, 0, 0) RELATIVE arm

COMPONENT coll1 = Slit(radius=r_pin1)
AT (0, 0, 3) RELATIVE arm

COMPONENT coll2 = Slit(radius=r_pin2)
AT (0, 0, 3+L_coll) RELATIVE arm

SPLIT 60 COMPONENT sample = Sans_spheres(R=r_sphere, Phi=0.04,
  Delta_rho=0.6, sigma_abs=0.5, xwidth=sample_wh, yheight=sample_wh, zdepth=0.005,
  target_index=2, focus_xw=0.6, focus_yh=0.6)
AT (0, 0, 0.2) RELATIVE coll2

COMPONENT STOP = Beamstop(radius=stop_r)
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
                        "guide onto a small sample; maximize the intensity "
                        "delivered within the instance's divergence limit. "
                        "The specification: the supermirror critical angle "
                        "m_coat x 0.099 deg/Angstrom x wl must not exceed "
                        "div_max (degrees), only neutrons within +/-div_max "
                        "are counted, and the guide exit may not be wider "
                        "than the sample (w_out <= det_wh)."),
        # context: sampled per instance; heldout ranges DISJOINT where given
        "context": {
            "src_wh":  {"train": (0.06, 0.14), "heldout": (0.06, 0.14)},
            "L_in":    {"train": (1.0, 2.0),   "heldout": (1.0, 2.0)},
            "L_guide": {"train": (6.0, 12.0),  "heldout": (12.5, 16.0)},
            "wl":      {"train": (3.0, 8.0),   "heldout": (3.0, 8.0)},
            "dwl":     {"train": (0.3, 1.0),   "heldout": (0.3, 1.0)},
            "det_wh":  {"train": (0.015, 0.03), "heldout": (0.015, 0.03)},
            # divergence specification (2026-09-15). Appended LAST so every
            # earlier context draw is unchanged. Floor 0.85 deg keeps the
            # m_coat = 1 baseline valid at the longest wavelength (8 A -> 0.79)
            "div_max": {"train": (0.85, 2.4), "heldout": (0.85, 2.4)},
        },
        "free_parameters": {"w_in": (0.01, 0.09), "w_out": (0.01, 0.09),
                            "m_coat": (1.0, 3.0)},
        "baseline": {"w_in": 0.012, "w_out": 0.012, "m_coat": 1.0},
        "static_checks": ["guide_divergence_spec", "guide_beam_size_spec"],
        # div_max is DERIVED from wl so the divergence spec always binds; the
        # range below is only the raw draw, which guide_context overwrites
        "context_fn": "guide_context",
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
                        "beamstop and the scattering pattern intact. The "
                        "beamstop radius stop_r is given per instance; a "
                        "configuration whose "
                        "unscattered beam is wider than the beamstop is "
                        "rejected before simulation. The sample's sphere "
                        "radius r_sphere (Angstrom) sets a resolution "
                        "requirement q_min <= 1/r_sphere: the unscattered "
                        "beam at the detector must stay within "
                        "wl x det_dist / (2 pi r_sphere) of the axis. The "
                        "beam must also fit the sample (radius at the sample "
                        "<= sample_wh / 2); illuminating past it only adds "
                        "holder background."),
        "context": {
            "src_r":    {"train": (0.015, 0.03), "heldout": (0.015, 0.03)},
            # train floor 2.5 m (was 2.0): at 2.4 m and the longest detector
            # distance the BASELINE pinholes' direct beam is exactly 0.020 m,
            # on the 0.02 m stop's edge; 2.5 m gives 0.0194 m, so the baseline
            # passes the direct-beam check on every train instance (2026-09-13)
            "L_coll":   {"train": (2.5, 4.0),    "heldout": (4.5, 6.0)},
            "wl":       {"train": (4.0, 8.0),    "heldout": (4.0, 8.0)},
            "r_sphere": {"train": (50.0, 150.0), "heldout": (50.0, 150.0)},
            "det_dist": {"train": (2.5, 3.5),    "heldout": (2.5, 3.5)},
            # beamstop radius, per instance (2026-09-15). Appended LAST so
            # every earlier context draw is unchanged. A FIXED 0.02 m stop was
            # the binding constraint on 86% of instances, so every instance had
            # the same feasible corner and one configuration fitted most of
            # them; the floor 0.011 keeps the 2.5 mm baseline valid (worst case
            # 0.0095 m at the stop plane).
            "stop_r":   {"train": (0.011, 0.025), "heldout": (0.011, 0.025)},
            # sample size, per instance (2026-09-15). Appended LAST so earlier
            # draws are unchanged. It bounds r_pin2 (illuminating past the
            # sample only adds holder background) INDEPENDENTLY of the
            # beamstop/resolution bound on r_pin1, so no single configuration
            # fits every instance's feasible set. Floor 0.007 keeps the 2.5 mm
            # baseline valid (its beam is ~0.0029 m at the sample).
            "sample_wh": {"train": (0.007, 0.016), "heldout": (0.007, 0.016)},
        },
        "free_parameters": {"r_pin1": (0.001, 0.02), "r_pin2": (0.001, 0.02)},
        # 2.5 mm (was 5 mm, 2026-09-15): the resolution specification's
        # tightest limit is ~0.0106 m at the detector; 5 mm pinholes reach
        # ~0.0198 m on short train collimations
        # nominal only: the real baseline is per instance (baseline_fn),
        # because one fixed action must fit the SMALLEST sample and is then
        # starved everywhere else (0-360 detector events, 2/25 exactly zero)
        "baseline": {"r_pin1": 0.0025, "r_pin2": 0.0025},
        "baseline_fn": "sans_baseline",
        # 8x the shared ncount: the tightest instances (7 mm sample, short
        # collimation) collect ~60 events at 1e5, below the L3 floor of 500
        "protocol": {"ncount": 8e5, "ncount_cheap": 8e4},
        # L1 geometric checks run on (context, action) before any simulation
        "static_checks": ["direct_beam_on_stop", "sans_resolution",
                          "sans_beam_fits_sample"],
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

# SANS sample: volume fraction 0.01 (was 0.001) and SPLIT 30 (was 10),
# 2026-09-15. The resolution specification forces small pinholes, and at the
# old settings the 2.5 mm baseline put only 10-100 events on the detector per
# 1e5-ray run (floor 500), i.e. ~+/-17% noise on the figure of merit.
PROTOCOL = {"ncount_cheap": 1e4, "ncount": 1e5, "statistics_floor": 500}


def family_protocol(family: str) -> dict:
    """PROTOCOL with the family's own overrides.

    ncount is per family (2026-09-15) because families differ by orders of
    magnitude in how many rays reach the FOM monitor. The guide collects
    13k+ events at 1e5; SANS counts only neutrons scattered through two
    pinholes and collected 60 on its tightest instances — under the L3
    floor, so calibration found no valid candidate and the instance kept an
    uncalibrated target. Raising the SHARED ncount would have cost the guide
    6x wall time for statistics it does not need.
    """
    return {**PROTOCOL, **FAMILIES[family].get("protocol", {})}

# geometry of SANS_INSTR, kept next to it so a change to the instrument text
# is a change here too
SANS_SOURCE_TO_COLL1 = 3.0
# focus window 4.5 cm (was 1 cm, 2026-09-15): with 1 cm, every pinhole
# above ~7 mm passed the whole emitted beam, so all such configurations gave
# the IDENTICAL figure of merit and the family had no design trade-off. The
# window now exceeds the largest pinhole, so pinhole size always changes flux.
SANS_FOCUS_HALF_DIAG = (0.045 / 2) * 2 ** 0.5   # focus_xw = focus_yh = 0.045
SANS_COLL2_TO_SAMPLE = 0.2
SANS_STOP_BEFORE_DETECTOR = 0.1
SANS_STOP_RADIUS = 0.02


def sans_direct_beam_radius(context: dict, action: dict) -> float:
    """Largest radius of the UNSCATTERED beam at the SANS beamstop plane.

    Straight-line penumbra through the two pinholes. The source aims at a
    focus window at pinhole 1, so pinhole 1's effective radius is capped at
    that window's half-diagonal (4.5 cm window: the cap no longer binds). Anything above SANS_STOP_RADIUS
    lands on the detector as direct beam, which the total-intensity FOM
    counts as scattering (note/sans-direct-beam-exploit-2026-09-13.md).
    Geometric, so approximate: it ignores gravity and slit edge scattering.
    """
    r1 = min(float(action["r_pin1"]), SANS_FOCUS_HALF_DIAG)
    r2 = float(action["r_pin2"])
    d = (SANS_COLL2_TO_SAMPLE + float(context["det_dist"])
         - SANS_STOP_BEFORE_DETECTOR)
    return r2 + (r1 + r2) * d / float(context["L_coll"])


def sans_stop_radius(context: dict) -> float:
    """The instance's beamstop radius (older contexts: the fixed default)."""
    return float(context.get("stop_r", SANS_STOP_RADIUS))


def sans_direct_beam_leaks(context: dict, action: dict) -> bool:
    return sans_direct_beam_radius(context, action) > sans_stop_radius(context)


def check_direct_beam_on_stop(context: dict, action: dict) -> dict:
    """L1 static check closing the SANS direct-beam hole: an unscattered
    beam wider than the beamstop reaches the detector, and the
    total-intensity FOM would count it as scattering."""
    r, stop = sans_direct_beam_radius(context, action), sans_stop_radius(context)
    if r <= stop:
        return {"pass": True}
    return {"pass": False,
            "detail": (f"direct beam radius {r:.4f} m at the beamstop exceeds "
                       f"this instance's {stop:.4f} m stop; the unscattered "
                       f"beam would reach the detector — narrow the pinholes")}


GUIDE_THETA_C_DEG_PER_AA = 0.099   # Ni critical angle per Angstrom, m = 1


def guide_critical_angle_deg(context: dict, action: dict) -> float:
    return (float(action["m_coat"]) * GUIDE_THETA_C_DEG_PER_AA
            * float(context["wl"]))


def guide_max_m(context: dict) -> float:
    return float(context["div_max"]) / (GUIDE_THETA_C_DEG_PER_AA
                                        * float(context["wl"]))


def check_guide_divergence_spec(context: dict, action: dict) -> dict:
    """The supermirror reflects up to m x 0.099 deg/A x lambda; the
    instance's divergence limit caps that, so the best coating depends on
    wavelength and limit instead of always being the maximum."""
    th, lim = guide_critical_angle_deg(context, action), float(context["div_max"])
    if th <= lim + 1e-12:
        return {"pass": True}
    return {"pass": False,
            "detail": (f"supermirror critical angle {th:.3f} deg (m_coat x "
                       f"0.099 deg/A x wl) exceeds the divergence limit "
                       f"div_max = {lim:.3f} deg; m_coat must be <= "
                       f"{guide_max_m(context):.3f} here")}


def check_guide_beam_size_spec(context: dict, action: dict) -> dict:
    w, lim = float(action["w_out"]), float(context["det_wh"])
    if w <= lim + 1e-12:
        return {"pass": True}
    return {"pass": False,
            "detail": (f"guide exit w_out = {w:.4f} m is wider than the sample "
                       f"(det_wh = {lim:.4f} m)")}


def sans_detector_beam_radius(context: dict, action: dict) -> float:
    """Unscattered-beam radius at the detector plane (straight-line penumbra
    through both pinholes; coll2 -> sample 0.2 m -> detector det_dist)."""
    r1 = min(float(action["r_pin1"]), SANS_FOCUS_HALF_DIAG)
    r2 = float(action["r_pin2"])
    d = SANS_COLL2_TO_SAMPLE + float(context["det_dist"])
    return r2 + (r1 + r2) * d / float(context["L_coll"])


def sans_sample_beam_radius(context: dict, action: dict) -> float:
    """Unscattered-beam radius at the sample plane (0.2 m after pinhole 2)."""
    r1 = min(float(action["r_pin1"]), SANS_FOCUS_HALF_DIAG)
    r2 = float(action["r_pin2"])
    return r2 + (r1 + r2) * SANS_COLL2_TO_SAMPLE / float(context["L_coll"])


def check_sans_beam_fits_sample(context: dict, action: dict) -> dict:
    r, lim = sans_sample_beam_radius(context, action), float(context["sample_wh"]) / 2
    if r <= lim:
        return {"pass": True}
    return {"pass": False,
            "detail": (f"beam radius {r:.4f} m at the sample exceeds half the "
                       f"sample width ({lim:.4f} m, sample_wh = "
                       f"{context['sample_wh']}); the excess only lights the "
                       f"holder — narrow the pinholes")}


SANS_BASELINE_FRACTION = 0.8   # of the tightest allowed beam radius


def sans_baseline(context: dict) -> dict:
    """A sensible-but-unoptimised starting point sized to THIS instance.

    Equal pinholes r give a beam radius r * (1 + 2a) at a plane a lengths
    downstream, so the largest valid r is the tightest of the three
    specifications; the baseline sits at SANS_BASELINE_FRACTION of it.
    """
    L = float(context["L_coll"])
    det = float(context["det_dist"])
    limits = [
        (float(context["sample_wh"]) / 2, SANS_COLL2_TO_SAMPLE / L),
        (sans_stop_radius(context),
         (SANS_COLL2_TO_SAMPLE + det - SANS_STOP_BEFORE_DETECTOR) / L),
        (sans_resolution_limit(context), (SANS_COLL2_TO_SAMPLE + det) / L),
    ]
    r = min(lim / (1 + 2 * a) for lim, a in limits) * SANS_BASELINE_FRACTION
    lo, hi = FAMILIES["sans_collimation"]["free_parameters"]["r_pin1"]
    r = round(min(max(r, lo), hi), 6)
    return {"r_pin1": r, "r_pin2": r}


BASELINE_FNS = {"sans_baseline": sans_baseline}


def sans_resolution_limit(context: dict) -> float:
    """Largest unscattered-beam radius at the detector that still reaches
    q_min <= 1/R: q ~ 2 pi theta / lambda with theta = r / det_dist, both
    lambda and R in Angstrom."""
    return (float(context["wl"]) * float(context["det_dist"])
            / (2 * math.pi * float(context["r_sphere"])))


def check_sans_resolution(context: dict, action: dict) -> dict:
    r, lim = sans_detector_beam_radius(context, action), sans_resolution_limit(context)
    if r <= lim:
        return {"pass": True}
    return {"pass": False,
            "detail": (f"unscattered beam radius {r:.4f} m at the detector "
                       f"exceeds the resolution limit {lim:.4f} m (q_min <= "
                       f"1/r_sphere needs radius <= wl x det_dist / (2 pi "
                       f"r_sphere)); narrow the pinholes")}


# name -> check(context, action) -> {"pass": bool, "detail"?: str}
STATIC_CHECKS = {"direct_beam_on_stop": check_direct_beam_on_stop,
                 "sans_beam_fits_sample": check_sans_beam_fits_sample,
                 "guide_divergence_spec": check_guide_divergence_spec,
                 "guide_beam_size_spec": check_guide_beam_size_spec,
                 "sans_resolution": check_sans_resolution}

# bump when check or baseline LOGIC changes without the family dict changing,
# so family_signature (and every calibration cache keyed on it) moves too
SPEC_VERSION = 2


GUIDE_M_LIMIT_RANGE = (1.2, 2.8)   # strictly inside the m_coat range (1.0, 3.0)


def guide_context(context: dict, rng) -> dict:
    """Make the divergence specification always bind.

    div_max used to be drawn independently of wl, so the implied coating
    limit guide_max_m = div_max / (0.099 * wl) landed at or above 3.0 -- the
    TOP of the m_coat range -- on 47% of train and 46% of held-out instances
    (2026-09-16). On those the spec could not bind at all, and the task
    collapsed to "max out the coating": a universal answer. The best fixed
    answer found by the n=150 probe was exactly m_coat = 3.0, and it won
    precisely where the limit was loose (limit median 4.27 on the instances
    it solved vs 2.54 on the rest).

    Draw the LIMIT strictly inside the range instead and derive div_max from
    it, so every instance has a binding, instance-specific divergence bar and
    the maximum coating is never legal anywhere.
    """
    m_limit = rng.uniform(*GUIDE_M_LIMIT_RANGE)
    context["div_max"] = round(
        m_limit * GUIDE_THETA_C_DEG_PER_AA * context["wl"], 6)
    return context


CONTEXT_FNS = {"guide_context": guide_context}


def family_signature(family: str) -> str:
    """Short hash of everything that defines a family's tasks. Calibration
    caches are keyed on it, so a redesigned family can never reuse optima
    computed for the old one (stale caches caused three bugs in a week)."""
    fam = FAMILIES[family]
    payload = {k: fam[k] for k in ("instr", "context", "free_parameters",
                                   "baseline", "fom", "constraints")}
    # optional hooks are recorded ONLY when a family uses them: including
    # them as None made every family's hash move whenever a new hook was
    # added, so a guide-only change discarded 150 certified SANS calibrations
    # (2026-09-16). SPEC_VERSION stays global and is for logic changes that
    # the dicts cannot express.
    payload.update({k: fam[k] for k in ("baseline_fn", "context_fn")
                    if fam.get(k)})
    payload.update(static_checks=fam.get("static_checks", []),
                   protocol=family_protocol(family), spec_version=SPEC_VERSION)
    return hashlib.sha1(json.dumps(payload, sort_keys=True).encode()).hexdigest()[:10]


def spec_lines(inst: dict) -> list:
    """The instance's specification with its numeric limits, for the prompt."""
    c, fam = inst["context"], inst["family"]
    if fam == "guide_divergence":
        return [f"  divergence limit div_max = {c['div_max']} deg: m_coat x 0.099 "
                f"deg/A x wl <= div_max, i.e. m_coat <= {guide_max_m(c):.3f} here; "
                f"only neutrons within +/-div_max are counted",
                f"  beam size: w_out <= det_wh = {c['det_wh']} m"]
    if fam == "sans_collimation":
        return [f"  beamstop: unscattered beam radius at the stop <= "
                f"stop_r = {sans_stop_radius(c):.4f} m",
                f"  sample: beam radius at the sample <= sample_wh / 2 = "
                f"{float(c['sample_wh']) / 2:.4f} m",
                f"  resolution: q_min <= 1/r_sphere, i.e. unscattered beam radius "
                f"at the detector <= {sans_resolution_limit(c):.4f} m here"]
    return []


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
    if fam.get("context_fn"):
        context = CONTEXT_FNS[fam["context_fn"]](context, rng)
    return {
        "id": f"{family}-{split}-{index:06d}",
        "family": family,
        "split": split,
        "index": index,
        "description": fam["description"],
        "context": context,
        "free_parameters": {k: list(v)
                            for k, v in fam["free_parameters"].items()},
        "baseline": (BASELINE_FNS[fam["baseline_fn"]](context)
                     if fam.get("baseline_fn") else dict(fam["baseline"])),
        "fom": dict(fam["fom"]),
        "constraints": [dict(c) for c in fam["constraints"]],
        "static_checks": list(fam.get("static_checks", [])),
        "family_signature": family_signature(family),
        "target_ratio": 1.0,  # L4 pass bar: fom >= target_ratio * baseline fom
        # rng is seeded from a string (deterministic across processes);
        # never use hash() here — string hashing is per-process randomized
        "protocol": {**family_protocol(family),
                     "seed": 1 + rng.randrange(2**31 - 1)},
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
            # the model must know the bar it is held to: the target is a
            # fraction of a constraint-filtered, fresh-seed-verified
            # classical optimum, not merely "beat the baseline"
            # (2026-09-13). The fraction is the difficulty knob, so quote
            # the one actually in force rather than a hardcoded 80%.
            pct = 100 * (inst.get("target_fraction") or 0.8)
            lines += [f"TARGET TO BEAT: {base_fom * tr:.6g} "
                      f"({tr:.2f}x the baseline). This target is {pct:.0f}% "
                      f"of the best design a classical optimizer found for "
                      f"this instance under the same simulation protocol, so "
                      f"beating the baseline alone is NOT sufficient."]
        else:
            lines += ["Target: beat the baseline configuration."]
    lines += ["", "Constraints (checked against the baseline's pattern — "
              "stay within band):"]
    lines += [f"  {c['monitor']}.{c['observable']} within "
              f"[{c['band'][0]}x, {c['band'][1]}x] of baseline"
              for c in inst["constraints"]]
    spec = spec_lines(inst) if "family" in inst else []
    if spec:
        lines += ["", "Specification for this instance (checked before any "
                  "simulation; a violation scores level 0):"] + spec
    lines += ["", "Evaluation runs at a fixed protocol (ncount "
              f"{inst['protocol']['ncount']:g}, env-controlled seed); "
              "iterate however you like, the graded run is the env's."]
    return "\n".join(lines)
