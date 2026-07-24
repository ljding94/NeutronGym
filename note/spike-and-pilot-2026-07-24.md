# Gate 3 (OpenRouter spike) + Gate 4 (pilot rubric) — both passed 2026-07-24

## Gate 3: non-Claude tool-calling fidelity through the real scaffold

Setup: headless `claude -p` + `ANTHROPIC_BASE_URL=https://openrouter.ai/api` +
`ANTHROPIC_AUTH_TOKEN=$OPENROUTER_API_KEY`, per-episode `MCSTAS_MCP_HOME`,
scratch cwd (no CLAUDE.md/skill leakage), `--allowedTools "mcp__mcstas"`,
`--strict-mcp-config`. Runner: `scripts/spike_openrouter.py` (verdicts from
transcript + disk, never from model claims).

| Model | MCP calls | Tool errors | Instrument + job on disk | Flux reported | Cost |
|---|---|---|---|---|---|
| google/gemini-3.6-flash | 9 (5 distinct) | 0 | ✓ / ✓ | ✓ I ± err, events, beam stats | $0.54 |
| google/gemini-3.5-flash-lite | 12 (6 distinct) | 0 | ✓ / ✓ | ✓ I ± err, events | $0.77 |

**Verdict: gate 3 passed** — zero tool-format failures on either model; both
completed the M1 acceptance task end-to-end through the identical scaffold.
The multi-model premise stands.

### Findings that shape M6

1. **OpenRouter account privacy policy gates the model matrix.** Under the
   current settings only Google models route ("No endpoints available
   matching your guardrail restrictions"): gpt-5.5, qwen, deepseek, kimi,
   glm, grok, mistral all blocked. Fix before M6: widen the toggle at
   openrouter.ai/settings/privacy, then re-probe (probe loop is in this
   spike script).
2. **The stream-json `result` field can be empty for non-Claude models** —
   the real final answer is the last assistant text block. The M6 harness
   must extract it that way (spike script now does).
3. Early instant failures ("model may not exist") were the policy block
   surfacing through Claude Code's error mapping — not a scaffold problem.
4. Models legitimately skip `get_results` when `run_simulation`'s return
   already carries detector totals — graders must not require specific tool
   sequences, only outcomes.
5. Cost anchor: ~$0.5–0.8 per simple episode on flash-class models.

## Gate 4: pilot tasks + grading rubric (mechanics)

Pilot assets: `benchmark/grader.py` (observable-based, no LLM judge;
role-matched monitors — agents pick their own names; statistics-aware
tolerances `max(rtol·ref, nσ·(err_ref+err_cand))`; hard-failure gating),
`benchmark/probe_memorization.py`, tasks P1 (SANS reproduce, full spec),
P2 (memorization probe, PSI_DMC), P3 (SANS underspecified — grades ONLY
specified observables).

Validation (in `tests/test_benchmark.py`, all passing):

- **Reference passes at a fresh seed** — tolerances absorb pure statistics.
- **Deliberately-wrong candidates fail with localized blame**: λ=8 → the
  5.5–6.5 Å monitor empties (statistics-floor hard failure) + 2d pattern
  shifts; R=30 Å spheres → 2d intensity/width checks fail.
- **Underspecification is not punished**: P3 grades only the specified
  λ-band observables; a different legitimate collimation still passes; 2d
  intensity is explicitly ungraded.
- **Memorization probe discriminates**, live-tested: feeding the real file
  scores similarity ≈ 1.0 → CONTAMINATED; gemini-3.5-flash-lite from memory
  scores 0.07/0.06 → unseen (correct: flash-lite has not memorized PSI_DMC).

**Verdict: gate 4 passed on mechanics** — the rubric separates right from
wrong from underspecified from memorized. Remaining M5 work is scale, not
mechanism: agent episodes on P1/P3 through the headless scaffold, then the
20–30-task inventory.
