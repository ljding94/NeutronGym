"""Generate tasks.html — human-friendly catalog of the benchmark tasks.

Sources: benchmark/tasks/**/*.json (the tasks), benchmark/tasks/T1/_validation.json
(self-validation), benchmark/inventory.json (machine-verification metadata).
Reference links point at the McStas example sources on GitHub (mccode-dev/McCode).

Regenerate after task changes: python3 scripts/tasks_report.py  (stdlib only)
"""

import glob
import html
import json
import os
from datetime import datetime

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(REPO, "benchmark", "tasks.html")
GREEN, AMBER, BLUE, GRAY = "#2da44e", "#bf8700", "#0969da", "#656d76"
GH = "https://github.com/mccode-dev/McCode/tree/main/mcstas-comps/examples"


def esc(x):
    return html.escape(str(x))


def load_all():
    tasks = []
    for p in sorted(glob.glob(os.path.join(REPO, "benchmark", "tasks", "**", "*.json"),
                              recursive=True)):
        if os.path.basename(p).startswith("_"):
            continue
        with open(p) as f:
            t = json.load(f)
        t["_path"] = os.path.relpath(p, REPO)
        tasks.append(t)
    val = {}
    vp = os.path.join(REPO, "benchmark", "tasks", "T1", "_validation.json")
    if os.path.isfile(vp):
        with open(vp) as f:
            val = {r["task"]: r for r in json.load(f)}
    inv = {}
    ip = os.path.join(REPO, "benchmark", "inventory.json")
    if os.path.isfile(ip):
        with open(ip) as f:
            inv = {e["name"]: e for e in json.load(f)["instruments"]}
    return tasks, val, inv


def gh_link(inv_entry):
    path = inv_entry.get("path", "")
    marker = "/resources/examples/"
    if marker not in path:
        return None
    rel = path.split(marker, 1)[1]
    return f"{GH}/{os.path.dirname(rel)}"


def ref_name(task):
    ref = (task.get("reference") or {}).get("instr", "")
    return ref.split(":", 1)[1] if ref.startswith("shipped:") else None


def badge(text, color):
    return (f'<span style="background:{color};color:#fff;padding:1px 8px;'
            f'border-radius:999px;font-size:11px;font-weight:600">{esc(text)}</span>')


