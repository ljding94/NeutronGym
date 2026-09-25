# DGX serving and training scripts (archived 2026-09-25)

Copied off `neutrons-dgx01` before deletion. ORNL cyber policy identified
Qwen3-8B/32B as unapproved for ORNL use on 2026-09-24; all serving processes
were stopped, the model weights and merged checkpoints removed, and these
scripts deleted from the machine at their request.

They are kept here as the **method record** for the paper: they document the
exact serving configuration behind every model number we report, which is not
recoverable from the eval files alone.

| file | what it did |
|---|---|
| `serve-qwen3-8b.sh` | the untrained-8B arm, port 8137 |
| `serve-qwen3-32b.sh` | the untrained-32B arm, tensor-parallel 4, port 8138 |
| `serve-m8-trained.sh` | served a merged fine-tuned checkpoint, port 8139 |
| `m8-serve-launcher.sh`, `m8-serve-launcher-pass.sh` | waited for a merge + free GPU, then served |
| `m8_train.py` | the earlier RAFT/SFT trainer (GRPO lives in `benchmark/harness/m8_grpo.py`) |
| `mcstas-run.sh` | McStas CPU-env wrapper — **no LLM involvement**; retained on the DGX |

**The detail that matters for reproduction:** both base arms ran with YaRN
rope scaling (`factor 4.0`, `original_max_position_embeddings 40960`) to reach
`--max-model-len 98304`. Qwen3's native 40960 sits below the measured 67–77k
peak episode context, so without this the long episodes would have been
truncated and the numbers would not reproduce.

No model weights were ever stored in this repository. The released artifacts
are LoRA adapters, which contain no base-model weights and name
`Qwen/Qwen3-8B` at revision `b968826d9c46dd6066d109eabc6255188de91218`.
