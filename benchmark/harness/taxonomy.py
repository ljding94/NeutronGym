"""Level-resolved failure taxonomy over episode evidence (M7 item 1).

Benchmark episodes are graded by `grader.py`, which does not stamp the
env's L1-L4 field — so the ladder is DERIVED here from what every episode
record already carries. Levels mirror `neutrongym.reward`:

  INFRA   episode.error / nonzero returncode — the API or endpoint failed,
          NOT a capability datapoint. MUST be excluded from pass rates.
  LEAK    reference_leak.leaked — invalid regardless of score.
  L0      no artifact: the agent left no built instrument at all.
          kind: "format" if the agent never engaged the tools (or emitted
          no parseable file one-shot); "incomplete" if it used tools but
          never finished — those are NOT protocol-mechanics failures.
  L1      syntax/compile: an .instr exists but does not translate/compile.
  L2      runtime: compiles, dies during the simulation run.
  L3      structural: runs, but a graded monitor role is missing or below
          the statistics floor (wrong instrument, right syntax).
  L4      scientific: runs and is gradable, but observables miss tolerance.
  PASS    every graded check within tolerance.

Cross-cutting split (SPEC §7 / M7): FORMAT failures are tool-calling and
protocol mechanics (never engaged the tools, burned the turn cap, empty
responses); PHYSICS failures are wrong instruments (L1-L4). L0 splits
between them by whether the agent ever called a tool.

Usage:
  python3 benchmark/harness/taxonomy.py [--sets m6,m6_final] [--json OUT]
"""

import argparse
import json
import os

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
EVIDENCE = os.path.join(REPO, "benchmark", "evidence")
LEVELS = ["PASS", "L4", "L3", "L2", "L1", "L0", "LEAK", "INFRA"]


def classify(report: dict) -> dict:
    """-> {level, kind, reason} for one episode report."""
    ep = report.get("episode") or {}
    grade = report.get("grade") or {}
    leak = (report.get("reference_leak") or {}).get("leaked")
    hard = " ".join(grade.get("hard_failures") or [])

    if ep.get("error") or ep.get("returncode"):
        err = str(ep.get("error") or f"returncode {ep.get('returncode')}")
        kind = ("endpoint_unreachable" if "ConnectError" in err
                else "provider_error" if "HTTP" in err else "harness_error")
        return {"level": "INFRA", "kind": kind, "reason": err[:120]}
    if leak:
        return {"level": "LEAK", "kind": "contamination",
                "reason": "reference leak in transcript"}
    if grade.get("pass"):
        return {"level": "PASS", "kind": "success", "reason": ""}

    if "no built instrument" in hard:
        engaged = bool(ep.get("mcp_calls"))
        scaffold = ep.get("scaffold") or ""
        # KIND FIX (2026-09-10, peer review): every L0 path used to return
        # "format", which made the format-vs-physics split a relabelling of
        # L0-vs-L1..L4 rather than a real cross-cut. An episode that made 50
        # validated tool calls and still finished no instrument is NOT a
        # tool/protocol-mechanics failure — it is an incomplete construction.
        if "oneshot" in scaffold:
            kind, reason = "format", "no parseable .instr in the answer"
        elif not engaged:
            kind, reason = "format", "never called a tool"
        elif ep.get("hit_turn_cap"):
            kind, reason = "incomplete", "turn cap reached mid-construction"
        else:
            kind, reason = "incomplete", "tools used but no instrument built"
        return {"level": "L0", "kind": kind, "reason": reason}
    if "(translate)" in hard or "(compile)" in hard:
        return {"level": "L1", "kind": "physics",
                "reason": "instrument does not compile"}
    if "(run)" in hard:
        return {"level": "L2", "kind": "physics",
                "reason": "runtime failure during simulation"}
    if "statistics floor" in hard or "no monitor with role" in hard:
        return {"level": "L3", "kind": "physics",
                "reason": hard[:100]}
    if hard:
        return {"level": "L3", "kind": "physics", "reason": hard[:100]}
    return {"level": "L4", "kind": "physics",
            "reason": f"observables outside tolerance "
                      f"({grade.get('checks_passed')})"}


def collect(sets):
    rows = []
    for s in sets:
        root = os.path.join(EVIDENCE, s)
        if not os.path.isdir(root):
            continue
        for d in sorted(os.listdir(root)):
            p = os.path.join(root, d, "report.json")
            if not os.path.isfile(p):
                continue
            with open(p) as f:
                r = json.load(f)
            parts = d.split("__")
            rows.append({"set": s, "task": r.get("task", parts[0]),
                         "arm": parts[1] if len(parts) > 1 else "?",
                         "model": d.split("__", 2)[2] if len(parts) > 2 else "?",
                         **classify(r)})
    return rows


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--sets", default="m6,m6_final")
    ap.add_argument("--json", default=None)
    args = ap.parse_args()
    rows = collect(args.sets.split(","))

    by_model = {}
    for r in rows:
        key = (r["model"], r["arm"])
        by_model.setdefault(key, {lv: 0 for lv in LEVELS})[r["level"]] += 1

    print(f"{'model':28} {'arm':8} " +
          " ".join(f"{lv:>5}" for lv in LEVELS) + "   valid  pass%")
    for (m, a), c in sorted(by_model.items()):
        valid = sum(c[lv] for lv in LEVELS if lv not in ("INFRA", "LEAK"))
        rate = f"{100*c['PASS']/valid:5.0f}" if valid else "    —"
        print(f"{m:28} {a:8} " + " ".join(f"{c[lv]:5}" for lv in LEVELS) +
              f"  {valid:5}  {rate}")

    tot = {lv: sum(c[lv] for c in by_model.values()) for lv in LEVELS}
    n_valid = sum(v for lv, v in tot.items() if lv not in ("INFRA", "LEAK"))
    print(f"\n{len(rows)} episodes: {tot['INFRA']} INFRA-excluded, "
          f"{tot['LEAK']} leak-invalid, {n_valid} valid capability datapoints")
    fmt = sum(1 for r in rows if r["kind"] == "format")
    phys = sum(1 for r in rows if r["kind"] == "physics")
    print(f"among valid failures: {fmt} format (tool/protocol mechanics), "
          f"{phys} physics (wrong instrument)")
    infra_kinds = {}
    for r in rows:
        if r["level"] == "INFRA":
            infra_kinds[r["kind"]] = infra_kinds.get(r["kind"], 0) + 1
    if infra_kinds:
        print(f"INFRA breakdown: {infra_kinds}")
    if args.json:
        with open(args.json, "w") as f:
            json.dump({"rows": rows, "by_model":
                       {f"{m}|{a}": c for (m, a), c in by_model.items()}},
                      f, indent=1)
        print(f"-> {os.path.relpath(args.json, REPO)}")


if __name__ == "__main__":
    main()
