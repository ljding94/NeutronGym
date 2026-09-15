"""Generate m8.html — the human-readable M8 trainability record.

Generated, never hand-edited (same rule as progress.html / guide.html /
tasks.html). Stdlib only, so it runs under any python.

Reads whatever exists under runs/m8/ and renders the evidence behind the
phase-0 gate decision:

  phase0_n15.json          the UNCALIBRATED measurement (the ceiling)
  phase0_calibrated.json   the same measurement with calibrated targets
  paired_*.json            per-instance McNemar analyses
  sweep.json               difficulty-response curve (+ per-bar paired view)
  sweep_oneshot.json       the feedback-budget arm, if it has been run

The figure is inline SVG rather than matplotlib: the point of this file is
that anyone can regenerate it from committed JSON with no scientific-python
stack, and a reviewer can read the numbers out of the table beside it.

Usage:
  python3 scripts/m8_report.py
"""

import glob
import json
import os
from datetime import datetime

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
RUNS = os.path.join(REPO, "runs", "m8")
OUT = os.path.join(REPO, "m8.html")

CSS = """
:root{--bg:#fbfbfd;--fg:#1a1a1f;--mut:#6b6b76;--line:#e3e3ea;--card:#fff;
--bad:#b3261e;--good:#1b6b3a;--warn:#8a5a00;--accent:#2b4c9b}
*{box-sizing:border-box}
body{margin:0;background:var(--bg);color:var(--fg);
font:15px/1.55 -apple-system,BlinkMacSystemFont,"Segoe UI",Roboto,sans-serif}
.wrap{max-width:1060px;margin:0 auto;padding:32px 20px 80px}
h1{font-size:26px;margin:0 0 4px} h2{font-size:19px;margin:34px 0 10px}
h3{font-size:15px;margin:20px 0 8px;color:var(--mut);
text-transform:uppercase;letter-spacing:.06em}
.sub{color:var(--mut);margin:0 0 22px;font-size:13px}
.card{background:var(--card);border:1px solid var(--line);border-radius:10px;
padding:16px 18px;margin:14px 0}
.verdict{border-left:4px solid var(--bad)}
.verdict.ok{border-left-color:var(--good)}
.verdict h2{margin-top:0}
table{border-collapse:collapse;width:100%;font-size:14px;margin:8px 0}
th,td{border-bottom:1px solid var(--line);padding:7px 9px;text-align:left}
th{color:var(--mut);font-weight:600;font-size:12px;
text-transform:uppercase;letter-spacing:.04em}
td.n,th.n{text-align:right;font-variant-numeric:tabular-nums}
.tag{display:inline-block;padding:1px 8px;border-radius:99px;font-size:12px;
font-weight:600}
.tag.bad{background:#fdecea;color:var(--bad)}
.tag.good{background:#e7f4ec;color:var(--good)}
.tag.warn{background:#fdf3e0;color:var(--warn)}
.note{color:var(--mut);font-size:13px;margin:6px 0}
code{background:#f1f1f5;padding:1px 5px;border-radius:4px;font-size:13px}
.scroll{overflow-x:auto}
.missing{color:var(--mut);font-style:italic}
"""


def load(name):
    p = os.path.join(RUNS, name)
    if not os.path.isfile(p):
        return None
    with open(p) as f:
        return json.load(f)


def esc(s):
    return (str(s).replace("&", "&amp;").replace("<", "&lt;")
            .replace(">", "&gt;"))


def pct(x):
    return "—" if x is None else f"{100 * x:.0f}%"


