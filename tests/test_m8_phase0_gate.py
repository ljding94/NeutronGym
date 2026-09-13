"""Phase-0 gate arithmetic.

The gate decides whether an entire training milestone runs, so its two
thresholds are tested directly. Both had defects found on 2026-09-13:

1. RAFT feasibility was `keepers >= 20` measured against `--n` instances
   per family. At the default --n 10 that is 20 train rollouts total, so it
   demanded a 100% keep rate and could only ever fail. Feasibility is a
   RATE: procedural instances are unlimited and free, so a healthy rate
   plus more instances is all a larger SFT set requires.
2. The vacuity call is a comparison of two sample proportions and can flip
   with n. It did: n=10/family read 0.70 vs 0.70, n=25/family read 0.60 vs
   0.76 on the same generator and the same bar.
"""

import importlib
import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))), "benchmark", "harness"))

phase0 = importlib.import_module("m8_phase0")


def test_feasibility_threshold_is_a_rate_not_a_count():
    assert 0 < phase0.MIN_KEEP_RATE < 1
    assert phase0.TARGET_KEEPERS >= 100


def test_the_observed_half_keep_rate_is_feasible():
    """0.5 was reported as a FAIL by the old count-based gate purely
    because only 20 rollouts had been sampled. It is a healthy rate."""
    assert 0.5 >= phase0.MIN_KEEP_RATE


def test_instances_needed_scales_inversely_with_rate():
    def needed(rate):
        return int(-(-phase0.TARGET_KEEPERS // rate))
    assert needed(1.0) == phase0.TARGET_KEEPERS
    assert needed(0.5) == 2 * phase0.TARGET_KEEPERS
    assert needed(0.25) == 4 * phase0.TARGET_KEEPERS
    # always rounds UP: a rate that leaves a remainder must not
    # under-provision the sampling run
    assert needed(0.3) * 0.3 >= phase0.TARGET_KEEPERS


def test_a_dead_rate_is_infeasible():
    assert not (0.0 >= phase0.MIN_KEEP_RATE)
    assert not (0.05 >= phase0.MIN_KEEP_RATE)
