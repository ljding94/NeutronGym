"""Evidence preservation must carry the infra-retry quarantine into git.

run_matrix --retry-infra moves a failed report into <set>/_infra/ before
re-running the cell, and the retry overwrites report.json. If preservation
skips _infra/, the committed record loses the fact that those held-out cells
ever failed on infrastructure — exactly the provenance the retry policy
depends on (an INFRA episode never reached the model, so re-running it did
not spend the once-only held-out axis).
"""

import json
import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))), "benchmark", "harness"))

import preserve_evidence as pe  # noqa: E402


def _setup(tmp_path, monkeypatch):
    runs = tmp_path / "runs" / "m6_final"
    dest = tmp_path / "evidence"
    monkeypatch.setattr(pe, "SETS", {"m6_final": str(runs)})
    monkeypatch.setattr(pe, "DEST", str(dest))
    return runs, dest


def test_infra_quarantine_is_preserved(tmp_path, monkeypatch):
    runs, dest = _setup(tmp_path, monkeypatch)
    cell = runs / "T1_BOYA_CARR__main__qwen3_8b"
    cell.mkdir(parents=True)
    (cell / "report.json").write_text(json.dumps({"grade": {"pass": True}}))
    (runs / "_infra").mkdir()
    old = {"episode": {"error": "ConnectError: refused"}}
    (runs / "_infra" / "T1_BOYA_CARR__main__qwen3_8b__infra1.json").write_text(
        json.dumps(old))
    (runs / "_infra" / "notes.txt").write_text("not evidence")

    pe.preserve()

    kept = dest / "m6_final" / "_infra" / "T1_BOYA_CARR__main__qwen3_8b__infra1.json"
    assert json.loads(kept.read_text()) == old
    assert not (dest / "m6_final" / "_infra" / "notes.txt").exists()
    # the retried cell's new report lands in its own directory as before
    assert json.loads((dest / "m6_final" / "T1_BOYA_CARR__main__qwen3_8b"
                       / "report.json").read_text()) == {"grade": {"pass": True}}


def test_infra_dir_is_not_mistaken_for_an_episode(tmp_path, monkeypatch):
    """_infra/ holds no report.json/transcript of its own; it must not spawn
    an empty evidence episode or copy anything under the episode schema."""
    runs, dest = _setup(tmp_path, monkeypatch)
    (runs / "_infra").mkdir(parents=True)
    (runs / "_infra" / "X__infra1.json").write_text("{}")
    n = pe.preserve()
    assert n == 1
    assert sorted(os.listdir(dest / "m6_final" / "_infra")) == ["X__infra1.json"]


def test_rerun_is_idempotent(tmp_path, monkeypatch):
    runs, dest = _setup(tmp_path, monkeypatch)
    (runs / "_infra").mkdir(parents=True)
    (runs / "_infra" / "X__infra1.json").write_text('{"a": 1}')
    pe.preserve()
    pe.preserve()
    assert json.loads((dest / "m6_final" / "_infra" / "X__infra1.json")
                      .read_text()) == {"a": 1}


def test_m8_records_are_preserved_by_allowlist(tmp_path, monkeypatch):
    m8 = tmp_path / "runs" / "m8"
    (m8 / "raft").mkdir(parents=True)
    (m8 / "train").mkdir()
    (m8 / "sweep.json").write_text('{"curve": []}')
    (m8 / "probe_guide_divergence_1x_n100.json").write_text("{}")
    (m8 / "raft" / "train.jsonl").write_text('{"best_level": 4}\n')
    (m8 / "raft" / "manifest.json").write_text("{}")
    (m8 / "raft" / "guide_divergence.jsonl").write_text("scratch\n")
    (m8 / "train" / "train_log.jsonl").write_text('{"step": 0}\n')
    (m8 / "rollouts").mkdir()
    (m8 / "rollouts" / "big.bin").write_text("x")
    monkeypatch.setattr(pe, "REPO", str(tmp_path))
    monkeypatch.setattr(pe, "DEST", str(tmp_path / "evidence"))

    n = pe.preserve_m8()

    got = sorted(str(p.relative_to(tmp_path / "evidence" / "m8"))
                 for p in (tmp_path / "evidence" / "m8").rglob("*") if p.is_file())
    assert got == ["probe_guide_divergence_1x_n100.json", "raft/manifest.json",
                   "raft/train.jsonl", "sweep.json", "train/train_log.jsonl"]
    assert n == 5
