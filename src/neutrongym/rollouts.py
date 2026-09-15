"""M8 rejection-sampling collector — env-dialogue rollouts -> filtered SFT.

The trainable core is parametric: an episode is a DIALOGUE against the gym
env (not the MCP tool loop) — the model reads the instance's task sheet,
proposes a JSON action over the free parameters, receives level-resolved
feedback (deepest level, failure detail, FOM ratio), and iterates to the
step cap. Rejection sampling keeps episodes whose best step clears a reward
threshold and exports them as SFT-ready message lists.

Format decision (recorded for review): SFT data = env-dialogue transcripts.
The alternative — full MCP tool-call trajectories through the reference
loop — remains available for a tool-use SFT stage later; the scaffold note
requires only that TRAINED-MODEL EVALUATION runs through the reference
loop, which is unchanged by the training-data format.

    from neutrongym.rollouts import collect
    collect(model="anthropic/claude-sonnet-5", n_instances=50,
            out_path="runs/m8/sft.jsonl", provider_pin="Google")
"""

import json
import os
import re
import time

from .env import NeutronGym

DIALOGUE_SYSTEM = (
    "You are optimizing a neutron instrument. Each turn, reply with ONLY a "
    "JSON object assigning every free design parameter, e.g. "
    '{"w_in": 0.05, "w_out": 0.03, "m_coat": 2.5}. You will receive '
    "simulation feedback (deepest validation level reached, figure-of-merit "
    "ratio vs baseline, constraint details). Improve on the baseline within "
    "the given bounds.")


def parse_action(answer: str, free: dict) -> dict | None:
    """The last JSON object in the answer that covers the free parameters."""
    for m in reversed(re.findall(r"\{[^{}]*\}", answer or "")):
        try:
            obj = json.loads(m)
        except json.JSONDecodeError:
            continue
        if isinstance(obj, dict) and set(free) <= set(obj):
            return {k: obj[k] for k in free}
    return None


def feedback_message(obs) -> str:
    fb = obs["feedback"]
    if fb is None:
        return "Begin."
    parts = [f"deepest level reached: L{fb['level']}"]
    if fb["failed_at"]:
        parts.append(f"failed at {fb['failed_at']}: {fb['detail']}")
    if fb["fom_ratio"] is not None:
        parts.append(f"FOM ratio vs baseline: {fb['fom_ratio']}")
    parts.append("Reply with your next JSON action.")
    return "; ".join(parts)


def rollout(env: NeutronGym, index: int, chat_fn) -> dict:
    """One dialogue episode; chat_fn(messages) -> assistant text."""
    obs, info = env.reset(index=index)
    if obs["instance"].get("no_headroom"):
        # no improvement exists at this bar; an episode here would teach or
        # score resubmitting the baseline (2026-09-15)
        return {"instance_id": info["instance_id"], "split": info["split"],
                "skipped": "no_headroom", "messages": [], "best_reward": 0.0,
                "best_level": None, "episode": []}
    messages = [{"role": "system", "content": DIALOGUE_SYSTEM},
                {"role": "user", "content": obs["prompt"]}]
    best_reward, best_level = 0.0, 0
    steps = 0
    while True:
        answer = chat_fn(messages)
        messages.append({"role": "assistant", "content": answer})
        action = parse_action(answer, obs["instance"]["free_parameters"])
        if action is None:
            messages.append({"role": "user",
                             "content": "Could not parse a JSON action "
                                        "covering all free parameters. "
                                        "Reply with ONLY the JSON object."})
            steps += 1
            if steps >= env.max_steps:
                break
            continue
        obs, reward, terminated, truncated, rec = env.step(action)
        steps += 1
        best_reward = max(best_reward, reward)
        best_level = max(best_level, rec["level"])
        messages.append({"role": "user", "content": feedback_message(obs)})
        if terminated or truncated:
            break
    return {"instance_id": info["instance_id"], "split": info["split"],
            "messages": messages, "best_reward": best_reward,
            "best_level": best_level,
            "episode": env.episode_record()["steps"]}


