#!/usr/bin/env bash
# Stop the background serial logger started by monitor-start.sh.
# Kills the script wrapper plus its pio/miniterm children.

set -euo pipefail

cd "$(dirname "$0")/.."

PID_FILE=.serial.pid

if [ ! -f "$PID_FILE" ]; then
  echo "not running"
  exit 0
fi

PID=$(cat "$PID_FILE")
pkill -P "$PID" 2>/dev/null || true
kill "$PID" 2>/dev/null || true
rm -f "$PID_FILE"
echo "stopped (pid $PID)"
