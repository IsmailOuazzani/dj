#!/usr/bin/env bash
# Tail the most recent serial log produced by pio's log2file filter.

set -euo pipefail

cd "$(dirname "$0")/.."

LINES="${1:-50}"

latest=$(ls -t logs/*.log 2>/dev/null | head -n1 || true)
if [ -z "$latest" ]; then
  echo "no log files in logs/ — has monitor:start been run?" >&2
  exit 1
fi

echo "# $latest"
tail -n "$LINES" "$latest"
