"""Readout-rule probe (2026-09-17): the gate must see context-reading rules.

A no-model rule -- guide w_out and m_coat at the limits the prompt prints --
passed 96.3% of held-out instances while the constant-policy gate reported
the family clean. These pin that every rule emits a VALID action at the
stated limit, so a probe failure means the rule is weak, not malformed.
"""

from neutrongym import generate, hacks, reward

N = 60


def _rules(fam):
    return hacks.readout_rules(fam, generate.FAMILIES[fam]["free_parameters"])


def test_guide_rules_sit_exactly_on_the_stated_limits_and_are_valid():
    rules = _rules("guide_divergence")
    assert len(rules) == hacks.READOUT_LEVELS
    for i in range(N):
        inst = generate.instance("guide_divergence", "heldout", i)
        c = inst["context"]
        for _, fn in rules[::4]:
            a = fn(c)
            assert reward._check_l1(inst, a)["pass"], (i, a)
            assert a["w_out"] == c["det_wh"]
            assert abs(a["m_coat"] - generate.guide_max_m(c)) < 1e-3


def test_sans_rules_put_one_pinhole_on_the_limit_and_are_valid():
    rules = _rules("sans_collimation")
    assert len(rules) == 2 * hacks.READOUT_LEVELS
    applicable = 0
    for i in range(N):
        inst = generate.instance("sans_collimation", "heldout", i)
        for _, fn in rules:
            a = fn(inst["context"])
            if a is None:
                continue
            applicable += 1
            assert reward._check_l1(inst, a)["pass"], (i, a)
    assert applicable > N * 5


class _Env:
    """Pass iff the action's w_in equals the instance's lucky value."""

    def __init__(self):
        self.i = None

    def reset(self, index):
        self.i = index
        inst = generate.instance("guide_divergence", "heldout", index)
        return {"instance": inst}, {}

    def step(self, a):
        return {}, 0.0, False, False, {"level": 4 if a["w_in"] == 0.06 else 3}


def test_probe_counts_passes_per_rule_and_feeds_the_gate_summary():
    rules = [("w_in=0.06", lambda c, inst=None: {"w_in": 0.06, "w_out": c["det_wh"], "m_coat": 1.0}),
             ("w_in=0.05", lambda c, inst=None: {"w_in": 0.05, "w_out": c["det_wh"], "m_coat": 1.0}),
             ("n/a", lambda c, inst=None: None)]
    probe = hacks.readout_policy_probe(_Env(), range(150), rules)
    by = {r["action"]: r["passes"] for r in probe["results"]}
    assert by == {"w_in=0.06": 150, "w_in=0.05": 0, "n/a": 0}
    v = hacks.summarize_constant_probe(probe)
    assert v["ok"] is False and v["best_action"] == "w_in=0.06"


def test_physics_model_rules_are_reported_separately_from_copy_rules():
    probe = {"n_instances": 150, "results": [
        {"action": "w_out=limit m_coat=limit w_in=0.06", "passes": 10, "pass_rate": 0.07, "source": "readout"},
        {"action": "physics: optics inversion kdiv=0.577 w_in=0.08", "passes": 47, "pass_rate": 0.31, "source": "readout"}]}
    copy, phys = hacks.split_rule_results(probe)
    assert [r["passes"] for r in copy["results"]] == [10]
    assert [r["passes"] for r in phys["results"]] == [47]
    assert hacks.summarize_constant_probe(copy)["ok"] is True       # the gate
    assert hacks.summarize_constant_probe(phys)["ok"] is False      # the reference


def test_guide_match_rules_are_all_physics_model_rules():
    rules = hacks.readout_rules("guide_match", generate.FAMILIES["guide_match"]["free_parameters"])
    assert rules and all(l.startswith(hacks.PHYSICS_RULE_PREFIX) for l, _ in rules)
