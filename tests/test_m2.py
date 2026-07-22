"""M2 tests: async jobs, restart survival, binary caching, spec extensions
(declares/WHEN/EXTEND/GROUP/SPLIT, string parameters), escape hatches,
examples corpus."""

import os
import subprocess
import time

import pytest

from mcstas_mcp import examples, execution, registry
from mcstas_mcp.config import resources_dir
from mcstas_mcp.execution import RunError
from mcstas_mcp.registry import SpecError
from tests.test_server import call

TEMPLATE_SANS = os.path.join(
    resources_dir(), "examples", "Templates", "templateSANS", "templateSANS.instr")


def _minimal(spec):
    registry.add_component(
        spec, "src", "Source_simple", at=[0, 0, 0],
        parameters={"xwidth": 0.02, "yheight": 0.02, "dist": 2,
                    "focus_xw": 0.05, "focus_yh": 0.05, "lambda0": 5, "dlambda": 1})
    registry.add_component(
        spec, "psd", "PSD_monitor", at=[0, 0, 2], relative="src",
        parameters={"xwidth": 0.1, "yheight": 0.1, "filename": "psd.dat"})
    return spec


def _poll_done(job_id, max_s=90):
    t0 = time.time()
    while time.time() - t0 < max_s:
        st = execution.job_status(job_id)
        if st["state"] != "running":
            return st
        time.sleep(0.5)
    raise TimeoutError(f"job {job_id} still running after {max_s}s")


# --- spec extensions -----------------------------------------------------------

def test_string_parameter_and_declare_render(spec):
    registry.add_parameter(spec, "fname", default="beam.dat", ptype="string")
    registry.add_declare(spec, "double", "counter", value=0)
    registry.add_component(
        spec, "src", "Source_simple", at=[0, 0, 0],
        parameters={"xwidth": 0.02, "yheight": 0.02, "dist": 2,
                    "focus_xw": 0.05, "focus_yh": 0.05, "lambda0": 5, "dlambda": 1})
    registry.add_component(
        spec, "psd", "PSD_monitor", at=[0, 0, 2], relative="src",
        when="counter >= 0",
        extend="counter = counter + 1;",
        parameters={"xwidth": 0.1, "yheight": 0.1, "filename": "fname"})
    src = registry.instr_source(registry.load("test_instr"))
    assert "string fname" in src and '"beam.dat"' in src
    assert "double counter" in src
    assert "WHEN (counter >= 0)" in src
    assert "EXTEND %{" in src and "counter = counter + 1;" in src
    assert "filename = fname" in src


def test_split_and_group_render(spec):
    _minimal(spec)
    registry.add_component(spec, "arm", "Arm", at=[0, 0, 1], relative="src",
                           group="monos", split=10)
    src = registry.instr_source(registry.load("test_instr"))
    assert "SPLIT 10 COMPONENT arm" in src
    assert "GROUP monos" in src


def test_when_validation_rejects_unknown_identifier(spec):
    _minimal(spec)
    with pytest.raises(SpecError, match="WHEN condition"):
        registry.add_component(spec, "a", "Arm", at=[0, 0, 1], when="mystery > 2")
    # particle state vars are legal
    registry.add_component(spec, "b", "Arm", at=[0, 0, 1], when="vz > 100")


def test_numeric_param_referencing_string_param_rejected(spec):
    registry.add_parameter(spec, "fname", default="x.dat", ptype="string")
    with pytest.raises(SpecError, match="string-typed"):
        registry.add_component(
            spec, "psd", "PSD_monitor", at=[0, 0, 1],
            parameters={"filename": "fname", "xwidth": "fname"})


# --- escape hatches ------------------------------------------------------------

def test_load_template_sans(spec):
    loaded, warnings = registry.load_from_instr(TEMPLATE_SANS, name="tsans")
    assert not warnings, warnings
    assert len(loaded["components"]) >= 8
    assert {p["name"] for p in loaded["parameters"]} >= {"lambda", "dlambda", "r"}
    assert any(c["split"] for c in loaded["components"]), "SPLIT lost in import"
    src = registry.instr_source(loaded)  # must rebuild to valid .instr
    assert "DEFINE INSTRUMENT tsans" in src


