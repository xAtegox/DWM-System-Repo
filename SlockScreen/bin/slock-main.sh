#!/bin/bash
# SlockScreen main logic. Invoked by slock-checker.sh (warn/lock) or by a
# dwm keybind (add). Never runs on a timer itself — only reacts.

source "$HOME/Linux_Config/SlockScreen/bin/common.sh"

BEEP_FILE="/usr/share/sounds/freedesktop/stereo/dialog-warning.oga"
BEEP_VOLUME=200000   # 65536 = 100%; this is ~305%. Will clip/distort at this level
                     # on purpose -- that's what makes it cut through. Lower if it's
                     # actually painful rather than just attention-grabbing.

beep() {
    if [[ -f "$BEEP_FILE" ]]; then
        paplay --volume="$BEEP_VOLUME" "$BEEP_FILE" 2>/dev/null
    else
        canberra-gtk-play -i dialog-warning 2>/dev/null
    fi
}

is_focused() {
    local wid="$1" cur cur_hex
    cur=$(xdotool getactivewindow 2>/dev/null)
    [[ -z "$cur" ]] && return 1
    cur_hex=$(printf "0x%08x" "$cur")
    [[ "$cur_hex" == "$wid" ]]
}

# Called once per checker cycle (every 5s) while remaining <= 60 AND the
# window is focused (checker already guarantees that at call time). Only
# beeps at specific marks so it doesn't spam every single cycle, and for
# the final stretch does its own second-by-second countdown that re-checks
# focus before every beep — so looking away mid-countdown stops it dead
# instead of finishing a countdown nobody's there to hear.
warn_action() {
    local id="$1" remaining="$2" wid="$3"

    case "$remaining" in
        120|60|40|20|10)
            beep
            notify-send -u critical "SlockScreen" "${id}: ${remaining}s of screen time left" 2>/dev/null
            ;;
        5)
            for t in 5 4 3 2 1; do
                is_focused "$wid" || return   # looked away — stop nagging
                beep
                notify-send -u critical "SlockScreen" "${id}: ${t}s of screen time left" 2>/dev/null
                (( t > 1 )) && sleep 1
            done
            ;;
    esac
}

lock_action() {
    local id="$1" wid="$2"
    local statefile="$LOCK_STATE_DIR/${wid}"

    # don't double-spawn an overlay for a window that's already locked
    if [[ -f "$statefile" ]] && kill -0 "$(cat "$statefile" 2>/dev/null)" 2>/dev/null; then
        return
    fi

    local conf_line match_type match_value
    conf_line=$(grep "^${id}|" "$CONF_FILE")
    IFS='|' read -r _ match_type match_value _ <<< "$conf_line"

    read -r wx wy ww wh <<< "$(xdotool getwindowgeometry --shell "$wid" 2>/dev/null \
        | awk -F= '/^X=|^Y=|^WIDTH=|^HEIGHT=/{print $2}' | tr '\n' ' ')"
    [[ -z "$ww" ]] && return   # window vanished before we could read it

    python3 "$OVERLAY_PY" "$id" "$wid" "$wx" "$wy" "$ww" "$wh" "$match_type" "$match_value" &
    echo $! > "$statefile"
}

add_action() {
    local wid class title id type match limit
    wid=$(xdotool getactivewindow)
    class=$(xdotool getwindowclassname "$wid")
    title=$(xdotool getwindowname "$wid")

    id=$(echo "" | dmenu -p "SlockScreen: name for this entry:")
    [[ -z "$id" ]] && return

    type=$(printf "class\ntitle" | dmenu -p "Match whole app (class) or a page/tab (title)?")
    [[ -z "$type" ]] && return

    if [[ "$type" == "class" ]]; then
        match="$class"
    else
        match=$(echo "$title" | dmenu -p "Title substring to match:")
        [[ -z "$match" ]] && return
    fi

    limit=$(echo "" | dmenu -p "Daily limit (minutes):")
    [[ -z "$limit" ]] && return

    grep -v "^${id}|" "$CONF_FILE" > "${CONF_FILE}.tmp" 2>/dev/null
    echo "${id}|${type}|${match}|${limit}" >> "${CONF_FILE}.tmp"
    mv "${CONF_FILE}.tmp" "$CONF_FILE"
    notify-send "SlockScreen" "Tracking '${id}' (${type}: ${match}) — ${limit}min/day"
}

case "$1" in
    warn) warn_action "$2" "$3" "$4" ;;
    lock) lock_action "$2" "$3" ;;
    add)  add_action ;;
    *) echo "usage: slock-main.sh {warn ID REMAINING | lock ID WINID | add}" ;;
esac
