# Scope evolution: environment-first framing + RL track

**Date:** 2026-07-24 · **Status: adopted with qualifications.** Digests external
feedback (Claude review via user). Supersedes the *framing* of
`scope-decision-2026-07-09.md` where they differ; the two research questions in
PLAN.md §Purpose stand — this adds a third and re-weights the artifact.

## The reframe

**The headline artifact is an environment, not a benchmark**: a fast,
physically-verifiable, inverse-design RL environment for LLM agents — neutron
optics is the substrate. Procedurally generated, densely rewarded, trainable.
The benchmark (frontier-model eval) falls out as a held-out slice.

Purpose set grows to three questions:
1. Reproduce from paper (held-out benchmark slice — unchanged)
2. Improve to target specs (becomes the *trainable core*, procedurally generated)
3. **Does physics-verifiable reward train?** Small-model delta: rejection
   sampling → filtered SFT → GRPO if signal; claim = 7B+training beats 7B
   baseline, approaching a larger untrained model.

## Adopted (and how it maps to what exists)

- **Three-tier reward ladder — we already built ~all of it without naming it:**
  static checks = registry validation + `validate_instrument` (free);
  cheap dynamic = low-ncount runs with staged diagnostics (sub-second to
  seconds); terminal = full-protocol runs graded by `benchmark/grader.py`
  (statistics-aware tolerances, no LLM judge). Env work = wrapping these as a
  reward API + shaping, not new machinery.
- **Tiered task speed, decided now:** fast tier (RL-trainable) = template
  families + truncated ncount; slow tier (eval-only) = full instruments.
- **Procedural instance generation** for target-spec tasks; T1
  paper-reproduction tasks are NOT procedurally generatable and stay as the
  curated held-out slice (see Qualifications).
- **Anti-reward-hacking**: multi-objective targets; held-out parameter
  regimes; Liouville/brilliance-transfer ≤ 1 as physics-native hack
  detection (already a skill rule; becomes an env check); ncount-truncation
  guard (reward computed at env-controlled protocol, never agent-chosen
  ncount); plus a **red-team-the-reward exercise reported in the paper** —
  fits our adversarial-review culture, cheap differentiation.
- **Rejection sampling before online RL** — de-risks the reward signal in
  weeks, independently publishable, no rollout/training co-location.
- **Sequencing with publishable artifacts at every stage**: env → frontier
  eval numbers (paper) → filtered SFT → GRPO. Env paper does not wait for RL.
- **Packaging**: pip/Docker one-command eval slice; McStas dependency pain is
  real (we hit ncrystal/ply ourselves) — solve for users.
- **RL recipe as recorded defaults** (phase-2 detail, not commitments):
  LoRA r=32–64 on all linear projections; critic-free (GRPO-class); Qwen-family
  7–8B base for legibility; existing agentic-RL framework (verify current
  tooling before committing); ~4/4 GPU split generation/training on 8×A100-40G;
  CPU-bound McStas rollouts don't compete with GPUs for silicon.

## Qualifications (our data / our stakes)

1. **Fast-tier feasibility is already half-verified, with one design
   consequence the feedback missed:** measured on this machine — compile ≈
   2–3 s, cached-binary parameter-only runs ≈ 0.3–1 s at ncount 1e4–1e5
   (M2 binary cache keys on comment-stripped .instr sha). Therefore the
   procedural generator must emit **parameterized template families**
   (compile once, sample thousands of parameter instances) for the RL fast
   tier; topology-varying instances pay the compile and live in the slow
   tier. Formal rollout-timing measurement is an M5 checklist item.
2. **Do not discard T1 paper-reproduction.** It is the scientifically
   distinctive, non-generatable content (verified paper↔model pairs,
   contamination evidence) and what makes the held-out slice interesting
   beyond RL plumbing. Environment-first framing *contains* the benchmark;
   it does not replace it.
3. **Venue logic updates, not inverts:** environment framing targets the
   same NeurIPS D&B / ICLR class but with an RL-infrastructure pitch;
   prior-art re-check immediately before writing is mandatory (this space
   moves monthly).
4. **Suite with autoMartiniAgent ("physics-verifiable reward" framework
   paper): parked as a strategic option** — depends on that project's state;
   not a plan commitment. Soft constraint adopted now: keep the env/reward
   API cleanly separable from McStas specifics so a common API remains cheap.
5. **Claim bar recorded verbatim**: the RL result is a delta validating the
   environment, not a frontier agent. 2–3 real GRPO runs max at this scale —
   one headline + ablations that reuse rollouts.

## Immediate open items (now in PLAN M5/M8)

- Empirical fast-tier rollout timing (template-family regime)
- Procedural generation schema for instrument configs
- Multi-objective target-spec format
- Agentic-RL framework selection test
- Prior-art re-check before positioning section
