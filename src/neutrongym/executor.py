"""Fast-tier executor: compile once per template family, run the binary directly.

The number the RL plan rests on (measured 2026-07-24, re-verified
2026-07-31): the mcrun wrapper costs ~2.4 s FLAT per invocation regardless
of ncount, while executing the compiled mccode binary directly costs
~0.04 s per 1e5-ray rollout including result parsing — ~25 rollouts/s/core.
So: one mcrun compile per template family (topology fixed), then every
env step invokes `<family>.out -n N -s SEED -d DIR param=value ...`.

Shares the comment-stripped-sha compile cache (`.build_meta.json`) with
`mcstas_mcp.execution`, so env-side and MCP-side runs never recompile each
other's binaries. MPI is never used here — fast-tier runs are far too short
to amortize it (and the cache key includes the MPI flag).

mccode runtime gotchas encoded here: the binary only mkdirs the LEAF output
dir (parent must exist, leaf must not); zero CLI parameters would trigger
the interactive mcreadparams prompt (we always pass every parameter, plus
stdin=DEVNULL as belt and braces); seed must be nonzero.
"""

import itertools
import os
import re
import shutil
import subprocess
import time

from mcstas_mcp import results
from mcstas_mcp.config import mcrun_path
from mcstas_mcp.execution import _compile_plan, _write_build_meta

COMPILE_TIMEOUT_S = 300
RUN_TIMEOUT_S = 60


def read_define_params(instr_path: str) -> dict:
    """name -> default (string or None) from the DEFINE INSTRUMENT(...) header."""
    with open(instr_path, errors="replace") as f:
        text = f.read()
    m = re.search(r"DEFINE\s+INSTRUMENT\s+\w+\s*\(", text)
    if not m:
        return {}
    depth, i = 1, m.end()
    while i < len(text) and depth:
        depth += {"(": 1, ")": -1}.get(text[i], 0)
        i += 1
    params = {}
    for part in text[m.end():i - 1].split(","):
        part = part.strip()
        if not part:
            continue
        decl, _, default = part.partition("=")
        name = decl.split()[-1].strip("* ")
        params[name] = default.strip() or None
    return params


class FamilyExecutor:
    """Compile-once / run-many executor for one template family's .instr."""

    def __init__(self, instr_path: str, workdir: str | None = None):
        self.instr = os.path.abspath(instr_path)
        if not os.path.isfile(self.instr):
            raise FileNotFoundError(self.instr)
        self.binary = self.instr[:-len(".instr")] + ".out"
        self.workdir = os.path.abspath(workdir) if workdir else os.path.join(
            os.path.dirname(self.instr), "rollouts")
        self.defaults = read_define_params(self.instr)
        self._seq = itertools.count()

    def compile(self, timeout: int = COMPILE_TIMEOUT_S) -> dict:
        """Ensure the family binary exists (mcrun -c at 1 ray); cached when
        the comment-stripped sha already matches."""
        need, sha = _compile_plan(self.instr, mpi=None)
        if not need:
            return {"ok": True, "binary": self.binary, "cached": True,
                    "compile_s": 0.0}
        out = os.path.join(self.workdir, f"_compile_{os.getpid()}")
        os.makedirs(self.workdir, exist_ok=True)
        shutil.rmtree(out, ignore_errors=True)
        # every parameter passed explicitly: a paramless CLI would hit the
        # interactive mcreadparams prompt (M0 gotcha)
        cmd = ([mcrun_path(), self.instr, "-c", "-n", "1", "-s", "1", "-d", out]
               + [f"{k}={v}" for k, v in self.defaults.items()
                  if v is not None])
        t0 = time.time()
        try:
            proc = subprocess.run(cmd, cwd=os.path.dirname(self.instr),
                                  capture_output=True, text=True,
                                  stdin=subprocess.DEVNULL, timeout=timeout)
        except subprocess.TimeoutExpired:
            return {"ok": False, "stage": "compile",
                    "diagnostics": [f"compile timed out after {timeout}s"]}
        finally:
            shutil.rmtree(out, ignore_errors=True)
        if proc.returncode != 0 or not os.path.isfile(self.binary):
            tail = (proc.stdout + proc.stderr).splitlines()[-15:]
            return {"ok": False, "stage": "compile", "diagnostics": tail}
        _write_build_meta({"instr": self.instr, "instr_sha": sha, "mpi": None})
        return {"ok": True, "binary": self.binary, "cached": False,
                "compile_s": round(time.time() - t0, 2)}

    def run(self, params: dict, ncount: float, seed: int,
            timeout: int = RUN_TIMEOUT_S, keep_output: bool = False) -> dict:
        """One fast-tier rollout. Returns {'ok', 'summary', 'elapsed_s'} on
        success; {'ok': False, 'stage', 'diagnostics'} on failure. The
        output dir is deleted after parsing unless keep_output."""
        if not seed:
            return {"ok": False, "stage": "setup",
                    "diagnostics": ["seed must be a nonzero int "
                                    "(mccode requirement)"]}
        if ncount < 1:
            return {"ok": False, "stage": "setup",
                    "diagnostics": [f"ncount must be >= 1 (got {ncount:g})"]}
        if not os.path.isfile(self.binary):
            return {"ok": False, "stage": "setup",
                    "diagnostics": ["family binary missing — call compile() "
                                    "first"]}
        os.makedirs(self.workdir, exist_ok=True)
        out = os.path.join(self.workdir,
                           f"r{os.getpid()}_{next(self._seq)}")
        shutil.rmtree(out, ignore_errors=True)  # leaf must not pre-exist
        cmd = ([self.binary, "-n", str(int(ncount)), "-s", str(int(seed)),
                "-d", out] + [f"{k}={v}" for k, v in (params or {}).items()])
        t0 = time.time()
        try:
            proc = subprocess.run(cmd, capture_output=True, text=True,
                                  stdin=subprocess.DEVNULL, timeout=timeout)
        except subprocess.TimeoutExpired:
            shutil.rmtree(out, ignore_errors=True)
            return {"ok": False, "stage": "run",
                    "diagnostics": [f"rollout timed out after {timeout}s"]}
        elapsed = time.time() - t0
        if proc.returncode != 0:
            tail = (proc.stdout + proc.stderr).splitlines()[-15:]
            shutil.rmtree(out, ignore_errors=True)
            return {"ok": False, "stage": "run", "diagnostics": tail}
        try:
            summary = results.summarize(out)
        except (FileNotFoundError, ValueError) as e:
            shutil.rmtree(out, ignore_errors=True)
            return {"ok": False, "stage": "parse", "diagnostics": [str(e)]}
        if not keep_output:
            shutil.rmtree(out, ignore_errors=True)
            summary.pop("output_dir", None)
        return {"ok": True, "summary": summary,
                "elapsed_s": round(elapsed, 4)}
