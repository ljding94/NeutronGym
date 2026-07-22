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
        spec = json.load(f)
    # migrate specs written before M2
    spec.setdefault("declares", [])
    spec.setdefault("initialize", None)
    for p in spec["parameters"]:
        p.setdefault("type", "double")
    for c in spec["components"]:
        for k in ("when", "extend", "group", "split"):
            c.setdefault(k, None)
    return spec


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
        "parameters": [],   # {name, type, default, unit, comment}
        "declares": [],     # {type, name, value, array}
        "initialize": None,  # raw C for the INITIALIZE block (escape hatch)
        "components": [],   # {name, component, at, relative, rotated,
                            #  rotated_relative, parameters, when, extend,
                            #  group, split}
        "created": time.strftime("%Y-%m-%d %H:%M:%S"),
    }
    workdir(name)  # create() is the only reader/writer allowed to make the dir
    save(spec)
    return spec


def _check_new_name(spec: dict, name: str, what: str):
    if not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", name):
        raise SpecError(
            f"'{name}' is not a valid {what} name (letters/digits/underscore, "
            "not starting with a digit)."
        )
    if any(p["name"] == name for p in spec["parameters"]):
        raise SpecError(f"'{name}' is already an instrument parameter name.")
    if any(d["name"] == name for d in spec["declares"]):
        raise SpecError(f"'{name}' is already a declared variable name.")
    if name in _component_names(spec):
        raise SpecError(f"'{name}' is already a component name in this instrument.")


def add_parameter(spec: dict, name: str, default=None, unit: str = "",
                  comment: str = "", ptype: str | None = None):
    _check_new_name(spec, name, "parameter")
    if ptype is None:
        ptype = "string" if isinstance(default, str) and not NUMBER_RE.match(default) \
            else "double"
    if ptype not in ("double", "int", "string"):
        raise SpecError(f"parameter type must be double, int, or string (got '{ptype}').")
    spec["parameters"].append(
        {"name": name, "type": ptype, "default": default, "unit": unit, "comment": comment}
    )
    save(spec)


def add_declare(spec: dict, dtype: str, name: str, value=None, array: int = 0):
    """DECLARE-block variable, usable in EXTEND code and WHEN conditions."""
    if dtype not in ("double", "int", "string"):
        raise SpecError(f"declare type must be double, int, or string (got '{dtype}').")
    _check_new_name(spec, name, "declare")
    spec["declares"].append({"type": dtype, "name": name, "value": value, "array": array})
    save(spec)


def _param_names(spec):
    return {p["name"] for p in spec["parameters"]}


def _known_names(spec):
    """Identifiers legal in expressions: instrument params + declares."""
    return _param_names(spec) | {d["name"] for d in spec["declares"]}


def _string_names(spec):
    return ({p["name"] for p in spec["parameters"] if p.get("type") == "string"}
            | {d["name"] for d in spec["declares"] if d.get("type") == "string"})


# per-ray state vars, legal in WHEN conditions and EXTEND code
PARTICLE_VARS = {"x", "y", "z", "vx", "vy", "vz", "t", "sx", "sy", "sz", "p",
                 "SCATTERED", "ABSORBED"}


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
    idents = IDENT_RE.findall(stripped)
    unknown = [t for t in idents if t not in _known_names(spec) and t not in C_ALLOW]
    if unknown:
        raise SpecError(
            f"{context}: unknown identifier(s) {unknown} in value '{v}'. Use a "
            "number, or first define instrument parameter(s) with add_parameter."
        )
    stringy = [t for t in idents if t in _string_names(spec)]
    if stringy:
        raise SpecError(
            f"{context}: '{', '.join(stringy)}' is a string-typed name and cannot "
            "be used in a numeric expression."
        )
    return v


