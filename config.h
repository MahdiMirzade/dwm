/* See LICENSE file for copyright and license details. */

/* Load essentials */
#include <X11/XF86keysym.h>             /* For XF86 multimedia binds */

/* appearance */
static const char *fonts[]              = { "monospace:size=9", "Vazir:size=9" };
static const unsigned int borderpx      = 0;    /* border pixel of windows */
static const unsigned int gappx         = 10;   /* gaps between windows */
static const unsigned int snap          = 32;   /* snap pixel */
static const unsigned int systraypinning= 1;    /* 0: sloppy systray follows selected monitor, 1: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, 1: systray on left of status text */
static const unsigned int systraypadding= 0;	/* systray vertical padding */
static const unsigned int systrayspacing= 2;    /* systray spacing */
static const int systraypinningfailfirst= 1;    /* 1: if pinning fails, display systray on the first monitor, 0: display systray on the last monitor*/
static const int showsystray            = 0;    /* 0 means no systray */
static const int showbar                = 1;    /* 0 means no bar */
static const int topbar                 = 1;    /* 0 means bottom bar */
static const int barheight              = 3;	/* Spacing around the bar's font */
static const int hidevacanttags         = 1;    /* 1 means hide empty tags / 0: show every tag */
static const int hidetagindicator       = 1;    /* 1 means hide default top-left square-shaped tag indicator */
static const int truecenteredtitle      = 0;    /* 1 means center the WM_NAME of the currently selected window on the status bar */
static const unsigned int colorfultag   = 0;    /* 0 means use SchemeSel for selected tag */
static char fgcolor[]                   = "#D8DEE9";
static char bgcolor[]                   = "#2E3440";
static char color0[]                    = "#3B4252";
static char color1[]                    = "#BF616A";
static char color2[]                    = "#A3BE8C";
static char color3[]                    = "#EBCB8B";
static char color4[]                    = "#81A1C1";
static char color5[]                    = "#B48EAD";
static char color6[]                    = "#88C0D0";
static char color7[]                    = "#E5E9F0";
static char color8[]                    = "#4C566A";
static char color9[]                    = "#BF616A";
static char color10[]                   = "#A3BE8C";
static char color11[]                   = "#EBCB8B";
static char color12[]                   = "#81A1C1";
static char color13[]                   = "#B48EAD";
static char color14[]                   = "#8FBCBB";
static char color15[]                   = "#ECEFF4";
static const char *colors[][3]          = {
        /*                  fg          bg          border   */
        [SchemeNorm]    = { color12,    color0,     color8 },
        [SchemeSel]     = { color0,   	color12,    color12 },
        [SchemeLayout]  = { color12,   	color0,     color8 },
};

/* if colorfultag == 1: */
static const char *colortags[][2] = {
        { color1,  color0 },
        { color2,  color0 },
        { color3,  color0 },
        { color4,  color0 },
        { color5,  color0 },
        { color6,  color0 },
        { color9,  color0 },
        { color10, color0 },
        { color11, color0 },
};
//static const char *colortags_sel[][2] = {
//        { color1,  color8 },
//        { color2,  color8 },
//        { color3,  color8 },
//        { color4,  color8 },
//        { color5,  color8 },
//        { color6,  color8 },
//        { color9,  color8 },
//        { color10, color8 },
//        { color11, color8 },
//};
static const char *colortags_sel[][2] = {
        { color0, color1  },
        { color0, color2  },
        { color0, color3  },
        { color0, color4  },
        { color0, color5  },
        { color0, color6  },
        { color0, color9  },
        { color0, color10 },
        { color0, color11 },
};

