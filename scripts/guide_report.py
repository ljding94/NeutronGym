"""Generate guide.html — how the agent drives McStas through MCP.

The tool reference is introspected live from the running FastMCP server
(names, docstrings, parameter schemas), so it cannot drift from the code.
Regenerate after changing server tools:

    conda run -n mcstas python scripts/guide_report.py

(needs the mcstas env, unlike progress_report.py)
"""

import asyncio
import base64
import html
import json
import os
from datetime import datetime

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(REPO, "guide.html")
GREEN, BLUE, GRAY = "#2da44e", "#0969da", "#656d76"


def get_tools():
    from fastmcp import Client
    from mcstas_mcp.server import mcp

    async def _run():
        async with Client(mcp) as client:
            return await client.list_tools()

    return asyncio.run(_run())


def esc(s):
    return html.escape(str(s))


def render_tool(tool):
    schema = tool.inputSchema or {}
    props = schema.get("properties", {})
    required = set(schema.get("required", []))
    rows = ""
    for pname, p in props.items():
        ptype = p.get("type") or "/".join(
            x.get("type", "?") for x in p.get("anyOf", []) if x.get("type") != "null"
        ) or "any"
        default = p.get("default", "—" if pname in required else "")
        req = "yes" if pname in required else ""
        rows += (f"<tr><td><code>{esc(pname)}</code></td><td>{esc(ptype)}</td>"
                 f"<td>{esc(req)}</td><td><code>{esc(default)}</code></td></tr>")
    params_table = (
        f"<table><tr><th>parameter</th><th>type</th><th>required</th><th>default</th></tr>{rows}</table>"
        if rows else "<p class='muted'>no parameters</p>"
    )
    return f"""
    <div class="tool" id="tool-{esc(tool.name)}">
      <h4><code>{esc(tool.name)}</code></h4>
      <p>{esc(tool.description or '')}</p>
      {params_table}
    </div>"""


def embed_png(relpath, caption):
    path = os.path.join(REPO, relpath)
    if not os.path.isfile(path):
        return (f"<p class='muted'>[{esc(relpath)} not found — run "
                "<code>conda run -n mcstas python scripts/m1_walkthrough.py</code> "
                "and regenerate this page]</p>")
    b64 = base64.b64encode(open(path, "rb").read()).decode()
    return (f"<figure><img src='data:image/png;base64,{b64}' alt='{esc(caption)}'>"
            f"<figcaption>{caption}</figcaption></figure>")


CODE_EXAMPLES = """
<h3>1 — Discover &amp; introspect (ground the agent, no hallucinated components)</h3>
<pre>list_components(category="optics", search="guide")
  → {"ok": true, "count": 14, "components": [{"name": "Guide", "category": "optics",
     "doc": "Models a rectangular guide tube centered on the Z axis..."}, ...]}

describe_component(name="Guide")
  → {"ok": true, "parameters": [
      {"name": "w1", "type": "double", "unit": "m", "required": true,
       "doc": "Width at the guide entry"}, ...]}</pre>

<h3>2 — Build the instrument (validated at every call)</h3>
<pre>create_instrument(name="demo_guide")
add_parameter(instrument_id="demo_guide", name="wl", default=5.0, unit="AA")
add_component(instrument_id="demo_guide", name="source", component="Source_simple",
              at=[0,0,0], parameters={"lambda0": "wl", "dlambda": "0.5*wl", ...})
add_component(instrument_id="demo_guide", name="guide", component="Guide",
              at=[0,0,1.5], relative="source",
              parameters={"w1": 0.03, "h1": 0.05, "l": 10.0, "m": 2.0, ...})</pre>
<p>Mistakes come back immediately with the fix in the message — this is the server's
core job (fail at tool-call time, not compile time):</p>
<pre>add_component(..., component="PSD_monitr", ...)
  → {"ok": false, "error": "No component type 'PSD_monitr' in the McStas library.
     Nearest matches: PSD_monitor, PSND_monitor, PSD_monitor_4PI. ..."}

set_parameters(..., parameters={"lambda0": "undeclared_var"})
  → {"ok": false, "error": "Parameter 'lambda0' of 'Source_simple' references
     unknown identifier(s) ['undeclared_var'] ... define instrument parameter(s)
     with add_parameter."}</pre>

<h3>3 — Run (compile + simulate, seconds at iteration scale)</h3>
<pre>run_simulation(instrument_id="demo_guide", ncount=1e6, seed=42)
  → {"ok": true, "job_id": "demo_guide_20260709_175054_59", "elapsed_s": 2.2,
     "detectors": [{"name": "psd", "I": "0.0302...", "err": ..., "N": ...}, ...]}</pre>

<h3>4 — Inspect results (summary statistics, never raw arrays)</h3>
<pre>get_results(job_id="demo_guide_20260709_175054_59")
  → {"ok": true, "monitors": [
      {"component": "psd", "intensity": 0.03026, "intensity_err": 4.5e-05,
       "events": 783625, "relative_err": 0.0015,
       "beam_center": {"X0": -0.0004, "Y0": -0.003},
       "beam_width": {"dX": 0.586, "dY": 1.16}, ...}],
     "low_statistics": []}

get_monitor_data(job_id="...", monitor="psd", format="png")
  → {"ok": true, "png_path": "/.../psd.png"}   # agent views it with the Read tool</pre>
"""

