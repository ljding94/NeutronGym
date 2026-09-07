"""M6 matrix runner — priority-ordered, resume-safe, budget-guarded.

Enumerates (priority, arm, model, task) episodes from the PINNED config
(benchmark/m6_config.json — the reproducibility contract), skips episodes
that already have a report.json (resume), runs the rest through
run_episode.py, and keeps a spend ledger from ACTUAL token usage x pinned
prices (never estimates). Hard-stops at the configured budget guard.

  python benchmark/harness/run_matrix.py --dry-run          # plan + cost est
  python benchmark/harness/run_matrix.py --priority 1 [--limit N] [--model ID]
  python benchmark/harness/run_matrix.py --report           # aggregate table
"""

import argparse
import json
import os
import subprocess
import sys
import time

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
CONFIG = os.path.join(REPO, "benchmark", "m6_config.json")
OUT = os.path.join(REPO, "runs", "m6")
LEDGER = os.path.join(OUT, "ledger.json")

EST_TOKENS = {"loop": (270_000, 9_000), "oneshot": (2_500, 20_000),
              "claude": (0, 0)}  # subscription: no OpenRouter spend


def load_config():
    with open(CONFIG) as f:
        return json.load(f)


def task_ids(cfg, names):
    out = []
    for n in names:
        if n == "dev":
            out += cfg["dev_split"]
        else:
            out += cfg["scored_sets"][n]
    return out


def model_base_url(cfg, model):
    """Resolve a local model's endpoint; 'env:VAR' comes from the
    environment (the pinned config stays machine-independent).
    Returns (base_url, available)."""
    raw = (cfg["models"].get(model) or {}).get("base_url")
    if not raw:
        return None, True  # OpenRouter model — always routable
    if raw.startswith("env:"):
        url = os.environ.get(raw[4:], "")
        return (url or None), bool(url)
    return raw, True


def enumerate_episodes(cfg, only_priority=None, only_model=None):
    """[(priority, arm_name, model_id_or_None, task_id, episode_dir)] —
    held-out tasks are structurally unreachable here (config keeps them in
    a FINAL_PASS_ONLY set that no priority references)."""
    eps = []
    for entry in cfg["priorities"]:
        if only_priority and entry["p"] != only_priority:
            continue
        models = (list(cfg["models"]) if entry["models"] == "all"
                  else entry["models"])
        for model in models:
            if model and cfg["models"][model].get("reserve"):
                continue  # final-pass reserve never enters the matrix
            if model and not model_base_url(cfg, model)[1]:
                continue  # local endpoint not up (env var unset) — skip
            if only_model and model != only_model:
                continue
            tasks = task_ids(cfg, entry["tasks"])
            if model and cfg["models"][model].get("slice"):
                allowed = set(cfg[cfg["models"][model]["slice"]]) \
                    | set(cfg["dev_split"])
                tasks = [t for t in tasks if t in allowed]
            for t in tasks:
                tag = (model or "subscription").replace("/", "_").replace(
                    "-", "_").replace(".", "_")
                ep = os.path.join(OUT, f"{t}__{entry['arm']}__{tag}")
                eps.append((entry["p"], entry["arm"], model, t, ep))
    return eps


def episode_cost(report_path, cfg, model):
    """Actual cost from recorded usage x pinned prices (0 for subscription)."""
    if model is None:
        return 0.0
    with open(report_path) as f:
        usage = (json.load(f).get("episode") or {}).get("usage") or {}
    pin, pout = cfg["models"][model]["price"]
    return (usage.get("prompt_tokens", 0) * pin
            + usage.get("completion_tokens", 0) * pout) / 1e6


def ledger_total():
    try:
        with open(LEDGER) as f:
            return json.load(f)
    except (OSError, json.JSONDecodeError):
        return {"episodes": [], "total_usd": 0.0}


def run_one(cfg, arm_name, model, task, ep_dir):
    arm = cfg["arms"][arm_name]
    cmd = [sys.executable, os.path.join(REPO, "benchmark", "harness",
                                        "run_episode.py"),
           task, "--scaffold", arm["scaffold"], "--episode-dir", ep_dir,
           "--max-turns", str(cfg["protocol"]["max_turns"])]
    if model:
        cmd += ["--model", model]
        url, _ = model_base_url(cfg, model)
        if url:
            cmd += ["--base-url", url, "--provider", ""]
        else:
            cmd += ["--provider", cfg["models"][model]["pin"]]
    if not arm["skill"]:
        cmd += ["--no-skill"]
    t0 = time.time()
    proc = subprocess.run(cmd, capture_output=True, text=True, timeout=3600)
    ok = os.path.isfile(os.path.join(ep_dir, "report.json"))
    return {"ok": ok, "wall_s": round(time.time() - t0, 1),
            "tail": proc.stdout.strip().splitlines()[-1] if proc.stdout
            else proc.stderr.strip().splitlines()[-1:]}