def curve_svg(curve, w=640, h=260):
    """Inline SVG of pass rate vs target fraction for both models."""
    if not curve:
        return ""
    pad_l, pad_b, pad_t, pad_r = 46, 38, 16, 12
    xs = [c["fraction"] for c in curve]
    x0, x1 = min(xs), max(xs)
    span = (x1 - x0) or 1.0

    def px(fr):
        return pad_l + (fr - x0) / span * (w - pad_l - pad_r)

    def py(rate):
        return h - pad_b - (rate or 0) * (h - pad_b - pad_t)

    out = [f'<svg viewBox="0 0 {w} {h}" width="100%" height="{h}" '
           f'role="img" aria-label="pass rate versus target difficulty">']
    # gridlines + y axis
    for frac in (0, 0.25, 0.5, 0.75, 1.0):
        y = py(frac)
        out.append(f'<line x1="{pad_l}" y1="{y:.1f}" x2="{w - pad_r}" '
                   f'y2="{y:.1f}" stroke="#e3e3ea" stroke-width="1"/>')
        out.append(f'<text x="{pad_l - 8}" y="{y + 4:.1f}" font-size="11" '
                   f'fill="#6b6b76" text-anchor="end">{int(frac * 100)}%</text>')
    for c in curve:
        x = px(c["fraction"])
        out.append(f'<text x="{x:.1f}" y="{h - pad_b + 16}" font-size="11" '
                   f'fill="#6b6b76" text-anchor="middle">'
                   f'{c["fraction"]:g}&#215;</text>')
    out.append(f'<text x="{w / 2:.0f}" y="{h - 4}" font-size="11" '
               f'fill="#6b6b76" text-anchor="middle">'
               f'target as a multiple of the classical optimum</text>')
    colors = {"qwen3-8b": "#2b4c9b", "qwen3-32b": "#b3261e"}
    for model, color in colors.items():
        pts = [(px(c["fraction"]), py((c.get(model) or {}).get("pass_rate")))
               for c in curve if c.get(model)]
        if not pts:
            continue
        d = " ".join(f"{'M' if i == 0 else 'L'}{x:.1f},{y:.1f}"
                     for i, (x, y) in enumerate(pts))
        out.append(f'<path d="{d}" fill="none" stroke="{color}" '
                   f'stroke-width="2.2"/>')
        for x, y in pts:
            out.append(f'<circle cx="{x:.1f}" cy="{y:.1f}" r="3.6" '
                       f'fill="{color}"/>')
    # legend
    lx = pad_l + 6
    for model, color in colors.items():
        out.append(f'<rect x="{lx}" y="{pad_t}" width="10" height="10" '
                   f'fill="{color}" rx="2"/>')
        out.append(f'<text x="{lx + 15}" y="{pad_t + 9}" font-size="11.5" '
                   f'fill="#1a1a1f">{esc(model)}</text>')
        lx += 110
    out.append("</svg>")
    return "".join(out)


def paired_table(res, title):
    a, b = res["models"]
    if res["ordering_supported"]:
        tag = f'<span class="tag good">ordering: {esc(res["leader"])}</span>'
    elif res.get("balanced"):
        tag = '<span class="tag bad">no ordering</span>'
    else:
        tag = (f'<span class="tag warn">underpowered, leans '
               f'{esc(res.get("leader"))}</span>')
    rows = "".join(
        f"<tr><td>{lbl}</td><td class='n'>{val}</td></tr>" for lbl, val in [
            ("paired instances", res["n_paired"]),
            ("both pass", res["both_pass"]),
            (f"{esc(a)} only", res[f"only_{a}"]),
            (f"{esc(b)} only", res[f"only_{b}"]),
            ("neither", res["neither"]),
            ("agreement", pct(res["agreement"])),
            ("McNemar exact p", res["mcnemar_p"]),
        ])
    return (f'<div class="card"><h3>{esc(title)}</h3>{tag}'
            f'<table>{rows}</table></div>')


