"""Results: summary statistics from mccode.sim + monitor rendering.

get_results never returns raw arrays (they waste agent context). The
mccode.sim 'values:' line gives integrated I/I_err/N per monitor; the
'statistics:' line gives beam center/width — parsed here directly (fast,
no McStasScript needed). PNG rendering goes through McStasScript's
verified-headless make_sub_plot.
"""

import os
import re


def _parse_data_blocks(sim_text: str):
    blocks = re.findall(r"begin data\n(.*?)end data", sim_text, re.DOTALL)
    for raw in blocks:
        entry = {}
        for line in raw.splitlines():
            key, _, val = line.strip().partition(": ")
            entry[key.strip()] = val.strip()
        yield entry


def _stats_dict(s: str):
    return {k: float(v) for k, v in re.findall(r"(\w+)=([-\d.eE+]+)", s or "")}


def summarize(output_dir: str) -> dict:
    sim = os.path.join(output_dir, "mccode.sim")
    if not os.path.isfile(sim):
        raise FileNotFoundError(f"no mccode.sim in {output_dir} — did the run succeed?")
    with open(sim, errors="replace") as f:
        text = f.read()

    ncount = re.search(r"^\s*Ncount: (\S+)", text, re.M)
    seed = re.search(r"^\s*Seed: (\S+)", text, re.M)
    params = dict(re.findall(r"^\s*Param: (\w+)=(\S+)", text, re.M))

    monitors = []
    for b in _parse_data_blocks(text):
        values = (b.get("values") or "").split()
        dims = re.match(r"array_(\d)d\((.+?)\)", b.get("type", ""))
        stats = _stats_dict(b.get("statistics", ""))
        signal = _stats_dict(b.get("signal", ""))
        total_i = float(values[0]) if values else 0.0
        total_e = float(values[1]) if len(values) > 1 else 0.0
        total_n = float(values[2]) if len(values) > 2 else 0.0
        monitors.append({
            "component": b.get("component", ""),
            "filename": b.get("filename", ""),
            "title": b.get("title", ""),
            "dims": [int(x) for x in dims.group(2).split(",")] if dims else [],
            "position": [float(x) for x in b.get("position", "0 0 0").split()],
            "intensity": total_i,          # integrated [n/s]
            "intensity_err": total_e,
            "events": total_n,             # statistics quality gate
            "relative_err": round(total_e / total_i, 4) if total_i else None,
            "beam_center": {k: v for k, v in stats.items() if k in ("X0", "Y0")},
            "beam_width": {k: v for k, v in stats.items() if k in ("dX", "dY")},
            "signal": signal,              # Min/Max/Mean per bin
            "xlabel": b.get("xlabel", ""),
            "ylabel": b.get("ylabel", ""),
            "limits": b.get("xylimits") or b.get("xlimits", ""),
        })
    return {
        "output_dir": output_dir,
        "ncount": float(ncount.group(1)) if ncount else None,
        "seed": int(seed.group(1)) if seed else None,
        "run_parameters": params,
        "monitors": monitors,
        "low_statistics": [m["component"] for m in monitors if m["events"] < 1000],
    }


def _find_monitor(data, monitor: str):
    for d in data:
        if d.name == monitor or getattr(d.metadata, "filename", "") == monitor:
            return d
    names = ", ".join(d.name for d in data)
    raise KeyError(f"no monitor '{monitor}' in this run (have: {names})")


def monitor_png(output_dir: str, monitor: str, log: bool = True) -> str:
    """Render one monitor to PNG; returns the file path."""
    import matplotlib

    matplotlib.use("Agg")
    import mcstasscript as ms

    data = ms.load_data(output_dir)
    d = _find_monitor(data, monitor)
    png = os.path.join(output_dir, f"{d.name}.png")
    ms.make_sub_plot([d], filename=png, log=log)
    return png


def monitor_array(output_dir: str, monitor: str, max_points: int = 200) -> dict:
    """Downsampled numeric view of one monitor (1D: curve; 2D: profiles)."""
    import numpy as np
    import mcstasscript as ms

    data = ms.load_data(output_dir)
    d = _find_monitor(data, monitor)
    intensity = np.asarray(d.Intensity)

    def ds(arr):
        arr = np.asarray(arr)
        step = max(1, len(arr) // max_points)
        return [round(float(x), 6) for x in arr[::step]]

    if intensity.ndim == 1:
        return {"monitor": d.name, "kind": "1d",
                "x": ds(d.xaxis), "intensity": ds(intensity),
                "error": ds(np.asarray(d.Error))}
    return {"monitor": d.name, "kind": "2d_profiles",
            "shape": list(intensity.shape),
            "x_profile": ds(intensity.sum(axis=0)),
            "y_profile": ds(intensity.sum(axis=1)),
            "note": "full 2D array withheld; use format='png' to see the image"}
