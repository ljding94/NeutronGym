"""M6 matrix-runner regressions: enumeration discipline (held-out never
reachable, reserves excluded, slices honored), cost from actual usage."""

import json
import os
import shutil
import sys
from pathlib import Path

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))

import run_matrix  # noqa: E402


CFG = run_matrix.load_config()


def test_heldout_tasks_structurally_unreachable():
    eps = run_matrix.enumerate_episodes(CFG)
    tasks = {t for _, _, _, t, _ in eps}
    assert "T1_BOYA_CARR" not in tasks and "T1_VENUS_SNS" not in tasks
    assert len(eps) > 100  # the matrix is real


def test_reserve_model_never_enters_matrix():
    eps = run_matrix.enumerate_episodes(CFG)
    assert all(m != "anthropic/claude-opus-5" for _, _, m, _, _ in eps)


def test_gpt_slice_honored():
    eps = run_matrix.enumerate_episodes(CFG)
    gpt_tasks = {t for _, _, m, t, _ in eps if m == "openai/gpt-5.2-pro"}
    assert gpt_tasks <= set(CFG["gpt_slice"]) | set(CFG["dev_split"])
    assert len(gpt_tasks) <= 8


def test_priority_and_model_filters():
    p2 = run_matrix.enumerate_episodes(CFG, only_priority=2)
    assert p2 and all(p == 2 for p, *_ in p2)
    assert any(m is None for _, _, m, _, _ in p2)  # subscription arm
    one = run_matrix.enumerate_episodes(
        CFG, only_model="meta-llama/llama-4-maverick")
    assert one and all(m == "meta-llama/llama-4-maverick"
                       for _, _, m, _, _ in one)


def test_episode_cost_from_actual_usage(tmp_path):
    rp = tmp_path / "report.json"
    rp.write_text(json.dumps({"episode": {"usage": {
        "prompt_tokens": 1_000_000, "completion_tokens": 100_000}}}))
    cost = run_matrix.episode_cost(str(rp), CFG, "anthropic/claude-sonnet-5")
    assert abs(cost - (2.0 + 1.0)) < 1e-9  # 1M*$2/M + 0.1M*$10/M
    assert run_matrix.episode_cost(str(rp), CFG, None) == 0.0


# --- --retry-infra (2026-09-13) -------------------------------------------
# The 10 held-out cells lost to a dead SSH tunnel and a withdrawn provider
# route left a report.json behind, so the runner's "done" check skipped them
# forever. Retrying them is only legitimate because an INFRA episode never
# reached the model, and therefore never spent the once-only held-out
# exposure. These guard that reasoning: retry exactly that class, never a
# graded one, and never destroy the evidence of the failure.

def _ep(tmp_path, name, report):
    d = tmp_path / name
    d.mkdir(parents=True, exist_ok=True)
    (d / "report.json").write_text(json.dumps(report))
    return str(d)


def test_infra_casualty_detects_unreachable_endpoint(tmp_path):
    d = _ep(tmp_path, "unreachable", {"episode": {
        "error": "chat completion failed after 3 tries: ConnectError: "
                 "[Errno 61] Connection refused"}})
    got = run_matrix.infra_casualty(d)
    assert got and got["level"] == "INFRA"
    assert got["kind"] == "endpoint_unreachable"


def test_infra_casualty_detects_withdrawn_provider_route(tmp_path):
    d = _ep(tmp_path, "http404", {"episode": {
        "error": 'chat completion failed after 3 tries: HTTP 404: '
                 '{"error":{"message":"No endpoints found"}}'}})
    got = run_matrix.infra_casualty(d)
    assert got and got["kind"] == "provider_error"


def test_graded_episode_is_never_an_infra_casualty(tmp_path):
    """A real capability result — pass or fail — must never be re-run: that
    WOULD spend the held-out axis twice and is the failure this guards."""
    for grade in ({"pass": True}, {"pass": False,
                                   "hard_failures": ["no built instrument"]}):
        d = _ep(tmp_path, f"graded{grade['pass']}",
                {"episode": {"mcp_calls": 12}, "grade": grade})
        assert run_matrix.infra_casualty(d) is None


def test_missing_report_is_not_a_casualty(tmp_path):
    d = tmp_path / "never_ran"
    d.mkdir()
    assert run_matrix.infra_casualty(str(d)) is None


def test_quarantine_preserves_the_infra_report_outside_the_episode_dir(
        tmp_path):
    """run_episode.py rmtree's its --episode-dir before running, so a
    quarantined report parked INSIDE it would be destroyed by the retry it
    documents. It must land in the sibling _infra/ directory."""
    d = _ep(tmp_path, "q", {"episode": {"error": "ConnectError: refused"}})
    dest = run_matrix.quarantine_infra_report(d)
    assert not os.path.isfile(os.path.join(d, "report.json"))
    assert os.path.dirname(dest) == str(tmp_path / "_infra")
    assert not dest.startswith(d + os.sep)
    assert "ConnectError" in json.load(open(dest))["episode"]["error"]
    # surviving a simulated retry that wipes the episode directory
    shutil.rmtree(d)
    assert os.path.isfile(dest)
    # a second failure must not clobber the first one's evidence
    os.makedirs(d, exist_ok=True)
    (Path(d) / "report.json").write_text(
        json.dumps({"episode": {"error": "ConnectError: again"}}))
    dest2 = run_matrix.quarantine_infra_report(d)
    assert dest2 != dest and os.path.isfile(dest)