def eval_section(ev):
    """Pass rates per arm plus the pre-registered verdict. The verdict card
    is red whenever the claim bar is not met, including a regression."""
    arms = ev.get("arms") or {}
    v = ev.get("verdict") or {}
    met = bool(v.get("claim_bar_met"))
    gain = v.get("pass_gain")
    ca = v.get("level_migration_cochran_armitage") or {}
    mc = v.get("paired_mcnemar") or {}
    if met:
        msg = f"<b>Claim bar MET</b>: pass gain {gain:+.3f}."
    elif gain is not None and gain < 0:
        msg = (f"<b>Claim bar NOT MET — training made the model worse</b>: "
               f"pass gain {gain:+.3f}.")
    else:
        msg = f"<b>Claim bar NOT MET</b>: pass gain {gain:+.3f}."
    out = [f"<div class='card {'verdict ok' if met else 'verdict'}'><p>{msg}"
           f"</p><p class='note'>Paired McNemar p={mc.get('mcnemar_p')} "
           f"(untrained-only {mc.get('only_untrained-8b')}, trained-only "
           f"{mc.get('only_trained-8b')}); level migration Cochran–Armitage "
           f"z={ca.get('z')} p={ca.get('p')}. Families "
           f"{esc(ev.get('families'))}, bar {ev.get('target_fraction')}&#215;, "
           f"n={ev.get('n_per_family')} per family per arm.</p></div>"]
    rows = "".join(
        f"<tr><td>{esc(arm)}</td><td class='n'>{pct(a.get('pass_rate'))}</td>"
        f"<td class='n'>{a.get('passes')}/{a.get('n_valid')}</td>"
        f"<td class='n'>{a.get('mean_level')}</td>"
        f"<td>{esc(a.get('level_histogram'))}</td></tr>"
        for arm, a in arms.items())
    out.append("<div class='scroll'><table><tr><th>arm</th><th class='n'>pass"
               "</th><th class='n'>passes</th><th class='n'>mean level</th>"
               "<th>L0..L4</th></tr>" + rows + "</table></div>")
    return "".join(out)


def constant_matches(ab, constant, tol=0.02):
    """True when some no-model constant action reaches the trained model's
    pass rate (within tol): the trained score then says nothing about design
    skill, whatever the ablation criteria concluded."""
    if not constant:
        return False
    trained = (ab.get("pass_rate") or {}).get("passing-turn")
    best = (constant.get("best_single_constant") or {}).get("pass_rate")
    return trained is not None and best is not None and best >= trained - tol


def ablation_section(ab, constant=None):
    """The pre-specified ablation verdict. Always labelled post-hoc: it was
    decided after the pre-registered result, so it can explain that result
    but never replace it. Never green when a no-model constant matches the
    trained model (red-team finding 7)."""
    verdict = ab.get("verdict", "?")
    degenerate = constant_matches(ab, constant)
    tone = ("verdict ok" if verdict == "supported" and not degenerate
            else "verdict")
    rates = ab.get("pass_rate") or {}
    label = {"passing_vs_untrained": "passing-turn vs untrained 8B",
             "per_turn_vs_untrained": "per-turn vs untrained 8B",
             "passing_vs_per_turn": "passing-turn vs per-turn"}
    rows = "".join(
        f"<tr><td>{label[k]}</td>"
        f"<td class='n'>{(ab.get(k) or {}).get('only_a')}</td>"
        f"<td class='n'>{(ab.get(k) or {}).get('only_b')}</td>"
        f"<td class='n'>{(ab.get(k) or {}).get('mcnemar_p')}</td></tr>"
        for k in label)
    return (f"<div class='card {tone}'><p><span class='tag warn'>post-hoc</span> "
            f"Mechanism <b>{esc(verdict)}</b> under the criteria fixed before "
            f"training (PLAN.md, 2026-09-14).</p><p class='note'>Pass rates: "
            f"untrained 8B {pct(rates.get('untrained-8b'))}, per-turn "
            f"{pct(rates.get('per-turn'))}, passing-turn "
            f"{pct(rates.get('passing-turn'))}. The pre-registered per-turn "
            f"result remains the headline M8 result.</p>"
            + (f"<p><b>No-model constant baseline matches it:</b> the constant "
               f"{esc((constant.get('best_single_constant') or {}).get('action'))} "
               f"passes {pct((constant.get('best_single_constant') or {}).get('pass_rate'))} "
               f"of the same instances (any-of-five constants "
               f"{pct(constant.get('any_of_five_constants_pass_rate'))}). The "
               f"trained score reflects a learned lookup, not instance-specific "
               f"design.</p>" if degenerate else "")
            + "</div>"
            "<div class='scroll'><table><tr><th>paired comparison</th>"
            "<th class='n'>only first</th><th class='n'>only second</th>"
            "<th class='n'>McNemar p</th></tr>" + rows + "</table></div>")