def test_load_failure_names_reader_limits():
    basis = os.path.join(resources_dir(), "examples", "SNS", "SNS_BASIS",
                         "SNS_BASIS.instr")
    with pytest.raises(SpecError, match="reader failed"):
        registry.load_from_instr(basis, name="basis_import")
    assert not registry.exists("basis_import")  # no half-imported leftovers


def test_export_instr_file(spec, tmp_path):
    _minimal(spec)
    dest = str(tmp_path / "exported.instr")
    out = call("export_instr_file", instrument_id="test_instr", path=dest)
    assert out["ok"] and os.path.isfile(out["path"])


# --- examples corpus -----------------------------------------------------------

def test_examples_index_and_search():
    all_ex = examples.list_examples()
    assert len(all_ex) >= 290
    hits = {e["name"] for e in examples.list_examples(search="templateSANS")}
    assert "templateSANS" in hits


def test_get_example_over_mcp():
    out = call("get_example", name="templateSANS")
    assert out["ok"] and "DEFINE INSTRUMENT" in out["source"]
    assert "detector_I=" in out["example_line"]
    bad = call("get_example", name="templateSAN")
    assert not bad["ok"] and "templateSANS" in bad["error"]


# --- async job model -----------------------------------------------------------

@pytest.mark.slow
def test_async_submit_poll_results(spec):
    _minimal(spec)
    job = execution.run_spec(registry.load("test_instr"), ncount=1e6, wait=0)
    assert job["state"] == "running" or job["state"] == "done"
    st = _poll_done(job["job_id"])
    assert st["state"] == "done", st
    res = execution.job_results(job["job_id"])
    assert res["ok"] and res["monitors"][0]["intensity"] > 0


@pytest.mark.slow
def test_results_on_running_job_says_poll(spec):
    _minimal(spec)
    job = execution.run_spec(registry.load("test_instr"), ncount=5e7, wait=0)
    try:
        res = execution.job_results(job["job_id"])
        if res.get("state") == "running":  # unless it finished very fast
            assert "job_status" in res["error"]
    finally:
        try:
            execution.cancel(job["job_id"])
        except RunError:
            pass


@pytest.mark.slow
def test_cancel_kills_process_group(spec):
    _minimal(spec)
    job = execution.run_spec(registry.load("test_instr"), ncount=1e8, wait=0)
    time.sleep(1)
    rec = execution.cancel(job["job_id"])
    assert rec["state"] == "cancelled"
    time.sleep(0.5)
    survivors = subprocess.run(["pgrep", "-f", "test_instr.out"], capture_output=True)
    assert survivors.returncode != 0, "simulation survived cancel_job"


@pytest.mark.slow
def test_binary_caching(spec):
    _minimal(spec)
    s = registry.load("test_instr")
    j1 = execution.run_spec(s, ncount=1e4, seed=1)
    j2 = execution.run_spec(registry.load("test_instr"), ncount=1e4, seed=2)
    assert j1["compiled"] is True
    assert j2["compiled"] is False, "unchanged instrument recompiled"
    assert j2["ok"]
    # changing a component parameter changes the .instr -> recompile
    registry.set_parameters(registry.load("test_instr"), "psd", {"nx": 64})
    j3 = execution.run_spec(registry.load("test_instr"), ncount=1e4, seed=3)
    assert j3["compiled"] is True and j3["ok"]


