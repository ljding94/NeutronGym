"""M8 phase 2 — LoRA SFT of Qwen3-8B on self-generated RAFT dialogues.

Runs on the DGX (GPU 7) in the dedicated training env, not the serving env.
Deliberately plain PyTorch + transformers + peft rather than a trainer
framework: the loss mask is the correctness-critical part of this whole
milestone, so it is written out and unit-tested here instead of trusted to
a library whose chat-template handling changes between releases.

PER-TURN PAIRS — how the two template traps are avoided by construction.
Each RAFT episode is a multi-turn dialogue. At step k the server rendered
the prompt as

    apply_chat_template(messages[:k], add_generation_prompt=True,
                        enable_thinking=False)

and the model produced messages[k]. Training therefore uses exactly that
(prompt, completion) pair for EVERY assistant turn: the prompt tokens are
masked, the completion plus its end-of-turn token carry the loss.

  * Template identity (plan §5.1): the prompt is rendered by the same
    tokenizer, same kwargs, same call the server made, so train-time and
    serve-time text cannot diverge — including Qwen3's empty think block,
    which the template injects only into the generation prompt.
  * Loss on every assistant turn (plan §5.3): one pair per turn means no
    turn can be silently left unmasked or masked out.

`--check-server URL` asks the running vLLM server to tokenize the same
messages and fails loudly if its token ids differ from ours.

LoRA r=32 on all seven projections; adapter merged into the base weights at
the end so evaluation serves plain weights (plan §5.4–5.5).

Usage (on the DGX):
  /netdisk/ldq/sft-env/bin/python m8_train.py --data train.jsonl \
      --base /netdisk/ldq/hf/.../Qwen3-8B --out /netdisk/ldq/ckpt/m8-raft
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
IGNORE = -100


def token_ids(encoded) -> list:
    """Plain token-id list from whatever apply_chat_template returned.

    transformers 5 returns a BatchEncoding for tokenize=True — a UserDict,
    NOT a dict — so an isinstance(x, dict) test misses it and list(x) yields
    its KEYS ("input_ids", "attention_mask"). That silently turned every
    prompt into two tokens until the server identity check caught it on
    2026-09-13. Handle mappings by key, and unwrap a batch of one.
    """
    if hasattr(encoded, "keys") and "input_ids" in encoded.keys():
        encoded = encoded["input_ids"]
    if hasattr(encoded, "tolist"):
        encoded = encoded.tolist()
    ids = list(encoded)
    if ids and isinstance(ids[0], (list, tuple)):
        if len(ids) != 1:
            raise ValueError(f"expected one sequence, got a batch of {len(ids)}")
        ids = list(ids[0])
    if not all(isinstance(t, int) for t in ids):
        raise TypeError("token ids must be ints — got "
                        f"{type(ids[0]).__name__ if ids else 'nothing'}")
    return ids


def turn_pairs(messages: list) -> list:
    """One (prompt_messages, completion_text) pair per assistant turn."""
    pairs = []
    for k, m in enumerate(messages):
        if m.get("role") == "assistant" and k > 0:
            pairs.append((messages[:k], m.get("content") or ""))
    return pairs


def encode_pair(tok, prompt_messages: list, completion: str,
                max_len: int, eos_text: str = "<|im_end|>") -> dict | None:
    """-> {input_ids, labels} with the prompt masked; None if it cannot fit
    without truncating the completion (never train on a clipped answer)."""
    prompt_ids = token_ids(tok.apply_chat_template(
        prompt_messages, add_generation_prompt=True, tokenize=True,
        **TEMPLATE_KWARGS))
    comp_ids = token_ids(tok(completion + eos_text,
                             add_special_tokens=False))
    if len(prompt_ids) + len(comp_ids) > max_len:
        return None
    return {"input_ids": list(prompt_ids) + list(comp_ids),
            "labels": [IGNORE] * len(prompt_ids) + list(comp_ids)}


def build_examples(records: list, tok, max_len: int) -> tuple[list, dict]:
    examples, dropped = [], 0
    for rec in records:
        if rec.get("best_level") != 4:
            continue
        for prompt_messages, completion in turn_pairs(rec["messages"]):
            ex = encode_pair(tok, prompt_messages, completion, max_len)
            if ex is None:
                dropped += 1
            else:
                examples.append(ex)
    return examples, {"episodes": len(records), "pairs": len(examples),
                      "dropped_too_long": dropped}


def check_server(tok, url: str, records: list, n: int = 5) -> None:
    """Fail unless the live server tokenizes our prompts identically."""
    import httpx
    model = httpx.get(f"{url}/models", timeout=30).json()["data"][0]["id"]
    base = url.rsplit("/v1", 1)[0]
    checked = 0
    for rec in records:
        for prompt_messages, _ in turn_pairs(rec["messages"]):
            ours = token_ids(tok.apply_chat_template(
                prompt_messages, add_generation_prompt=True, tokenize=True,
                **TEMPLATE_KWARGS))
            resp = httpx.post(f"{base}/tokenize", timeout=60, json={
                "model": model, "messages": prompt_messages,
                "add_generation_prompt": True,
                "chat_template_kwargs": TEMPLATE_KWARGS}).json()
            theirs = resp.get("tokens")
            if list(ours) != list(theirs or []):
                raise SystemExit(
                    f"TEMPLATE MISMATCH vs server on {rec.get('instance_id')}"
                    f": ours {len(ours)} tokens, server "
                    f"{len(theirs or [])} — do not train until fixed")
            checked += 1
            if checked >= n:
                print(f"template identity OK on {checked} prompts vs {url}")
                return
    print(f"template identity OK on {checked} prompts vs {url}")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--data", required=True)
    ap.add_argument("--base", required=True)
    ap.add_argument("--out", required=True)
    ap.add_argument("--rank", type=int, default=32)
    ap.add_argument("--alpha", type=int, default=64)
    ap.add_argument("--lr", type=float, default=1e-4)
    ap.add_argument("--epochs", type=int, default=2)
    ap.add_argument("--tokens-per-step", type=int, default=32000)
    ap.add_argument("--max-len", type=int, default=8192)
    ap.add_argument("--seed", type=int, default=20260913)
    ap.add_argument("--check-server", default=None,
                    help="OpenAI-style base URL of the vLLM server serving "
                         "the same base model, e.g. http://localhost:8137/v1")
    ap.add_argument("--dry-run", action="store_true",
                    help="build examples + template check, no training")
    ap.add_argument("--smoke", type=int, default=0, metavar="N",
                    help="train on the first N examples for one epoch with "
                         "one optimizer step per example, save the adapter, "
                         "skip the merge: checks memory, peft wiring and "
                         "that the loss is finite, in minutes")
    args = ap.parse_args()

    # tokenizer-only until the dry-run exits: the template check needs just
    # transformers + httpx, so it runs in the serving env before the
    # training env (torch/peft) exists
    from transformers import AutoTokenizer

    random.seed(args.seed)
    with open(args.data) as f:
        records = [json.loads(line) for line in f if line.strip()]
    tok = AutoTokenizer.from_pretrained(args.base)
    if args.check_server:
        check_server(tok, args.check_server, records)
    examples, stats = build_examples(records, tok, args.max_len)
    print(f"data: {stats}")
    os.makedirs(args.out, exist_ok=True)
    with open(os.path.join(args.out, "data_stats.json"), "w") as f:
        json.dump({**stats, **vars(args)}, f, indent=1)
    if args.dry_run or not examples:
        return
    if args.smoke:
        examples = examples[:args.smoke]
        args.epochs = 1
        args.tokens_per_step = 1

    import torch
    from peft import LoraConfig, get_peft_model
    from transformers import AutoModelForCausalLM

    torch.manual_seed(args.seed)
    model = AutoModelForCausalLM.from_pretrained(
        args.base, dtype=torch.bfloat16, device_map={"": 0})
    model.gradient_checkpointing_enable()
    model.enable_input_require_grads()
    model = get_peft_model(model, LoraConfig(
        r=args.rank, lora_alpha=args.alpha, lora_dropout=0.0,
        target_modules=TARGET_MODULES, task_type="CAUSAL_LM"))
    model.print_trainable_parameters()
    opt = torch.optim.AdamW((p for p in model.parameters()
                             if p.requires_grad), lr=args.lr,
                            weight_decay=0.0)

    total_tokens = sum(len(e["input_ids"]) for e in examples) * args.epochs
    total_steps = max(1, math.ceil(total_tokens / args.tokens_per_step))
    warmup = max(1, total_steps // 20)

    def lr_at(step):
        if step < warmup:
            return args.lr * (step + 1) / warmup
        prog = (step - warmup) / max(1, total_steps - warmup)
        return args.lr * 0.5 * (1 + math.cos(math.pi * prog))

    log = open(os.path.join(args.out, "train_log.jsonl"), "w")
    model.train()
    step, acc_tokens, acc_loss, acc_n = 0, 0, 0.0, 0
    t0 = time.time()
    for epoch in range(args.epochs):
        order = list(range(len(examples)))
        random.shuffle(order)
        for i in order:
            ex = examples[i]
            ids = torch.tensor([ex["input_ids"]], device=model.device)
            labels = torch.tensor([ex["labels"]], device=model.device)
            n_target = int((labels[:, 1:] != IGNORE).sum())
            out = model(input_ids=ids, labels=labels)
            # token-weighted accumulation: long completions count for what
            # they contain, not one example each
            (out.loss * n_target / args.tokens_per_step).backward()
            acc_loss += float(out.loss) * n_target
            acc_n += n_target
            acc_tokens += len(ex["input_ids"])
            if acc_tokens >= args.tokens_per_step:
                for g in opt.param_groups:
                    g["lr"] = lr_at(step)
                torch.nn.utils.clip_grad_norm_(model.parameters(), 1.0)
                opt.step()
                opt.zero_grad(set_to_none=True)
                rec = {"step": step, "epoch": epoch, "lr": lr_at(step),
                       "loss": acc_loss / max(1, acc_n),
                       "elapsed_s": round(time.time() - t0, 1)}
                log.write(json.dumps(rec) + "\n")
                log.flush()
                print(rec, flush=True)
                step += 1
                acc_tokens, acc_loss, acc_n = 0, 0.0, 0
    if acc_tokens:
        torch.nn.utils.clip_grad_norm_(model.parameters(), 1.0)
        opt.step()
        opt.zero_grad(set_to_none=True)
    log.close()

    adapter_dir = os.path.join(args.out, "adapter")
    model.save_pretrained(adapter_dir)
    if args.smoke:
        peak = torch.cuda.max_memory_allocated() / 2**30
        print(f"SMOKE OK: {step} steps, peak GPU memory {peak:.1f} GiB, "
              f"adapter {adapter_dir}")
        return
    merged = model.merge_and_unload()
    merged_dir = os.path.join(args.out, "merged")
    merged.save_pretrained(merged_dir, safe_serialization=True)
    tok.save_pretrained(merged_dir)
    print(f"-> adapter {adapter_dir}\n-> merged {merged_dir} "
          f"({round(time.time() - t0)}s)")


if __name__ == "__main__":
    main()
