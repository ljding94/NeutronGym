# Pilot episode leak audit: agents fetched the reference instrument (2026-07-30)

**Status: finding + fix design; implemented same day** (server benchmark
mode + episode Read deny rules + transcript audit; `tests/test_sandbox.py`,
demo: `scripts/sandbox_walkthrough.py`). Triggered by a design question ("do agents
need a sandbox so they can't reach the ground-truth `.instr`?") — a transcript
audit of all 7 pilot episodes answered it empirically: **yes; 4 of 7 episodes
accessed the reference instrument at test time** through legitimately-allowed
tools. Caveats the 2026-07-24 "pilot episodes 4/4 PASS" claim (mechanics
validation stands; capability signal in leaked episodes is void).

## Audit (all pilot transcripts, leak-class tool calls)

P1/P3/T1-pilot all reference `shipped:templateSANS`.

| Episode | Leak-class calls | Verdict |
|---|---|---|
| P1_sans_reproduce__claude | — | clean |
| P1_sans_reproduce__gemini-3.6-flash | `list_examples("sans")` → `get_example("templateSANS")` | **leaked** |
| P3_sans_underspecified__claude | `list_examples("sans")` → `get_example("templateSANS")` | **leaked** |
| P3_sans_underspecified__gemini (1st) | — | clean (infra-failed episode) |
| P3_…__gemini_retry | `get_example("templateSANS")` + **`load_instr_file(<conda examples path>/templateSANS.instr)`** | **leaked** (loaded the reference into its own registry) |
| P3_…__gemini_retry2 | — | clean |
| T1_templateSANS__claude | `get_example("templateSANS")` | **leaked** |

At least 2 of the 4 counted passes (plus the extra T1 episode) saw the
reference source mid-episode. No malice involved: consulting shipped examples
is normal McStas practice and nothing in the episode config forbade it.

## Root cause

`run_episode.py` grants `--allowedTools mcp__mcstas, Skill, Read`:

- `mcp__mcstas` includes `list_examples`/`get_example` (returns **full
  `.instr` source**) and `load_instr_file` (accepts any absolute path — the
  conda example library and `benchmark/instruments/` included).
- `Read` is unscoped (needed for skill reference files, but also reads the
  conda examples dir and `benchmark/refcache/` given a guessed path).

## What survives / what doesn't

- **Survives:** harness + grader mechanics validation (episode isolation,
  candidate discovery, protocol re-run, tolerance behavior — none depend on
  how the agent produced the instrument); T1 task self-validation, T2
  calibration, memorization probe (no agent episodes involved).
- **Void:** any capability reading of the leaked episodes. The clean episodes
  (P1__claude PASS, P3__gemini_retry2 PASS) remain genuine capability
  signal.

## Fix design (benchmark sandbox, three layers — M5 work item)

1. **Server-side benchmark mode** (`MCSTAS_MCP_BENCHMARK=1`, set by
   `run_episode.py`): `list_examples`/`get_example` disabled (clean tool-level
   error); `load_instr_file` refuses paths outside the episode's
   `MCSTAS_MCP_HOME`/cwd. Server-side because tool allowlists drift and
   OpenRouter models must face identical rules.
2. **Read scoping**: episode-local permission deny rules for the conda
   `share/mcstas/resources` tree, `benchmark/`, and `~/.mcstas-mcp` outside
   the episode home (skill files stay readable).
3. **Transcript audit as backstop** (grading-side, mandatory): `run_episode.py`
   post-scans the transcript for leak-class calls (example tools, out-of-tree
   `load_instr_file`/`Read` targets) and stamps `reference_leak` in
   `report.json`; a leaked episode is INVALID regardless of score. Belt and
   braces — the audit catches whatever future tool surface changes miss.

T2 is exempt where the baseline is task input by design (the agent is *given*
the baseline instrument); its exemption list is per-task, not global.

## Consequences

- Pilot claim restated everywhere as: **4/4 mechanics-validated; capability
  signal caveated (2/4 counted episodes leaked)**. Clean re-runs under the
  sandbox happen with the M6 matrix (pilot tasks are cheap; no separate rerun
  campaign needed).
- M6 protocol gains a standing rule: every scored episode must pass the leak
  audit (layer 3) — reported per-episode like the memorization probe.
- The M1-era design choice that put example browsing in the server was right
  for the *interactive* tool (it stays); benchmark mode is a restriction, not
  a removal.
