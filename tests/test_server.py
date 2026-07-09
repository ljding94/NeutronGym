"""MCP-layer tests: call tools through the FastMCP in-process client."""

import asyncio
import json

import pytest
from fastmcp import Client

from mcstas_mcp.server import mcp


def call(tool, **kwargs):
    async def _run():
        async with Client(mcp) as client:
            res = await client.call_tool(tool, kwargs)
            return json.loads(res.content[0].text)

    return asyncio.run(_run())


def test_tools_exposed():
    async def _list():
        async with Client(mcp) as client:
            return [t.name for t in await client.list_tools()]

    names = asyncio.run(_list())
    for want in ("list_components", "describe_component", "create_instrument",
                 "add_component", "set_parameters", "run_simulation",
                 "get_results", "get_monitor_data"):
        assert want in names


def test_list_components_over_mcp():
    out = call("list_components", category="sources")
    assert out["ok"] and out["count"] >= 10
    assert any(c["name"] == "Source_simple" for c in out["components"])


def test_validation_error_reaches_agent():
    call("create_instrument", name="mcp_test")
    out = call("add_component", instrument_id="mcp_test", name="psd",
               component="PSD_monitr", at=[0, 0, 1])
    assert not out["ok"]
    assert "PSD_monitor" in out["error"]  # nearest-match suggestion survives MCP
