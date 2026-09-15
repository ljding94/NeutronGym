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
N_RANDOM = 30           # random valid samples before local refinement
PATTERN_ROUNDS = 6      # full-neighbourhood pattern search around the best
# v1 ("calibration/") verified the selected optimum at a FRESH seed while
# agents were scored at the protocol seed. At 1e5 rays that mismatch is
# +/-3-10%, so an instance's own optimum passed its own 1.0x target only
# ~half the time and fixed answers near the optimum passed ~50-60% of
# instances (2026-09-15). v2 scores every candidate exactly as an agent is
# scored (reward.score, protocol seed: common random numbers), so target and
# agent are compared noise-free, and caches from v1 are never read.
CAL_DIR = "calibration_v2"


def _valid_fom(inst: dict, action: dict, fexec, base: dict):
    """FOM of a candidate through the agent's own ladder (valid = L3: static
    geometry, runs, statistics floor, Liouville, bands), else None."""
    rec = reward.score(inst, action, fexec, base)
    return rec["fom"] if rec.get("level", 0) >= 3 and rec.get("fom") else None


def calibrate_instance(inst: dict, fexec, base: dict,
                       n_random: int = N_RANDOM,
                       rounds: int = PATTERN_ROUNDS) -> dict:
    """Strong classical optimum for one instance, scored at the protocol
    seed. The baseline is always a candidate, so the optimum is never below
    it. Deterministic in the instance id."""
    if not base.get("fom"):
        return {"ok": False, "version": 2, "reason": "baseline FOM unavailable"}
    rng = random.Random(f"calib2/{inst['id']}")
    free = inst["free_parameters"]
    cands = [dict(inst["baseline"])] + [
        {k: round(rng.uniform(lo, hi), 6) for k, (lo, hi) in free.items()}
        for _ in range(n_random)]
    best_a, best_f, evals = None, None, 0
    for a in cands:
        f = _valid_fom(inst, a, fexec, base)
        evals += 1
        if f is not None and (best_f is None or f > best_f):
            best_a, best_f = a, f
    if best_a is None:
        return {"ok": False, "version": 2,
                "reason": "no valid candidate (baseline included)"}
    frac = 1.0 / 8
    for _ in range(rounds):
        improved = False
        for a in hacks._neighbourhood(best_a, free, frac):
            f = _valid_fom(inst, a, fexec, base)
            evals += 1
            if f is not None and f > best_f:
                best_a, best_f, improved = a, f, True
        if not improved:
            frac /= 2
    over = best_f / base["fom"]
    return {"ok": True, "version": 2,
            "method": ("baseline + random + pattern search, every candidate "
                       "scored by reward.score at the protocol seed"),
            "classical_action": best_a, "classical_fom": best_f,
            "classical_fom_verified": best_f,
            "baseline_fom": base["fom"], "classical_over_baseline": over,
            "target_fom": TARGET_FRACTION * best_f,
            # full precision on purpose: an instance's own optimum must score
            # a ratio of exactly 1.0 against a 1.0x target, never 1+rounding
            "target_ratio": TARGET_FRACTION * over,
            "fraction": TARGET_FRACTION, "evals": evals,
            "n_random": n_random, "rounds": rounds}


def cache_path(workdir: str, inst: dict) -> str:
    return os.path.join(workdir, CAL_DIR, f"{inst['id']}.json")


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
        # rounded far below the ladder's 1e-9 L4 tolerance: tidy values for
        # rescaled bars without ever flipping an exact-optimum comparison
        raw = round(fraction * over, 12)
    return {"target_ratio": max(raw, 1.0), "raw_target_ratio": raw,
            "no_headroom": raw <= 1.0, "classical_over_baseline": over,
            "classical_action": rec.get("classical_action")}


def calibrated_target_ratio(inst: dict, fexec, base: dict, workdir: str,
                            fraction: float | None = None) -> float | None:
    """Target ratio only (floored at 1.0); None if calibration failed."""
    cal = calibration_for(inst, fexec, base, workdir, fraction)
    return None if cal is None else cal["target_ratio"]
