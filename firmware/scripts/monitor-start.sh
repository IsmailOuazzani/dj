#!/usr/bin/env bash
# Start `pio device monitor` in the background so a headless caller (agent / CI)
# can tail the log it produces. Idempotent — re-running while already started is a no-op.
#
# We have to wrap pio in `script -q /dev/null` because pyserial's miniterm calls
# termios.tcgetattr(stdin) unconditionally on init and dies without a TTY.
# `script` allocates a pseudo-TTY on stdin; pio writes its own output to
# logs/<env>-<timestamp>.log via the `log2file` monitor_filters setting in platformio.ini.

set -euo pipefail

cd "$(dirname "$0")/.."

PID_FILE=.serial.pid

if [ -f "$PID_FILE" ] && kill -0 "$(cat "$PID_FILE")" 2>/dev/null; then
  echo "already running (pid $(cat "$PID_FILE"))"
  exit 0
fi

mkdir -p logs

nohup script -q /dev/null pio device monitor --quiet </dev/null >/dev/null 2>&1 &
PID=$!
echo "$PID" > "$PID_FILE"
echo "started (pid $PID); pio writes to logs/<env>-<timestamp>.log"
