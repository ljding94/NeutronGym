"""SFT loss-mask regressions for m8_train.py — run locally with a fake
tokenizer, no torch or model weights needed.

The mask is the correctness-critical part of M8 training: a prompt token
left in the loss teaches the model to generate the env's feedback, and an
assistant turn masked out silently shrinks the dataset. Both failures are
invisible in the loss curve.
"""

import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))), "benchmark", "harness"))

import m8_train  # noqa: E402

GEN = "<|im_start|>assistant\n<think>\n\n</think>\n\n"


class FakeTok:
    """Character-level tokenizer with a Qwen3-shaped chat template: the empty
    think block appears ONLY in the generation prompt, never in rendered
    history — the exact asymmetry that makes full-conversation rendering
    diverge from what the server produced at each step."""

    def __init__(self):
        self.calls = []

    def _render(self, messages, add_generation_prompt):
        s = "".join(f"<|im_start|>{m['role']}\n{m['content']}<|im_end|>\n"
                    for m in messages)
        return s + (GEN if add_generation_prompt else "")

    def apply_chat_template(self, messages, add_generation_prompt=False,
                            tokenize=True, **kw):
        self.calls.append(kw)
        text = self._render(messages, add_generation_prompt)
        return [ord(c) for c in text] if tokenize else text

    def __call__(self, text, add_special_tokens=False):
        return {"input_ids": [ord(c) for c in text]}


DIALOGUE = [
    {"role": "system", "content": "sys"},
    {"role": "user", "content": "task"},
    {"role": "assistant", "content": "A1"},
    {"role": "user", "content": "fb1"},
    {"role": "assistant", "content": "A2"},
    {"role": "user", "content": "fb2"},
    {"role": "assistant", "content": "A3"},
]


def _decode(ids):
    return "".join(chr(i) for i in ids)


def test_one_pair_per_assistant_turn():
    pairs = m8_train.turn_pairs(DIALOGUE)
    assert [c for _, c in pairs] == ["A1", "A2", "A3"]
    # each prompt is exactly the history the server saw at that step
    assert [len(p) for p, _ in pairs] == [2, 4, 6]
    assert pairs[1][0][-1] == {"role": "user", "content": "fb1"}


def test_trailing_feedback_after_last_answer_is_not_a_target():
    pairs = m8_train.turn_pairs(DIALOGUE + [{"role": "user",
                                             "content": "final fb"}])
    assert len(pairs) == 3


def test_mask_covers_prompt_and_loss_covers_only_the_completion():
    tok = FakeTok()
    prompt, completion = m8_train.turn_pairs(DIALOGUE)[1]
    ex = m8_train.encode_pair(tok, prompt, completion, max_len=10_000)
    assert len(ex["input_ids"]) == len(ex["labels"])
    target = [t for t in ex["labels"] if t != m8_train.IGNORE]
    assert _decode(target) == "A2<|im_end|>"
    n_masked = sum(1 for t in ex["labels"] if t == m8_train.IGNORE)
    masked_text = _decode(ex["input_ids"][:n_masked])
    # the served generation prompt, think block included, is all masked
    assert masked_text.endswith(GEN)
    assert "fb1" in masked_text and "A1" in masked_text


def test_prompt_is_rendered_with_thinking_disabled():
    tok = FakeTok()
    prompt, completion = m8_train.turn_pairs(DIALOGUE)[0]
    m8_train.encode_pair(tok, prompt, completion, max_len=10_000)
    assert tok.calls and all(c.get("enable_thinking") is False
                             for c in tok.calls)


def test_overlong_pair_is_dropped_not_truncated():
    tok = FakeTok()
    prompt, completion = m8_train.turn_pairs(DIALOGUE)[2]
    assert m8_train.encode_pair(tok, prompt, completion, max_len=20) is None


def test_build_examples_skips_non_passing_episodes_and_counts_drops():
    tok = FakeTok()
    recs = [{"instance_id": "a", "best_level": 4, "messages": DIALOGUE},
            {"instance_id": "b", "best_level": 3, "messages": DIALOGUE}]
    examples, stats = m8_train.build_examples(recs, tok, max_len=10_000)
    assert stats == {"episodes": 2, "pairs": 3, "dropped_too_long": 0}
    assert len(examples) == 3


