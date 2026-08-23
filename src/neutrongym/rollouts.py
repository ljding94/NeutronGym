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


def collect(model: str, n_instances: int, out_path: str,
            family: str = "guide_divergence", split: str = "train",
            reward_threshold: float = 1.0, max_steps: int = 6,
            temperature: float = 0.7, provider_pin: str | None = "Google",
            chat_fn=None) -> dict:
    """Rejection-sampled SFT set: keep episodes with best_reward >=
    threshold (default 1.0 = reached L3-valid with full structural pass;
    1.0+ means improved). Sampling temperature deliberately > 0 — diversity
    is the point; kept episodes are re-validated by their recorded rewards,
    not by sampling settings. chat_fn injectable for tests."""
    from .agent import chat_completion, resolve_backend
    import httpx

    env = NeutronGym(family=family, split=split, max_steps=max_steps)
    if chat_fn is not None:
        call_model = chat_fn
    else:
        b_url, key = resolve_backend(model)
        http = httpx.Client()

        def call_model(msgs):
            resp = chat_completion(http, b_url, key, model, msgs, [],
                                   temperature, provider_pin)
            return resp["choices"][0]["message"].get("content") or ""

    os.makedirs(os.path.dirname(os.path.abspath(out_path)), exist_ok=True)
    kept = total = 0
    t0 = time.time()
    with open(out_path, "w") as f:
        for idx in range(n_instances):
            ep = rollout(env, idx, call_model)
            total += 1
            if ep["best_reward"] >= reward_threshold:
                kept += 1
                f.write(json.dumps({
                    "instance_id": ep["instance_id"], "split": ep["split"],
                    "model": model, "best_reward": ep["best_reward"],
                    "best_level": ep["best_level"],
                    "messages": ep["messages"]}) + "\n")
    return {"model": model, "family": family, "split": split,
            "instances": total, "kept": kept,
            "keep_rate": round(kept / total, 3) if total else 0.0,
            "reward_threshold": reward_threshold,
            "wall_s": round(time.time() - t0, 1), "out": out_path}
