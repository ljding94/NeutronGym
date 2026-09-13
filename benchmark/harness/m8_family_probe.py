"""M8 per-family probe — is there a clean 8B-vs-32B ordering, and how are
passes earned?

The difficulty sweep (2026-09-13) found its only significant 32B lead in the
SANS family, which has a direct-beam reward hole: a constant all-max pinhole
policy with no model passes 8/25 held-out instances at 1.0×, mostly at
ordinary-looking FOM ratios, so FOM filtering cannot separate leakage from
skill. On the clean guide family the lead was +16 points at 1.0× but not
significant at n=25 (McNemar 4v8, p=0.39).

This probe answers both open questions with one protocol identical to the
sweep (same `rollouts.evaluate`, held-out split, temperature 0, 6 steps):

  * n large enough to resolve the guide-family ordering;
  * the action that first cleared L4, so every SANS pass is classified as
    leaking or not by the instrument's own geometry
    (`generate.sans_direct_beam_leaks`).

Paired McNemar is reported on raw passes and on leak-free passes.

Usage:
  python benchmark/harness/m8_family_probe.py --family guide_divergence \\
      --fraction 1.0 --n 100
  python benchmark/harness/m8_family_probe.py --family sans_collimation \\
      --fraction 1.0 --n 25
"""

import argparse
import json
import os
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))

from neutrongym import generate, rollouts  # noqa: E402
from m8_paired import analyze, pair_rows  # noqa: E402

MODELS = {"qwen3-8b": "NEUTRONGYM_VLLM_URL_8B",
          "qwen3-32b": "NEUTRONGYM_VLLM_URL_32B"}


def classify_rows(family: str, split: str, rows: list) -> list:
    """Annotate each row with `leak` (None when not applicable) and
    `clean_pass` — a pass that did not depend on direct-beam leakage."""
    out = []
    for r in rows:
        r = dict(r)
        leak = None
        if family == "sans_collimation" and r.get("pass_action"):
            ctx = generate.instance(family, split, r["instance"])["context"]
            leak = generate.sans_direct_beam_leaks(ctx, r["pass_action"])
        r["leak"] = leak
        r["clean_pass"] = r.get("best_level") == 4 and not leak
        out.append(r)
    return out


def as_clean_levels(rows: list) -> list:
    """Rows whose leaking passes are demoted below L4, for a paired test on
    leak-free passes only."""
    return [dict(r, best_level=(3 if r.get("leak") else r.get("best_level")))
            for r in rows]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--family", required=True, choices=list(generate.FAMILIES))
    ap.add_argument("--fraction", type=float, required=True)
    ap.add_argument("--n", type=int, required=True)
    ap.add_argument("--out", default=None)
    args = ap.parse_args()
    for var in MODELS.values():
        if not os.environ.get(var):
            raise SystemExit(f"{var} not set")
    out = args.out or os.path.join(
        REPO, "runs", "m8",
        f"probe_{args.family}_{args.fraction:g}x_n{args.n}.json")

    record = {"family": args.family, "fraction": args.fraction, "n": args.n,
              "split": "heldout", "temperature": 0.0, "max_steps": 6,
              "heldout": {}, "summary": {}}
    for model, var in MODELS.items():
        r = rollouts.evaluate(model, args.n, args.family, "heldout",
                              base_url=os.environ[var], temperature=0.0,
                              target_fraction=args.fraction)
        rows = classify_rows(args.family, "heldout", r["rows"])
        record["heldout"][model] = {args.family: {"rows": rows}}
        valid = [x for x in rows if x.get("best_level") is not None]
        passes = sum(1 for x in valid if x["best_level"] == 4)
        leaks = sum(1 for x in valid if x.get("leak"))
        clean = sum(1 for x in valid if x["clean_pass"])
        record["summary"][model] = {
            "n_valid": len(valid), "passes": passes, "leaking_passes": leaks,
            "clean_passes": clean,
            "pass_rate": round(passes / len(valid), 4) if valid else None,
            "clean_pass_rate": round(clean / len(valid), 4) if valid else None}
        print(f"  {model:10} pass {passes}/{len(valid)}  leaking {leaks}  "
              f"clean {clean}/{len(valid)}  ({r['wall_s']}s)", flush=True)

    m = tuple(MODELS)
    raw = analyze(pair_rows(record, m), m)
    clean_rec = {"heldout": {k: {args.family: {"rows": as_clean_levels(
        v[args.family]["rows"])}} for k, v in record["heldout"].items()}}
    cln = analyze(pair_rows(clean_rec, m), m)
    for name, res in (("raw", raw), ("clean", cln)):
        record[f"paired_{name}"] = {k: res[k] for k in (
            "only_qwen3-8b", "only_qwen3-32b", "mcnemar_p", "agreement",
            "ordering_supported", "balanced", "leader")}
        print(f"  paired {name:5}: 8B-only {res['only_qwen3-8b']} vs "
              f"32B-only {res['only_qwen3-32b']}  p={res['mcnemar_p']}  "
              f"ordering={res['ordering_supported']}")
    os.makedirs(os.path.dirname(out), exist_ok=True)
    with open(out, "w") as f:
        json.dump(record, f, indent=1)
    print(f"-> {os.path.relpath(out, REPO)}")


if __name__ == "__main__":
    main()
