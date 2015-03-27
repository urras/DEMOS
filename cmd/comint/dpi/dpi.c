/* ����� �������� ��� ������ � ���������� � �������� ������ */

#ifdef SYSV
#   include <termio.h>
#else
#   include <sgtty.h>
#endif
#include <signal.h>
#include "tty_descr.h"
#include <stdio.h>

extern char *getenv();
long         lseek();

#if ('�'&037) == ('A'&037)
#   define koi8
#endif

#ifdef SYSV
#    define index strchr
#else
#    define xread read
#endif

#undef c
#undef s
#undef e

#define                  maxbuf 2048      /* ������ ������ ������ */
/*  � ������ ������ ������������� �������� ���������,
    � �� ��� - ����� ��� ������   */
static union {
    char            _buf[maxbuf];         /* ����� ����� */
    struct s_tdescr _t;                   /* ��������� ����. ��������� */
} _u;
#define t _u._t

static char     *buf  = _u._buf;       /* ����� ��� ������ */
static int      lbuf = maxbuf;        /* ������ ������ ������ */
static int      xbuf;                 /* ������ ������ � ����� */

static int      grreg;                /* ���� ������������ ������ */
static int      dpreg=0;              /* ����: ���� �� ������������� */

#ifdef BSD
    static struct sgttya old, new;          /* ��������� ��������� */
#else
#   ifdef SYSV
        static struct termio old, new;
/*      static struct tchars tcold, tcnew;  /* ����������� ������� */
#   else
        static struct sgttyb old, new;      /* ��������� ��������� */
#   endif
#endif


/* ������������� ���������� ������������ ������ */

static grbeg() {
    char c, *p;

    if( !grreg ) {
        grreg = 1;
        for( p = t.gr_beg; c = *p++; ) dpcodout( c );
    }
}

static grend() {
    char c, *p;

    if( grreg > 0 ) {
        grreg = 0;
        for( p = t.gr_end; c = *p++; ) dpcodout( c );
    }
}

/* ������������� */
/* ��������������� �������� ���������, ���������� �����,
   ����������� dpi-���� ��������� */

static onintr() { dpend();  exit(0); }

dpinit() {
    unsigned u;
    char    *p;
    static   int      (*fsig)();
    extern int (*signal())();

    tcinit(&t);
    u = sizeof t;
    lbuf -= u;  buf += u;       /* ����� ������ ������ ������ */

#ifdef BSD
    ioctl(0,TIOCGETA,&old);               /* �������� ������ ����. */
    new = old;
    new.sg_flags = (old.sg_flags & ~(CRMOD|ECHO|XTABS|RAW)) | CBREAK;
    if( new.sg_flags & LCASE )  new.sg_flags &= ~UCASE;
    /* �������� �������� V6 �������, �� �������-������� */
    new.sg_erase  = 0;   new.sg_kill   = 0;
    /* �������� �������� V7 �������, �� �������-������� */
    new.sg_eofc   = 0;   new.sg_brkc   = 0;
    /* ��� V7 ������� ���� �� ��������, �� ����� ���� �� */
/*  new.sg_startc = 0;   new.sg_stopc  = 0;  new.sg_quitc  = 0; */
    /* �������� �������� BSD �������, �� �������-������� */
    new.sg_width  = 0;   new.sg_length = 0;  new.sg_quitc  = 0;
    new.sg_dsuspc = 0;   new.sg_rprntc = 0;  new.sg_flushc = 0;
    new.sg_werasc = 0;   new.sg_lnextc = 0;
    new.sg_eofc   = 0;   new.sg_brkc   = 0;
#else
#   ifdef SYSV
        ioctl(0,TCGETA,&old);
        new = old;
        new.c_iflag = old.c_iflag & ~ICRNL;
        new.c_oflag = ( old.c_oflag | ONLRET ) & ~(ONLCR|TAB3);
        new.c_lflag = old.c_lflag & ~(ICANON|ECHO|ECHOE|ECHOK);
	new.c_cc[VINTR] = ctrl('c');
        new.c_cc[VERASE] = new.c_cc[VKILL] = new.c_cc[VEOF] =
        new.c_cc[VQUIT] = 0;
        new.c_cc[VMIN] = 1;
#       ifdef UCASE
            if( new.c_iflag & IUCLC )  new.c_oflag &= ~OLCUC;
#       endif
#   else
       /* ��������� ��� V7 et al. */
        ioctl(0,TIOCGETP,&old);               /* �������� ������ ����. */
        new = old;
        new.sg_flags = (old.sg_flags & ~(CRMOD|ECHO|XTABS|RAW)) | CBREAK;
#       ifdef UCASE
            if( new.sg_flags & LCASE )  new.sg_flags &= ~UCASE;
#       endif
        /* �������� �������� V6 �������, �� �������-������� */
        new.sg_erase  = 0;   new.sg_kill   = 0;
#   endif
#endif

    fsig = signal(SIGINT,SIG_IGN);   /* �������� ������� �� ctrl-C */
    if( (int)fsig == (int)SIG_DFL )  fsig = onintr;
    signal( SIGINT, fsig );
}

