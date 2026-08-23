"""Neutron instrument arithmetic — do these with code, not in your head.

Usage (CLI):
    python resolution_calcs.py convert lam=5
    python resolution_calcs.py bragg d=3.355 lam=2.4
    python resolution_calcs.py chopper L=25 lam=5 nu=100 theta0=10
    python resolution_calcs.py frame L=25 nu=100
    python resolution_calcs.py guide m=2 lam=5
    python resolution_calcs.py sans lam=6 Ldet=5 rdet=0.32 rstop=0.02

All functions importable; stdlib only.
"""

import math
import sys

V_LAMBDA = 3956.034  # v[m/s] * lambda[AA]
E_LAMBDA = 81.804    # E[meV] * lambda[AA]^2


def lambda_to_velocity(lam):
    """AA -> m/s"""
    return V_LAMBDA / lam


def lambda_to_energy(lam):
    """AA -> meV"""
    return E_LAMBDA / lam**2


def energy_to_lambda(E):
    """meV -> AA"""
    return math.sqrt(E_LAMBDA / E)


def bragg_angle(d, lam, order=1):
    """Bragg angle theta [deg] for d-spacing d [AA]; take-off is 2*theta."""
    s = order * lam / (2 * d)
    if s > 1:
        raise ValueError(f"lambda {lam} AA unreachable with d={d} AA (order {order})")
    return math.degrees(math.asin(s))


def chopper_phase(L, lam, nu, t0=0.0):
    """Phase [deg] for a DiskChopper at distance L [m] to pass lambda [AA],
    for chopper frequency nu [Hz]; t0 = emission-time offset [s]."""
    t = L / lambda_to_velocity(lam) + t0
    return (360.0 * nu * t) % 360.0


def chopper_opening_time(theta0, nu):
    """Opening duration [s] of a slit of theta0 [deg] at nu [Hz]."""
    return theta0 / (360.0 * nu)


def chopper_dlambda(L, nu, theta0):
    """Wavelength spread FWHM [AA] a single chopper opening passes at
    distance L [m] (burst-time contribution only)."""
    return V_LAMBDA * chopper_opening_time(theta0, nu) / L


def frame_max_lambda(L, nu_rep):
    """Longest lambda [AA] that beats the next pulse over L [m] at source
    repetition nu_rep [Hz] (frame-overlap limit)."""
    return V_LAMBDA / (L * nu_rep)


def guide_critical_angle(m, lam):
    """Supermirror critical angle [deg] for coating m at lambda [AA]."""
    return m * 0.099 * lam


def guide_m_for_divergence(full_div_deg, lam):
    """Minimum m so the guide supports a beam of given FULL divergence [deg]
    at lambda [AA] (half-divergence must be < critical angle)."""
    return (full_div_deg / 2.0) / (0.099 * lam)


def sans_q(lam, two_theta_deg):
    """Q [1/AA] for scattering angle 2theta [deg] at lambda [AA]."""
    return 4 * math.pi / lam * math.sin(math.radians(two_theta_deg) / 2)


def sans_q_range(lam, Ldet, rdet, rstop):
    """(Qmin, Qmax) [1/AA] for detector at Ldet [m], detector half-size
    rdet [m], beamstop radius rstop [m]."""
    return (sans_q(lam, math.degrees(math.atan2(rstop, Ldet))),
            sans_q(lam, math.degrees(math.atan2(rdet, Ldet))))


def quadrature(*terms):
    """Combine independent resolution contributions."""
    return math.sqrt(sum(t * t for t in terms))


def tof_dE_over_E(dt_over_t):
    """Relative energy resolution from relative timing width: dE/E = 2 dt/t."""
    return 2.0 * dt_over_t


def _cli():
    if len(sys.argv) < 2:
        print(__doc__)
        return
    mode, kw = sys.argv[1], dict(a.split("=") for a in sys.argv[2:])
    g = {k: float(v) for k, v in kw.items()}
    if mode == "convert":
        lam = g.get("lam") or energy_to_lambda(g["E"])
        print(f"lambda = {lam:.4g} AA   E = {lambda_to_energy(lam):.4g} meV   "
              f"v = {lambda_to_velocity(lam):.5g} m/s   k = {2*math.pi/lam:.4g} 1/AA")
    elif mode == "bragg":
        th = bragg_angle(g["d"], g["lam"], int(g.get("order", 1)))
        print(f"theta = {th:.4f} deg   take-off 2theta = {2*th:.4f} deg")
    elif mode == "chopper":
        ph = chopper_phase(g["L"], g["lam"], g["nu"], g.get("t0", 0.0))
        line = f"phase = {ph:.3f} deg   ToF(L) = {g['L']/lambda_to_velocity(g['lam'])*1e3:.4g} ms"
        if "theta0" in g:
            dt = chopper_opening_time(g["theta0"], g["nu"])
            line += (f"   opening = {dt*1e6:.4g} us   "
                     f"dlambda(burst) = {chopper_dlambda(g['L'], g['nu'], g['theta0']):.4g} AA")
        print(line)
    elif mode == "frame":
        print(f"frame-overlap lambda_max = {frame_max_lambda(g['L'], g['nu']):.4g} AA")
    elif mode == "guide":
        print(f"theta_c = {guide_critical_angle(g['m'], g['lam']):.4g} deg   "
              f"(full divergence supported ~ {2*guide_critical_angle(g['m'], g['lam']):.4g} deg)")
    elif mode == "sans":
        qmin, qmax = sans_q_range(g["lam"], g["Ldet"], g["rdet"], g["rstop"])
        print(f"Qmin = {qmin:.4g} 1/AA   Qmax = {qmax:.4g} 1/AA")
    else:
        print(__doc__)


if __name__ == "__main__":
    _cli()
