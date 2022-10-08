/* See LICENSE file for copyright and license details. */


/* https://gist.github.com/palopezv/efd34059af6126ad970940bcc6a90f2e
 * hardware multimedia keys
 */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;  /* border pixel of windows */
static const unsigned int snap      = 20; /* snap pixel */
static const int showbar            = 1;  /* 0 means no bar */
static const int topbar             = 1;  /* 0 means bottom bar */
static const char *fonts[]          = { "Fira Code:size=14" };
static const char dmenufont[]       = "Fira Code:size=16";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_middlepart[]  = "#2d2d2d";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_middlepart,  col_middlepart },
};

/* tagging */
static const char *tags[] = { "dev", "app", "med", "web", "etc" };
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
  { "Gimp",        NULL,       NULL,       1 << 4,            1,           -1 },
  { "Chromium",    NULL,       NULL,       1 << 3,            0,           -1 },
  { "firefox",     NULL,       NULL,       1 << 3,            0,           -1 },
  { "Signal",      NULL,       NULL,       1 << 1,            0,           -1 },
  { "Tor Browser", NULL,       NULL,       1 << 4,            0,           -1 },
  { "mpv",         NULL,       NULL,           ~0,            1,           -1 },
  { "Joplin",      NULL,       NULL,       1 << 2,            0,           -1 },
  { "code-oss",    NULL,       NULL,       1 << 0,            0,           -1 },
  { "spotify",     NULL,       NULL,       1 << 4,            0,           -1 }, // spotify ignores this rule because the devs dont care to follow the spec :/
  { NULL,          NULL,       "Picture-in-picture",           ~0,            1,           -1 }, // chromium's PiP
  { NULL,          NULL,       "Picture-in-Picture",           ~0,            1,           -1 }, // firefox' PiP
};
/* layout(s) */
static const float mfact     = 0.5;  /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tile resizals */
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]",      tile },    /* first entry is default */
	{ "FL",      NULL },    /* no layout function means floating behavior */
	{ "MN",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-i", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_middlepart, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "kitty", NULL };
//
// CUSTOM
//
// pulseaudio volume
static const char *upvol[]   = { "/home/nils/.scripts/volumectrl", "up", NULL };
static const char *downvol[] = { "/home/nils/.scripts/volumectrl", "down", NULL };
static const char *mutevol[] = { "/home/nils/.scripts/volumectrl", "togglemute", NULL };
// utilmenu
static const char *utilmenu[] = { "/home/nils/.scripts/utilmenu", NULL };
// brightness ctrl
static const char *xblinc[] = { "/usr/bin/xbacklight", "-inc", "2", NULL };
static const char *xbldec[] = { "/usr/bin/xbacklight", "-dec", "2", NULL };
// clipmenu
static const char *clipmenucmd[] = { "/usr/bin/clipmenu", NULL };
// screenshot area
static const char *screenshot_area[] = { "/home/nils/.scripts/screenshot", "area", NULL };
// screenshot all
static const char *screenshot_all[] = { "/home/nils/.scripts/screenshot", "all", NULL };
// infopanel
static const char *infopanel[] = { "/home/nils/.scripts/infopanel", "run", NULL };
// nautilus
static const char *nautilus[] = { "nautilus", "--new-window", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_s,      spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
// i dont even know what they do. i dont use them
//	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
//	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_p,      togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
// only five workspaces
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
//
// CUSTOM
//
// hardware multimedia keys
  { MODKEY,                       XK_F2,     spawn, {.v = downvol } },
  { MODKEY,                       XK_F1,     spawn, {.v = mutevol } },
  { MODKEY,                       XK_F3,     spawn, {.v = upvol } },
  { MODKEY,                       XK_F11,    spawn, {.v = xbldec } },
  { MODKEY,                       XK_F12,    spawn, {.v = xblinc } },
  { MODKEY,                       XK_d,      spawn, {.v = utilmenu } },
  { MODKEY,                       XK_c,      spawn, {.v = clipmenucmd } },
  { 0,                            XK_Print,  spawn, {.v = screenshot_area } },
  { ShiftMask,                    XK_Print,  spawn, {.v = screenshot_all } },
  { MODKEY,                       XK_v,      spawn, {.v = infopanel } },
  { MODKEY,                       XK_a,      spawn, {.v = nautilus } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

