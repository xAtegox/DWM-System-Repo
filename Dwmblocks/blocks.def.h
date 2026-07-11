//Modify this file to change what commands output to your statusbar, and recompile using the make command.
#define HOME "/home/atego"

static const Block blocks[] = {
    /*Icon*/    /*Command*/                                                                           /*Update Interval*/ /*Update Signal*/

    {"",        HOME "/dotfiles/scripts/.config/scripts/statusbar/musicplaying",   0,                11},
    {"",        HOME "/dotfiles/scripts/.config/scripts/statusbar/screenrecord",   0,                12},
  //  {"",        HOME "/dotfiles/scripts/.config/scripts/statusbar/weather",      180,                15},
    /* For the inline shell checks, you break the string just like you did for SHCMD in dwm */
    {"", "[ $(cat /sys/class/net/e*/carrier 2>/dev/null | grep -c 1) -eq 0 ] && " HOME "/dotfiles/scripts/.config/scripts/statusbar/wifi", 30, 0},
    {"",        HOME "/dotfiles/scripts/.config/scripts/statusbar/volume",           0,                10},
    {"", "[ -d /sys/class/power_supply/BAT0 ] && " HOME "/dotfiles/scripts/.config/scripts/statusbar/battery", 60, 0},
    {"",        HOME "/dotfiles/scripts/.config/scripts/statusbar/timedate",       30,                 0},
};

//sets delimiter between status commands. NULL character ('\0') means no delimiter.
static char delim[] = " | ";
static unsigned int delimLen = 3;