def main():
    uncal, cal = load("phase0_n15.json"), load("phase0_calibrated.json")
    p_cal = load("paired_calibrated.json")
    p_unc = load("paired_uncalibrated.json")
    sweep, oneshot = load("sweep.json"), load("sweep_oneshot.json")
    ev = load("eval_guide_1x_n300.json")
    ablation = load("ablation_readout.json")
    constant = load("constant_probe_guide_1x_n300.json")

    H = [f"<!doctype html><meta charset='utf-8'><title>NeutronGym — M8 "
         f"trainability record</title><style>{CSS}</style><div class='wrap'>"]
    H.append("<h1>M8 — trainability: the gate, and why it stopped the "
             "experiment</h1>")
    H.append(f"<p class='sub'>Generated {datetime.now():%Y-%m-%d %H:%M} by "
             f"<code>scripts/m8_report.py</code> from <code>runs/m8/*.json"
             f"</code>. Never edit by hand — regenerate.</p>")

    # ---- verdict -------------------------------------------------------
    if cal:
        g = cal.get("gates", {})
        p8, p32 = g.get("baseline_8b_pass"), g.get("baseline_32b_pass")
        H.append("<div class='card verdict'><h2>Phase-0 gate: FAILED on "
                 "vacuity</h2>"
                 f"<p>On held-out procedural instances with calibrated "
                 f"targets, the untrained <b>Qwen3-8B scores {pct(p8)}</b> "
                 f"and the untrained <b>Qwen3-32B scores {pct(p32)}</b> — a "
                 f"gap of <b>{(p32 or 0) - (p8 or 0):+.3f}</b>. "
                 f"&ldquo;Approaching a larger untrained model&rdquo; "
                 f"presumes the larger model is ahead. It is not, so the "
                 f"claim bar has no target and cannot be evaluated on this "
                 f"axis.</p>"
                 "<p class='note'>The gate ran before any data generation "
                 "or GPU time, which is what it was for.</p></div>")

    # ---- calibration ---------------------------------------------------
    if uncal and cal:
        gu, gc = uncal.get("gates", {}), cal.get("gates", {})
        H.append("<h2>Calibration: the first measurement was a ceiling</h2>")
        H.append("<p class='note'>Procedural instances shipped with "
                 "<code>target_ratio = 1.0</code>, so L4 meant beating a "
                 "deliberately undersized baseline. <code>calibrate.py</code>"
                 " now sets per-instance targets at 0.8&#215; a "
                 "constraint-filtered, Liouville-checked, "
                 "fresh-seed-re-verified classical optimum — the same "
                 "discipline as the T2 benchmark tasks.</p>")
        H.append("<div class='scroll'><table><tr><th>target</th>"
                 "<th class='n'>8B pass</th><th class='n'>32B pass</th>"
                 "<th class='n'>gap</th></tr>"
                 f"<tr><td>uncalibrated (beat the baseline)</td>"
                 f"<td class='n'>{pct(gu.get('baseline_8b_pass'))}</td>"
                 f"<td class='n'>{pct(gu.get('baseline_32b_pass'))}</td>"
                 f"<td class='n'>"
                 f"{(gu.get('baseline_32b_pass') or 0) - (gu.get('baseline_8b_pass') or 0):+.3f}"
                 f"</td></tr>"
                 f"<tr><td>calibrated (0.8&#215; classical)</td>"
                 f"<td class='n'>{pct(gc.get('baseline_8b_pass'))}</td>"
                 f"<td class='n'>{pct(gc.get('baseline_32b_pass'))}</td>"
                 f"<td class='n'>"
                 f"{(gc.get('baseline_32b_pass') or 0) - (gc.get('baseline_8b_pass') or 0):+.3f}"
                 f"</td></tr></table></div>")
        H.append("<p class='note'><b>Transferable lesson:</b> an environment "
                 "whose reward ladder is not calibrated per instance will "
                 "report a trainability signal that is really a ceiling.</p>")

    # ---- paired --------------------------------------------------------
    if p_cal or p_unc:
        H.append("<h2>Paired view — equal rates are not equal behaviour</h2>")
        H.append("<p class='note'>Marginal pass rates cannot distinguish "
                 "&ldquo;the models behave alike&rdquo; from &ldquo;the "
                 "models succeed on different instances&rdquo;. McNemar's "
                 "exact test conditions on exactly the disagreements, which "
                 "are the only episodes carrying information about an "
                 "ordering.</p>")
        if p_cal:
            H.append(paired_table(p_cal, "calibrated (the gate data)"))
        if p_unc:
            H.append(paired_table(p_unc, "uncalibrated (the ceiling run)"))
        H.append("<p class='note'>Measured at temperature 0 with one episode "
                 "per instance, so a disagreement is deterministic for that "
                 "model&ndash;instance pair and cannot be split into "
                 "capability versus luck without resampling.</p>")

    # ---- difficulty sweep ----------------------------------------------
    H.append("<h2>Difficulty response — is the bar too low, or the task "
             "flat?</h2>")
    if not sweep:
        H.append("<p class='missing'>runs/m8/sweep.json not present yet — "
                 "run <code>benchmark/harness/m8_difficulty_sweep.py</code>."
                 "</p>")
    else:
        v = sweep.get("verdict", {})
        cls = "verdict" if v.get("hypothesis") == "H_flat" else "verdict ok"
        msg = ("<b>H_flat.</b> No difficulty separates the two models, so "
               "the 0.8&#215; bar was not a ceiling — the task does not "
               "discriminate model scale at any bar."
               if v.get("hypothesis") == "H_flat" else
               f"<b>H_saturated.</b> The models DO separate at "
               f"{esc(v.get('separating_fractions'))} — the original bar "
               f"was a ceiling, not a dead task.")
        H.append(f"<div class='card {cls}'><p>{msg}</p>"
                 f"<p class='note'>Largest absolute gap "
                 f"{v.get('max_abs_gap')} at "
                 f"{v.get('max_gap_fraction')}&#215;; separation threshold "
                 f"{sweep.get('separation_threshold')} (the plan's "
                 f"pre-registered &ge;10-point rule). Feedback budget: "
                 f"{sweep.get('max_steps', 6)} step(s).</p></div>")
        H.append(curve_svg(sweep.get("curve") or []))
        rows = []
        for c in sweep.get("curve") or []:
            m8, m32 = c.get("qwen3-8b") or {}, c.get("qwen3-32b") or {}
            pr = (sweep.get("paired") or {}).get(str(c["fraction"]), {})
            rows.append(
                f"<tr><td>{c['fraction']:g}&#215;</td>"
                f"<td class='n'>{pct(m8.get('pass_rate'))}</td>"
                f"<td class='n'>{pct(m32.get('pass_rate'))}</td>"
                f"<td class='n'>{c['pass_gap']:+.3f}</td>"
                f"<td class='n'>{m8.get('mean_level')}</td>"
                f"<td class='n'>{m32.get('mean_level')}</td>"
                f"<td class='n'>{m8.get('steps_to_success_median')}</td>"
                f"<td class='n'>{m32.get('steps_to_success_median')}</td>"
                f"<td class='n'>{pr.get('mcnemar_p', '—')}</td></tr>")
        H.append("<div class='scroll'><table><tr><th>target</th>"
                 "<th class='n'>8B pass</th><th class='n'>32B pass</th>"
                 "<th class='n'>gap</th><th class='n'>8B mean lvl</th>"
                 "<th class='n'>32B mean lvl</th><th class='n'>8B steps</th>"
                 "<th class='n'>32B steps</th><th class='n'>McNemar p</th>"
                 "</tr>" + "".join(rows) + "</table></div>")
        H.append("<p class='note'>Both models see the same instances at the "
                 "same bars, and the prompt always states the bar in force. "
                 "Steps are the median over PASSING episodes only.</p>")

    # ---- phase-3 evaluation --------------------------------------------
    H.append("<h2>Trained vs untrained — the pre-registered evaluation</h2>")
    if not ev:
        H.append("<p class='missing'>runs/m8/eval_guide_1x_n300.json not "
                 "present yet — run <code>benchmark/harness/m8_eval.py</code>."
                 "</p>")
    else:
        H.append(eval_section(ev))

    # ---- post-hoc ablation ----------------------------------------------
    H.append("<h2>Post-hoc ablation — train only the passing turn</h2>")
    if not ablation:
        H.append("<p class='missing'>runs/m8/ablation_readout.json not present "
                 "yet — run <code>benchmark/harness/m8_ablation_readout.py"
                 "</code> after the ablation evaluation.</p>")
    else:
        H.append(ablation_section(ablation, constant))

    # ---- feedback budget -----------------------------------------------
    H.append("<h2>Feedback budget — search versus reasoning</h2>")
    if not oneshot:
        H.append("<p class='missing'>Not run yet. With six steps of dense "
                 "per-step FOM feedback the task is hill-climbing — the job "
                 "this architecture delegates to scipy. At "
                 "<code>--max-steps 1</code> there is no feedback to climb, "
                 "so the model must predict parameters from the physics "
                 "alone; that is the direct test of whether scale matters "
                 "for reasoning rather than search.</p>")
    else:
        rows = []
        for c in oneshot.get("curve") or []:
            m8, m32 = c.get("qwen3-8b") or {}, c.get("qwen3-32b") or {}
            rows.append(f"<tr><td>{c['fraction']:g}&#215;</td>"
                        f"<td class='n'>{pct(m8.get('pass_rate'))}</td>"
                        f"<td class='n'>{pct(m32.get('pass_rate'))}</td>"
                        f"<td class='n'>{c['pass_gap']:+.3f}</td></tr>")
        H.append(f"<p class='note'>One graded action per episode, no "
                 f"feedback (<code>--max-steps "
                 f"{oneshot.get('max_steps')}</code>).</p>")
        H.append("<div class='scroll'><table><tr><th>target</th>"
                 "<th class='n'>8B pass</th><th class='n'>32B pass</th>"
                 "<th class='n'>gap</th></tr>" + "".join(rows) +
                 "</table></div>")

    # ---- provenance ----------------------------------------------------
    files = sorted(os.path.basename(p)
                   for p in glob.glob(os.path.join(RUNS, "*.json")))
    H.append("<h2>Provenance</h2><p class='note'>Rendered from: " +
             ", ".join(f"<code>runs/m8/{esc(f)}</code>" for f in files) +
             ". Regenerate with <code>python3 scripts/m8_report.py</code>."
             "</p></div>")

    with open(OUT, "w") as f:
        f.write("".join(H))
    print(f"wrote {os.path.relpath(OUT, REPO)} "
          f"({len(files)} source files, sweep={'yes' if sweep else 'no'}, "
          f"oneshot={'yes' if oneshot else 'no'})")


if __name__ == "__main__":
    main()
