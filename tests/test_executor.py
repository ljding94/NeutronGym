"""FamilyExecutor regressions: compile-once cache, direct-binary rollouts,
failure surfaces. Slow tests compile a real (minimal) McStas instrument."""

import os

import pytest

from neutrongym import executor as ex

PROBE = """\
DEFINE INSTRUMENT exec_probe(gws=0.03, wl=5.0)
TRACE
COMPONENT origin = Progress_bar() AT (0,0,0) ABSOLUTE
COMPONENT src = Source_simple(xwidth=0.1, yheight=0.1, dist=1.5,
  focus_xw=gws, focus_yh=gws, lambda0=wl, dlambda=0.5*wl)
  AT (0,0,0) RELATIVE origin
COMPONENT mon = PSD_monitor(nx=20, ny=20, xwidth=0.1, yheight=0.1,
  filename="psd.dat", restore_neutron=1)
  AT (0,0,1.5) RELATIVE src
END
"""


# --- fast: pure logic ------------------------------------------------------------

def test_read_define_params_typed_and_defaulted(tmp_path):
    p = tmp_path / "x.instr"
    p.write_text("DEFINE INSTRUMENT x(double a=1.5, int n=3,\n"
                 "  string fn=\"f.dat\", bare)\nTRACE\nEND\n")
    params = ex.read_define_params(str(p))
    assert params == {"a": "1.5", "n": "3", "fn": '"f.dat"', "bare": None}


def test_run_guards_before_touching_mcstas(tmp_path):
    p = tmp_path / "y.instr"
    p.write_text(PROBE)
    fe = ex.FamilyExecutor(str(p))
    assert ex.FamilyExecutor(str(p)).run({}, 1e4, seed=0)["stage"] == "setup"
    assert fe.run({}, 0.5, seed=1)["stage"] == "setup"
    out = fe.run({}, 1e4, seed=1)  # no binary yet
    assert not out["ok"] and "compile" in " ".join(out["diagnostics"])


def test_missing_instr_raises():
    with pytest.raises(FileNotFoundError):
        ex.FamilyExecutor("/nope/ghost.instr")


# --- slow: real compile + rollouts -----------------------------------------------

@pytest.fixture(scope="module")
def family(tmp_path_factory):
    d = tmp_path_factory.mktemp("family")
    instr = d / "exec_probe.instr"
    instr.write_text(PROBE)
    fe = ex.FamilyExecutor(str(instr))
    res = fe.compile()
    assert res["ok"], res
    return fe, res


@pytest.mark.slow
def test_compile_then_cache_hit(family, tmp_path):
    fe, first = family
    assert first["cached"] is False and os.path.isfile(fe.binary)
    again = ex.FamilyExecutor(fe.instr).compile()
    assert again["ok"] and again["cached"] is True


@pytest.mark.slow
def test_rollout_summary_and_cleanup(family):
    fe, _ = family
    out = fe.run({"gws": 0.04, "wl": 6.0}, ncount=1e4, seed=42)
    assert out["ok"], out
    s = out["summary"]
    assert s["monitors"] and s["monitors"][0]["events"] > 0
    assert s["run_parameters"]["wl"] == "6"
    assert out["elapsed_s"] < 5
    # rollout dirs removed after parsing (RL would fill the disk otherwise)
    leftovers = [x for x in os.listdir(fe.workdir) if x.startswith("r")]
    assert leftovers == []


@pytest.mark.slow
def test_keep_output_keeps_files(family):
    fe, _ = family
    out = fe.run({"gws": 0.03, "wl": 5.0}, ncount=1e3, seed=7,
                 keep_output=True)
    assert out["ok"]
    assert os.path.isfile(os.path.join(out["summary"]["output_dir"],
                                       "mccode.sim"))


@pytest.mark.slow
def test_content_change_busts_cache(family):
    fe, _ = family
    with open(fe.instr) as f:
        text = f.read()
    try:
        with open(fe.instr, "w") as f:
            f.write(text.replace("dist=1.5", "dist=1.6"))
        res = ex.FamilyExecutor(fe.instr).compile()
        assert res["ok"] and res["cached"] is False
    finally:
        with open(fe.instr, "w") as f:
            f.write(text)
        ex.FamilyExecutor(fe.instr).compile()  # restore cache for other tests


@pytest.mark.slow
def test_bad_parameter_fails_at_run_stage(family):
    fe, _ = family
    out = fe.run({"nonexistent_param": 1}, ncount=1e3, seed=5)
    assert not out["ok"] and out["stage"] == "run"
