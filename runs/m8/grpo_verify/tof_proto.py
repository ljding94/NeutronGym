"""Prototype: TOF chopper-pair family (2026-09-20).
A third archetype in the TIME domain: chopper phase selects the wavelength
(lambda = 3956 * dt / L_ch), opening angle and frequency set the spread.
Targets: mean wavelength and wavelength spread at the sample."""
import itertools, json, math, os, random, statistics as st
from concurrent.futures import ProcessPoolExecutor
from neutrongym import reward
from neutrongym.executor import FamilyExecutor

INSTR = r"""DEFINE INSTRUMENT fam_tof_chopper(double L_ch=8.0, double L_sample=2.0,
  double lam0=5.0, double dlam=4.0, double src_r=0.02,
  double nu=100, double phase=30, double theta2=6)
TRACE
COMPONENT arm = Arm() AT (0,0,0) ABSOLUTE
COMPONENT src = Source_simple(radius=src_r, dist=1.0, focus_xw=0.03, focus_yh=0.03,
  lambda0=lam0, dlambda=dlam, flux=1e12) AT (0,0,0) RELATIVE arm
COMPONENT ch1 = DiskChopper(theta_0=5, radius=0.35, yheight=0.05, nu=nu, nslit=1, isfirst=1)
  AT (0,0,1.0) RELATIVE arm
COMPONENT ch2 = DiskChopper(theta_0=theta2, radius=0.35, yheight=0.05, nu=nu, nslit=1, phase=phase)
  AT (0,0,1.0+L_ch) RELATIVE arm
COMPONENT lmon = L_monitor(nL=200, filename="lam.dat", xwidth=0.04, yheight=0.06,
  Lmin=0.2, Lmax=20, restore_neutron=1) AT (0,0,1.0+L_ch+L_sample) RELATIVE arm
END
"""
WD = "/netdisk/ldq/grpo/tof_proto"
NU = [20, 30, 40, 50, 60]
PH = [round(20 + 10 * k, 1) for k in range(33)]
TH = [2, 5, 9, 14, 20]
_FX = {}

def obs(job):
    i, ctx, a, seed = job
    if "fx" not in _FX:
        _FX["fx"] = FamilyExecutor(os.path.join(WD, "fam_tof_chopper.instr"), workdir=os.path.join(WD, "rollouts"))
    out = _FX["fx"].run({**ctx, **a}, ncount=5e6, seed=seed or 31 + i)
    if not out.get("ok"): return i, a, None
    m = reward._monitor(out["summary"], "lmon")
    if not m or (m.get("events") or 0) < 500: return i, a, None
    return i, a, (m["center_of_mass"], m["beam_width"]["dX"], m.get("events"))

def contexts(n, seed=17):
    rng = random.Random(seed)
    return [dict(L_ch=round(rng.uniform(5.0, 12.0), 3), L_sample=round(rng.uniform(1.0, 3.0), 3),
                 lam0=5.0, dlam=4.0, src_r=0.02) for _ in range(n)]

if __name__ == "__main__":
    os.makedirs(WD, exist_ok=True); open(os.path.join(WD, "fam_tof_chopper.instr"), "w").write(INSTR)
    c = FamilyExecutor(os.path.join(WD, "fam_tof_chopper.instr"), workdir=os.path.join(WD, "rollouts")).compile()
    assert c["ok"], c["diagnostics"][-4:]
    C = contexts(10); idx = list(range(10))
    grid = [dict(nu=a, phase=b, theta2=t) for a, b, t in itertools.product(NU, PH, TH)]
    rng = random.Random(5)
    # draw the hidden design from the feasible region: pick a target wavelength
    # in the source band, then solve phase = 360 * nu * lambda * L_ch / 3956
    hidden = {}
    for i in idx:
        for _ in range(50):
            nu = round(rng.uniform(20, 60), 1); lam = rng.uniform(2.5, 7.5)
            ph = 360 * nu * lam * C[i]["L_ch"] / 3956.0
            if 20 <= ph <= 340:
                hidden[i] = dict(nu=nu, phase=round(ph, 1), theta2=round(rng.uniform(2, 20), 1)); break
        else:
            hidden[i] = dict(nu=30.0, phase=180.0, theta2=8.0)
    jobs = [(i, C[i], a, None) for i in idx for a in grid]
    jobs += [(i, C[i], hidden[i], None) for i in idx] + [(i, C[i], hidden[i], 4242) for i in idx]
    res, tgt, noise = {i: {} for i in idx}, {}, {}
    with ProcessPoolExecutor(max_workers=48) as ex:
        for i, a, o in ex.map(obs, jobs, chunksize=16):
            if a == hidden[i]:
                (tgt.setdefault(i, o) if i not in tgt else noise.setdefault(i, []).append(o))
            else:
                res[i][json.dumps(a, sort_keys=True)] = o
    live = [i for i in idx if tgt.get(i)]
    print("usable hidden designs:", len(live), "of", len(idx))
    d = [(abs(o[0]-tgt[i][0])/tgt[i][0], abs(o[1]-tgt[i][1])/tgt[i][1]) for i in live for o in noise.get(i, []) if o]
    if d: print(f"seed noise: mean-lambda {st.median(x[0] for x in d):.4f}, spread {st.median(x[1] for x in d):.4f}")
    valid = [sum(1 for o in res[i].values() if o) for i in live]
    print(f"designs giving any counts: median {st.median(valid):.0f}/{len(grid)} (phase must be right)")
    for tol in (0.05, 0.03):
        ok = lambda o, t: o is not None and all(abs(o[k]-t[k]) <= tol*t[k] for k in (0, 1))
        vols = [sum(ok(o, tgt[i]) for o in res[i].values()) for i in live]
        keys = set().union(*[set(res[i]) for i in live])
        best = max(keys, key=lambda k: sum(ok(res[i].get(k), tgt[i]) for i in live))
        nn = 0
        for i in live:
            j = min((j for j in live if j != i), key=lambda j: sum(abs(tgt[j][k]-tgt[i][k])/tgt[i][k] for k in (0, 1)))
            cand = [k for k in res[j] if res[j][k]]
            kj = min(cand, key=lambda k: sum(abs(res[j][k][x]-tgt[j][x])/tgt[j][x] for x in (0, 1)))
            nn += ok(res[i].get(kj), tgt[i])
        print(f"tol {tol:.0%}: solutions/instance median {st.median(vols):.1f}/{len(grid)} "
              f"(zero-solution {sum(v==0 for v in vols)}/{len(live)}) | best constant "
              f"{sum(ok(res[i].get(best), tgt[i]) for i in live)}/{len(live)} | lookup {nn}/{len(live)}")
    json.dump({"contexts": C, "targets": {str(k): v for k, v in tgt.items()}}, open(os.path.join(WD, "proto.json"), "w"))
