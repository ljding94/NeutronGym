"""Ground-truth tests: shipped examples vs their %Example expected values.

Every McStas example header carries '%Example: <params> Detector: <mon>_I=<v>'
— the mctest mechanism. Running the example and comparing integrated
intensity validates the whole chain (mcrun subprocess -> mccode.sim parsing)
and is the seed of the benchmark T1 grading harness.
"""

import os
import re

import pytest

from mcstas_mcp import execution, results
from mcstas_mcp.config import resources_dir

CASES = ["Templates/templateSANS/templateSANS.instr",
         "Templates/templateTOF/templateTOF.instr"]


def _example_line(instr_path):
    text = open(instr_path, errors="replace").read()
    m = re.search(r"%Example:\s*(.*?)\s*Detector:\s*(\w+)_I=([-\d.eE+]+)", text)
    if not m:
        pytest.skip(f"no %Example line in {instr_path}")
    params = dict(re.findall(r"(\w+)=(\S+)", m.group(1)))
    return params, m.group(2), float(m.group(3))


@pytest.mark.slow
@pytest.mark.parametrize("rel", CASES)
def test_shipped_example_matches_expected(rel, tmp_path):
    instr = os.path.join(resources_dir(), "examples", rel)
    params, monitor, expected = _example_line(instr)

    job = execution.run_instr_file(instr, params, ncount=1e6,
                                   workdir=str(tmp_path), seed=1)
    assert job["ok"], job.get("diagnostics")

    summary = results.summarize(job["output_dir"])
    mon = next(m for m in summary["monitors"] if m["component"] == monitor)
    assert mon["events"] > 1000, "not enough statistics to grade"
    # %Example values are often quoted to 1-2 significant figures, so allow
    # 15% model tolerance or 5 sigma of this run's own statistics
    tol = max(0.15 * abs(expected), 5 * mon["intensity_err"])
    assert abs(mon["intensity"] - expected) < tol, (
        f"{monitor}: got {mon['intensity']:g}, expected {expected:g} (tol {tol:g})"
    )
