"""Regression tests for the 2026-07-20 acceptance-test incident: MCP clients
launch the server binary with a PATH that lacks the conda env, and the server
must self-locate mcrun rather than die on the first tool call."""

import asyncio
import json
import os
import sys

from mcstas_mcp import config


def test_mcrun_path_self_locates_without_env_on_path(monkeypatch):
    monkeypatch.setenv("PATH", "/usr/bin:/bin")
    config.mcrun_path.cache_clear()
    try:
        assert config.mcrun_path() == os.path.join(config.ENV_BIN, "mcrun")
    finally:
        config.mcrun_path.cache_clear()


def test_server_works_when_launched_with_minimal_path():
    """Full stdio round-trip exactly as Claude Code launches it, minus PATH."""
    from fastmcp import Client
    from fastmcp.client.transports import StdioTransport

    server_bin = os.path.join(os.path.dirname(sys.executable), "mcstas-mcp")
    assert os.path.isfile(server_bin)
    env = {k: v for k, v in os.environ.items() if k != "PATH"}
    env["PATH"] = "/usr/bin:/bin:/usr/sbin:/sbin"  # no conda env anywhere

    async def _run():
        async with Client(StdioTransport(server_bin, [], env=env)) as client:
            res = await client.call_tool("describe_component", {"name": "Guide"})
            return json.loads(res.content[0].text)

    data = asyncio.run(_run())
    assert data["ok"], data
    assert [p["name"] for p in data["parameters"] if p["required"]] == ["w1", "h1", "l"]
