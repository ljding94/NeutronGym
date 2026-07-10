"""Tests added during the M1 review: hardening, reproducibility, diagnostics."""

import pytest

from mcstas_mcp import catalog, execution, registry
from mcstas_mcp.registry import SpecError


def _minimal(spec):
    registry.add_component(
        spec, "src", "Source_simple", at=[0, 0, 0],
        parameters={"xwidth": 0.02, "yheight": 0.02, "dist": 2,
                    "focus_xw": 0.05, "focus_yh": 0.05, "lambda0": 5, "dlambda": 1},
    )
    registry.add_component(
        spec, "psd", "PSD_monitor", at=[0, 0, 2], relative="src",
        parameters={"xwidth": 0.1, "yheight": 0.1, "filename": "psd.dat"},
    )
    return spec


def test_describe_cache_immune_to_mutation():
    d = catalog.describe("Guide")
    d["parameters"].clear()
    assert catalog.describe("Guide")["parameters"], "cache was mutated by caller"


def test_remove_component(spec):
    _minimal(spec)
    registry.remove_component(spec, "psd")
    assert [c["name"] for c in spec["components"]] == ["src"]


def test_remove_component_with_dependents_refused(spec):
    _minimal(spec)  # psd is RELATIVE to src
    with pytest.raises(SpecError, match="RELATIVE to it"):
        registry.remove_component(spec, "src")


def test_remove_unknown_component(spec):
    with pytest.raises(SpecError, match="No component named"):
        registry.remove_component(spec, "ghost")


def test_get_instrument_survives_incomplete_instrument(spec):
    """Incomplete instruments must still be inspectable (was a crash)."""
    from tests.test_server import call

    registry.add_component(spec, "guide", "Guide", at=[0, 0, 1])  # w1/h1/l unset
    out = call("get_instrument", instrument_id="test_instr")
    assert out["ok"]
    assert out["missing_required"] == {"guide": ["w1", "h1", "l"]}
    assert out["instr_source"] is None and "instr_source_error" in out


def test_job_ids_unique_within_one_second(spec):
    """rule 8: deterministic, collision-free output dirs."""
    ids = set()
    for _ in range(3):
        try:
            execution.run_instr_file("/dev/null", {}, ncount=1e3, timeout=1,
                                     workdir=registry.workdir("test_instr"))
        except Exception:
            pass
        jobs = execution._load_jobs()
        ids.update(jobs)
    assert len(ids) == 3


@pytest.mark.slow
def test_seed_reproducibility(spec):
    """Same seed + ncount = bit-identical intensity (benchmark relies on it)."""
    _minimal(spec)
    s = registry.load("test_instr")
    r1 = execution.run_spec(s, ncount=1e4, seed=99)
    r2 = execution.run_spec(s, ncount=1e4, seed=99)
    assert r1["ok"] and r2["ok"]
    m1 = execution.job_results(r1["job_id"])["monitors"][0]
    m2 = execution.job_results(r2["job_id"])["monitors"][0]
    assert m1["intensity"] == m2["intensity"]
    assert m1["events"] == m2["events"]


def test_mcstasscript_check_reaches_agent_cleanly(spec):
    """A bare allowlisted identifier ('sqrt') passes our check but is caught
    by McStasScript's write-time check — the agent must get a clean error,
    not a traceback."""
    from tests.test_server import call

    registry.add_component(
        spec, "src", "Source_simple", at=[0, 0, 0],
        parameters={"xwidth": 0.02, "yheight": 0.02, "dist": 2,
                    "focus_xw": 0.05, "focus_yh": 0.05,
                    "lambda0": "sqrt", "dlambda": 1},
    )
    out = call("run_simulation", instrument_id="test_instr", ncount=1e3)
    assert not out["ok"]
    assert "not recognized" in out["error"]


@pytest.mark.slow
def test_compile_failure_diagnostics_captured(spec):
    """A value that passes both our validation AND McStasScript's (non-alpha
    strings are skipped by its checker) but breaks compilation must come back
    with stage + diagnostics (backengine would have hidden this)."""
    registry.add_component(
        spec, "src", "Source_simple", at=[0, 0, 0],
        # 'sqrt(' passes both validators (identifier sqrt is allowlisted;
        # McStasScript skips non-isalpha values) but is a C syntax error
        parameters={"xwidth": 0.02, "yheight": 0.02, "dist": 2,
                    "focus_xw": 0.05, "focus_yh": 0.05,
                    "lambda0": "sqrt(", "dlambda": 1},
    )
    job = execution.run_spec(registry.load("test_instr"), ncount=1e3)
    assert not job["ok"]
    assert job["stage"] in ("translate", "compile")
    assert any("error" in ln.lower() for ln in job["diagnostics"])