def _check_when(spec: dict, v: str) -> str:
    """Validate a WHEN condition: logical/comparison ops allowed, identifiers
    must resolve to instrument params, declares, or per-ray state vars."""
    without_strings = re.sub(r'"[^"]*"', "", v)
    if re.search(r"[;{}\[\]]", without_strings):
        raise SpecError(
            f"WHEN condition '{v}' contains statement punctuation (; {{ }} [ ]) — "
            "pass a boolean expression like 'lambda > 2 && flag == 1'."
        )
    stripped = re.sub(
        r"(?<![\w.])(0[xX][0-9a-fA-F]+|\d+\.?\d*([eE][-+]?\d+)?)", " ", without_strings
    )
    unknown = [
        t for t in IDENT_RE.findall(stripped)
        if t not in _known_names(spec) and t not in C_ALLOW
        and t not in PARTICLE_VARS
    ]
    if unknown:
        raise SpecError(
            f"WHEN condition '{v}': unknown identifier(s) {unknown}. Usable names: "
            "instrument parameters, add_declare variables, per-ray state "
            "(x, y, z, vx, vy, vz, t, p, SCATTERED)."
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
            if v in _known_names(spec):
                ref = next((p for p in spec["parameters"] if p["name"] == v), None) \
                    or next(d for d in spec["declares"] if d["name"] == v)
                if ref.get("type") == "string":
                    return v  # references a string instrument parameter/declare
                raise SpecError(
                    f"'{pname}' of '{comp_type}' needs a string, but '{v}' is a "
                    f"{ref.get('type', 'double')} — add_parameter(..., "
                    "ptype='string') for file-name parameters."
                )
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
                  rotated=None, rotated_relative=None, parameters=None, after=None,
                  when=None, extend=None, group=None, split=None, _validate=True):
    """Append (or insert) a component; returns list of warning strings.

    when: boolean condition (validated); extend: raw C appended after the
    component's TRACE (escape hatch, not validated); group: exclusive-group
    name; split: SPLIT count for variance reduction. _validate=False is used
    only by load_from_instr (values come from a real .instr file).
    """
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
    if when is not None and _validate:
        when = _check_when(spec, str(when).strip())
    if group is not None and not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", str(group)):
        raise SpecError(f"'{group}' is not a valid GROUP name.")
    if split is not None:
        if isinstance(split, bool) or not isinstance(split, int) or split < 1:
            raise SpecError("'split' must be a positive integer (SPLIT ray count).")
    at = _check_vector(spec, "at", at) if _validate else list(at)
    if rotated is not None:
        rotated = _check_vector(spec, "rotated", rotated) if _validate else list(rotated)
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
        "parameters": (_validate_params(spec, component, parameters or {})
                       if _validate else dict(parameters or {})),
        "when": when,
        "extend": extend,
        "group": group,
        "split": split,
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
            kwargs = {"comment": note or ""}
            if p["default"] is not None:
                default = p["default"]
                if p.get("type") == "string" and not str(default).startswith('"'):
                    default = f'"{default}"'
                kwargs["value"] = default
            if p.get("type", "double") in ("int", "string"):
                instr.add_parameter(p["type"], p["name"], **kwargs)
            else:
                instr.add_parameter(p["name"], **kwargs)
        for d in spec["declares"]:
            dkw = {}
            if d.get("value") is not None:
                dkw["value"] = d["value"]
            if d.get("array"):
                dkw["array"] = d["array"]
            instr.add_declare_var(d["type"], d["name"], **dkw)
        if spec.get("initialize"):
            instr.append_initialize(spec["initialize"])
        for c in spec["components"]:
            kwargs = {"AT": c["at"]}
            if c["relative"] and c["relative"] != "ABSOLUTE":
                kwargs["RELATIVE"] = c["relative"]
            if c["rotated"]:
                kwargs["ROTATED"] = c["rotated"]
                if c["rotated_relative"] and c["rotated_relative"] != "ABSOLUTE":
                    kwargs["ROTATED_RELATIVE"] = c["rotated_relative"]
            for key, kw in (("when", "WHEN"), ("extend", "EXTEND"),
                            ("group", "GROUP"), ("split", "SPLIT")):
                if c.get(key):
                    kwargs[kw] = c[key]
            comp = instr.add_component(c["name"], c["component"], **kwargs)
            if c["parameters"]:
                comp.set_parameters(**c["parameters"])
        instr.write_full_instrument()
    return os.path.join(wd, f"{spec['name']}.instr")


