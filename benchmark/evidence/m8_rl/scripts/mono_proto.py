"""Prototype landscape for a Bragg-monochromator family (2026-09-17)."""
import itertools, json, math, os, random, statistics as st
from concurrent.futures import ProcessPoolExecutor
from neutrongym import reward
from neutrongym.executor import FamilyExecutor

INSTR = r"""DEFINE INSTRUMENT fam_mono(double lam=4.0, double DM=3.355, double mosaic=40,
  double win=0.02, double L1=2.0, double L2=1.5, double src_w=0.04,
  double theta=36, double alpha1=40, double alpha2=40)
DECLARE
%{
double lmin;
double lmax;
%}
INITIALIZE
%{
lmin = lam*(1-win);
lmax = lam*(1+win);
%}
TRACE
COMPONENT origin = Arm() AT (0,0,0) ABSOLUTE
COMPONENT src = Source_simple(xwidth=src_w, yheight=src_w, dist=L1, focus_xw=0.08, focus_yh=0.08,
  lambda0=lam, dlambda=0.5*lam) AT (0,0,0) RELATIVE origin
COMPONENT col1 = Collimator_linear(xwidth=0.08, yheight=0.08, length=0.3, divergence=alpha1, divergenceV=0)
  AT (0,0,L1-0.5) RELATIVE origin
COMPONENT mono_arm = Arm() AT (0,0,L1) RELATIVE origin ROTATED (0,theta,0) RELATIVE origin
COMPONENT mono = Monochromator_flat(zwidth=0.1, yheight=0.1, mosaich=mosaic, mosaicv=mosaic, r0=0.8, DM=DM)
  AT (0,0,0) RELATIVE mono_arm
COMPONENT out_arm = Arm() AT (0,0,L1) RELATIVE origin ROTATED (0,2*theta,0) RELATIVE origin
COMPONENT col2 = Collimator_linear(xwidth=0.08, yheight=0.08, length=0.3, divergence=alpha2, divergenceV=0)
  AT (0,0,0.4) RELATIVE out_arm
COMPONENT lwin = L_monitor(nL=10, filename="lwin.dat", xwidth=0.03, yheight=0.05, Lmin=lmin, Lmax=lmax,
  restore_neutron=1) AT (0,0,L2) RELATIVE out_arm
COMPONENT lall = L_monitor(nL=60, filename="lall.dat", xwidth=0.03, yheight=0.05, Lmin=0.2, Lmax=15,
  restore_neutron=1) AT (0,0,L2+0.001) RELATIVE out_arm
END
"""
WD = "/netdisk/ldq/grpo/mono_proto"
CRYSTALS = [("PG002", 3.355), ("PG004", 1.6775), ("Si111", 3.1356), ("Ge311", 1.7057), ("Cu220", 1.278)]
_FX = {}

def contexts(n, seed=5):
    rng = random.Random(seed); out = []
    while len(out) < n:
        name, dm = rng.choice(CRYSTALS); lam = rng.uniform(1.5, 6.0)
        if lam / (2 * dm) >= 0.97: continue
        th = math.degrees(math.asin(lam / (2 * dm)))
        if not 12 <= th <= 70: continue
        out.append(dict(lam=round(lam, 4), DM=dm, mosaic=round(rng.uniform(20, 60), 1),
                        win=round(rng.uniform(0.005, 0.03), 4), L1=round(rng.uniform(1.5, 3.0), 3),
                        L2=round(rng.uniform(1.0, 2.0), 3), crystal=name, bragg=th))
    return out

def run(job):
    ci, ctx, a = job
    if "fx" not in _FX:
        _FX["fx"] = FamilyExecutor(os.path.join(WD, "fam_mono.instr"), workdir=os.path.join(WD, "rollouts"))
    p = {k: v for k, v in ctx.items() if k not in ("crystal", "bragg")}; p.update(a)
    out = _FX["fx"].run(p, ncount=1e6, seed=1234 + ci)
    if not out.get("ok"): return ci, a, None, None, None
    s = reward._monitor(out["summary"], "lwin"); t = reward._monitor(out["summary"], "lall")
    return ci, a, s["intensity"], t["intensity"], s.get("events")

if __name__ == "__main__":
    os.makedirs(WD, exist_ok=True); open(os.path.join(WD, "fam_mono.instr"), "w").write(INSTR)
    c = FamilyExecutor(os.path.join(WD, "fam_mono.instr"), workdir=os.path.join(WD, "rollouts")).compile()
    assert c["ok"], c
    ctxs = contexts(12)
    OFF = [-1.0, -0.5, -0.25, 0.0, 0.25, 0.5, 1.0]; AL = [10, 20, 30, 45, 60, 90, 120]
    jobs = [(i, c_, dict(theta=round(c_["bragg"] + o, 4), alpha1=a1, alpha2=a2))
            for i, c_ in enumerate(ctxs) for o in OFF for a1 in AL for a2 in AL]
    res = {}
    with ProcessPoolExecutor(max_workers=48) as ex:
        for ci, a, S, T, ev in ex.map(run, jobs, chunksize=16):
            res.setdefault(ci, []).append((a, S or 0.0, T or 0.0, ev or 0))
    summary = {}
    for name, fom in (("S", lambda S, T: S), ("S2/T", lambda S, T: S * S / T if T > 0 else 0.0)):
        print(f"=== FOM = {name}")
        opt = {}
        for ci, rows in sorted(res.items()):
            best = max(rows, key=lambda r: fom(r[1], r[2])); fb = fom(best[1], best[2])
            opt[ci] = fb
            b = best[0]; ctx = ctxs[ci]
            print(f"  inst {ci:2} {ctx['crystal']} lam {ctx['lam']:.2f} mos {ctx['mosaic']:4.1f} win {ctx['win']:.3f} | "
                  f"opt dtheta {b['theta']-ctx['bragg']:+.2f} a1 {b['alpha1']} a2 {b['alpha2']} | events {best[3]}")
        # constant (dtheta, a1, a2) policies where theta = bragg + dtheta (Bragg rule + fixed collimation)
        keys = {(round(r[0]['theta'] - ctxs[ci]['bragg'], 2), r[0]['alpha1'], r[0]['alpha2']) for ci, rows in res.items() for r in rows}
        def share(k, bar=0.85):
            n = 0
            for ci, rows in res.items():
                for a, S, T, ev in rows:
                    if (round(a['theta'] - ctxs[ci]['bragg'], 2), a['alpha1'], a['alpha2']) == k:
                        n += opt[ci] > 0 and fom(S, T) > bar * opt[ci]
            return n
        best_k = max(keys, key=share)
        print(f"  best 'Bragg angle + fixed collimation' rule {best_k}: passes 0.85 on {share(best_k)}/12, 0.95 on {share(best_k, 0.95)}/12")
        fullopen = (0.0, 120, 120)
        print(f"  'Bragg angle + fully open' rule: 0.85 on {share(fullopen)}/12")
    json.dump({"contexts": ctxs, "results": {str(k): v for k, v in res.items()}}, open(os.path.join(WD, "proto.json"), "w"))
