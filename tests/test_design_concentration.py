"""The diagnostic that caught bender v1's class-degeneracy (2026-09-21)."""

import json
import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))), "benchmark", "harness"))

import design_concentration as dc  # noqa: E402


def _row(instance, level, action=None):
    return {"instance": instance, "best_level": level, "pass_action": action}


def test_fully_instance_specific_policy_scores_one():
    """tof_chopper's signature: 138 distinct designs for 140 passes."""
    rows = [_row(i, 4, {"nu": float(i)}) for i in range(20)]
    rec = dc.concentration(rows)
    assert rec["passes"] == 20
    assert rec["distinct"] == 20
    assert rec["distinct_frac"] == 1.0
    assert rec["max_reuse"] == 1


def test_one_design_solving_many_instances_is_visible():
    """bender v1's signature: one geometry passed 17 instances."""
    shared = {"r_curve": 250.0, "w_ch": 0.06, "m_coat": 3.5}
    rows = [_row(i, 4, shared) for i in range(17)]
    rows += [_row(100 + i, 4, {"r_curve": float(i)}) for i in range(5)]
    rec = dc.concentration(rows)
    assert rec["passes"] == 22
    assert rec["max_reuse"] == 17
    assert rec["max_reuse_design"] == shared
    assert rec["max_reuse_instances"] == list(range(17))
    assert rec["distinct_frac"] < 0.3
    assert rec["top_n_share"] > 0.9


def test_failing_rows_and_missing_designs_are_not_counted():
    rows = [_row(1, 3, {"a": 1}),            # did not pass
            _row(2, 4, None),                 # passed, no design recorded
            _row(3, 4, {"a": 2})]
    rec = dc.concentration(rows)
    assert rec["passes"] == 1 and rec["distinct"] == 1


def test_no_passes_does_not_divide_by_zero():
    rec = dc.concentration([_row(1, 3), _row(2, 2)])
    assert rec["passes"] == 0 and rec["distinct_frac"] is None
    assert rec["max_reuse"] == 0 and rec["max_reuse_design"] is None


def test_target_span_flags_a_design_covering_the_whole_range():
    """The tell: one design passing instances whose targets span nearly the
    entire range is not solving them through the physics."""
    targets = {i: [3.0 + 0.1 * i, 0.5] for i in range(50)}   # 3.0 .. 7.9
    wide = dc.target_span(targets, list(range(0, 50, 5)))    # 3.0 .. 7.5
    assert wide["max_span_fraction"] > 0.9
    narrow = dc.target_span(targets, [10, 11, 12])           # 4.0 .. 4.2
    assert narrow["max_span_fraction"] < 0.1


def test_target_span_handles_degenerate_inputs():
    assert dc.target_span({}, [1]) is None
    assert dc.target_span({1: [5.0]}, []) is None
    assert dc.target_span({1: [5.0], 2: [5.0]}, [1]) is None   # no range at all


def test_load_rows_reads_the_eval_layout(tmp_path):
    payload = {"heldout": {"trained-8b": {"bender": {"rows": [_row(1, 4, {"a": 1})]}}}}
    p = tmp_path / "eval.json"
    p.write_text(json.dumps(payload))
    rows = dc.load_rows(str(p), "trained-8b", "bender")
    assert len(rows) == 1 and rows[0]["instance"] == 1
