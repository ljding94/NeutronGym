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

import itertools
import json
import os
import random

from . import hacks, reward

# 0.85 since 2026-09-16 (was 0.8, the T2 benchmark discipline). At 0.8 the
# guide family could not be certified free of constant-policy degeneracy: its
# best fixed answer passed 23/150 = 15.3% of held-out instances, upper95
# 21.0%, over the 20% ceiling. At 0.85 it passes 20/150 = 13.3%, upper95
# 18.8% -- certified. Raising the bar is the cheap half of the fix; the other
# half (the guide's inert divergence spec) is tracked separately.
TARGET_FRACTION = 0.85
# Cache entries predating the `fraction` key were all written at 0.8. This
# must NOT track TARGET_FRACTION: defaulting an old entry to whatever the
# current bar happens to be silently mis-scales it.
LEGACY_FRACTION = 0.8
N_RANDOM = 30           # random valid samples before local refinement
PATTERN_ROUNDS = 6      # v2 only (kept for the signature of old call sites)
# v3 search (2026-09-16). v2 ran ONE pattern search from the best of 31
# samples for a fixed 6 rounds, halving the step every non-improving round.
# Optima sit on specification boundaries, where most +/-step moves are
# invalid, so the step collapsed and the search stopped early (SANS: 79
# evals). A fixed candidate from the probe pool then beat the "optimum" on
# 60-70% of instances in every family, by more than 1/0.85 on 20-25%.
N_STARTS = 3                 # pattern searches from the best distinct seeds
START_STEP = 1 / 8
MIN_STEP = 1 / 512           # search runs until the step is this small
MAX_STEP = 1 / 4             # steps grow back after an improvement
MAX_EVALS = 2000             # per instance, hard cap
# Low-dimensional families also get half-step moves. SANS's two pinholes trade
# off along a ridge that is neither axis-aligned nor 45 deg, so +/-step moves
# stalled on it: one of 10 previously-beaten instances still ended at 0.78 of
# the best-known answer. Adding +/-step/2 (1:2 and 2:1 directions) fixed 10/10
# at fewer evals than 8 starts (median 577 vs 640). Not used above 2
# parameters: 5**k moves (124 for the guide) where 3**k already reached 10/10.
HALF_STEP_MAX_DIMS = 2


def _search_neighbourhood(action: dict, free: dict, step: float) -> list:
    if len(free) > HALF_STEP_MAX_DIMS:
        return hacks._neighbourhood(action, free, step)
    axes = []
    for k, (lo, hi) in free.items():
        d = (hi - lo) * step
        axes.append(sorted({round(min(hi, max(lo, action[k] + m * d)), 6)
                            for m in (-1, -0.5, 0.0, 0.5, 1)}))
    me = hacks._key(action)
    return [c for c in (dict(zip(free, combo))
                        for combo in itertools.product(*axes))
            if hacks._key(c) != me]
# v1 ("calibration/") verified the selected optimum at a FRESH seed while
# agents were scored at the protocol seed. At 1e5 rays that mismatch is
# +/-3-10%, so an instance's own optimum passed its own 1.0x target only
# ~half the time and fixed answers near the optimum passed ~50-60% of
# instances (2026-09-15). v2 scores every candidate exactly as an agent is
# scored (reward.score, protocol seed: common random numbers), so target and
# agent are compared noise-free, and caches from v1 are never read.
CAL_DIR = "calibration_v3"


def _valid_fom(inst: dict, action: dict, fexec, base: dict):
    """FOM of a candidate through the agent's own ladder (valid = L3: static
    geometry, runs, statistics floor, Liouville, bands), else None."""
    rec = reward.score(inst, action, fexec, base)
    return rec["fom"] if rec.get("level", 0) >= 3 and rec.get("fom") else None


