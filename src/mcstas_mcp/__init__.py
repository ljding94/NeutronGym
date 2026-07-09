"""mcstas-mcp: MCP server exposing McStas to LLM agents.

Design: note/m1-server-design-2026-07-09.md. McStasScript handles component
introspection, instrument construction, and data loading; this package owns
execution (subprocess mcrun with captured diagnostics) and the instrument
registry (declarative JSON specs).
"""

__version__ = "0.1.0"
