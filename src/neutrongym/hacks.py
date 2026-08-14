"""Physics-native anti-hacking checks — Liouville/conservation bounds.

Passive neutron optics cannot increase phase-space density (Liouville), so
the intensity a monitor can collect is bounded by source brightness x the
monitor's phase-space acceptance: I <= flux * A[cm^2] * Omega[sr] * dl[AA].
A reward-hacked or simulation-artifact configuration that reports more is
unphysical by construction — no reference needed, which is what makes this
check trainable-tier-safe (it cannot be gamed by matching a reference).

Calibrated empirically 2026-08-05 (guide family, 1e6 rays): the best
physical configurations reach 93-99% of the analytic bound and never cross
it — i.e. the family's FOM plateau IS the Liouville limit, and
`utilization` (I/bound) is reported per episode as an analysis field.

Per-family bound sharpness (documented honestly):
- guide_divergence: étendue bound at the FOM monitor — SHARP (legit optimum
  ~0.99x bound; margin 1.10 covers 1e5-ray statistics).
- sans_collimation: conservation bound through the pinhole chain — LOOSE
  (scattering redistributes into 4pi; detected << incident). Catches gross
  weight-multiplication hacks; the band constraints do the fine work.
"""

import math

LIOUVILLE_MARGIN = 1.10  # legit max ~0.99x bound; 1e5-ray stats ~1-2%


def _guide_bound(context, action):
    # Divergence_monitor: det_wh x det_wh window, +/-0.5 deg both axes,
    # full source band 2*dwl; family template flux = 1 [n/s/cm^2/sr/AA]
    area = (context["det_wh"] * 100.0) ** 2
    omega = (2 * math.radians(0.5)) ** 2
    return 1.0 * area * omega * (2 * context["dwl"])


def _sans_bound(context, action):
    # conservation through the collimation: nothing downstream can exceed
    # what pinhole 2 admits. B * A_pin2 * Omega(pin1 seen from pin2) * dl,
    # flux = 1e8 in the family template. Action-dependent (pin radii are
    # the free parameters) — the allowance scales physically with them.
    r1 = float(action.get("r_pin1", context.get("r_pin1", 0.005)))
    r2 = float(action.get("r_pin2", context.get("r_pin2", 0.005)))
    area2 = math.pi * (r2 * 100.0) ** 2
    omega = math.pi * r1 ** 2 / context["L_coll"] ** 2
    dl = 2 * 0.05 * context["wl"]
    return 1e8 * area2 * omega * dl


BOUNDS = {
    "guide_divergence": {"fn": _guide_bound, "kind": "etendue", "sharp": True},
    "sans_collimation": {"fn": _sans_bound, "kind": "conservation",
                         "sharp": False},
}


def liouville_check(inst: dict, action: dict, fom_intensity) -> dict:
    """{"pass", "bound", "value", "utilization", "kind"} for the instance's
    FOM monitor. Unknown families pass with kind "none" (never block a
    family that has no derived bound — add one instead)."""
    spec = BOUNDS.get(inst["family"])
    if spec is None or fom_intensity is None:
        return {"pass": True, "kind": "none", "bound": None,
                "value": fom_intensity, "utilization": None}
    bound = spec["fn"](inst["context"], action)
    util = fom_intensity / bound if bound else float("inf")
    return {"pass": util <= LIOUVILLE_MARGIN, "kind": spec["kind"],
            "bound": round(bound, 9), "value": fom_intensity,
            "utilization": round(util, 4)}
