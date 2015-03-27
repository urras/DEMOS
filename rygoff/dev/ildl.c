/*
 * Вставка-удаление строк переписыванием физ. памяти на
 * видеомониторе Э-85
 *
 * $Log:	ildl.c,v $
 * Revision 1.1  90/01/30  11:34:35  rygoff
 * Initial revision
 * 
 */

#include "video.h"
#if     NVI > 0
#include "param.h"
#include <sys/conf.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/tty.h>
#include <sys/systm.h>
#include <sys/iopage.h>
#include <sys/seg.h>

struct vireg {
	short   vi_rid;         /* регистр идентификации      */
	short   vi_m1;
	short   vi_rks;         /* регистр команд и состояния */
	short   vi_rdc;         /* рег. управления данными B  */
	short   vi_rcol;        /* рег. - / - G и R           */
	short   vi_rmap;        /* карта цветов               */
	short   vi_rsh;         /* регистр сдвига             */
	short   vi_ry;          /* регистр координаты x       */
	short   vi_rx;          /* регистр координаты y       */
	short   vi_rvl;         /* регистр длины вектора      */
	short   vi_rbf;         /* регистр функции яркости    */
	short   vi_rpa;         /* регистр физического адреса */
};

#define VIADDR ((struct vireg *)0174400)
#define PROTO5  ((64<<8) | RW)
#define VIMEM   0160000   /* phys addr in clicks */

int viHSR, viLSR;
int viWy0, viWy1;
int vicolor;
struct tty vi11[1];

vi_il(li)
{
	segm    save5;
	register short *p, *q, *pl;
	short fword, lword;
	short fmask, lmask;
	int cx, cxx;
	int Srdc, Srcol, colctr;

	if( li < viHSR || li > viLSR-1 ) {
		vi_CLRL(li);
		return;
	}

	fword = viWy0    *12;
	lword = (viWy1+1)*12;

	fmask = ~((1<<(fword%16))-1);
	lmask =   (1<<(lword%16))-1;

	fword /= 16;
	lword /= 16;

	if( vicolor ) {
		Srdc  = VIADDR->vi_rdc;
		Srcol = VIADDR->vi_rcol;
		VIADDR->vi_rcol &= ~(040|(040<<8));     /* only B */
	}
	saveseg5(save5);
	for( cx = viLSR-1 ; cx >= li ; cx-- ) {
	    mapseg5((VIMEM + 20*cx), PROTO5);
	    for( cxx = 0 ; cxx < 10 ; cxx++ ) {
		colctr = 0;
OtherColor:
		p = (int *)SEG5 + fword + 64*cxx;
		q  = p + 640;
		pl = p + lword - fword;
		if( fmask != ~0 ) {
			*q &= ~fmask;
			*q++ |= *p++ & fmask;
		}
		while( p < pl )
			*q++ = *p++;
		if( lmask ) {
			*q &= ~lmask;
			*q |= *p & lmask;
		}
		if( vicolor ) {
			switch( colctr++ ) {
			    case 0:     /* B is done, next G */
				VIADDR->vi_rdc  &= ~040;
				VIADDR->vi_rcol |= 040;
				goto OtherColor;
			    case 1:     /* G is done, next R */
				VIADDR->vi_rcol &= ~040;
				VIADDR->vi_rcol |= (040<<8);
				goto OtherColor;
			    case 2:     /* R is done, restore B */
				VIADDR->vi_rcol &= ~(040<<8);
				VIADDR->vi_rdc  |= 040;
			}
		}
	    }
	}
	restorseg5(save5);
	if( vicolor ) {
		VIADDR->vi_rdc  = Srdc;
		VIADDR->vi_rcol = Srcol;
	}
	vi_CLRL(li);
}

