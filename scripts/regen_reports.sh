#!/bin/sh
# Regenerate all three dashboards (the only sanctioned way — never hand-edit them).
# progress/tasks are stdlib-only; guide introspects the live server so it needs the conda env.
set -e
cd "$(dirname "$0")/.."
python3 scripts/progress_report.py
python3 scripts/tasks_report.py
conda run -n mcstas python scripts/guide_report.py
