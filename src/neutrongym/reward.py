"""Reward ladder — level-resolved (L1 syntax → L4 scientific), env-controlled.

Every scored action yields a record with the deepest level PASSED as a
first-class field: the raw material for the failure taxonomy (M7) and the
"why does training improve" attribution (M8). Reward is computed at the
env's protocol (ncount + seed fixed per instance) — never at agent-chosen
statistics.

Ladder for parametric actions on a template family:
  L1 static      free (no simulation): the action names exactly the free
                 parameters, all values finite and inside bounds
  L2 runtime     cheap dynamic: truncated-ncount run succeeds
  L3 structural  terminal run at the protocol: FOM monitor present, its
                 statistics above floor, band constraints vs the baseline
                 pattern hold (bands, not max-only — the beamstop-leakage
                 red-team lesson)
  L4 scientific  FOM beats target_ratio x baseline FOM

Default shaping (a knob, not a law — documented so it can be changed in one
place): +0.25 per level L1-L3, plus at L4 0.25 * min(fom_ratio, 2), where
fom_ratio = fom / (baseline_fom * target_ratio) for maximize. Max 1.25.
"""

import math

from . import hacks

# canonical observable accessor (migrated from benchmark/harness/grader.py —
# the harness imports it back; dependency arrow points into the package)
_NESTED = {
    "beam_width_x": ("beam_width", "dX"),
    "beam_width_y": ("beam_width", "dY"),
    "beam_center_x": ("beam_center", "X0"),
    "beam_center_y": ("beam_center", "Y0"),
}


def get_observable(mon: dict, name: str):
    if name in _NESTED:
        outer, inner = _NESTED[name]
        return (mon.get(outer) or {}).get(inner)
    return mon.get(name)


def _monitor(summary: dict, component: str):
    for m in summary.get("monitors", []):
        if m.get("component") == component:
            return m
    return None


def baseline(inst: dict, fexec) -> dict:
    """Run the instance's baseline configuration at the terminal protocol.
    Cached by the env per instance — one run, reused by every score()."""
    proto = inst["protocol"]
    out = fexec.run({**inst["context"], **inst["baseline"]},
                    ncount=proto["ncount"], seed=proto["seed"])
    if not out["ok"]:
        return {"ok": False, "detail": out}
    summary = out["summary"]
    fom_mon = _monitor(summary, inst["fom"]["monitor"])
    fom = get_observable(fom_mon, inst["fom"]["metric"]) if fom_mon else None
    if not fom:
        # a zero-flux baseline cannot normalise anything: calibration would
        # bail and the instance would fall back to an UNCALIBRATED target
        # (2026-09-15). Fail loudly instead — env.reset raises.
        return {"ok": False, "detail": (
            f"baseline FOM is {fom!r} on {inst['id']}: the baseline collects "
            f"no signal, so no target can be defined for this instance")}
    floor = proto.get("statistics_floor", 0)
    events = (fom_mon.get("events") or 0) if fom_mon else 0
    if events < floor:
        # normalising every candidate by a noise estimate is worse than
        # failing: raise the family's ncount or tighten its generator ranges
        return {"ok": False, "detail": (
            f"baseline statistics below floor on {inst['id']}: {events:g} < "
            f"{floor:g} events")}
    cons = {}
    for c in inst["constraints"]:
        mon = _monitor(summary, c["monitor"])
        cons[f"{c['monitor']}.{c['observable']}"] = (
            get_observable(mon, c["observable"]) if mon else None)
    return {"ok": True, "fom": fom, "constraints": cons,
            "elapsed_s": out["elapsed_s"]}


def _check_l1(inst: dict, action: dict) -> dict:
    free = inst["free_parameters"]
    extra = sorted(set(action) - set(free))
    missing = sorted(set(free) - set(action))
    if extra or missing:
        return {"pass": False,
                "detail": f"action must set exactly the free parameters; "
                          f"missing={missing} extra={extra}"}
    for k, (lo, hi) in free.items():
        v = action[k]
        if not isinstance(v, (int, float)) or isinstance(v, bool) \
                or not math.isfinite(v):
            return {"pass": False, "detail": f"{k}={v!r} is not a finite number"}
        if not lo <= v <= hi:
            return {"pass": False,
                    "detail": f"{k}={v:g} outside bounds [{lo}, {hi}]"}
    if inst.get("static_checks"):
        from .generate import STATIC_CHECKS
        for name in inst["static_checks"]:
            res = STATIC_CHECKS[name](inst["context"], action)
            if not res["pass"]:
                return {"pass": False, "check": name,
                        "detail": res.get("detail", name)}
    return {"pass": True}