/* ������������  � DPI - ����� */

dpbeg() {

    /* ������������� ��� ������ ����� */
    if(!dpreg) {
        dpinit();
        dpreg = 1;
    }
    /* ������� ������ ��������� */
#ifdef BSD
    ioctl(0,TIOCSETA,&new);
#else
#   ifdef SYSV
        ioctl(0,TCSETA,&new);
#   else
        ioctl(0,TIOCSETP,&new);
#   endif
#endif
    xbuf=0;         /* ������������� ������ */
    sdps( t.beg );  /* ������������� ������ */
    grreg = ( t.param.tflag & a_gr ) ? 0 : -1;
}


/* ������������ � ������� ����� */

dpend() {
    grend();
    dpo( lf );  dpp( 0, t.param.ydim-1 );
    sdps( t.end );
    scrbufout();
#ifdef BSD
    ioctl(0,TIOCSETA,&old);
#else
#   ifdef SYSV
        ioctl(0,TCSETA,&old);
#   else
        ioctl(0,TIOCSETP,&old);
#   endif
#endif
}


/* �������� ��������� ��������� */

struct s_dpd dpd() {
    return( t.param );
}


/* ����� ������ ������� */

dpo(             c )
    int c;
{
    int m, i, j;
    char *p;
    static struct { unsigned gind:5;
                    unsigned gspc:3; } gstr[] =
        { 0, 0,  0, 1,  1, 0,  1, 1,  2, 0,  2, 1,  2, 2,  2, 3,
          3, 0,  3, 1,  3, 2,  3, 3,  4, 0,  4, 1,  4, 2,  4, 3,
          5, 0,  5, 1,  5, 2,  5, 3,  6, 0,  6, 1,  6, 2,  6, 3,
          7, 0,  7, 1,  7, 2,  7, 3,  8, 0,  8, 1,  8, 2,  8, 3,
          9, 0,  9, 1,  9, 2,  9, 3, 10, 0, 10, 1, 10, 2, 10, 3,
          0, 4,  1, 4,  2, 4,  3, 4,  4, 4,  5, 4,  6, 4,  7, 4,
          8, 4,  9, 4,  0, 5,  1, 5,  2, 5,  3, 5,  4, 5,  5, 5,
          6, 5 };

    c &= 0377;
    if( c < 040 ) {             /* ����������� ������� */
        grend();
        if( c == lf )  dpo(rn);
        if( c==0 )  {scrbufout();  return;}
        c = (c>=020) ? t.sp_out[c-020] : t.sp_inp[c] ;
        m = (c&0377)>>5;  c &= 037;
        switch( m ) {
            case cnt:                    sdpo(c);                  break;
            case esc_cnt: sdpo(t.escape);sdpo(c);                  break;
            case esc_sgn: sdpo(t.escape);sdpo(c+' ');              break;
            case esc:     sdpo(t.escape);sdpo(c+'@');              break;
            case prf:     sdps(t.pref);  sdpo(c+'@'); sdps(t.suff);break;
            case prf_cnt: sdps(t.pref);  sdpo(c);     sdps(t.suff);break;
        }

    } else if( c<0200 || c>=0300 ) {
        grend();
        sdpo(c);
    } else {                            /* ������������� */
        i = gstr[ c - 0200 ].gspc;
        j = gstr[ c - 0200 ].gind;
        grbeg();
        if( grreg < 0 && c < 0250 ) sdpo( t.s_g[i][j] );
        else if( grreg == 1 )       dpcodout( t.s_g[i][j] );
    }
}


/* ���� ������� */

