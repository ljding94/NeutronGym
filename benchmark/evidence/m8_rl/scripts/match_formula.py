"""Geometric-optics formula rule on the target-matching prototype."""
import json, math, os, sys
from concurrent.futures import ProcessPoolExecutor
sys.path.insert(0, "/netdisk/ldq/tmp")
import match_proto as mp

def clip(v, lo, hi): return max(lo, min(hi, v))

def formula(c, t, w_in, kdiv=1/math.sqrt(3), kspot=1/math.sqrt(12)):
    spot_m = t[0] / 100.0                      # cm -> m (std)
    div_deg = t[1]                              # std, deg
    m = clip(div_deg / (kdiv * 0.099 * c["wl"]), 1.0, 3.0)
    spread = c["d_sample"] * math.tan(math.radians(div_deg))
    core = spot_m ** 2 - spread ** 2
    w_out = clip(math.sqrt(core) / kspot if core > 0 else 0.01, 0.01, 0.09)
    return dict(w_in=w_in, w_out=round(w_out, 4), m_coat=round(m, 3))

if __name__ == "__main__":
    data = json.load(open(os.path.join(mp.WD, "proto.json")))
    C = data["contexts"]; T = {int(k): v for k, v in data["targets"].items()}
    variants = []
    for w_in in (0.03, 0.05, 0.07, 0.09):
        for kd in (1 / math.sqrt(3), 0.5, 0.4):            # try a few calibrations of the formula
            variants.append((w_in, kd))
    jobs = [(i, C[i][0], formula(C[i][0], T[i], w_in, kd), 777 + i) for (w_in, kd) in variants for i in range(16)]
    with ProcessPoolExecutor(max_workers=48) as ex:
        meas = list(ex.map(mp.run, jobs))
    k = 0
    for (w_in, kd) in variants:
        rows = meas[k:k + 16]; k += 16
        p5 = sum(mp.ok(m, T[i], 0.05) for i, _, m in rows)
        p10 = sum(mp.ok(m, T[i], 0.10) for i, _, m in rows)
        errs = [(abs(m[0]-T[i][0])/T[i][0], abs(m[1]-T[i][1])/T[i][1]) if m else (9, 9) for i, _, m in rows]
        med_s = sorted(e[0] for e in errs)[8]; med_d = sorted(e[1] for e in errs)[8]
        print(f"formula rule w_in={w_in} kdiv={kd:.3f}: pass 5% {p5}/16, 10% {p10}/16 | median rel err spot {med_s:.2f} div {med_d:.2f}")
