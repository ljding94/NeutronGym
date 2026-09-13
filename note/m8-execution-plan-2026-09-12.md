# M8 execution plan — SFT trainability result (rev 2 · 2026-09-12)

> ## ⛔ PHASE-0 GATE RESULT (2026-09-13): DO NOT PROCEED AS DESIGNED
>
> The gate ran before any spend and **failed on vacuity**. On held-out
> procedural instances with calibrated targets:
>
> | readout | untrained 8B | untrained 32B | discriminates? |
> |---|---|---|---|
> | pass rate (n=20 ea.) | **0.70** | **0.70** | **no — identical** |
> | steps to success (median) | 3.5 | 2.0 | directionally, underpowered |
> | FOM ratio (median/max) | 1.12 / 1.36 | 1.19 / 3.58 | directionally |
>
> (FOM ratio is measured against the CALIBRATED TARGET, not the baseline:
> `fom / (baseline_fom * target_ratio)`, so 1.0 is exactly the bar. A
> median of ~1.1 means the typical episode lands just over it.)
>
> **A 4x parameter increase buys nothing on this task.** "Approaching a
> larger untrained model" is therefore VACUOUS on this axis — there is no
> gap to approach — and a task insensitive to a 4x scale difference is
> unlikely to show an interpretable SFT delta either.
>
> This is the peer review's finding B confirmed empirically: we would have
> been training the job the architecture delegates to scipy. Dense
> per-step FOM feedback turns the parametric task into hill-climbing that
> any competent small model solves; classical search still beats every
> model (RESULTS.md Table 5, 628/510 sigma).
>
> **Prior (uncalibrated) measurement, kept as the "before" record:** 8B
> 80%, 32B 93% — an outright ceiling. Calibration (targets at 0.8x the
> constraint-filtered, fresh-seed-verified classical best) lowered both to
> 70% but did not create discrimination.
>
> **Status: awaiting user decision between the options in §10.** No data
> generated, no GPU time spent, ~$78 budget intact.

**Status: revised after peer review, then STOPPED BY ITS OWN GATE.**
Deadlines: **abstract Sep 18 (6 days) · full paper Sep 25 (13 days).**
Budget: **$77.84** left. Host verified: GPU 7 fully free; GPUs 0–4 are our
own vLLM (8B on 0, 32B TP4 on 1–4); 5–6 hold another user's job; `/netdisk`
94T free, root still 100% full.

**Rev 2 changes the design in three substantial ways.** The review found a
confound that would have sunk the result at review time, a framing problem
about *what* we are training, and a possible vacuity in the claim bar that
must be tested before any money is spent. All three are addressed below.

---

## 1. The claim, narrowed on purpose

Standing bar: *7B + training beats the 7B baseline, approaching a larger
untrained model.*

**Committed framing (rev 2): this is an ENVIRONMENT-TRAINABILITY result —
evidence that the physics-verifiable reward signal is learnable — NOT an
instrument-design-capability result.** The distinction is forced by our own
architecture: `CLAUDE.md` delegates continuous-parameter optimization to
scipy and reserves the agent for topology, and `RESULTS.md` Table 5 shows
classical random search already reaching 2.95× and 1.47× at 628σ/510σ,
unbeaten by any agent. Training an LLM to do the job we gave scipy is only
defensible as a statement about the *reward*, so:

- **The classical reference appears in the headline comparison**, not as an
  afterthought. A trained 8B far below classical is the honest context.
- The paper says plainly that this is not a claim that LLMs should replace
  the optimizer.

## 2. Data generation: SELF-GENERATED (RAFT), not distillation

**Rev-2 change, and the most important one.** The original plan sampled
from claude-sonnet-5 and gemini-3.6-flash, filtered by reward, and trained
the 8B on the survivors. That is **knowledge distillation from frontier
models with the reward acting only as a selector over someone else's
behaviour** — and the obvious null ("imitating sonnet teaches completion")
cannot be separated from our claim, because the reward never generated
anything. A reviewer says this in one line.

