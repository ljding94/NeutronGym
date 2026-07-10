"""Regression tests for the adversarial-review findings (C1-C2, M1-M6, minors).

Each test names the finding it pins. See the review integrated into
note/m1-server-design-2026-07-09.md.
"""

import os
import subprocess
import threading

import pytest

from mcstas_mcp import execution, registry
from mcstas_mcp.config import home_dir
from mcstas_mcp.execution import RunError
from mcstas_mcp.registry import SpecError
from tests.test_server import call


def _src(spec, **extra):
    registry.add_component(
        spec, "src", "Source_simple", at=[0, 0, 0],
        parameters={"xwidth": 0.02, "yheight": 0.02, "dist": 2,
                    "focus_xw": 0.05, "focus_yh": 0.05, "lambda0": 5, "dlambda": 1,
                    **extra},
    )
    return spec


# --- C1: silent wrong physics -------------------------------------------------

def test_c1_comma_expression_rejected(spec):
    """'(1, 2)' compiles via C's comma operator to 2 — must never build."""
    with pytest.raises(SpecError, match="not allowed in a scalar"):
        _src(spec, lambda0="(1, 2)")


def test_c1_list_and_dict_values_rejected(spec):
    with pytest.raises(SpecError, match="numbers or strings"):
        _src(spec, lambda0=[1, 2, 3])
    with pytest.raises(SpecError, match="numbers or strings"):
        registry.add_component(spec, "s2", "Source_simple", at=[0, 0, 0],
                               parameters={"lambda0": {"a": 1}})


def test_c1_array_indexing_rejected(spec):
    registry.add_parameter(spec, "wl", default=5.0)
    with pytest.raises(SpecError, match="not allowed in a scalar"):
        _src(spec, lambda0="wl[2]")


# --- C2: stdin isolation -------------------------------------------------------

def test_c2_mcrun_never_inherits_server_stdin(spec, monkeypatch):
    """The server's stdin is the MCP transport; children must get DEVNULL."""
    captured = {}

    def fake_popen(*args, **kwargs):
        captured.update(kwargs)
        raise RuntimeError("probe stop")

    monkeypatch.setattr(subprocess, "Popen", fake_popen)
    with pytest.raises(RuntimeError, match="probe stop"):
        execution.run_instr_file("/dev/null", {}, ncount=1e3)
    assert captured["stdin"] == subprocess.DEVNULL
    assert captured["start_new_session"] is True  # M4: killable process group


# --- M1: concurrent mutations --------------------------------------------------

def test_m1_concurrent_adds_all_persist(spec):
    _src(spec)
    errors = []

    def add(offset):
        for i in range(10):
            out = call("add_component", instrument_id="test_instr",
                       name=f"arm_{offset}_{i}", component="Arm",
                       at=[0, 0, 1 + offset + i / 100], relative="src")
            if not out.get("ok"):
                errors.append(out)

    threads = [threading.Thread(target=add, args=(k,)) for k in (1, 2)]
    for t in threads:
        t.start()
    for t in threads:
        t.join()
    assert not errors
    assert len(registry.load("test_instr")["components"]) == 21  # src + 20 arms


# --- M2: bool coercion ----------------------------------------------------------

def test_m2_bool_coerced_to_int(spec):
    registry.add_component(
        spec, "psd", "PSD_monitor", at=[0, 0, 1],
        parameters={"xwidth": 0.1, "yheight": 0.1, "filename": "p.dat",
                    "restore_neutron": True},
    )
    assert registry.load("test_instr")["components"][0]["parameters"][
        "restore_neutron"] == 1


# --- M3: expression tokenizer ----------------------------------------------------

def test_m3_scientific_notation_accepted(spec):
    registry.add_parameter(spec, "wl", default=5.0)
    _src(spec, dlambda="1e-3*wl")  # must not tokenize 'e' as identifier
    registry.set_parameters(spec, "src", {"dlambda": "2.5E+1*sin(wl)"})
    registry.set_parameters(spec, "src", {"dlambda": "0x1F"})
    with pytest.raises(SpecError, match="unknown identifier"):
        registry.set_parameters(spec, "src", {"dlambda": "undeclared*2"})


# --- M4: timeout kills the whole process tree ------------------------------------

@pytest.mark.slow
def test_m4_timeout_leaves_no_orphan(spec):
    _src(spec)
    job = execution.run_spec(registry.load("test_instr"), ncount=1e8, timeout=3)
    assert not job["ok"]
    assert any("timed out" in d for d in job["diagnostics"])
    survivors = subprocess.run(["pgrep", "-f", "test_instr.out"],
                               capture_output=True)
    assert survivors.returncode != 0, "simulation binary survived the timeout"


# --- M5: parametrized geometry ----------------------------------------------------

def test_m5_parametrized_at_via_mcp(spec):
    registry.add_parameter(spec, "L1", default=2.0)
    out = call("add_component", instrument_id="test_instr", name="arm",
               component="Arm", at=[0, 0, "L1"])
    assert out["ok"], out
    src = call("get_instrument", instrument_id="test_instr")["instr_source"]
    import re

    assert re.search(r"AT \([\d. ,]*L1\)", src), "parametrized AT not in .instr"


def test_m5_at_expression_validated(spec):
    with pytest.raises(SpecError, match="unknown identifier"):
        registry.add_component(spec, "arm", "Arm", at=[0, 0, "ghost_len"])


# --- M6: parameter name validation -------------------------------------------------

def test_m6_bad_parameter_names_rejected(spec):
    for bad in ("2bad", "a b", "x-y"):
        with pytest.raises(SpecError, match="not a valid parameter name"):
            registry.add_parameter(spec, bad)
    _src(spec)
    with pytest.raises(SpecError, match="already a component name"):
        registry.add_parameter(spec, "src")


# --- minors -------------------------------------------------------------------------

def test_ncount_below_one_rejected():
    for bad in (0.5, 0, -5):
        with pytest.raises(RunError, match="ncount"):
            execution.run_instr_file("/dev/null", {}, ncount=bad)


def test_load_ghost_leaves_no_directory():
    with pytest.raises(SpecError):
        registry.load("ghost_instrument")
    assert not os.path.isdir(
        os.path.join(home_dir(), "instruments", "ghost_instrument"))


def test_numeric_on_string_param_rejected(spec):
    with pytest.raises(SpecError, match="string parameter"):
        registry.add_component(spec, "psd", "PSD_monitor", at=[0, 0, 1],
                               parameters={"filename": 5})


def test_rotated_relative_without_rotated_rejected(spec):
    _src(spec)
    with pytest.raises(SpecError, match="without 'rotated'"):
        registry.add_component(spec, "arm", "Arm", at=[0, 0, 1],
                               rotated_relative="src")


def test_monitor_data_format_validated():
    out = call("get_monitor_data", job_id="nope", monitor="m", format="imag")
    assert not out["ok"] and "png" in out["error"] and "array" in out["error"]


def test_keyerror_message_not_double_quoted():
    from mcstas_mcp.server import _err

    msg = _err(KeyError("no monitor 'x'"))["error"]
    assert not msg.startswith('"')