def _check_constraints(inst: dict, summary: dict, base: dict) -> list:
    out = []
    for c in inst["constraints"]:
        key = f"{c['monitor']}.{c['observable']}"
        base_v = base["constraints"].get(key)
        mon = _monitor(summary, c["monitor"])
        val = get_observable(mon, c["observable"]) if mon else None
        if base_v is None or base_v == 0:
            out.append({"constraint": key, "pass": True,
                        "note": "baseline value unavailable — band skipped"})
            continue
        lo, hi = c["band"][0] * base_v, c["band"][1] * base_v
        lo, hi = min(lo, hi), max(lo, hi)
        ok = val is not None and lo <= val <= hi
        out.append({"constraint": key, "pass": bool(ok), "value": val,
                    "band": [lo, hi]})
    return out


def score(inst: dict, action: dict, fexec, base: dict) -> dict:
    """Score one action. base = reward.baseline(inst, fexec) (env-cached).
    Returns the level-resolved record; never raises on agent-caused
    failure — failures ARE the signal."""
    proto = inst["protocol"]
    levels, elapsed = {}, 0.0
    record = {"instance": inst["id"], "action": dict(action), "levels": levels,
              "level": 0, "reward": 0.0, "fom": None,
              "baseline_fom": base.get("fom"), "elapsed_s": 0.0}

    levels["L1"] = _check_l1(inst, action)
    if not levels["L1"]["pass"]:
        return record
    record["level"], record["reward"] = 1, 0.25

    run_params = {**inst["context"], **action}
    cheap = fexec.run(run_params, ncount=proto["ncount_cheap"],
                      seed=proto["seed"])
    elapsed += cheap.get("elapsed_s", 0.0)
    levels["L2"] = {"pass": cheap["ok"]}
    if not cheap["ok"]:
        levels["L2"]["detail"] = (cheap.get("diagnostics") or [])[-5:]
        record["elapsed_s"] = round(elapsed, 4)
        return record
    record["level"], record["reward"] = 2, 0.5

    term = fexec.run(run_params, ncount=proto["ncount"], seed=proto["seed"])
    elapsed += term.get("elapsed_s", 0.0)
    record["elapsed_s"] = round(elapsed, 4)
    if not term["ok"]:
        levels["L3"] = {"pass": False,
                        "detail": (term.get("diagnostics") or [])[-5:]}
        return record
    summary = term["summary"]

    fom_mon = _monitor(summary, inst["fom"]["monitor"])
    floor = proto.get("statistics_floor", 0)
    constraints = _check_constraints(inst, summary, base)
    # Liouville utilization is only meaningful above the statistics floor —
    # a starved monitor's intensity estimate is noise (measured 2026-08-05:
    # 1.14 "utilization" at ~250 events collapsed to 0.99 at 10x rays)
    stats_ok = fom_mon is not None and (fom_mon.get("events") or 0) >= floor
    liouville = (hacks.liouville_check(inst, action, fom_mon["intensity"])
                 if stats_ok else
                 {"pass": True, "kind": "not_evaluated",
                  "note": "below statistics floor", "utilization": None,
                  "bound": None, "value": None})
    if fom_mon is None:
        detail = "FOM monitor missing from output"
    elif not stats_ok:
        detail = (f"FOM monitor statistics below floor "
                  f"({fom_mon.get('events'):g} < {floor:g} events)")
    elif not liouville["pass"]:
        detail = (f"unphysical_gain: FOM intensity {liouville['value']:g} "
                  f"exceeds the {liouville['kind']} bound "
                  f"{liouville['bound']:g} (Liouville — passive optics "
                  f"cannot beat source brightness)")
    elif not all(c["pass"] for c in constraints):
        detail = "constraint band violated"
    else:
        detail = None
    levels["L3"] = {"pass": detail is None, "constraints": constraints,
                    "liouville": liouville}
    if detail is not None:
        levels["L3"]["detail"] = detail
        return record
    record["level"], record["reward"] = 3, 0.75

    fom = get_observable(fom_mon, inst["fom"]["metric"]) if fom_mon else None
    record["fom"] = fom
    base_fom, tr = base.get("fom"), inst.get("target_ratio", 1.0)
    if not base_fom or fom is None:
        levels["L4"] = {"pass": False, "detail": "FOM unavailable"}
        return record
    ratio = (fom / (base_fom * tr)) if inst["fom"]["maximize"] \
        else (base_fom * tr) / fom if fom else 0.0
    # strict >: grading is deterministic at the fixed seed, so resubmitting
    # the baseline scores ratio exactly 1.0 — that is not an improvement
    levels["L4"] = {"pass": ratio > 1.0 + 1e-9, "fom_ratio": round(ratio, 6)}
    record["reward"] = round(0.75 + 0.25 * max(0.0, min(ratio, 2.0)), 6)
    if levels["L4"]["pass"]:
        record["level"] = 4
    return record
