"""M8 GRPO data: every episode of the untrained 8B, failures included.

SFT on the model's own successes regressed three times: it only ever saw
states the model handled well, so at test time it applied a copied rule from
states it had never been trained on (e.g. SANS r_pin1 stuck at its minimum).
Step-level GRPO trains on decisions from ALL of these states, so every
episode is kept here, and each is written the moment it finishes (an
interrupted run keeps its progress).

Usage (DGX, mcstas env):
  NEUTRONGYM_VLLM_URL_8B=http://localhost:8137/v1 \\
  python benchmark/harness/m8_states.py --family guide_divergence \\
      --start 0 --n 600 --workers 8 --out runs/m8/grpo_states_guide.jsonl
"""

import argparse
import json
import os
import time
from concurrent.futures import ProcessPoolExecutor

MODEL = "qwen3-8b"
_ENV = {}


def states_from_episode(ep: dict) -> list:
    """Message prefixes that end just before an assistant turn."""
    msgs = ep.get("messages") or []
    return [msgs[:k] for k, m in enumerate(msgs)
            if m.get("role") == "assistant" and k > 0]


def _one(job):
    family, index, max_steps, frac, temperature = job
    import httpx
    from neutrongym import rollouts
    from neutrongym.agent import LOCAL_REQUEST_TIMEOUT_S, chat_completion
    from neutrongym.env import NeutronGym
    if "env" not in _ENV:
        _ENV["env"] = NeutronGym(family=family, split="train",
                                 max_steps=max_steps, target_fraction=frac)
        _ENV["http"] = httpx.Client()
    url = os.environ["NEUTRONGYM_VLLM_URL_8B"]

    def chat(msgs):
        r = chat_completion(_ENV["http"], url, "EMPTY", MODEL, msgs, [], temperature,
                            None, chat_extra={"chat_template_kwargs": {"enable_thinking": False}},
                            timeout=LOCAL_REQUEST_TIMEOUT_S)
        return r["choices"][0]["message"].get("content") or ""

    try:
        ep = rollouts.rollout(_ENV["env"], index, chat)
    except Exception as e:  # noqa: BLE001 -- one bad episode must not stop the run
        return {"family": family, "index": index, "error": str(e)[:200]}
    return {"family": family, "index": index, "split": "train",
            "skipped": ep.get("skipped"), "best_level": ep.get("best_level"),
            "best_reward": ep.get("best_reward"), "messages": ep.get("messages"),
            "rewards": [s.get("reward") for s in ep.get("episode", [])],
            "levels": [s.get("level") for s in ep.get("episode", [])]}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--family", required=True)
    ap.add_argument("--start", type=int, default=0)
    ap.add_argument("--n", type=int, required=True)
    ap.add_argument("--workers", type=int, default=8)
    ap.add_argument("--max-steps", type=int, default=10)
    ap.add_argument("--target-fraction", type=float, default=0.85)
    ap.add_argument("--temperature", type=float, default=0.7)
    ap.add_argument("--out", required=True)
    a = ap.parse_args()
    if not os.environ.get("NEUTRONGYM_VLLM_URL_8B"):
        raise SystemExit("NEUTRONGYM_VLLM_URL_8B not set")
    from neutrongym.env import NeutronGym
    NeutronGym(family=a.family, split="train")          # compile before forking
    done = set()
    if os.path.exists(a.out):                           # resume
        done = {json.loads(l)["index"] for l in open(a.out) if l.strip()}
    jobs = [(a.family, i, a.max_steps, a.target_fraction, a.temperature)
            for i in range(a.start, a.start + a.n) if i not in done]
    t0, n_states, n_err, n_pass = time.time(), 0, 0, 0
    with open(a.out, "a") as f, ProcessPoolExecutor(max_workers=a.workers) as ex:
        for k, rec in enumerate(ex.map(_one, jobs), 1):
            f.write(json.dumps(rec) + "\n"); f.flush()
            n_err += "error" in rec
            n_pass += rec.get("best_level") == 4
            n_states += len(states_from_episode(rec))
            if k % 25 == 0 or k == len(jobs):
                print(f"  {k}/{len(jobs)} episodes, {n_states} states, "
                      f"{n_pass} passed, {n_err} errored ({time.time() - t0:.0f}s)", flush=True)
    print(json.dumps({"episodes": len(jobs), "states": n_states, "passed": n_pass,
                      "errored": n_err, "resumed_from": len(done),
                      "wall_s": round(time.time() - t0)}))


if __name__ == "__main__":
    main()
