# M8 execution plan — SFT trainability result (2026-09-12, FOR REVIEW)

**Status: proposed, not started. Awaiting user approval.**
Deadlines: **abstract Sep 18 (6 days) · full paper Sep 25 (13 days).**
Budget: **$77.84** of the $200 OpenRouter ceiling remains. A100s: 8 free
as of Sep 10 (re-verify). Both vLLM endpoints alive and supervised.

---

## 1. The claim we are trying to earn

Standing bar, verbatim: *7B + training beats the 7B baseline, approaching
a larger untrained model. The delta validates the environment, not a
frontier agent.*

**Primary readout is level migration, not just pass rate.** Both untrained
baselines are L0-dominated on the benchmark (tools engaged, instrument
never completed). If filtered SFT works, L0 should convert into
L1–L4/PASS. That is measurable even if pass counts move little, and it
directly tests "does physics-verifiable reward teach task completion?"

## 2. Design decisions — these are what I most want reviewed

### 2.1 Train and evaluate on the PROCEDURAL axis (not the T1 benchmark)

Training data will be env-dialogue rollouts (JSON parameter actions on
procedural instances). The M6 benchmark is MCP tool-loop *construction* of
T1 instruments. Those differ in both interaction format and skill, so
training on one and claiming on the other would be a train/test mismatch
that most likely shows nothing — and we would burn the window learning it.

**Proposal:** the headline claim lives on **held-out procedural regimes**
(the generalization axis: context parameters drawn from intervals disjoint
from training — `L_guide` 12.5–16 m vs train 6–12 m; `L_coll` 4.5–6 m vs
train 2–4 m). T1 benchmark numbers are reported as a **transfer check**,
with the format mismatch stated, never as the headline.

*Why this is defensible:* research question 3 is "does physics-verifiable
reward train?" — the procedural core IS the trainable object; McStasBench
is the held-out slice for questions 1–2. The scaffold decision requires
only that trained-model evaluation runs through the reference loop, which
it will.

### 2.2 This axis gives us the statistical power M6 lacked

M6's arm comparisons died at n=17 (all p ≥ 0.70). **Procedural instances
are free, deterministic, and unlimited** — 0.03 s/rollout, $0 API. We can
run **n = 200 instances per condition**, which resolves a 10-point pass-rate
difference comfortably. This is a genuine methodological advantage of the
environment and should be stated as such in the paper.

### 2.3 Baselines must be measured on the SAME axis, before training

**Gap in my earlier recommendation:** the M6 baselines (8B 1/17, 32B 3/17)
are benchmark numbers. They are NOT baselines for a procedural-axis claim.
So phase 1 includes measuring **untrained 8B and untrained 32B on the same
held-out procedural instances** the trained model will face. Free, local,
~1 h. Without this there is no denominator.

---

## 3. Phases

| # | Phase | When | Cost | Blocking? |
|---|---|---|---|---|
| 0 | Prereqs + baselines on the procedural axis | Sep 12 | $0 | yes |
| 1 | Rejection sampling (data generation) | Sep 12–13 | ~$25 | yes |
| 2 | SFT training (LoRA, Qwen3-8B) | Sep 13–15 | $0 | yes |
| 3 | Evaluation + level-migration analysis | Sep 15–16 | $0 | yes |
| 4 | Freeze, write into paper | Sep 17 | — | — |
| — | **ABSTRACT DUE** | **Sep 18** | | |
| 5 | GRPO go/no-go (explicit gate, see §6) | Sep 17 | — | no |
| — | **PAPER DUE** | **Sep 25** | | |

### Phase 0 — prereqs (Sep 12, ~2 h, $0.05)
- Re-run the 10 infra'd **held-out benchmark cells** (needs your OK).
- **Measure untrained 8B + 32B on 200 held-out procedural instances each**
  via `rollouts.py` scoring only (no training) → the real baselines.
- Verify A100 availability and `/netdisk` space for a training env.
- Confirm the serving chat template matches what training will assume
  (non-thinking, the pinned config).

