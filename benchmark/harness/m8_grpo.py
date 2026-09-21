"""M8 step-level GRPO for Qwen3-8B (2026-09-16).

Why: RAFT/passing-turn SFT regressed three times. It clones the model's own
successful turns, so it only sees states the model already handles and learns
rules like "keep r_pin1, snap r_pin2 to the hint" that trap it elsewhere.

What: each training example is ONE decision -- a message prefix from the
untrained 8B's own episodes (m8_states.py), failures included. For each state
sample G completions from the current policy, score each action with the
environment's reward (reward_server.py; reward depends only on instance and
action), and update with a group-normalised policy gradient plus a KL penalty
to the frozen base (LoRA disabled). Groups with no reward spread carry no
signal and are skipped.

Same template discipline as m8_train.py: prompts are rendered with the chat
template exactly as vLLM renders them (enable_thinking=False), and only
completion tokens carry loss. Logits are computed only for the completion
positions (left padding), because full-vocabulary logits over 8 x 4k-token
prompts would not fit on a 40 GB GPU.

Usage (DGX, GPU 7):
  CUDA_VISIBLE_DEVICES=7 /netdisk/ldq/sft-env/bin/python m8_grpo.py \\
     --states grpo_states_guide.jsonl --base <Qwen3-8B> --out <ckpt dir> \\
     --reward-url http://127.0.0.1:8199/score --family guide_divergence
"""

import argparse
import json
import math
import os
import random
import time

TARGET_MODULES = ["q_proj", "k_proj", "v_proj", "o_proj",
                  "gate_proj", "up_proj", "down_proj"]
TEMPLATE_KWARGS = {"enable_thinking": False}


def load_states(path: str, max_turn: int | None = None) -> list:
    """(family, index, prompt_messages) for every decision point in every
    non-errored, non-skipped episode."""
    out = []
    for line in open(path):
        if not line.strip():
            continue
        ep = json.loads(line)
        if ep.get("error") or ep.get("skipped"):
            continue
        msgs = ep.get("messages") or []
        turn = 0
        for k, m in enumerate(msgs):
            if m.get("role") == "assistant" and k > 0:
                turn += 1
                if max_turn is None or turn <= max_turn:
                    out.append({"family": ep["family"], "index": ep["index"],
                                "turn": turn, "messages": msgs[:k]})
    return out


def shape_rewards(results: list, sparse: bool = False) -> list:
    """Environment reward per sampled action, or pass/fail only.

    The ladder gives partial credit for a valid, well-measured design near the
    target (0.75 + 0.25 * min(ratio, 2)); sparse gives 1.0 for an L4 pass and
    0.0 otherwise. Ablation pre-registered 2026-09-18.
    """
    if sparse:
        return [1.0 if r["level"] == 4 else 0.0 for r in results]
    return [r["reward"] for r in results]


def group_advantages(rewards: list, min_std: float = 1e-3):
    """(r - mean) / std within one group; None when the group has no spread."""
    n = len(rewards)
    mean = sum(rewards) / n
    std = math.sqrt(sum((r - mean) ** 2 for r in rewards) / n)
    if std < min_std:
        return None
    return [(r - mean) / std for r in rewards]


def trim_completion(ids: list, eos_ids: set, pad_id: int) -> list:
    """Generated ids up to and including the first end token; padding dropped."""
    out = []
    for t in ids:
        if t == pad_id and t not in eos_ids:
            break
        out.append(t)
        if t in eos_ids:
            break
    return out


def left_pad_batch(prompt_ids: list, completions: list, pad_id: int):
    """Sequences prompt+completion left-padded to a common length, so every
    completion ends at the last position. -> (input_ids, attention, comp_mask,
    width) where comp_mask marks completion tokens in the last `width`
    positions (the only logits that are computed)."""
    width = max(len(c) for c in completions)
    seqs = [prompt_ids + c for c in completions]
    total = max(len(s) for s in seqs)
    ids, attn, cmask = [], [], []
    for c, s in zip(completions, seqs):
        pad = total - len(s)
        ids.append([pad_id] * pad + s)
        attn.append([0] * pad + [1] * len(s))
        # right-aligned completions: last len(c) positions of the width window
        cmask.append([0] * (width - len(c)) + [1] * len(c))
    return ids, attn, cmask, width


