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

Constant-policy degeneracy gate (added 2026-09-15). A family whose held-out
instances are passed by one fixed configuration, with no model, cannot tell
design skill from a lookup — however well its targets are calibrated. The
M8 record found both failure modes with this one probe: the SANS
direct-beam hole (red-team finding 6) and the guide family at the 1.0x bar,
where (w_in 0.05, w_out 0.03, m_coat 2.5) passes ~50% of held-out instances
(finding 7). Every family must pass it before its pass rates are read as
capability.
"""

import itertools
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


# --------------------------------------------------------------------------
# constant-policy degeneracy gate
# --------------------------------------------------------------------------

CONSTANT_GRID_LEVELS = 5       # 3 levels misses the guide degeneracy at
                               # (0.05, 0.03, 2.5); 5 levels contains it
CONSTANT_MAX_PASS_RATE = 0.20  # above this, one fixed configuration solves
                               # too much of the family for pass rates to
                               # mean design skill


def constant_candidates(inst: dict, levels: int = CONSTANT_GRID_LEVELS) -> list:
    """Fixed actions to probe: an evenly spaced grid over every free
    parameter's bounds (corners and midpoint included) plus the baseline.
    Deduplicated, order-stable, always within bounds."""
    axes = []
    for k, (lo, hi) in inst["free_parameters"].items():
        step = (hi - lo) / (levels - 1)
        axes.append([(k, round(lo + i * step, 6)) for i in range(levels)])
    out, seen = [], set()
    for combo in itertools.chain([tuple(inst["baseline"].items())],
                                 itertools.product(*axes)):
        key = tuple(sorted(combo))
        if key not in seen:
            seen.add(key)
            out.append(dict(combo))
    return out


def constant_policy_probe(env, indices, candidates: list) -> dict:
    """Score every candidate action once on every instance, no model.

    `env` is a NeutronGym (or anything with reset(index=) and step(action)
    returning the gym 5-tuple with a level-resolved record last). Resetting
    once per instance keeps the baseline and calibration cached; each step
    is scored independently, so the order of candidates cannot matter.
    """
    passes = [0] * len(candidates)
    indices = list(indices)
    for idx in indices:
        env.reset(index=idx)
        for j, action in enumerate(candidates):
            rec = env.step(dict(action))[4]
            passes[j] += rec.get("level") == 4
    n = len(indices)
    return {"n_instances": n,
            "results": [{"action": dict(a), "passes": p,
                         "pass_rate": round(p / n, 4) if n else None}
                        for a, p in zip(candidates, passes)]}


def summarize_constant_probe(probe: dict,
                             max_rate: float = CONSTANT_MAX_PASS_RATE) -> dict:
    """Gate verdict: the family passes only if no single constant clears
    `max_rate`. Reports the best constant and the top five."""
    ranked = sorted(probe["results"], key=lambda r: -(r["pass_rate"] or 0))
    best = ranked[0] if ranked else {"action": None, "pass_rate": None}
    rate = best["pass_rate"] or 0
    return {"ok": rate <= max_rate, "max_rate": max_rate,
            "best_action": best["action"], "best_pass_rate": best["pass_rate"],
            "n_instances": probe["n_instances"],
            "n_candidates": len(probe["results"]),
            "top5": ranked[:5]}