def test_lora_targets_all_seven_projections():
    assert set(m8_train.TARGET_MODULES) == {
        "q_proj", "k_proj", "v_proj", "o_proj",
        "gate_proj", "up_proj", "down_proj"}


class BatchEncodingLike(__import__("collections").UserDict):
    """transformers 5's BatchEncoding: a UserDict (NOT a dict) holding
    input_ids and attention_mask."""


class HFShapedTok(FakeTok):
    def apply_chat_template(self, messages, add_generation_prompt=False,
                            tokenize=True, **kw):
        ids = super().apply_chat_template(messages, add_generation_prompt,
                                          tokenize, **kw)
        return BatchEncodingLike(input_ids=ids,
                                 attention_mask=[1] * len(ids))

    def __call__(self, text, add_special_tokens=False):
        ids = [ord(c) for c in text]
        return BatchEncodingLike(input_ids=ids, attention_mask=[1] * len(ids))


def test_batchencoding_return_is_unwrapped_not_listed_by_keys():
    """Regression for the 2026-09-13 bug: list(BatchEncoding) is its keys,
    which made every prompt two tokens long."""
    enc = BatchEncodingLike(input_ids=[5, 6, 7], attention_mask=[1, 1, 1])
    assert not isinstance(enc, dict)
    assert m8_train.token_ids(enc) == [5, 6, 7]


def test_encode_pair_with_hf_shaped_tokenizer_masks_the_real_prompt():
    prompt, completion = m8_train.turn_pairs(DIALOGUE)[1]
    ex = m8_train.encode_pair(HFShapedTok(), prompt, completion,
                              max_len=10_000)
    n_masked = sum(1 for t in ex["labels"] if t == m8_train.IGNORE)
    assert n_masked > 50, "prompt collapsed to a handful of tokens"
    assert _decode(ex["input_ids"][:n_masked]).endswith(GEN)
    assert _decode([t for t in ex["labels"] if t != m8_train.IGNORE]) == \
        "A2<|im_end|>"


def test_token_ids_unwraps_a_batch_of_one_and_rejects_real_batches():
    import pytest
    assert m8_train.token_ids([[1, 2, 3]]) == [1, 2, 3]
    with pytest.raises(ValueError):
        m8_train.token_ids([[1, 2], [3, 4]])
    with pytest.raises(TypeError):
        m8_train.token_ids(["input_ids", "attention_mask"])


EPISODE = [
    {"role": "system", "content": "sys"},
    {"role": "user", "content": "task"},
    {"role": "assistant", "content": "A1"},
    {"role": "user", "content": "deepest level reached: L2; failed at L2: x; Reply with your next JSON action."},
    {"role": "assistant", "content": "A2"},
    {"role": "user", "content": "deepest level reached: L3; FOM ratio vs baseline: 0.8; Reply with your next JSON action."},
    {"role": "assistant", "content": "A3"},
    {"role": "user", "content": "deepest level reached: L4; FOM ratio vs baseline: 1.01; Reply with your next JSON action."},
]


def test_passing_mode_keeps_only_the_l4_turn_with_its_full_history():
    pairs = m8_train.turn_pairs(EPISODE, "passing")
    assert [c for _, c in pairs] == ["A3"]
    assert len(pairs[0][0]) == 6          # everything the server saw before A3


def test_all_mode_is_unchanged_by_the_feedback_text():
    assert [c for _, c in m8_train.turn_pairs(EPISODE)] == ["A1", "A2", "A3"]


def test_passing_mode_takes_the_first_l4_turn():
    ep = EPISODE + [{"role": "assistant", "content": "A4"},
                    {"role": "user", "content": "deepest level reached: L4; again"}]
    assert [c for _, c in m8_train.turn_pairs(ep, "passing")] == ["A3"]


def test_passing_mode_yields_nothing_without_l4_feedback_and_is_counted():
    tok = FakeTok()
    recs = [{"best_level": 4, "messages": EPISODE},
            {"best_level": 4, "messages": DIALOGUE}]   # no L4 feedback text
    examples, stats = m8_train.build_examples(recs, tok, 10_000, "passing")
    assert len(examples) == 1
    assert stats["episodes_without_passing_turn"] == 1
    assert stats["turns"] == "passing"


def test_turns_rejects_unknown_modes():
    import pytest
    with pytest.raises(ValueError):
        m8_train.turn_pairs(EPISODE, "final")