RESULTS_GUIDE = f"""
<table>
<tr><th>field</th><th>meaning</th><th>how to use it</th></tr>
<tr><td><code>intensity</code></td><td>integrated neutron rate on the monitor [n/s]
    at nominal source power</td><td>the headline number ("flux at detector").
    Independent of ncount — more rays only reduce the error.</td></tr>
<tr><td><code>intensity_err</code></td><td>1&sigma; statistical error on intensity</td>
    <td>error shrinks ~1/&radic;ncount; quote I &plusmn; err</td></tr>
<tr><td><code>events</code></td><td>Monte-Carlo rays that reached this monitor</td>
    <td><strong>quality gate: below ~1000 events, numbers are noise</strong> —
    such monitors are listed in <code>low_statistics</code>; rerun with higher ncount</td></tr>
<tr><td><code>relative_err</code></td><td>err / intensity</td>
    <td>&lt;1% = solid; &gt;10% = do not compare designs on this</td></tr>
<tr><td><code>beam_center</code> (X0, Y0)</td><td>1st moment of the distribution, in the
    monitor's axis units (cm for PSD, &Aring; for wavelength monitors, ...)</td>
    <td>is the beam where it should be? A drifting X0 = misalignment</td></tr>
<tr><td><code>beam_width</code> (dX, dY)</td><td>2nd moment (RMS width)</td>
    <td>beam size / bandwidth; compare against slit-guide-sample dimensions</td></tr>
<tr><td><code>signal</code> (Min/Max/Mean)</td><td>per-bin statistics</td>
    <td>quick uniformity check; Max&asymp;Mean = flat, Max&Gt;Mean = peaked</td></tr>
</table>

<h3>Reproducibility</h3>
<p>Every run records <code>ncount</code>, <code>seed</code>, and all parameter values
(in the job record and inside <code>mccode.sim</code>). Same seed + same ncount =
bit-identical results. The benchmark relies on this.</p>

<h3>Where things live on disk</h3>
<table>
<tr><th>path</th><th>content</th></tr>
<tr><td><code>~/.mcstas-mcp/instruments/&lt;name&gt;/</code></td>
    <td>spec.json (source of truth), generated .instr, one output dir per job</td></tr>
<tr><td><code>~/.mcstas-mcp/jobs.json</code></td><td>job records (survive restarts)</td></tr>
<tr><td><code>&lt;job dir&gt;/mccode.sim</code></td><td>run metadata + per-monitor summary lines</td></tr>
<tr><td><code>&lt;job dir&gt;/*.dat</code></td><td>full histograms (text; header + I/err/N)</td></tr>
</table>

<h3>Look at it yourself (humans)</h3>
<pre>conda activate mcstas
python scripts/view_instrument.py &lt;path/to&gt;.instr            # interactive 3D geometry
python scripts/view_instrument.py &lt;path/to&gt;.instr --diagram  # component schematic
python scripts/m1_walkthrough.py                             # full server demo, end to end
mcplot &lt;job output dir&gt;                                      # all monitors, native plotter</pre>

<h3>Worked example — reading the demo monitors</h3>
{embed_png("runs/m1_demo/lmon.png",
  "Wavelength monitor after the guide. The band spans exactly the configured "
  "wl ± 0.5·wl = 2.5–7.5 Å (sharp edges = source definition). Intensity rising "
  "with λ is real physics: an m=2 supermirror guide reflects long wavelengths "
  "more efficiently (critical angle ∝ λ).")}
{embed_png("runs/m1_demo/psd.png",
  "PSD (beam footprint) 10 cm behind the guide exit. The bright core matches the "
  "2×4 cm guide exit cross-section, slightly diverged; log color scale makes the "
  "faint halo visible. beam_center ≈ (0,0) confirms alignment.")}
"""


