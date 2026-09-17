import collections, json, math, statistics as st
from neutrongym import generate, reward
from neutrongym.env import NeutronGym
d = json.load(open("runs/m8/eval_match05grpoLR_n300.json"))
H = d["heldout"]
for arm in ("untrained-8b", "untrained-32b", "trained-8b"):
    rows = H[arm]["guide_match"]["rows"]
    steps = collections.Counter(r["steps"] for r in rows if r["best_level"] == 4)
    acts = [json.dumps(r["pass_action"], sort_keys=True) for r in rows if r.get("pass_action")]
    print(f"{arm:14} passes {sum(r['best_level']==4 for r in rows):3}/300 | distinct pass actions {len(set(acts)):3} | "
          f"turn-to-pass: 1st turn {steps.get(1,0)}, 2-3 {steps.get(2,0)+steps.get(3,0)}, 4+ {sum(v for k,v in steps.items() if k>=4)}")
    allsteps = collections.Counter(r["steps"] for r in rows)
    print(f"{'':14} steps used (all episodes): median {st.median([r['steps'] for r in rows])}, 10-turn episodes {allsteps.get(10,0)}")
# does the trained model beat the physics formula on the SAME instances?
env = NeutronGym(family="guide_match", split="heldout", max_steps=10**9)
tr = {r["instance"]: r for r in H["trained-8b"]["guide_match"]["rows"]}
both = only_t = only_f = neither = 0
fresh_ok = fresh_n = 0
for i in range(300):
    obs, _ = env.reset(index=i); inst = obs["instance"]; c = inst["context"]; t = inst["targets"]
    m = max(1.0, min(3.0, t[1] / ((1/math.sqrt(3)) * 0.099 * c["wl"])))
    spread = c["d_sample"] * math.tan(math.radians(t[1]))
    core = (t[0]/100.0)**2 - spread**2
    w_out = max(0.01, min(0.09, math.sqrt(core)*math.sqrt(12) if core > 0 else 0.01))
    f_pass = env.step({"w_in": 0.08, "w_out": round(w_out,5), "m_coat": round(m,4)})[4]["level"] == 4
    t_pass = tr[i]["best_level"] == 4
    both += t_pass and f_pass; only_t += t_pass and not f_pass; only_f += f_pass and not t_pass; neither += not t_pass and not f_pass
print(f"trained vs physics formula on the same instances: both {both}, trained-only {only_t}, formula-only {only_f}, neither {neither}")
# fresh-seed robustness: re-measure target and action at new seeds
import copy
for i in range(0, 60):
    r = tr[i]
    if not r.get("pass_action"): continue
    obs, _ = env.reset(index=i); inst = obs["instance"]
    for seed in (2024, 4048):
        p = {**inst["context"]}
        out_h = env.exec.run({**p, **inst["hidden_action"]}, ncount=inst["protocol"]["ncount"], seed=seed)
        out_a = env.exec.run({**p, **r["pass_action"]}, ncount=inst["protocol"]["ncount"], seed=seed)
        th = reward.match_measurements(inst, out_h["summary"]); me = reward.match_measurements(inst, out_a["summary"])
        if None in th or None in me: continue
        fresh_n += 1; fresh_ok += all(abs(x-y)/y <= 0.05 for x, y in zip(me, th))
print(f"fresh seeds (60 instances x 2): passing actions still within tolerance {fresh_ok}/{fresh_n}")
