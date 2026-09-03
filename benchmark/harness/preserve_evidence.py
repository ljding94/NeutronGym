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
SETS = {"m6": "runs/m6", "pilot": "runs/pilot"}
DEST = os.path.join(REPO, "benchmark", "evidence")

KEEP_TOP = {"report.json", "transcript.jsonl", "report_regraded.json",
            "ledger.json", "summary.json", "acceptance.json"}
ARTIFACT_EXT = (".instr", ".json", ".png")


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


if __name__ == "__main__":
    n = preserve()
    print(f"{n} evidence files -> {os.path.relpath(DEST, REPO)}/ "
          "(commit them; runs/ stays scratch)")
