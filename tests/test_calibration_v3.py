"""Calibration v3 (2026-09-16): multi-start adaptive search.

v2 ran one pattern search from the best of 31 samples for a fixed 6 rounds,
halving the step on every non-improving round. Optima sit on specification
boundaries where most +/-step moves are invalid, so the step collapsed early
(SANS stopped after 79 evals) and a fixed candidate from the degeneracy probe
beat the calibrated "optimum" on 60-70% of instances in every family -- by
more than 1/0.85 on 20-25%. These tests pin what v3 promises.
"""

from neutrongym import calibrate, hacks, reward


class _Boundary:
    """FOM = x * y, but x + y > 1 is infeasible (the run fails): the optimum
    (0.5, 0.5), FOM 0.25, sits ON the constraint boundary, where a
    coordinate search is most likely to stall."""

    def __init__(self):
        self.terminal = []

    def run(self, params, ncount, seed, **kw):
        x, y = params["x"], params["y"]
        if ncount == 1e5:
            self.terminal.append((round(x, 6), round(y, 6)))
        if x + y > 1.0 + 1e-12:
            return {"ok": False, "diagnostics": ["infeasible"]}
        return {"ok": True, "elapsed_s": 0.0, "summary": {"monitors": [
            {"component": "det", "intensity": x * y, "events": 1e6}]}}


def _inst(iid="bnd-1"):
    return {"id": iid, "family": "stub", "context": {}, "static_checks": [],
            "free_parameters": {"x": (0.0, 1.0), "y": (0.0, 1.0)},
            "baseline": {"x": 0.1, "y": 0.1},
            "protocol": {"ncount": 1e5, "ncount_cheap": 1e4, "seed": 5,
                         "statistics_floor": 500},
            "fom": {"monitor": "det", "metric": "intensity", "maximize": True},
            "constraints": []}


def _run(**kw):
    fx, inst = _Boundary(), _inst()
    base = reward.baseline(inst, fx)
    return fx, inst, calibrate.calibrate_instance(inst, fx, base, **kw)


def test_finds_an_optimum_that_sits_on_the_constraint_boundary():
    _, _, rec = _run()
    assert rec["ok"] and rec["version"] == 3
    assert rec["classical_fom"] >= 0.99 * 0.25, rec["classical_action"]
    assert rec["classical_action"]["x"] + rec["classical_action"]["y"] <= 1.0


def test_no_action_is_simulated_twice():
    fx, _, _ = _run()
    # the baseline run by reward.baseline is the only permitted repeat
    counts = {}
    for a in fx.terminal:
        counts[a] = counts.get(a, 0) + 1
    repeats = {a: c for a, c in counts.items() if c > 1 and a != (0.1, 0.1)}
    assert not repeats, repeats


def test_the_probe_grid_is_always_a_seed():
    """No fixed answer the degeneracy gate tries may beat the optimum simply
    because calibration never looked at it."""
    fx, inst, _ = _run()
    seen = set(fx.terminal)
    grid = hacks.constant_candidates(inst)
    feasible = [g for g in grid if g["x"] + g["y"] <= 1.0]
    assert feasible
    for g in feasible:
        assert (round(g["x"], 6), round(g["y"], 6)) in seen, g


def test_optimum_is_never_below_the_best_seed_or_baseline():
    fx, inst, rec = _run()
    grid = [g for g in hacks.constant_candidates(inst) if g["x"] + g["y"] <= 1.0]
    assert rec["classical_fom"] >= max(g["x"] * g["y"] for g in grid)
    assert rec["classical_over_baseline"] >= 1.0


def test_eval_cap_is_respected_and_reported():
    fx, _, rec = _run(max_evals=40)
    assert rec["ok"] and rec["evals"] <= 40 and rec["hit_eval_cap"] is True
    assert len(set(fx.terminal)) <= 41       # + the baseline's own run


def test_v2_caches_are_never_read():
    assert calibrate.CAL_DIR == "calibration_v3"
