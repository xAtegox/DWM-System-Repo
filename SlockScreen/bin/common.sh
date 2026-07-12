#!/bin/bash
# SlockScreen - shared paths & helpers. Sourced by slock-checker.sh and slock-main.sh.

BASE_DIR="$HOME/Linux_Config/SlockScreen"
CONF_FILE="$BASE_DIR/config/apps.conf"
DATA_FILE="$BASE_DIR/data/usage.dat"
LOCK_STATE_DIR="$BASE_DIR/data/locks"
OVERLAY_PY="$BASE_DIR/bin/slock-overlay.py"
RESET_HOUR=9   # daily reset time (24h)

mkdir -p "$BASE_DIR/config" "$BASE_DIR/data" "$LOCK_STATE_DIR"
touch "$CONF_FILE" "$DATA_FILE"

# Epoch of the most recent RESET_HOUR:00 boundary (today's if already passed, else yesterday's)
period_start_epoch() {
    local now today_reset
    now=$(date +%s)
    today_reset=$(date -d "today ${RESET_HOUR}:00" +%s)
    if (( now >= today_reset )); then
        echo "$today_reset"
    else
        date -d "yesterday ${RESET_HOUR}:00" +%s
    fi
}

# Prints seconds used for a given id, transparently resetting if the stored
# value belongs to a previous day's period.
get_usage() {
    local id="$1" pstart line used last state
    pstart=$(period_start_epoch)
    line=$(grep "^${id}|" "$DATA_FILE")
    [[ -z "$line" ]] && { echo 0; return; }
    IFS='|' read -r _ used last state <<< "$line"
    if (( last < pstart )); then
        echo 0
    else
        echo "$used"
    fi
}

get_state() {
    local id="$1" line state
    line=$(grep "^${id}|" "$DATA_FILE")
    [[ -z "$line" ]] && { echo OK; return; }
    IFS='|' read -r _ _ _ state <<< "$line"
    echo "${state:-OK}"
}

set_usage() {
    local id="$1" used="$2" state="$3" now
    now=$(date +%s)
    grep -v "^${id}|" "$DATA_FILE" > "${DATA_FILE}.tmp" 2>/dev/null
    echo "${id}|${used}|${now}|${state}" >> "${DATA_FILE}.tmp"
    mv "${DATA_FILE}.tmp" "$DATA_FILE"
}
