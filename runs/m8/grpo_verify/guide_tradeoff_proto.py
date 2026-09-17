"""Prototype: guide with NO hard specs (FOM already counts only within +/-div_max
on the det_wh sample). Is the optimum interior and instance-specific?"""
import itertools, json, os, statistics as st
from concurrent.futures import ProcessPoolExecutor
from neutrongym import generate, reward
from neutrongym.executor import FamilyExecutor

FAM = "guide_divergence"
WD = "/netdisk/ldq/mcstas-mcp-home/families"
W_IN = [0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09]
W_OUT = [0.01, 0.015, 0.02, 0.025, 0.03, 0.035, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09]
M = [1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0]
_FX = {}

def fom(job):
    i, a = job
    if "fx" not in _FX:
        _FX["fx"] = FamilyExecutor(generate.family_instr(FAM, WD), workdir=os.path.join(WD, FAM, "rollouts"))
    inst = generate.instance(FAM, "heldout", i)
    out = _FX["fx"].run({**inst["context"], **a}, ncount=inst["protocol"]["ncount"], seed=inst["protocol"]["seed"])
    if not out.get("ok"): return i, a, None
    mon = reward._monitor(out["summary"], "divmon")
    return i, a, (reward.get_observable(mon, "intensity") if mon else None)

if __name__ == "__main__":
    FamilyExecutor(generate.family_instr(FAM, WD), workdir=os.path.join(WD, FAM, "rollouts")).compile()
    idx = list(range(16))
    grid = [dict(w_in=a, w_out=b, m_coat=c) for a, b, c in itertools.product(W_IN, W_OUT, M)]
    res = {i: {} for i in idx}
    with ProcessPoolExecutor(max_workers=48) as ex:
        for i, a, f in ex.map(fom, [(i, a) for i in idx for a in grid], chunksize=64):
            res[i][json.dumps(a, sort_keys=True)] = f or 0.0
    rows = []
    for i in idx:
        c = generate.instance(FAM, "heldout", i)["context"]
        best = max(res[i], key=res[i].get); b = json.loads(best); fb = res[i][best]
        lim_m = generate.guide_max_m(c)
        # readout rule on this grid: w_out = nearest to det_wh, m = nearest <= limit, best w_in
        wo = min(W_OUT, key=lambda x: abs(x - c["det_wh"])); mm = max([m for m in M if m <= lim_m] or [1.0])
        rule = max(res[i][json.dumps(dict(w_in=w, w_out=wo, m_coat=mm), sort_keys=True)] for w in W_IN)
        rows.append((i, c, b, fb, rule / fb if fb else None, lim_m))
        print(f"inst {i:2}: wl {c['wl']:.1f} div_max {c['div_max']:.2f} det_wh {c['det_wh']:.3f} L {c['L_guide']:.1f} | "
              f"opt w_in {b['w_in']} w_out {b['w_out']} (x{b['w_out']/c['det_wh']:.2f} det_wh) m {b['m_coat']} "
              f"(limit {lim_m:.2f}) | old readout rule reaches {rule/fb:.2f} of optimum", flush=True)
    # best single constant across these 16 instances (fraction of each optimum)
    keys = list(res[0])
    def frac(k): return [res[i][k] / max(res[i].values()) for i in idx]
    bestc = max(keys, key=lambda k: sum(x > 0.85 for x in frac(k)))
    print("best constant:", bestc, "passes 0.85 on", sum(x > 0.85 for x in frac(bestc)), "/16; median frac", round(st.median(frac(bestc)), 3))
    json.dump({str(i): res[i] for i in idx}, open("/netdisk/ldq/tmp/guide_tradeoff_proto.json", "w"))
