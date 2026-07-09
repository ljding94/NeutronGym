# Study: instrument papers ↔ .instr files (benchmark test cases)

**Date:** 2026-07-09 · Web research for (paper, `.instr`) pairs and held-out candidates. All DOIs verified this session via Crossref/DataCite/PubMed/publisher pages/GitHub API — none from model memory. GitHub contamination evidence: `gh api search/code` over all public GitHub + full recursive tree of `mccode-dev/McCode` (509 .instr files) on 2026-07-09. Complements the in-file citation table in `study-mcstas-software-2026-07-09.md` §3.

## Headline findings

1. **~17 verified paper↔model pairs** exist among shipped/public models — enough for the T1 "seen" tier. Strongest pairs (paper cited in the .instr header itself): **SNS_BASIS, SNS_ARCS, PSI_DMC, ILL_IN6**.
2. **9 held-out candidates** (2024–26 papers, no public .instr on GitHub) identified with per-instrument contamination evidence — enough for the held-out tier.
3. **Novelty confirmed**: no published 2024–2026 work applies LLMs to McStas or neutron-instrument simulation reproduction. Nearest: VISION (X-ray beamline assistant), LLM microscopy agents, non-LLM ML McStas tooling.
4. **McStasScript has no peer-reviewed paper** — cite GitHub + Zenodo DOI 10.5281/zenodo.6560751.

## 1. Paper ↔ model pairs (shipped or public)