@pytest.mark.slow
def test_loaded_example_reproduces_direct_run(tmp_path):
    """Converter fidelity: templateSANS imported through the reader must give
    the same physics as running the shipped file directly (same seed)."""
    direct = execution.run_instr_file(
        TEMPLATE_SANS, {"lambda": 6}, ncount=1e5, seed=7, workdir=str(tmp_path))
    assert direct["ok"], direct.get("diagnostics")
    loaded, _ = registry.load_from_instr(TEMPLATE_SANS, name="tsans_fid")
    imported = execution.run_spec(loaded, ncount=1e5, seed=7)
    assert imported["ok"], imported.get("diagnostics")
    d = execution.job_results(direct["job_id"])
    i = execution.job_results(imported["job_id"])
    dm = next(m for m in d["monitors"] if m["component"] == "detector")
    im = next(m for m in i["monitors"] if m["component"] == "detector")
    tol = 3 * (dm["intensity_err"] + im["intensity_err"])
    assert abs(dm["intensity"] - im["intensity"]) <= tol


@pytest.mark.slow
def test_validate_instrument_tool(spec):
    registry.add_component(spec, "guide", "Guide", at=[0, 0, 1])
    out = call("validate_instrument", instrument_id="test_instr")
    assert not out["ok"] and "w1" in out["error"]
    registry.set_parameters(registry.load("test_instr"), "guide",
                            {"w1": 0.03, "h1": 0.05, "l": 5})
    _minimal_after_guide(registry.load("test_instr"))
    out2 = call("validate_instrument", instrument_id="test_instr")
    assert out2["ok"], out2


def _minimal_after_guide(spec):
    registry.add_component(
        spec, "src", "Source_simple", at=[0, 0, 0], after=None,
        parameters={"xwidth": 0.02, "yheight": 0.02, "dist": 2,
                    "focus_xw": 0.05, "focus_yh": 0.05, "lambda0": 5, "dlambda": 1})


# --- M2 acceptance: restart survival -------------------------------------------

@pytest.mark.slow
def test_restart_survival_acceptance(tmp_path):
    """Kill the server mid-run; registry and job results survive the restart
    (fresh server process over real stdio, shared MCSTAS_MCP_HOME)."""
    import asyncio
    import json as _json
    import sys

    from fastmcp import Client
    from fastmcp.client.transports import StdioTransport

    server_bin = os.path.join(os.path.dirname(sys.executable), "mcstas-mcp")
    env = dict(os.environ, MCSTAS_MCP_HOME=str(tmp_path / "home"))

    def transport():
        return StdioTransport(server_bin, [], env=env)

    async def _call(c, tool, **kw):
        r = await c.call_tool(tool, kw)
        return _json.loads(r.content[0].text)

    async def session1():
        async with Client(transport()) as c:
            assert (await _call(c, "create_instrument", name="surv"))["ok"]
            assert (await _call(
                c, "add_component", instrument_id="surv", name="src",
                component="Source_simple", at=[0, 0, 0],
                parameters={"xwidth": 0.02, "yheight": 0.02, "dist": 2,
                            "focus_xw": 0.05, "focus_yh": 0.05,
                            "lambda0": 5, "dlambda": 1}))["ok"]
            assert (await _call(
                c, "add_component", instrument_id="surv", name="psd",
                component="PSD_monitor", at=[0, 0, 2], relative="src",
                parameters={"xwidth": 0.1, "yheight": 0.1,
                            "filename": "psd.dat"}))["ok"]
            job = await _call(c, "run_simulation", instrument_id="surv",
                              ncount=1e8, wait_s=0)
            assert job.get("state") in ("running", "done"), job
            return job["job_id"]
        # leaving the context kills the server; mcrun runs detached

    job_id = asyncio.run(session1())

    async def session2():
        async with Client(transport()) as c:
            listed = await _call(c, "list_instruments")
            assert "surv" in listed["instruments"], "registry lost on restart"
            for _ in range(180):
                st = await _call(c, "job_status", job_id=job_id)
                assert st["ok"], st
                if st["state"] != "running":
                    break
                await asyncio.sleep(1)
            assert st["state"] == "done", st
            res = await _call(c, "get_results", job_id=job_id)
            assert res["ok"] and res["monitors"][0]["intensity"] > 0

    asyncio.run(session2())
