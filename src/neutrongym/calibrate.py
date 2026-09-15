"""Per-instance target calibration — gives procedural tasks a real bar.

Found by M8 phase 0 (2026-09-13): procedural instances shipped with
`target_ratio = 1.0`, i.e. L4 meant merely beating a deliberately
UNDERSIZED baseline (the T2 calibration chose w_in=0.012/m_coat=1.5 to sit
3-4x below the FOM plateau on purpose). Result: the untrained Qwen3-8B
already passed ~83% of held-out instances and the 32B 100% — a ceiling,
with no headroom for any trainability claim to live in.

The benchmark's T2 tasks do not have this problem because their targets are
calibrated: **0.8 x the constraint-filtered, fresh-seed-re-verified
classical best** (2.55x baseline for the guide task, unbeaten by any agent
in M6). This module applies the SAME discipline per procedural instance.

Deliberately harder, not easier: this removes a ceiling that would have
made a trained-model result meaningless, and it reuses machinery that has
already been red-teamed (bounds-filtered ensemble; winner's-curse guard).
"""

import json
import os
import random

from . import hacks, reward

TARGET_FRACTION = 0.8   # same as the T2 benchmark discipline
N_SAMPLES = 30          # matched-compute budget, as recorded for T2
FRESH_SEED_OFFSET = 7919


def _constraints_ok(inst, summary, base):
    """Reuse the ladder's own band checks — a classical 'best' that
    violates the bands is not a legitimate target (the beamstop-leakage
    lesson: unconstrained maximisation finds exploits)."""
    return all(c["pass"] for c in reward._check_constraints(inst, summary,
                                                            base))


def calibrate_instance(inst: dict, fexec, base: dict,
                       n_samples: int = N_SAMPLES) -> dict:
    """Constraint-filtered random search over the instance's free
    parameters; returns the fresh-seed-re-verified best and the resulting
    target_ratio. Deterministic in the instance id."""
    rng = random.Random(f"calib/{inst['id']}")
    proto = inst["protocol"]
    best = None
    for _ in range(n_samples):
        action = {k: round(rng.uniform(lo, hi), 6)
                  for k, (lo, hi) in inst["free_parameters"].items()}
        # a classical "best" the agent's own L1 would reject is not a
        # legitimate target: 22 of 35 cached SANS optima were direct-beam
        # leaks before this check existed (2026-09-13)
        if not reward._check_l1(inst, action)["pass"]:
            continue
        out = fexec.run({**inst["context"], **action},
                        ncount=proto["ncount"], seed=proto["seed"])
        if not out["ok"]:
            continue
        summary = out["summary"]
        mon = reward._monitor(summary, inst["fom"]["monitor"])
        if not mon or (mon.get("events") or 0) < proto.get(
                "statistics_floor", 0):
            continue
        fom = reward.get_observable(mon, inst["fom"]["metric"])
        if fom is None or not _constraints_ok(inst, summary, base):
            continue
        # physics sanity: a "best" above the Liouville bound is an artifact
        lio = hacks.liouville_check(inst, action, fom)
        if not lio["pass"]:
            continue
        if best is None or fom > best["fom"]:
            best = {"fom": fom, "action": action}
    if best is None:
        return {"ok": False, "reason": "no constraint-valid sample found"}

    # winner's-curse guard: the max over noisy evaluations is biased up, so
    # re-verify the SELECTED point at a fresh seed and use that value
    fresh = fexec.run({**inst["context"], **best["action"]},
                      ncount=proto["ncount"],
                      seed=proto["seed"] + FRESH_SEED_OFFSET)
    if not fresh["ok"]:
        return {"ok": False, "reason": "fresh-seed re-verification failed"}
    mon = reward._monitor(fresh["summary"], inst["fom"]["monitor"])
    verified = reward.get_observable(mon, inst["fom"]["metric"]) if mon else None
    if verified is None or not base.get("fom"):
        return {"ok": False, "reason": "no verified FOM"}

    target_fom = TARGET_FRACTION * verified
    return {"ok": True, "classical_fom_selected": best["fom"],
            "classical_fom_verified": verified,
            "classical_action": best["action"],
            "baseline_fom": base["fom"],
            "classical_over_baseline": round(verified / base["fom"], 4),
            "target_fom": target_fom,
            "target_ratio": round(target_fom / base["fom"], 6),
            "n_samples": n_samples, "fraction": TARGET_FRACTION}


def cache_path(workdir: str, inst: dict) -> str:
    return os.path.join(workdir, "calibration", f"{inst['id']}.json")


def calibration_for(inst: dict, fexec, base: dict, workdir: str,
                    fraction: float | None = None) -> dict | None:
    """Cached calibration for one instance at one bar, or None if the
    classical search failed.

    `target_ratio` is never below 1.0: L4 means beating the baseline, and a
    target below it let resubmitting the baseline pass (2026-09-15: 74/600
    SANS train instances, 55% of that family's RAFT data). Such instances are
    flagged `no_headroom` — the classical search found no improvement at this
    bar — and rollouts skip them rather than train or score on them.

    `fraction` overrides TARGET_FRACTION. The expensive part, the
    constraint-filtered classical search, depends only on the instance, so a
    different fraction is a pure rescale of the cached optimum.
    """
    p = cache_path(workdir, inst)
    if os.path.isfile(p):
        with open(p) as f:
            rec = json.load(f)
    else:
        rec = calibrate_instance(inst, fexec, base)
        os.makedirs(os.path.dirname(p), exist_ok=True)
        with open(p, "w") as f:
            json.dump(rec, f, indent=1)
    if not rec.get("ok"):
        return None
    over = rec.get("classical_over_baseline")
    if over is None:  # pre-2026-09-13 cache entry
        over = rec["target_ratio"] / rec.get("fraction", TARGET_FRACTION)
    if fraction is None or fraction == rec.get("fraction", TARGET_FRACTION):
        raw = rec["target_ratio"]
    else:
        raw = round(fraction * over, 6)
    return {"target_ratio": max(raw, 1.0), "raw_target_ratio": raw,
            "no_headroom": raw <= 1.0, "classical_over_baseline": over,
            "classical_action": rec.get("classical_action")}


def calibrated_target_ratio(inst: dict, fexec, base: dict, workdir: str,
                            fraction: float | None = None) -> float | None:
    """Target ratio only (floored at 1.0); None if calibration failed."""
    cal = calibration_for(inst, fexec, base, workdir, fraction)
    return None if cal is None else cal["target_ratio"]
