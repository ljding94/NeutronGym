import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))), "benchmark", "harness"))

import precalibrate  # noqa: E402


def test_jobs_cover_exactly_the_requested_range():
    j = precalibrate.jobs("sans_collimation", "train", 300, 5)
    assert j == [("sans_collimation", "train", i) for i in (300, 301, 302, 303, 304)]
    assert precalibrate.jobs("x", "heldout", 0, 0) == []
