"""m8.html generator regressions.

The dashboard is the human-inspectable artifact for the M8 record, and it
is regenerated from JSON rather than hand-written, so the failure mode to
guard is silent misrendering: a missing input that produces a confident
empty figure, or a curve that plots the wrong series.
"""

import importlib
import json
import os
import sys

import pytest

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, "scripts"))

m8_report = importlib.import_module("m8_report")


def test_curve_svg_is_empty_for_no_data_not_a_blank_axis():
    """An empty figure with axes would read as 'measured, found nothing'."""
    assert m8_report.curve_svg([]) == ""


def test_curve_svg_plots_one_point_per_fraction_per_model():
    curve = [
        {"fraction": 0.8, "pass_gap": 0.16,
         "qwen3-8b": {"pass_rate": 0.60}, "qwen3-32b": {"pass_rate": 0.76}},
        {"fraction": 1.0, "pass_gap": 0.22,
         "qwen3-8b": {"pass_rate": 0.36}, "qwen3-32b": {"pass_rate": 0.58}},
    ]
    svg = m8_report.curve_svg(curve)
    assert svg.startswith("<svg") and svg.endswith("</svg>")
    assert svg.count("<circle") == 4          # 2 fractions x 2 models
    assert svg.count("<path") == 2            # one line per model
    assert "qwen3-8b" in svg and "qwen3-32b" in svg
    # both axis labels present
    assert "0.8&#215;" in svg and "1&#215;" in svg


def test_curve_svg_tolerates_a_missing_model_series():
    curve = [{"fraction": 1.0, "pass_gap": 0.0,
              "qwen3-8b": {"pass_rate": 0.4}}]
    svg = m8_report.curve_svg(curve)
    assert svg.count("<circle") == 1


def test_curve_svg_handles_a_null_pass_rate_without_crashing():
    """A cell with no valid episodes carries pass_rate None (infra), which
    must not raise or silently plot as a real zero at the axis."""
    curve = [{"fraction": 1.0, "pass_gap": 0.0,
              "qwen3-8b": {"pass_rate": None},
              "qwen3-32b": {"pass_rate": 0.5}}]
    svg = m8_report.curve_svg(curve)
    assert svg.count("<circle") == 2


def test_paired_table_labels_the_three_verdicts_distinctly():
    base = {"models": ["qwen3-8b", "qwen3-32b"], "n_paired": 20,
            "both_pass": 10, "neither": 2, "agreement": 0.6}
    balanced = dict(base, **{"only_qwen3-8b": 4, "only_qwen3-32b": 4,
                             "mcnemar_p": 1.0, "ordering_supported": False,
                             "balanced": True, "leader": None})
    under = dict(base, **{"only_qwen3-8b": 1, "only_qwen3-32b": 5,
                          "mcnemar_p": 0.219, "ordering_supported": False,
                          "balanced": False, "leader": "qwen3-32b"})
    ordered = dict(base, **{"only_qwen3-8b": 0, "only_qwen3-32b": 8,
                            "mcnemar_p": 0.008, "ordering_supported": True,
                            "balanced": False, "leader": "qwen3-32b"})
    assert "no ordering" in m8_report.paired_table(balanced, "t")
    assert "underpowered" in m8_report.paired_table(under, "t")
    assert "ordering: qwen3-32b" in m8_report.paired_table(ordered, "t")


def test_pct_distinguishes_missing_from_zero():
    assert m8_report.pct(None) == "—"
    assert m8_report.pct(0) == "0%"
    assert m8_report.pct(0.76) == "76%"


def test_report_runs_with_no_inputs_and_says_what_is_missing(tmp_path,
                                                             monkeypatch):
    """A fresh checkout has no runs/m8 — the page must still build and
    state what has not been measured rather than implying a null."""
    monkeypatch.setattr(m8_report, "RUNS", str(tmp_path / "empty"))
    out = tmp_path / "m8.html"
    monkeypatch.setattr(m8_report, "OUT", str(out))
    m8_report.main()
    html = out.read_text()
    assert "sweep.json not present yet" in html
    assert "Not run yet" in html


def test_report_renders_a_full_sweep(tmp_path, monkeypatch):
    runs = tmp_path / "m8"
    runs.mkdir()
    (runs / "sweep.json").write_text(json.dumps({
        "max_steps": 6, "separation_threshold": 0.1,
        "verdict": {"hypothesis": "H_saturated", "max_abs_gap": 0.22,
                    "max_gap_fraction": 1.0, "separating_fractions": [0.8, 1.0]},
        "paired": {"0.8": {"mcnemar_p": 0.021}},
        "curve": [{"fraction": 0.8, "pass_gap": 0.16,
                   "qwen3-8b": {"pass_rate": 0.6, "mean_level": 3.54,
                                "steps_to_success_median": 3.0},
                   "qwen3-32b": {"pass_rate": 0.76, "mean_level": 3.66,
                                 "steps_to_success_median": 1.0}}]}))
    monkeypatch.setattr(m8_report, "RUNS", str(runs))
    out = tmp_path / "m8.html"
    monkeypatch.setattr(m8_report, "OUT", str(out))
    m8_report.main()
    html = out.read_text()
    assert "H_saturated" in html
    assert "was a ceiling, not a dead task" in html
    assert "<svg" in html
    assert "0.021" in html        # the paired p reaches the table
    assert "76%" in html


