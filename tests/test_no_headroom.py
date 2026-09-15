"""Fix 1 (2026-09-15): L4 must always mean beating the baseline.

After the SANS direct-beam filter, the 30-sample classical search could not
beat the baseline on 74/600 train instances, so their target fell below it
and resubmitting the baseline passed L4 — 55% of the SANS RAFT data.
"""

import glob
import json
import os

import pytest

from neutrongym import calibrate, rollouts


class _NoRun:
    def run(self, *a, **k):
        raise AssertionError("cached calibration must not simulate")


def _cache(tmp_path, iid, over, fraction=0.8):
    d = tmp_path / calibrate.CAL_DIR
    d.mkdir(parents=True, exist_ok=True)
    (d / f"{iid}.json").write_text(json.dumps({
        "ok": True, "classical_over_baseline": over, "fraction": fraction,
        "target_ratio": round(fraction * over, 6),
        "classical_action": {"r_pin1": 0.008, "r_pin2": 0.008}}))


def test_target_below_baseline_is_floored_and_flagged(tmp_path):
    inst = {"id": "sans_collimation-train-000001"}
    _cache(tmp_path, inst["id"], over=0.9)
    cal = calibrate.calibration_for(inst, _NoRun(), {"fom": 1.0}, str(tmp_path), fraction=1.0)
    assert cal["raw_target_ratio"] == 0.9
    assert cal["target_ratio"] == 1.0
    assert cal["no_headroom"] is True
    assert cal["classical_action"] == {"r_pin1": 0.008, "r_pin2": 0.008}
    assert calibrate.calibrated_target_ratio(inst, _NoRun(), {"fom": 1.0},
                                             str(tmp_path), fraction=1.0) == 1.0


def test_exactly_at_baseline_has_no_headroom(tmp_path):
    inst = {"id": "g-1"}
    _cache(tmp_path, inst["id"], over=1.25)          # 0.8 * 1.25 = 1.0
    assert calibrate.calibration_for(inst, _NoRun(), {"fom": 1.0}, str(tmp_path))["no_headroom"]


def test_headroom_depends_on_the_bar(tmp_path):
    inst = {"id": "g-2"}
    _cache(tmp_path, inst["id"], over=1.1)
    cal_hi = calibrate.calibration_for(inst, _NoRun(), {"fom": 1.0}, str(tmp_path), fraction=1.0)
    cal_lo = calibrate.calibration_for(inst, _NoRun(), {"fom": 1.0}, str(tmp_path), fraction=0.8)
    assert cal_hi["no_headroom"] is False and cal_hi["target_ratio"] == 1.1
    assert cal_lo["no_headroom"] is True and cal_lo["target_ratio"] == 1.0


def test_failed_calibration_is_none(tmp_path):
    d = tmp_path / calibrate.CAL_DIR; d.mkdir()
    (d / "x.json").write_text(json.dumps({"ok": False}))
    assert calibrate.calibration_for({"id": "x"}, _NoRun(), {}, str(tmp_path)) is None


class _StubEnv:
    max_steps = 6

    def __init__(self, skip):
        self.skip, self.steps, self.resets = set(skip), [], []

    def reset(self, index):
        self.resets.append(index)
        self._inst = {"id": f"i{index}", "free_parameters": {"a": (0.0, 1.0)},
                      "no_headroom": index in self.skip}
        self.steps = []
        return ({"instance": self._inst, "prompt": "p"},
                {"instance_id": self._inst["id"], "split": "train"})

    def step(self, action):
        rec = {"level": 4, "reward": 1.2, "action": dict(action),
               "levels": {"L4": {"fom_ratio": 1.2}}}
        self.steps.append(rec)
        obs = {"instance": self._inst, "feedback": {"level": 4, "failed_at": None,
                                                   "detail": None, "fom_ratio": 1.2}}
        return obs, 1.2, True, False, rec

    def episode_record(self):
        return {"steps": list(self.steps)}


def test_rollout_skips_without_asking_the_model():
    def boom(msgs):
        raise AssertionError("model must not be called on a no-headroom instance")
    ep = rollouts.rollout(_StubEnv({0}), 0, boom)
    assert ep["skipped"] == "no_headroom" and ep["episode"] == []


def test_collect_never_writes_no_headroom_episodes(tmp_path, monkeypatch):
    monkeypatch.setattr(rollouts, "NeutronGym", lambda **kw: _StubEnv({0, 2}))
    out = tmp_path / "k.jsonl"
    r = rollouts.collect("m", 4, str(out), family="sans_collimation",
                         chat_fn=lambda m: '{"a": 0.5}')
    assert r["skipped_no_headroom"] == 2
    assert r["instances"] == 2 and r["kept"] == 2
    ids = [json.loads(l)["instance_id"] for l in out.read_text().splitlines()]
    assert ids == ["i1", "i3"]


def test_evaluate_drops_no_headroom_instances_instead_of_scoring_them(monkeypatch):
    monkeypatch.setattr(rollouts, "NeutronGym", lambda **kw: _StubEnv({1, 3}))
    r = rollouts.evaluate("m", 4, "sans_collimation", "heldout",
                          chat_fn=lambda m: '{"a": 0.5}')
    assert r["skipped_no_headroom"] == 2
    assert [row["instance"] for row in r["rows"]] == [0, 2]
    assert r["pass_rate"] == 1.0


@pytest.mark.slow
def test_real_sans_train_instance_without_headroom_rejects_the_baseline():
    """On a cached SANS train instance whose classical optimum is below the
    baseline, the env must flag it and the baseline must not reach L4."""
    from neutrongym import generate
    from neutrongym.env import NeutronGym
    home = os.path.expanduser(f"~/.mcstas-mcp/families/sans_collimation/{calibrate.CAL_DIR}")
    idx = None
    for p in sorted(glob.glob(f"{home}/sans_collimation-train-*.json")):
        rec = json.load(open(p))
        if rec.get("ok") and rec["classical_over_baseline"] < 1.0:
            idx = int(os.path.basename(p)[:-5].rsplit("-", 1)[1]); break
    if idx is None:
        pytest.skip("no cached no-headroom SANS train instance on this machine")
    env = NeutronGym(family="sans_collimation", split="train", target_fraction=1.0)
    obs, _ = env.reset(index=idx)
    inst = obs["instance"]
    assert inst["no_headroom"] is True and inst["target_ratio"] >= 1.0
    rec = env.step(dict(inst["baseline"]))[4]
    assert rec["level"] < 4
