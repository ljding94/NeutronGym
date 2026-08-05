"""Episode-output-contract + visualization regressions (visualize.py,
run_episode task-id entry, tasks_report thumbnail embed)."""

import json
import os
import sys

import pytest

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))
sys.path.insert(0, os.path.join(REPO, "scripts"))

import run_episode  # noqa: E402
import tasks_report  # noqa: E402
import visualize  # noqa: E402


# --- task-id entry point ---------------------------------------------------------

def test_resolve_task_by_id_and_path():
    p = run_episode.resolve_task("T1_PSI_DMC")
    assert p.endswith(os.path.join("T1", "T1_PSI_DMC.json"))
    direct = os.path.join(REPO, "benchmark", "tasks", "P1_sans_reproduce.json")
    assert run_episode.resolve_task(direct) == direct


def test_resolve_task_unknown_lists_known_ids():
    with pytest.raises(SystemExit) as e:
        run_episode.resolve_task("T1_NOPE")
    assert "T1_PSI_DMC" in str(e.value)  # error names the valid ids


# --- bundle ----------------------------------------------------------------------

def test_bundle_writes_contract_and_survives_visual_failures(tmp_path,
                                                             monkeypatch):
    instr = tmp_path / "cand.instr"
    instr.write_text("DEFINE INSTRUMENT cand()\nTRACE\nEND\n")
    monkeypatch.setattr(visualize, "diagram_png",
                        lambda i, o: open(o, "wb").write(b"png") and o)
    monkeypatch.setattr(visualize, "webgl_trace", lambda i, p, o, rays=30: None)
    out = tmp_path / "artifacts"
    m = visualize.bundle(str(instr), {"lam": 6}, str(out))
    assert os.path.isfile(m["instr"]) and m["instr"].endswith("cand.instr")
    with open(m["params"]) as f:
        assert json.load(f) == {"lam": 6}
    assert m["diagram"] and os.path.isfile(m["diagram"])
    assert m["trace"] is None
    assert any("trace" in e for e in m["errors"])  # failure reported, not raised


def test_bundle_missing_instr_reports_not_raises(tmp_path):
    m = visualize.bundle(str(tmp_path / "ghost.instr"), {}, str(tmp_path / "a"))
    assert m["instr"] is None and m["errors"]


def test_iter_ref_targets_covers_tasks_with_references():
    targets = {tid: (instr, params)
               for tid, instr, params in visualize.iter_ref_targets()}
    assert "T1_PSI_DMC" in targets and "T2_guide_divergence" in targets
    instr, params = targets["T2_guide_divergence"]
    assert instr.endswith("t2_guide_divergence.instr")  # T2 ref IS the baseline
    assert params  # reference parameters travel with the visuals
    assert "P2_memorization_psi_dmc" not in targets  # probe: no reference field


# --- tasks_report thumbnail embed ------------------------------------------------

def test_refviz_thumbnail_embeds_when_present(tmp_path):
    d = tmp_path / "T1_X"
    d.mkdir()
    (d / "diagram.png").write_bytes(b"\x89PNG_fake")
    html = tasks_report.refviz_img("T1_X", refviz_root=str(tmp_path))
    assert "data:image/png;base64," in html and "T1_X" in html


def test_refviz_degrades_gracefully_when_absent(tmp_path):
    assert tasks_report.refviz_img("T1_MISSING", refviz_root=str(tmp_path)) == ""


# --- real renders (slow: McStasScript reader + full compile) ---------------------

@pytest.mark.slow
def test_diagram_png_real(tmp_path):
    from mcstas_mcp.config import resources_dir
    sans = os.path.join(resources_dir(), "examples", "Templates",
                        "templateSANS", "templateSANS.instr")
    png = visualize.diagram_png(sans, str(tmp_path / "d.png"))
    assert png and os.path.getsize(png) > 5000


@pytest.mark.slow
def test_webgl_trace_real(tmp_path):
    from mcstas_mcp.config import resources_dir
    sans = os.path.join(resources_dir(), "examples", "Templates",
                        "templateSANS", "templateSANS.instr")
    out = visualize.webgl_trace(sans, {"lambda": 6}, str(tmp_path / "trace"),
                                rays=10)
    assert out and os.path.isfile(os.path.join(out, "index.html"))
