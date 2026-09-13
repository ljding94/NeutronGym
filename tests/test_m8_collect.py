"""RAFT collection runner: only real L4 passes become training data.

The keep rule is the whole point of RAFT — if a non-pass slips into the SFT
set, the model is trained on behaviour the reward rejected and the
"the reward trains" claim is contaminated.
"""

import json
import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))), "benchmark", "harness"))

import m8_collect  # noqa: E402


def _write(path, recs):
    with open(path, "w") as f:
        for r in recs:
            f.write(json.dumps(r) + "\n")


def test_strict_threshold_excludes_resubmitting_the_target():
    """reward = 0.75 + 0.25 * min(ratio, 2); ratio exactly 1.0 -> reward
    exactly 1.0, which the ladder does NOT count as an L4 pass."""
    assert 1.0 < m8_collect.STRICT_L4_REWARD
    assert 0.75 + 0.25 * 1.001 >= m8_collect.STRICT_L4_REWARD


def test_merge_keeps_only_l4_and_counts_per_family(tmp_path):
    msgs = [{"role": "user", "content": "x"}]
    _write(tmp_path / "guide_divergence.jsonl", [
        {"instance_id": "g0", "best_level": 4, "messages": msgs},
        {"instance_id": "g1", "best_level": 3, "messages": msgs},
    ])
    _write(tmp_path / "sans_collimation.jsonl", [
        {"instance_id": "s0", "best_level": 4, "messages": msgs},
    ])
    stats = m8_collect.merge(str(tmp_path),
                             ["guide_divergence", "sans_collimation"])
    assert stats == {"train_examples": 2,
                     "by_family": {"guide_divergence": 1,
                                   "sans_collimation": 1}}
    ids = [json.loads(line)["instance_id"]
           for line in open(tmp_path / "train.jsonl")]
    assert ids == ["g0", "s0"]


def test_merge_tolerates_a_family_that_never_ran(tmp_path):
    """An interrupted run leaves a family file missing; merging must report
    zero for it rather than crash or invent data."""
    _write(tmp_path / "guide_divergence.jsonl",
           [{"instance_id": "g0", "best_level": 4, "messages": []}])
    stats = m8_collect.merge(str(tmp_path),
                             ["guide_divergence", "sans_collimation"])
    assert stats["by_family"]["sans_collimation"] == 0
    assert stats["train_examples"] == 1
