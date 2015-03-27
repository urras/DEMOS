# include "scr.h"
# include "menu.h"

static drawhead (), clrmenu (), drawmenu (), upmenu ();

static initmenu ()
{
	register struct menu *m;
	register struct smenu *sm;
	int col, w;

	col = 3;
	for (m=menu; m->mname; ++m) {
		m->len = 4 + strlen (m->mname);
		m->col = col;
		col += m->len;
		m->hgh = 0;
		m->wid = 0;
		for (sm=m->submenu; sm->sname; ++sm) {
			sm->row = m->hgh;
			w = 4 + strlen (sm->sname);
			if (w > m->wid)
				m->wid = w;
			++m->hgh;
		}
	}
}

getboxes ()
{
	register struct menu *m;

	for (m=menu; m->mname; ++m) {
		if (m->box)
			VFreeBox (m->box);
		m->box = VGetBox (1, m->col-1, m->hgh+3, m->wid+4);
	}
}

getmenu ()
{
	if (! menu[0].len)
		initmenu ();

	getboxes ();
	VMPutString (LINES-1, 0, "\0011\16      \17 2\16      \17 3\16      \17 4\16      \17 5\16      \17 6\16      \17 7\16      \17 8\16      \17 9\16      \01710\16Quit \17\2");
	for (;;) {
		drawhead (nmenu);
		for (;;) {
			drawmenu (&menu[nmenu]);
			hidecursor ();
			VSync ();
			switch (KeyGet ()) {
			default:
				VBeep ();
				continue;
			case cntrl (']'):          /* redraw screen */
				VRedraw ();
				continue;
			case cntrl ('M'):
				clrmenu (&menu[nmenu]);
				return (1);
			case cntrl ('J'):
				clrmenu (&menu[nmenu]);
				return (2);
			case cntrl ('C'):
			case cntrl ('['):
			case meta ('J'):        /* f0 */
				clrmenu (&menu[nmenu]);
				return (0);
			case meta ('r'):        /* right */
				clrmenu (&menu[nmenu]);
				if (! menu[++nmenu].mname)
					nmenu = 0;
				break;
			case meta ('l'):        /* left */
				clrmenu (&menu[nmenu]);
				if (--nmenu < 0) {
					for (nmenu=0; menu[nmenu].mname; ++nmenu);
					--nmenu;
				}
				break;
			case meta ('u'):        /* up */
				upmenu (&menu[nmenu]);
				continue;
			case meta ('d'):        /* down */
				downmenu (&menu[nmenu]);
				continue;
			}
			break;
		}
	}
}

static drawhead (nm)
{
	register i, reverse;

	VSetDim ();
	reverse = VStandOut ();
	VMove (0, 0);
	for (i=0; i<80; ++i)
		VPutChar (' ');
	VMove (0, 2);
	for (i=0; menu[i].mname; ++i) {
		if (i == nm) {
			VStandEnd ();
			VSetBold ();
		}
		VPutChar (' ');
		VPutChar (i==nm && !reverse ? '[' : ' ');
		VPutString (menu[i].mname);
		VPutChar (i==nm && !reverse ? ']' : ' ');
		VPutChar (' ');
		if (i == nm) {
			VStandOut ();
			VSetDim ();
		}
	}
	VSetNormal ();
	VStandEnd ();
}

static clrmenu (m)
register struct menu *m;
{
	VUngetBox (m->box);
}

static drawmenu (m)
register struct menu *m;
{
	register i, reverse;

	VClearBox (1, m->col-1, m->hgh+3, m->wid+4);
	VSetBold ();
	VDrawBox (1, m->col, m->hgh+2, m->wid+2);
	VSetDim ();
	for (i=0; m->submenu[i].sname; ++i)
		if (! m->submenu[i].sname[0])
			VHorLine (i+2, m->col+2, m->wid-2);
	VSetNormal ();
	for (i=0; m->submenu[i].sname; ++i) {
		if (! m->submenu[i].sname[0])
			continue;
		if (i == m->nsm)
			reverse = VStandOut ();
		else if (! m->submenu[i].active)
			VSetDim ();
		VMove (2+i, m->col+1);
		VPutChar (i==m->nsm && !reverse ? '[' : ' ');
		printw ("%c %-*s", m->submenu[i].stag ? '*' : ' ',
			m->wid-4, m->submenu[i].sname);
		VPutChar (i==m->nsm && !reverse ? ']' : ' ');
		if (i == m->nsm)
			VStandEnd ();
		else if (! m->submenu[i].active)
			VSetNormal ();
	}
}

static upmenu (m)
register struct menu *m;
{
	for (;;) {
		if (--m->nsm < 0) {
			for (m->nsm=0; m->submenu[m->nsm].sname; ++m->nsm);
			--m->nsm;
		}
		if (m->submenu[m->nsm].active)
			break;
	}
}

downmenu (m)
register struct menu *m;
{
	for (;;) {
		if (! m->submenu[++m->nsm].sname)
			m->nsm = 0;
		if (m->submenu[m->nsm].active)
			break;
	}
}
