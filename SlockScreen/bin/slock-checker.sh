#!/bin/bash
# SlockScreen watcher — put in .xinitrc BEFORE `exec dwm`, as a background job:
#   ~/Linux_Config/SlockScreen/bin/slock-checker.sh &
#
# Does the bare minimum every 5s: one wmctrl call, one xdotool call, some
# string matching. No GUI work happens here — that's all delegated to
# slock-main.sh, invoked (and forgotten) only when something actually needs
# to happen.

source "$HOME/Linux_Config/SlockScreen/bin/common.sh"
MAIN="$BASE_DIR/bin/slock-main.sh"
INTERVAL=5

while true; do
    sleep "$INTERVAL"

    [[ -s "$CONF_FILE" ]] || continue

    win_list=$(wmctrl -lx 2>/dev/null)
    [[ -z "$win_list" ]] && continue
    # xdotool prints window IDs in decimal; wmctrl prints them zero-padded hex.
    # Convert so the two can be compared as plain strings below.
    active_id_dec=$(xdotool getactivewindow 2>/dev/null)
    active_id=""
    [[ -n "$active_id_dec" ]] && active_id=$(printf "0x%08x" "$active_id_dec")

    while IFS='|' read -r id type match limit; do
        [[ -z "$id" || "$id" == \#* ]] && continue
        limit_sec=$(( limit * 60 ))

        # find a currently open window matching this config entry
        match_wid=""
        while IFS= read -r wline; do
            [[ -z "$wline" ]] && continue
            wid=$(awk '{print $1}' <<< "$wline")
            wclass=$(awk '{print $3}' <<< "$wline")
            wtitle=$(cut -d' ' -f5- <<< "$wline")
            if [[ "$type" == "class" && "$wclass" == *"$match"* ]]; then
                match_wid="$wid"; break
            elif [[ "$type" == "title" && "$wtitle" == *"$match"* ]]; then
                match_wid="$wid"; break
            fi
        done <<< "$win_list"

        [[ -z "$match_wid" ]] && continue

        used=$(get_usage "$id")
        state=$(get_state "$id")
        remaining=$(( limit_sec - used ))

        # already over budget -> lock on sight, even if not focused
        # (this is what makes it "instant" right after a reboot)
        if (( remaining <= 0 )); then
            [[ "$state" != "LOCKED" ]] && set_usage "$id" "$used" "LOCKED"
            "$MAIN" lock "$id" "$match_wid" &
            continue
        fi

        # only accrue time / warn while the window is actually focused
        if [[ "$match_wid" == "$active_id" ]]; then
            used=$(( used + INTERVAL ))
            remaining=$(( limit_sec - used ))
            if (( remaining <= 0 )); then
                set_usage "$id" "$used" "LOCKED"
                "$MAIN" lock "$id" "$match_wid" &
            elif (( remaining <= 120 )); then
                set_usage "$id" "$used" "WARN"
                "$MAIN" warn "$id" "$remaining" "$match_wid" &
            else
                set_usage "$id" "$used" "$state"
            fi
        fi
    done < "$CONF_FILE"
done
