#!/bin/sh

# Window Maker style widgets for dwm

SCREEN_W=$(xdpyinfo | awk '/dimensions/{print $2}' | cut -d'x' -f1)

X=$((SCREEN_W - 80))
Y=50
GAP=70

spawn_widget() {
  "$@" &
  sleep 0.3
}

move_widget() {
  CLASS="$1"
  POS_Y="$2"

  WIN=$(xdotool search --class "$CLASS" | head -n 1)

  if [ -n "$WIN" ]; then
    xdotool windowmove "$WIN" "$X" "$POS_Y"
  fi
}

# Start widgets
spawn_widget wmclock
spawn_widget wmcpuload -w
spawn_widget wmbattery
spawn_widget wmwifi -w
spawn_widget wmgtemp

sleep 1

# Place them vertically
move_widget WMClock $Y
move_widget WMCPULoad $((Y + GAP))
move_widget WMBattery $((Y + GAP * 2))
move_widget WMWifi $((Y + GAP * 3))
move_widget WMGTemp $((Y + GAP * 4))
