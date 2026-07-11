/* appearance */
static unsigned int borderpx = 1; // Window Border pixel
static unsigned int snap = 5; // Pixel Snap to edge
static const unsigned int gappih = 20;
static const unsigned int gappiv = 20;
static const unsigned int gappoh = 20;
static const unsigned int gappov = 30;
static int smartgaps = 0;
static const int swallowfloating = 0;
static int showbar = 1;
static const int showtitle = 1;
static const int showtags = 1;
static const int showlayout = 1;
static const int showstatus = 1;
static const int showfloating = 0;
static int topbar = 1;
static char dmenufont[] = "Iosevka Nerd Font:size=12";
static const char *fonts[] = {
    "Iosevka Nerd Font:size=12",
    "NotoColorEmoji:pixelsize=12:antialias=true:autohint=true"
};

// Colours if xrdb isnt loaded
static char normbgcolor[] = "#1f1c14";
static char normbordercolor[] = "#1f1c14";
static char normfgcolor[] = "#B5976E";
static char selfgcolor[] = "#1f1c14";
static char selbordercolor[] = "#928c82";
static char selbgcolor[] = "#B5976E";
static char *colors[][3] = {

    [SchemeNorm] = {normfgcolor, normbgcolor, normbordercolor},
    [SchemeSel]  = {selbgcolor,  selfgcolor,  selbordercolor},
    [SchemeStatus]   = {normfgcolor,   normbgcolor, normbgcolor},
    [SchemeTagsSel]  = {normfgcolor,   normbgcolor, normbgcolor},
    [SchemeTagsNorm] = {selbordercolor, normbgcolor, normbgcolor},
    [SchemeInfoSel]  = {normfgcolor,   normbgcolor, normbgcolor},
    [SchemeInfoNorm] = {normfgcolor,   normbgcolor, normbgcolor},
};

static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
static const Rule rules[] = {
    {"neofetch",          NULL,         "Welcome",      0, 1, 1, 0, -1},  
    {"st-256color",       NULL,         NULL,           0, 0, 1, 0, -1},
    {"fzfmenu",           NULL,         NULL,           0, 1, 1, 1, -1},   
    {"mpv",               NULL,         NULL,           0, 1, 0, 1, -1},  
    {"Nsxiv",             NULL,         NULL,           0, 1, 0, 1, -1},   
    {NULL,                NULL,         "Event Tester", 0, 0, 0, 1, -1}, 
    {NULL, NULL, "emacs-everywhere", 0, 1, 0, 1, -1}, /* emacs-everywhere popup */
    {"musicwiki",         "musicwiki",  NULL,           0, 1, -1 },
    {"floating_script",   NULL,         NULL,           0, 1, -1 },
    {"st-256color",       NULL,         "music-player", 0, 1, -1 },
    { NULL,               NULL,         "CamPreview",   0, 1, -1 },
    { "wal-picker",       NULL,         NULL,           0, 1, 0, 1, -1 },
    { "clipboard-picker", NULL,         NULL,           0, 1, 0, 1, -1 },
    { "app-launcher",     NULL,         NULL,           0, 1, 0, 1, -1 },
    { "scratchpad",       NULL,         NULL,           0, 1, 1, 0, -1 },
    { NULL, NULL, "Powermenu", 0, 1, -1 },
    { "Music Preview", NULL, NULL, 0, 1, 0, 1, -1 },
    { "Wallpaper Picker", NULL, NULL, 1, 0, -1 },
    { NULL, NULL, "Welcome", 0, 1, -1 },
    { "WorldPainter", NULL, NULL, 0, 1, -1 },
    {"fzfmenu",NULL, NULL,        0, 1, 1, 1, -1},     /* fzf menu (any title) */
    {"mpv",    NULL, NULL,        0, 1, 0, 1, -1},     /* mpv video player */
    {"Nsxiv",  NULL, NULL,        0, 1, 0, 1, -1},     /* nsxiv image preview */

};

