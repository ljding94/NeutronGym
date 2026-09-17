import collections, json, statistics as st
from neutrongym import generate, reward, calibrate
from neutrongym.env import NeutronGym
d = json.load(open("runs/m8/eval_guide085grpo_n300.json"))
rows = d["heldout"]["trained-8b"]["guide_divergence"]["rows"]
u = {r["instance"]: r for r in d["heldout"]["untrained-8b"]["guide_divergence"]["rows"]}
print("steps to finish:", sorted(collections.Counter(r["steps"] for r in rows).items()))
acts = [r["pass_action"] for r in rows if r.get("pass_action")]
print("distinct pass actions:", len({json.dumps(a, sort_keys=True) for a in acts}), "of", len(acts))
wo, mc, wi, fr = [], [], [], []
for r in rows:
    a = r.get("pass_action")
    if not a: continue
    c = generate.instance("guide_divergence", "heldout", r["instance"])["context"]
    wo.append(a["w_out"] / c["det_wh"]); mc.append(a["m_coat"] / generate.guide_max_m(c)); wi.append(a["w_in"])
    fr.append(r["best_fom_ratio"])
q = lambda xs: (round(min(xs), 3), round(st.median(xs), 3), round(max(xs), 3))
print("w_out / beam-size limit (min, median, max):", q(wo))
print("m_coat / coating limit  (min, median, max):", q(mc))
print("w_in                    (min, median, max):", q(wi), "distinct", len(set(wi)))
print("FOM / target            (min, median, max):", q(fr), "| > 1/0.85 (beats classical optimum):", sum(x > 1/0.85 for x in fr))
failed = [r["instance"] for r in rows if r["best_level"] != 4]
print("trained failures:", failed, "| untrained passed those:", [u[i]["best_level"] == 4 for i in failed])

# fresh-seed, 10x ncount re-verification of passing actions against the classical optimum
env = NeutronGym(family="guide_divergence", split="heldout", target_fraction=0.85)
wd = env.exec
ok = n = 0; ratios = []
for r in rows[:60]:
    a = r.get("pass_action")
    if not a: continue
    obs, _ = env.reset(index=r["instance"]); inst = obs["instance"]
    rec = json.load(open(calibrate.cache_path(env.workdir + "/guide_divergence", inst)))
    opt = rec["classical_action"]
    for seed in (11, 22, 33):
        p = dict(inst["context"])
        fa = env.exec.run({**p, **a}, ncount=1e6, seed=seed)
        fo = env.exec.run({**p, **opt}, ncount=1e6, seed=seed)
        ga = reward.get_observable(reward._monitor(fa["summary"], inst["fom"]["monitor"]), inst["fom"]["metric"])
        go = reward.get_observable(reward._monitor(fo["summary"], inst["fom"]["monitor"]), inst["fom"]["metric"])
        ratios.append(ga / go); n += 1; ok += ga > 0.85 * go
print(f"fresh seeds x 1e6 rays, first 60 instances x 3 seeds: action/optimum median {st.median(ratios):.3f} min {min(ratios):.3f}; passes 0.85 bar {ok}/{n}")
