"""Environment reward as an HTTP service for GRPO training (M8, 2026-09-16).

The trainer runs in the DGX training env (torch, no McStas); scoring needs the
McStas env. POST /score with
  {"family": ..., "split": "train",
   "items": [{"index": 12, "completion": "<raw model text>"}, ...]}
returns {"results": [{"reward", "level", "fom_ratio", "parsed"}, ...]} in
order. Scoring is stateless in the instance and the action (reward.score), so
items run in parallel across worker processes. An unparseable completion
scores 0.0 without simulating, exactly as the rollout loop treats it.

Usage (DGX): /netdisk/ldq/mcstas-run.sh python benchmark/harness/reward_server.py \\
      --port 8199 --workers 48
"""

import argparse
import json
from concurrent.futures import ProcessPoolExecutor
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

_ENVS = {}


def score_item(job):
    family, split, frac, max_steps, index, completion = job
    from neutrongym import generate, rollouts
    free = generate.FAMILIES[family]["free_parameters"]
    action = rollouts.parse_action(completion, free)
    if action is None:
        return {"reward": 0.0, "level": 0, "fom_ratio": None, "parsed": False}
    from neutrongym.env import NeutronGym
    key = (family, split)
    if key not in _ENVS:
        _ENVS[key] = NeutronGym(family=family, split=split, target_fraction=frac,
                                max_steps=max_steps)
    env = _ENVS[key]
    env.reset(index=index)
    _, reward, _, _, rec = env.step(action)
    return {"reward": float(reward), "level": rec["level"],
            "fom_ratio": rec["levels"].get("L4", {}).get("fom_ratio"), "parsed": True}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--port", type=int, default=8199)
    ap.add_argument("--workers", type=int, default=48)
    ap.add_argument("--target-fraction", type=float, default=0.85)
    a = ap.parse_args()
    from neutrongym import generate
    from neutrongym.env import NeutronGym
    for fam in generate.FAMILIES:                        # compile before forking
        NeutronGym(family=fam, split="train")
    pool = ProcessPoolExecutor(max_workers=a.workers)

    class H(BaseHTTPRequestHandler):
        def do_POST(self):
            req = json.loads(self.rfile.read(int(self.headers["Content-Length"])))
            jobs = [(req["family"], req.get("split", "train"), a.target_fraction,
                     10**6, it["index"], it["completion"]) for it in req["items"]]
            body = json.dumps({"results": list(pool.map(score_item, jobs))}).encode()
            self.send_response(200); self.send_header("Content-Type", "application/json")
            self.send_header("Content-Length", str(len(body))); self.end_headers()
            self.wfile.write(body)

        def log_message(self, *args):
            pass

    print(f"reward server on :{a.port} with {a.workers} workers", flush=True)
    ThreadingHTTPServer(("127.0.0.1", a.port), H).serve_forever()


if __name__ == "__main__":
    main()
