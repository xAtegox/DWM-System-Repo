#!/usr/bin/env python3
"""
SlockScreen overlay. Spawned by slock-main.sh, one per locked window.
Uses override_redirect so it's never managed by dwm -- no window rules
needed, it just floats on top at the exact geometry of the target window.

Pulls its palette from pywal's colors.json when available, so it matches
whatever wallpaper theme is currently active, and falls back to a plain
dark scheme otherwise. Layout is fully relative (place() with relx/rely,
fonts recalculated from current height) so it looks right whether the
locked window is a full screen browser or a small floating terminal.
"""
import json
import os
import subprocess
import sys
import tkinter as tk

app_id, win_id, x, y, w, h, match_type, match_value = sys.argv[1:9]
x, y, w, h = int(x), int(y), int(w), int(h)

# ---- theme -----------------------------------------------------------

DEFAULT_THEME = {
    "bg": "#0c0c0c",
    "fg": "#f2f2f2",
    "accent": "#e0797a",
    "muted": "#8a8a8a",
    "panel": "#1c1c1c",
}


def load_theme():
    theme = dict(DEFAULT_THEME)
    path = os.path.expanduser("~/.cache/wal/colors.json")
    try:
        with open(path) as f:
            data = json.load(f)
        special = data.get("special", {})
        colors = data.get("colors", {})
        theme["bg"] = special.get("background", theme["bg"])
        theme["fg"] = special.get("foreground", theme["fg"])
        theme["accent"] = colors.get("color1", theme["accent"])
        theme["muted"] = colors.get("color8", theme["muted"])
        theme["panel"] = colors.get("color0", theme["panel"])
    except Exception:
        pass
    return theme


THEME = load_theme()

# ---- window ------------------------------------------------------------

root = tk.Tk()
root.overrideredirect(True)
root.geometry(f"{w}x{h}+{x}+{y}")
root.configure(bg=THEME["bg"])
root.attributes("-topmost", True)

canvas = tk.Canvas(root, bg=THEME["bg"], highlightthickness=0, bd=0)
canvas.place(relx=0, rely=0, relwidth=1, relheight=1)

clock_lbl = tk.Label(root, fg=THEME["fg"], bg=THEME["bg"])
msg_lbl = tk.Label(root, text="You ran out of usable time", fg=THEME["accent"], bg=THEME["bg"])
sub_lbl = tk.Label(
    root, text=f"'{app_id}' is locked until the next daily reset",
    fg=THEME["muted"], bg=THEME["bg"],
)


def close_target():
    if match_type == "title":
        # this is a browser tab, not a standalone app -- close just the tab
        subprocess.run(["xdotool", "windowactivate", win_id])
        subprocess.run(["xdotool", "key", "--window", win_id, "ctrl+w"])
    else:
        # graceful close request first; some apps ignore this, so fall back
        # to actually killing the process if it's still there a moment later
        subprocess.run(["wmctrl", "-ic", win_id])
        pid_result = subprocess.run(
            ["xdotool", "getwindowpid", win_id], capture_output=True, text=True
        )
        pid = pid_result.stdout.strip()
        if pid.isdigit():
            time.sleep(0.5)
            still_open = subprocess.run(
                ["xdotool", "getwindowname", win_id], capture_output=True
            )
            if still_open.returncode == 0:
                subprocess.run(["kill", "-9", pid])
    root.destroy()


btn = tk.Button(
    root, text="Close app / tab", command=close_target,
    bg=THEME["panel"], fg=THEME["fg"], activebackground=THEME["accent"],
    activeforeground=THEME["bg"], relief="flat", bd=0,
)

# ---- responsive layout ---------------------------------------------------

MIN_H_FOR_SUBTITLE = 160
MIN_H_FOR_ICON = 120


def draw_icon(size):
    canvas.delete("icon")
    if h < MIN_H_FOR_ICON:
        return
    cx, cy = w / 2, h * 0.18
    body_w, body_h = size, size * 0.75
    shackle_r = size * 0.32
    canvas.create_arc(
        cx - shackle_r, cy - size * 0.55 - shackle_r,
        cx + shackle_r, cy - size * 0.55 + shackle_r,
        start=0, extent=180, style="arc", outline=THEME["muted"],
        width=max(2, int(size * 0.08)), tags="icon",
    )
    canvas.create_rectangle(
        cx - body_w / 2, cy - size * 0.55,
        cx + body_w / 2, cy - size * 0.55 + body_h,
        fill=THEME["panel"], outline=THEME["muted"],
        width=max(1, int(size * 0.04)), tags="icon",
    )


def layout():
    """Recompute fonts, positions, and visibility for the current w/h."""
    canvas.config(width=w, height=h)

    icon_size = max(24, min(h, w) * 0.14)
    draw_icon(icon_size)

    clock_size = max(14, int(h * 0.11))
    msg_size = max(10, int(h * 0.05))
    sub_size = max(8, int(h * 0.032))

    clock_lbl.config(font=("monospace", clock_size, "bold"))
    msg_lbl.config(font=("sans-serif", msg_size), wraplength=max(120, int(w * 0.85)))
    sub_lbl.config(font=("sans-serif", sub_size), wraplength=max(120, int(w * 0.85)))

    icon_bottom = 0.34 if h >= MIN_H_FOR_ICON else 0.12
    clock_lbl.place(relx=0.5, rely=icon_bottom, anchor="n")
    msg_lbl.place(relx=0.5, rely=icon_bottom + 0.16, anchor="n")

    if h >= MIN_H_FOR_SUBTITLE:
        sub_lbl.place(relx=0.5, rely=icon_bottom + 0.26, anchor="n")
        btn.config(padx=max(10, int(w * 0.03)), pady=6)
        btn.place(relx=0.5, rely=0.86, anchor="s")
    else:
        sub_lbl.place_forget()
        btn.config(padx=max(8, int(w * 0.03)), pady=4)
        btn.place(relx=0.5, rely=0.94, anchor="s")

    btn.config(font=("sans-serif", max(9, int(h * 0.045))))


layout()

# ---- ticking clock + target watcher (single Tk-thread poll) -----------

import time  # noqa: E402


def tick():
    clock_lbl.config(text=time.strftime("%H:%M:%S"))
    root.after(1000, tick)


tick()

last_geom = (x, y, w, h)


def check_target():
    global last_geom, w, h

    result = subprocess.run(
        ["xdotool", "getwindowname", win_id], capture_output=True, text=True
    )
    if result.returncode != 0:
        root.destroy()
        return
    if match_type == "title" and match_value not in result.stdout:
        root.destroy()
        return

    geom = subprocess.run(
        ["xdotool", "getwindowgeometry", "--shell", win_id],
        capture_output=True, text=True,
    )
    if geom.returncode == 0:
        vals = dict(line.split("=", 1) for line in geom.stdout.splitlines() if "=" in line)
        try:
            gx, gy = int(vals["X"]), int(vals["Y"])
            gw, gh = int(vals["WIDTH"]), int(vals["HEIGHT"])
        except (KeyError, ValueError):
            gx = gy = gw = gh = None
        if gw and (gx, gy, gw, gh) != last_geom:
            root.geometry(f"{gw}x{gh}+{gx}+{gy}")
            w, h = gw, gh
            last_geom = (gx, gy, gw, gh)
            layout()

    root.after(1000, check_target)


root.after(1000, check_target)

root.grab_set()
root.focus_force()
root.mainloop()
