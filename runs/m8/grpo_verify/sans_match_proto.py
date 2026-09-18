"""Prototype: SANS target-matching (pattern width + scattered intensity)."""
import itertools, json, math, os, random, statistics as st
from concurrent.futures import ProcessPoolExecutor
from neutrongym import generate, reward
from neutrongym.executor import FamilyExecutor

FAM = "sans_collimation"
WD = "/netdisk/ldq/mcstas-mcp-home/families"
R = [0.001, 0.002, 0.003, 0.004, 0.005, 0.006, 0.008, 0.010, 0.012, 0.014, 0.016, 0.018, 0.020]
_FX = {}

def obs(job):
    i, a, seed = job
    if "fx" not in _FX:
        _FX["fx"] = FamilyExecutor(generate.family_instr(FAM, WD), workdir=os.path.join(WD, FAM, "rollouts"))
    inst = generate.instance(FAM, "heldout", i)
    if generate.sans_direct_beam_leaks(inst["context"], a) or \
       generate.sans_sample_beam_radius(inst["context"], a) > inst["context"]["sample_wh"] / 2:
        return i, a, None
    out = _FX["fx"].run({**inst["context"], **a}, ncount=inst["protocol"]["ncount"],
                        seed=seed or inst["protocol"]["seed"])
    if not out.get("ok"): return i, a, None
    m = reward._monitor(out["summary"], "detector")
    if not m or (m.get("events") or 0) < 500: return i, a, None
    return i, a, (m["beam_width"]["dX"], m["intensity"], m.get("events"))

if __name__ == "__main__":
    FamilyExecutor(generate.family_instr(FAM, WD), workdir=os.path.join(WD, FAM, "rollouts")).compile()
    idx = list(range(12))
    grid = [dict(r_pin1=a, r_pin2=b) for a, b in itertools.product(R, R)]
    rng = random.Random(3)
    hidden = {i: dict(r_pin1=round(rng.uniform(0.002, 0.018), 5), r_pin2=round(rng.uniform(0.002, 0.008), 5)) for i in idx}
    jobs = [(i, a, None) for i in idx for a in grid]
    jobs += [(i, hidden[i], None) for i in idx]
    jobs += [(i, hidden[i], 4242) for i in idx] + [(i, hidden[i], 777) for i in idx]   # noise probe
    res, tgt, noise = {i: {} for i in idx}, {}, {}
    with ProcessPoolExecutor(max_workers=48) as ex:
        for i, a, o in ex.map(obs, jobs, chunksize=8):
            k = json.dumps(a, sort_keys=True)
            if a == hidden[i]:
                (tgt.setdefault(i, o) if i not in tgt else noise.setdefault(i, []).append(o))
            else:
                res[i][k] = o
    print("noise between seeds (width, intensity) rel diff:")
    diffs = []
    for i in idx:
        if tgt.get(i) and noise.get(i):
            for o in noise[i]:
                if o: diffs.append((abs(o[0]-tgt[i][0])/tgt[i][0], abs(o[1]-tgt[i][1])/tgt[i][1]))
    print("  median width", round(st.median(d[0] for d in diffs), 4), "median intensity", round(st.median(d[1] for d in diffs), 4),
          "| max", round(max(d[0] for d in diffs), 4), round(max(d[1] for d in diffs), 4))
    live = [i for i in idx if tgt.get(i)]
    print("instances with a usable hidden design:", len(live), "of", len(idx))
    for tw, ti in ((0.05, 0.10), (0.05, 0.15), (0.08, 0.20)):
        ok = lambda o, t: o is not None and t is not None and abs(o[0]-t[0]) <= tw*t[0] and abs(o[1]-t[1]) <= ti*t[1]
        tol = (tw, ti)
        vols = []
        for i in live:
            n = sum(ok(o, tgt[i]) for o in res[i].values() if o)
            vols.append(n)
        keys = set().union(*[set(res[i]) for i in live])
        best = max(keys, key=lambda k: sum(ok(res[i].get(k), tgt[i]) for i in live))
        nn = 0
        for i in live:
            j = min((j for j in live if j != i), key=lambda j: abs(tgt[j][0]-tgt[i][0])/tgt[i][0] + abs(tgt[j][1]-tgt[i][1])/tgt[i][1])
            kj = min((k for k in res[j] if res[j][k]), key=lambda k: abs(res[j][k][0]-tgt[j][0])/tgt[j][0] + abs(res[j][k][1]-tgt[j][1])/tgt[j][1])
            nn += ok(res[i].get(kj), tgt[i])
        print(f"tol width {tw:.0%} / intensity {ti:.0%}: grid solutions per instance median {st.median(vols):.1f}/{len(grid)} "
              f"(zero-solution {sum(v==0 for v in vols)}/{len(live)}) | best constant "
              f"{sum(ok(res[i].get(best), tgt[i]) for i in live)}/{len(live)} | nearest-instance lookup {nn}/{len(live)}")