**New primary path: sample from the untrained 8B itself, keep
reward ≥ threshold, train on its own successes (RAFT).** This isolates the
reward's contribution, and it runs on our own vLLM at **$0**.

- Blocker found by review and **now fixed**: `rollouts.collect()` could not
  reach local models (`resolve_backend` rejects a bare id without
  `base_url`). Plumbed through, with the non-thinking flag and the local
  timeout, in this commit.
- **Fallback if self-generated data is too thin:** seed from frontier
  models, but then run the ablation that separates the hypotheses —
  **train on UNFILTERED vs REWARD-FILTERED data from the same source.**
  Filtered > unfiltered is the evidence that the reward does work. The
  second training run is free and local.

## 3. Phase 0 is now a HARD GATE, before any spend

Three stop conditions, all measured on **held-out procedural instances**,
all free:

| Check | Stop condition | Why |
|---|---|---|
| **32B vacuity** | if untrained 32B ≈ untrained 8B | "approaching a larger untrained model" has no target — the bar cannot be evaluated. Redefine the readout (level migration / FOM distribution) BEFORE generating data |
| **8B keep rate** | if self-generated keep rate ≈ 0 | RAFT has nothing to train on; decide seed-vs-abort with evidence, not hope |
| **Baselines exist** | — | M6's 1/17 and 3/17 are BENCHMARK numbers and are not denominators for a procedural claim |

Phase 0 doubles as the feasibility test for phase 1: **if the 8B's keep
rate is healthy we may not need to spend anything at all.**

## 4. Statistics — pre-registered before data exists

- **Primary endpoint, named now:** level migration on held-out procedural
  instances, trained vs untrained 8B, tested with **Cochran–Armitage
  trend** (or Mann–Whitney on level rank). Ordinal, one test, no
  multiple-comparison harvesting. Everything else is descriptive.
- **Pre-registered effect size:** a pass-rate claim requires **≥10 points
  absolute** improvement, or exceeding the untrained 32B's rate — not
  merely p<0.05. With a ~0 baseline, n=200 makes 4–5 successes
  "significant", which would reduce the claim to *any success at all*.
- **Stated honestly:** intervals are over the procedural generator's
  distribution, not over neutron instrument design. The unlimited-n
  advantage buys precision about a narrower population than "instrument
  design" implies.
- **T1 transfer check is load-bearing even though secondary:** a null
  transfer result gets stated prominently, not buried — otherwise the
  contribution reads as "training on X improves X".

## 5. SFT mechanics — traps named by the serving side

1. **Chat template identity.** Serving is non-thinking ONLY because the
   client sends `chat_template_kwargs {"enable_thinking": false}`
   per-request (vLLM 0.28 has no server flag). **Qwen3's tokenizer template
   defaults to THINKING**, so TRL would render thinking-formatted targets
   while we serve non-thinking. Render with `enable_thinking=False` and
   **assert the rendered string matches what the server produces for the
   same messages.**
2. **YaRN.** Both endpoints serve with YaRN factor 4.0 for the 98304
   benchmark context; training uses the stock config. Fix: **serve the
   trained model WITHOUT YaRN at `max-model-len ≤ 40960` for the procedural
   eval** — rollouts are ~6 short steps, nothing needs long context, and
   train/serve then match exactly.
3. **Mask loss on EVERY assistant turn** (multi-turn dialogues; the common
   bug masks only the first).
4. **All seven projections** (q,k,v,o,gate,up,down), LoRA r=32–64.
5. **MERGE the adapter for evaluation** — vLLM's `--max-lora-rank` defaults
   to 16 and merged weights remove adapter-runtime numerical differences
   from a number we intend to freeze.

## 6. Phases