#include "vanitygaps.c"
#include <X11/XF86keysym.h>

static const float mfact = 0.55;    // Factor of master area
static const int nmaster = 1;       // Number of masters
static const int resizehints = 1;
static const int lockfullscreen = 0; // Wether you can toggle fullscreen off or on (1 means perma fulscreen)
static const Layout layouts[] = {
    {"", tile},
    {"", NULL},
    {"", monocle},
    {"", spiral},
    {"󱤆", dwindle},
};

#define MODKEY Mod4Mask // Windows Key for MODKEY

                      
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } } // DEFINES FOR $HOME COMAND
                                                                          

#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},


#define STACKKEYS(MOD, ACTION)                                                 \
  {MOD, XK_j, ACTION##stack, {.i = INC(+1)}},                                  \
      {MOD,                                                                    \
       XK_k,                                                                   \
       ACTION##stack,                                                          \
       {.i = INC(-1)}}, /*{ MOD, XK_grave, ACTION##stack, {.i = PREVSEL } },   \
                        { MOD, XK_q,     ACTION##stack, {.i = 0 } },           \
                        { MOD, XK_a,     ACTION##stack, {.i = 1 } },           \
                        { MOD, XK_z,     ACTION##stack, {.i = 2 } },           \
                        { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */


#define GTKCMD(cmd)                                                            \
  {                                                                            \
    .v = (const char *[]) { "/usr/bin/gtk-launch", cmd, NULL }                 \
  }

// Some easyer shortcuts to use
#define STATUSBAR "dwmblocks"
#define BROWSER   "helium-browser"
#define CHATCLIENT "discord"
#define HOME "/home/atego" // Easy way to change who owns this

// Utility Comands

static char dmenumon[2] = "0"; 
static const char *dmenucmd[] = {"dmenu_run", "-m",  dmenumon,       "-fn",
                                 dmenufont,   "-nb", normbgcolor,    "-nf",
                                 normfgcolor, "-sb", selbordercolor, "-sf",
                                 selfgcolor,  NULL};

// Define What Is Main Terminal
static const char *termcmd[] = {"kitty", NULL};
static const char *stcmd[] = {"/usr/local/bin/st", "-e", "zsh", "-c", "cat ~/.cache/wal/sequences 2>/dev/null; exec zsh", NULL};

// Some utility, spawn app when tag middleclicked
static const Arg tagexec[] = {
    {.v = termcmd}, /* 1 */
    {.v = termcmd}, /* 2 */
    {.v = termcmd}, /* 3 */
    {.v = termcmd}, /* 4 */
    {.v = termcmd}, /* 5 */
    {.v = termcmd}, /* 6 */
    {.v = termcmd}, /* 7 */
    {.v = termcmd}, /* 8 */
    {.v = termcmd}, /* 9 */
};

static const Key keys[] = {
    {MODKEY,                             XK_p,            spawn,            {.v = dmenucmd}},    // Open Dmenu
    {MODKEY,                             XK_Return,       spawn,            {.v = termcmd}},     // Spawn Terminal (this config is kitty)
    {MODKEY | ShiftMask,                 XK_Return,       spawn,            {.v = stcmd}},       // Spawn ST
    {MODKEY | ControlMask,               XK_b,            togglebar,        {0}},                // Toggle Top Bar
    STACKKEYS(MODKEY, focus) STACKKEYS(MODKEY | ShiftMask, push)
    {MODKEY | ShiftMask,                 XK_i,            incnmaster,       {.i = +1}},          // Increrase master windows
    {MODKEY | ControlMask,               XK_i,            incnmaster,       {.i = -1}},          // Decrerase Master windows
    {MODKEY,                             XK_Tab,          view,             {0}},                // Swap between 2 last visited tab
    {MODKEY,                             XK_h,            setmfact,         {.f = -0.05}},       // decrerase master area
    {MODKEY,                             XK_l,            setmfact,         {.f = +0.05}},       // increrase master area
    {MODKEY,                             XK_0,            view,             {.ui = ~0}},         // View All
    {MODKEY | ShiftMask,                 XK_0,            tag,              {.ui = ~0}},         // View All
    {MODKEY | ControlMask | ShiftMask,   XK_q,            quit,             {1}},                // Refresh Dwm
    {MODKEY | ShiftMask,                 XK_BackSpace,    quit,             {0}},                // Quit Dwm
    {MODKEY,                             XK_q,            killclient,       {0}},                // Kill focused window
    {MODKEY | ShiftMask,                 XK_q,            killclient,       {.ui = 1}},          // Kill unfocused windows
    {MODKEY | ControlMask,               XK_x,            xrdb,             {.v = NULL}},        // Refresh Xrdb Colours
    {MODKEY,                             XK_f,            togglefullscreen, {0}},                // Toggle Fulscreen
//  {MODKEY,                             XK_k,            setlayout,        {.v = &layouts[1]}}, // Tile Layout
//  {MODKEY | ShiftMask | ControlMask,   XK_k,            setlayout,        {.v = &layouts[2]}}, // Monocle Layout
//  {MODKEY | ShiftMask,                 XK_k,            setlayout,        {.v = &layouts[3]}}, // Spiral Layout
//  {MODKEY | ControlMask,               XK_k,            setlayout,        {.v = &layouts[4]}}, // Dwindle Layout
//  {MODKEY | ControlMask,               XK_space,        setlayout,        {0}},                // Default Layout
    {MODKEY | ShiftMask,                 XK_space,        togglefloating,   {0}},                // Floating Window
    {MODKEY,                             XK_space,        zoom,             {0}},                // Zoom?
    {MODKEY | ControlMask,               XK_space,        focusmaster,      {0}},                // Focus on master
    {MODKEY | ControlMask | ShiftMask,   XK_l,            togglesticky,     {0}},                // Make window follow trough tabs
    // Multi monitor control
    {MODKEY,                             XK_bracketright, focusmon,         {.i = -1}},          // idk
    {MODKEY | ShiftMask,                 XK_bracketright, tagmon,           {.i = -1}},          // idk
    {MODKEY,                             XK_bracketleft,  focusmon,         {.i = +1}},          // idk
    {MODKEY | ShiftMask,                 XK_bracketleft,  tagmon,           {.i = +1}},          // idk
    // Vanity gaps control
    {MODKEY,                             XK_g,            incrgaps,         {.i = -3}},          // Decrerase size of gaps
    {MODKEY | ShiftMask,                 XK_g,            incrgaps,         {.i = +3}},          // Increrase size of gaps
    // Tag Keys
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
            TAGKEYS(XK_9, 8)
    // Toggle dwblocks
    {MODKEY | ControlMask,  XK_t,     togglebarstatus, {0}},
    // Specific app keybinds
    {MODKEY,                           XK_m,     spawn, {.v = (const char *[]) {"kitty", "-e", "rmpc", NULL}}}, // MUSIC PLAYER
    {MODKEY,                           XK_a,     spawn, {.v = (const char *[]) {"kitty", "-e", "anipy-cli", NULL}}}, // ANIME PLAYER
    {MODKEY,                           XK_b,     spawn, {.v = (const char *[]) {BROWSER, NULL}}}, // BROWSER
    {MODKEY,                           XK_d,     spawn, {.v = (const char *[]) {"vesktop", NULL}}}, // DISCORD
    {MODKEY | ShiftMask,               XK_d,     spawn, {.v = (const char *[]) {"/bin/bash", HOME "/.config/scripts/app-players/yt-music-tool", NULL}}}, // MUSIC DATA EDDITOR
    {MODKEY | ShiftMask,               XK_b,     spawn, {.v = (const char *[]) {"kitty", "-e", "btop", NULL}}}, // STATISTIC SCREEN
    {MODKEY,                           XK_c,     spawn, {.v = (const char *[]) {HOME "/.config/scripts/custom-helpers/cal-check", NULL}}}, // CALLENDAR CHECKER
    {MODKEY | ShiftMask,               XK_n,     spawn, {.v = (const char *[]) {"kitty", "-e", "nvim", NULL}}}, // NVIM
    {MODKEY,                           XK_n,     spawn, {.v = (const char *[]) {"zennotes", NULL}}}, // ZEN NOTES
    {MODKEY | ShiftMask,               XK_f,     spawn, {.v = (const char *[]) {"nautilus", NULL}}}, // NAUTILOUS
    {MODKEY | ControlMask | ShiftMask, XK_b,     spawn, {.v = (const char *[]) {"foliate", NULL}}}, // BOOK READER
    {MODKEY,                           XK_y,     spawn, {.v = (const char *[]) {HOME "/.config/scripts/app-players/ytplay-launcher", NULL}}},  // YOUTUBE PLAYER
    {MODKEY | ShiftMask,               XK_r,     spawn, {.v = (const char *[]) {HOME "/.config/scripts/audio-video/screenrecord", "toggle", NULL}}}, // SCREEN RECORD
    {MODKEY | ControlMask,             XK_a,     spawn, {.v = (const char *[]) {HOME "/.config/scripts/audio-video/audiorecording", "toggle", NULL}}}, // AUDIO RECORD
    {MODKEY | ShiftMask,               XK_w,     spawn, {.v = (const char *[]) {HOME "/.config/scripts/images-photos-wallpapers/gif-test", NULL}}}, // WALLPAPER PICKER
    {MODKEY | ControlMask | ShiftMask, XK_w,     spawn, {.v = (const char *[]) {"onlyoffice-desktopeditors", NULL}}}, // MS OFFICE
    {MODKEY,                           XK_z,     spawn, {.v = (const char *[]) {"zeditor", NULL}}}, // ZED EDITOR
    {MODKEY,                           XK_t,     spawn, {.v = (const char *[]) {HOME "/.config/scripts/system/toggle-kitty-opacity", NULL}}}, // KITTY TRANSPARENCY
    {MODKEY | ShiftMask,               XK_t,     spawn, {.v = (const char *[]) {HOME "/.config/scripts/system/trackpad-toggle", NULL}}}, // TRACKPAD TOGGLE
    {MODKEY | ShiftMask,               XK_m,     spawn, {.v = (const char *[]) {"prismlauncher", NULL}}}, // PRISMLAUNCHER
    {MODKEY | ControlMask,             XK_j,     spawn, {.v = (const char *[]) {HOME "/.config/scripts/audio-video/cam.sh", "--view", NULL}}}, // CAMERA PREVIEW
    {MODKEY,                           XK_Shift_R, spawn, {.v = (const char *[]){HOME "/.config/scripts/system/powermenu", NULL}}}, // BOOT MENU
    {MODKEY | ShiftMask | ControlMask, XK_s,     spawn, {.v = (const char *[]) { "steam", NULL}}}, // STEAM
    {MODKEY,                           XK_BackSpace, spawn, {.v = (const char *[]){HOME "/.config/scripts/system/lock", NULL}}}, // LOCK SCREEN
    // Special Function keys
    {0, XF86XK_AudioMute,             spawn, SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle; pkill -RTMIN+10 dwmblocks")},
    {0, XF86XK_AudioLowerVolume,      spawn, SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5%; pkill -RTMIN+10 dwmblocks")},
    {0, XF86XK_AudioRaiseVolume,      spawn, SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5%; pkill -RTMIN+10 dwmblocks")},
    {0, XF86XK_AudioMicMute,          spawn, SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle; pkill -RTMIN+10 dwmblocks")},
    {0, XF86XK_Tools,                 spawn, SHCMD("kitty -e rmpc") },
    {0, XF86XK_AudioStop,             spawn, SHCMD("mpc stop; pkill -RTMIN+11 dwmblocks") },
    {0, XF86XK_AudioPrev,             spawn, SHCMD("mpc prev; pkill -RTMIN+11 dwmblocks") },
    {0, XF86XK_AudioPlay,             spawn, SHCMD("mpc toggle; pkill -RTMIN+11 dwmblocks") },
    {0, XF86XK_AudioNext,             spawn, SHCMD("mpc next; pkill -RTMIN+11 dwmblocks") },
    {0, XF86XK_Mail,                  spawn, SHCMD("helium-browser https://mail.google.com") },
    // Backup Keybinds for special function keys
    {MODKEY, XK_F9,             spawn, SHCMD("helium-browser https://mail.google.com") },
    {MODKEY, XK_F2,             spawn, {.v = (const char *[]) {HOME "/.config/scripts/images-photos-wallpapers/screenshot-minimal.sh", NULL}}}, // SCREENSHOT

    // Laptop Brightness Utility
    {0, XF86XK_MonBrightnessUp,       spawn, SHCMD("brightnessctl set +1%")},
    {0, XF86XK_MonBrightnessDown,     spawn, SHCMD("brightnessctl set 1%-")},
    // Screenshot Binds
    {MODKEY | ShiftMask,               XK_s,  spawn, {.v = (const char *[]) {HOME "/.config/scripts/images-photos-wallpapers/screenshot-minimal.sh", NULL}}}, // SCREENSHOT
    {MODKEY | ControlMask,             XK_s,  spawn, {.v = (const char *[]) {HOME "/.config/scripts/images-photos-wallpapers/screenshot.sh", NULL}}}, // SPECIFIC SCREENSHOT
    // Input - Output binds
    {MODKEY,                XK_o,     spawn, {.v = (const char *[]) {HOME "/.config/scripts/audio-video/audio-switch", "output", NULL}}},  //AUDIO OUTPUT, swap speaker
    {MODKEY,                XK_i,     spawn, {.v = (const char *[]) {HOME "/.config/scripts/audio-video/audio-switch", "input", NULL}}},  //AUDIO INPUT, swap mic
};

// Button Definitions
// From Here on i have no idea

/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
/* click                event mask           button          function argument */ 
/* Location        Modifier   Button     Function   Argument */
#ifndef __OpenBSD__
    {ClkWinTitle,  0,          Button2,        zoom,         {0}},
    {ClkStatusText,0,          Button1,        sigstatusbar, {.i = 1}},
    {ClkStatusText,0,          Button2,        sigstatusbar, {.i = 2}},
    {ClkStatusText,0,          Button3,        sigstatusbar, {.i = 3}},
    {ClkStatusText,0,          Button4,        sigstatusbar, {.i = 4}},
    {ClkStatusText,0,          Button5,        sigstatusbar, {.i = 5}},
    {ClkStatusText,ShiftMask,  Button1,        sigstatusbar, {.i = 6}},
#endif
    {ClkStatusText,ShiftMask,  Button3,        spawn,
     SHCMD("st -e nvim ~/.local/src/dwmblocks/blocks.h")},
    {ClkClientWin, MODKEY,     Button1,        movemouse,    {0}},      /* left */
    {ClkClientWin, MODKEY,     Button2,        defaultgaps,  {0}},      /* middle */
    {ClkClientWin, MODKEY,     Button3,        resizemouse,  {0}},      /* right */
    {ClkClientWin, MODKEY,     Button4,        incrgaps,     {.i = +1}},/* scroll up */
    {ClkClientWin, MODKEY,     Button5,        incrgaps,     {.i = -1}},/* scroll down */
    {ClkTagBar,    0,          Button1,        view,         {0}},
    {ClkTagBar,    0,          Button3,        toggleview,   {0}},
    {ClkTagBar,    MODKEY,     Button1,        tag,          {0}},
    {ClkTagBar,    MODKEY,     Button3,        toggletag,    {0}},
    {ClkRootWin,   0,          Button2,        togglebar,    {0}}, /* hide bar */
};
