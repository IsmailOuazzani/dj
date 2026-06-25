#!/usr/bin/env bash
# Live-follow the latest serial log produced by the background monitor.
# Blocks until Ctrl-C.

set -euo pipefail

cd "$(dirname "$0")/.."

latest=$(ls -t logs/*.log 2>/dev/null | head -n1 || true)
if [ -z "$latest" ]; then
  echo "no log files in logs/ — has monitor:start been run?" >&2
  exit 1
fi

echo "# following $latest (Ctrl-C to stop)"
tail -F -n 0 "$latest"