def main():
    tools = get_tools()
    now = datetime.now().strftime("%Y-%m-%d %H:%M")
    tools_html = "".join(render_tool(t) for t in tools)
    toc = " · ".join(
        f"<a href='#tool-{esc(t.name)}'><code>{esc(t.name)}</code></a>" for t in tools
    )

    page = f"""<!DOCTYPE html>
<html lang="en"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>NeutronGym — Agent &harr; McStas Guide</title>
<style>
  :root {{ color-scheme: light dark; }}
  body {{ font: 15px/1.55 -apple-system, BlinkMacSystemFont, "Segoe UI", sans-serif;
         margin: 0; background: #f6f8fa; color: #1f2328; }}
  @media (prefers-color-scheme: dark) {{
    body {{ background: #0d1117; color: #e6edf3; }}
    .card {{ background: #161b22 !important; border-color: #30363d !important; }}
    pre, code, th {{ background: #21262d !important; }}
    .flow span {{ background: #21262d !important; border-color: #30363d !important; }}
  }}
  .wrap {{ max-width: 980px; margin: 0 auto; padding: 32px 20px 60px; }}
  h1 {{ font-size: 26px; margin: 0 0 4px; }}
  h2 {{ font-size: 20px; margin: 34px 0 10px; border-bottom: 2px solid {BLUE}22; padding-bottom: 4px; }}
  h3 {{ font-size: 16px; margin: 20px 0 8px; }}
  h4 {{ margin: 0 0 6px; }}
  .meta {{ color: {GRAY}; font-size: 13px; margin-bottom: 20px; }}
  .card {{ background: #fff; border: 1px solid #d0d7de; border-radius: 10px; padding: 18px 22px; margin: 14px 0; }}
  pre {{ background: #f0f1f3; border-radius: 8px; padding: 12px 14px; overflow-x: auto;
        font-size: 12.5px; line-height: 1.45; }}
  code {{ background: #f0f1f3; padding: 1px 5px; border-radius: 4px; font-size: 12.5px; }}
  pre code {{ background: none; padding: 0; }}
  table {{ border-collapse: collapse; width: 100%; font-size: 13.5px; margin: 8px 0; }}
  th, td {{ text-align: left; padding: 6px 10px; border-bottom: 1px solid #d0d7de55; vertical-align: top; }}
  th {{ background: #f6f8fa; font-size: 12px; }}
  .muted {{ color: {GRAY}; font-size: 13px; }}
  .flow {{ display: flex; flex-wrap: wrap; gap: 6px; align-items: center; margin: 14px 0; }}
  .flow span {{ background: #fff; border: 1px solid #d0d7de; border-radius: 8px;
               padding: 8px 12px; font-size: 13px; font-weight: 600; }}
  .flow em {{ color: {GRAY}; font-style: normal; }}
  .tool {{ border-top: 1px solid #d0d7de55; padding-top: 12px; margin-top: 12px; }}
  figure {{ margin: 14px 0; }}
  figure img {{ max-width: 100%; border: 1px solid #d0d7de; border-radius: 8px; }}
  figcaption {{ font-size: 13px; color: {GRAY}; margin-top: 6px; }}
  a {{ color: {BLUE}; }}
</style></head><body><div class="wrap">
  <h1>NeutronGym: how the agent drives McStas</h1>
  <div class="meta">Generated {now} from the live server ({len(tools)} tools) ·
    <a href="progress.html">progress dashboard</a> ·
    design: <code>note/m1-server-design-2026-07-09.md</code></div>

  <h2>The pipeline</h2>
  <div class="card">
    <div class="flow">
      <span>Claude agent</span> <em>&rarr; tool calls (MCP/stdio) &rarr;</em>
      <span>mcstas-mcp server</span> <em>&rarr; validate &rarr; generate .instr &rarr;</em>
      <span>mcrun</span> <em>&rarr; C compile &rarr; Monte-Carlo run &rarr;</em>
      <span>mccode.sim + .dat</span> <em>&rarr; parsed &rarr;</em>
      <span>summary stats / PNG</span>
    </div>
    <p>MCP (Model Context Protocol) is the plug-in mechanism of Claude Code: the
    <code>.mcp.json</code> at the repo root points at the <code>mcstas-mcp</code>
    binary (in the conda env), Claude Code starts it as a subprocess, and the agent
    sees its tools next to the built-in ones. The server does three jobs the raw
    CLI cannot: <strong>validate every call immediately</strong> against the real
    component library (agents cannot hallucinate components or parameters),
    <strong>run simulations safely</strong> (timeouts, capped ncount, captured
    compiler/runtime diagnostics), and <strong>return summaries, not dumps</strong>
    (a 128&times;128 detector image is ~50k numbers; the agent gets 10).</p>
    <p class="muted">Server code: <code>src/mcstas_mcp/</code> — catalog.py
    (introspection), registry.py (instrument specs + validation), execution.py
    (subprocess mcrun), results.py (statistics), server.py (tool layer).</p>
  </div>

  <h2>The agent's workflow, with real calls</h2>
  <div class="card">{CODE_EXAMPLES}</div>

  <h2>Tool reference <span class="muted">(auto-generated from the live server)</span></h2>
  <div class="card">
    <p class="muted">{toc}</p>
    {tools_html}
  </div>

  <h2>Understanding the results</h2>
  <div class="card">{RESULTS_GUIDE}</div>

  <footer class="muted" style="margin-top:30px">
    Regenerate after changing server tools:
    <code>conda run -n mcstas python scripts/guide_report.py</code>
  </footer>
</div></body></html>
"""
    with open(OUT, "w") as f:
        f.write(page)
    print(f"wrote {os.path.relpath(OUT, REPO)}: {len(tools)} tools documented")


if __name__ == "__main__":
    main()