def cmd_run(cfg, args):
    led = ledger_total()
    eps = enumerate_episodes(cfg, args.priority, args.model)
    todo = [e for e in eps
            if not os.path.isfile(os.path.join(e[4], "report.json"))]
    print(f"{len(eps)} episodes in scope, {len(eps) - len(todo)} done, "
          f"{len(todo)} to run; ledger ${led['total_usd']:.2f}")
    ran = 0
    for p, arm, model, task, ep_dir in todo:
        if args.limit and ran >= args.limit:
            break
        if led["total_usd"] >= cfg["budget"]["hard_stop_usd"]:
            print(f"HARD STOP: ledger ${led['total_usd']:.2f} >= "
                  f"${cfg['budget']['hard_stop_usd']}")
            break
        print(f"[p{p}] {task} / {arm} / {model or 'subscription'} ...",
              flush=True)
        res = run_one(cfg, arm, model, task, ep_dir)
        ran += 1
        cost = 0.0
        if res["ok"]:
            cost = episode_cost(os.path.join(ep_dir, "report.json"), cfg,
                                model)
        led["episodes"].append({"task": task, "arm": arm, "model": model,
                                "usd": round(cost, 4),
                                "wall_s": res["wall_s"], "ok": res["ok"]})
        led["total_usd"] = round(led["total_usd"] + cost, 4)
        os.makedirs(OUT, exist_ok=True)
        with open(LEDGER, "w") as f:
            json.dump(led, f, indent=1)
        print(f"    -> {'ok' if res['ok'] else 'FAILED'} "
              f"${cost:.2f} ({res['wall_s']}s) ledger ${led['total_usd']:.2f}"
              f" | {res['tail']}")
        if led["total_usd"] >= cfg["budget"]["warn_at_usd"]:
            print(f"    WARN: past ${cfg['budget']['warn_at_usd']} warning "
                  "threshold")
    print(f"done: ran {ran}; ledger ${led['total_usd']:.2f}")


def cmd_dry_run(cfg, args):
    eps = enumerate_episodes(cfg, args.priority, args.model)
    total = 0.0
    by = {}
    for p, arm, model, task, ep_dir in eps:
        scaffold = cfg["arms"][arm]["scaffold"]
        tin, tout = EST_TOKENS[scaffold]
        price = cfg["models"][model]["price"] if model else (0, 0)
        est = (tin * price[0] + tout * price[1]) / 1e6
        done = os.path.isfile(os.path.join(ep_dir, "report.json"))
        key = (p, arm, model or "subscription")
        n, c, d = by.get(key, (0, 0.0, 0))
        by[key] = (n + 1, c + est, d + done)
        total += 0 if done else est
    for (p, arm, model), (n, c, d) in sorted(by.items(),
                                             key=lambda kv: kv[0][0]):
        print(f"  p{p} {arm:12} {model:34} {n:3} eps ({d} done)  "
              f"est ${c:6.2f}")
    print(f"estimated remaining spend: ${total:.2f} "
          f"(ledger so far ${ledger_total()['total_usd']:.2f})")


def cmd_report(cfg):
    rows = {}
    for d in sorted(os.listdir(OUT)) if os.path.isdir(OUT) else []:
        rp = os.path.join(OUT, d, "report.json")
        if not os.path.isfile(rp):
            continue
        r = json.load(open(rp))
        task = r["task"]
        arm, model = d.split("__")[1], d.split("__", 2)[2]
        g = r["grade"]
        leak = (r.get("reference_leak") or {}).get("leaked")
        key = (model, arm)
        rows.setdefault(key, []).append(
            {"task": task, "pass": bool(g["pass"]) and not leak,
             "score": g["score"], "leaked": leak})
    print(f"{'model':38} {'arm':10} {'n':>3} {'pass':>5} {'mean':>6} "
          f"{'leaks':>5}")
    summary = []
    for (model, arm), rs in sorted(rows.items()):
        n = len(rs)
        npass = sum(r["pass"] for r in rs)
        mean = sum(r["score"] for r in rs) / n
        leaks = sum(1 for r in rs if r["leaked"])
        print(f"{model:38} {arm:10} {n:>3} {npass:>3}/{n:<3} {mean:6.2f} "
              f"{leaks:>5}")
        summary.append({"model": model, "arm": arm, "n": n, "pass": npass,
                        "mean_score": round(mean, 3), "leaks": leaks,
                        "tasks": rs})
    os.makedirs(OUT, exist_ok=True)
    with open(os.path.join(OUT, "summary.json"), "w") as f:
        json.dump(summary, f, indent=1)
    print(f"-> {os.path.relpath(os.path.join(OUT, 'summary.json'), REPO)}")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--report", action="store_true")
    ap.add_argument("--priority", type=int, default=None)
    ap.add_argument("--model", default=None)
    ap.add_argument("--limit", type=int, default=None)
    args = ap.parse_args()
    cfg = load_config()
    if args.report:
        cmd_report(cfg)
    elif args.dry_run:
        cmd_dry_run(cfg, args)
    else:
        cmd_run(cfg, args)


if __name__ == "__main__":
    main()
