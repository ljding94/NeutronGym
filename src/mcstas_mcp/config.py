"""Paths and shared helpers."""

import functools
import os
import re
import shutil
import subprocess
import sys

ANSI_RE = re.compile(r"\x1b\[[0-9;]*m")

# The dir holding our interpreter — in the conda env this is also where
# mcrun/mcstas live (and where the mcstas-mcp entry point itself is).
ENV_BIN = os.path.dirname(os.path.abspath(sys.executable))


def _ensure_env_bin_on_path():
    """MCP clients may launch the server with a PATH lacking the conda env
    (2026-07-20 acceptance-test incident): prepend our own bin dir so mcrun,
    mcstas, cc discovery, and McStasScript's PATH-based auto-detection all
    resolve regardless of how we were spawned."""
    parts = os.environ.get("PATH", "").split(os.pathsep)
    if ENV_BIN not in parts:
        os.environ["PATH"] = ENV_BIN + os.pathsep + os.environ.get("PATH", "")


_ensure_env_bin_on_path()


def strip_ansi(text: str) -> str:
    return ANSI_RE.sub("", text)


def home_dir() -> str:
    """Server state root. MCSTAS_MCP_HOME overrides (used by tests)."""
    path = os.environ.get(
        "MCSTAS_MCP_HOME", os.path.join(os.path.expanduser("~"), ".mcstas-mcp")
    )
    os.makedirs(path, exist_ok=True)
    return path


@functools.lru_cache(maxsize=1)
def mcrun_path() -> str:
    # self-locate first: same bin dir as our interpreter (conda env layout)
    candidate = os.path.join(ENV_BIN, "mcrun")
    if os.path.isfile(candidate) and os.access(candidate, os.X_OK):
        return candidate
    path = shutil.which("mcrun")
    if not path:
        raise RuntimeError(
            f"mcrun not found next to the interpreter ({ENV_BIN}) or on PATH — "
            "install mcstas into this environment"
        )
    return path


@functools.lru_cache(maxsize=1)
def resources_dir() -> str:
    """McStas resources dir (components + examples), via mcrun --showcfg."""
    out = subprocess.run(
        [mcrun_path(), "--showcfg=resourcedir"],
        capture_output=True, text=True, timeout=30, stdin=subprocess.DEVNULL,
    )
    path = out.stdout.strip().splitlines()[-1] if out.stdout.strip() else ""
    if not os.path.isdir(path):
        raise RuntimeError(f"could not locate McStas resources (got {path!r})")
    return path
