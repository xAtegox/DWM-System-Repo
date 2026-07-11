//Modify this file to change what commands output to your statusbar, and recompile using the make command.
#define HOME "/home/atego"

static const Block blocks[] = {
    /*Icon*/    /*Command*/                                                                           /*Update Interval*/ /*Update Signal*/

    {"",        HOME "/.config/scripts/statusbar/musicplaying",   0,                11},
    {"",        HOME "/.config/scripts/statusbar/screenrecord",   0,                12},
    {"", "[ $(cat /sys/class/net/e*/carrier 2>/dev/null | grep -c 1) -eq 0 ] && " HOME "/.config/scripts/statusbar/wifi", 30, 0},
    {"", "[ $(pactl list short source-outputs | wc -l) -eq 0 ] || " HOME "/.config/scripts/statusbar/microphone", 0, 13},
    {"",        HOME "/.config/scripts/statusbar/volume",         0,                10},
    {"", "ls -d /sys/class/power_supply/BAT* &>/dev/null && " HOME "/.config/scripts/statusbar/battery", 120, 0},
    {"",        HOME "/.config/scripts/statusbar/timedate",       30,                0},
};

//sets delimiter between status commands. NULL character ('\0') means no delimiter.
static char delim[] = " | ";
static unsigned int delimLen = 3;
