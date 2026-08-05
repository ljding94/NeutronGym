"""Procedural generator regressions: determinism, held-out regimes,
instance schema, prompt rendering. All fast (no simulation)."""

from neutrongym import generate


def test_instances_deterministic_across_calls():
    a = generate.instance("guide_divergence", "train", 42)
    b = generate.instance("guide_divergence", "train", 42)
    assert a == b
    assert a["protocol"]["seed"] == b["protocol"]["seed"] != 0


def test_instances_differ_by_index_and_split():
    ids = {generate.instance("guide_divergence", s, i)["protocol"]["seed"]
           for s in ("train", "heldout") for i in range(20)}
    assert len(ids) == 40  # no seed collisions in a small draw
    a = generate.instance("guide_divergence", "train", 0)["context"]
    b = generate.instance("guide_divergence", "train", 1)["context"]
    assert a != b


def test_heldout_regime_disjoint_from_train():
    for fam, cfg in generate.FAMILIES.items():
        disjoint = [k for k, rr in cfg["context"].items()
                    if rr["train"][1] < rr["heldout"][0]
                    or rr["heldout"][1] < rr["train"][0]]
        assert disjoint, f"{fam}: no context parameter has a held-out regime"
        for i in range(50):
            tr = generate.instance(fam, "train", i)["context"]
            ho = generate.instance(fam, "heldout", i)["context"]
            for k in disjoint:
                lo, hi = cfg["context"][k]["train"]
                assert lo <= tr[k] <= hi
                assert not lo <= ho[k] <= hi  # genuinely outside train range


def test_context_and_free_parameters_are_disjoint_instrument_params(tmp_path):
    from neutrongym.executor import read_define_params
    for fam in generate.FAMILIES:
        inst = generate.instance(fam, "train", 0)
        path = generate.family_instr(fam, str(tmp_path))
        declared = set(read_define_params(path))
        assert set(inst["context"]).isdisjoint(inst["free_parameters"])
        assert set(inst["context"]) | set(inst["free_parameters"]) == declared


def test_bad_split_rejected():
    import pytest
    with pytest.raises(ValueError):
        generate.instance("guide_divergence", "test", 0)


def test_prompt_contains_the_contract():
    inst = generate.instance("sans_collimation", "train", 7)
    p = generate.render_prompt(inst, {"fom": 0.123})
    for needle in (inst["id"], "r_pin1", "baseline", "maximize",
                   "0.123", "env-controlled seed"):
        assert needle in p, needle
    # fixed context values are disclosed; free params carry bounds
    assert str(inst["context"]["L_coll"]) in p
