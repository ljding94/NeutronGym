"""M8 pre-eval gate: the trained checkpoint is served like the base model.

Run once the merged checkpoint answers on its port, before `m8_eval.py`.
Fails loudly (exit 1) on any of:

  id        the trained server reports the expected served-model name
  parity    for real held-out guide prompts, /tokenize on the trained
            server returns exactly the token ids the base 8B server returns
            (the merge saved its own chat_template.jinja; a template drift
            would make trained-vs-untrained differ in prompt, not weights)
  action    a temperature-0 completion on a held-out guide prompt parses to
            a JSON action covering every free parameter within bounds —
            the minimum that makes the eval measure design, not formatting

Usage:
  NEUTRONGYM_VLLM_URL_8B=http://localhost:8137/v1 \\
  NEUTRONGYM_VLLM_URL_TRAINED=http://localhost:8139/v1 \\
  python benchmark/harness/m8_serve_check.py
"""

import json
import os
import sys

import httpx

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))

from neutrongym import generate  # noqa: E402
from neutrongym.rollouts import DIALOGUE_SYSTEM, parse_action  # noqa: E402

TRAINED_NAME = "qwen3-8b-m8raft"
KW = {"enable_thinking": False}


def served_id(url):
    return httpx.get(f"{url}/models", timeout=30).json()["data"][0]["id"]


def tokenize(url, model, messages):
    base = url.rsplit("/v1", 1)[0]
    r = httpx.post(f"{base}/tokenize", timeout=60, json={
        "model": model, "messages": messages, "add_generation_prompt": True,
        "chat_template_kwargs": KW}).json()
    return r.get("tokens")


def main():
    base_url = os.environ["NEUTRONGYM_VLLM_URL_8B"]
    trained_url = os.environ["NEUTRONGYM_VLLM_URL_TRAINED"]
    ok = True

    tid, bid = served_id(trained_url), served_id(base_url)
    print(f"id: trained={tid} base={bid}")
    if tid != TRAINED_NAME:
        print(f"FAIL id: expected {TRAINED_NAME}"); ok = False

    fam = "guide_divergence"
    for i in (0, 1, 2):
        inst = generate.instance(fam, "heldout", i)
        msgs = [{"role": "system", "content": DIALOGUE_SYSTEM},
                {"role": "user", "content": generate.render_prompt(inst)}]
        a, b = tokenize(trained_url, tid, msgs), tokenize(base_url, bid, msgs)
        same = a == b and a is not None
        print(f"parity heldout {i}: trained {len(a or [])} tokens, base "
              f"{len(b or [])} tokens -> {'OK' if same else 'MISMATCH'}")
        ok = ok and same

    inst = generate.instance(fam, "heldout", 0)
    msgs = [{"role": "system", "content": DIALOGUE_SYSTEM},
            {"role": "user", "content": generate.render_prompt(inst)}]
    resp = httpx.post(f"{trained_url}/chat/completions", timeout=300, json={
        "model": tid, "messages": msgs, "temperature": 0.0,
        "max_tokens": 512, "chat_template_kwargs": KW}).json()
    text = resp["choices"][0]["message"].get("content") or ""
    action = parse_action(text, inst["free_parameters"])
    print(f"completion: {text[:200]!r}")
    if action is None:
        print("FAIL action: no JSON action covering all free parameters"); ok = False
    else:
        bad = [k for k, (lo, hi) in inst["free_parameters"].items()
               if not lo <= float(action[k]) <= hi]
        print(f"action: {json.dumps(action)} out-of-bounds={bad}")
        ok = ok and not bad

    print("SERVE CHECK", "PASS" if ok else "FAIL")
    sys.exit(0 if ok else 1)


if __name__ == "__main__":
    main()
