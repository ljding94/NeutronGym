"""Matched-compute classical baseline: the budget must actually bind."""

import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))), "benchmark", "harness"))

import classical_baseline as cb  # noqa: E402


class _Env:
    """Scores |w_in - 0.05| as the miss; passes below 0.001."""

    def __init__(self):
        self.calls = 0

    def reset(self, index):
        from neutrongym import generate
        return {"instance": generate.instance("guide_match", "heldout", index)}, {}

    def step(self, a):
        self.calls += 1
        err = abs(a["w_in"] - 0.05)
        return {}, 0.0, False, False, {"level": 4 if err < 0.001 else 3,
                                       "match": {"rel_err": [err, 0.0]}}


def test_budget_counts_every_simulation_and_stops():
    env = _Env()
    b = cb.Budget(env, 0, budget=10)
    for _ in range(25):
        b({"w_in": 0.09})
    assert env.calls == 10 and b.used == 10


def test_pass_is_recorded_when_any_evaluated_design_passes():
    env = _Env()
    b = cb.Budget(env, 0, budget=10)
    b({"w_in": 0.09}); assert not b.passed
    b({"w_in": 0.0505}); assert b.passed
    assert b.best is not None and b.best < 0.001


def test_invalid_designs_score_worst_not_best():
    class _Invalid(_Env):
        def step(self, a):
            self.calls += 1
            return {}, 0.0, False, False, {"level": 0}      # no "match" -> invalid

    b = cb.Budget(_Invalid(), 0, budget=3)
    assert b({"w_in": 0.05}) == 1e6 and b.best is None


def test_summarize_reports_per_method_pass_rates():
    rows = [{"method": "random", "passed": True, "best_error": 0.01, "sims_used": 10},
            {"method": "random", "passed": False, "best_error": 0.2, "sims_used": 10},
            {"method": "coordinate", "passed": True, "best_error": 0.02, "sims_used": 9}]
    s = cb.summarize(rows)
    assert s["random"]["pass_rate"] == 0.5 and s["coordinate"]["passes"] == 1
    assert s["random"]["median_sims"] == 10
