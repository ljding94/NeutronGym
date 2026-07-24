# Pilot agent episodes (P1/P3 × 2 models) + fast-tier rollout timing

**Date:** 2026-07-24 · Completes the M5 pilot (agent episodes through the real
scaffold, graded end-to-end) and the fast-tier timing item the RL plan rests on.

## Pilot results: 4/4 passes on clean infrastructure

Runner: `benchmark/run_episode.py` — isolated episode (fresh MCSTAS_MCP_HOME,
scratch cwd, skill installed for Claude-default config), headless `claude -p`,
grading of the on-disk artifact re-run under the env-controlled protocol.

| Episode | Verdict | Checks | Turns | MCP calls | Cost | Notes |
|---|---|---|---|---|---|---|
| P1 × claude-fable-5 (+skill) | **PASS 1.0** | 6/6 | 24 | 19 | $2.49 | skill invoked; `validate_instrument` before running; intensity within 0.25% of hidden reference |
| P3 × claude-fable-5 (+skill) | **PASS 1.0** | 2/2 | 33 | 26 | $3.86 | assumptions disclosed (underspecified collimation); only specified observables graded |
| P1 × gemini-3.6-flash | **PASS 1.0** | 6/6 | 31 | 27 | $2.73 | reproduction so exact the same-seed observables are bit-identical to the reference |
| P3 × gemini-3.6-flash | **PASS 1.0** | 2/2 | 29 | 23 | $2.49 | assumptions disclosed; two earlier attempts failed on infrastructure (below) |

Grader verdicts required zero manual overrides; monitor role-matching handled
four different naming schemes without special-casing.

## Failure taxonomy entries (from the two invalid P3-gemini attempts)

1. **Provider stream drop** ("API Error: stream closed before completion",
   mid-episode after 8 healthy calls). Class: infrastructure, not capability.
   **M6 policy: 1 automatic retry on stream/API errors; retried episodes
   flagged; infra failures excluded from capability metrics.**
2. **Harness bug (ours, fixed + regression-covered):** a relative
   `--episode-dir` made MCSTAS_MCP_HOME relative; the MCP server (different
   cwd) scattered the registry and broke mcrun output dirs. Runner now
   abspaths the episode dir. Grading verdicts must always come from disk
   forensics — this bug produced "agent left no instrument" verdicts that
   transcript inspection immediately contradicted.

Observed behaviors worth keeping (small-n, informal): gemini (no skill
invocation available to it — it never called Skill even when installed... it
was not offered in its config) matched Claude on the fully-specified P1;
Claude's skill-following was visible (validate-before-run, seed discipline,
assumption disclosure). The rigorous ±skill comparison remains the M5/M6
ablation.

## Fast-tier timing: the mcrun wrapper is the bottleneck, not physics

`benchmark/measure_fast_tier.py` (template-family regime, cached binary):

| Path | Time/rollout |
|---|---|
| Template compile (one-time per family) | ~3.1 s |
| Via `run_spec` (rebuild + mcrun wrapper), ncount 1e3–1e5 | **~2.43 s flat** |
| Via `run_spec`, ncount 1e6 | ~2.84 s |
| **Direct binary exec** (`./name.out --ncount=1e5 --seed=… par=v`), validated output | **~0.04 s** |

The flat 2.4 s is fixed overhead (mcrun's Python wrapper startup + per-run
McStasScript rebuild) — ray tracing is nearly free at fast-tier ncount.

**Environment executor decision (M5):** fast-tier rollouts execute the
compiled binary directly — mcrun only compiles, once per template family.
Measured ~25 rollouts/s/core (~200/s on 8 cores) + ~1 ms mccode.sim parse:
environment throughput will never bound RL training; LLM generation will.
The agent-facing MCP path keeps the wrapper (2.4 s is fine interactively);
the env's `step()` uses the binary path.

## Cost anchor for M6 budgeting

Full pilot episode ≈ $2.5–3.9 (frontier/flash class, 24–33 turns). A
50-task × 3-model × pass@1 matrix lands ≈ $400–600 at these prices — above
the original $100–300 estimate; revisit task count / model mix / prompt
caching at M6 planning.
