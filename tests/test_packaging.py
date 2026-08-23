"""M5.5 packaging + contamination-machinery regressions."""

import json
import os
import subprocess
import sys

import pytest

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))

import probe_memorization as probe  # noqa: E402


def test_skill_ships_in_package():
    import neutrongym
    text = neutrongym.skill_text()
    assert "mcstas-instrument-design" in text and len(text) > 2000
    # canonical location is inside the package; top-level path is a symlink
    assert os.path.islink(os.path.join(REPO, "skills",
                                       "mcstas-instrument-design"))
    with open(os.path.join(REPO, "skills", "mcstas-instrument-design",
                           "SKILL.md")) as f:
        assert f.read() == text  # symlink chain resolves to the same file


def test_probe_scores_empty_and_none_answers_as_unseen():
    ref = "COMPONENT a = Source_simple()\nCOMPONENT b = PSD_monitor()\n"
    s = probe.score("", ref)
    assert s["similarity"] == 0.0 and s["keyfact_recall"] == 0.0


def test_heldout_tasks_committed_and_self_validated():
    for tid in ("T1_BOYA_CARR", "T1_VENUS_SNS"):
        p = os.path.join(REPO, "benchmark", "tasks", "T1", tid + ".json")
        task = json.load(open(p))
        assert task["split"] == "heldout"
        assert "probe_hint" in task and "NOT a facility-validated" \
            in task["provenance"]
        ref = os.path.join(REPO, task["reference"]["instr"])
        assert os.path.isfile(ref)
        assert task["reference"]["parameters"], \
            "explicit run params required (mcreadparams gotcha)"
        roles = [m["role"] for m in task["grading"]["monitors"]]
        assert "2d" in roles and "wavelength" in roles


def test_perturbed_variants_carry_the_pair_proof():
    d = os.path.join(REPO, "benchmark", "tasks", "T1_perturbed")
    if not os.path.isdir(d):
        pytest.skip("no perturbed variants generated yet")
    tasks = [f for f in os.listdir(d)
             if f.endswith(".json") and not f.startswith("_")]
    for f in tasks:
        t = json.load(open(os.path.join(d, f)))
        assert t["split"] == "perturbed"
        assert t["perturbation"]["scale"] > 1
        assert t["validation"]["canonical_fails"], \
            "a variant the canonical passes cannot detect memorization"
        assert os.path.isfile(os.path.join(REPO, t["reference"]["instr"]))


@pytest.mark.slow
def test_neutrongym_eval_cli_smoke():
    out = subprocess.run(
        [sys.executable, "-m", "neutrongym.cli", "--instances", "4"],
        capture_output=True, text=True, timeout=300,
        env={**os.environ, "MCSTAS_MCP_HOME":
             os.path.join(REPO, "runs", "cli_smoke_home")})
    assert out.returncode == 0, out.stdout + out.stderr
    assert "[PASS]" in out.stdout
