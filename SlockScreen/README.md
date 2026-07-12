# SlockScreen

A self-imposed daily screen-time limiter for CachyOS + dwm, enforced with a
floating lock overlay rather than a honor-system timer.

---

## Disclaimer

This project is **vibecoded** — built iteratively through conversation with
an AI assistant rather than from a formal spec or design document. It works,
it's been tested against real usage on the author's own machine, and each
reported bug was fixed as it was found — but it hasn't had a security review
or production-grade hardening. Treat it as a personal utility and read
through the scripts before running them.

---

## Overview

SlockScreen assigns a daily time budget (in minutes) to specific applications
and specific browser tabs/websites. Time only counts down while the tracked
window is actually focused — background tabs and unfocused apps don't spend
the budget. Once the budget is exhausted, a lock overlay is placed over the
window: a clock, a message, and a button to close the app or tab. Budgets
reset daily at **9:00 AM**, and the reset is computed from stored timestamps
rather than "since last boot," so it behaves correctly across reboots.

The system is split into an always-running watcher and an on-demand handler,
so it costs effectively nothing while under budget:

| Component | Role |
| --- | --- |
| `slock-checker.sh` | The only continuously running piece. A 5-second loop doing one `wmctrl` call and one `xdotool` call per cycle to check tracked windows against the config. |
| `slock-main.sh` | Invoked by the checker (or a dwm keybind) only when something needs to happen: playing a warning beep, spawning the lock overlay, or adding a new tracked entry. Never runs on its own schedule. |
| `slock-overlay.py` | The lock screen itself (Tkinter). Only ever launched by `slock-main.sh`. |

---

## Requirements

```bash
sudo pacman -S xdotool wmctrl tk python
```

Assumed already present:

- `canberra-gtk-play` / `paplay` (libcanberra + PulseAudio/PipeWire) — warning beep
- `dunst` — for `notify-send` toasts
- `dmenu` — for the add-app prompts
- `pywal` — optional; if `~/.cache/wal/colors.json` exists, the lock screen
  themes itself to the current wallpaper palette, otherwise falls back to a
  plain dark scheme

---

## File Layout

```
~/Linux_Config/SlockScreen/
├── bin/
│   ├── common.sh          # shared functions — sourced only, never run directly
│   ├── slock-checker.sh   # the loop — goes in .xinitrc
│   ├── slock-main.sh      # the logic — checker and keybind call this
│   └── slock-overlay.py   # the lock screen UI
├── config/
│   └── apps.conf          # tracked apps/sites, one entry per line
└── data/                  # auto-created; not intended to be hand-edited
    ├── usage.dat          # per-entry seconds used, last-write time, state
    └── locks/             # one pidfile per currently-locked window
```

---

## Installation

```bash
mkdir -p ~/Linux_Config/SlockScreen
cp -r bin config ~/Linux_Config/SlockScreen/
chmod +x ~/Linux_Config/SlockScreen/bin/*.sh ~/Linux_Config/SlockScreen/bin/*.py
touch ~/Linux_Config/SlockScreen/config/apps.conf
```

### 1. Start the checker at login

Add this line to `~/.xinitrc`, **before** `exec dwm`:

```bash
~/Linux_Config/SlockScreen/bin/slock-checker.sh &
```

### 2. Add a dwm keybind for adding tracked apps

In dwm's `config.h`, near your other `static const char *foo[]` command arrays:

```c
static const char *slockadd[] = { "/home/atego/Linux_Config/SlockScreen/bin/slock-main.sh", "add", NULL };
```

And in the `keys[]` array, on a combo that isn't already bound:

```c
{ MODKEY|ShiftMask, XK_t, spawn, {.v = slockadd } },
```

Rebuild and reinstall dwm, then restart it:

```bash
cd ~/path/to/your/dwm/source
sudo make clean install
```

### 3. Log out and back into X

Confirm the checker is running:

```bash
pgrep -fa slock-checker
```

---

## Usage

### Adding a tracked app or website

Focus the target window/tab, then either press the bound keybind or run:

```bash
~/Linux_Config/SlockScreen/bin/slock-main.sh add
```

You'll be prompted for:

1. **A name** for the entry (label only, e.g. `youtube`, `minecraft`)
2. **Match type**
   - `class` — matches WM_CLASS. Use for standalone applications; WM_CLASS
     stays constant regardless of what's displayed inside the app.
   - `title` — matches a substring of the window title. Use for specific
     websites/tabs, since browsers put the page title in the window title
     (e.g. `<page title> - YouTube`).
3. **Daily limit**, in minutes.

This appends a line to `apps.conf` in the format:

```
id|type|match|daily_limit_minutes
```

Entries can also be added or edited directly:

```
youtube|title|YouTube|30
minecraft|class|Minecraft|90
reddit|title|reddit.com|15
terminal|class|kitty|120
```

If an app is already open and over budget at the time it's added, it will
be locked on the checker's next cycle — no need to reopen it.

### Clearing state

Wipe current locks and usage without touching the tracked app list:

```bash
pkill -f slock-overlay
rm -f ~/Linux_Config/SlockScreen/data/locks/*
rm -f ~/Linux_Config/SlockScreen/data/usage.dat
touch ~/Linux_Config/SlockScreen/data/usage.dat
```

Wipe the tracked app list as well:

```bash
> ~/Linux_Config/SlockScreen/config/apps.conf
```

---

## Behavior Reference

| Behavior | Detail |
| --- | --- |
| Time accrual | Only while the matched window is focused |
| Reset | 9:00 AM, computed from stored timestamps vs. the most recent 9AM boundary — not a cron job |
| Reboot handling | The checker scans all open windows every cycle, not just the focused one, so an over-budget app already open at login is locked within 5 seconds |
| Warning schedule | Beep + notification at 120s, 60s, 40s, 20s, 10s, then a focus-verified second-by-second countdown at 5–1s. Looking away mid-countdown stops it immediately. |
| Beep volume | Boosted well past 100% via `paplay --volume` (`BEEP_VOLUME` in `slock-main.sh`, 65536 = 100%) |
| Overlay positioning | Borderless (`override_redirect`) window matched to the target's geometry, re-synced every second so moving/resizing the underlying floating window drags the overlay with it |
| Overlay theming | Pulled from `~/.cache/wal/colors.json` if present |
| Small-window handling | Subtitle and icon are dropped below certain height thresholds to avoid overlap on small windows |
| Close button (tab entries) | Focuses the browser window and sends `ctrl+w` to close just that tab |
| Close button (app entries) | Graceful close request first; force-kills the process half a second later if it's still open |
| Automatic kill | None — the overlay persists indefinitely until manually closed via the button; there is no timeout-based force-close |

### Known Limitation

Website tracking matches on window *title*, not URL. Switching away from a
tracked tab makes the title stop matching, so the overlay disappears while
you're on a different tab — this is not a bypass; switching back while still
over budget re-locks within the next 5-second cycle. Accurate URL-based
tracking would require a browser extension, which is out of scope here.

---

## Troubleshooting

| Symptom | Check |
| --- | --- |
| Keybind does nothing | `pgrep -fa slock-main` right after pressing it. If empty, `config.h` likely wasn't rebuilt/reinstalled. |
| Add-app runs but nothing appears | Run `bash -x slock-main.sh add` manually to see where it fails. |
| Time never accrues | Confirm `pgrep -fa slock-checker` is running, and confirm the app's real WM_CLASS/title via `wmctrl -lx` while focused — compare exactly against `apps.conf`. |
| Usage file looks empty during a test | `usage.dat` can't be watched from a terminal while the tracked window is focused — checking the file steals focus. Focus the app, wait, then check. |
