/*
 * Процедуры вывода на печать
 *  $Log:	lprf.c,v $
 * Revision 1.2  90/12/12  17:12:12  korotaev
 * Правки под ДВК (my and dw).
 * 
 * Revision 1.1  87/05/13  19:04:21  alex
 * Initial revision
 * 
 *
 *
 */
#define LPLCASE
#include "../../include/lpreg.h"
struct  lpdevice  *LPADDR = {0177514};
static int outrus;
#define isalpha(c) (c>='a' && c<='z' || c>='A' && c<='Z' || (unsigned)c>=0300&&(unsigned)c<=0376)
#define islat(c) (c>='a' && c<='z' || c>='A' && c<='Z' )
#define iscirill(c) ((unsigned)c>=0340&&(unsigned)c<=0376)
#define islower(c) (c>='a' && c<='z' || (unsigned)c>=0300&&(unsigned)c<=0337)
#define toupper(c) ((c)^040)

#define CLAT 017
#define CRUS 016
lputchar(c)
register c;
{
	register s;
	register unsigned timo;
	c &= 0377;
	timo = 60000;
	/*
	 * Ждать готовности консоли (но в течение разумного  времени)
	 */
	while((LPADDR->lpcs&LP_RDY) == 0)
		if(--timo == 0)
			break;
	if(c == 0)
		return;
/* Обработка регистров вывода (lcase, cyrill) */
#ifdef LPLCASE
	if(islower(c)) c = toupper(c);
#endif
	if(iscirill(c)&&!outrus) {lputchar(CRUS); }
	if(islat(c)&&outrus) {lputchar(CLAT); }
	if(c==CLAT) outrus=0;
	if(c==CRUS) outrus=1;
	s = LPADDR->lpcs;
	LPADDR->lpcs = 0;
	LPADDR->lpdb = c;
	if(c == '\n') {
		lputchar('\r');
		lputchar(0200);
		lputchar(0200);
		lputchar(0200);
		lputchar(0200);
	}
	lputchar(0);
	LPADDR->lpcs = s;
}

