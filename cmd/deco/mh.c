# include "scr.h"
# include "dir.h"
# include "deco.h"
# include "menu.h"

/* menu handler */

extern setfull (), setdwid (), reread (), setstatus (), directory ();
extern genhelp (), view (), edit (), quit (), setpattern (), makelink ();
extern swappanels (), switchpanels (), shell (), usermenu (), cmpdir ();
extern switchcmdreg (), fullscreen (), delete (), findname ();
extern copy (), renmove (), makedir (), histmenu (), VRedraw ();
extern setshellname (), setviewname (), seteditname (), writeinitfile ();
extern switchalign (), setsort (), setrevsort (), settypesort ();
extern tagall (), untagall (), menuedit (), extedit ();

# ifdef S_IFLNK
extern makeslink ();
# endif

static struct smenu lmenu [] = {
	{       "Brief",                'b',    setfull,        },
	{       "Long",                 'l',    setfull,        },
	{       "Full",                 'f',    setfull,        },
	{       "Status           ^L",  's',    setstatus,      },
	{       "Align extensions",     'a',    switchalign,    },
	{       "",                     0,      0,              },
	{       "Sort by name",         'n',    setsort,        },
	{       "Extension",            'x',    setsort,        },
	{       "Time",                 't',    setsort,        },
	{       "Size",                 'z',    setsort,        },
	{       "Unsorted",             'u',    setsort,        },
	{       "Reverse",              'r',    setrevsort,     },
	{       "Sort by type",         'y',    settypesort,    },
	{       "",                     0,      0,              },
	{       "Re-read          ^R",  'r',    reread,         },
	{       "Pattern",              'p',    setpattern,     },
	{       0,                      0,      0,              },
};

static struct smenu rmenu [] = {
	{       "Brief",                'b',    setfull,        },
	{       "Long",                 'l',    setfull,        },
	{       "Full",                 'f',    setfull,        },
	{       "Status           ^L",  's',    setstatus,      },
	{       "Align extensions",     'a',    switchalign,    },
	{       "",                     0,      0,              },
	{       "Sort by name",         'n',    setsort,        },
	{       "Extension",            'x',    setsort,        },
	{       "Time",                 't',    setsort,        },
	{       "Size",                 'z',    setsort,        },
	{       "Unsorted",             'u',    setsort,        },
	{       "Reverse",              'r',    setrevsort,     },
	{       "Sort by type",         'y',    settypesort,    },
	{       "",                     0,      0,              },
	{       "Re-read          ^R",  'r',    reread,         },
	{       "Pattern",              'p',    setpattern,     },
	{       0,                      0,      0,              },
};

static struct smenu fmenu [] = {
	{       "Help             F1",  'h',    genhelp,        },
	{       "User menu        F2",  'u',    usermenu,       },
	{       "View             F3",  'v',    view,           },
	{       "Edit             F4",  'e',    edit,           },
	{       "Copy             F5",  'c',    copy,           },
	{       "Link",                 'l',    makelink,       },
# ifdef S_IFLNK
	{       "Symbol link",          's',    makeslink,      },
# endif
	{       "Rename/move      F6",  'r',    renmove,        },
	{       "Make directory   F7",  'm',    makedir,        },
	{       "Delete           F8",  'd',    delete,         },
	{       "",                     0,      0,              },
	{       "Select group     +",   'g',    tagall,         },
	{       "Unselect group   -",   'u',    untagall,       },
	{       "",                     0,      0,              },
	{       "Quit             F10", 'q',    quit,           },
	{       0,                      0,      0,              },
};

static struct smenu cmenu [] = {
	{       "Run subshell",                 's',    shell,          },
	{       "Compare directories",          'c',    cmpdir,         },
	{       "Find file            ^K",      'f',    findname,       },
	{       "History              ^B",      'i',    histmenu,       },
	{       "",                             0,      0,              },
	{       "Home directory       ^\\",     'o',    directory,      },
	{       "Root directory       ^^",      'r',    directory,      },
	{       "Redraw screen        ^]",      'd',    VRedraw,        },
	{       "",                             0,      0,              },
	{       "Menu file edit",               'm',    menuedit,       },
	{       "Extensions file edit",         'x',    extedit,        },
	{       0,                              0,      0,              },
};

