"""Paths and shared helpers."""

import functools
import os
import re
import shutil
import subprocess

ANSI_RE = re.compile(r"\x1b\[[0-9;]*m")


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
    path = shutil.which("mcrun")
    if not path:
        raise RuntimeError(
            "mcrun not found on PATH — run inside the 'mcstas' conda env"
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
