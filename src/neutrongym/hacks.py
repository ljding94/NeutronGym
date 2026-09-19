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
    # Divergence_monitor: det_wh x det_wh window, +/-div_max deg both axes
    # (the instance's divergence specification since 2026-09-15; 0.5 deg for
    # older contexts), full source band 2*dwl; template flux = 1
    # [n/s/cm^2/sr/AA]
    area = (context["det_wh"] * 100.0) ** 2
    omega = (2 * math.radians(context.get("div_max", 0.5))) ** 2
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
# The probe must run at the bar the family is actually graded on. Since
# calibration v2 a 1.0x bar means "strictly beat a strong same-seed optimum",
# which nothing fixed achieves, so probing there would clear every family
# vacuously (2026-09-15).
CONSTANT_GATE_MAX_FRACTION = 0.99


def check_gate_fraction(fraction: float) -> float:
    if not 0 < fraction <= CONSTANT_GATE_MAX_FRACTION:
        raise ValueError(
            f"gate fraction {fraction} is vacuous: at or above 1.0x no fixed "
            f"configuration can beat each instance's own optimum, so the gate "
            f"would pass any family. Probe at the bar the family is graded on "
            f"(<= {CONSTANT_GATE_MAX_FRACTION}).")
    return fraction


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


CONSTANT_REFINE_ROUNDS = 4      # local search around the best candidate


def _key(action: dict) -> tuple:
    return tuple(sorted(action.items()))


def _neighbourhood(action: dict, free: dict, step_frac: float) -> list:
    """Every combination of -step / 0 / +step per free parameter (diagonals
    included: a sweet spot can need both parameters to move together),
    clipped to bounds."""
    axes = []
    for k, (lo, hi) in free.items():
        step = (hi - lo) * step_frac
        axes.append(sorted({round(min(hi, max(lo, action[k] + d)), 6)
                            for d in (-step, 0.0, step)}))
    names = list(free)
    out = []
    for combo in itertools.product(*axes):
        cand = dict(zip(names, combo))
        if _key(cand) != _key(action):
            out.append(cand)
    return out


def constant_policy_probe(env, indices, candidates: list, classical: bool = True,
                          refine_rounds: int = CONSTANT_REFINE_ROUNDS) -> dict:
    """Score fixed actions on every instance, no model.

    Candidates come from three sources, because a grid alone missed a sharp
    sweet spot (SANS (0.008, 0.008) passed 51% of held-out instances while
    the 5-level grid reported 12%, 2026-09-15):
      grid       the caller's list (constant_candidates: baseline + grid)
      classical  each probed instance's own calibrated classical optimum,
                 tried as a constant on EVERY instance
      refined    a local neighbourhood search around the best candidate

    Instances flagged `no_headroom` are skipped — they are not improvement
    tasks. `env` needs reset(index=) returning (obs, info) and step(action)
    returning the gym 5-tuple with a level-resolved record last; obs
    ["instance"] supplies bounds, baseline and classical optimum when present.
    """
    indices = list(indices)
    valid, free, base_key, classical_actions = [], None, None, []
    for idx in indices:
        obs, _ = env.reset(index=idx)
        inst = (obs or {}).get("instance") or {}
        if inst.get("no_headroom"):
            continue
        valid.append(idx)
        free = free or inst.get("free_parameters")
        if base_key is None and inst.get("baseline"):
            base_key = _key(inst["baseline"])
        if classical and inst.get("classical_action"):
            classical_actions.append(dict(inst["classical_action"]))

    pool = {}

    def add(action, source):
        k = _key(action)
        if k not in pool:
            pool[k] = {"action": dict(action), "passes": None,
                       "source": "baseline" if k == base_key else source}
        return k

    def score(keys):
        keys = [k for k in keys if pool[k]["passes"] is None]
        if not keys:
            return
        for k in keys:
            pool[k]["passes"] = 0
        for idx in valid:
            env.reset(index=idx)
            for k in keys:
                rec = env.step(dict(pool[k]["action"]))[4]
                pool[k]["passes"] += rec.get("level") == 4

    score([add(a, "grid") for a in candidates]
          + [add(a, "classical") for a in classical_actions])

    if free and valid and pool:
        best = max(pool, key=lambda k: pool[k]["passes"])
        step_frac = 1.0 / 8
        for _ in range(refine_rounds):
            neigh = [add(a, "refined")
                     for a in _neighbourhood(pool[best]["action"], free, step_frac)]
            score(neigh)
            challenger = max(pool, key=lambda k: pool[k]["passes"])
            if pool[challenger]["passes"] > pool[best]["passes"]:
                best = challenger
            else:
                step_frac /= 2

    n = len(valid)
    return {"n_instances": n, "skipped_no_headroom": len(indices) - n,
            "results": [{"action": v["action"], "passes": v["passes"],
                         "pass_rate": round(v["passes"] / n, 4) if n else None,
                         "source": v["source"]}
                        for v in pool.values()]}


