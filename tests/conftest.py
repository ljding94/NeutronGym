import os

import pytest


@pytest.fixture(autouse=True)
def isolated_home(tmp_path, monkeypatch):
    """Every test gets a throwaway ~/.mcstas-mcp equivalent."""
    monkeypatch.setenv("MCSTAS_MCP_HOME", str(tmp_path / "mcp_home"))
    # keep McStasScript/mcrun CWD side effects out of the repo
    work = tmp_path / "cwd"
    work.mkdir()
    monkeypatch.chdir(work)
    yield


@pytest.fixture
def spec():
    from mcstas_mcp import registry

    return registry.create("test_instr")
