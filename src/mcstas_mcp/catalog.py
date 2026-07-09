"""Component catalog: JSON-friendly introspection over the McStas library.

Wraps McStasScript's ComponentReader (parses SETTING PARAMETERS + %P docs
from .comp files). All shipped McStasScript helpers are print-only; this
module returns data. Parsed metadata is cached in memory for the server's
lifetime (374 .comp files ≈ one-time cost).
"""

import difflib
import functools
import re

from .config import resources_dir


@functools.lru_cache(maxsize=1)
def _reader():
    from mcstasscript.helper.component_reader import ComponentReader

    # input_path must exist but is irrelevant here (no local .comp overrides)
    return ComponentReader(resources_dir(), input_path=".")


@functools.lru_cache(maxsize=1)
def _index():
    """{name: {category, path}} for every installed component."""
    r = _reader()
    return {
        name: {"category": r.component_category[name], "path": r.component_path[name]}
        for name in r.component_path
    }


@functools.lru_cache(maxsize=512)
def _one_line_doc(name: str) -> str:
    """First sentence of the %D block of a .comp file."""
    try:
        with open(_index()[name]["path"], errors="replace") as f:
            text = f.read(8000)
    except OSError:
        return ""
    m = re.search(r"%D(?:escription)?\s*\n(.*?)(?:\n\s*\*?\s*\n|%)", text, re.DOTALL)
    if not m:
        return ""
    line = " ".join(
        ln.strip().lstrip("*").strip() for ln in m.group(1).splitlines()
    ).strip()
    return (line[:157] + "...") if len(line) > 160 else line


def component_names():
    return sorted(_index())


def nearest(name: str, n: int = 5):
    return difflib.get_close_matches(name, list(_index()), n=n, cutoff=0.4)


def exists(name: str) -> bool:
    return name in _index()


def list_components(category=None, search=None):
    out = []
    for name, meta in sorted(_index().items()):
        if category and meta["category"] != category:
            continue
        if search:
            key = search.lower().replace("_", "")
            hay = name.lower().replace("_", "") + _one_line_doc(name).lower()
            if key not in hay:
                continue
        out.append(
            {"name": name, "category": meta["category"], "doc": _one_line_doc(name)}
        )
    return out


def categories():
    counts = {}
    for meta in _index().values():
        counts[meta["category"]] = counts.get(meta["category"], 0) + 1
    return counts


@functools.lru_cache(maxsize=512)
def describe(name: str):
    """Full parameter metadata for one component. Raises KeyError if unknown."""
    if name not in _index():
        raise KeyError(name)
    info = _reader().read_name(name)
    params = []
    for p in info.parameter_names:
        default = info.parameter_defaults.get(p)
        params.append(
            {
                "name": p,
                "type": info.parameter_types.get(p) or "double",
                "unit": info.parameter_units.get(p, ""),
                "default": default,
                "required": default is None,
                "doc": info.parameter_comments.get(p, ""),
            }
        )
    return {
        "name": name,
        "category": info.category,
        "description": _one_line_doc(name),
        "parameters": params,
    }


def required_params(name: str):
    return [p["name"] for p in describe(name)["parameters"] if p["required"]]


def param_types(name: str):
    return {p["name"]: p["type"] for p in describe(name)["parameters"]}
