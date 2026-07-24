# Study: T1 curation — machine verification + paper/pair research (2026-07-24)

Two parallel tracks: local machine-verification of shipped candidates
(`benchmark/build_inventory.py` → `benchmark/inventory.json`) and web research
(OA status, new pairs, held-out re-check; DOIs verified via Crossref/Unpaywall/
Semantic Scholar/Zenodo/GitHub APIs this session).

## A. Machine-verified reference instruments (this env, ncount 1e5, seed 1)

**30/35 candidates runnable; 25 verified against their `%Example` ground-truth
values** (all agreeing). Runtimes 4–13 s at 1e5. Excluded with recorded triage:

| Instrument | Status | Cause |
|---|---|---|
| RITA-II | physics-params | TAS defaults (EI=EF=0) can't close the scattering triangle — needs valid (EI,EF,Q) from the paper |
| SNS_ARCS | missing-data | needs SNS source file `source_sct521_bu_17_1.dat` (fetchable from MCViNE) |
| ISIS_TOSCA_preupgrade | run failure | needs per-instrument triage |
| ISIS_CRISP, ISIS_IMAT | env-incompatible | contrib components fail clang/arm64 compile |

Sweep lesson (now in the script): instruments without `%Example` lines must be
run with their DEFINE defaults passed explicitly (zero CLI params = the
interactive-prompt hang).

## B. Open-access status of anchor papers (task-input consequence)

Only **3 of 12** anchor papers have legally usable full-text links:
ARCS (Caltech repository), SESANS (IUCr gold OA), IMAT (green, ORO — verify by
hand). PAYWALLED: BASIS, PSI_DMC-validation, IN5, LET, SANS2d, D11, IN13,
FOCUS, RITA-II. **T1 task-input policy: use the paper PDF only for OA anchors;
for paywalled anchors the task input is a spec sheet derived from the paper +
facility pages (which doubles as the pipeline-decomposition control variant).**

## C. New (paper, model) pairs beyond shipped examples (best first)

| Instrument | Paper access | Model source | Quality note |
|---|---|---|---|
| **CNCS** (SNS) | OA arXiv:1109.1482 + 1609.00348 | mcvine/resources `CNCS-2020.instr` (+2 dated versions) | ORNL-maintained, real |
| **MARI** (ISIS) | OA (NIM A 1056, 168646) | mducle/eniius + mducle/mcstas_horace (author-written); Zenodo 10.5281/zenodo.8314687 CC-BY | strong |
| **MAPS** (ISIS) | OA arXiv:1812.08583 | mducle/eniius, mcstas_horace | real |
| **BIFROST** (ESS) | OA QuBS 9(1) 5 (2025) MDPI | KristineKrighaar/BIFROST_Virtual_Experiments (26 .instr!) + g5t/niess | active 2026 |
| **CSPEC** (ESS) | OA arXiv:2105.05552 | Harry-Rich/trex_reduction | McStasScript-generated 2026 — quality-eval first |
| **T-REX** (ESS) | suite paper | nicolai3008/Njord_Remora (hand-written 2025) | detailed |
| **PANDA** (MLZ) | OA gold JLSRF 1 A12 | ammerritt-gh vPANDA (Jul 2026, GPL-3) | quality-eval first |
| **HEIMDAL** (ESS) | paywalled | SINE2020WP3_instruments (GPL-2) | real |
| **MERLIN** (ISIS) | check | mducle/eniius | reinforces shipped pair |
| **MAGiC** (ESS) | not pinned | SINE2020WP3 front+back | real |
| PSI SANS-I optics | OA PRSA 2025 + Zenodo CC-BY | zenodo 15183615 | T2-style optics task |
| CSNS GPPD | — | mccode-dev/Schools | **burns GPPD as held-out** |

Watch item: **mccode-dev/PaNRAID** (created 2026-03; DIADEM course Sept 2026,
Willendrup/Farhi/Robledo) — the McStas core team building AI datasets from
digital twins. Track for related work + contamination.

## D. Held-out re-check (2026-07-24)

8/9 clean and unchanged (BOYA — now with OA preprint arXiv:2501.01143 —
CSNS DGS/EMD, PIONEER, MUSHROOM, HBS, POLANO, VENUS). **PIK now caveated**:
39 "PIK" code hits — public PNPI guide-system models (karpenka/*, 2016–19) —
suite instruments themselves still absent, but check overlap with
arXiv:2412.00223 before finalizing, and treat GitHub code search as
non-deterministic (previous 0-hit result did not reproduce).

**Contamination-rot policy:** 2026 is seeing rapid model publication (T-REX
and CSPEC models Mar–Apr 2026, vPANDA Jul 2026, LET/T-REX Zenodo datasets
Jun–Jul 2026, PaNRAID Sept 2026). **Re-run the full contamination sweep
immediately before freezing the benchmark split**; ESS/ISIS-spectrometer
instruments decay fastest. GitHub search covers default branches only; the
three 7z Zenodo archives were not unpacked (verify before relying on them);
facility GitLabs remain unsearched.

## Combined T1 candidate pool after this study

~30 machine-verified shipped instruments + ~10 vetted-quality external pairs
≈ **40 candidates for the 20–30-task selection**, with per-candidate: runnable
status, ground-truth mechanism (%Example or protocol runs), paper access
class (OA/spec-sheet), and contamination status.
