"""M3 skill checks: structure constraints + physics of resolution_calcs."""

import math
import os
import re
import subprocess
import sys

import pytest

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SKILL = os.path.join(REPO, "skills", "mcstas-instrument-design")
sys.path.insert(0, os.path.join(SKILL, "scripts"))

import resolution_calcs as rc  # noqa: E402


def test_skill_structure():
    text = open(os.path.join(SKILL, "SKILL.md")).read()
    assert len(text.splitlines()) < 200, "SKILL.md must stay under 200 lines"
    front = re.match(r"---\n(.*?)\n---\n", text, re.DOTALL)
    assert front and "name: mcstas-instrument-design" in front.group(1)
    assert "description:" in front.group(1)
    for ref in ("units-and-conventions", "figures-of-merit",
                "instrument-archetypes", "component-guide",
                "verification-checklist"):
        path = os.path.join(SKILL, "references", f"{ref}.md")
        assert os.path.isfile(path), f"missing reference {ref}"
        assert ref in text, f"SKILL.md must point to {ref}"


def test_skill_encodes_observed_failures():
    """Rules born from real transcripts must stay in the skill."""
    text = open(os.path.join(SKILL, "SKILL.md")).read().lower()
    assert "seed" in text          # M1 acceptance run never fixed a seed
    assert "1000 events" in text   # statistics floor
    assert "restore_neutron" in text
    assert "assumption" in text    # disclose source-brightness assumptions


def test_conversions():
    assert rc.lambda_to_energy(1.8) == pytest.approx(25.25, rel=1e-3)
    assert rc.lambda_to_velocity(3.956034) == pytest.approx(1000.0, rel=1e-4)
    assert rc.energy_to_lambda(rc.lambda_to_energy(5.0)) == pytest.approx(5.0)
    assert rc.lambda_to_energy(6.271) == pytest.approx(2.08, rel=2e-3)  # Si(111) BS


def test_bragg():
    # PG(002), lambda=2.37 AA -> theta ~ 20.7 deg
    assert rc.bragg_angle(3.355, 2.37) == pytest.approx(20.69, abs=0.05)
    with pytest.raises(ValueError):
        rc.bragg_angle(3.355, 8.0)  # unreachable


def test_chopper_and_frame():
    # 5 AA over 25 m: v = 791.2 m/s, t = 31.6 ms; at 100 Hz -> 3.16 periods
    t = 25 / rc.lambda_to_velocity(5)
    assert rc.chopper_phase(25, 5, 100) == pytest.approx((360 * 100 * t) % 360, rel=1e-6)
    assert rc.chopper_opening_time(10, 100) == pytest.approx(10 / 36000)
    # frame overlap: 25 m at 14 Hz allows lambda up to ~11.3 AA
    assert rc.frame_max_lambda(25, 14) == pytest.approx(3956.034 / 350, rel=1e-6)


def test_guide_and_sans():
    assert rc.guide_critical_angle(2, 5) == pytest.approx(0.99)
    assert rc.guide_m_for_divergence(1.98, 5) == pytest.approx(2.0)
    # small-angle limit: Q ~ 2*pi*r/(lam*L)
    qmin, qmax = rc.sans_q_range(6, 5, 0.32, 0.02)
    assert qmin == pytest.approx(2 * math.pi * 0.02 / (6 * 5), rel=1e-3)
    assert qmax == pytest.approx(2 * math.pi * 0.32 / (6 * 5), rel=5e-3)


def test_quadrature_and_tof():
    assert rc.quadrature(3, 4) == pytest.approx(5)
    assert rc.tof_dE_over_E(0.01) == pytest.approx(0.02)


def test_cli_runs():
    out = subprocess.run(
        [sys.executable, os.path.join(SKILL, "scripts", "resolution_calcs.py"),
         "chopper", "L=25", "lam=5", "nu=100", "theta0=10"],
        capture_output=True, text=True)
    assert out.returncode == 0 and "phase =" in out.stdout
