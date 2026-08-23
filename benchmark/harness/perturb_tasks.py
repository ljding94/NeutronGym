"""Perturbed variants of seen-tier T1 tasks (contamination control, M5.5).

A variant changes a HARD-CODED component parameter in the reference (never
a run-time DEFINE parameter — a memorizer would receive those in the
prompt and run its memorized file at the right values anyway). The spec
sheet the agent sees carries the perturbed value; grading runs against the
perturbed reference. So:

  spec-follower  -> builds the perturbed physics -> PASSES
  memorizer      -> emits the canonical file     -> FAILS

Every accepted variant must prove BOTH sides (the same discipline as T2
validation): the perturbed reference passes its own task at a fresh seed,
AND the canonical reference FAILS the variant's grading. Trials are
screened at reduced ncount, the accepted perturbation is validated at the
full protocol. Perturbed references are committed to
benchmark/instruments/perturbed/ (diffable provenance).

Usage:
  conda run -n mcstas python benchmark/harness/perturb_tasks.py [T1_id ...]
"""

import copy
import glob
import json
import os
import sys
import tempfile

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(REPO, "benchmark", "harness"))
os.environ.setdefault("MCSTAS_MCP_HOME",
                      tempfile.mkdtemp(prefix="perturb_"))

import author_tasks  # noqa: E402
import grader  # noqa: E402

from mcstas_mcp import registry  # noqa: E402

OUT_INSTR = os.path.join(REPO, "benchmark", "instruments", "perturbed")
TASKS = os.path.join(REPO, "benchmark", "tasks")
SCALE = 1.15
SCREEN_NCOUNT = 1e5
FRESH_SEED = 4321


def _as_number(v):
    """Reader-loaded specs store values as strings — accept pure numerics
    ('0.09', '2.566e0'), reject expressions/param references/files."""
    if isinstance(v, bool):
        return None
    if isinstance(v, (int, float)):
        return float(v)
    if isinstance(v, str):
        try:
            return float(v.strip())
        except ValueError:
            return None
    return None


# parameters that move graded observables hardest, first: d-spacings and
# wavelength selections shift spectra (CoM rtol is 5%); frequencies/phases
# shift TOF; geometry knobs come last (guides re-collimate 15% away)
PRIORITY = ("dm", "lambda", "lam", "wavelength", "lmin", "lmax", "phase",
            "frequency", "nu", "theta", "angle", "curv", "dist", "radius",
            "length")


def _priority(name):
    low = name.lower()
    for rank, kw in enumerate(PRIORITY):
        if kw in low:
            return rank
    return len(PRIORITY)


def candidate_targets(spec):
    """(component_index, param, value) candidates: numeric hard-coded
    parameters on non-monitor components, physics-sensitive names first,
    then upstream-first."""
    out = []
    for i, c in enumerate(spec["components"]):
        if "monitor" in c["component"].lower():
            continue
        for k, v in (c.get("parameters") or {}).items():
            num = _as_number(v)
            if num is not None and num != 0 and "file" not in k.lower():
                out.append((i, k, num))
    return sorted(out, key=lambda t: (_priority(t[1]), t[0]))


def perturbed_spec(spec, target):
    i, k, v = target
    s = copy.deepcopy(spec)
    s["components"][i]["parameters"][k] = round(v * SCALE, 9)
    return s


def try_variant(task, spec, target, canonical_path):
    """Screen at low ncount: perturbed ref must self-pass AND the canonical
    reference must fail. Returns (variant_task, pert_path) or None."""
    i, k, _ = target
    name = f"{task['id'].lower()}_pert"  # one clean name; trials overwrite
    pert = perturbed_spec(spec, target)
    pert["name"] = name
    os.makedirs(OUT_INSTR, exist_ok=True)
    pert_path = os.path.join(OUT_INSTR, f"{name}.instr")
    registry.export_instr(pert, dest=pert_path)

    proto = {**task["protocol"], "ncount": SCREEN_NCOUNT}
    wd = os.path.join(REPO, "runs", "perturb", task["id"])
    ref_p = grader.run_protocol(pert_path, task["reference"].get(
        "parameters", {}), proto, wd, f"pert_{i}_{k}")
    if not ref_p.get("ok"):
        return None
    canon = grader.run_protocol(canonical_path, task["reference"].get(
        "parameters", {}), {**proto, "seed": FRESH_SEED}, wd, f"canon_{i}_{k}")
    self_chk = grader.run_protocol(pert_path, task["reference"].get(
        "parameters", {}), {**proto, "seed": FRESH_SEED}, wd, f"self_{i}_{k}")

    vt = copy.deepcopy(task)
    vt["id"] = task["id"] + "_pert"
    vt["split"] = "perturbed"
    vt["reference"] = {"instr": os.path.relpath(pert_path, REPO),
                       "parameters": task["reference"].get("parameters", {})}
    vt["perturbation"] = {"component": spec["components"][i]["name"],
                          "parameter": k, "scale": SCALE,
                          "canonical": task["reference"]["instr"]}
    vt["notes"] = (f"Perturbed variant of {task['id']} (contamination "
                   f"control): {spec['components'][i]['name']}.{k} x{SCALE}. "
                   "A memorizer reproducing the canonical file fails this "
                   "task; a spec-follower passes.")
    roles = [m["role"] for m in task["grading"]["monitors"]]
    vt["prompt"] = author_tasks.render_prompt(
        vt["id"], task.get("class", "instrument"), pert, roles,
        task["reference"].get("parameters", {}))

    if not (self_chk.get("ok") and canon.get("ok")):
        return None
    # screening runs at reduced ncount: scale the statistics floor with it
    vs = copy.deepcopy(vt)
    vs.setdefault("grading", {})["min_events"] = 100
    self_grade = grader.grade(vs, self_chk, ref_p)
    canon_grade = grader.grade(vs, canon, ref_p)
    if os.environ.get("PERTURB_DEBUG"):
        print(f"    trial {spec['components'][i]['name']}.{k}: "
              f"self={'PASS' if self_grade['pass'] else 'fail'}"
              f"({self_grade.get('checks_passed')}, "
              f"hard={self_grade['hard_failures'][:1]}) "
              f"canon={'PASS' if canon_grade['pass'] else 'fail'}"
              f"({canon_grade.get('checks_passed')})")
    if self_grade["pass"] and not canon_grade["pass"]:
        return vt, pert_path
    return None


