#!/usr/bin/env bash

WALL_DIR="$HOME/Pictures/wallpaper"
WAL_LAST="$(dirname "$0")/wal_last.txt"
BACKENDS=(wal colorthief colorz haishoku)
BACKEND_IDX=0

# --- Spawn centered kitty window and re-exec inside it ---
if [[ "$1" != "--inside" ]]; then
    kitty \
        --class  "wal-picker" \
        --title  "Wallpaper Picker" \
        -o "initial_window_width=1000" \
        -o "initial_window_height=600" \
        -o "background=#1a1510" \
        -o "foreground=#E8D5B7" \
        -o "cursor=#E8D5B7" \
        -o "remember_window_size=no" \
        -o "placement_strategy=center" \
        -e "$0" --inside "$@"
    exit
fi
shift  # remove --inside

# --- Ueberzugpp setup ---
UB_PID_FILE="/tmp/.wal_$$"
cleanup() {
    ueberzugpp cmd -s "$SOCKET" -a exit 2>/dev/null
    rm -f "$UB_PID_FILE"
}
trap cleanup EXIT

ueberzugpp layer --no-stdin --silent --pid-file "$UB_PID_FILE" 2>/dev/null
UB_PID=$(cat "$UB_PID_FILE")
export SOCKET="/tmp/ueberzugpp-${UB_PID}.socket"

# --- Apply wallpaper + refresh everything ---
apply() {
    local img="$1"
    local backend="$2"

    notify-send "Wallpaper" "Applying with backend: $backend..." -t 2000

    wal -i "$img" --backend "$backend" -t -n -q

    feh --bg-scale "$img"
    xrdb -merge "$HOME/.Xresources" 2>/dev/null

    # Refresh terminal colors
    for pts in /dev/pts/*; do
        [ -w "$pts" ] && cat "$HOME/.cache/wal/sequences" >"$pts" 2>/dev/null
    done

    # Refresh kitty
    kitty @ set-colors --all --configured "$HOME/.cache/wal/colors-kitty.conf" 2>/dev/null

    # Refresh dunst
    killall dunst 2>/dev/null
    sleep 0.15
    dunst -conf "$HOME/.cache/wal/dunstrc" &

    # Refresh dwmblocks
    pkill -RTMIN+10 dwmblocks 2>/dev/null

    # Save last used wallpaper
    echo "$img" > "$WAL_LAST"

    notify-send "Wallpaper" "$(basename "$img") applied!" -t 3000
}

# --- Main loop: allows backend switching without closing ---
while true; do
    BACKEND="${BACKENDS[$BACKEND_IDX]}"
    BACKEND_LIST=""
    for i in "${!BACKENDS[@]}"; do
        if [[ $i -eq $BACKEND_IDX ]]; then
            BACKEND_LIST+="► ${BACKENDS[$i]}  "
        else
            BACKEND_LIST+="  ${BACKENDS[$i]}  "
        fi
    done

    HEADER="[$BACKEND]  ctrl-b:backend  ctrl-r:random"

    IMG=$(find -L "$WALL_DIR" -type f \( -iname "*.jpg" -o -iname "*.png" -o -iname "*.jpeg" \) \
        | sort \
        | fzf \
            --reverse \
            --no-sort \
            --prompt="▶ " \
            --pointer="›" \
            --color="bg:#1a1510,bg+:#3d2e1a,fg:#E8D5B7,fg+:#ffffff,prompt:#D4A96A,pointer:#D4A96A,header:#9a8060,hl:#D4A96A,hl+:#ffcc77" \
            --no-border \
            --header="$HEADER" \
            --delimiter="/" \
            --with-nth="-1" \
            --preview="ueberzugpp cmd -s $SOCKET -i walpreview -a add \
                -x \$FZF_PREVIEW_LEFT \
                -y \$FZF_PREVIEW_TOP \
                --max-width \$FZF_PREVIEW_COLUMNS \
                --max-height \$FZF_PREVIEW_LINES \
                -f {}" \
            --preview-window="right:55%:border-left" \
            --bind="ctrl-b:become(echo __NEXT_BACKEND__)" \
            --bind="ctrl-r:become(echo __RANDOM__)" \
            --bind="esc:become(echo __QUIT__)" \
    )

    case "$IMG" in
        __QUIT__|"")
            exit 0
            ;;
        __NEXT_BACKEND__)
            BACKEND_IDX=$(( (BACKEND_IDX + 1) % ${#BACKENDS[@]} ))
            continue
            ;;
        __RANDOM__)
            IMG=$(find -L "$WALL_DIR" -type f \( -iname "*.jpg" -o -iname "*.png" -o -iname "*.jpeg" \) \
                | shuf -n 1)
            apply "$IMG" "$BACKEND"
            exit 0
            ;;
        *)
            apply "$IMG" "$BACKEND"
            exit 0
            ;;
    esac
done