def instr_source(spec: dict) -> str:
    path = build_instr_file(spec)
    with open(path) as f:
        return f.read()


def export_instr(spec: dict, dest: str | None = None) -> str:
    """Write the generated .instr; returns its path (workdir by default)."""
    import shutil

    path = build_instr_file(spec)
    if dest:
        dest = os.path.abspath(os.path.expanduser(dest))
        os.makedirs(os.path.dirname(dest), exist_ok=True)
        shutil.copy(path, dest)
        return dest
    return path


def _strip_reader_boilerplate(code: str, name: str) -> str | None:
    code = code.replace(f"// Start of initialize for generated {name}", "").strip()
    return code or None


def load_from_instr(path: str, name: str | None = None):
    """Escape hatch: parse an existing .instr into a spec via McStasScript's
    reader (best-effort — the reader has known failure classes)."""
    import shutil

    import mcstasscript as ms
    from mcstasscript.interface import reader as msreader

    path = os.path.abspath(os.path.expanduser(path))
    if not os.path.isfile(path):
        raise SpecError(f"No file at {path}.")
    name = name or re.sub(r"\W", "_", os.path.splitext(os.path.basename(path))[0])
    if exists(name):
        raise SpecError(
            f"Instrument '{name}' already exists — pass a different name to load into."
        )
    wd = workdir(name)
    with _build_lock:
        try:
            instr = ms.McStas_instr(name, input_path=wd)
            msreader.McStas_file(path).add_to_instr(instr)
        except Exception as e:
            shutil.rmtree(wd, ignore_errors=True)
            raise SpecError(
                f"McStasScript's .instr reader failed on {os.path.basename(path)}: "
                f"{type(e).__name__}: {e}. Known failure classes: DECLARE names "
                "shadowing DEFINE parameters; C lines in EXTEND starting with a "
                "keyword (e.g. 'groupNumber=0;'). Either simplify the file or "
                "rebuild it via add_component."
            ) from e

        spec = create(name, description=f"loaded from {path}")
        for p in instr.parameters:
            unit = getattr(p, "unit", "") or ""
            spec["parameters"].append({
                "name": p.name,
                "type": getattr(p, "type", "") or "double",
                "default": p.value,
                "unit": "" if unit == "dimensionless" else unit,
                "comment": getattr(p, "comment", "") or "",
            })
        for d in instr.declare_list:
            spec["declares"].append({
                "type": getattr(d, "type", "double"),
                "name": d.name,
                "value": getattr(d, "value", None),
                "array": getattr(d, "vector", 0) or 0,
            })
        spec["initialize"] = _strip_reader_boilerplate(
            getattr(instr, "initialize_section", "") or "", name)
        save(spec)

        warnings = []
        for c in instr.component_list:
            rel = (c.AT_relative or "ABSOLUTE").replace("RELATIVE", "").strip() or None
            rot_rel = (c.ROTATED_relative or "ABSOLUTE").replace("RELATIVE", "").strip() or None
            rotated = [str(x).strip() for x in c.ROTATED_data]
            has_rot = rotated != ["0", "0", "0"] or rot_rel is not None
            when = (c.WHEN or "").removeprefix("WHEN").strip() or None
            params = {p: getattr(c, p) for p in c.parameter_names
                      if getattr(c, p) is not None}
            try:
                add_component(
                    spec, c.name, c.component_name,
                    at=[str(x).strip() for x in c.AT_data],
                    relative=rel,
                    rotated=rotated if has_rot else None,
                    rotated_relative=rot_rel if has_rot else None,
                    parameters=params,
                    when=when,
                    extend=(c.EXTEND or "").strip() or None,
                    group=(c.GROUP or "").strip() or None,
                    # the reader stores bare 'SPLIT' (no count) as '' — that
                    # is McStas's default SPLIT 10
                    split=(10 if str(c.SPLIT).strip() == "" and c.SPLIT != 0
                           else int(c.SPLIT) if str(c.SPLIT).strip().isdigit()
                           and int(c.SPLIT) > 0 else None),
                    _validate=False,
                )
            except SpecError as e:
                warnings.append(f"{c.name}: {e}")
    return load(name), warnings
