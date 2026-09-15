"""Candidate selection for the no-model constant probe, now family-general."""

import json
import os
import sys

import pytest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))), "benchmark", "harness"))

import m8_constant_probe as cp  # noqa: E402


def _episode(final_action, earlier=None):
    msgs = [{"role": "system", "content": "s"}, {"role": "user", "content": "t"}]
    for a in (earlier or []) + [final_action]:
        msgs += [{"role": "assistant", "content": json.dumps(a)},
                 {"role": "user", "content": "deepest level reached: L3"}]
    return {"best_level": 4, "messages": msgs}


def test_modal_passing_actions_counts_final_turns_only(tmp_path):
    free = {"r_pin1": (0.001, 0.02), "r_pin2": (0.001, 0.02)}
    common = {"r_pin1": 0.008, "r_pin2": 0.006}
    rare = {"r_pin1": 0.004, "r_pin2": 0.004}
    probe_only = {"r_pin1": 0.02, "r_pin2": 0.02}
    recs = [_episode(common, earlier=[probe_only])] * 3 + [_episode(rare)]
    p = tmp_path / "train.jsonl"
    p.write_text("\n".join(json.dumps(r) for r in recs) + "\n")
    got = cp.modal_passing_actions(str(p), free)
    assert got[0] == common and got[1] == rare
    assert probe_only not in got          # exploration turns never count


def test_candidates_for_sans_use_the_gate_grid():
    got = cp.candidate_actions("sans_collimation", None, grid=True)
    assert len(got) in (25, 26)
    assert all(set(a) == {"r_pin1", "r_pin2"} for a in got)


def test_non_guide_family_requires_data_or_grid():
    with pytest.raises(SystemExit):
        cp.candidate_actions("sans_collimation", None, grid=False)


def test_guide_defaults_are_unchanged():
    assert cp.candidate_actions("guide_divergence", None, grid=False) == cp.DEFAULT_ACTIONS
