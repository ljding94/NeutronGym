# Released artifacts

Two LoRA adapters for **Qwen/Qwen3-8B**, trained only by step-level GRPO
against NeutronGym's environment reward — no human demonstrations and no
distillation from a stronger model. Results and baselines for each are in
`note/m8-results-summary-2026-09-18.md`.

| adapter | family | untrained 8B | trained 8B | slice |
|---|---|---|---|---|
| `guide_match-grpo-lora` | `guide_match` (±5%) | 11.3% | **76.7%** | held-out 300–599 |
| `sans_match-grpo-lora` | `sans_match` (±1.5%) | 26.0% | **44.3%** | held-out 600–899 |

Both slices are unbiased: neither was used to choose checkpoints, tolerances
or the family design.

## Configuration

LoRA r = 32, alpha = 64, on all seven projections
(q, k, v, o, gate, up, down); bf16. Trained with 120 steps at lr 1e-5
(KL 0.02) then 100 at lr 3e-5 (KL 0.01), 8 states x 8 samples per step;
`sans_match` continued to 340 steps. One A100-40GB, ~3.5 h per family.

## Use

```python
from peft import PeftModel
from transformers import AutoModelForCausalLM, AutoTokenizer

base = AutoModelForCausalLM.from_pretrained("Qwen/Qwen3-8B", dtype="bfloat16")
model = PeftModel.from_pretrained(base, "guide_match-grpo-lora")
tok = AutoTokenizer.from_pretrained("Qwen/Qwen3-8B")
```

Serve the merged weights to reproduce the reported numbers; the evaluation
renders prompts with `enable_thinking=False` and reads back a single JSON
object per turn (`benchmark/harness/m8_eval.py`).

## Files and checksums

The adapter weights are not in this repository (335 MB each). They are staged
at `artifacts/release/` when built from the checkpoints, and identified by:

- `guide_match-grpo-lora/adapter_model.safetensors` — 349MB, sha256 `461342cc2c80409d7c4a37b5d849a94816841988cc8121187a52fdbc51150024`
- `sans_match-grpo-lora/adapter_model.safetensors` — 349MB, sha256 `cc16b744c56d542486971d26e5d2dab252dd4af1c6dd6432f477472e1d1450c9`

`adapter_config.json` records `base_model_name_or_path: Qwen/Qwen3-8B`; the
training-time value was a local path and was rewritten for release.
