"""Results of record — the manuscript's citable numbers, committed to git.

Raw per-episode evidence lives in `benchmark/evidence/` (committed); the
DERIVED tables the paper quotes were previously only in gitignored
`runs/`, i.e. one laptop wipe from gone. This regenerates them
deterministically from the evidence into `benchmark/results/` (JSON, for
plotting/checking) plus `benchmark/RESULTS.md` (human/manuscript-ready).

Every table carries its validity accounting: INFRA (endpoint/provider/
harness failures) and LEAK episodes are excluded from rates and reported
separately — the 2026-09-10 correction exists because that was not always
true.

Usage: python3 benchmark/harness/results_report.py
"""

import glob
import json
import math
import os
import sys
from datetime import datetime


def fisher_exact_p(a, b, c, d):
    """Two-sided Fisher exact p for [[a,b],[c,d]] — stdlib only (report
    generators run under plain python3, no scipy)."""
    n = a + b + c + d
    if n == 0:
        return 1.0

    def prob(x):
        return (math.comb(a + b, x) * math.comb(c + d, a + c - x)
                / math.comb(n, a + c))

    lo = max(0, a + c - (c + d))
    hi = min(a + b, a + c)
    obs = prob(a)
    return min(1.0, sum(prob(x) for x in range(lo, hi + 1)
                        if prob(x) <= obs * (1 + 1e-9)))


def _rate(rows, arm, models=None, tasks=None):
    sel = [r for r in rows if r["arm"] == arm
           and r["level"] not in ("INFRA", "LEAK")
           and (models is None or r["model"] in models)
           and (tasks is None or r["task"] in tasks)]
    return sum(1 for r in sel if r["level"] == "PASS"), len(sel)

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))
import taxonomy  # noqa: E402

OUT_DIR = os.path.join(REPO, "benchmark", "results")
LEVELS = ["PASS", "L4", "L3", "L2", "L1", "L0"]


DEV_SPLIT = None


def _dev_split():
    """Dev-split task ids (debugging set) — must not be mixed into scored
    rows: gemini's main row carried 5 dev tasks while its one-shot row
    carried none, so the arms were not compared like-for-like (peer review
    2026-09-10)."""
    global DEV_SPLIT
    if DEV_SPLIT is None:
        with open(os.path.join(REPO, "benchmark", "m6_config.json")) as f:
            DEV_SPLIT = set(json.load(f).get("dev_split") or [])
    return DEV_SPLIT


def _rows(sets, scored_only=True):
    rows = taxonomy.collect(sets)
    if scored_only:
        rows = [r for r in rows if r["task"] not in _dev_split()]
    return rows


def _table(rows):
    """(model, arm) -> counts + validity accounting."""
    agg = {}
    for r in rows:
        a = agg.setdefault((r["model"], r["arm"]),
                           {lv: 0 for lv in LEVELS + ["INFRA", "LEAK"]})
        a[r["level"]] += 1
    out = []
    for (model, arm), c in sorted(agg.items()):
        valid = sum(c[lv] for lv in LEVELS)
        out.append({
            "model": model, "arm": arm, "valid_episodes": valid,
            "passes": c["PASS"],
            "pass_rate": round(c["PASS"] / valid, 4) if valid else None,
            "levels": {lv: c[lv] for lv in LEVELS},
            "infra_excluded": c["INFRA"], "leak_invalid": c["LEAK"]})
    return out


def _md_table(rows, title, note=""):
    md = [f"### {title}", ""]
    if note:
        md += [note, ""]
    md += ["| model | arm | valid n | passes | pass rate | "
           + " | ".join(LEVELS[1:]) + " | infra excl. |",
           "|---|---|---:|---:|---:|" + "---:|" * (len(LEVELS) - 1) + "---:|"]
    for r in rows:
        rate = "—" if r["pass_rate"] is None else f"{r['pass_rate']:.2f}"
        md.append(f"| `{r['model']}` | {r['arm']} | {r['valid_episodes']} | "
                  f"{r['passes']} | {rate} | "
                  + " | ".join(str(r["levels"][lv]) for lv in LEVELS[1:])
                  + f" | {r['infra_excluded']} |")
    return "\n".join(md) + "\n"