def calibrate_instance(inst: dict, fexec, base: dict,
                       n_random: int = N_RANDOM, n_starts: int = N_STARTS,
                       max_evals: int = MAX_EVALS, rounds=None) -> dict:
    """Strong classical optimum for one instance, scored at the protocol
    seed (common random numbers with the agent). Deterministic in the id.

    Seeds: baseline + `n_random` uniform samples + the constant-probe grid
    (the gate's own candidates, so no fixed answer the gate tries can beat
    the optimum for want of having been looked at). Then an adaptive pattern
    search from each of the `n_starts` best distinct seeds: the step doubles
    (up to MAX_STEP) after an improving move and halves otherwise, stopping
    at MIN_STEP. Every action is scored at most once. `rounds` is accepted
    and ignored for v2 call sites.
    """
    if not base.get("fom"):
        return {"ok": False, "version": 3, "reason": "baseline FOM unavailable"}
    rng = random.Random(f"calib2/{inst['id']}")
    free = inst["free_parameters"]
    seen: dict = {}

    def fom(a):
        k = hacks._key(a)
        if k not in seen:
            if len(seen) >= max_evals:
                return None          # over budget: unscored, and NOT counted
            seen[k] = _valid_fom(inst, a, fexec, base)
        return seen[k]

    seeds = [dict(inst["baseline"])] + [
        {k: round(rng.uniform(lo, hi), 6) for k, (lo, hi) in free.items()}
        for _ in range(n_random)] + hacks.constant_candidates(inst)
    scored = [(fom(a), a) for a in seeds]
    valid = sorted([(f, a) for f, a in scored if f is not None],
                   key=lambda t: -t[0])
    if not valid:
        return {"ok": False, "version": 3, "evals": len(seen),
                "reason": "no valid candidate (baseline and grid included)"}
    starts, used = [], set()
    for f, a in valid:
        if hacks._key(a) not in used:
            starts.append((f, a)); used.add(hacks._key(a))
        if len(starts) == n_starts:
            break
    best_f, best_a = valid[0]
    for f, a in starts:
        step = START_STEP
        while step >= MIN_STEP and len(seen) < max_evals:
            moves = [(fom(c), c) for c in _search_neighbourhood(a, free, step)]
            moves = [(mf, c) for mf, c in moves if mf is not None]
            top = max(moves, key=lambda t: t[0], default=None)
            if top and top[0] > f:
                f, a = top
                step = min(step * 2, MAX_STEP)
            else:
                step /= 2
        if f > best_f:
            best_f, best_a = f, a
    over = best_f / base["fom"]
    return {"ok": True, "version": 3,
            "method": ("baseline + random + probe grid seeds, adaptive "
                       "multi-start pattern search, every candidate scored "
                       "by reward.score at the protocol seed"),
            "classical_action": best_a, "classical_fom": best_f,
            "classical_fom_verified": best_f,
            "baseline_fom": base["fom"], "classical_over_baseline": over,
            "target_fom": TARGET_FRACTION * best_f,
            # full precision on purpose: an instance's own optimum must score
            # a ratio of exactly 1.0 against a 1.0x target, never 1+rounding
            "target_ratio": TARGET_FRACTION * over,
            "fraction": TARGET_FRACTION, "evals": len(seen),
            "n_random": n_random, "n_starts": n_starts,
            "hit_eval_cap": len(seen) >= max_evals}


def cache_path(workdir: str, inst: dict) -> str:
    """Keyed on the family signature when the instance carries one, so a
    changed family definition never reads optima computed for the old one."""
    sig = inst.get("family_signature")
    parts = [workdir, CAL_DIR] + ([sig] if sig else []) + [f"{inst['id']}.json"]
    return os.path.join(*parts)


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
    cached_fraction = rec.get("fraction", LEGACY_FRACTION)
    over = rec.get("classical_over_baseline")
    if over is None:  # pre-2026-09-13 cache entry
        over = rec["target_ratio"] / cached_fraction
    # resolve the default BEFORE comparing: `fraction is None` used to return
    # the cached target_ratio verbatim, so after TARGET_FRACTION moved from
    # 0.8 to 0.85 every cached instance would have kept grading at 0.8 while
    # the module claimed 0.85 (2026-09-16)
    want = TARGET_FRACTION if fraction is None else fraction
    if want == cached_fraction:
        raw = rec["target_ratio"]
    else:
        # rounded far below the ladder's 1e-9 L4 tolerance: tidy values for
        # rescaled bars without ever flipping an exact-optimum comparison
        raw = round(want * over, 12)
    return {"target_ratio": max(raw, 1.0), "raw_target_ratio": raw,
            "no_headroom": raw <= 1.0, "classical_over_baseline": over,
            "classical_action": rec.get("classical_action")}


def calibrated_target_ratio(inst: dict, fexec, base: dict, workdir: str,
                            fraction: float | None = None) -> float | None:
    """Target ratio only (floored at 1.0); None if calibration failed."""
    cal = calibration_for(inst, fexec, base, workdir, fraction)
    return None if cal is None else cal["target_ratio"]
