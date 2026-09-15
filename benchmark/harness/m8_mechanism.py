"""Why RAFT SFT regressed the 8B — reproducible mechanism record.

The pre-registered M8 evaluation (guide, 1.0x, n=300 paired) found the
trained 8B significantly worse than the untrained 8B. This script recomputes
the evidence for the mechanism and writes it to JSON, so the analysis
section cites numbers a reader can regenerate rather than console output:

  training data   how often episodes open at the all-max corner, keep w_in
                  pinned there on exploration turns, and what the passing
                  action looks like
  first move      the opening action of each 8B on the same held-out prompts
  replays         temperature-0 trained-model episodes on held-out instances
                  (regressed and not), checking for the fixed w_in-pinned
                  sweep and its exact recurrence

Usage:
  NEUTRONGYM_VLLM_URL_8B=... NEUTRONGYM_VLLM_URL_TRAINED=... \
  python benchmark/harness/m8_mechanism.py [--prompts 20] [--replays 15]
"""

import argparse
import collections
import json
import os
import re
import sys

import httpx

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))

from neutrongym.env import NeutronGym  # noqa: E402
from neutrongym.rollouts import DIALOGUE_SYSTEM, parse_action, rollout  # noqa: E402

CORNER = (0.09, 0.09, 3.0)


def key(a):
    return (round(float(a["w_in"]), 3), round(float(a["w_out"]), 3),
            round(float(a["m_coat"]), 2))


def data_stats(path):
    seqs = []
    for line in open(path):
        if not line.strip():
            continue
        rec = json.loads(line)
        s = []
        for m in rec["messages"]:
            if m["role"] != "assistant":
                continue
            mm = re.search(r"\{.*\}", m["content"] or "", re.S)
            try:
                s.append(key(json.loads(mm.group(0))) if mm else None)
            except Exception:
                s.append(None)
        seqs.append(s)
    open_corner = [s for s in seqs if s and s[0] == CORNER]
    pinned = [s for s in open_corner
              if len(s) > 1 and all(a and a[0] == 0.09 for a in s[1:-1])]
    turns = sum(len(s) for s in seqs)
    return {"episodes": len(seqs), "assistant_turns": turns,
            "open_at_corner": len(open_corner),
            "open_at_corner_and_w_in_pinned_on_exploration": len(pinned),
            "final_action_top": [[list(k), c] for k, c in collections.Counter(
                s[-1] for s in seqs if s and s[-1]).most_common(5)],
            "distinct_sequences": len({tuple(s) for s in seqs})}


def chat_fn(url, model):
    cli = httpx.Client(timeout=600)

    def f(msgs):
        r = cli.post(f"{url}/chat/completions", json={
            "model": model, "messages": msgs, "temperature": 0.0,
            "max_tokens": 512,
            "chat_template_kwargs": {"enable_thinking": False}}).json()
        return r["choices"][0]["message"].get("content") or ""
    return f


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--prompts", type=int, default=20)
    ap.add_argument("--replays", type=int, default=15,
                    help="per group (regressed / other)")
    ap.add_argument("--eval", default=os.path.join(REPO, "runs", "m8", "eval_guide_1x_n300.json"))
    ap.add_argument("--data", default=os.path.join(REPO, "runs", "m8", "raft", "train.jsonl"))
    ap.add_argument("--out", default=os.path.join(REPO, "runs", "m8", "mechanism.json"))
    ap.add_argument("--trained-model", default="qwen3-8b-m8raft",
                    help="served name of the trained checkpoint (the post-hoc "
                         "passing-turn ablation is qwen3-8b-m8raft-pass)")
    args = ap.parse_args()
    arms = {"untrained-8b": (os.environ["NEUTRONGYM_VLLM_URL_8B"], "qwen3-8b"),
            "trained-8b": (os.environ["NEUTRONGYM_VLLM_URL_TRAINED"], args.trained_model)}
    rec = {"trained_model": args.trained_model, "eval": os.path.relpath(args.eval, REPO),
           "training_data": data_stats(args.data)}
    print("training data:", json.dumps(rec["training_data"]), flush=True)

    env = NeutronGym(family="guide_divergence", split="heldout",
                     target_fraction=1.0, max_steps=6)
    first = {arm: 0 for arm in arms}
    for i in range(args.prompts):
        obs, _ = env.reset(index=i)
        msgs = [{"role": "system", "content": DIALOGUE_SYSTEM},
                {"role": "user", "content": obs["prompt"]}]
        for arm, (url, model) in arms.items():
            a = parse_action(chat_fn(url, model)(msgs),
                             obs["instance"]["free_parameters"])
            first[arm] += bool(a) and key(a) == CORNER
    rec["first_move_at_corner"] = {"prompts": args.prompts, **first}
    print("first move at corner:", rec["first_move_at_corner"], flush=True)

    ev = json.load(open(args.eval))["heldout"]
    u = {r["instance"]: r for r in ev["untrained-8b"]["guide_divergence"]["rows"]}
    t = {r["instance"]: r for r in ev["trained-8b"]["guide_divergence"]["rows"]}
    groups = {"regressed": [i for i in sorted(u) if u[i]["best_level"] == 4
                            and t[i]["best_level"] != 4][:args.replays],
              "other": [i for i in sorted(u) if not (u[i]["best_level"] == 4
                        and t[i]["best_level"] != 4)][:args.replays]}
    call = chat_fn(*arms["trained-8b"])
    traj = collections.Counter()
    rec["replays"] = {}
    for g, idxs in groups.items():
        rows = []
        for i in idxs:
            ep = rollout(env, i, call)
            s = [key(x["action"]) for x in ep["episode"]]
            traj[tuple(s)] += 1
            rows.append({"instance": i, "best_level": ep["best_level"],
                         "actions": [list(a) for a in s]})
        n = len(rows)
        rec["replays"][g] = {
            "n": n,
            "open_corner_then_w_out_0.03": sum(
                1 for r in rows if [tuple(a) for a in r["actions"][:2]]
                == [CORNER, (0.09, 0.03, 2.5)]),
            "w_in_pinned_whole_episode": sum(
                1 for r in rows if all(a[0] == 0.09 for a in r["actions"])),
            "passes": sum(1 for r in rows if r["best_level"] == 4),
            "episodes": rows}
        print(g, {k: v for k, v in rec["replays"][g].items() if k != "episodes"}, flush=True)
    top, count = traj.most_common(1)[0]
    rec["most_common_trajectory"] = {"actions": [list(a) for a in top],
                                     "count": count,
                                     "of": sum(traj.values())}
    print("most common trajectory:", rec["most_common_trajectory"], flush=True)
    with open(args.out, "w") as f:
        json.dump(rec, f, indent=1)
    print(f"-> {os.path.relpath(args.out, REPO)}")


if __name__ == "__main__":
    main()
