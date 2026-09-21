"""Prototype: curved-guide (bender) archetype (2026-09-20).
A bender transmits only wavelengths above a cutoff set by curvature, channel
width and coating: lambda_c ~ sqrt(2w/r) / (0.0017 * m). Targets: the mean
transmitted wavelength and its spread at the exit."""
import itertools, json, math, os, random, statistics as st
from concurrent.futures import ProcessPoolExecutor
from neutrongym import reward
from neutrongym.executor import FamilyExecutor

INSTR = r"""DEFINE INSTRUMENT fam_bender(double L_bend=20.0, double L_out=1.0,
  double src_wh=0.10, double lam0=5.0, double dlam=4.5,
  double r_curve=200, double w_ch=0.03, double m_coat=2.5)
TRACE
COMPONENT arm = Arm() AT (0,0,0) ABSOLUTE
COMPONENT src = Source_simple(xwidth=src_wh, yheight=src_wh, dist=2.0,
  focus_xw=w_ch, focus_yh=0.05, lambda0=lam0, dlambda=dlam, flux=1e12) AT (0,0,0) RELATIVE arm
COMPONENT bend = Bender(w=w_ch, h=0.05, r=r_curve, l=L_bend, k=1, d=0.0005,
  ma=m_coat, mi=m_coat, ms=m_coat) AT (0,0,2.0) RELATIVE arm
COMPONENT lmon = L_monitor(nL=200, filename="lam.dat", xwidth=0.12, yheight=0.08,
  Lmin=0.2, Lmax=14, restore_neutron=1) AT (0,0,L_bend+L_out) RELATIVE bend
END
"""
WD = "/netdisk/ldq/grpo/bender_proto"
R = [40, 70, 110, 160, 220, 300, 400, 550]
W = [0.02, 0.03, 0.045, 0.06, 0.08]
M = [1.5, 2.0, 2.5, 3.0, 4.0]
_FX = {}

def obs(job):
    i, ctx, a, seed = job
    if "fx" not in _FX:
        _FX["fx"] = FamilyExecutor(os.path.join(WD, "fam_bender.instr"), workdir=os.path.join(WD, "rollouts"))
    out = _FX["fx"].run({**ctx, **a}, ncount=2e6, seed=seed or 71 + i)
    if not out.get("ok"): return i, a, None
    m = reward._monitor(out["summary"], "lmon")
    if not m or (m.get("events") or 0) < 500: return i, a, None
    return i, a, (m["center_of_mass"], m["beam_width"]["dX"], m.get("events"))

def contexts(n, seed=23):
    rng = random.Random(seed)
    # the incident spectrum varies per instance, so a fixed bender transmits a
    # different band on each: without this the design -> observable mapping is
    # context-free and another instance's solution transfers (lookup 7/10)
    return [dict(L_bend=round(rng.uniform(12.0, 30.0), 3), L_out=1.0, src_wh=0.10,
                 lam0=round(rng.uniform(3.0, 7.5), 3), dlam=round(rng.uniform(2.0, 4.0), 3))
            for _ in range(n)]

if __name__ == "__main__":
    os.makedirs(WD, exist_ok=True); open(os.path.join(WD, "fam_bender.instr"), "w").write(INSTR)
    c = FamilyExecutor(os.path.join(WD, "fam_bender.instr"), workdir=os.path.join(WD, "rollouts")).compile()
    assert c["ok"], c["diagnostics"][-4:]
    C = contexts(10); idx = list(range(10))
    grid = [dict(r_curve=r, w_ch=w, m_coat=m) for r, w, m in itertools.product(R, W, M)]
    rng = random.Random(8)
    hidden = {i: dict(r_curve=round(rng.uniform(50, 500), 1), w_ch=round(rng.uniform(0.02, 0.08), 4),
                      m_coat=round(rng.uniform(1.5, 4.0), 2)) for i in idx}
    jobs = [(i, C[i], a, None) for i in idx for a in grid]
    jobs += [(i, C[i], hidden[i], None) for i in idx] + [(i, C[i], hidden[i], 555) for i in idx]
    res, tgt, noise = {i: {} for i in idx}, {}, {}
    with ProcessPoolExecutor(max_workers=28) as ex:
        for i, a, o in ex.map(obs, jobs, chunksize=8):
            if a == hidden[i]:
                (tgt.setdefault(i, o) if i not in tgt else noise.setdefault(i, []).append(o))
            else:
                res[i][json.dumps(a, sort_keys=True)] = o
    live = [i for i in idx if tgt.get(i)]
    d = [(abs(o[0]-tgt[i][0])/tgt[i][0], abs(o[1]-tgt[i][1])/tgt[i][1]) for i in live for o in noise.get(i, []) if o]
    print("usable hidden designs:", len(live), "of", len(idx))
    if d: print(f"seed noise: mean-lambda {st.median(x[0] for x in d):.4f}, spread {st.median(x[1] for x in d):.4f}")
    for tw, ts in ((0.005, 0.02), (0.0025, 0.01)):
        ok = lambda o, t: o is not None and abs(o[0]-t[0]) <= tw*t[0] and abs(o[1]-t[1]) <= ts*t[1]
        vols = [sum(ok(o, tgt[i]) for o in res[i].values()) for i in live]
        keys = set().union(*[set(res[i]) for i in live])
        best = max(keys, key=lambda k: sum(ok(res[i].get(k), tgt[i]) for i in live))
        nn = 0
        for i in live:
            j = min((j for j in live if j != i), key=lambda j: sum(abs(tgt[j][k]-tgt[i][k])/tgt[i][k] for k in (0,1)))
            cand = [k for k in res[j] if res[j][k]]
            kj = min(cand, key=lambda k: sum(abs(res[j][k][x]-tgt[j][x])/tgt[j][x] for x in (0,1)))
            nn += ok(res[i].get(kj), tgt[i])
        print(f"tol lam {tw:.0%}/spread {ts:.0%}: solutions/instance median {st.median(vols):.1f}/{len(grid)} "
              f"(zero {sum(v==0 for v in vols)}/{len(live)}) | best constant {sum(ok(res[i].get(best), tgt[i]) for i in live)}/{len(live)} | lookup {nn}/{len(live)}")
