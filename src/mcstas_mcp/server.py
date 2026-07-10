"""FastMCP server exposing McStas to agents (stdio transport).

Tool-call-time validation returns actionable error messages; simulations
run in server-owned subprocesses with captured diagnostics. See
note/m1-server-design-2026-07-09.md.
"""

from typing import Optional

from fastmcp import FastMCP

from . import catalog, execution, registry, results
from .registry import SpecError
from .execution import RunError

mcp = FastMCP(
    "mcstas",
    instructions=(
        "McStas neutron ray-tracing simulations. Workflow: list/describe "
        "components -> create_instrument -> add_component (beam order: source, "
        "optics, sample, monitors) -> run_simulation at low ncount (1e5-1e6) -> "
        "get_results (never compare designs when a monitor has events < 1000). "
        "Always describe_component before first use of a component type."
    ),
)


def _err(e: Exception) -> dict:
    # KeyError's str() wraps the message in extra quotes — unwrap it
    msg = e.args[0] if isinstance(e, KeyError) and e.args else str(e)
    return {"ok": False, "error": str(msg)}


@mcp.tool
def list_components(category: Optional[str] = None, search: Optional[str] = None) -> dict:
    """List available McStas components. Categories: sources, optics, samples,
    monitors, misc, contrib, union, sasmodels, obsolete. Use search to filter
    by name/description substring."""
    comps = catalog.list_components(category=category, search=search)
    return {"ok": True, "count": len(comps), "categories": catalog.categories(),
            "components": comps}


@mcp.tool
def describe_component(name: str) -> dict:
    """Full parameter table (type, unit, default, required, doc) for one
    component type. Call this before first use of any component."""
    try:
        return {"ok": True, **catalog.describe(name)}
    except KeyError:
        near = catalog.nearest(name)
        return _err(ValueError(
            f"No component '{name}'."
            + (f" Nearest matches: {', '.join(near)}." if near else "")
        ))


@mcp.tool
def create_instrument(name: str, description: str = "") -> dict:
    """Create a new empty instrument. The name is the instrument_id used by
    all other tools."""
    try:
        spec = registry.create(name, description)
        return {"ok": True, "instrument_id": spec["name"]}
    except SpecError as e:
        return _err(e)


@mcp.tool
def add_parameter(instrument_id: str, name: str, default: Optional[float] = None,
                  unit: str = "", comment: str = "") -> dict:
    """Add an instrument-level parameter (settable per run), e.g. wavelength."""
    try:
        with registry.spec_lock:
            spec = registry.load(instrument_id)
            registry.add_parameter(spec, name, default=default, unit=unit, comment=comment)
        return {"ok": True}
    except SpecError as e:
        return _err(e)


@mcp.tool
def add_component(instrument_id: str, name: str, component: str,
                  at: list[float | str], relative: Optional[str] = None,
                  rotated: Optional[list[float | str]] = None,
                  rotated_relative: Optional[str] = None,
                  parameters: Optional[dict] = None,
                  after: Optional[str] = None) -> dict:
    """Append a component to the instrument (beam order matters!).
    at = [x,y,z] meters; elements may be numbers or expressions over instrument
    parameters (e.g. [0, 0, "L1"]). relative = an earlier component's name
    (recommended) or omit for ABSOLUTE. parameters = {param: value}; string
    values that are file names are auto-quoted. Validated immediately against
    the component library."""
    try:
        with registry.spec_lock:
            spec = registry.load(instrument_id)
            warnings = registry.add_component(
                spec, name, component, at, relative=relative, rotated=rotated,
                rotated_relative=rotated_relative, parameters=parameters, after=after,
            )
        return {"ok": True, "warnings": warnings,
                "component_order": [c["name"] for c in spec["components"]]}
    except SpecError as e:
        return _err(e)


@mcp.tool
def set_parameters(instrument_id: str, component_name: str, parameters: dict) -> dict:
    """Set/overwrite parameters on an existing component instance."""
    try:
        with registry.spec_lock:
            spec = registry.load(instrument_id)
            warnings = registry.set_parameters(spec, component_name, parameters)
        return {"ok": True, "warnings": warnings}
    except SpecError as e:
        return _err(e)


