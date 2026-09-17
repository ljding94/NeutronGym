import json, math
from neutrongym import generate
from neutrongym.env import NeutronGym
env = NeutronGym(family="guide_divergence", split="heldout", target_fraction=0.85, max_steps=10**6)
res = {}
for w_in in (0.045, 0.05, 0.054, 0.06, 0.07):
    passes = 0
    for i in range(300):
        obs, _ = env.reset(index=i); c = obs["instance"]["context"]
        a = {"w_in": w_in, "w_out": c["det_wh"],
             "m_coat": math.floor(generate.guide_max_m(c) * 1000) / 1000}   # the prompt's stated limit, 3 d.p.
        passes += env.step(a)[4]["level"] == 4
    res[w_in] = passes
    print(f"rule policy (w_out = stated limit, m_coat = stated limit, w_in = {w_in}): {passes}/300 = {passes/300:.1%}", flush=True)
json.dump(res, open("runs/m8/guide_rule_policy_heldout.json", "w"))