def main():
    os.makedirs(OUT_DIR, exist_ok=True)
    matrix_rows = _rows(["m6"])
    final_rows = _rows(["m6_final"])
    matrix, final = _table(matrix_rows), _table(final_rows)

    # spend (ledger lives in gitignored runs/; snapshot the total here)
    ledger_path = os.path.join(REPO, "runs", "m6", "ledger.json")
    spend = None
    if os.path.isfile(ledger_path):
        with open(ledger_path) as f:
            led = json.load(f)
        spend = {"total_usd": led.get("total_usd"),
                 "ledger_entries": len(led.get("episodes", []))}

    contamination = {}
    for p in sorted(glob.glob(os.path.join(REPO, "benchmark",
                                           "contamination", "*.json"))):
        with open(p) as f:
            rec = json.load(f)
        contamination[rec["model"]] = {
            "memorized": rec.get("memorized"),
            "n_references": len(rec.get("references") or {}),
            "status": rec.get("status")}

    t2_path = os.path.join(REPO, "benchmark", "t2_baseline_arms.json")
    t2 = json.load(open(t2_path)) if os.path.isfile(t2_path) else None

    def kinds(rows):
        k = {}
        for r in rows:
            if r["level"] not in ("PASS", "INFRA", "LEAK"):
                k[r["kind"]] = k.get(r["kind"], 0) + 1
        return k

    record = {
        "generated": datetime.now().strftime("%Y-%m-%d %H:%M"),
        "provenance": {
            "config": "benchmark/m6_config.json (pinned: models, provider "
                      "pins, protocol, arms, budget)",
            "evidence": "benchmark/evidence/{m6,m6_final,pilot}/ (committed "
                        "per-episode reports, transcripts, artifacts)",
            "regenerate": "python3 benchmark/harness/results_report.py",
            "validity_rule": "INFRA (endpoint/provider/harness failures) and "
                             "LEAK episodes are excluded from all rates and "
                             "reported separately",
        },
        "matrix": matrix, "held_out_final_pass": final,
        "failure_kinds": {"matrix": kinds(matrix_rows),
                          "held_out": kinds(final_rows)},
        "spend": spend, "contamination_probes": contamination,
        "t2_classical_baselines": t2,
    }
    with open(os.path.join(OUT_DIR, "m6_results.json"), "w") as f:
        json.dump(record, f, indent=1)

    n_matrix = sum(r["valid_episodes"] for r in matrix)
    n_final = sum(r["valid_episodes"] for r in final)
    infra = sum(r["infra_excluded"] for r in matrix + final)
    leaks = sum(r["leak_invalid"] for r in matrix + final)

    md = [
        "# NeutronGym — results of record",
        "",
        f"*Generated {record['generated']} by "
        "`benchmark/harness/results_report.py` from the committed evidence "
        "in `benchmark/evidence/`. **These are the numbers the manuscript "
        "cites.** Regenerate after any re-run; diff the JSON to see what "
        "moved.*",
        "",
        "**Validity rule:** INFRA (endpoint/provider/harness failures) and "
        "LEAK episodes are excluded from every rate and reported "
        "separately. This is not cosmetic — on 2026-09-10 an unnoticed "
        "dead SSH tunnel put 77 infra failures into the tables as "
        "capability zeros, which invalidated an entire model row until "
        "caught and re-run.",
        "",
        f"**Totals:** {n_matrix} valid matrix episodes · {n_final} valid "
        f"held-out episodes · {infra} infra-excluded · {leaks} leak-invalid "
        f"(zero leaks across the whole campaign) · "
        f"${(spend or {}).get('total_usd', 0):.2f} OpenRouter spend.",
        "",
        _md_table([r for r in matrix if r["valid_episodes"] >= 5],
                  "Table 1 — Main matrix (seen-tier scored set)",
                  "`main` = reference loop with MCP + skill · `oneshot` = "
                  "plain-LLM, no tools · `noskill` = loop without the design "
                  "skill · `claude_code` = production-harness comparison "
                  "arm. Dev-split tasks are EXCLUDED so every arm is scored "
                  "on the same 17 tasks. Rows with fewer than 5 valid "
                  "episodes are omitted as uninformative — notably "
                  "`openai/gpt-5.2-pro`, which ran a single episode "
                  "(a PASS) before being cut on cost at $29.21/episode; "
                  "n=1 is a footnote, not a 100% pass rate."),
        "",
        _md_table(final, "Table 2 — Held-out final pass (once-only touch)",
                  "Instruments with no public `.instr` (BOYA, VENUS), "
                  "authored for this benchmark and touched exactly once. "
                  "The tool loop outscores one-shot here (paired, 4 models: "
                  "7/8 vs 2/8, Fisher p=0.041) — **but see the statistical "
                  "note below: these instruments are significantly EASIER "
                  "than the seen-tier set (p<0.001), which is an unexcluded "
                  "alternative explanation for the reversal.**"),
        "",
        "### Table 3 — Failure kinds (valid failures only)",
        "",
        "*`format` = never engaged the tools, or emitted no parseable file "
        "one-shot · `incomplete` = used the tools but finished no "
        "instrument · `physics` = built something that compiles, runs, or "
        "scores wrongly. The format/incomplete split matters: an episode "
        "with 50 validated tool calls and no finished instrument is not a "
        "protocol-mechanics failure.*",
        "",
        "| set | format | incomplete | physics |",
        "|---|---:|---:|---:|",
        f"| matrix | {record['failure_kinds']['matrix'].get('format', 0)} | "
        f"{record['failure_kinds']['matrix'].get('incomplete', 0)} | "
        f"{record['failure_kinds']['matrix'].get('physics', 0)} |",
        f"| held-out | {record['failure_kinds']['held_out'].get('format', 0)} "
        f"| {record['failure_kinds']['held_out'].get('incomplete', 0)} "
        f"| {record['failure_kinds']['held_out'].get('physics', 0)} |",
        "",
        "### Table 4 — Contamination probes (per model, temperature 0, "
        "provider-pinned)",
        "",
        "| model | references memorized | n probed |",
        "|---|---|---:|",
    ]
    for m, c in sorted(contamination.items()):
        mem = ("probe incomplete" if c["memorized"] is None
               else ", ".join(c["memorized"]) or "none")
        md.append(f"| `{m}` | {mem} | {c['n_references']} |")
    # --- statistical power: the section the peer review forced ---
    api = [m for m in {r["model"] for r in matrix_rows}
           if not m.startswith("qwen") and "maverick" not in m
           and "gpt" not in m and m != "subscription"]
    md += ["", "### Statistical power — read this before quoting any "
           "arm-vs-arm difference", "",
           "Two-sided Fisher exact on the pass counts. **At n=17 tasks per "
           "cell, seen-tier differences between arms are NOT resolvable**, "
           "and the one difference that is significant argues against the "
           "headline reading rather than for it.", "",
           "| comparison | counts | p | reading |", "|---|---|---:|---|"]
    for m in sorted(api):
        lp, ln = _rate(matrix_rows, "main", {m})
        op, on = _rate(matrix_rows, "oneshot", {m})
        if not (ln and on):
            continue
        p = fisher_exact_p(lp, ln - lp, op, on - op)
        md.append(f"| seen: `{m}` loop vs one-shot | {lp}/{ln} vs {op}/{on} "
                  f"| {p:.3f} | not resolvable |")
    hlp, hln = _rate(final_rows, "main")
    hop, hon = _rate(final_rows, "oneshot")
    # paired = models with VALID episodes in BOTH arms (a model whose
    # one-shot cells were all INFRA is not a pair — that mistake turns the
    # paired figure back into the pooled one)
    def _has(arm, m):
        return _rate(final_rows, arm, {m})[1] > 0

    paired = sorted(m for m in {r["model"] for r in final_rows}
                    if _has("main", m) and _has("oneshot", m))
    plp, pln = _rate(final_rows, "main", set(paired))
    pop, pon = _rate(final_rows, "oneshot", set(paired))
    p_paired = fisher_exact_p(plp, pln - plp, pop, pon - pop)
    smp, smn = _rate(matrix_rows, "main")
    p_diff = fisher_exact_p(hlp, hln - hlp, smp, smn - smp)
    q8p, q8n = _rate(matrix_rows, "main", {"qwen3_8b"})
    q32p, q32n = _rate(matrix_rows, "main", {"qwen3_32b"})
    md += [
        f"| held-out: loop vs one-shot (paired, {len(paired)} models) | "
        f"{plp}/{pln} vs {pop}/{pon} | {p_paired:.3f} | suggestive, n small, "
        "unadjusted |",
        f"| **held-out vs seen difficulty (loop arm)** | {hlp}/{hln} vs "
        f"{smp}/{smn} | **{p_diff:.4f}** | **held-out tasks are EASIER — "
        "a confound for the row above** |",
        f"| RL ladder: 8B vs 32B (loop) | {q8p}/{q8n} vs {q32p}/{q32n} | "
        f"{fisher_exact_p(q8p, q8n - q8p, q32p, q32n - q32p):.3f} | ordering "
        "holds as a strict pass-set superset; rate difference underpowered |",
        "",
        "**What this means for the manuscript.** The defensible claims are "
        "the ones that do not rest on small pass-rate differences: zero "
        "reference leaks across the whole campaign; T2 unsolved by every "
        "model in every arm; the untrained open-weights floor and its "
        "strict-superset ordering; and the *distributional* failure "
        "structure of Table 3 (one-shot dies at L1, the loop dies at L0). "
        "Arm-vs-arm superiority on the seen tier is **not** supported, and "
        "the held-out reversal cannot be attributed to novelty while the "
        "difficulty difference is uncontrolled — controlling it needs "
        "held-out instruments matched to seen-tier complexity, which is "
        "future work, not a claim this data can carry.",
        "",
        "### Table 5 — T2 classical baselines "
           f"({(t2 or {}).get('verify_ncount', 0):.0e} rays, fresh seed "
           f"{(t2 or {}).get('fresh_seed', '—')})",
           "",
           "*Two independent baselines, not three: the mcrun optimizer arm "
           "was discarded during calibration (nelder-mead escaped parameter "
           "bounds), so `classical_best` IS the constraint-filtered "
           "random-search winner.* **No agent, in any arm, has yet beaten "
           "either T2 target — the improvement tier is unsolved across the "
           "whole matrix, which is the headroom the RL track targets.**",
           "", "| task | initial FOM | classical best | improvement |",
           "|---|---:|---:|---:|"]
    for task, rec in ((t2 or {}).get("tasks") or {}).items():
        arms = rec.get("arms", {})
        ini, best = arms.get("initial", {}), arms.get("classical_best", {})
        if not (ini and best):
            continue
        sig = rec.get("improvement_sigma")
        sig_s = f"{sig:.1f}σ" if isinstance(sig, (int, float)) else "—"
        md.append(f"| `{task}` | {ini['fom']:.6g} | {best['fom']:.6g} | "
                  f"{best['fom']/ini['fom']:.2f}× ({sig_s}) |")
    md += ["", "---", "",
           "Raw evidence per episode (report, transcript, built `.instr`, "
           "diagram): `benchmark/evidence/<set>/<task>__<arm>__<model>/`."]
    with open(os.path.join(REPO, "benchmark", "RESULTS.md"), "w") as f:
        f.write("\n".join(md) + "\n")
    print(f"wrote benchmark/RESULTS.md + benchmark/results/m6_results.json "
          f"({n_matrix} matrix + {n_final} held-out valid episodes)")


if __name__ == "__main__":
    main()
