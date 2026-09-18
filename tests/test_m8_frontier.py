"""Frontier arm: spend accounting and the pre-registered guards."""

import os
import sys

import pytest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))), "benchmark", "harness"))

import m8_frontier as fr  # noqa: E402


def test_prices_and_pins_come_from_the_pinned_config():
    p = fr.price_table()
    assert p["anthropic/claude-sonnet-5"] == {"in": 2.0, "out": 10.0, "pin": "Google"}
    assert p["openai/gpt-5.2-pro"]["pin"] == "OpenAI"


def test_cost_is_per_million_tokens():
    c = fr.cost_usd({"prompt_tokens": 1_000_000, "completion_tokens": 100_000},
                    {"in": 2.0, "out": 10.0})
    assert c == pytest.approx(3.0)


def test_metered_stops_at_the_spend_ceiling_instead_of_overspending():
    m = fr.Metered("x", {"in": 1.0, "out": 1.0, "pin": None}, max_usd=1.0)
    m.usage = {"prompt_tokens": 2_000_000, "completion_tokens": 0}   # $2 spent
    assert m.spent == pytest.approx(2.0)
    with pytest.raises(fr.BudgetExceeded):
        m([{"role": "user", "content": "hi"}])
