"""Reward-ladder regressions with a fake executor — every level's pass and
fail surface, shaping monotonicity, deepest-level-reached semantics. No
simulation: the ladder's logic must be testable in milliseconds."""

from neutrongym import generate, hacks, reward


def _summary(fom_intensity=0.001, events=5000, div_width=0.3, psd_width=0.9):
    return {"monitors": [
        {"component": "divmon", "intensity": fom_intensity, "events": events,
         "beam_width": {"dX": div_width, "dY": div_width}},
        {"component": "psd", "intensity": fom_intensity * 2, "events": 50000,
         "beam_width": {"dX": psd_width, "dY": psd_width}},
    ]}


class FakeExec:
    """Scripted executor: returns queued results per (ncount) call order."""

    def __init__(self, *results):
        self.queue = list(results)
        self.calls = []

    def run(self, params, ncount, seed, **kw):
        self.calls.append({"params": dict(params), "ncount": ncount,
                           "seed": seed})
        out = self.queue.pop(0)
        out.setdefault("elapsed_s", 0.01)
        return out


INST = generate.instance("guide_divergence", "train", 0)
BASE = {"ok": True, "fom": 0.001,
        "constraints": {"divmon.beam_width_x": 0.3, "psd.beam_width_x": 0.9}}
# spec-valid for this instance (2026-09-15 specifications): exit no wider
# than the sample, coating at or below the divergence-limited maximum
GOOD_ACTION = {"w_in": 0.05, "w_out": INST["context"]["det_wh"],
               "m_coat": round(min(2.5, generate.guide_max_m(INST["context"])) - 1e-3, 3)}
# Liouville bound for this instance and action — intensities below are set
# relative to it, so the test does not depend on the counted window size
BOUND = hacks._guide_bound(INST["context"], GOOD_ACTION)


def test_l1_rejects_bounds_extras_missing_and_nonnumbers():
    for action, frag in [
        ({"w_in": 0.5, "w_out": 0.02, "m_coat": 2}, "outside bounds"),
        ({"w_in": 0.02, "w_out": 0.02}, "missing"),
        ({**GOOD_ACTION, "hack": 1}, "extra"),
        ({"w_in": float("nan"), "w_out": 0.02, "m_coat": 2}, "finite"),
        ({"w_in": True, "w_out": 0.02, "m_coat": 2}, "finite"),
    ]:
        rec = reward.score(INST, action, FakeExec(), BASE)
        assert rec["level"] == 0 and rec["reward"] == 0.0
        assert frag in rec["levels"]["L1"]["detail"]
        assert "L2" not in rec["levels"]  # no simulation spent on L1 failures


def test_l2_runtime_failure_stops_ladder():
    fx = FakeExec({"ok": False, "stage": "run", "diagnostics": ["boom"]})
    rec = reward.score(INST, GOOD_ACTION, fx, BASE)
    assert rec["level"] == 1 and rec["reward"] == 0.25
    assert rec["levels"]["L2"]["detail"] == ["boom"]
    assert len(fx.calls) == 1  # terminal run never launched
    assert fx.calls[0]["ncount"] == INST["protocol"]["ncount_cheap"]


def test_l3_statistics_floor_and_constraint_bands():
    starved = FakeExec({"ok": True, "summary": _summary()},
                       {"ok": True, "summary": _summary(events=3)})
    rec = reward.score(INST, GOOD_ACTION, starved, BASE)
    assert rec["level"] == 2 and "floor" in rec["levels"]["L3"]["detail"]

    leaking = FakeExec({"ok": True, "summary": _summary()},
                       {"ok": True, "summary": _summary(div_width=0.02)})
    rec = reward.score(INST, GOOD_ACTION, leaking, BASE)
    assert rec["level"] == 2
    assert rec["levels"]["L3"]["detail"] == "constraint band violated"
    bad = [c for c in rec["levels"]["L3"]["constraints"] if not c["pass"]]
    assert bad and bad[0]["constraint"] == "divmon.beam_width_x"


def test_l4_improvement_and_shaping_monotone():
    def run(fom):
        fx = FakeExec({"ok": True, "summary": _summary()},
                      {"ok": True, "summary": _summary(fom_intensity=fom)})
        return reward.score(INST, GOOD_ACTION, fx, BASE)

    # all intensities physical: the largest stays under the Liouville bound
    assert 0.004 < BOUND
    worse, same, better, huge = (run(f) for f in
                                 (0.0005, 0.001, 0.002, 0.004))
    assert worse["level"] == 3 and not worse["levels"]["L4"]["pass"]
    assert same["level"] == 3  # resubmitting baseline is NOT an improvement
    assert better["level"] == 4 and better["levels"]["L4"]["pass"]
    assert (worse["reward"] < same["reward"] < better["reward"]
            <= huge["reward"] == 1.25)  # capped at ratio 2


def test_unphysical_gain_fails_l3():
    """Liouville gate: intensity above source-brightness x acceptance is a
    reward hack (or simulation artifact) by construction, whatever the
    constraints say."""
    fx = FakeExec({"ok": True, "summary": _summary()},
                  {"ok": True, "summary": _summary(fom_intensity=200 * BOUND)})
    rec = reward.score(INST, GOOD_ACTION, fx, BASE)
    assert rec["level"] == 2
    assert "unphysical_gain" in rec["levels"]["L3"]["detail"]
    lio = rec["levels"]["L3"]["liouville"]
    assert not lio["pass"] and lio["utilization"] > 100
    # a physical score carries the utilization analysis field
    fx2 = FakeExec({"ok": True, "summary": _summary()},
                   {"ok": True, "summary": _summary(fom_intensity=0.8 * BOUND)})
    rec2 = reward.score(INST, GOOD_ACTION, fx2, BASE)
    assert rec2["levels"]["L3"]["liouville"]["pass"]
    assert 0.5 < rec2["levels"]["L3"]["liouville"]["utilization"] < 1.1


def test_protocol_is_env_controlled():
    fx = FakeExec({"ok": True, "summary": _summary()},
                  {"ok": True, "summary": _summary()})
    reward.score(INST, GOOD_ACTION, fx, BASE)
    for call in fx.calls:
        assert call["seed"] == INST["protocol"]["seed"]
        # context params ride every run; the agent cannot override them
        for k, v in INST["context"].items():
            assert call["params"][k] == v


def test_get_observable_nested_and_flat():
    mon = {"intensity": 2.5, "beam_width": {"dX": 0.1},
           "center_of_mass": None}
    assert reward.get_observable(mon, "intensity") == 2.5
    assert reward.get_observable(mon, "beam_width_x") == 0.1
    assert reward.get_observable(mon, "beam_width_y") is None
    assert reward.get_observable(mon, "center_of_mass") is None