vi_dl(li)
{
	segm    save5;
	register short *p, *q, *pl;
	short fword, lword;
	short fmask, lmask;
	int cx, cxx;
	int Srdc, Srcol, colctr;

	if( li < viHSR || li > viLSR-1 ) {
		vi_CLRL(li);
		return;
	}

	fword = viWy0    *12;
	lword = (viWy1+1)*12;

	fmask = ~((1<<(fword%16))-1);
	lmask =   (1<<(lword%16))-1;

	fword /= 16;
	lword /= 16;

	if( vicolor ) {
		Srdc  = VIADDR->vi_rdc;
		Srcol = VIADDR->vi_rcol;
		VIADDR->vi_rcol &= ~(040|(040<<8));     /* only B */
	}
	saveseg5(save5);
	for( cx = li ; cx < viLSR ; cx++ ) {
	    mapseg5((VIMEM + 20*cx), PROTO5);
	    for( cxx = 0 ; cxx < 10 ; cxx++ ) {
		colctr = 0;
OtherColor:
		p = (int *)SEG5 + fword + 64*cxx;
		q  = p + 640;
		pl = p + lword - fword;
		if( fmask != ~0 ) {
			*p &= ~fmask;
			*p++ |= *q++ & fmask;
		}
		while( p < pl )
			*p++ = *q++;
		if( lmask ) {
			*p &= ~lmask;
			*p |= *q & lmask;
		}
		if( vicolor ) {
			switch( colctr++ ) {
			    case 0:     /* B is done, next G */
				VIADDR->vi_rdc  &= ~040;
				VIADDR->vi_rcol |= 040;
				goto OtherColor;
			    case 1:     /* G is done, next R */
				VIADDR->vi_rcol &= ~040;
				VIADDR->vi_rcol |= (040<<8);
				goto OtherColor;
			    case 2:     /* R is done, restore B */
				VIADDR->vi_rcol &= ~(040<<8);
				VIADDR->vi_rdc  |= 040;
			}
		}
	    }
	}
	restorseg5(save5);
	if( vicolor ) {
		VIADDR->vi_rdc  = Srdc;
		VIADDR->vi_rcol = Srcol;
	}
	vi_CLRL(viLSR);
}

vi_RU(ds)
{
	if( viLSR == viHSR ) {
		vi_CLRL(viLSR);
		return;
	}
	vi_dl(viHSR);
	if(ds)
		vi_dl(viHSR);
}

vi_RD(ds)
{
	if( viLSR == viHSR ) {
		vi_CLRL(viLSR);
		return;
	}
	vi_il(viHSR);
	if(ds)
		vi_il(viHSR);
}


short  Oreg, Italic, Iflag, Uflag, DoubleSize, AltFont, Bold;
int viinv, viMODE, viRDC, viR, viG, viB;

/*
 * Стек режимов выделений
 */
#define VISS 5

struct VSV {
	char    vs_italic;
	char    vs_iflag;
	char    vs_uflag;
	char    vs_rdc;
	char    vs_dsize;
	char    vs_altfont;
	int     vs_inv;
	char    vs_bold;
	char    vs_R, vs_G, vs_B;
} vistack[VISS];

static int visptr = 0;

viSave()
{
	register struct VSV *VS;

	if( visptr >= VISS ) visptr--;
	VS = &vistack[visptr++];
	VS->vs_italic = Italic;
	VS->vs_iflag  = Iflag;
	VS->vs_uflag  = Uflag;
	VS->vs_dsize = DoubleSize;
	VS->vs_altfont = AltFont;
	VS->vs_inv    = viinv;
	VS->vs_bold   = Bold;
	VS->vs_R      = viR;
	VS->vs_G      = viG;
	VS->vs_B      = viB;
	VS->vs_rdc    = viMODE | viRDC;
}

viRestore()
{
	register struct VSV *VS;

	if( --visptr < 0 ) visptr = 0;
	VS = &vistack[visptr];
	Italic = VS->vs_italic;
	Iflag  = VS->vs_iflag;
	Uflag  = VS->vs_uflag;
	DoubleSize = VS->vs_dsize;
	AltFont    = VS->vs_altfont;
	viinv  = VS->vs_inv;
	Bold   = VS->vs_bold;
	viR    = VS->vs_R;
	viG    = VS->vs_G;
	viB    = VS->vs_B;
	viRDC  = VS->vs_rdc & ~07;
	viMODE = VS->vs_rdc &  07;
	RDC( viMODE );
}

int vireset, viWx0, viWx1, cshift, CX, CY, Gfill;

/*
 * Сброс видеотерминала
 */
viRESET()
{
	extern GrafText;

	vireset++;
	vi11[0].t_state &= ~(BUSY|TTSTOP|TIMEOUT);
	vi11[0].t_flags |= ECHO|CRMOD;
	vi11[0].t_flags &= ~(RAW|CBREAK);
	vi11[0].t_line   = DFLT_LDISC;
	ttychars( &vi11[0] );
	vi11[0].t_local |= LCRTBS|LCRTERA|LCRTKIL|LCTLECH|LDECCTQ;
	vi11[0].t_local &= ~LLITOUT;
	vi11[0].t_width = 80;
	viRDC = 040;  /* enable phys */
	RDC( viMODE = 02 );
	AltFont = 0;
	DoubleSize = 0;
	Iflag = 0;
	Uflag = 0;
	Italic = 1;
	GrafText = 0;
	viWx0 = viWy0 = 0;
	viWy1 = 79; viWx1 = 23;
	viHSR = 0;
	viLSR = 23;
	cshift = 0;
	CX = 23;
	CY = 0;
	Gfill = 0;
	wakeup( &(vi11[0].t_outq) );
	if( vicolor ) viMapReset();
}

#endif NVI