| # | Phase | When | Cost | Gate |
|---|---|---|---|---|
| 0 | Baselines + vacuity + keep-rate on held-out procedural (8B, 32B) | Sep 12 | $0 | **hard, §3** |
| 1 | RAFT self-generated sampling (train split) | Sep 12–13 | **$0** | 20-episode probe first |
| 1b | *(only if needed)* frontier seed + filtered-vs-unfiltered ablation | Sep 13 | ~$25 | keep-rate evidence |
| 2 | LoRA SFT on GPU 7 (stop 32B server if more cards needed) | Sep 13–15 | $0 | install timebox |
| 3 | Eval: trained vs untrained 8B vs 32B **vs classical** | Sep 15–16 | $0 | — |
| 4 | Freeze + write | Sep 17 | — | — |

## 7. Gates (hard dates)

- **Sep 13 EOD** — phase-1 keep rate/spend outside bounds → stop before
  training (the discipline that saved $145 on gpt-5.2-pro).
- **Sep 14 EOD** — no training run started → abandon SFT, write
  env+eval-only.
- **Sep 16 EOD** — no trained checkpoint evaluated → freeze; abstract
  claims env+eval-only.
- **Sep 17** — numbers frozen, fresh-seed re-verified.
- **GRPO: NO** (reviewer agrees unreservedly).
- Install timebox 3 h is optimistic — vLLM failed three times on this host
  (no python3-venv, missing `Python.h`, ninja off PATH). Reusing
  `vllm-env2`'s managed Python removes the header trap; the abort stays.

## 8. What I need from you

1. **Approve the RAFT redesign** (§2) — self-generated, $0, isolates the
   reward. This replaces the $25–30 generation spend.
2. **Approve the narrowed framing** (§1) — environment-trainability, with
   the classical reference in the headline.
3. **Approve the phase-0 hard gate** (§3), including that we may stop and
   redefine the readout before spending anything.
4. **Approve the 10 held-out benchmark cells** (still outstanding, ~$0.05).
5. Note: no new endpoint needed; GPU 7 is free and sufficient for LoRA.

## 9. Running regardless

Figure 1 + paper skeleton (`paper/OUTLINE.md`, committed) — the abstract is
due in 6 days whatever M8 yields.


---

## 10. Decision point (2026-09-13) — the gate says stop; what now?

**Option A — accept env+eval-only, and report the negative result as a
methods finding.** *(my recommendation)*
Cost: 0 days. The paper keeps everything already earned — the environment,
271 leak-free episodes, the red-team section, the failure taxonomy, T2
unsolved by all — and ADDS a measured contribution that is genuinely
useful to anyone building executable environments: **dense-feedback
parametric optimization is not a discriminating training task.** A 4x
model-scale increase changes nothing, so the task cannot support a
trainability claim regardless of training method. Stating that with data
is worth more than a null SFT result nobody can interpret.

**Option B — spend ~1 day testing whether EFFICIENCY discriminates.**
Raise n to 100/condition (free, local) on steps-to-success and FOM
quality. If the 32B's 2.0-vs-3.5-step advantage holds up, SFT could target
convergence efficiency. Risk: even if it works, "the trained model
converges in fewer steps" is a much weaker claim than the standing bar,
and it is still the scipy-delegated task. Gate: Sep 14 EOD.

**Option C — pivot to the CONSTRUCTION axis, where scale does
discriminate** (M6: 8B 1/17 vs 32B 3/17, huge headroom). Honest
arithmetic: self-generated RAFT needs ~17 episodes per keeper at the 8B's
~6% success rate, so ~200 keepers means ~3400 MCP episodes at minutes
each — **50-280 hours. Not feasible before Sep 25.** Correct target for
the follow-up paper, not this one.

**Option D — redesign the reward feedback** (remove per-step FOM, forcing
physics reasoning over hill-climbing). Genuinely interesting, plausibly
restores discrimination — but it is an environment redesign 5 days before
the abstract with an unknown outcome, and everything measured so far would
need re-measuring against the new signal.

**What does not change under any option:** M6 is complete and defensible,
the environment ships, and the paper has a full evaluation story. The RL
track degrades along the pre-agreed ladder to env+eval-only, which was
always the stated fallback.
