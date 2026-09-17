"""Per-turn feedback must describe the numbers correctly (2026-09-16).

fom_ratio is fom / TARGET, but was shown as "FOM ratio vs baseline", so a
design at 0.93 of a 10x-baseline target read as being below the baseline.
And the trained SANS model resubmitted one design for 8 turns with nothing
telling it so.
"""

from neutrongym import rollouts


def _obs(**fb):
    base = {"level": 3, "failed_at": "L4", "detail": None, "fom": 9.3,
            "fom_ratio": 0.93, "repeat_of": None}
    base.update(fb)
    return {"feedback": base, "baseline_fom": 1.0}


def test_fom_is_reported_against_both_baseline_and_target():
    msg = rollouts.feedback_message(_obs())
    assert "9.3x the baseline" in msg
    assert "0.93 of the target" in msg and "pass needs > 1" in msg
    assert "ratio vs baseline" not in msg


def test_identical_resubmission_is_called_out():
    msg = rollouts.feedback_message(_obs(repeat_of=2))
    assert "identical to your turn 2" in msg


def test_no_repeat_note_on_a_new_action():
    assert "identical" not in rollouts.feedback_message(_obs())


def test_system_prompt_no_longer_misdescribes_the_ratio_or_leaks_guide_names():
    s = rollouts.DIALOGUE_SYSTEM
    assert "ratio vs baseline" not in s
    assert "w_in" not in s and "m_coat" not in s
    assert "target" in s
