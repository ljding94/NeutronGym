from mcstas_mcp import catalog


def test_full_library_found():
    assert len(catalog.component_names()) >= 370  # 374 in McStas 3.7.12


def test_categories_cover_core():
    cats = catalog.categories()
    for want in ("sources", "optics", "samples", "monitors", "contrib", "union"):
        assert cats.get(want), f"missing category {want}"


def test_describe_psd_monitor():
    d = catalog.describe("PSD_monitor")
    params = {p["name"]: p for p in d["parameters"]}
    assert params["nx"]["type"] == "int"
    assert params["nx"]["default"] == 90
    assert params["filename"]["type"] == "string"
    assert d["category"] == "monitors"
    assert d["description"]  # one-line doc extracted from %D


def test_required_params_guide():
    req = catalog.required_params("Guide")
    assert {"w1", "h1", "l"} <= set(req)


def test_nearest_match_suggestions():
    assert "PSD_monitor" in catalog.nearest("PSD_monitr")


def test_search_and_category_filter():
    guides = catalog.list_components(category="optics", search="guide")
    names = [c["name"] for c in guides]
    assert "Guide_gravity" in names
    assert all(c["category"] == "optics" for c in guides)
