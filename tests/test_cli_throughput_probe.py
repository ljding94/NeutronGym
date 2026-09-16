"""The throughput probe must run at the family's own protocol.

ncount became per family on 2026-09-15 (SANS 8e5, guide 1e5). The probe
hardcoded 1e5, so it reported SANS throughput 8x too optimistically and
printed "at 1e5" regardless of what it had actually run.
"""

from neutrongym import cli, generate


class _Recorder:
    def __init__(self, ok=True):
        self.ncounts, self.ok = [], ok

    def run(self, params, ncount, seed, **kw):
        self.ncounts.append(ncount)
        return ({"ok": True, "elapsed_s": 0.25} if self.ok
                else {"ok": False, "diagnostics": ["boom"]})


def test_probe_uses_the_instance_protocol_not_a_literal():
    for fam in generate.FAMILIES:
        inst = generate.instance(fam, "train", 0)
        fx = _Recorder()
        rps, ncount, diag = cli.throughput_probe(fx, inst, n=3)
        assert diag is None
        assert ncount == inst["protocol"]["ncount"]
        assert fx.ncounts == [ncount] * 3
        assert rps == 4.0                      # 1 / 0.25 s


def test_sans_is_probed_at_more_rays_than_the_guide():
    def probed(fam):
        fx = _Recorder()
        cli.throughput_probe(fx, generate.instance(fam, "train", 0), n=1)
        return fx.ncounts[0]

    assert probed("sans_collimation") > probed("guide_divergence")


def test_a_failed_run_reports_diagnostics_instead_of_a_throughput():
    inst = generate.instance("guide_divergence", "train", 0)
    rps, ncount, diag = cli.throughput_probe(_Recorder(ok=False), inst, n=3)
    assert rps == 0.0 and diag == ["boom"]
