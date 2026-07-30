#!/bin/sh

# Window Maker dockapps for dwm

# kill old instances
pkill wmclock
pkill wmcpuload
pkill wmbattery
pkill wmwifi
pkill wmgtemp

sleep 0.5

# start dockapps
wmclock &
sleep 0.2

wmcpuload -bw &
sleep 0.2

wmbattery &
sleep 0.2

wmwifi -bw &
sleep 0.2

wmgtemp &
sleep 0.5

# screen size
SCREEN_W=$(xdpyinfo | awk '/dimensions/{print $2}' | cut -dx -f1)

X=$((SCREEN_W - 80))
Y=50
GAP=70

move_window() {
  NAME="$1"
  POS="$2"

  ID=$(xdotool search --name "$NAME" | head -n1)

  if [ -n "$ID" ]; then
    xdotool windowmove "$ID" "$X" "$POS"
  fi
}

# place widgets
move_window "wmclock" $Y
move_window "wmcpuload" $((Y + GAP))
move_window "wmbattery" $((Y + GAP * 2))
move_window "wmwifi" $((Y + GAP * 3))
move_window "wmgtemp" $((Y + GAP * 4))