def task_card(t, val, inv):
    name = ref_name(t)
    e = inv.get(name, {}) if name else {}
    v = val.get(t["id"])
    parts = [f'<div class="card" id="{esc(t["id"])}">']
    if v and v.get("pass"):
        status = badge("validated", GREEN)
    elif t["id"].startswith("P"):
        status = badge("pilot-validated", GREEN)
    elif t.get("kind") == "improve":
        status = (badge("calibrated", GREEN) if t.get("targets")
                  else badge("uncalibrated", AMBER))
    elif t.get("kind") == "open_design":
        status = badge("definition — rubric pending", AMBER)
    else:
        status = badge("unvalidated", AMBER)
    parts.append(f'<h3><code>{esc(t["id"])}</code> {status} '
                 f'{badge(t.get("split", t["tier"]), BLUE if t.get("split") != "dev" else GRAY)}</h3>')
    meta = [f'class: <strong>{esc(t.get("class", t["kind"]))}</strong>']
    if name:
        meta.append(f'reference: <code>{esc(name)}</code>')
        link = gh_link(e)
        if link:
            meta.append(f'<a href="{esc(link)}">source on GitHub</a>')
    if e.get("example_line"):
        meta.append(f'ground truth: <code>%Example: {esc(e["example_line"][:60])}</code>')
    chk = e.get("example_check")
    if chk:
        meta.append("reference verified in-env: "
                    + ("✅" if chk.get("agree") else "❌"))
    parts.append('<p class="meta">' + " · ".join(meta) + "</p>")

    paper = t.get("paper")
    if paper:
        doi = paper.get("doi")
        doi_html = (f' — <a href="https://doi.org/{esc(doi)}">doi:{esc(doi)}</a>'
                    if doi else "")
        parts.append(f'<p class="meta">paper: {esc(paper["cite"])}{doi_html} '
                     f'({esc(paper.get("access", "?"))})</p>')

    proto = t.get("protocol", {})
    if proto:
        parts.append(f'<p class="meta">protocol: ncount {proto.get("ncount"):g}, '
                     f'seed {proto.get("seed")}</p>')
    monitors = t.get("grading", {}).get("monitors", [])
    if monitors:
        rows = ""
        for m in monitors:
            obs = ", ".join(f'{o} (rtol {c.get("rtol")}' +
                            (f', {c.get("nsigma")}σ' if c.get("nsigma") else "") + ")"
                            for o, c in m.get("observables", {}).items()) or "presence only"
            rows += f"<tr><td><code>{esc(m['role'])}</code></td><td>{esc(obs)}</td></tr>"
        parts.append("<table><tr><th>graded monitor role</th><th>observables "
                     "(tolerance)</th></tr>" + rows + "</table>")
    if t.get("kind") == "improve":
        fp = t.get("free_parameters", {})
        parts.append('<p class="meta">free parameters: ' + ", ".join(
            f"<code>{esc(k)}</code> ∈ [{lo:g}, {hi:g}]" for k, (lo, hi) in fp.items())
            + "</p>")
        tgt = t.get("targets", {}).get("fom_min")
        base = t.get("baselines", {})
        if tgt is not None:
            def num(x):
                return f"{x:.5g}" if isinstance(x, (int, float)) else "—"
            cb = base.get("classical_best", {})
            ob = base.get("optimizer", {})
            rows = (f"<tr><td>baseline</td><td>{num(base.get('initial', {}).get('fom'))}</td></tr>"
                    f"<tr><td><strong>target (agent must beat)</strong></td><td><strong>{num(tgt)}</strong></td></tr>"
                    f"<tr><td>classical best ({esc(cb.get('source', '?'))}, constraint-filtered)</td>"
                    f"<td>{num(cb.get('fom'))}</td></tr>"
                    f"<tr><td>mcrun optimizer ({ob.get('iterations')} iters)</td>"
                    f"<td>{num(ob.get('fom'))}{'' if ob.get('fom') else ' <span class=meta>(' + esc((ob.get('note') or '')[:60]) + ')</span>'}</td></tr>"
                    f"<tr><td>constrained random search ({base.get('random_search', {}).get('samples')} samples)</td>"
                    f"<td>{num(base.get('random_search', {}).get('fom'))}</td></tr>")
            parts.append(f"<table><tr><th>FOM ({esc(t['fom']['monitor'])} intensity)</th>"
                         f"<th>n/s</th></tr>{rows}</table>")
        for c in t.get("constraints", []):
            if c.get("max_value") is not None:
                parts.append(f'<p class="meta">constraint: <code>{esc(c["role"])}.'
                             f'{esc(c["observable"])}</code> ≤ {c["max_value"]:.4g} '
                             f'({c["max_factor_of_baseline"]}× baseline)</p>')
    if t.get("kind") == "open_design":
        floors = t.get("grading", {}).get("automatic_floors", [])
        if floors:
            parts.append("<p class=\"meta\">automatic floors: "
                         + "; ".join(esc(f["description"]) for f in floors) + "</p>")
        parts.append(f'<p class="meta">⚠ {esc(t.get("grading", {}).get("expert_rubric", ""))}</p>')
    if t.get("kind") == "memorization_probe":
        parts.append(f'<p>{esc(t.get("notes", ""))}</p>')
    prompt = t.get("prompt")
    if prompt:
        parts.append(f'<details><summary>task prompt ({len(prompt)} chars)</summary>'
                     f'<pre>{esc(prompt)}</pre></details>')
    parts.append(f'<p class="meta">file: <code>{esc(t["_path"])}</code></p>')
    parts.append("</div>")
    return "\n".join(parts)


