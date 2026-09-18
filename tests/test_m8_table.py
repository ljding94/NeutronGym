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
