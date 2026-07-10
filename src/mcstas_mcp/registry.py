"""Instrument registry: declarative JSON specs + call-time validation.

The spec (not a live McStasScript object, not a .instr file) is the source
of truth — deterministic, diffable, survives restarts. McStasScript objects
are rebuilt from the spec when a .instr file is needed.

Validation happens at tool-call time (fail at the cheapest point):
  - component type exists (else nearest matches)
  - instance names unique; RELATIVE targets exist
  - parameter names valid for the component (else nearest matches)
  - identifiers in parameter values resolve to instrument parameters or C
    math (closes McStasScript's isalpha() loophole)
  - string-typed parameters are auto-quoted for McStas ('"file.dat"')
Missing required params are WARNINGS at build time and errors at run time.
"""

import difflib
import json
import os
import re
import threading
import time

from . import catalog
from .config import home_dir

# identifiers legal in C expressions without declaration
C_ALLOW = {
    "sin", "cos", "tan", "asin", "acos", "atan", "atan2", "sinh", "cosh",
    "tanh", "exp", "log", "log10", "pow", "sqrt", "fabs", "fmod", "floor",
    "ceil", "PI", "M_PI", "NAN", "INFINITY", "RAD2DEG", "DEG2RAD",
}
IDENT_RE = re.compile(r"[A-Za-z_][A-Za-z0-9_]*")
NUMBER_RE = re.compile(r"^[-+]?(\d+\.?\d*|\.\d+)([eE][-+]?\d+)?$")


class SpecError(ValueError):
    """Validation failure; message tells the agent what to do next."""


# serialize load-mutate-save cycles: FastMCP runs tools in worker threads and
# concurrent mutations of one spec would otherwise silently lose updates
spec_lock = threading.RLock()


def _instruments_dir():
    d = os.path.join(home_dir(), "instruments")
    os.makedirs(d, exist_ok=True)
    return d


def workdir(name: str) -> str:
    d = os.path.join(_instruments_dir(), name)
    os.makedirs(d, exist_ok=True)
    return d


def _spec_path(name: str) -> str:
    # no makedirs here: exists()/load() must not create directories
    return os.path.join(_instruments_dir(), name, "spec.json")


def exists(name: str) -> bool:
    return os.path.isfile(_spec_path(name))


def load(name: str) -> dict:
    if not exists(name):
        known = list_instruments()
        hint = f" Known instruments: {', '.join(known)}." if known else ""
        raise SpecError(
            f"No instrument named '{name}'. Create it with create_instrument.{hint}"
        )
    with open(_spec_path(name)) as f:
        return json.load(f)


def save(spec: dict):
    spec["modified"] = time.strftime("%Y-%m-%d %H:%M:%S")
    path = _spec_path(spec["name"])
    tmp = path + ".tmp"
    with open(tmp, "w") as f:  # atomic: a crash mid-write can't corrupt the spec
        json.dump(spec, f, indent=2)
    os.replace(tmp, path)


def list_instruments():
    d = _instruments_dir()
    return sorted(n for n in os.listdir(d) if os.path.isfile(os.path.join(d, n, "spec.json")))


def create(name: str, description: str = "") -> dict:
    if not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", name):
        raise SpecError(
            f"'{name}' is not a valid instrument name (letters/digits/underscore, "
            "not starting with a digit)."
        )
    if exists(name):
        raise SpecError(
            f"Instrument '{name}' already exists. Use it directly, or pick a new name."
        )
    spec = {
        "name": name,
        "description": description,
        "parameters": [],   # {name, default, unit, comment}
        "components": [],   # {name, component, at, relative, rotated, rotated_relative, parameters}
        "created": time.strftime("%Y-%m-%d %H:%M:%S"),
    }
    workdir(name)  # create() is the only reader/writer allowed to make the dir
    save(spec)
    return spec


def add_parameter(spec: dict, name: str, default=None, unit: str = "", comment: str = ""):
    if not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", name):
        raise SpecError(
            f"'{name}' is not a valid parameter name (letters/digits/underscore, "
            "not starting with a digit)."
        )
    if any(p["name"] == name for p in spec["parameters"]):
        raise SpecError(f"Instrument parameter '{name}' already exists.")
    if name in _component_names(spec):
        raise SpecError(f"'{name}' is already a component name in this instrument.")
    spec["parameters"].append(
        {"name": name, "default": default, "unit": unit, "comment": comment}
    )
    save(spec)


def _param_names(spec):
    return {p["name"] for p in spec["parameters"]}


def _component_names(spec):
    return [c["name"] for c in spec["components"]]


