"""Frontier models on a procedural family, with per-model spend guards.

Pre-registered 2026-09-18: 100 held-out instances, the same 10-turn loop,
temperature 0, provider-pinned (no fallbacks — provider drift between episodes
is a measurement confound, `note/openrouter-model-roster-2026-08-05.md`).
A model whose running cost passes --max-usd aborts rather than silently
spending; its partial rows are kept and marked.

Usage:
  OPENROUTER_API_KEY=... python benchmark/harness/m8_frontier.py \\
      --models anthropic/claude-sonnet-5,google/gemini-3.6-flash \\
      --family guide_match --start-index 300 --n 100 --max-usd 15
"""

import argparse
import json
import os
import time

from neutrongym import rollouts

CONFIG = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__)))), "benchmark", "m6_config.json")


class BudgetExceeded(RuntimeError):
    pass


def price_table(path: str = CONFIG) -> dict:
    """{model: {"in": $/Mtok, "out": $/Mtok, "pin": provider}} from the M6 config."""
    with open(path) as f:
        cfg = json.load(f)
    return {m: {"in": v["price"][0], "out": v["price"][1], "pin": v.get("pin")}
            for m, v in cfg["models"].items()}


def cost_usd(usage: dict, price: dict) -> float:
    return (usage.get("prompt_tokens", 0) * price["in"]
            + usage.get("completion_tokens", 0) * price["out"]) / 1e6


class Metered:
    """chat_fn wrapper that accumulates usage and stops at a spend ceiling."""

    def __init__(self, model, price, max_usd, timeout=180):
        self.model, self.price, self.max_usd = model, price, max_usd
        self.usage = {"prompt_tokens": 0, "completion_tokens": 0}
        self.calls, self.timeout = 0, timeout
        import httpx
        self.http = httpx.Client()

    @property
    def spent(self):
        return cost_usd(self.usage, self.price)

    def __call__(self, messages):
        from neutrongym.agent import chat_completion, resolve_backend
        if self.spent >= self.max_usd:
            raise BudgetExceeded(f"{self.model}: ${self.spent:.2f} >= ${self.max_usd}")
        url, key = resolve_backend(self.model)
        resp = chat_completion(self.http, url, key, self.model, messages, [], 0.0,
                               self.price.get("pin"), timeout=self.timeout)
        for k in self.usage:
            self.usage[k] += (resp.get("usage") or {}).get(k) or 0
        self.calls += 1
        return resp["choices"][0]["message"].get("content") or ""


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--models", required=True)
    ap.add_argument("--family", default="guide_match")
    ap.add_argument("--n", type=int, default=100)
    ap.add_argument("--start-index", type=int, default=300)
    ap.add_argument("--max-steps", type=int, default=10)
    ap.add_argument("--target-fraction", type=float, default=0.85)
    ap.add_argument("--match-tolerance", type=float, default=None,
                    help="override the family's pass tolerance (difficulty knob)")
    ap.add_argument("--max-usd", type=float, default=15.0, help="per model")
    ap.add_argument("--out", default=None)
    a = ap.parse_args()
    if not os.environ.get("OPENROUTER_API_KEY"):
        raise SystemExit("OPENROUTER_API_KEY not set")
    prices = price_table()
    record = {"family": a.family, "n_per_family": a.n, "start_index": a.start_index,
              "match_tolerance": a.match_tolerance,
              "max_steps": a.max_steps, "target_fraction": a.target_fraction,
              "split": "heldout", "temperature": 0.0, "arms": {}, "heldout": {}}
    for model in a.models.split(","):
        if model not in prices:
            raise SystemExit(f"no price/pin for {model} in {CONFIG}")
        chat = Metered(model, prices[model], a.max_usd)
        t0, aborted = time.time(), None
        try:
            r = rollouts.evaluate(model, a.n, a.family, "heldout", chat_fn=chat,
                                  max_steps=a.max_steps, start_index=a.start_index,
                                  target_fraction=a.target_fraction,
                                  match_tolerance=a.match_tolerance)
            rows = r["rows"]
        except BudgetExceeded as e:
            aborted, rows = str(e), []
        errored = sum(r.get("error") is not None for r in rows)
        passes = sum(r.get("best_level") == 4 for r in rows)
        record["heldout"][model] = {a.family: {"rows": rows}}
        record["arms"][model] = {
            "passes": passes, "n": len(rows),
            "pass_rate": round(passes / len(rows), 4) if rows else None,
            "errored": errored, "error_rate": round(errored / len(rows), 4) if rows else None,
            "cost_usd": round(chat.spent, 3), "calls": chat.calls,
            "usage": chat.usage, "pin": prices[model].get("pin"),
            "wall_s": round(time.time() - t0), "aborted": aborted,
            # pre-registered: >10% errored means infra-limited, not capability
            "infra_limited": bool(rows) and errored / len(rows) > 0.10}
        s = record["arms"][model]
        print(f"  {model:32} {passes:3}/{len(rows)} = {s['pass_rate']} | errored {errored} "
              f"| ${s['cost_usd']} | {s['wall_s']}s" + (f" | ABORTED {aborted}" if aborted else ""), flush=True)
        out = a.out or f"runs/m8/frontier_{a.family}_n{a.n}_from{a.start_index}.json"
        json.dump(record, open(out, "w"), indent=1)
    print(f"total ${sum(v['cost_usd'] for v in record['arms'].values()):.2f} -> {out}")


if __name__ == "__main__":
    main()
