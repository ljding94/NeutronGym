import pytest

from mcstas_mcp import registry
from mcstas_mcp.registry import SpecError


def test_create_and_reload(spec):
    assert registry.exists("test_instr")
    assert registry.load("test_instr")["name"] == "test_instr"


def test_duplicate_instrument_rejected(spec):
    with pytest.raises(SpecError, match="already exists"):
        registry.create("test_instr")


def test_unknown_component_suggests_nearest(spec):
    with pytest.raises(SpecError, match="Nearest matches.*PSD_monitor"):
        registry.add_component(spec, "psd", "PSD_monitr", at=[0, 0, 1])


def test_unknown_param_suggests_nearest(spec):
    with pytest.raises(SpecError, match="Did you mean.*xwidth"):
        registry.add_component(
            spec, "src", "Source_simple", at=[0, 0, 0], parameters={"xwidht": 0.02}
        )


def test_duplicate_component_name_rejected(spec):
    registry.add_component(spec, "src", "Source_simple", at=[0, 0, 0])
    with pytest.raises(SpecError, match="already used"):
        registry.add_component(spec, "src", "Arm", at=[0, 0, 1])


def test_relative_to_unknown_component(spec):
    with pytest.raises(SpecError, match="not a component"):
        registry.add_component(spec, "psd", "PSD_monitor", at=[0, 0, 1], relative="ghost")


def test_isalpha_loophole_closed(spec):
    """underscored undeclared identifiers pass McStasScript silently — not us."""
    registry.add_component(spec, "src", "Source_simple", at=[0, 0, 0])
    with pytest.raises(SpecError, match="unknown identifier.*undeclared_var"):
        registry.set_parameters(spec, "src", {"lambda0": "undeclared_var"})


def test_expression_over_instrument_parameter_ok(spec):
    registry.add_parameter(spec, "wl", default=5.0)
    registry.add_component(
        spec, "src", "Source_simple", at=[0, 0, 0],
        parameters={"lambda0": "wl", "dlambda": "0.1*wl"},
    )
    comp = registry.load("test_instr")["components"][0]
    assert comp["parameters"]["dlambda"] == "0.1*wl"


def test_string_param_auto_quoted(spec):
    registry.add_component(
        spec, "psd", "PSD_monitor", at=[0, 0, 1], parameters={"filename": "beam.dat"}
    )
    comp = registry.load("test_instr")["components"][0]
    assert comp["parameters"]["filename"] == '"beam.dat"'


def test_missing_required_reported_as_warning(spec):
    warnings = registry.add_component(spec, "guide", "Guide", at=[0, 0, 1])
    assert any("w1" in w for w in warnings)
    assert registry.missing_required(spec) == [("guide", ["w1", "h1", "l"])]


def test_build_instr_file(spec):
    registry.add_parameter(spec, "wl", default=5.0)
    registry.add_component(
        spec, "src", "Source_simple", at=[0, 0, 0],
        parameters={"xwidth": 0.02, "yheight": 0.02, "dist": 2,
                    "focus_xw": 0.05, "focus_yh": 0.05, "lambda0": "wl", "dlambda": 1},
    )
    registry.add_component(
        spec, "psd", "PSD_monitor", at=[0, 0, 2], relative="src",
        parameters={"nx": 50, "ny": 50, "xwidth": 0.1, "yheight": 0.1,
                    "filename": "psd.dat"},
    )
    path = registry.build_instr_file(registry.load("test_instr"))
    text = open(path).read()
    assert "DEFINE INSTRUMENT test_instr" in text
    assert 'COMPONENT psd = PSD_monitor' in text
    assert 'filename="psd.dat"' in text.replace(" ", "")
    assert "RELATIVE src" in text
