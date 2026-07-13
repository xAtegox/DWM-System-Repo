/* user and group to drop privileges to */
static const char *user  = "nobody";
static const char *group = "nobody";

/* Font settings for the time text (showtime patch) */
static const float textsize = 64.0;
static const char *textfamily = "serif";
static const double textcolorred = 255;
static const double textcolorgreen = 255;
static const double textcolorblue = 255;
static const char *clockcolor = "#7ab878";

/* DWM logo settings (dwmlogoandblurscreen patch) */
static const int logosize = 75;
static const int logow = 12; /* Grid width and height for right center alignment */
static const int logoh = 6;

static XRectangle rectangles[9] = {
    {0, 3, 1, 3},
    {1, 3, 2, 1},
    {0, 5, 8, 1},
    {3, 0, 1, 5},
    {5, 3, 1, 2},
    {7, 3, 1, 2},
    {8, 3, 4, 1},
    {9, 4, 1, 2},
    {11,4, 1, 2},
};

/* Enable blur */
#define BLUR
/* Set blur radius */
static const int blurRadius = 4;
/* Enable Pixelation (uncomment to enable) */
//#define PIXELATION
/* Set pixelation radius */
static const int pixelSize = 0;

static const char *colorname[NUMCOLS] = {
    [BACKGROUND] = "white",
    [INIT] =   "2d2d2d",    /* after initialization */
    [INPUT] =  "#005577",   /* during input */
    [FAILED] = "#CC3333",   /* wrong password */
    [CAPS] =   "red",       /* CapsLock on */
};

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
    { "color0", STRING, &colorname[INIT] },
    { "color4", STRING, &colorname[INPUT] },
    { "color1", STRING, &colorname[FAILED] },
    { "color3", STRING, &colorname[CAPS] },
    { "color14", STRING, &clockcolor },
};

/* treat a cleared input like a wrong password (color) */
static const int failonclear = 1;
