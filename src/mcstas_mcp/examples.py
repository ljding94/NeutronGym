"""Shipped-example corpus: 297 .instr files with %Example ground-truth lines.

Few-shot material for agents (get_example) and the seed of benchmark T1
task discovery. Metadata is parsed lazily from headers and cached.
"""

import functools
import glob
import os
import re

from .config import resources_dir


@functools.lru_cache(maxsize=1)
def _index():
    root = os.path.join(resources_dir(), "examples")
    out = {}
    for p in sorted(glob.glob(os.path.join(root, "**", "*.instr"), recursive=True)):
        out.setdefault(os.path.splitext(os.path.basename(p))[0], p)
    return out


@functools.lru_cache(maxsize=512)
def _meta(name: str):
    with open(_index()[name], errors="replace") as f:
        head = f.read(8000)
    site = re.search(r"%INSTRUMENT_SITE:\s*(\S+)", head)
    desc = re.search(r"%D(?:escription)?\s*\n(.*?)\n\s*(?:\*\s*)?\n", head, re.DOTALL)
    desc_line = ""
    if desc:
        desc_line = " ".join(
            ln.strip().lstrip("*").strip() for ln in desc.group(1).splitlines()
        ).strip()[:160]
    example = re.search(r"%Example:\s*(.+)", head)
    return {
        "name": name,
        "site": site.group(1) if site else "",
        "doc": desc_line,
        "example_line": example.group(1).strip() if example else None,
    }


def list_examples(search: str | None = None):
    out = []
    for name in _index():
        meta = _meta(name)
        if search:
            key = search.lower().replace("_", "")
            hay = (name + meta["site"] + meta["doc"]).lower().replace("_", "")
            if key not in hay:
                continue
        out.append(meta)
    return out


def get_example(name: str):
    if name not in _index():
        near = [n for n in _index()
                if name.lower().replace("_", "") in n.lower().replace("_", "")]
        hint = f" Did you mean: {', '.join(near[:5])}?" if near else ""
        raise KeyError(f"No shipped example named '{name}'.{hint} "
                       "Use list_examples(search=...) to browse.")
    path = _index()[name]
    with open(path, errors="replace") as f:
        source = f.read()
    return {**_meta(name), "path": path, "source": source}
