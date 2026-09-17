"""Prototype: target-matching guide family (2026-09-17)."""
import itertools, json, math, os, random, statistics as st
from concurrent.futures import ProcessPoolExecutor
from neutrongym import generate, reward
from neutrongym.executor import FamilyExecutor

INSTR = r"""DEFINE INSTRUMENT fam_guide_match(double src_wh=0.10, double L_in=1.5,
  double L_guide=10, double wl=5.0, double dwl=0.5, double d_sample=0.5,
  double w_in=0.02, double w_out=0.02, double m_coat=2.0)
TRACE
COMPONENT src = Source_simple(xwidth=src_wh, yheight=src_wh, dist=L_in,
  focus_xw=w_in, focus_yh=w_in, lambda0=wl, dlambda=dwl)
AT (0, 0, 0) ABSOLUTE
COMPONENT guide = Guide(w1=w_in, h1=w_in, w2=w_out, h2=w_out, l=L_guide, m=m_coat)
AT (0, 0, L_in) RELATIVE src
COMPONENT divmon = Divergence_monitor(nh=40, nv=40, filename="divw.dat", xwidth=0.3, yheight=0.3,
  maxdiv_h=4, maxdiv_v=4, restore_neutron=1)
AT (0, 0, L_guide + d_sample) RELATIVE guide
COMPONENT psd = PSD_monitor(nx=100, ny=100, filename="psdw.dat", xwidth=0.2, yheight=0.2, restore_neutron=1)
AT (0, 0, L_guide + d_sample + 0.001) RELATIVE guide
END
"""
WD = "/netdisk/ldq/grpo/match_proto"
W_IN = [0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09]
W_OUT = [0.01, 0.015, 0.02, 0.025, 0.03, 0.035, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09]
M = [1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0]
_FX = {}

def ctxs(n):
    rng = random.Random(11); out = []
    for i in range(n):
        c = generate.instance("guide_divergence", "heldout", i)["context"]
        c = {k: c[k] for k in ("src_wh", "L_in", "L_guide", "wl", "dwl")}
        c["d_sample"] = round(rng.uniform(0.1, 1.5), 3)
        hidden = dict(w_in=round(rng.uniform(0.01, 0.09), 4), w_out=round(rng.uniform(0.01, 0.09), 4),
                      m_coat=round(rng.uniform(1.0, 3.0), 3))
        out.append((c, hidden))
    return out

def run(job):
    ci, c, a, seed = job
    if "fx" not in _FX:
        _FX["fx"] = FamilyExecutor(os.path.join(WD, "fam_guide_match.instr"), workdir=os.path.join(WD, "rollouts"))
    o = _FX["fx"].run({**c, **a}, ncount=1e5, seed=seed)
    if not o.get("ok"): return ci, a, None
    p = reward._monitor(o["summary"], "psd"); d = reward._monitor(o["summary"], "divmon")
    if not p or not d or (p.get("events") or 0) < 500: return ci, a, None
    return ci, a, (p["beam_width"]["dX"], d["beam_width"]["dX"], p["intensity"])

def ok(meas, tgt, tol):
    return meas is not None and abs(meas[0] - tgt[0]) <= tol * tgt[0] and abs(meas[1] - tgt[1]) <= tol * tgt[1]

if __name__ == "__main__":
    os.makedirs(WD, exist_ok=True); open(os.path.join(WD, "fam_guide_match.instr"), "w").write(INSTR)
    assert FamilyExecutor(os.path.join(WD, "fam_guide_match.instr"), workdir=os.path.join(WD, "rollouts")).compile()["ok"]
    C = ctxs(16)
    grid = [dict(w_in=a, w_out=b, m_coat=m) for a, b, m in itertools.product(W_IN, W_OUT, M)]
    with ProcessPoolExecutor(max_workers=48) as ex:
        tg = {ci: meas for ci, a, meas in ex.map(run, [(i, c, h, 777 + i) for i, (c, h) in enumerate(C)])}
        res = {i: {} for i in range(16)}
        for ci, a, meas in ex.map(run, [(i, c, a, 777 + i) for i, (c, _) in enumerate(C) for a in grid], chunksize=64):
            res[ci][json.dumps(a, sort_keys=True)] = meas
    keys = list(res[0])
    for tol in (0.05, 0.10):
        print(f"=== tolerance {int(tol*100)}%")
        vol = []
        for i, (c, h) in enumerate(C):
            n = sum(ok(res[i][k], tg[i], tol) for k in keys); vol.append(n)
            print(f"  inst {i:2}: d {c['d_sample']:.2f} wl {c['wl']:.1f} target spot {tg[i][0]:.3f} cm div {tg[i][1]:.3f} deg | "
                  f"hidden {h} | grid designs passing {n}/{len(keys)}")
        print(f"  passing share of grid per instance: median {st.median(vol)/len(keys):.3%}, max {max(vol)/len(keys):.3%}")
        bestc = max(keys, key=lambda k: sum(ok(res[i][k], tg[i], tol) for i in range(16)))
        print(f"  best constant {bestc}: passes {sum(ok(res[i][bestc], tg[i], tol) for i in range(16))}/16")
        # nearest-neighbour lookup: use the grid design that best matches the instance with the closest targets
        def err(meas, t): return math.inf if meas is None else abs(meas[0]-t[0])/t[0] + abs(meas[1]-t[1])/t[1]
        nn = 0
        for i in range(16):
            j = min((j for j in range(16) if j != i), key=lambda j: err(tg[j], tg[i]))
            kj = min(keys, key=lambda k: err(res[j][k], tg[j]))
            nn += ok(res[i][kj], tg[i], tol)
        print(f"  nearest-other-instance lookup: {nn}/16")
    json.dump({"contexts": C, "targets": tg, "results": {str(i): res[i] for i in res}}, open(os.path.join(WD, "proto.json"), "w"))