def post_json(url: str, payload: dict, timeout: float = 600.0) -> dict:
    import urllib.request
    req = urllib.request.Request(url, data=json.dumps(payload).encode(),
                                 headers={"Content-Type": "application/json"})
    with urllib.request.urlopen(req, timeout=timeout) as r:
        return json.loads(r.read())


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--states", required=True,
                    help="one states file, or several comma-separated")
    ap.add_argument("--base", required=True)
    ap.add_argument("--out", required=True)
    ap.add_argument("--family", required=True,
                    help="one family, or several comma-separated for joint training")
    ap.add_argument("--split", default="train")
    ap.add_argument("--reward-url", default="http://127.0.0.1:8199/score")
    ap.add_argument("--steps", type=int, default=120)
    ap.add_argument("--states-per-step", type=int, default=8)
    ap.add_argument("--group", type=int, default=8)
    ap.add_argument("--temperature", type=float, default=1.0)
    ap.add_argument("--max-new-tokens", type=int, default=96)
    ap.add_argument("--max-prompt", type=int, default=6000)
    ap.add_argument("--lr", type=float, default=1e-5)
    ap.add_argument("--kl", type=float, default=0.02)
    ap.add_argument("--rank", type=int, default=32)
    ap.add_argument("--alpha", type=int, default=64)
    ap.add_argument("--save-every", type=int, default=30)
    ap.add_argument("--seed", type=int, default=20260916)
    ap.add_argument("--init-adapter", default=None,
                    help="continue from a saved LoRA adapter (e.g. adapter_step120)")
    ap.add_argument("--sparse-reward", action="store_true",
                    help="pass/fail reward only (ablation of the ladder's shaping)")
    ap.add_argument("--start-step", type=int, default=0,
                    help="step number of --init-adapter, so logs and saves continue from it")
    ap.add_argument("--dry-run", action="store_true",
                    help="one step with generation and scoring, no update")
    a = ap.parse_args()

    import torch
    from peft import LoraConfig, get_peft_model
    from transformers import AutoModelForCausalLM, AutoTokenizer
    sys_path = os.path.dirname(os.path.abspath(__file__))
    import sys
    sys.path.insert(0, sys_path)
    from m8_train import token_ids

    rng = random.Random(a.seed + a.start_step)
    torch.manual_seed(a.seed)
    os.makedirs(a.out, exist_ok=True)
    families = [f.strip() for f in a.family.split(",") if f.strip()]
    states = []
    for path in [p.strip() for p in a.states.split(",") if p.strip()]:
        states += [s for s in load_states(path) if s["family"] in families]
    tok = AutoTokenizer.from_pretrained(a.base)
    pad_id = tok.pad_token_id if tok.pad_token_id is not None else tok.eos_token_id
    eos_ids = {tok.convert_tokens_to_ids("<|im_end|>"), tok.eos_token_id}

    def render(msgs):
        return token_ids(tok.apply_chat_template(msgs, add_generation_prompt=True,
                                                 tokenize=True, **TEMPLATE_KWARGS))

    model = AutoModelForCausalLM.from_pretrained(a.base, dtype=torch.bfloat16).cuda()
    model.gradient_checkpointing_enable()
    model.enable_input_require_grads()
    if a.init_adapter:
        from peft import PeftModel
        model = PeftModel.from_pretrained(model, a.init_adapter, is_trainable=True)
    else:
        model = get_peft_model(model, LoraConfig(r=a.rank, lora_alpha=a.alpha,
                                                 target_modules=TARGET_MODULES,
                                                 lora_dropout=0.0, task_type="CAUSAL_LM"))
    opt = torch.optim.AdamW([p for p in model.parameters() if p.requires_grad], lr=a.lr)
    log = open(os.path.join(a.out, "grpo_log.jsonl"), "a")
    from collections import Counter
    print(f"states: {len(states)} across {dict(Counter(s['family'] for s in states))}", flush=True)

    def completion_logprobs(ids, attn, cmask, width, adapter=True):
        ids_t = torch.tensor(ids, device="cuda")
        attn_t = torch.tensor(attn, device="cuda")
        ctx = torch.enable_grad() if adapter else torch.no_grad()
        with ctx:
            if adapter:
                out = model(input_ids=ids_t, attention_mask=attn_t, logits_to_keep=width + 1)
            else:
                with model.disable_adapter():
                    out = model(input_ids=ids_t, attention_mask=attn_t, logits_to_keep=width + 1)
            logits = out.logits[:, :-1, :].float()          # predicts the last `width` tokens
            target = ids_t[:, -width:]
            lp = torch.log_softmax(logits, dim=-1).gather(-1, target.unsqueeze(-1)).squeeze(-1)
        return lp, torch.tensor(cmask, device="cuda", dtype=lp.dtype)

    t0 = time.time()
    for step in range(a.start_step + 1, a.start_step + a.steps + 1):
        batch = rng.sample(states, a.states_per_step)
        groups = []
        model.eval()
        for st in batch:
            p_ids = render(st["messages"])
            if len(p_ids) > a.max_prompt:
                continue
            with torch.no_grad():
                gen = model.generate(torch.tensor([p_ids], device="cuda"),
                                     attention_mask=torch.ones(1, len(p_ids), device="cuda", dtype=torch.long),
                                     do_sample=True, temperature=a.temperature, top_p=1.0,
                                     max_new_tokens=a.max_new_tokens, num_return_sequences=a.group,
                                     pad_token_id=pad_id)
            comps = [trim_completion(g[len(p_ids):].tolist(), eos_ids, pad_id) for g in gen]
            comps = [c if c else [next(iter(eos_ids))] for c in comps]
            texts = [tok.decode(c, skip_special_tokens=True) for c in comps]
            groups.append({"state": st, "prompt": p_ids, "comps": comps, "texts": texts})
        items = [{"index": g["state"]["index"], "family": g["state"]["family"],
                  "completion": t} for g in groups for t in g["texts"]]
        results = post_json(a.reward_url, {"family": families[0], "split": a.split,
                                           "items": items})["results"]
        k = 0
        for g in groups:
            g["results"] = results[k:k + len(g["texts"])]; k += len(g["texts"])
            g["adv"] = group_advantages(shape_rewards(g["results"], a.sparse_reward))
        rewards = shape_rewards(results, a.sparse_reward)
        used = [g for g in groups if g["adv"] is not None]
        stats = {"step": step, "sparse": bool(a.sparse_reward),
                 "reward_mean": round(sum(rewards) / max(len(rewards), 1), 4),
                 "pass_frac": round(sum(r["reward"] > 1.0 for r in results) / max(len(results), 1), 4),
                 "valid_frac": round(sum(r["level"] >= 3 for r in results) / max(len(results), 1), 4),
                 "parse_frac": round(sum(r["parsed"] for r in results) / max(len(results), 1), 4),
                 "groups": len(groups), "groups_with_signal": len(used)}
        if a.dry_run:
            print(json.dumps(stats), flush=True)
            for g in groups[:2]:
                print("  sample:", g["texts"][:3], [r["reward"] for r in g["results"][:3]], flush=True)
            return
        if used:
            model.train()
            opt.zero_grad()
            n_seq = sum(len(g["comps"]) for g in used)
            kl_sum, pg_sum = 0.0, 0.0
            for g in used:
                ids, attn, cmask, width = left_pad_batch(g["prompt"], g["comps"], pad_id)
                with torch.no_grad():
                    ref_lp, _ = completion_logprobs(ids, attn, cmask, width, adapter=False)
                lp, m = completion_logprobs(ids, attn, cmask, width, adapter=True)
                adv = torch.tensor(g["adv"], device="cuda", dtype=lp.dtype).unsqueeze(1)
                ntok = m.sum(dim=1).clamp(min=1)
                diff = ref_lp - lp
                kl = (torch.exp(diff) - diff - 1) * m               # k3 estimator, per token
                pg = -(adv * lp * m)
                loss = ((pg.sum(1) + a.kl * kl.sum(1)) / ntok).sum() / n_seq
                loss.backward()
                kl_sum += float((kl.sum(1) / ntok).sum()); pg_sum += float((pg.sum(1) / ntok).sum())
            torch.nn.utils.clip_grad_norm_([p for p in model.parameters() if p.requires_grad], 1.0)
            opt.step()
            stats.update(kl=round(kl_sum / n_seq, 5), pg=round(pg_sum / n_seq, 5))
        stats["elapsed_s"] = round(time.time() - t0, 1)
        log.write(json.dumps(stats) + "\n"); log.flush()
        print(json.dumps(stats), flush=True)
        if step % a.save_every == 0 or step == a.start_step + a.steps:
            model.save_pretrained(os.path.join(a.out, f"adapter_step{step}"))
    model.save_pretrained(os.path.join(a.out, "adapter"))
    merged = model.merge_and_unload()
    merged.save_pretrained(os.path.join(a.out, "merged"))
    tok.save_pretrained(os.path.join(a.out, "merged"))
    print(f"-> merged {os.path.join(a.out, 'merged')} ({time.time() - t0:.0f}s)", flush=True)


if __name__ == "__main__":
    main()
