/*
 * unctrl.h
 */

extern	char	*_unctrl[];	/* defined in /usr/src/lib/curses/unctrl.c */

#define	unctrl(ch)	(_unctrl[ch & 0177])