def evaluate(model: str, n_instances: int, family: str, split: str,
             base_url: str | None = None, api_key: str | None = None,
             provider_pin: str | None = None, temperature: float = 0.0,
             max_steps: int = 6, start_index: int = 0,
             chat_fn=None, target_fraction: float | None = None) -> dict:
    """Measure a policy on procedural instances WITHOUT filtering — the
    primitive behind both the phase-0 baselines and the phase-3 trained-vs-
    untrained comparison, so both are measured identically.

    Returns per-instance best level/reward/FOM ratio plus the level
    histogram. Temperature 0 by default: this is measurement, not sampling.
    """
    from .agent import (LOCAL_REQUEST_TIMEOUT_S, REQUEST_TIMEOUT_S,
                        chat_completion, resolve_backend)
    import httpx

    env = NeutronGym(family=family, split=split, max_steps=max_steps,
                     target_fraction=target_fraction)
    if chat_fn is not None:
        call_model = chat_fn
    else:
        b_url, key = resolve_backend(model, base_url, api_key)
        local = bool(base_url)
        extra = ({"chat_template_kwargs": {"enable_thinking": False}}
                 if local else None)
        http = httpx.Client()

        def call_model(msgs):
            resp = chat_completion(
                http, b_url, key, model, msgs, [], temperature,
                None if local else provider_pin, chat_extra=extra,
                timeout=(LOCAL_REQUEST_TIMEOUT_S if local
                         else REQUEST_TIMEOUT_S))
            return resp["choices"][0]["message"].get("content") or ""

    rows, t0, skipped = [], time.time(), []
    for i in range(start_index, start_index + n_instances):
        try:
            ep = rollout(env, i, call_model)
        except Exception as e:  # noqa: BLE001 — one bad episode must not
            rows.append({"instance": i, "error": str(e)[:120],
                         "best_level": None, "best_reward": None})
            continue
        if ep.get("skipped"):
            # dropped from every arm alike: same cached calibration, so the
            # paired comparison stays on identical instances
            skipped.append(i)
            continue
        best_fom = max((s.get("levels", {}).get("L4", {}).get("fom_ratio")
                        or 0) for s in ep["episode"]) if ep["episode"] else 0
        # the action that first cleared L4 — needed to audit HOW a pass was
        # earned (e.g. the SANS direct-beam leak, 2026-09-13), which a pass
        # rate or a FOM ratio alone cannot show
        passing = next((s for s in ep["episode"] if s.get("level") == 4),
                       None)
        rows.append({"instance": i, "instance_id": ep["instance_id"],
                     "best_level": ep["best_level"],
                     "best_reward": ep["best_reward"],
                     "best_fom_ratio": best_fom,
                     "pass_action": (dict(passing["action"])
                                     if passing else None),
                     "steps": len(ep["episode"])})
    hist = {lv: sum(1 for r in rows if r["best_level"] == lv)
            for lv in range(5)}
    valid = [r for r in rows if r["best_level"] is not None]
    return {"model": model, "family": family, "split": split,
            "target_fraction": target_fraction,
            "skipped_no_headroom": len(skipped),
            "n": len(rows), "n_valid": len(valid), "errors":
            len(rows) - len(valid), "level_histogram": hist,
            "pass_rate": (round(hist[4] / len(valid), 4) if valid else None),
            "mean_best_reward": (round(sum(r["best_reward"] for r in valid)
                                       / len(valid), 4) if valid else None),
            "wall_s": round(time.time() - t0, 1), "rows": rows}


def collect(model: str, n_instances: int, out_path: str,
            family: str = "guide_divergence", split: str = "train",
            reward_threshold: float = 1.0, max_steps: int = 6,
            temperature: float = 0.7, provider_pin: str | None = "Google",
            base_url: str | None = None, api_key: str | None = None,
            chat_fn=None, target_fraction: float | None = None,
            start_index: int = 0) -> dict:
    """Rejection-sampled SFT set: keep episodes with best_reward >=
    threshold (default 1.0 = reached L3-valid with full structural pass;
    1.0+ means improved). Sampling temperature deliberately > 0 — diversity
    is the point; kept episodes are re-validated by their recorded rewards,
    not by sampling settings. chat_fn injectable for tests."""
    from .agent import (LOCAL_REQUEST_TIMEOUT_S, REQUEST_TIMEOUT_S,
                        chat_completion, resolve_backend)
    import httpx

    env = NeutronGym(family=family, split=split, max_steps=max_steps,
                     target_fraction=target_fraction)
    if chat_fn is not None:
        call_model = chat_fn
    else:
        # base_url routes to a LOCALLY served model — required for
        # self-generated (RAFT) sampling, which is both free and the only
        # design that isolates the reward's contribution from distillation
        # (peer review 2026-09-12). Without it resolve_backend rejects a
        # bare id like "qwen3-8b".
        b_url, key = resolve_backend(model, base_url, api_key)
        local = bool(base_url)
        # local models serve non-thinking ONLY via this per-request field
        extra = ({"chat_template_kwargs": {"enable_thinking": False}}
                 if local else None)
        http = httpx.Client()

        def call_model(msgs):
            resp = chat_completion(
                http, b_url, key, model, msgs, [], temperature,
                None if local else provider_pin, chat_extra=extra,
                timeout=(LOCAL_REQUEST_TIMEOUT_S if local
                         else REQUEST_TIMEOUT_S))
            return resp["choices"][0]["message"].get("content") or ""

    os.makedirs(os.path.dirname(os.path.abspath(out_path)), exist_ok=True)
    kept = total = skipped = 0
    t0 = time.time()
    with open(out_path, "w") as f:
        # start_index lets a second collection batch cover a disjoint
        # instance range instead of resampling the same instances
        for idx in range(start_index, start_index + n_instances):
            ep = rollout(env, idx, call_model)
            if ep.get("skipped"):
                skipped += 1
                continue
            total += 1
            if ep["best_reward"] >= reward_threshold:
                kept += 1
                f.write(json.dumps({
                    "instance_id": ep["instance_id"], "split": ep["split"],
                    "model": model, "best_reward": ep["best_reward"],
                    "best_level": ep["best_level"],
                    "messages": ep["messages"]}) + "\n")
    return {"model": model, "base_url": base_url,
            "self_generated": bool(base_url),
            "family": family, "split": split,
            "instances": total, "kept": kept, "start_index": start_index,
            "skipped_no_headroom": skipped,
            "keep_rate": round(kept / total, 3) if total else 0.0,
            "reward_threshold": reward_threshold,
            "wall_s": round(time.time() - t0, 1), "out": out_path}