def _check_expression(spec: dict, context: str, v: str) -> str:
    """Validate a string as a scalar C expression over known names.

    Rejects unknown identifiers (closing McStasScript's isalpha() loophole)
    and statement-like punctuation that C would silently mis-evaluate — e.g.
    '(1, 2)' compiles via the comma operator and yields 2: wrong physics
    with no error anywhere.
    """
    without_strings = re.sub(r'"[^"]*"', "", v)
    if re.search(r"[,\[\]{};=?]", without_strings):
        raise SpecError(
            f"{context}: value '{v}' contains characters not allowed in a scalar "
            "expression (, [ ] {{ }} ; = ?). Pass a single number or an arithmetic "
            "expression over instrument parameters."
        )
    # strip numeric/hex literals first so 1e-3 does not tokenize as ident 'e'
    stripped = re.sub(
        r"(?<![\w.])(0[xX][0-9a-fA-F]+|\d+\.?\d*([eE][-+]?\d+)?)", " ", without_strings
    )
    unknown = [
        t for t in IDENT_RE.findall(stripped)
        if t not in _param_names(spec) and t not in C_ALLOW
    ]
    if unknown:
        raise SpecError(
            f"{context}: unknown identifier(s) {unknown} in value '{v}'. Use a "
            "number, or first define instrument parameter(s) with add_parameter."
        )
    return v


def _check_value(spec: dict, comp_type: str, pname: str, value):
    """Validate one parameter value; returns the (possibly coerced) value."""
    ptype = catalog.param_types(comp_type).get(pname, "double")
    if isinstance(value, bool):  # JSON true/false — natural for 0/1 flag params
        return int(value)
    if isinstance(value, (int, float)):
        if ptype == "string" and value != 0:
            raise SpecError(
                f"'{pname}' of '{comp_type}' is a string parameter — pass a file "
                f"name string, not the number {value} (0 means 'no file')."
            )
        return value
    if not isinstance(value, str):
        raise SpecError(
            f"Parameter '{pname}' of '{comp_type}' got {type(value).__name__} "
            f"{value!r} — values must be numbers or strings, not lists/objects."
        )
    v = value.strip()
    if ptype == "string":
        if v in ("0", "NULL"):  # McStas convention for "no file"
            return 0
        if not (v.startswith('"') and v.endswith('"')):
            if v in _param_names(spec):
                return v  # references a string instrument parameter
            return f'"{v}"'  # auto-quote literals: file.dat -> "file.dat"
        return v
    # numeric parameter given as a string: number, or expression over known names
    if NUMBER_RE.match(v):
        return v
    return _check_expression(spec, f"Parameter '{pname}' of '{comp_type}'", v)


def _check_vector(spec: dict, label: str, vec):
    """Validate an AT/ROTATED 3-vector; elements may be numbers or expressions."""
    if not (isinstance(vec, (list, tuple)) and len(vec) == 3):
        raise SpecError(f"'{label}' must be a 3-list [x, y, z].")
    out = []
    for el in vec:
        if isinstance(el, bool) or not isinstance(el, (int, float, str)):
            raise SpecError(f"'{label}' elements must be numbers or expression strings.")
        if isinstance(el, str) and not NUMBER_RE.match(el.strip()):
            _check_expression(spec, f"'{label}' element", el.strip())
        out.append(el)
    return out


def _validate_params(spec: dict, comp_type: str, params: dict) -> dict:
    valid = set(catalog.param_types(comp_type))
    cleaned = {}
    for k, v in params.items():
        if k not in valid:
            near = difflib.get_close_matches(k, list(valid), n=3, cutoff=0.4)
            hint = f" Did you mean: {', '.join(near)}?" if near else ""
            raise SpecError(
                f"'{comp_type}' has no parameter '{k}'.{hint} "
                f"Use describe_component('{comp_type}') for the full list."
            )
        cleaned[k] = _check_value(spec, comp_type, k, v)
    return cleaned