int dpi() {
    char         cc;
    int c, i, j;
    enum { t_cnt, t_esc, t_prf } typ;

beg:
    if( empty(0) )  scrbufout();
    xread(0,&cc,1);  c = cc & 0377;

    if(c==de) return(c);
    if( c < ' ' ) {
        typ = t_cnt;
        if( c != 0  &&  c == t.escape ) {
            if(empty(0)) return(es);
            for( i=0; c==t.pref[i]; ++i )  c = sdpi();
            if( i == 0 )  c = sdpi();
	    if(empty(0)) typ = (i && t.pref[i]==0) ? t_prf : t_esc ;
	    else if( sdpi() == '~' ) {
		switch(c) {
		    case '1': return(bl);
		    case '2': return(ic);
		    case '3': return(bl);
		    case '4': return(ic);
		    case '5': return(pu);
		    case '6': return(pd);
		    case '7': return(dl);
		    case '8': return(il);
		    default:  dps("Unknown sequence");
			      dpi();
			      return(bl);
		}
	    }
	    else {
		dps("Unknown character");
		dpi();
		return(bl);
	    }
        }

        switch( typ ) {
          case t_cnt:
            i = cnt*040;
            break;
          case t_esc:
            i = c>='@' ? esc*040 : (c>=' ' ? esc_sgn*040 : esc_cnt*040);
            break;
          default:
            i = c>='@' ? prf*040 : prf_cnt*040;
            break;
        }
        c = (c&037) + i;
        for( i=0; i<32; ++i )  if( ((t.sp_inp[i])&0377) == c )  break;
        if( i < 32 ) {
            c = i;
        } else {
            for( j=0; j<13; ++j )  {
                if( ((t.sp_fnc[j])&0377) == c )  break;
            }
            if( j>=13 )  goto beg;
            else c = j + 0200;
        }
    } else if( c == 0177 ) {    /* � ����� ��� ������... */
        for( i=0; i<32; ++i )  if((( t.sp_inp[i])&0377 ) == c ) {
            c = i;
            break;
        }
    }
    return( c );
}


/* ���� ������� � ��������� ������������� �������� */

static int sdpi() {
    char         cc;
    int c;

    xread(0,&cc,1);  c = cc & 0377;

    if( c >= 0300 ) {
#ifndef koi8
         static char uc_to_as[] = {
             'A', 'B', 'W', 'G', 'D', 'E', 'V', 'Z',
             'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
             'R', 'S', 'T', 'U', 'F', 'H', 'C', '^',
             '[', ']', '"', 'Y', 'X', '\\','@', 'Q',
         };
         c = uc_to_as[c&037];
#else
         c = (c&037)+'@';
#endif
    }
    return( c );
}


/* ��������, ��� �� ����� ������ */

dpa() {
    scrbufout();
    return( empty(0) ? no_symb : dpi() );
}


/* ������� ������� � ������� (x,y) */

struct s_dpp dpp( x, y )
    int  x, y;
{
    int i, c;
    struct s_dpp res;
    char        *p;
    char *tgoto();

    if( *t.sdpp && t.ldpp ) { /* ���� ���������������� "�� �������" */
        p = t.sdpp;
        for( i=0; i<t.ldpp; ++i ) {
            c = *p++;
            if(      i == t.xdpp ) {
                c += x;
                dpcodout(c);
            }
            else if( i == t.ydpp ) {
                c += y;
                dpcodout(c);
            }
            else sdpo( c );
        }
    }
    else if( *t.sdpp && !t.ldpp) { /* ���������������� ����� tgoto */
        for(p=tgoto(t.sdpp,x,y); c = *p; ++p) dpcodout(c);
    }
    else { /* ������ ��� ����������������: t.sdpp = "" */
        register int j;
        dpo(s_spec);
        for(j=0; j<x ; j++) dpo(cr);
        for(j=0; j<y ; j++) dpo(cd);
    }
    return( res );
    /* ������������, �� �� ����������� res! ��� ���� � ���������! */
}


/* ����� ������ �������� �� \0 */

static sdps( str )
    char    *str;
{
    while( *str )  sdpo( *str++ );
}


/* ����� ������ ������� */

static sdpo( c )
    char     c;
{
    buf[xbuf] = c;
    if( ++xbuf >= lbuf )  scrbufout();
}


/* ����� ������ */

static scrbufout() {
    if( xbuf )  write(1,buf,xbuf);
    xbuf = 0;
}

#ifndef koi8
    static char IN_tab[] = {
        0376,0340,0341,0366,0344,0345,0364,0343,
        0365,0350,0351,0352,0353,0354,0355,0356,
        0357,0377,0360,0361,0362,0363,0346,0342,
        0374,0373,0347,0370,0375,0371,0367,0372,
        0336,0300,0301,0326,0304,0305,0324,0303,
        0325,0310,0311,0312,0313,0314,0315,0316,
        0317,0337,0320,0321,0322,0323,0306,0302,
        0334,0333,0307,0330,0335,0331,0327,0332
    };
