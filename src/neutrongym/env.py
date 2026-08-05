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

from . import generate, reward
from .executor import FamilyExecutor


class NeutronGym:
    def __init__(self, family: str = "guide_divergence", split: str = "train",
                 workdir: str | None = None, max_steps: int = 32):
        if family not in generate.FAMILIES:
            raise ValueError(f"unknown family {family!r} — have "
                             f"{sorted(generate.FAMILIES)}")
        self.family, self.split, self.max_steps = family, split, max_steps
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
        base = self._baselines.get(inst["id"])
        if base is None:
            base = reward.baseline(inst, self.exec)
            if not base["ok"]:
                raise RuntimeError(
                    f"baseline failed for {inst['id']} — the generator "
                    f"produced an unrunnable context (fix the family ranges, "
                    f"do not skip silently): {base['detail']}")
            self._baselines[inst["id"]] = base
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
            },
        }
        return obs, rec["reward"], terminated, truncated, rec

    def episode_record(self) -> dict:
        """The rollout log for this episode: instance + every step's
        level-resolved record (feed this to M8 logging as-is)."""
        return {"instance": self.instance, "baseline": self._base,
                "steps": list(self._episode)}
