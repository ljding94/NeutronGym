import glob
import os

import pytest

from mcstas_mcp import execution, registry, results
from mcstas_mcp.execution import RunError


def _minimal(spec):
    registry.add_parameter(spec, "wl", default=5.0)
    registry.add_component(
        spec, "src", "Source_simple", at=[0, 0, 0],
        parameters={"xwidth": 0.02, "yheight": 0.02, "dist": 2,
                    "focus_xw": 0.05, "focus_yh": 0.05, "lambda0": "wl", "dlambda": 1},
    )
    registry.add_component(
        spec, "psd", "PSD_monitor", at=[0, 0, 2], relative="src",
        parameters={"nx": 50, "ny": 50, "xwidth": 0.1, "yheight": 0.1,
                    "filename": "psd.dat"},
    )
    return spec


def test_run_refuses_missing_required(spec):
    registry.add_component(spec, "guide", "Guide", at=[0, 0, 1])
    with pytest.raises(RunError, match="guide: w1, h1, l"):
        execution.run_spec(spec, ncount=1e4)


def test_run_refuses_unknown_run_parameter(spec):
    _minimal(spec)
    with pytest.raises(RunError, match="Unknown instrument parameter"):
        execution.run_spec(spec, ncount=1e4, parameters={"lambda": 4})


def test_ncount_cap():
    with pytest.raises(RunError, match="cap"):
        execution.run_instr_file("/dev/null", {}, ncount=1e9)


def test_seed_zero_rejected():
    with pytest.raises(RunError, match="non-zero"):
        execution.run_instr_file("/dev/null", {}, seed=0)


@pytest.mark.slow
def test_minimal_run_end_to_end(spec):
    _minimal(spec)
    job = execution.run_spec(spec, ncount=2e4, seed=42)
    assert job["ok"], job.get("diagnostics")
    assert job["seed"] == 42

    summary = execution.job_results(job["job_id"])
    (mon,) = summary["monitors"]
    assert mon["component"] == "psd"
    assert mon["intensity"] > 0
    assert mon["events"] > 0
    # workdir hygiene: artifacts inside the instrument workdir only
    wd = registry.workdir("test_instr")
    assert os.path.isfile(os.path.join(wd, "test_instr.instr"))
    assert not glob.glob(os.path.join(os.getcwd(), "*.out"))

    png = results.monitor_png(job["output_dir"], "psd")
    assert os.path.getsize(png) > 5000

    arr = results.monitor_array(job["output_dir"], "psd")
    assert arr["kind"] == "2d_profiles" and arr["shape"] == [50, 50]


@pytest.mark.slow
def test_per_run_parameter_override(spec):
    _minimal(spec)
    job = execution.run_spec(spec, ncount=1e4, parameters={"wl": 8.0}, seed=7)
    assert job["ok"]
    summary = execution.job_results(job["job_id"])
    assert summary["run_parameters"]["wl"] == "8"