#endif

/* ����� ������� �������� ��������� ���� c */

static dpcodout(c)
char c;
{
     /* ��� ��� ���������� ������� ��� ����,
        ����� ������ ������������� ��������,
        ������� �� ������� ��� ���.���� ���
        ��� ������ � UCODE ������� ���������
        �� ��� ���, ��� ��� �������.
        ��� ������ ������ - ��� ����.
        � ��� ����������������, ��������, ����,
        ���� ��������� ��������� ���!     */

        if( (old.c_iflag & IUCLC)  &&  c>=0140 ) {
#ifdef koi8
                c += 0300 - 0140;
#else
                c  = IN_tab[c-0140];
#endif
        }
        sdpo(c);
}

/* ��� hdps: ������� ���� ����������� ������� ��� ����� */
dpincod(i)
int i;
{ return(t.sp_inp[i]);}

/* ��������� ����� */

/* ����������:   1 - �������� ����������;
                 0 - �������� �� �������;
                -1 - �������� ���������.
*/

dpc( ncf, ncb )
{
    struct s_color cc;
    char *index();
    char *p;
    int c;
    int vf, vb;
    char *tparm();

    if( t.param.nf <= 0 ) return( 0 );
    if( ncb < 0 ) {     /* ���������� ������ �������� ���� */
        if( ncf < 0 || ncf >= 16 ||
        ( vf = index( t.smf, ncf<10?ncf+'0':ncf-10+'A') - t.smf ) < 0 )  {
            return( -1 );
        } else {
            if( t.param.tflag & a_c2 ) {  /* ����� ��������������� ������ */
                vb = t.color.cb;
                for( p=tparm( t.scf, vb, vf ); c = *p; ++p ) dpcodout( c );
            } else
                for( p=tparm( t.scf, vf ); c = *p; ++p ) dpcodout( c );
            t.color.cf = vf;
        }
    } else if( ncf < 0 ) { /* ������ ������� ���� */
        if( ncb >= 16 ||
           ( vb = index( t.smb, ncb<10?ncb+'0':ncb-10+'A' ) - t.smb ) < 0 )  {
            return( -1 );
        } else {
            if( t.param.tflag & a_c2 ) {  /* ����� ��������������� ������ */
                vf = t.color.cf;
/*              for( p=tgoto( t.scf, vb, vf ); c = *p; ++p ) dpcodout( c ); */
                for( p=tparm( t.scf, vb, vf ); c = *p; ++p ) dpcodout( c );
            } else
/*              for( p=tgoto( t.scb, 0, vb ); c = *p; ++p ) dpcodout( c ); */
                for( p=tparm( t.scb, vb ); c = *p; ++p ) dpcodout( c );
            t.color.cb = vb;
        }
    } else {        /* ��� ����� */
        if(( vf = index( t.smf, ncf<10?ncf+'0':ncf-10+'A' ) - t.smf ) < 0   ||
           ( vb = index( t.smb, ncb<10?ncb+'0':ncb-10+'A' ) - t.smb ) < 0 )  {
           return( -1 );
        } else {
            if( t.param.tflag & a_c2 ) {
/*              for( p=tgoto( t.scf, vb, vf ); c = *p; ++p ) dpcodout( c ); */
                for( p=tparm( t.scf, vb, vf ); c = *p; ++p ) dpcodout( c );
            } else {
/*              for( p=tgoto( t.scf, 0, vf ); c = *p; ++p ) dpcodout( c ); */
                for( p=tparm( t.scf, vf ); c = *p; ++p ) dpcodout( c );
/*              for( p=tgoto( t.scb, 0, vb ); c = *p; ++p ) dpcodout( c ); */
                for( p=tparm( t.scb, vb ); c = *p; ++p ) dpcodout( c );
            }
            t.color.cf = vf;
            t.color.cb = vb;
        }
    }
    return( 1 );
}

/* �������������

dpg( gd, ind ) {
    char c;
    char *p;

    if(( p = t.gr_beg ) == NULL )
        return( 0 ); * �������� �� ������������ ������������� *
    for( ; c = *p; ++p ) dpcodout( c ); * ��������� *
    dpcodout( t.s_g[ gd ][ ind ] );
    for( p = t.gr_end; c = *p; ++p ) dpcodout( c ); * ���������� *
    return( 1 );
}
*/

struct s_color *dcolor() { return( &t.color ); }
