"""Generate progress.html — a human-friendly dashboard — from PLAN.md.

PLAN.md stays the single source of truth; this just renders it. Conventions
it relies on:
  - the quick-sync recap is the bullets under "## Recap" (rendered as the
    top card; the heading's parenthetical carries the as-of date)
  - milestones are "### M<n> — Title" headings under "## Milestones";
    a completed milestone has "DONE" (with the checkmark emoji) in its heading
  - checklist items are "- [ ]" / "- [x]" bullets
  - the acceptance criterion bullet starts with "- **Accept"
  - de-risk gates are the table under "## De-risk gates"; a gate counts as
    passed when the text "de-risk gate <n> passed" appears anywhere in PLAN.md
  - standing decisions are the bullets under "## Standing decisions"

Usage: python scripts/progress_report.py   (any python3; stdlib only)
"""

import html
import os
import re
import subprocess
from datetime import datetime

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PLAN = os.path.join(REPO, "PLAN.md")
OUT = os.path.join(REPO, "progress.html")

GREEN, AMBER, GRAY = "#2da44e", "#bf8700", "#8b949e"


def md_inline(text):
    """Escape HTML, then render the little markdown we use inline."""
    text = html.escape(text)
    text = re.sub(r"\[([^\]]+)\]\(([^)]+)\)", r'<a href="\2">\1</a>', text)
    text = re.sub(r"\*\*([^*]+)\*\*", r"<strong>\1</strong>", text)
    text = re.sub(r"`([^`]+)`", r"<code>\1</code>", text)
    return text


def parse_plan(text):
    lines = text.splitlines()
    milestones, gates, decisions = [], [], []
    recap = {"asof": "", "items": []}
    section = None
    current = None
    in_code = False

    for line in lines:
        if line.startswith("```"):
            in_code = not in_code
            continue
        if in_code:
            continue

        if line.startswith("## "):
            section = line[3:].strip()
            current = None
            if section.startswith("Recap"):
                m = re.search(r"as of ([0-9-]+)", section)
                recap["asof"] = m.group(1) if m else ""
            continue

        if section and section.startswith("Recap"):
            m = re.match(r"^- (.+)$", line)
            if m:
                recap["items"].append(m.group(1))

        elif section and section.startswith("Milestones"):
            m = re.match(r"^### (M\d+(?:\.\d+)?) — (.+)$", line)
            if m:
                title = m.group(2)
                done = "✅" in title or re.search(r"\bDONE\b", title)
                title = re.sub(r"\s*✅?\s*DONE\s*", " — done ", title).strip(" —")
                current = {
                    "id": m.group(1),
                    "title": title,
                    "done": bool(done),
                    "items": [],
                    "accept": None,
                    "notes": [],
                }
                milestones.append(current)
                continue
            if current is None:
                continue
            m = re.match(r"^- \[([ x])\] (.+)$", line)
            if m:
                current["items"].append((m.group(1) == "x", m.group(2)))
                continue
            m = re.match(r"^- (\*\*Accept.*)$", line)
            if m:
                current["accept"] = m.group(1)
                continue
            m = re.match(r"^- (?!\[)(.+)$", line)
            if m:
                current["notes"].append(m.group(1))

        elif section and section.startswith("De-risk gates"):
            cells = [c.strip() for c in line.strip().strip("|").split("|")]
            if len(cells) >= 4 and re.match(r"^\d+$", cells[0]):
                gates.append(
                    {"n": cells[0], "gate": cells[1], "when": cells[2], "kill": cells[3]}
                )

        elif section and section.startswith("Standing decisions"):
            m = re.match(r"^- (.+)$", line)
            if m:
                decisions.append(m.group(1))

    passed = {
        m for m in re.findall(r"de-risk gate (\d+) passed", text, flags=re.IGNORECASE)
    }
    for g in gates:
        g["passed"] = g["n"] in passed
    return milestones, gates, decisions, recap


def git_info():
    try:
        sha = subprocess.run(
            ["git", "rev-parse", "--short", "HEAD"],
            capture_output=True, text=True, cwd=REPO,
        ).stdout.strip()
        date = subprocess.run(
            ["git", "log", "-1", "--format=%cd", "--date=short"],
            capture_output=True, text=True, cwd=REPO,
        ).stdout.strip()
        return f"{sha} ({date})" if sha else "no commits"
    except OSError:
        return "git unavailable"


def status_of(ms):
    if ms["done"]:
        return "done", GREEN, "done"
    if any(ok for ok, _ in ms["items"]):
        return "in progress", AMBER, "active"
    return "pending", GRAY, "pending"