/* tagging */
static const char *tags[] = { "𐎠", "𐎡", "𐎢", "𐎣", "𐎤", "𐎥", "𐎦", "𐎧", "𐎨" };
//static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class        instance    title           tags mask   isfloating  monitor */
	//{ "mpv",        NULL,       NULL,           0,          1,          -1 },
	//{ "Gimp",       NULL,       NULL,           0,          1,          -1 },
	//{ "Firefox",    NULL,       NULL,           1 << 8,     0,          -1 },
        { "st-float",   NULL,       NULL,           0,          1,          -1 },
        { NULL,		"st-float", NULL,           0,          1,          -1 },
	{ NULL,         NULL,       "Event Tester", 0,          0,          -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.5; 	/* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    	/* number of clients in master area */
static const int resizehints = 1;    	/* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; 	/* 1 will force focus on the fullscreen window */

//static const Layout layouts[] = {
//	/* symbol           arrange function */
//        { "Tile",           tile },    /* first entry is default */
//        { "Monocle",        monocle },
//        { "Float",          NULL },    /* no layout function means floating behavior */
//        { NULL,             NULL },
//};

static const Layout layouts[] = {
	/* symbol       arrange function */
        { "[]=",        tile },    /* first entry is default */
        { "[M]",        monocle },
        { "><>",        NULL },    /* no layout function means floating behavior */
        { NULL,         NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTMOD Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
        { ALTMOD,                       KEY,      focusnthmon,    {.i  = TAG } }, \
        { ALTMOD|ShiftMask,             KEY,      tagnthmon,      {.i  = TAG } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-i", "-p", "Run:", NULL };
//static const char *termcmd[]  = { "st", NULL };
//static const char *termcmdfloat[]  = { "st", "-c", "st-float", NULL };
static const char *termcmd[]  = { "tabbed", "-cd", "-r", "2", "st", "-w", "''", NULL };
static const char *termcmdfloat[]  = { "tabbed", "-cd", "-n", "st-float", "-r", "2", "st", "-w", "''", NULL };

static Key keys[] = {
	/* modifier                     key         function        argument */
	{ MODKEY,                       XK_a,       spawn,          SHCMD("dmenu_drun") },
	{ MODKEY,                       XK_c,       spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return,  spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_Return,  spawn,          {.v = termcmdfloat } },
	{ MODKEY|ShiftMask,             XK_b,       spawn,          SHCMD("dwm_bar -M") },
	{ MODKEY,                       XK_b,       togglebar,      {0} },
	{ MODKEY,                       XK_j,       focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,       focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,       incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,       incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,       setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,       setmfact,       {.f = +0.05} },
        { MODKEY|ShiftMask,             XK_j,       movestack,      {.i = +1 } },
        { MODKEY|ShiftMask,             XK_k,       movestack,      {.i = -1 } },
	{ MODKEY,                       XK_space,   zoom,           {0} },
	{ MODKEY,                       XK_Tab,     view,           {0} },
	{ MODKEY,                       XK_t,       setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_m,       setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_f,       setlayout,      {.v = &layouts[2]} },
        { MODKEY|ControlMask,           XK_comma,   cyclelayout,    {.i = -1 } },
        { MODKEY|ControlMask,           XK_period,  cyclelayout,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_space,   togglefloating, {0} },
        { MODKEY|ShiftMask,             XK_f,       togglefullscr,  {0} },
	{ MODKEY,                       XK_s,       togglesticky,   {0} },
	{ MODKEY,                       XK_0,       view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,       tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,   focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,  focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,   tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,  tagmon,         {.i = +1 } },
        { MODKEY,                       XK_F5,      xrdb,           {.v = NULL } },
        { MODKEY,                       XK_minus,   setgaps,        {.i = -1 } },
        { MODKEY,                       XK_equal,   setgaps,        {.i = +1 } },
        { MODKEY|ShiftMask,             XK_equal,   setgaps,        {.i = 0  } },
        { MODKEY|ControlMask,           XK_minus,   shiftview,      {.i = -1 } },
        { MODKEY|ControlMask,           XK_equal,   shiftview,      {.i = +1 } },
	TAGKEYS(                        XK_1,                       0)
	TAGKEYS(                        XK_2,                       1)
	TAGKEYS(                        XK_3,                       2)
	TAGKEYS(                        XK_4,                       3)
	TAGKEYS(                        XK_5,                       4)
	TAGKEYS(                        XK_6,                       5)
	TAGKEYS(                        XK_7,                       6)
	TAGKEYS(                        XK_8,                       7)
	TAGKEYS(                        XK_9,                       8)
	{ MODKEY|ShiftMask,             XK_r,       restart,        {0} },
	{ MODKEY,			XK_q,       killclient,     {0} },
	{ MODKEY|ControlMask,           XK_q,       quit,           {0} },
	{ MODKEY|ShiftMask,             XK_q,       spawn,          SHCMD("powermenu") },
        { MODKEY,                       XK_r,       spawn,          SHCMD("dmenurecord") },
        { MODKEY,                       XK_w,       spawn,          SHCMD("surf-open") },
        { MODKEY|ControlMask,           XK_w,       spawn,          SHCMD("surf") },
        { MODKEY|ShiftMask,             XK_m,       spawn,          SHCMD("dmenu_man") },
        { MODKEY|ShiftMask,             XK_w,       spawn,          SHCMD("st -c \"st-float\" -g \"100x20\" -e sh -c \"SET_BG_SELECT=1 set-bg\"") },
        { MODKEY|ShiftMask,             XK_u,       spawn,          SHCMD("update") },
        { MODKEY|ShiftMask,             XK_t,       spawn,          SHCMD("set-theme -s") },
        { MODKEY|ShiftMask,             XK_g,       spawn,          SHCMD("gtt") },
        { MODKEY|ShiftMask,             XK_y,       spawn,          SHCMD("myt -d") },
        { MODKEY|ControlMask,           XK_l,       spawn,          SHCMD("lock") },
        { MODKEY,                       XK_grave,   spawn,          SHCMD("emoji insert") },
        { ControlMask,                  XK_grave,   spawn,          SHCMD("emoji clipboard") },
        { 0,                            XK_Print,   spawn,          SHCMD("screenshot") },
        { ShiftMask,                    XK_Print,   spawn,          SHCMD("screenshot -t 5") },
        { ControlMask,                  XK_Print,   spawn,          SHCMD("screenshot -s yes") },
        { ControlMask|ShiftMask,        XK_Print,   spawn,          SHCMD("screenshot -s yes -t 5") },
        { 0,            XF86XK_WebCam,              spawn,          SHCMD("webcam") },
        { 0,            XF86XK_AudioPlay,           spawn,          SHCMD("media-controller toggle") },
        { 0,            XF86XK_AudioPause,          spawn,          SHCMD("media-controller toggle") },
        { 0,            XF86XK_AudioNext,           spawn,          SHCMD("media-controller next") },
        { 0,            XF86XK_AudioPrev,           spawn,          SHCMD("media-controller prev") },
        { ShiftMask,    XF86XK_AudioPlay,           spawn,          SHCMD("media-controller pause-all") },
        { ShiftMask,    XF86XK_AudioPause,          spawn,          SHCMD("media-controller pause-all") },
        { ShiftMask,    XF86XK_AudioNext,           spawn,          SHCMD("media-controller seek-fwd") },
        { ShiftMask,    XF86XK_AudioPrev,           spawn,          SHCMD("media-controller seek-bwd") },
        { 0,            XF86XK_MonBrightnessUp,     spawn,          SHCMD("bl -i") },
        { 0,            XF86XK_MonBrightnessDown,   spawn,          SHCMD("bl -d") },
        { 0,            XF86XK_AudioRaiseVolume,    spawn,          SHCMD("volume up") },
        { 0,            XF86XK_AudioLowerVolume,    spawn,          SHCMD("volume down") },
        { 0,            XF86XK_AudioMute,           spawn,          SHCMD("volume mute") },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	/* placemouse options, choose which feels more natural:
	 *    0 - tiled position is relative to mouse cursor
	 *    1 - tiled postiion is relative to window center
	 *    2 - mouse pointer warps to window center
	 *
	 * The moveorplace uses movemouse or placemouse depending on the floating state
	 * of the selected client. Set up individual keybindings for the two if you want
	 * to control these separately (i.e. to retain the feature to move a tiled window
	 * into a floating position).
	 */
	{ ClkClientWin,         MODKEY,         Button1,        moveorplace,    {.i = 1} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
        { ClkTagBar,            0,              Button4,        shiftview,      {.i = -1} },
        { ClkTagBar,            0,              Button5,        shiftview,      {.i = +1} },
        { ClkLtSymbol,          0,              Button1,        cyclelayout,    {.i = +1} },
        { ClkLtSymbol,          0,              Button3,        cyclelayout,    {.i = -1} },
        { ClkLtSymbol,          0,              Button2,        setlayout,      {.v = &layouts[0]} },
        { ClkLtSymbol,          0,              Button4,        cyclelayout,    {.i = -1} },
        { ClkLtSymbol,          0,              Button5,        cyclelayout,    {.i = +1} },
	{ ClkWinTitle,          0,              Button2,        togglesticky,   {0} },
        { ClkStatusText,        0,              Button3,        spawn,          SHCMD("dwm_bar -M") },
};

