# Red-teaming the reward: five findings, four fixes, one bound (2026-08-05)

**Status: consolidated record — raw material for the paper's
red-team-the-reward section.** Every scored signal in NeutronGym was
attacked before any agent number counted. Five real findings, each caught
by our own process before contaminating results:

## The findings

1. **Beamstop-leakage hack (2026-07-24, T2 calibration).** Unconstrained
   flux maximization found 672 n/s of *direct beam* — max-only resolution
   constraints have the wrong sign for leakage (it *shrinks* the apparent
   width). Fix: **pattern-integrity BAND constraints** (min AND max vs the
   baseline pattern). Bands are now the L3 structural check in the env
   families too.
2. **Optimizer bounds escape (2026-07-24, T2 calibration).** `mcrun
   --optimize` nelder-mead ignores parameter bounds (escaped to w=2.1 m).
   Fix: classical baselines are **bounds+constraint-filtered ensembles**,
   never a raw optimizer output.
3. **Winner's curse (2026-07-24, T2 calibration).** The best of 30 noisy
   evaluations is biased upward — the selected "best" fell below its own
   target at a fresh seed and the guard honestly rejected our original
   guide task. Fix: **fresh-seed re-verification of any selected best**;
   the discipline now also applies to all RL numbers (frozen ~Sep 10).
4. **Monitor-matching decoy (2026-08-05, reference-loop shakedown).**
   Role-matching by most-events graded an agent's *pre-sample diagnostic
   monitor* (which the skill tells agents to add!) against the reference's
   scattering detector: two different frontier models "failed" P1 3/6 with
   identical 1500×-hot signatures despite building correct beamstopped
   instruments. Fix: **position-aware matching** (nearest to the reference
   monitor); both episodes regrade to PASS 6/6. Found *by the reference
   loop*, not by curation — the measurement instrument audits the grader.
5. **Statistical false-positive in our own new check (2026-08-05, this
   sweep).** The first Liouville-gate sweep flagged a legitimate action at
   "1.14× the bound" — on a monitor with ~250 events, i.e. noise; at 10×
   rays it collapsed to 0.99. Fix: **the Liouville gate only evaluates
   above the statistics floor** (the same floor discipline the grader has
   always enforced). We red-teamed the red-team.

## The Liouville/brilliance bound (shipped 2026-08-05)

`src/neutrongym/hacks.py`, wired into reward L3 as `unphysical_gain`:
passive optics cannot beat source brightness, so FOM intensity is bounded
by `flux × A_monitor[cm²] × Ω_monitor[sr] × Δλ[Å]` — **no reference
needed**, which makes it safe for the trainable tier (it cannot be gamed
by matching anything).

- **guide_divergence: SHARP.** Calibrated at 1e6 rays: the best physical
  configs reach 0.93–0.99× the analytic bound — the family's FOM plateau
  IS the Liouville limit, so `utilization` (reported in every episode
  record) measures how close a design sits to the physical optimum.
- **sans_collimation: LOOSE (conservation).** Scattering redistributes
  into 4π, so the pinhole-chain conservation bound sits well above real
  signals; it catches gross weight-multiplication hacks, the bands do the
  fine-grained work. Documented honestly; a sharper scattering bound is
  possible with a transmission monitor if ever needed.
- Margin 1.10 over the analytic bound (Source_simple's weighting
  approximation + protocol-level statistics; legit ceiling measured 1.016).

## Verification (`scripts/redteam_sweep.py`, runs/redteam/sweep.json)

56 corner/boundary/random-extreme actions through the full ladder on real
physics: 0 false unphysical flags; max legit utilization 1.016; 16
constraint-band catches; statistics floor catches starved corners first.
A fabricated 100×-bound summary fails L3 with `unphysical_gain`
(regression: `tests/test_reward.py::test_unphysical_gain_fails_l3`).

## Residual risks (open-eyes list)

- T3 open design has no reference AND free topology — degenerate-config
  space is far larger than the parametric tier's; the automatic floors +
  Liouville bound apply, but T3 stays outside the scored set until the
  expert rubric exists (standing decision).
- The SANS bound's looseness means a factor-few unphysical gain there
  would pass Liouville and must be caught by bands/floor.
- Bounds are per-FOM-monitor; a future family whose FOM is not an
  intensity needs its own bound derivation (BOUNDS registry is per-family
  by design; unknown families pass with kind "none" — add a bound when
  adding a family).