def render(milestones, gates, decisions, recap):
    total = sum(len(m["items"]) for m in milestones)
    checked = sum(1 for m in milestones for ok, _ in m["items"] if ok)
    pct = round(100 * checked / total) if total else 0
    now = datetime.now().strftime("%Y-%m-%d %H:%M")

    chips = "".join(
        f'<span class="chip {status_of(m)[2]}" title="{html.escape(m["title"])}">{m["id"]}</span>'
        for m in milestones
    )

    cards = []
    for ms in milestones:
        label, color, cls = status_of(ms)
        n_done = sum(1 for ok, _ in ms["items"] if ok)
        items = "".join(
            f'<li class="{"ok" if ok else ""}"><span class="tick">'
            f'{"✓" if ok else "○"}</span>{md_inline(t)}</li>'
            for ok, t in ms["items"]
        )
        accept = (
            f'<div class="accept">{md_inline(ms["accept"])}</div>' if ms["accept"] else ""
        )
        notes = "".join(f'<div class="note">{md_inline(n)}</div>' for n in ms["notes"])
        cards.append(f"""
      <div class="card {cls}">
        <div class="card-head">
          <span class="mid">{ms["id"]}</span>
          <span class="badge" style="background:{color}">{label}</span>
          <span class="count">{n_done}/{len(ms["items"])}</span>
        </div>
        <h3>{md_inline(ms["title"])}</h3>
        <ul class="checklist">{items}</ul>
        {accept}{notes}
      </div>""")

    gate_rows = "".join(
        f"<tr><td>{'✅' if g['passed'] else '⏳'}</td><td>{g['n']}</td>"
        f"<td>{md_inline(g['gate'])}</td><td>{md_inline(g['when'])}</td>"
        f"<td>{md_inline(g['kill'])}</td></tr>"
        for g in gates
    )
    decision_items = "".join(f"<li>{md_inline(d)}</li>" for d in decisions)

    workflow_html = """
  <h2>How the benchmark works</h2>
  <div class="flow">
    <div class="lane">
      <div class="lane-title">Reference side · no LLM anywhere</div>
      <div class="step">shipped / curated reference <code>.instr</code><br>
        <span class="dim">(McStas example library, <code>benchmark/instruments/</code>)</span></div>
      <div class="arrow">&darr;</div>
      <div class="step"><code>build_inventory.py</code> — runs each candidate: eligible? &rarr; <code>inventory.json</code></div>
      <div class="arrow">&darr;</div>
      <div class="step"><code>author_tasks.py</code> — NL spec sheet + grading contract;
        <code>validate_tasks.py</code> — reference passes its own task at a fresh seed</div>
      <div class="arrow">&darr;</div>
      <div class="step"><code>refcache/</code> — reference observables, run <strong>once</strong> at the task protocol</div>
    </div>
    <div class="lane">
      <div class="lane-title">Agent side · the only LLM</div>
      <div class="step">task prompt = <strong>NL spec sheet only</strong><br>
        <span class="dim">never the reference file or its parameters</span></div>
      <div class="arrow">&darr;</div>
      <div class="step">agent in the <strong>NeutronGym reference loop</strong> ± skill
        <span class="dim">(Claude Code = comparison arm; sandboxed — no example tools, no Read)</span></div>
      <div class="arrow">&darr;</div>
      <div class="step">MCP server: discover components, construct + validate &rarr; the agent's <strong>own</strong> <code>.instr</code></div>
      <div class="arrow">&darr;</div>
      <div class="step">iterate: <code>run_simulation</code> at low ncount &rarr; <code>get_results</code></div>
    </div>
    <div class="lane">
      <div class="lane-title">Grading · headless, programmatic</div>
      <div class="step">harness re-runs the agent's <code>.instr</code> at the <strong>env-controlled protocol</strong>
        <span class="dim">(fixed ncount + seed — never the agent's own runs)</span></div>
      <div class="arrow">&darr;</div>
      <div class="step"><code>grader.py</code> — per monitor role, per observable:<br>
        <code>|cand &minus; ref| &le; max(rtol&middot;|ref|, n&sigma;&middot;errors)</code></div>
      <div class="arrow">&darr;</div>
      <div class="step"><code>report.json</code> — pass/score, per-check deltas, deepest level reached (L1&ndash;L4), leak-audit flag</div>
      <div class="arrow">&darr;</div>
      <div class="step"><code>artifacts/</code> — candidate <code>.instr</code> + params, diagram PNG, real-scale webgl trace
        <span class="dim">(harness-rendered post-episode; side-by-side with the reference visuals in <code>runs/refviz/</code>)</span></div>
    </div>
  </div>
  <div class="flow-note">T2 (improve) tasks skip the reference comparison: the agent is <em>given</em> a
    baseline + quantitative targets and <code>grade_improvement</code> checks its figure-of-merit against the
    pre-calibrated, fresh-seed-verified classical best with constraint bands. No LLM judge in either path.</div>
  <div class="gates ladder">
    <div class="lane-title">Simulation budget ladder — ncount per stage</div>
    <table>
      <tr><th>Stage</th><th>ncount</th><th>Role</th></tr>
      <tr><td>Fast-tier RL rollouts</td><td>1e5</td><td>~0.04 s/rollout via direct binary execution — dense training signal</td></tr>
      <tr><td>Curation sweep (<code>build_inventory.py</code>)</td><td>1e5</td><td>cheap eligibility check across candidate references</td></tr>
      <tr><td>Agent iteration inside an episode</td><td>agent's choice, ~1e5&ndash;1e6 (cap 1e8)</td><td>design-loop feedback — never graded</td></tr>
      <tr><td><strong>Grading protocol (every task, both sides)</strong></td><td><strong>1e6, fixed seed</strong></td><td>reference and candidate measured identically; ~0.1% relative error on well-lit monitors; &lt;1,000-event monitors hard-fail as ungradable</td></tr>
      <tr><td>Final validation of headline claims</td><td>&ge;1e8, fresh seed</td><td>production bar — T2 improvements must survive fresh-seed re-verification</td></tr>
    </table>
    <div class="flow-note">Weak-signal roles on the largest spectrometers (IN5, LET) get per-task higher-stat
      protocols instead of a global ncount bump (planned, grow-T1 work).</div>
  </div>
"""

    recap_html = ""
    if recap["items"]:
        recap_items = "".join(f"<li>{md_inline(r)}</li>" for r in recap["items"])
        asof = f' <span class="asof">as of {recap["asof"]}</span>' if recap["asof"] else ""
        recap_html = f"""
  <div class="recap">
    <div class="recap-head">Quick sync{asof}</div>
    <ul>{recap_items}</ul>
  </div>"""

    return f"""<!DOCTYPE html>
<html lang="en"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>NeutronGym — Progress</title>
<style>
  :root {{ color-scheme: light dark; }}
  * {{ box-sizing: border-box; }}
  body {{ font: 15px/1.5 -apple-system, BlinkMacSystemFont, "Segoe UI", sans-serif;
         margin: 0; background: #f6f8fa; color: #1f2328; }}
  @media (prefers-color-scheme: dark) {{
    body {{ background: #0d1117; color: #e6edf3; }}
    .card, .gates, .decisions, .lane {{ background: #161b22 !important; border-color: #30363d !important; }}
    .flow .step {{ background: #1c2128 !important; border-color: #30363d !important; }}
    .accept {{ background: #1c2128 !important; }}
    th {{ background: #21262d !important; }}
    code {{ background: #21262d !important; }}
  }}
  .wrap {{ max-width: 1080px; margin: 0 auto; padding: 32px 20px 60px; }}
  h1 {{ margin: 0 0 4px; font-size: 26px; }}
  .meta {{ color: #656d76; font-size: 13px; margin-bottom: 18px; }}
  .overall {{ background: #d0d7de55; border-radius: 8px; height: 14px; overflow: hidden; margin: 10px 0 6px; }}
  .overall > div {{ height: 100%; background: {GREEN}; width: {pct}%; transition: width .3s; }}
  .chips {{ margin: 10px 0 26px; }}
  .chip {{ display: inline-block; padding: 3px 10px; border-radius: 999px; font-size: 12px;
           font-weight: 600; margin-right: 6px; color: #fff; }}
  .chip.done {{ background: {GREEN}; }} .chip.active {{ background: {AMBER}; }}
  .chip.pending {{ background: {GRAY}; }}
  .grid {{ display: grid; grid-template-columns: repeat(auto-fill, minmax(320px, 1fr)); gap: 16px; }}
  .card {{ background: #fff; border: 1px solid #d0d7de; border-radius: 10px; padding: 16px 18px; }}
  .card.done {{ border-left: 4px solid {GREEN}; }}
  .card.active {{ border-left: 4px solid {AMBER}; }}
  .card.pending {{ border-left: 4px solid #d0d7de; }}
  .card-head {{ display: flex; align-items: center; gap: 8px; margin-bottom: 6px; }}
  .mid {{ font-weight: 700; font-size: 13px; color: #656d76; }}
  .badge {{ color: #fff; font-size: 11px; font-weight: 600; padding: 2px 8px; border-radius: 999px; }}
  .count {{ margin-left: auto; font-size: 12px; color: #656d76; }}
  h3 {{ margin: 0 0 10px; font-size: 15px; }}
  .checklist {{ list-style: none; margin: 0; padding: 0; font-size: 13.5px; }}
  .checklist li {{ padding: 3px 0 3px 24px; position: relative; }}
  .checklist li.ok {{ color: #656d76; }}
  .tick {{ position: absolute; left: 2px; font-weight: 700; }}
  li.ok .tick {{ color: {GREEN}; }}
  .accept {{ margin-top: 10px; padding: 8px 10px; background: #f6f8fa; border-radius: 6px;
             font-size: 12.5px; }}
  .note {{ margin-top: 8px; font-size: 12.5px; color: #656d76; }}
  .gates, .decisions {{ background: #fff; border: 1px solid #d0d7de; border-radius: 10px;
                        padding: 16px 18px; margin-top: 26px; }}
  .recap {{ background: #fff; border: 1px solid #d0d7de; border-left: 4px solid #0969da;
            border-radius: 10px; padding: 14px 18px; margin: 4px 0 18px; }}
  .recap-head {{ font-weight: 700; font-size: 13px; text-transform: uppercase;
                 letter-spacing: .04em; color: #0969da; margin-bottom: 8px; }}
  .recap .asof {{ font-weight: 400; text-transform: none; letter-spacing: 0; color: #656d76; }}
  .recap ul {{ margin: 0; padding-left: 20px; font-size: 13.5px; }}
  .recap li {{ margin: 4px 0; }}
  .flow {{ display: grid; grid-template-columns: repeat(auto-fit, minmax(280px, 1fr)); gap: 14px; }}
  .lane {{ background: #fff; border: 1px solid #d0d7de; border-radius: 10px; padding: 14px; }}
  .lane-title {{ font-weight: 700; font-size: 12px; text-transform: uppercase;
                 letter-spacing: .04em; color: #656d76; margin-bottom: 10px; }}
  .flow .step {{ background: #f6f8fa; border: 1px solid #d0d7de88; border-radius: 8px;
                 padding: 8px 10px; font-size: 12.5px; }}
  .flow .arrow {{ text-align: center; color: #656d76; font-size: 14px; line-height: 1.4; }}
  .flow .dim {{ color: #656d76; }}
  .flow-note {{ margin-top: 12px; font-size: 12.5px; color: #656d76; }}
  h2 {{ font-size: 18px; margin: 26px 0 12px; }}
  table {{ border-collapse: collapse; width: 100%; font-size: 13.5px; }}
  th, td {{ text-align: left; padding: 6px 10px; border-bottom: 1px solid #d0d7de55; vertical-align: top; }}
  th {{ background: #f6f8fa; font-size: 12px; }}
  code {{ background: #f0f1f3; padding: 1px 5px; border-radius: 4px; font-size: 12.5px; }}
  a {{ color: #0969da; }}
  footer {{ margin-top: 34px; font-size: 12.5px; color: #656d76; }}
</style></head><body><div class="wrap">
  <h1>NeutronGym — Progress</h1>
  <div class="meta">the executable RL environment for neutron instrument design · McStasBench is its held-out benchmark slice</div>
  <div class="meta">Generated {now} · commit {git_info()} · source of truth: <code>PLAN.md</code>
   · <a href="guide.html">how the agent drives McStas &rarr;</a></div>
  {recap_html}
  <div class="overall"><div></div></div>
  <div class="meta"><strong>{checked}/{total}</strong> checklist items complete ({pct}%)</div>
  <div class="chips">{chips}</div>
  <div class="grid">{"".join(cards)}
  </div>
  {workflow_html}
  <h2>De-risk gates</h2>
  <div class="gates"><table>
    <tr><th></th><th>#</th><th>Gate</th><th>When</th><th>Kills the plan if</th></tr>
    {gate_rows}
  </table></div>
  <h2>Standing decisions</h2>
  <div class="decisions"><ul>{decision_items}</ul></div>
  <footer>Regenerate after editing PLAN.md: <code>python scripts/progress_report.py</code>
  &nbsp;·&nbsp; view: <code>open progress.html</code></footer>
</div></body></html>
"""


def main():
    with open(PLAN) as f:
        text = f.read()
    milestones, gates, decisions, recap = parse_plan(text)
    if not milestones:
        raise SystemExit("no milestones parsed from PLAN.md — heading format changed?")
    with open(OUT, "w") as f:
        f.write(render(milestones, gates, decisions, recap))
    total = sum(len(m["items"]) for m in milestones)
    done = sum(1 for m in milestones for ok, _ in m["items"] if ok)
    print(f"wrote {os.path.relpath(OUT, REPO)}: {len(milestones)} milestones, "
          f"{done}/{total} items done, {sum(g['passed'] for g in gates)}/{len(gates)} gates passed")


if __name__ == "__main__":
    main()
