"""Fixture tests for the level/kind classifier.

`taxonomy.classify` decides, by SUBSTRING MATCHING on grader messages,
which level every episode in the paper's tables lands at. A loose or
stale rule would misclassify systematically and silently — the exact
defect class this project has now hit three times (infra-as-capability,
a degenerate format/physics split, and a case-insensitive match in an
ad-hoc analysis script that invented a bug that did not exist).

So: fixtures for every level and kind, plus a COUPLING test asserting
that grader.py still emits the literal strings the classifier keys on.
If a grader message is reworded, that test fails loudly instead of the
taxonomy quietly reclassifying a slice of the record.
"""

import os
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))

import taxonomy  # noqa: E402


def report(hard=(), passed=False, episode=None, leak=False, checks="0/6"):
    return {"task": "T", "episode": episode or {"mcp_calls": {"x": 1}},
            "grade": {"pass": passed, "score": 1.0 if passed else 0.0,
                      "checks_passed": checks, "hard_failures": list(hard)},
            "reference_leak": {"leaked": leak}}


def test_every_level_classifies_from_real_grader_strings():
    cases = [
        # (report, expected level, expected kind)
        (report(passed=True), "PASS", "success"),
        (report(hard=["candidate failed to run (translate)"]), "L1",
         "physics"),
        (report(hard=["candidate failed to run (compile)"]), "L1", "physics"),
        (report(hard=["candidate failed to run (run)"]), "L2", "physics"),
        (report(hard=["role '2d': only 0 events — below statistics floor, "
                      "ungradable"]), "L3", "physics"),
        (report(hard=["no monitor with role 'energy' in candidate"]), "L3",
         "physics"),
        (report(), "L4", "physics"),  # no hard failures, checks missed
    ]
    for rep, level, kind in cases:
        got = taxonomy.classify(rep)
        assert (got["level"], got["kind"]) == (level, kind), (rep, got)


def test_l0_kinds_distinguish_format_from_incomplete():
    hard = ["agent left no built instrument in the episode registry"]
    never = taxonomy.classify(report(hard, episode={"mcp_calls": {}}))
    assert (never["level"], never["kind"]) == ("L0", "format")

    oneshot = taxonomy.classify(report(
        hard, episode={"scaffold": "plain-llm-oneshot", "mcp_calls": {}}))
    assert (oneshot["level"], oneshot["kind"]) == ("L0", "format")

    # used the tools, never finished — NOT a protocol-mechanics failure
    busy = taxonomy.classify(report(
        hard, episode={"mcp_calls": {"add_component": 50}}))
    assert (busy["level"], busy["kind"]) == ("L0", "incomplete")

    capped = taxonomy.classify(report(
        hard, episode={"mcp_calls": {"add_component": 9},
                       "hit_turn_cap": True}))
    assert (capped["level"], capped["kind"]) == ("L0", "incomplete")


def test_infra_and_leak_take_precedence_over_any_score():
    infra = taxonomy.classify(report(
        passed=True, episode={"error": "ConnectError: refused"}))
    assert infra["level"] == "INFRA" and infra["kind"] == "endpoint_unreachable"
    prov = taxonomy.classify(report(episode={"error": "HTTP 404: nope"}))
    assert prov["kind"] == "provider_error"
    rc = taxonomy.classify(report(episode={"returncode": 1}))
    assert rc["level"] == "INFRA" and rc["kind"] == "harness_error"
    leak = taxonomy.classify(report(passed=True, leak=True))
    assert leak["level"] == "LEAK"


def test_grader_still_emits_the_strings_the_classifier_keys_on():
    """Coupling guard: reword a grader message and this fails, instead of
    the taxonomy silently reclassifying part of the record."""
    with open(os.path.join(REPO, "benchmark", "harness", "grader.py")) as f:
        src = f.read()
    # note: the stage suffix "(translate)"/"(compile)"/"(run)" is
    # interpolated, so only the stem is contiguous in the source
    for literal in ("candidate failed to run", "below statistics floor",
                    "no monitor with role"):
        assert literal in src, f"grader.py no longer emits {literal!r}"
    # and the stage values the classifier splits L1 from L2 on
    assert 'job.get("stage")' in src or "stage" in src
    with open(os.path.join(REPO, "benchmark", "harness",
                           "run_episode.py")) as f:
        assert "no built instrument" in f.read()


def test_classification_is_case_sensitive():
    """Loose casing is how a phantom bug got reported (2026-09-10): an
    ad-hoc script upper-cased its haystack and 'matched' prose comments."""
    shouty = taxonomy.classify(report(hard=["CANDIDATE FAILED TO RUN "
                                            "(TRANSLATE)"]))
    assert shouty["level"] != "L1"  # exact-cased rules only
