"""Generated result table: interval and paired maths."""

import os
import sys

import pytest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))), "benchmark", "harness"))

import m8_table as t  # noqa: E402


def test_clopper_pearson_against_known_values():
    lo, hi = t.clopper_pearson(230, 300)          # the reported main row
    assert lo == pytest.approx(0.714, abs=0.006) and hi == pytest.approx(0.815, abs=0.006)
    assert lo < 230 / 300 < hi
    assert t.clopper_pearson(0, 300)[0] == 0.0
    assert t.clopper_pearson(300, 300)[1] == 1.0
    lo0, hi0 = t.clopper_pearson(0, 300)
    assert hi0 == pytest.approx(0.0122, abs=0.002)


def test_rate_counts_only_level_4_passes_and_flags_errors():
    rows = [{"instance": 0, "best_level": 4}, {"instance": 1, "best_level": 3},
            {"instance": 2, "best_level": None, "error": "boom"}]
    r = t.rate(rows)
    assert r["passes"] == 1 and r["n"] == 3 and r["errored"] == 1


def test_paired_uses_shared_instances_only():
    a = [{"instance": i, "best_level": 4 if i < 5 else 3} for i in range(10)]
    b = [{"instance": i, "best_level": 4 if i == 0 else 3} for i in range(8)]
    p = t.paired(a, b)
    assert p["n"] == 8 and p["only_a"] == 4 and p["only_b"] == 0
    # exact McNemar on (4, 0) discordant pairs: 2 * (1/2)**4
    assert p["mcnemar_p"] == pytest.approx(0.125)


def test_specs_cover_every_gated_family():
    """The table is generated, never transcribed -- so a family that reaches
    the paper must be regenerable. bender and the sans_match 32B arm were
    added 2026-09-21; before that the generator only knew guide_match, so
    the README's 'regenerate from these records' no longer reproduced the
    paper's table."""
    assert set(t.SPECS) == {"guide_match", "sans_match", "tof_chopper", "bender"}
    for fam, spec in t.SPECS.items():
        arms = [label for label, _, _ in spec["arms"]]
        assert "untrained 8B" in arms, fam
        assert "untrained 32B" in arms, fam          # the four-family claim
        assert any(a.startswith("GRPO 8B") for a in arms), fam
        assert spec["slice"], fam


def test_sans_match_is_reported_on_the_unbiased_slice():
    """300-599 chose sans_match's checkpoint, so only 600-899 is unbiased
    for the model selection picked. Every other family reports on 300-599."""
    assert "600-899" in t.SPECS["sans_match"]["slice"]
    for fam in ("guide_match", "tof_chopper", "bender"):
        assert "300-599" in t.SPECS[fam]["slice"], fam
    for _, fname, _ in t.SPECS["sans_match"]["arms"]:
        assert "unbiased" in fname, fname


def test_build_pairs_trained_against_both_untrained_arms(tmp_path):
    def rec(passes):
        return {"heldout": {"arm": {"bender": {"rows": [
            {"instance": i, "best_level": 4 if i < passes else 3} for i in range(10)]}}}}
    import json as _json
    files = {"eval_bender_fresh_untrained-8b.json": 2,
             "eval_bender_fresh_untrained-32b.json": 4,
             "eval_bender_fresh_trained-8b.json": 8}
    for name, k in files.items():
        r = rec(k)
        r["heldout"][{"eval_bender_fresh_untrained-8b.json": "untrained-8b",
                      "eval_bender_fresh_untrained-32b.json": "untrained-32b",
                      "eval_bender_fresh_trained-8b.json": "trained-8b"}[name]] = \
            r["heldout"].pop("arm")
        (tmp_path / name).write_text(_json.dumps(r))
    built = t.build("bender", str(tmp_path))
    assert built is not None
    assert "GRPO 8B vs untrained 8B" in built["paired"]
    assert "GRPO 8B vs untrained 32B" in built["paired"]


def test_build_returns_none_when_no_records_present(tmp_path):
    assert t.build("bender", str(tmp_path)) is None