def binomial_upper_bound(k: int, n: int, alpha: float = 0.05) -> float:
    """One-sided upper (1-alpha) Clopper-Pearson limit on a binomial rate.

    Exact, stdlib only. Used so the gate certifies on what the evidence can
    support rather than on a point estimate.
    """
    if n <= 0 or k >= n:
        return 1.0
    lo, hi = k / n, 1.0
    for _ in range(100):
        mid = (lo + hi) / 2
        plausible = sum(math.comb(n, i) * mid ** i * (1 - mid) ** (n - i)
                        for i in range(k + 1)) > alpha
        lo, hi = (mid, hi) if plausible else (lo, mid)
    return (lo + hi) / 2


READOUT_LEVELS = 17   # levels for the parameter a readout rule leaves free


def _levels(lo, hi, n=READOUT_LEVELS):
    return [round(lo + (hi - lo) * k / (n - 1), 6) for k in range(n)]


def readout_rules(family: str, free: dict) -> list:
    """No-model policies that READ the instance's stated limits (2026-09-17).

    The constant-policy gate probes fixed actions only. On the guide family a
    two-line rule -- w_out and m_coat at the limits the prompt prints, fixed
    w_in -- passed 96.3% of held-out instances, and GRPO learned exactly that
    (18.0% -> 98.7%). A family must be checked against rules like these too.
    -> [(label, fn(context) -> action | None)]; None = rule not applicable."""
    from . import generate
    rules = []
    if family == "guide_divergence":
        lo, hi = free["m_coat"]
        wlo, whi = free["w_out"]

        def m_lim(c):
            return max(lo, min(hi, math.floor(generate.guide_max_m(c) * 1000) / 1000))

        def w_lim(c):
            return max(wlo, min(whi, float(c["det_wh"])))

        for w_in in _levels(*free["w_in"]):
            rules.append((f"w_out=limit m_coat=limit w_in={w_in}",
                          lambda c, inst=None, w=w_in: {"w_in": w, "w_out": w_lim(c), "m_coat": m_lim(c)}))
    elif family == "sans_collimation":
        rlo, rhi = free["r_pin1"]
        for r1 in _levels(rlo, rhi):
            def f(c, inst=None, r1=r1):
                r2 = math.floor(generate.sans_max_r2(c, r1) * 1e5) / 1e5
                return None if r2 < rlo else {"r_pin1": r1, "r_pin2": min(rhi, r2)}
            rules.append((f"r_pin1={r1} r_pin2=limit", f))
        for r2 in _levels(rlo, rhi):
            def g(c, inst=None, r2=r2):
                r1 = math.floor(generate.sans_max_r1(c, r2) * 1e5) / 1e5
                return None if r1 < rlo else {"r_pin1": min(rhi, r1), "r_pin2": r2}
            rules.append((f"r_pin2={r2} r_pin1=limit", g))
    elif family == "sans_match":
        # two geometric widths, both linear in (r1, r2):
        #   w = k * (r2 + (r1 + r2) * a),  a = distance / L_coll
        # so the pair is the exact solution of a 2x2 system, up to the profile
        # constant k relating a monitor's std to the beam half-width.
        rlo, rhi = free["r_pin1"]

        def solve(c, t, k):
            L = float(c["L_coll"])
            a_s = generate.SANS_COLL2_TO_SAMPLE / L
            a_t = (generate.SANS_COLL2_TO_SAMPLE + float(c["det_dist"])
                   - generate.SANS_STOP_BEFORE_DETECTOR) / L
            ws, wt = t[0] / 100.0 / k, t[1] / 100.0 / k      # cm -> m, de-scale
            det = (1 + a_s) * a_t - (1 + a_t) * a_s
            if abs(det) < 1e-12:
                return None
            r2 = (ws * a_t - wt * a_s) / det
            r1 = (wt * (1 + a_s) - ws * (1 + a_t)) / det
            if not (rlo <= r1 <= rhi and rlo <= r2 <= rhi):
                return None
            return {"r_pin1": round(r1, 6), "r_pin2": round(r2, 6)}

        for k in (0.5, 0.577, 0.45, 0.6):
            def h(c, inst=None, k=k):
                return solve(c, inst["targets"], k) if inst and inst.get("targets") else None
            rules.append((f"physics: width inversion k={k}", h))
    elif family == "guide_match":
        # geometric-optics inversions of the stated targets (prototype best:
        # 6/16 at +/-5%): divergence std ~ kdiv * 0.099 * m * wl  ->  m_coat;
        # spot std^2 ~ (w_out / sqrt(12))^2 + (d_sample * divergence)^2  ->  w_out
        lo, hi = free["m_coat"]
        wlo, whi = free["w_out"]
        for kdiv in (1 / math.sqrt(3), 0.5, 0.45, 0.4):
            for w_in in _levels(*free["w_in"], n=9):
                def h(c, inst=None, kdiv=kdiv, w_in=w_in):
                    if not inst or not inst.get("targets"):
                        return None
                    spot_m, div_deg = inst["targets"][0] / 100.0, inst["targets"][1]
                    m = max(lo, min(hi, div_deg / (kdiv * 0.099 * float(c["wl"]))))
                    spread = float(c["d_sample"]) * math.tan(math.radians(div_deg))
                    core = spot_m ** 2 - spread ** 2
                    w_out = max(wlo, min(whi, math.sqrt(core) * math.sqrt(12) if core > 0 else wlo))
                    return {"w_in": w_in, "w_out": round(w_out, 5), "m_coat": round(m, 4)}
                rules.append((f"physics: optics inversion kdiv={kdiv:.3f} w_in={w_in}", h))
    return rules


