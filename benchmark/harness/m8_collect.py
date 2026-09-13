"""M8 phase 1 — self-generated (RAFT) SFT data from the untrained 8B.

Samples the untrained Qwen3-8B on TRAIN-split procedural instances (whose
context regimes are disjoint from the held-out split the evaluation uses),
keeps only episodes that actually reach L4 at the calibrated bar, and writes
them as chat-format JSONL for LoRA SFT. The data is the model's OWN
successful behaviour, selected by the reward — not a frontier model's —
which is what separates "the reward trains" from distillation
(note/m8-execution-plan-2026-09-12.md §2).

The keep rule is a strict L4 pass, not `best_reward >= 1.0`: a reward of
exactly 1.0 is a FOM ratio of exactly 1.0, i.e. resubmitting the target, and
the ladder does not count that as a pass. Keepers must clear the same bar
the evaluation later scores.

Collection runs family by family and writes a manifest with the keep rate
and wall time per family, so an interrupted run is visible and a short one
is not mistaken for a thin rate.

Usage:
  python benchmark/harness/m8_collect.py --target-fraction 1.0 \
      --n 200 [--temperature 0.7] [--out runs/m8/raft]
"""

import argparse
import json
import os
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))

from neutrongym import generate, rollouts  # noqa: E402

MODEL = "qwen3-8b"
URL_VAR = "NEUTRONGYM_VLLM_URL_8B"
# strictly above a FOM ratio of 1.0: reward = 0.75 + 0.25 * min(ratio, 2)
STRICT_L4_REWARD = 1.0 + 1e-6


def merge(out_dir: str, families: list) -> dict:
    """Concatenate per-family keeper files into train.jsonl -> stats."""
    total = 0
    by_family = {}
    with open(os.path.join(out_dir, "train.jsonl"), "w") as dst:
        for fam in families:
            p = os.path.join(out_dir, f"{fam}.jsonl")
            n = 0
            if os.path.isfile(p):
                with open(p) as src:
                    for line in src:
                        if not line.strip():
                            continue
                        rec = json.loads(line)
                        if rec.get("best_level") != 4:
                            continue  # never train on a non-pass
                        dst.write(json.dumps(rec) + "\n")
                        n += 1
            by_family[fam] = n
            total += n
    return {"train_examples": total, "by_family": by_family}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--n", type=int, default=200,
                    help="train instances sampled per family")
    ap.add_argument("--target-fraction", type=float, required=True,
                    help="calibrated bar the keepers must clear; use the "
                         "bar the evaluation will score")
    ap.add_argument("--temperature", type=float, default=0.7)
    ap.add_argument("--max-steps", type=int, default=6)
    ap.add_argument("--families", default=",".join(generate.FAMILIES))
    ap.add_argument("--out", default=os.path.join(REPO, "runs", "m8", "raft"))
    args = ap.parse_args()
    url = os.environ.get(URL_VAR)
    if not url:
        raise SystemExit(f"{URL_VAR} not set — RAFT samples the local 8B")

    families = args.families.split(",")
    os.makedirs(args.out, exist_ok=True)
    manifest = {"model": MODEL, "split": "train", "n_per_family": args.n,
                "target_fraction": args.target_fraction,
                "temperature": args.temperature, "max_steps": args.max_steps,
                "keep_rule": "strict L4 pass at the calibrated bar",
                "families": {}}
    for fam in families:
        r = rollouts.collect(
            MODEL, args.n, os.path.join(args.out, f"{fam}.jsonl"),
            family=fam, split="train", reward_threshold=STRICT_L4_REWARD,
            max_steps=args.max_steps, temperature=args.temperature,
            base_url=url, target_fraction=args.target_fraction)
        manifest["families"][fam] = {k: r[k] for k in
                                     ("instances", "kept", "keep_rate",
                                      "wall_s")}
        print(f"  {fam:18} kept {r['kept']}/{r['instances']} "
              f"rate={r['keep_rate']} ({r['wall_s']}s)", flush=True)
        with open(os.path.join(args.out, "manifest.json"), "w") as f:
            json.dump(manifest, f, indent=1)

    manifest.update(merge(args.out, families))
    with open(os.path.join(args.out, "manifest.json"), "w") as f:
        json.dump(manifest, f, indent=1)
    print(f"-> {manifest['train_examples']} train examples "
          f"{manifest['by_family']} in {os.path.relpath(args.out, REPO)}")


if __name__ == "__main__":
    main()
