"""Compute calibration targets for a range of instances, in parallel.

Collection and evaluation call env.reset(), which calibrates an instance on
first use. Since calibration v3 that is ~30 s per SANS instance, so a
600-instance RAFT collection would spend ~5 h calibrating one instance at a
time. Calibration is independent per instance, so do it up front.

Usage:
  python benchmark/harness/precalibrate.py --family sans_collimation \
      --split train --start 0 --n 600 --workers 7
"""

import argparse
import json
import os
import time
from concurrent.futures import ProcessPoolExecutor

from neutrongym import calibrate
from neutrongym.env import NeutronGym

_ENVS = {}


def jobs(family, split, start, n):
    return [(family, split, i) for i in range(start, start + n)]


def _one(job):
    family, split, i = job
    key = (family, split)
    if key not in _ENVS:
        _ENVS[key] = NeutronGym(family=family, split=split)
    env = _ENVS[key]
    obs, _ = env.reset(index=i)
    inst = obs["instance"]
    rec = json.load(open(calibrate.cache_path(os.path.join(env.workdir, family), inst)))
    return i, rec.get("ok"), rec.get("evals"), bool(inst.get("no_headroom"))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--family", required=True)
    ap.add_argument("--split", required=True, choices=("train", "heldout"))
    ap.add_argument("--start", type=int, default=0)
    ap.add_argument("--n", type=int, required=True)
    ap.add_argument("--workers", type=int, default=7)
    a = ap.parse_args()
    NeutronGym(family=a.family, split=a.split)      # compile before forking
    t0, failed, no_headroom = time.time(), [], []
    with ProcessPoolExecutor(max_workers=a.workers) as ex:
        for k, (i, ok, evals, nh) in enumerate(ex.map(_one, jobs(a.family, a.split, a.start, a.n)), 1):
            failed += [] if ok else [i]
            no_headroom += [i] if nh else []
            if k % 25 == 0 or k == a.n:
                print(f"  {k}/{a.n} calibrated ({time.time() - t0:.0f}s)", flush=True)
    print(json.dumps({"family": a.family, "split": a.split, "start": a.start, "n": a.n,
                      "failed": failed, "no_headroom": no_headroom,
                      "wall_s": round(time.time() - t0)}))
    if failed:
        raise SystemExit(f"calibration failed on {len(failed)} instances: {failed[:10]}")


if __name__ == "__main__":
    main()
