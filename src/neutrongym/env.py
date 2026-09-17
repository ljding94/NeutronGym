"""NeutronGym — the gym-style environment over procedural instrument design.

reset() draws a procedural instance (deterministic in (family, split,
index)), ensures the family binary exists (compile-once, shared cache), and
runs the instance's baseline configuration once at the terminal protocol.
step(action) scores a parameter proposal through the reward ladder and
returns dense, level-resolved feedback. Every step's full record is kept in
the episode log — level-resolved rollout logging from the very first
trajectory (the M8 attribution analysis cannot be reconstructed after the
fact).

Observations are plain dicts; `obs["prompt"]` is the rendered NL task sheet
for LLM agents (the reference loop / SFT data path). State lives under
MCSTAS_MCP_HOME (families/ subdir) so tests and episodes inherit the same
isolation override the rest of the stack uses.

Typical use:
    env = NeutronGym(family="guide_divergence", split="train")
    obs, info = env.reset(index=0)
    obs, reward, terminated, truncated, info = env.step(
        {"w_in": 0.05, "w_out": 0.03, "m_coat": 2.5})
"""

import os

from mcstas_mcp.config import home_dir

from . import calibrate, generate, reward
from .executor import FamilyExecutor


class NeutronGym:
    def __init__(self, family: str = "guide_divergence", split: str = "train",
                 workdir: str | None = None, max_steps: int = 32,
                 calibrated: bool = True,
                 target_fraction: float | None = None):
        if family not in generate.FAMILIES:
            raise ValueError(f"unknown family {family!r} — have "
                             f"{sorted(generate.FAMILIES)}")
        self.family, self.split, self.max_steps = family, split, max_steps
        # calibrated targets (default ON since 2026-09-13): without them L4
        # means merely beating a deliberately undersized baseline, which the
        # untrained 8B already cleared ~83% of the time — no headroom for
        # any trainability claim. calibrated=False reproduces the old
        # (trivial) bar for comparison.
        self.calibrated = calibrated
        # difficulty knob: fraction of the classical optimum L4 demands.
        # None = the module default (0.8, the T2 benchmark discipline).
        # Sweeping it is free — see calibrate.calibrated_target_ratio.
        self.target_fraction = target_fraction
        self.workdir = workdir or os.path.join(home_dir(), "families")
        self.exec = FamilyExecutor(
            generate.family_instr(family, self.workdir),
            workdir=os.path.join(self.workdir, family, "rollouts"))
        comp = self.exec.compile()
        if not comp["ok"]:
            raise RuntimeError(f"family {family} failed to compile: "
                               f"{comp.get('diagnostics')}")
        self._baselines: dict = {}
        self._counter = 0
        self.instance = None
        self._base = None
        self._steps = 0
        self._episode: list = []

    def reset(self, index: int | None = None):
        """-> (obs, info). index selects the deterministic instance;
        omitted = next sequential."""
        if index is None:
            index, self._counter = self._counter, self._counter + 1
        inst = generate.instance(self.family, self.split, index)
        family_dir = self.family
        base = self._baselines.get(inst["id"])
        if base is None:
            base = reward.baseline(inst, self.exec)
            if not base["ok"]:
                raise RuntimeError(
                    f"baseline failed for {inst['id']} — the generator "
                    f"produced an unrunnable context (fix the family ranges, "
                    f"do not skip silently): {base['detail']}")
            self._baselines[inst["id"]] = base
        if self.calibrated:
            cal = calibrate.calibration_for(
                inst, self.exec, base, os.path.join(self.workdir, family_dir),
                fraction=self.target_fraction)
            if cal is not None:
                inst["target_ratio"] = cal["target_ratio"]
                # classical search could not beat the baseline at this bar:
                # not an improvement task, so rollouts skip it
                inst["no_headroom"] = cal["no_headroom"]
                inst["classical_action"] = cal["classical_action"]
                if "targets" in cal:
                    inst["targets"] = cal["targets"]
            inst["target_calibrated"] = cal is not None
            inst["target_fraction"] = (self.target_fraction
                                       if self.target_fraction is not None
                                       else calibrate.TARGET_FRACTION)
        else:
            inst["target_calibrated"] = False
        self.instance, self._base = inst, base
        self._steps, self._episode = 0, []
        obs = {
            "prompt": generate.render_prompt(inst, base),
            "instance": inst,
            "baseline_fom": base["fom"],
            "feedback": None,
        }
        return obs, {"instance_id": inst["id"], "split": inst["split"]}

    def step(self, action: dict):
        """-> (obs, reward, terminated, truncated, info). info is the full
        level-resolved record; terminated on L4 pass (beat the target)."""
        if self.instance is None:
            raise RuntimeError("call reset() before step()")
        rec = reward.score(self.instance, action, self.exec, self._base)
        # an exact resubmission is flagged in the feedback: the trained SANS
        # model (2026-09-16) resubmitted one design for 8 straight turns
        repeat_of = next((s["step"] for s in self._episode
                          if s.get("action") == rec.get("action")), None)
        self._steps += 1
        rec["step"] = self._steps
        self._episode.append(rec)
        terminated = bool(rec["levels"].get("L4", {}).get("pass"))
        truncated = self._steps >= self.max_steps
        failed = next((lv for lv in ("L1", "L2", "L3", "L4")
                       if not rec["levels"].get(lv, {}).get("pass", False)),
                      None)
        obs = {
            "prompt": None,  # unchanged from reset
            "instance": self.instance,
            "baseline_fom": self._base["fom"],
            "feedback": {
                "level": rec["level"],
                "failed_at": failed if rec["level"] < 4 else None,
                "detail": rec["levels"].get(failed, {}).get("detail")
                if failed else None,
                "fom": rec["fom"],
                "fom_ratio": rec["levels"].get("L4", {}).get("fom_ratio"),
                "repeat_of": repeat_of,
                "match": rec.get("match"),
            },
        }
        return obs, rec["reward"], terminated, truncated, rec

    def episode_record(self) -> dict:
        """The rollout log for this episode: instance + every step's
        level-resolved record (feed this to M8 logging as-is)."""
        return {"instance": self.instance, "baseline": self._base,
                "steps": list(self._episode)}