static struct smenu omenu [] = {
	{       "Switch panels      ^I",        'w',    switchpanels,   },
	{       "Swap panels        ^U",        's',    swappanels,     },
	{       "Full screen        ^F",        'f',    fullscreen,     },
	{       "Double width       ^W",        'd',    setdwid,        },
	{       "Command line mode  ^P",        'c',    switchcmdreg,   },
	{       "",                     0,      0,              },
	{       "Viewer...",                    'v',    setviewname,    },
	{       "Editor...",                    'e',    seteditname,    },
	{       "Shell...",                     'h',    setshellname,   },
	{       "",                     0,      0,              },
	{       "Save setup",                   'a',    writeinitfile,  },
	{       0,                              0,      0,              },
};

struct menu menu [] = {
	{       "Left",         'l',            lmenu,  },
	{       "Files",        'f',            fmenu,  },
	{       "Commands",     'c',            cmenu,  },
	{       "Options",      'o',            omenu,  },
	{       "Right",        'r',            rmenu,  },
	{       0,              0,              0,      },
};

int nmenu;
extern useredit, userview, usecshell;

runmenu (mkey)
{
	register struct menu *m;
	register struct smenu *sm;
	int ret;

	nmenu = 0;
	for (m=menu; m->mname; ++m) {
		if (m->key == mkey)
			nmenu = m-menu;
		for (sm=m->submenu; sm->sname; ++sm)
			sm->active = sm->exec != 0;
		m->nsm = -1;
		downmenu (m);
	}
oncemore:
	for (m=menu; m->mname; ++m) {
		for (sm=m->submenu; sm->sname; ++sm) {
			sm->active = sm->exec != 0;
			sm->stag = tag (m->key, sm->key);
		}
	}
	ret = getmenu ();
	if (! ret)
		return;
	m = &menu[nmenu];
	sm = &m->submenu[m->nsm];
	if (sm->exec)
		(*sm->exec) (m->key, sm->key);
	if (ret > 1)
		goto oncemore;
}

tag (k, sk)
{
	switch (k) {
	case 'l':
		switch (sk) {
		case 'b':       return (left.full==0);
		case 'l':       return (left.full==1 || !fullwin && left.full==2);
		case 'f':       return (fullwin && left.full==2);
		case 's':       return (right.status);
		case 'p':       return (left.pattern[0]);
		case 'n':       return (left.sort == SORTNAME);
		case 'x':       return (left.sort == SORTEXT);
		case 't':       return (left.sort == SORTTIME);
		case 'z':       return (left.sort == SORTSIZE);
		case 'u':       return (left.sort == SORTSKIP);
		case 'a':       return (left.alignext);
		case 'y':       return (left.typesort);
		case 'r':       return (left.revsort);
		}
		break;
	case 'r':
		switch (sk) {
		case 'b':       return (right.full==0);
		case 'l':       return (right.full==1 || !fullwin && right.full==2);
		case 'f':       return (fullwin && right.full==2);
		case 's':       return (left.status);
		case 'p':       return (right.pattern[0]);
		case 'n':       return (right.sort == SORTNAME);
		case 'x':       return (right.sort == SORTEXT);
		case 't':       return (right.sort == SORTTIME);
		case 'z':       return (right.sort == SORTSIZE);
		case 'u':       return (right.sort == SORTSKIP);
		case 'a':       return (right.alignext);
		case 'y':       return (right.typesort);
		case 'r':       return (right.revsort);
		}
		break;
	case 'o':
		switch (sk) {
		case 's':       return (left.basecol != 0);
		case 'f':       return (H == LINES-7);
		case 'h':       return (usecshell);
		case 'd':       return (fullwin);
		case 'c':       return (cmdreg);
		case 'v':       return (userview);
		case 'e':       return (useredit);
		}
		break;
	}
	return (0);
}
