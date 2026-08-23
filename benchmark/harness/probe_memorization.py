"""Memorization probe (contamination control, SPEC §7 Study A).

Asks a model — WITHOUT tools — to emit a shipped .instr file from memory,
then scores the answer against the real file:

  similarity : difflib ratio over normalized text (whitespace/comments folded)
  keyfacts   : recall of the component-type sequence (the structural skeleton)

A (paper, instrument) pair counts as 'unseen' for a model only if that model
FAILS the probe (both scores below the task thresholds).

Usage:
  python benchmark/harness/probe_memorization.py benchmark/tasks/P2_memorization_psi_dmc.json \
      --model google/gemini-3.5-flash-lite            # via OpenRouter
  python benchmark/harness/probe_memorization.py <task> --answer-file reply.txt   # offline scoring
"""

import argparse
import difflib
import json
import os
import re
import sys
import urllib.request

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


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


def ask_openrouter(model: str, prompt: str, max_tokens: int = 16000,
                   provider: str | None = "Google") -> str:
    payload = {
        "model": model, "max_tokens": max_tokens,
        "temperature": 0,  # the contamination record must be stable
        "messages": [{"role": "user", "content": prompt}],
    }
    if provider:  # provider-pinning consistency policy (2026-08-05 note)
        payload["provider"] = {"order": [provider],
                               "allow_fallbacks": False}
    req = urllib.request.Request(
        "https://openrouter.ai/api/v1/chat/completions",
        data=json.dumps(payload).encode(),
        headers={"Authorization": f"Bearer {os.environ['OPENROUTER_API_KEY']}",
                 "Content-Type": "application/json"})
    with urllib.request.urlopen(req, timeout=300) as resp:
        data = json.load(resp)
    # None content (empty/refusal turn) scores as no reproduction — unseen
    return data["choices"][0]["message"]["content"] or ""


PROBE_PROMPT = (
    "Output the complete contents of the McStas example instrument file "
    "{name}.instr, exactly as it ships with the McStas 3.x distribution, "
    "as a single code block. Do not use any tools. If you cannot reproduce "
    "it exactly, reproduce it as accurately as you can from memory.")
DEFAULT_THRESHOLDS = {"similarity_memorized": 0.5, "keyfact_memorized": 0.7}


def probe_all(model: str) -> dict:
    """Per-task probe over every scored task with a shipped reference
    (M5.5 contamination item a). One record per model, committed to
    benchmark/contamination/<model_tag>.json — a (task, model) pair counts
    as unseen only if the model FAILS its probe."""
    import glob

    from mcstas_mcp import examples
    results = {}
    paths = sorted(glob.glob(os.path.join(REPO, "benchmark", "tasks", "**",
                                          "T*.json"), recursive=True))
    for p in paths:
        with open(p) as f:
            task = json.load(f)
        ref = (task.get("reference") or {}).get("instr", "")
        if ref.startswith("shipped:"):
            name = ref.split(":", 1)[1]
            prompt = PROBE_PROMPT.format(name=name)
            get_ref = lambda: examples.get_example(name)["source"]  # noqa: E731
        elif task.get("probe_hint"):
            # held-out reference authored by us: probe by instrument
            # identity — expected UNSEEN, and the record proves it per model
            name = os.path.splitext(os.path.basename(ref))[0]
            prompt = ("Output a complete McStas .instr model of "
                      f"{task['probe_hint']}, as a single code block. Do "
                      "not use any tools. If you have never seen such a "
                      "model, construct your best attempt from memory.")
            get_ref = lambda: open(os.path.join(REPO, ref),  # noqa: E731
                                   errors="replace").read()
        else:
            continue  # perturbed variants share their canonical's probe
        if name in results:  # tasks sharing a reference share the probe
            results[name]["tasks"].append(task["id"])
            continue
        reference = get_ref()
        try:
            answer = ask_openrouter(model, prompt)
        except Exception as e:  # noqa: BLE001 — one probe must not kill the batch
            print(f"  {name:24} PROBE ERROR: {str(e)[:60]} — retrying once")
            answer = ask_openrouter(model, prompt)
        s = score(answer, reference)
        s["memorized"] = (
            s["similarity"] >= DEFAULT_THRESHOLDS["similarity_memorized"]
            or s["keyfact_recall"] >= DEFAULT_THRESHOLDS["keyfact_memorized"])
        s["tasks"] = [task["id"]]
        results[name] = s
        print(f"  {name:24} sim={s['similarity']:.3f} "
              f"keyfacts={s['keyfact_recall']:.3f} "
              f"{'MEMORIZED' if s['memorized'] else 'unseen'}")
    out_dir = os.path.join(REPO, "benchmark", "contamination")
    os.makedirs(out_dir, exist_ok=True)
    tag = re.sub(r"\W", "_", model)
    record = {"model": model, "thresholds": DEFAULT_THRESHOLDS,
              "references": results,
              "memorized": sorted(n for n, s in results.items()
                                  if s["memorized"])}
    with open(os.path.join(out_dir, f"{tag}.json"), "w") as f:
        json.dump(record, f, indent=1)
    n_mem = len(record["memorized"])
    print(f"{model}: {n_mem}/{len(results)} references memorized "
          f"-> benchmark/contamination/{tag}.json")
    return record


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("task", nargs="?",
                    help="task JSON (single-probe mode); omit with --all")
    ap.add_argument("--model", help="OpenRouter model id")
    ap.add_argument("--all", action="store_true",
                    help="probe every scored task's shipped reference for "
                         "--model; record to benchmark/contamination/")
    ap.add_argument("--answer-file", help="score a saved answer instead of querying")
    args = ap.parse_args()
    if args.all:
        if not args.model:
            sys.exit("--all needs --model")
        probe_all(args.model)
        return
    if not args.task:
        sys.exit("need a task JSON or --all")
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
