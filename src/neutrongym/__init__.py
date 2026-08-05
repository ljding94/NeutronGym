"""NeutronGym — the environment package (headline artifact).

Will hold, per the M5 build (PLAN.md, layout decision 2026-07-30):
  reward.py    — reward-ladder API, level-resolved (L1 syntax -> L4 scientific)
  executor.py  — fast-tier step(): direct binary execution, compile-once
                 per template family
  generate.py  — procedural instance generator (template families,
                 held-out parameter regimes)
  agent.py     — the reference loop: minimal model-agnostic scaffold
                 (note/scaffold-decision-2026-07-30.md)

`src/mcstas_mcp/` is the MCP server the agent talks to; this package is the
environment around it.
"""
