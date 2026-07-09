"""Generate progress.html — a human-friendly dashboard — from PLAN.md.

PLAN.md stays the single source of truth; this just renders it. Conventions
it relies on:
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
            continue

        if section and section.startswith("Milestones"):
            m = re.match(r"^### (M\d+) — (.+)$", line)
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
    return milestones, gates, decisions


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


def render(milestones, gates, decisions):
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

    return f"""<!DOCTYPE html>
<html lang="en"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>McStasBench — Progress</title>
<style>
  :root {{ color-scheme: light dark; }}
  * {{ box-sizing: border-box; }}
  body {{ font: 15px/1.5 -apple-system, BlinkMacSystemFont, "Segoe UI", sans-serif;
         margin: 0; background: #f6f8fa; color: #1f2328; }}
  @media (prefers-color-scheme: dark) {{
    body {{ background: #0d1117; color: #e6edf3; }}
    .card, .gates, .decisions {{ background: #161b22 !important; border-color: #30363d !important; }}
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
  h2 {{ font-size: 18px; margin: 26px 0 12px; }}
  table {{ border-collapse: collapse; width: 100%; font-size: 13.5px; }}
  th, td {{ text-align: left; padding: 6px 10px; border-bottom: 1px solid #d0d7de55; vertical-align: top; }}
  th {{ background: #f6f8fa; font-size: 12px; }}
  code {{ background: #f0f1f3; padding: 1px 5px; border-radius: 4px; font-size: 12.5px; }}
  a {{ color: #0969da; }}
  footer {{ margin-top: 34px; font-size: 12.5px; color: #656d76; }}
</style></head><body><div class="wrap">
  <h1>McStasBench — Progress</h1>
  <div class="meta">Generated {now} · commit {git_info()} · source of truth: <code>PLAN.md</code>
   · <a href="guide.html">how the agent drives McStas &rarr;</a></div>
  <div class="overall"><div></div></div>
  <div class="meta"><strong>{checked}/{total}</strong> checklist items complete ({pct}%)</div>
  <div class="chips">{chips}</div>
  <div class="grid">{"".join(cards)}
  </div>
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
    milestones, gates, decisions = parse_plan(text)
    if not milestones:
        raise SystemExit("no milestones parsed from PLAN.md — heading format changed?")
    with open(OUT, "w") as f:
        f.write(render(milestones, gates, decisions))
    total = sum(len(m["items"]) for m in milestones)
    done = sum(1 for m in milestones for ok, _ in m["items"] if ok)
    print(f"wrote {os.path.relpath(OUT, REPO)}: {len(milestones)} milestones, "
          f"{done}/{total} items done, {sum(g['passed'] for g in gates)}/{len(gates)} gates passed")


if __name__ == "__main__":
    main()