def add_component(spec: dict, name: str, component: str, at, relative=None,
                  rotated=None, rotated_relative=None, parameters=None, after=None):
    """Append (or insert) a component; returns list of warning strings."""
    if not catalog.exists(component):
        near = catalog.nearest(component)
        hint = f" Nearest matches: {', '.join(near)}." if near else ""
        raise SpecError(
            f"No component type '{component}' in the McStas library.{hint} "
            "Use list_components(search=...) to browse."
        )
    if name in _component_names(spec):
        raise SpecError(
            f"Component instance name '{name}' is already used. Names must be unique."
        )
    if name in _param_names(spec):
        raise SpecError(f"'{name}' is already an instrument parameter name.")
    at = _check_vector(spec, "at", at)
    if rotated is not None:
        rotated = _check_vector(spec, "rotated", rotated)
    elif rotated_relative:
        raise SpecError("'rotated_relative' given without 'rotated' — it would be "
                        "silently ignored; pass rotated=[rx, ry, rz] too.")
    for ref, label in ((relative, "relative"), (rotated_relative, "rotated_relative")):
        if ref and ref != "ABSOLUTE" and ref not in _component_names(spec):
            raise SpecError(
                f"'{label}' references '{ref}' which is not a component of this "
                f"instrument (have: {', '.join(_component_names(spec)) or 'none'})."
            )
    if after is not None and after not in _component_names(spec):
        raise SpecError(f"'after' references unknown component '{after}'.")

    entry = {
        "name": name,
        "component": component,
        "at": list(at),
        "relative": relative,
        "rotated": list(rotated) if rotated else None,
        "rotated_relative": rotated_relative,
        "parameters": _validate_params(spec, component, parameters or {}),
    }
    if after is None:
        spec["components"].append(entry)
    else:
        idx = _component_names(spec).index(after) + 1
        spec["components"].insert(idx, entry)
    save(spec)

    missing = [p for p in catalog.required_params(component) if p not in entry["parameters"]]
    if missing:
        return [
            f"'{name}' ({component}) is missing required parameter(s): "
            f"{', '.join(missing)} — set them before running."
        ]
    return []


def set_parameters(spec: dict, component_name: str, parameters: dict):
    for c in spec["components"]:
        if c["name"] == component_name:
            c["parameters"].update(_validate_params(spec, c["component"], parameters))
            save(spec)
            missing = [
                p for p in catalog.required_params(c["component"])
                if p not in c["parameters"]
            ]
            return (
                [f"'{component_name}' still missing required: {', '.join(missing)}"]
                if missing else []
            )
    raise SpecError(
        f"No component named '{component_name}' in instrument '{spec['name']}' "
        f"(have: {', '.join(_component_names(spec)) or 'none'})."
    )


def remove_component(spec: dict, component_name: str):
    names = _component_names(spec)
    if component_name not in names:
        raise SpecError(
            f"No component named '{component_name}' to remove "
            f"(have: {', '.join(names) or 'none'})."
        )
    dependents = [
        c["name"] for c in spec["components"]
        if component_name in (c["relative"], c["rotated_relative"]) and c["name"] != component_name
    ]
    if dependents:
        raise SpecError(
            f"Cannot remove '{component_name}': component(s) {', '.join(dependents)} "
            "are positioned RELATIVE to it. Re-anchor or remove those first."
        )
    spec["components"] = [c for c in spec["components"] if c["name"] != component_name]
    save(spec)


def missing_required(spec: dict):
    """[(component_name, [missing params])] across the instrument."""
    out = []
    for c in spec["components"]:
        miss = [p for p in catalog.required_params(c["component"]) if p not in c["parameters"]]
        if miss:
            out.append((c["name"], miss))
    return out


# McStasScript caches dynamically-generated component classes globally and
# FastMCP runs sync tools in worker threads — serialize instrument builds.
_build_lock = threading.Lock()


def build_instr_file(spec: dict) -> str:
    """Rebuild a McStasScript instrument from the spec and write name.instr.

    Returns the .instr path. McStasScript's own immediate checks act as a
    second validation layer; its write-time check_for_errors() runs too.
    """
    import mcstasscript as ms

    wd = workdir(spec["name"])
    with _build_lock:
        # absolute input_path keeps all McStas_instr side effects (<name>_db/,
        # the .instr itself) in the workdir without a process-global chdir
        instr = ms.McStas_instr(spec["name"], input_path=wd)
        for p in spec["parameters"]:
            # never pass unit= to McStasScript: libpyvinyl validates it with
            # pint, which rejects McStas units like "AA" — fold into comment
            note = f"[{p['unit']}] {p['comment']}".strip("[] ") if p["unit"] else p["comment"]
            instr.add_parameter(p["name"], value=p["default"], comment=note or "")
        for c in spec["components"]:
            kwargs = {"AT": c["at"]}
            if c["relative"] and c["relative"] != "ABSOLUTE":
                kwargs["RELATIVE"] = c["relative"]
            if c["rotated"]:
                kwargs["ROTATED"] = c["rotated"]
                if c["rotated_relative"] and c["rotated_relative"] != "ABSOLUTE":
                    kwargs["ROTATED_RELATIVE"] = c["rotated_relative"]
            comp = instr.add_component(c["name"], c["component"], **kwargs)
            if c["parameters"]:
                comp.set_parameters(**c["parameters"])
        instr.write_full_instrument()
    return os.path.join(wd, f"{spec['name']}.instr")


def instr_source(spec: dict) -> str:
    path = build_instr_file(spec)
    with open(path) as f:
        return f.read()