@mcp.tool
def remove_component(instrument_id: str, component_name: str) -> dict:
    """Remove a component instance. Refused if other components are positioned
    RELATIVE to it (re-anchor them first)."""
    try:
        with registry.spec_lock:
            spec = registry.load(instrument_id)
            registry.remove_component(spec, component_name)
        return {"ok": True,
                "component_order": [c["name"] for c in spec["components"]]}
    except SpecError as e:
        return _err(e)


@mcp.tool
def get_instrument(instrument_id: str) -> dict:
    """Current instrument state: parameters, component list in beam order,
    and the generated .instr source (also written to disk for the human's
    view_instrument.py)."""
    try:
        spec = registry.load(instrument_id)
    except SpecError as e:
        return _err(e)
    out = {
        "ok": True,
        "instrument_id": spec["name"],
        "parameters": spec["parameters"],
        "components": [
            {k: c[k] for k in ("name", "component", "at", "relative", "parameters")}
            for c in spec["components"]
        ],
        "missing_required": dict(registry.missing_required(spec)),
        "instr_file": registry.workdir(spec["name"]) + f"/{spec['name']}.instr",
    }
    # an incomplete instrument (missing required params) cannot be written to
    # .instr — still return its state so the agent can see what to fix
    try:
        out["instr_source"] = registry.instr_source(spec) if spec["components"] else ""
    except Exception as e:
        out["instr_source"] = None
        out["instr_source_error"] = str(e)
    return out


@mcp.tool
def run_simulation(instrument_id: str, ncount: float = 1e6,
                   parameters: Optional[dict] = None, seed: Optional[int] = None,
                   mpi: Optional[int] = None, gravity: bool = False,
                   timeout: int = 600) -> dict:
    """Compile and run the instrument (synchronous; ncount capped at 1e8,
    timeout capped at 1800 s). Iterate at ncount 1e5-1e6; go high only for
    final validation. Returns job_id for get_results, plus per-detector
    totals. On failure returns the stage (translate/compile/run) and
    diagnostics."""
    try:
        spec = registry.load(instrument_id)
        job = execution.run_spec(spec, ncount=ncount, parameters=parameters,
                                 seed=seed, mpi=mpi, gravity=gravity,
                                 timeout=min(int(timeout), 1800))
        return job
    except (SpecError, RunError) as e:
        return _err(e)
    except Exception as e:
        # McStasScript write-time checks (McStasError etc.) must reach the
        # agent as a clean message, not a traceback
        return _err(e)


@mcp.tool
def get_results(job_id: str) -> dict:
    """Summary statistics per monitor for a finished job: integrated
    intensity/error/events, beam center and width, signal min/max/mean.
    Monitors listed in low_statistics (<1000 events) are unreliable —
    rerun with higher ncount before drawing conclusions."""
    try:
        return execution.job_results(job_id)
    except (RunError, FileNotFoundError) as e:
        return _err(e)


@mcp.tool
def get_monitor_data(job_id: str, monitor: str, format: str = "png",
                     log: bool = True) -> dict:
    """One monitor in detail. format='png' renders the plot and returns its
    file path (view it with the Read tool). format='array' returns a
    downsampled numeric curve (1D) or x/y profiles (2D). monitor = component
    name or, for components writing several files, the filename."""
    if format not in ("png", "array"):
        return _err(ValueError(f"format must be 'png' or 'array', got '{format}'"))
    try:
        job = execution.get_job(job_id)
        if not job["ok"]:
            return _err(RunError(f"job {job_id} failed; no data to plot"))
        if format == "png":
            path = results.monitor_png(job["output_dir"], monitor, log=log)
            return {"ok": True, "png_path": path,
                    "hint": "use the Read tool on png_path to view the image"}
        return {"ok": True, **results.monitor_array(job["output_dir"], monitor)}
    except (RunError, KeyError, FileNotFoundError) as e:
        return _err(e)


@mcp.tool
def list_instruments() -> dict:
    """Instruments in the registry (survive server restarts)."""
    return {"ok": True, "instruments": registry.list_instruments()}


def main():
    mcp.run()


if __name__ == "__main__":
    main()
