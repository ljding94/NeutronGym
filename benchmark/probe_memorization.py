"""Memorization probe (contamination control, SPEC §7 Study A).

Asks a model — WITHOUT tools — to emit a shipped .instr file from memory,
then scores the answer against the real file:

  similarity : difflib ratio over normalized text (whitespace/comments folded)
  keyfacts   : recall of the component-type sequence (the structural skeleton)

A (paper, instrument) pair counts as 'unseen' for a model only if that model
FAILS the probe (both scores below the task thresholds).

Usage:
  python benchmark/probe_memorization.py benchmark/tasks/P2_memorization_psi_dmc.json \
      --model google/gemini-3.5-flash-lite            # via OpenRouter
  python benchmark/probe_memorization.py <task> --answer-file reply.txt   # offline scoring
"""

import argparse
import difflib
import json
import os
import re
import sys
import urllib.request

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def normalize(text: str) -> str:
    out = []
    for ln in text.splitlines():
        ln = re.sub(r"//.*$", "", ln).strip()
        if ln and not ln.startswith("*") and not ln.startswith("/*"):
            out.append(re.sub(r"\s+", " ", ln))
    return "\n".join(out)


def component_sequence(text: str) -> list:
    return re.findall(r"COMPONENT\s+\w+\s*=\s*(\w+)", text)


def score(answer: str, reference: str) -> dict:
    sim = difflib.SequenceMatcher(None, normalize(answer),
                                  normalize(reference)).ratio()
    ref_seq = component_sequence(reference)
    ans_seq = component_sequence(answer)
    matcher = difflib.SequenceMatcher(None, ans_seq, ref_seq)
    keyfacts = (sum(b.size for b in matcher.get_matching_blocks())
                / len(ref_seq)) if ref_seq else 0.0
    return {"similarity": round(sim, 3), "keyfact_recall": round(keyfacts, 3),
            "reference_components": len(ref_seq),
            "answer_components": len(ans_seq)}


def ask_openrouter(model: str, prompt: str, max_tokens: int = 16000) -> str:
    req = urllib.request.Request(
        "https://openrouter.ai/api/v1/chat/completions",
        data=json.dumps({
            "model": model, "max_tokens": max_tokens,
            "messages": [{"role": "user", "content": prompt}],
        }).encode(),
        headers={"Authorization": f"Bearer {os.environ['OPENROUTER_API_KEY']}",
                 "Content-Type": "application/json"})
    with urllib.request.urlopen(req, timeout=300) as resp:
        data = json.load(resp)
    return data["choices"][0]["message"]["content"]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("task")
    ap.add_argument("--model", help="OpenRouter model id")
    ap.add_argument("--answer-file", help="score a saved answer instead of querying")
    args = ap.parse_args()
    with open(args.task) as f:
        task = json.load(f)
    probe = task["probe"]

    ref_name = probe["reference"].split(":", 1)[1]
    from mcstas_mcp import examples
    reference = examples.get_example(ref_name)["source"]

    if args.answer_file:
        answer = open(args.answer_file, errors="replace").read()
        source = args.answer_file
    else:
        if not args.model:
            sys.exit("need --model or --answer-file")
        answer = ask_openrouter(args.model, probe["prompt"])
        source = args.model

    s = score(answer, reference)
    th = probe["thresholds"]
    s["memorized"] = (s["similarity"] >= th["similarity_memorized"]
                      or s["keyfact_recall"] >= th["keyfact_memorized"])
    s["verdict"] = ("CONTAMINATED (treat as seen-tier for this model)"
                    if s["memorized"] else "unseen for this model")
    s["subject"] = source
    print(json.dumps(s, indent=2))


if __name__ == "__main__":
    main()
