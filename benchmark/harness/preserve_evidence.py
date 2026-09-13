"""Preserve the citable evidence from episode runs into git (M6 decision
2026-09-02, resolving the 07-30 parked evidence-preservation question).

Copies, per episode, exactly the files the paper's claims cite — the
graded report, the full transcript, and the agent-built artifact
(.instr/params/diagram) — from gitignored runs/ into committed
benchmark/evidence/<set>/<episode>/. Regenerable bulk (compiled binaries,
simulation outputs, webgl trace apps) stays out: ~22 MB in, ~765 MB out.
Idempotent: re-run after new episodes; identical files produce no diff.

Usage: python3 benchmark/harness/preserve_evidence.py
"""

import os
import shutil

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SETS = {"m6": "runs/m6", "pilot": "runs/pilot",
        "m6_final": "runs/m6_final"}  # the once-only held-out pass
DEST = os.path.join(REPO, "benchmark", "evidence")

KEEP_TOP = {"report.json", "transcript.jsonl", "report_regraded.json",
            "ledger.json", "summary.json", "acceptance.json"}
ARTIFACT_EXT = (".instr", ".json", ".png")
INFRA_DIR = "_infra"  # run_matrix.quarantine_infra_report's destination

# M8 records are flat files, not per-episode directories, so they get an
# explicit allowlist: every number the trainability section cites (gate,
# sweep, paired and family probes, eval) plus the exact SFT data and its
# manifest. Rollout scratch and checkpoints stay out.
M8_SRC = "runs/m8"
M8_FILES = ("*.json", "sweep.log", "raft/manifest.json", "raft/train.jsonl",
            "train/*.jsonl", "train/*.json", "train/*.log")


def preserve():
    copied = 0
    for name, rel in SETS.items():
        src_root = os.path.join(REPO, rel)
        if not os.path.isdir(src_root):
            continue
        for entry in sorted(os.listdir(src_root)):
            src = os.path.join(src_root, entry)
            if os.path.isfile(src) and entry in KEEP_TOP:
                dst = os.path.join(DEST, name, entry)
                os.makedirs(os.path.dirname(dst), exist_ok=True)
                shutil.copy2(src, dst)
                copied += 1
                continue
            if not os.path.isdir(src):
                continue
            if entry == INFRA_DIR:
                # reports displaced by run_matrix --retry-infra: the only
                # surviving record that those cells once failed on infra
                for fn in sorted(os.listdir(src)):
                    p = os.path.join(src, fn)
                    if os.path.isfile(p) and fn.endswith(".json"):
                        dst = os.path.join(DEST, name, INFRA_DIR, fn)
                        os.makedirs(os.path.dirname(dst), exist_ok=True)
                        shutil.copy2(p, dst)
                        copied += 1
                continue
            for fn in KEEP_TOP:
                p = os.path.join(src, fn)
                if os.path.isfile(p):
                    dst = os.path.join(DEST, name, entry, fn)
                    os.makedirs(os.path.dirname(dst), exist_ok=True)
                    shutil.copy2(p, dst)
                    copied += 1
            art = os.path.join(src, "artifacts")
            if os.path.isdir(art):
                for fn in sorted(os.listdir(art)):
                    p = os.path.join(art, fn)
                    if os.path.isfile(p) and fn.endswith(ARTIFACT_EXT):
                        dst = os.path.join(DEST, name, entry, "artifacts", fn)
                        os.makedirs(os.path.dirname(dst), exist_ok=True)
                        shutil.copy2(p, dst)
                        copied += 1
    return copied


def preserve_m8():
    import glob
    src_root = os.path.join(REPO, M8_SRC)
    copied = 0
    if not os.path.isdir(src_root):
        return 0
    for pattern in M8_FILES:
        for p in sorted(glob.glob(os.path.join(src_root, pattern))):
            if not os.path.isfile(p):
                continue
            rel = os.path.relpath(p, src_root)
            dst = os.path.join(DEST, "m8", rel)
            os.makedirs(os.path.dirname(dst), exist_ok=True)
            shutil.copy2(p, dst)
            copied += 1
    return copied


if __name__ == "__main__":
    n = preserve() + preserve_m8()
    print(f"{n} evidence files -> {os.path.relpath(DEST, REPO)}/ "
          "(commit them; runs/ stays scratch)")