def test_report_names_h_flat_when_nothing_separates(tmp_path, monkeypatch):
    runs = tmp_path / "m8"
    runs.mkdir()
    (runs / "sweep.json").write_text(json.dumps({
        "max_steps": 6, "separation_threshold": 0.1,
        "verdict": {"hypothesis": "H_flat", "max_abs_gap": 0.02,
                    "max_gap_fraction": 0.8, "separating_fractions": []},
        "curve": [{"fraction": 0.8, "pass_gap": 0.0,
                   "qwen3-8b": {"pass_rate": 0.7},
                   "qwen3-32b": {"pass_rate": 0.7}}]}))
    monkeypatch.setattr(m8_report, "RUNS", str(runs))
    out = tmp_path / "m8.html"
    monkeypatch.setattr(m8_report, "OUT", str(out))
    m8_report.main()
    html = out.read_text()
    assert "H_flat" in html
    assert "does not discriminate model scale at any bar" in html


@pytest.mark.parametrize("bad", ["<script>", "a&b", "x<y"])
def test_escaping(bad):
    assert "<script>" not in m8_report.esc(bad)
    assert "&" in m8_report.esc(bad) or "<" not in m8_report.esc(bad)


def test_eval_section_flags_a_regression_as_not_met():
    ev = {"families": ["guide_divergence"], "target_fraction": 1.0,
          "n_per_family": 300,
          "arms": {"untrained-8b": {"pass_rate": 0.4033, "passes": 121,
                                    "n_valid": 300, "mean_level": 3.213,
                                    "level_histogram": {"4": 121}},
                   "trained-8b": {"pass_rate": 0.3133, "passes": 94,
                                  "n_valid": 300, "mean_level": 3.03,
                                  "level_histogram": {"4": 94}}},
          "verdict": {"pass_gain": -0.09, "claim_bar_met": False,
                      "level_migration_cochran_armitage": {"z": -2.9478, "p": 0.0032},
                      "paired_mcnemar": {"mcnemar_p": 0.0013,
                                         "only_untrained-8b": 47,
                                         "only_trained-8b": 20}}}
    html = m8_report.eval_section(ev)
    assert "training made the model worse" in html
    assert "verdict ok" not in html
    assert "31%" in html and "40%" in html
    assert "0.0013" in html


def test_eval_section_marks_a_met_bar_green():
    ev = {"arms": {}, "verdict": {"pass_gain": 0.12, "claim_bar_met": True}}
    html = m8_report.eval_section(ev)
    assert "verdict ok" in html and "Claim bar MET" in html


def _readout(verdict):
    return {"verdict": verdict, "label": "post-hoc",
            "pass_rate": {"untrained-8b": 0.4033, "per-turn": 0.3133,
                          "passing-turn": 0.39},
            "passing_vs_untrained": {"only_a": 30, "only_b": 26, "mcnemar_p": 0.69},
            "per_turn_vs_untrained": {"only_a": 47, "only_b": 20, "mcnemar_p": 0.0013},
            "passing_vs_per_turn": {"only_a": 15, "only_b": 38, "mcnemar_p": 0.002}}


def test_ablation_section_is_always_labelled_post_hoc():
    for v in ("supported", "refuted", "inconclusive"):
        html = m8_report.ablation_section(_readout(v))
        assert "post-hoc" in html
        assert "remains the headline M8 result" in html
        assert v in html


def test_ablation_section_only_green_when_supported():
    assert "verdict ok" in m8_report.ablation_section(_readout("supported"))
    assert "verdict ok" not in m8_report.ablation_section(_readout("refuted"))
    assert "verdict ok" not in m8_report.ablation_section(_readout("inconclusive"))


def test_ablation_section_shows_all_three_comparisons():
    html = m8_report.ablation_section(_readout("supported"))
    assert "passing-turn vs untrained 8B" in html
    assert "per-turn vs untrained 8B" in html
    assert "passing-turn vs per-turn" in html
    assert "0.0013" in html and "0.002" in html


def test_report_says_ablation_missing_before_readout(tmp_path, monkeypatch):
    monkeypatch.setattr(m8_report, "RUNS", str(tmp_path / "empty"))
    out = tmp_path / "m8.html"
    monkeypatch.setattr(m8_report, "OUT", str(out))
    m8_report.main()
    assert "ablation_readout.json not present yet" in out.read_text()