def finalize(vt, pert_path, task):
    """Full-protocol validation pair for the accepted perturbation."""
    wd = os.path.join(REPO, "runs", "perturb", vt["id"] + "_final")
    params = task["reference"].get("parameters", {})
    ref = grader.run_protocol(pert_path, params, task["protocol"], wd, "ref")
    if not ref.get("ok"):
        return None
    self_chk = grader.run_protocol(
        pert_path, params, {**task["protocol"], "seed": FRESH_SEED},
        wd, "self")
    canon = grader.run_protocol(
        grader._resolve_instr(task["reference"]["instr"]), params,
        {**task["protocol"], "seed": FRESH_SEED}, wd, "canon")
    self_grade = grader.grade(vt, self_chk, ref)
    canon_grade = grader.grade(vt, canon, ref)
    if not (self_grade["pass"] and not canon_grade["pass"]):
        return None
    return {"self": self_grade["checks_passed"],
            "canonical_fails": [f"{c['role']}.{c['observable']}"
                                for c in canon_grade["checks"]
                                if not c["pass"]] or ["hard failure"]}


def main():
    only = set(sys.argv[1:]) or None
    results = []
    for p in sorted(glob.glob(os.path.join(TASKS, "T1", "T1_*.json"))):
        task = json.load(open(p))
        if task.get("split") != "seen" or (only and task["id"] not in only):
            continue
        canonical = grader._resolve_instr(task["reference"]["instr"])
        try:
            spec, _ = registry.load_from_instr(
                canonical, name=f"{task['id'].lower()}_canon")
        except Exception as e:  # noqa: BLE001 — one bad task must not kill the run
            print(f"{'ERROR':10} {task['id']:24} reader: {str(e)[:80]}")
            results.append({"task": task["id"], "variant": False,
                            "detail": f"reader failed: {str(e)[:120]}"})
            continue
        # reader-loaded NULL sentinels (unset pointer params) break the
        # McStasScript rebuild check — omitting them is equivalent
        for c in spec["components"]:
            c["parameters"] = {k: v for k, v in (c.get("parameters") or
                                                 {}).items() if v != "NULL"}
        found = None
        # all targets at the gentle scale first, escalate only if needed
        trials = [(t, s) for s in (1.15, 1.4)
                  for t in candidate_targets(spec)[:5]][:8]
        for target, scale in trials:
            globals()["SCALE"] = scale
            try:
                out = try_variant(task, spec, target, canonical)
            except Exception as e:  # noqa: BLE001
                if os.environ.get("PERTURB_DEBUG"):
                    print(f"    trial error: {str(e)[:100]}")
                continue
            if out:
                vt, pert_path = out
                val = finalize(vt, pert_path, task)
                if val:
                    vt["validation"] = val
                    dest = os.path.join(TASKS, "T1_perturbed",
                                        vt["id"] + ".json")
                    os.makedirs(os.path.dirname(dest), exist_ok=True)
                    with open(dest, "w") as f:
                        json.dump(vt, f, indent=1)
                    found = (target, val)
                    break
        tag = "OK" if found else "NO-VARIANT"
        detail = (f"{spec['components'][found[0][0]]['name']}."
                  f"{found[0][1]} x{SCALE} | canonical fails: "
                  f"{','.join(found[1]['canonical_fails'][:3])}"
                  if found else "no candidate passed the pair condition")
        print(f"{tag:10} {task['id']:24} {detail}")
        results.append({"task": task["id"], "variant": bool(found),
                        "detail": detail})
    n = sum(r["variant"] for r in results)
    os.makedirs(os.path.join(TASKS, "T1_perturbed"), exist_ok=True)
    with open(os.path.join(TASKS, "T1_perturbed", "_generation.json"),
              "w") as f:
        json.dump(results, f, indent=1)
    print(f"\n{n}/{len(results)} seen-tier tasks gained a validated "
          f"perturbed variant -> benchmark/tasks/T1_perturbed/")


if __name__ == "__main__":
    main()