| Instrument | Facility / Class | Primary paper (verified DOI) | Public McStas model |
|---|---|---|---|
| IN5 | ILL / cold ToF spectrometer | Ollivier & Mutka, J. Phys. Soc. Jpn. 80, SB003 (2011), [10.1143/JPSJS.80SB.SB003](https://doi.org/10.1143/JPSJS.80SB.SB003); also Neutron News 21(2) (2010), 10.1080/10448631003757573 | **Shipped**: ILL/ILL_IN5, ILL_IN5_Spots, ILL_H16_IN5, Mantid variants; ESS/ESS_IN5_reprate |
| IN6 | ILL / cold time-focusing ToF | Blanc, ILL Report 83BL21G (1983); Scherm et al., ILL Report 76S235 (1976) — no DOI (facility reports, cited in .instr header) | **Shipped**: ILL/ILL_IN6, ILL_H15_IN6 |
| D11 | ILL / pinhole SANS | Lindner & Schweins, Neutron News 21(2) (2010), [10.1080/10448631003697985](https://doi.org/10.1080/10448631003697985); design: Lieutenant et al., J. Appl. Cryst. 40 (2007), 10.1107/S0021889807038253 | **Shipped**: ILL/ILL_H15_D11 (full H15 guide) |
| D22 | ILL / SANS | No standalone primary paper found (ILL web pages are the standard citation). Sister D33: Dewhurst, Meas. Sci. Technol. 19, 034007 (2008) | **Shipped**: ILL/ILL_H512_D22 (D33: ILL_H142_D33) |
| IN13 | ILL / thermal backscattering | Natali et al., Neutron News 19(4) (2008), [10.1080/10448630802474083](https://doi.org/10.1080/10448630802474083) | **Shipped**: ILL/ILL_IN13 |
| IN16 | ILL / cold backscattering | Frick & Gonzalez, Physica B 301 (2001), [10.1016/S0921-4526(01)00492-6](https://doi.org/10.1016/S0921-4526(01)00492-6) | **None public** (only guide models ending at IN16 position) |
| FOCUS | PSI / hybrid ToF spectrometer | Janssen et al., Physica B 276–278, 89 (2000), [10.1016/S0921-4526(99)01253-3](https://doi.org/10.1016/S0921-4526(99)01253-3) | **Shipped**: PSI/PSI_Focus |
| DMC | PSI / powder diffractometer | Schefer et al., NIM A 288, 477 (1990), [10.1016/0168-9002(90)90141-R](https://doi.org/10.1016/0168-9002(90)90141-R); validation: Willendrup et al., Physica B 385–386, 1032 (2006), 10.1016/j.physb.2006.05.329 | **Shipped**: PSI/PSI_DMC, PSI_DMC_simple |
| LET | ISIS / cold multi-chopper spectrometer | Bewley, Taylor, Bennington, NIM A 637, 128 (2011), [10.1016/j.nima.2011.01.173](https://doi.org/10.1016/j.nima.2011.01.173) | **Shipped**: ISIS/ISIS_LET (updated model Sept 2025) |
| SANS2d | ISIS / ToF SANS | Heenan et al., Neutron News 22(2) (2011), [10.1080/10448632.2011.569531](https://doi.org/10.1080/10448632.2011.569531) | **Shipped**: ISIS/ISIS_SANS2d (written by Heenan) |
| BASIS | SNS / backscattering | Mamontov & Herwig, Rev. Sci. Instrum. 82, 085109 (2011), [10.1063/1.3626214](https://doi.org/10.1063/1.3626214) — cited in header | **Shipped**: SNS/SNS_BASIS |
| ARCS | SNS / wide-angle chopper spectrometer | Abernathy et al., Rev. Sci. Instrum. 83, 015114 (2012), [10.1063/1.3680104](https://doi.org/10.1063/1.3680104) — cited in header | **Shipped**: SNS/SNS_ARCS |
| SEQUOIA | SNS / fine-resolution chopper spectrometer | Granroth et al., J. Phys. Conf. Ser. 251, 012058 (2010), [10.1088/1742-6596/251/1/012058](https://doi.org/10.1088/1742-6596/251/1/012058) | Public (not shipped): mcvine/resources-original, granrothge/mcstas |
| LOKI | ESS / broadband SANS | Jackson & Kanaki, Zenodo (2013), [10.5281/zenodo.13302](https://doi.org/10.5281/zenodo.13302); ESS suite: Andersen et al., NIM A 957, 163402 (2020) | Public: ess-dmsc-dram/LOKI_Simulation_Environment, mccode-dev/SINE2020WP3_instruments |
| BIFROST | ESS / indirect CAMEA spectrometer | Toft-Petersen et al., Rev. Sci. Instrum. 96, 043904 (2025), [10.1063/5.0258847](https://doi.org/10.1063/5.0258847); McStas study: Klausz et al., J. Appl. Cryst. 54 (2021), 10.1107/S1600576720016192 | Public: mccode-dev/SINE2020WP8 (full chain), parked ESS_BIFROST_shielding |
| ODIN | ESS / imaging | Strobl, Phys. Procedia 69, 18 (2015), [10.1016/j.phpro.2015.07.002](https://doi.org/10.1016/j.phpro.2015.07.002); choppers: Schmakat et al., NIM A 979 (2020) | Public: SINE2020WP3 (Sword_ODIN, ODIN-v23) |
| DREAM | ESS / bispectral powder diffractometer | Schweika et al., J. Phys. Conf. Ser. 746, 012013 (2016), [10.1088/1742-6596/746/1/012013](https://doi.org/10.1088/1742-6596/746/1/012013) | **None public** (searched) |
| MIRACLES | ESS / ToF backscattering | Tsapatsaris et al., Rev. Sci. Instrum. 87 (2016), [10.1063/1.4961569](https://doi.org/10.1063/1.4961569) | Public: ess-dmsc/instrument-udp-readout |
| TOFTOF | MLZ / cold multi-chopper spectrometer | Unruh, Neuhaus, Petry, NIM A 580, 1414 (2007), [10.1016/j.nima.2007.07.015](https://doi.org/10.1016/j.nima.2007.07.015) | **None public** |
| NG7 SANS | NIST / 30 m SANS | Glinka et al., J. Appl. Cryst. 31, 430 (1998), [10.1107/S0021889897017020](https://doi.org/10.1107/S0021889897017020) | **None public** |
| CANDOR | NIST / energy-analyzing reflectometer | Maliszewskyj et al., NIM A 907, 10 (2018), [10.1016/j.nima.2018.05.023](https://doi.org/10.1016/j.nima.2018.05.023) | **None public** |

Additional shipped real-instrument examples not individually verified this session: HZB_FLEX, HZB_NEAT, ILL_D2B/D4/IN4/IN8/IN12/IN20/IN22, FZJ KWS-2, ISIS HET/MERLIN/OSIRIS/CRISP/IMAT/TOSCA.

Note: instruments with papers but NO public model (IN16, TOFTOF, NG7, CANDOR, DREAM) are natural **T3 open-design** or additional held-out reproduction tasks.

## 2. Held-out candidates (2024–26 papers, no public .instr)

Evidence: `gh api search/code?q=<NAME>+extension:instr` (public GitHub, default branches) + grep of the 509-file McCode tree, run 2026-07-09.

| # | Instrument / Facility | Class | Citation (verified) | Evidence |
|---|---|---|---|---|
| 1 | **BOYA**, CARR (China) | Multiplexing cold spectrometer | Wang et al., RSI 96, 073902 (2025), [10.1063/5.0256044](https://doi.org/10.1063/5.0256044) | 0 GitHub hits |
| 2 | **Cold DGS**, CSNS-II | Direct-geometry inelastic | Zhao et al., RSI 97, 053303 (2026), [10.1063/5.0321019](https://doi.org/10.1063/5.0321019) | 2 hits, both unrelated |
| 3 | **EMD**, CSNS | Engineering diffractometer | Zhou et al., NIM A 1063, 169246 (2024), [10.1016/j.nima.2024.169246](https://doi.org/10.1016/j.nima.2024.169246); commissioning JINST 20, P04029 (2025) | 1 hit, unrelated |
| 4 | **PIONEER**, SNS STS | Polarized single-crystal diffractometer | Liu & Torres, RSI 96, 033904 (2025), [10.1063/5.0259079](https://doi.org/10.1063/5.0259079); arXiv:2503.10938 | 0 hits |
| 5 | **Indirect crystal ToF (MUSHROOM-type)**, FRM II | Inverse-geometry ToF | Tang, Herb, Voigt, Georgii, JINST 19, P11001 (2024), [10.1088/1748-0221/19/11/p11001](https://doi.org/10.1088/1748-0221/19/11/p11001) | 0 hits |
| 6 | **Macromolecular diffractometer**, Jülich HBS | Compact-source + SELENE optics | Ma, Lieutenant, Voigt, Schrader, Gutberlet, RSI 95, 065104 (2024), [10.1063/5.0203509](https://doi.org/10.1063/5.0203509) | only 2 generic HBS-moderator tests |
| 7 | **POLANO**, J-PARC (upgrade) | Polarized chopper spectrometer | Ino et al., JPS Conf. Proc. 45, 011008 (2026), [10.7566/jpscp.45.011008](https://doi.org/10.7566/jpscp.45.011008) | 0 hits |
| 8 | **PIK suite** (IN2/IN3/SEM/DEDM/TENZOR), PNPI | Reactor instruments, polarizing optics | arXiv:[2412.00223](https://arxiv.org/abs/2412.00223) (2024) | 0 hits |
| 9 | **VENUS**, SNS BL-10 (weaker) | ToF imaging | Popova, Gallmeier, Bilheux, Hanks, Nucl. Eng. Des. 428, 113542 (2024), [10.1016/j.nucengdes.2024.113542](https://doi.org/10.1016/j.nucengdes.2024.113542) | 1 hit, unrelated; paper is shielding-focused |

**Caveats**: GitHub code search covers only default branches of public repos; models may live on facility GitLabs (ESS/PSI/ILL). Re-check `git.esss.dk` before finalizing any ESS-adjacent held-out task. Excluded (public models found): BIFROST, LOKI, ODIN, MAGiC, MIRACLES, BEER.

## 3. Core methodology references (verified)

| Reference | Citation |
|---|---|
| McStas original | Lefmann & Nielsen, Neutron News 10(3) (1999), [10.1080/10448639908233684](https://doi.org/10.1080/10448639908233684) |
| McStas (i) | Willendrup & Lefmann, J. Neutron Res. 22(1), 1–16, [10.3233/JNR-190108](https://doi.org/10.3233/JNR-190108) |
| McStas (ii) | Willendrup & Lefmann, J. Neutron Res. 23(1) (2021), [10.3233/JNR-200186](https://doi.org/10.3233/JNR-200186) |
| McStasScript | **No peer-reviewed paper.** Cite GitHub PaNOSC-ViNYL/McStasScript + Zenodo [10.5281/zenodo.6560751](https://doi.org/10.5281/zenodo.6560751) (Bertelsen) |
| guide_bot | Bertelsen & Lefmann, NIM A (2017), [10.1016/j.nima.2017.06.012](https://doi.org/10.1016/j.nima.2017.06.012) |
| ESS instrument suite | Andersen et al., NIM A 957, 163402 (2020), [10.1016/j.nima.2020.163402](https://doi.org/10.1016/j.nima.2020.163402) |
| McStas+ML SANS (2025) | Robledo, Lieutenant, Willendrup, arXiv:[2501.06054](https://arxiv.org/abs/2501.06054) |
| mcstas_gisans (2026) | Klausz et al., J. Appl. Cryst. 59, 827 (2026), [10.1107/S160057672600213X](https://doi.org/10.1107/S160057672600213X) |
| VISION beamline assistant (2025) | arXiv:[2412.18161](https://arxiv.org/abs/2412.18161); MLST (2025), [10.1088/2632-2153/add9e4](https://doi.org/10.1088/2632-2153/add9e4) — X-ray, nearest LLM-agent related work |
| LLM microscopy agents (2025) | arXiv:[2501.10385](https://arxiv.org/abs/2501.10385) |

**Gap finding for the paper**: no published 2024–2026 work applying LLMs to McStas instrument-file generation or neutron simulation reproduction (searched arXiv, Google, OSTI). Supports the novelty claim.

Unverified/no-DOI items flagged: Lefmann & Nielsen page numbers; Heenan ICANS-XVII (2006); ILL IN6 internal reports.