### Phase 1 — rejection sampling (Sep 12–13, ~$25)
- Generators: **claude-sonnet-5** (best M6 scorer) + **gemini-3.6-flash**
  (cheap breadth). Both already pinned and verified.
- Target: **~1500 rollouts across both families, train split only**, at
  temperature 0.7 for diversity; keep episodes whose best step reaches
  **reward ≥ 1.0** (structurally valid + improvement).
- Expected keep rate 30–60% → **~500–900 SFT examples**. Enough for LoRA.
- Held-out regimes are NEVER sampled for training data.
- Cost estimate: env-dialogue episodes are small (~3–6k tokens each);
  1500 × ~5k ≈ 7.5M tokens ≈ $15–30 at the pinned prices.

### Phase 2 — SFT (Sep 13–15, $0)
- **Qwen3-8B + LoRA r=32–64 on ALL linear projections** (standing decision
  — attention-only low-rank is where "LoRA underperforms" comes from).
- Framework: TRL `SFTTrainer` + peft, installed to **/netdisk** (root FS
  is full; reuse the vllm-env2 pattern with `HF_HOME` relocated).
- Masked loss on assistant turns only; chat template identical to serving.
- Small sweep if time permits: 1–3 epochs, LR 1e-4/2e-4. Otherwise one run.
- Checkpoint served via vLLM (adapter or merged) for evaluation.

### Phase 3 — evaluation (Sep 15–16, $0)
Four conditions on the **same 200 held-out procedural instances**:
1. untrained 8B (baseline)
2. **trained 8B (the claim)**
3. untrained 32B (comparator)
4. random/classical reference (already have the machinery)

Outputs: pass rate with Fisher CIs, **level histogram migration
(L0→L1–L4/PASS)**, FOM-ratio distributions, and a T1 benchmark transfer
check (17 tasks, secondary, mismatch stated).

---

## 4. Risk register

| Risk | Likelihood | Mitigation |
|---|---|---|
| Training-stack install repeats the vLLM saga (full root FS, no sudo) | **high** | reuse the proven `/netdisk` + uv pattern; **timebox to 3 h**, then fall back to a CPU-free alternative or abort to env+eval-only |
| A100s reoccupied by other users | medium | check first; LoRA on 8B needs 1–2 cards, not 8 |
| Keep rate too low → thin SFT set | medium | raise rollout count (cheap); or lower threshold to ≥0.75 (structurally valid) and report the relaxation |
| Trained model regresses (catastrophic forgetting of tool format) | medium | LoRA (low LR), eval checkpoints, keep untrained as control |
| No delta at all | **real** | report honestly — a null result on a physics-verifiable reward IS a finding, and the ladder drops to env+eval-only |
| Any single failure eats the margin | high | hard abort gate, §6 |

## 5. What this does NOT include

- **GRPO**: cannot fit 13 days with any margin (needs days/run plus
  analysis). Treated as a gated stretch, §6 — not planned content.
- **T3 rubric**, T1 growth: already cut.

## 6. Decision gates (dates are hard)

- **Sep 14 EOD** — if no training run has *started*, abandon SFT and write
  env+eval-only. (Protects the paper over the result.)
- **Sep 16 EOD** — if no trained checkpoint has been *evaluated*, freeze
  what exists; abstract claims env+eval-only.
- **Sep 17** — RL numbers frozen, fresh-seed re-verified, whatever they say.
- **Sep 17** — GRPO go/no-go: **my recommendation is NO**, unless SFT
  finished early AND shows a delta AND ≥5 clear days remain.

## 7. What I need from you

1. **Approve the procedural-axis design** (§2.1) — the single most
   consequential choice here.
2. **Approve the 10 held-out benchmark cells** re-run (§ phase 0).
3. **Confirm ~$25–30 of the remaining $77.84** for generation.
4. **Confirm the A100s are free for training** (or tell me who to ask).
5. Note the gates in §6 — I will hold them rather than let the paper slip.

## 8. Running in parallel regardless (no approval needed)

Figure 1 and the paper skeleton, per the Figure-1-first discipline. These
do not depend on M8 and the abstract is due in 6 days either way.