PHYSICS_RULE_PREFIX = "physics:"


def split_rule_results(probe: dict) -> tuple[dict, dict]:
    """(copy-type rules, physics-model rules). The ceiling is enforced on the
    first kind only (2026-09-17): copying printed limits, fixed answers and
    lookups need no physics, while a one-shot physics-model estimate IS design
    knowledge -- it is reported as a reference arm, not a disqualifier. Every
    physically sensible few-parameter task has such an estimate."""
    def sub(keep):
        return dict(probe, results=[r for r in probe["results"] if keep(r)])
    phys = lambda r: str(r["action"]).startswith(PHYSICS_RULE_PREFIX)
    return sub(lambda r: not phys(r)), sub(phys)


def readout_policy_probe(env, indices, rules: list) -> dict:
    """Score every readout rule on every instance (no model). Same result
    shape as constant_policy_probe, so summarize_constant_probe applies."""
    valid = []
    for idx in indices:
        obs, _ = env.reset(index=idx)
        if not obs["instance"].get("no_headroom"):
            valid.append(idx)
    passes = {label: 0 for label, _ in rules}
    for idx in valid:
        obs, _ = env.reset(index=idx)
        ctx = obs["instance"]["context"]
        for label, fn in rules:
            a = fn(ctx, obs["instance"])
            if a is not None:
                passes[label] += env.step(a)[4].get("level") == 4
    n = len(valid)
    return {"n_instances": n, "skipped_no_headroom": len(list(indices)) - n,
            "results": [{"action": label, "passes": v,
                         "pass_rate": round(v / n, 4) if n else None,
                         "source": "readout"} for label, v in passes.items()]}


def summarize_constant_probe(probe: dict,
                             max_rate: float = CONSTANT_MAX_PASS_RATE) -> dict:
    """Gate verdict. A family fails if any single constant passes more than
    `max_rate` of instances, or if the baseline itself ever passes (L4 must
    mean improvement).

    The verdict is made on the UPPER confidence limit, not the observed rate
    (2026-09-15). At n=25 the guide's best constant passed 5/25 = 20.0%,
    exactly the ceiling, and was reported as passing -- but the 95% interval
    was [7%, 41%], so that run could not distinguish a clean family from one
    twice over the limit. Certifying "no constant solves this family" on 25
    instances is not something the data supports; ~150 are needed. A run that
    is merely too small now reports ok=False with underpowered=True, which is
    a different statement from the family failing.
    """
    ranked = sorted(probe["results"], key=lambda r: -(r["pass_rate"] or 0))
    best = ranked[0] if ranked else {"action": None, "pass_rate": None,
                                     "passes": 0}
    rate = best["pass_rate"] or 0
    n = probe["n_instances"]
    upper = binomial_upper_bound(best.get("passes") or 0, n)
    baseline_passes = sum(r["passes"] or 0 for r in probe["results"]
                          if r.get("source") == "baseline")
    return {"ok": upper <= max_rate and baseline_passes == 0,
            "max_rate": max_rate, "best_action": best["action"],
            "best_pass_rate": best["pass_rate"],
            "best_pass_rate_upper": round(upper, 4),
            # observed rate is fine but the sample cannot certify the ceiling
            "underpowered": rate <= max_rate < upper,
            "best_source": best.get("source"),
            "baseline_passes": baseline_passes,
            "n_instances": n,
            "skipped_no_headroom": probe.get("skipped_no_headroom", 0),
            "n_candidates": len(probe["results"]),
            "top5": ranked[:5]}