def main():
    tasks, val, inv = load_all()
    t1 = [t for t in tasks if t["tier"] == "T1" and t["id"].startswith("T1_")]
    t2 = [t for t in tasks if t["tier"] == "T2"]
    t3 = [t for t in tasks if t["tier"] == "T3"]
    pilots = [t for t in tasks if t["id"].startswith("P")]
    classes = {}
    for t in t1:
        classes[t.get("class", "?")] = classes.get(t.get("class", "?"), 0) + 1
    now = datetime.now().strftime("%Y-%m-%d %H:%M")

    rows = ""
    for t in sorted(t1, key=lambda x: (x.get("split", ""), x["id"])):
        name = ref_name(t)
        e = inv.get(name, {})
        v = val.get(t["id"], {})
        link = gh_link(e)
        paper = t.get("paper") or {}
        doi = paper.get("doi")
        rows += (
            f'<tr><td><a href="#{esc(t["id"])}"><code>{esc(t["id"])}</code></a></td>'
            f'<td>{esc(t.get("class", ""))}</td>'
            f'<td>{esc(t.get("split", ""))}</td>'
            f'<td>{"✅" if v.get("pass") else "—"}</td>'
            f'<td>{"✅" if e.get("example_check", {}).get("agree") else "—"}</td>'
            f'<td>{f"<a href={chr(34)}{esc(link)}{chr(34)}>source</a>" if link else "—"}</td>'
            f'<td>{f"<a href={chr(34)}https://doi.org/{esc(doi)}{chr(34)}>doi</a>" if doi else "—"}</td></tr>')

    cards = "\n".join(task_card(t, val, inv) for t in
                      sorted(t1, key=lambda x: (x.get("split", ""), x["id"])))
    pilot_cards = "\n".join(task_card(t, val, inv) for t in pilots)
    t2_cards = "\n".join(task_card(t, val, inv) for t in sorted(t2, key=lambda x: x["id"]))
    t3_cards = "\n".join(task_card(t, val, inv) for t in sorted(t3, key=lambda x: x["id"]))
    class_chips = " · ".join(f"{k}: {n}" for k, n in sorted(classes.items()))

    page = f"""<!DOCTYPE html>
<html lang="en"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>McStasBench — Task Catalog</title>
<style>
  :root {{ color-scheme: light dark; }}
  body {{ font: 15px/1.55 -apple-system, BlinkMacSystemFont, "Segoe UI", sans-serif;
         margin: 0; background: #f6f8fa; color: #1f2328; }}
  @media (prefers-color-scheme: dark) {{
    body {{ background: #0d1117; color: #e6edf3; }}
    .card {{ background: #161b22 !important; border-color: #30363d !important; }}
    pre, code, th {{ background: #21262d !important; }}
  }}
  .wrap {{ max-width: 1020px; margin: 0 auto; padding: 32px 20px 60px; }}
  h1 {{ font-size: 26px; margin: 0 0 4px; }}
  h2 {{ font-size: 19px; margin: 30px 0 10px; }}
  h3 {{ margin: 0 0 6px; font-size: 15px; }}
  .meta {{ color: {GRAY}; font-size: 13px; margin: 3px 0; }}
  .card {{ background: #fff; border: 1px solid #d0d7de; border-radius: 10px;
          padding: 14px 18px; margin: 12px 0; }}
  table {{ border-collapse: collapse; width: 100%; font-size: 13px; margin: 8px 0; }}
  th, td {{ text-align: left; padding: 5px 9px; border-bottom: 1px solid #d0d7de55; }}
  th {{ background: #f6f8fa; font-size: 12px; }}
  code {{ background: #f0f1f3; padding: 1px 5px; border-radius: 4px; font-size: 12.5px; }}
  pre {{ background: #f0f1f3; border-radius: 8px; padding: 12px; overflow-x: auto;
        font-size: 12px; line-height: 1.45; white-space: pre-wrap; }}
  details summary {{ cursor: pointer; color: {BLUE}; font-size: 13px; margin: 6px 0; }}
  a {{ color: {BLUE}; }}
</style></head><body><div class="wrap">
  <h1>McStasBench — Task Catalog</h1>
  <p class="meta">Generated {now} · {len(t1)} T1 + {len(t2)} T2 + {len(t3)} T3 tasks + {len(pilots)} pilot/control ·
    <a href="../progress.html">progress</a> · <a href="../guide.html">how the agent works</a></p>

  <h2>Task naming: P vs T</h2>
  <div class="card">
  <p><strong>P tasks (pilot &amp; control)</strong> were built first, to validate the
  grading machinery itself (de-risk gate 4) — they are not scored benchmark
  items. <strong>P1</strong>: hand-authored full-spec SANS reproduction, the prototype
  the T1 generator is modeled on. <strong>P3</strong>: the same physics deliberately
  under-specified — a control proving the rubric grades only what a task
  specifies and never punishes legitimate design choices. <strong>P2</strong>: the
  memorization probe — not a reproduction task but a contamination-control
  mechanism, run per evaluated model (a reference counts as "unseen" for a
  model only if that model fails to emit the file from memory).</p>
  <p><strong>T tasks (benchmark tiers)</strong> are the scored benchmark. The number is
  the tier, mapping to the project's research questions: <strong>T1</strong> reproduce an
  instrument from its specification (this page), <strong>T2</strong> improve a design against
  calibrated quantitative targets (on this page — targets set by classical
  optimization, hardened by red-teaming), <strong>T3</strong> open-ended design
  (definitions below; expert rubric pending).
  T1 tasks are auto-authored at scale from machine-verified references and
  each is self-validated before admission.</p>
  </div>

  <h2>Where tasks come from</h2>
  <div class="card">
  <p><strong>1 — Raw material:</strong> the ~297 example instruments shipped inside
  <a href="https://www.mcstas.org/">McStas</a> itself
  (<a href="{GH}">browse them on GitHub</a>) — written over ~25 years by facility
  instrument scientists; many are validated models of real instruments, each
  carrying a <code>%Example:</code> self-test line with an expected detector value.</p>
  <p><strong>2 — Curation:</strong> literature study links models to their
  instrument papers (DOIs verified; see
  <code>note/study-instrument-papers-2026-07-09.md</code>,
  <code>note/study-paper-pairs-2026-07-24.md</code>); a machine-verification
  sweep (<code>benchmark/build_inventory.py</code>) confirms each reference
  compiles, runs, and reproduces its <code>%Example</code> value in this
  environment.</p>
  <p><strong>3 — Manufacturing:</strong> <code>benchmark/author_tasks.py</code>
  converts each reference <code>.instr</code> into a natural-language spec-sheet
  prompt and derives the grading contract from the reference's own monitors.
  Every task is then self-validated: the reference must pass its own task at a
  fresh random seed. Grading is observable-based — no LLM judge.</p>
  <p class="meta">Seen-tier references are public by construction (that's the
  tier's definition); the memorization probe (P2) and the future held-out tier
  (2024–26 instruments with no public model) control for contamination.</p>
  </div>

  <h2>T1 reproduction tasks ({len(t1)}) <span class="meta">{class_chips}</span></h2>
  <table>
    <tr><th>task</th><th>class</th><th>split</th><th>self-validated</th>
        <th>ref verified in-env</th><th>reference</th><th>paper</th></tr>
    {rows}
  </table>
  {cards}

  <h2>T2 improvement tasks ({len(t2)})</h2>
  <p class="meta">Improve a baseline instrument against calibrated targets under a
  multi-objective contract. Targets are set from what the classical optimizer
  (mcrun --optimize) achieves — achievable-but-not-trivial by construction —
  and random search under matched compute provides the discrimination floor.</p>
  {t2_cards}

  <h2>T3 open-design tasks ({len(t3)}) <span class="meta">definitions — expert rubric pending (M6/M7)</span></h2>
  <p class="meta">Goal-only specifications. Automatic floors (compiles, statistics,
  geometry, Liouville sanity) are machine-checkable; design-quality grading
  requires the expert rubric and these tasks are NOT yet in the scored set.</p>
  {t3_cards}

  <h2>Pilot &amp; control tasks ({len(pilots)})</h2>
  {pilot_cards}

  <footer class="meta" style="margin-top:28px">
    Regenerate: <code>python3 scripts/tasks_report.py</code> · tasks live in
    <code>benchmark/tasks/</code> · grading: <code>benchmark/grader.py</code>
  </footer>
</div></body></html>
"""
    with open(OUT, "w") as f:
        f.write(page)
    print(f"wrote benchmark/tasks.html: {len(t1)} T1 + {len(t2)} T2 + {len(t3)} T3 + {len(pilots)} pilot")


if __name__ == "__main__":
    main()
