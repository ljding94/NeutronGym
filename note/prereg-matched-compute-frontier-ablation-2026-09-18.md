# Pre-registration: matched-compute classical baseline, frontier models, reward ablation (2026-09-18)

Written before any of these runs. Decision rules are fixed here so the results
cannot be reinterpreted after the fact. All three use `guide_match`, pass =
spot size AND divergence within ±5% of the instance's targets, temperature 0
for measurement arms, env-controlled protocol (ncount 1e5, per-instance seed).

## 1. Matched-compute classical baseline (CPU)

**Question.** The trained model gets 10 simulations per episode. What does a
classical optimizer get with the same budget, on the same instances?

**Instances.** Held-out 300–599 (the fresh slice, n=300) — the slice the
reported model numbers use.

**Budget.** Exactly 10 terminal simulations per instance, the same as the
agent's 10 turns. Reported alongside a 30-simulation arm as a sensitivity
check (3x budget), which is NOT the headline comparison.

**Optimizers** (all given the true parametrization, which the agent is not):
- **random search** — 10 uniform draws inside the bounds;
- **Nelder–Mead** — scipy, from the family baseline, capped at the budget;
- **coordinate descent** — the pattern search used by calibration, capped.

Each minimises the same objective the environment grades: the worst relative
error across the two targets. An optimizer "passes" an instance if any design
it evaluated within budget is a pass. Every simulation is counted against the
budget, including the starting point.

**Decision rule, fixed now.**
- If the best classical arm at budget 10 passes **< 69.0%** (the weaker GRPO
  seed), report "GRPO beats hand-coded physics AND classical search at matched
  budget".
- If it passes **≥ 69.0%**, report "at matched budget classical search is
  competitive with or better than the trained model; what RL buys is reaching
  that level from natural language without being given the parametrization".
- Either way the table carries all three optimizers, both budgets, and the
  paired McNemar against the seed-1 model.

**Threat to validity to state:** the optimizers act on numeric parameters
directly, while the agent must also parse the task and emit valid JSON; the
comparison is of search efficiency per simulation, not of equivalent
interfaces.

## 2. Frontier models (OpenRouter)

**Instances.** The first 100 of the fresh slice (held-out 300–399), same
10-turn loop, temperature 0, identical prompts and feedback.

**Models and pins** (pins per `benchmark/m6_config.json`, no fallbacks):
- `anthropic/claude-sonnet-5` (pin Google) — frontier tier;
- `google/gemini-3.6-flash` (pin Google) — mid tier;
- `openai/gpt-5.2-pro` (pin OpenAI) — frontier, **25 instances only**, run
  last and only if the first two cost less than $15 combined.

**Budget.** Hard stop at **$30** for this experiment; abort any model whose
running cost exceeds $15. Estimated: ~$5 sonnet-5, ~$2 gemini-3.6-flash,
~$13 gpt-5.2-pro on its 25-instance slice.

**Decision rule, fixed now.**
- If the best frontier model passes **< 50%** on its 100 instances, report "a
  GRPO-trained 8B outperforms frontier models on this task".
- If it passes **≥ 50%**, report "the environment discriminates model quality;
  the trained 8B reaches frontier-level performance on this family".
- If a model errors on > 10% of episodes, its row is reported as infra-limited
  and excluded from the comparison (the M6 taxonomy lesson: infra failures are
  not capability zeros).

**Comparison caveat:** the 8B numbers are on 300 instances, the frontier rows
on 100 (25 for gpt-5.2-pro); confidence intervals are reported per row and the
8B number is recomputed on the same 100-instance sub-slice for the paired
comparison.

## 3. Sparse-versus-ladder reward ablation (GPU)

**Question.** Does the level-resolved reward ladder matter for training, or
would pass/fail alone do?

**Setup.** Identical to the reported recipe in every respect — same states
file (seed-1 states), same schedule (120 steps at lr 1e-5, then 100 at 3e-5),
same seed — except the reward: **1.0 for an L4 pass, 0.0 otherwise**, with no
partial credit for validity, statistics or proximity to target.

**Evaluation.** Fresh slice 300–599, n=300, same protocol.

**Decision rule, fixed now.**
- Ladder − sparse **≥ 10 points**: report the ladder as load-bearing for
  training, with the gap as the evidence.
- Gap **< 10 points in either direction**: report that the ladder is not
  required for training on this family, and defend it only as a measurement
  and diagnosis tool (which is how the paper already frames it).
- Sparse **better by ≥ 10 points**: report that honestly as a negative result
  for the shaping.

**Expected failure mode worth watching:** with sparse reward most groups have
zero reward spread (the untrained model passes ~1% of sampled actions), so the
run may produce few usable gradients. The count of groups with signal per step
is logged and will be reported either way.
