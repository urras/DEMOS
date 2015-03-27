/* ������������ �.�. ���������, 1988 �. */
/* ��������� ��������� �������� ��������� ����� termcap */

/* ���� �.�. ( demos/* ) 1990 �. ) */
/* �������������� ��� PC */

#include <stdio.h>
/*#include <sgtty.h>*/
#include "tty_descr.h"

extern char *getenv(), *strcat(), *strcpy(), *tgetstr(), *malloc();
extern int tgetent(), tgetnum();
char *scan_delay(), *strget();

static char prefix[20];  /* ������� ��� ��������� ����� */
static int  escape = 033;         /* ��� escape-������� */

tcinit(dref)
struct s_tdescr *dref;            /* �������� ��������� */
{
#define descr (*dref)
char   *term_name;                /* ��� ��������� */
char    tbuf[1024];               /* ����� ��� termcap */
char    sbuf[24], *psbuf = sbuf;  /* ����� ��� �������� ����� */
char    buf5[200], *s5 = buf5;    /* ����� ��� ���������� ��������
                                     ���������� ����� �������� */

/* ������� �����, ��� ������� ���� ������������� ����� */
static struct icod {
    int  cod;
    char *tnam;
} spc[] = {
    de,        "dc",
    cu,        "ku",
    cd,        "kd",
    cr,        "kr",
    cl,        "kl",
    ic,        "kI",
    dc,        "kD",
    il,        "kh",  /* Home */
    dl,        "kH",  /* End  */
    es,        "kS",
    el,        "kE",
    ru,        "kR",
    rd,        "kF",
    pu,        "PU",
    pd,        "PD",
    s_finish,  "kH",
    s_spec,    "kh"   /* �� ��������� ��������� ��������� home */
};

/* �������� �������������� ���������� */

static struct icod spcf[] = {
    f1,        "f1",
    f2,        "f2",
    f3,        "f3",
    f4,        "f4",
    f5,        "f5",
    f6,        "f6",
    f7,        "f7",
    f8,        "f8",
    f9,        "f9",
    f10,       "f0",
    f11,       "f.",
    f12,       "f-"
};

#define nspc  (sizeof spc/sizeof spc[0])
#define nspcf (sizeof spcf/sizeof spcf[0])

/* ������ ����������� �� ������ */
#define wrerr(s) write(2,s,strlen(s))

int   i, pack;
char *p;


/* �������������� termcap */

    term_name = getenv("TERM");
    printf("TERM=%s\n\r", term_name);
    if( (i=tgetent( tbuf, term_name )) <= 0 )    {
	printf("  �� ������� ������� ������ �� %s: code %d\n", getenv("TERMCAP"), i );
        exit(1);
    }

/* ��������� ���� ���������� �������� */

    {
#       define dout(cod) descr.sp_out[cod-16]

        /* ��������! ��� ��������� ������ ����.��� >= 16 ! */
        static struct prp { char cod, opt, *prop; } prp[] = {
            cu, 0, "up",   cd, 0, "do",   cl, 0, "kl",   cr, 0, "nd",

                /* ������ ������� ������ "kl" ������ "bs",
                ������������ � �������� ��� SYSV       */

            el, 1, "ce",   es, 1, "cd",   ic, 1, "ic",   dc, 1, "dc",
            il, 1, "al",   dl, 1, "dl",   ru, 1, "sf",   rd, 1, "sr",
            s_spec, 1, "ho", 0, 0
        };
        struct prp *pp;

       /* ���������� �������� ���������� ����� */

        for( i=0; i<sizeof descr.sp_out; ++i )
            descr.sp_out[i] = 0;

        for( pp = &prp[0];  pp->cod;  ++pp ) {
            psbuf = &sbuf[0];
            p = tgetstr(pp->prop,&psbuf);
            if( p && (i=pack_code(p)) >= 0 )
                 dout(pp->cod) = i;
        }

        if( tgetflag("bs")) {
            if(( i=pack_code("\b")) >=0 ) dout(cl) = i;
        }
        if( dout(ru) == 0 )  dout(ru) = lf;
    }


/* ������ ���� �������� ������ �� ��������� */
/* �� ��������� ����������� ���.��� == ��������� */

    for( i=1; i<sizeof descr.sp_inp; ++i )
            descr.sp_inp[i] = i;

/* �������������� �� �������� ���� ��������� ������ ��� ����� */

    for( i=0; i<nspc; ++i ) {
        psbuf = &sbuf[0];
        if( spc[i].tnam && (p=tgetstr(spc[i].tnam,&psbuf)) &&
             ( pack = pack_code(p)) >= 0)
        /* ���� ��� ���������� � ������������� */
            descr.sp_inp[spc[i].cod] = pack;
    }

/* ���� �������������� ���������� */

    for( i=0; i<nspcf; ++i ) {
        psbuf = &sbuf[0];
        if( spcf[i].tnam && (p=tgetstr(spcf[i].tnam, &psbuf)) &&
             ( pack = pack_code(p)) >= 0)
        /* ���� ��� ���������� � ������������� */
            descr.sp_fnc[spcf[i].cod - 0200] = pack;
    }

/* x-������ y-������ ����������� */
    descr.param.xdim =  tgetnum("co");
    descr.param.ydim =  tgetnum("li");
    descr.param.nf   =  tgetnum("Nf");
    descr.param.nb   =  tgetnum("Nb");
    descr.param.tflag = (dout(ic)&&dout(dc)?a_icdc:0) +
                        (dout(il)&&dout(dl)?a_ildl:0) +
                        (dout(rd)?a_rd:0) +
                        ( !tgetflag("UC")?a_lower:0) +
                        (  tgetflag("C1")?a_c1:0) +
                        (  tgetflag("C2")?a_c2:0);


/* �����a������� ��������� ������ */
    if((descr.beg=tgetstr("ti",&s5))==0) descr.beg = strget("",&s5);
#ifdef fun_key
    --s5; tgetstr("ks",&s5); /* ��������� �-����� ���������� */
#endif
/* ���������� ��������� ������ */
    if((descr.end=tgetstr("te",&s5))==0) descr.end = strget("",&s5);
#ifdef fun_key
    --s5; tgetstr("ke",&s5); /* ���������� �-����� ���������� */
#endif

/* ������� � ������� ���       */
/* ��������� ����������� ����� */
/* ������� � ����� ������� ������� ���������� pack_code */
/* �����, ��� � ��� ����� � ��� ������ �� ���� � ��� ��! */
/* ������� ������ ���� �� ����! ��� �������� ����! */
    descr.pref = strget(prefix, &s5);
/* ������� ���� ������������� �� ����������! */
/* ����� �� �� �����? �� ������� �� �� ���� ������ �������? */
    descr.suff = strget("",&s5);

/* ������������ ������ ����������������. */
/* ������� �������� ��������� �� �� �������.
   ���� �� ���������� �������� � ������ ������� ��� tgoto() */
    psbuf = &sbuf[0];
    if( p=tgetstr("cm",&psbuf) ) {
        register char *q, *pp;
        register int   xp, yp;
        pp = p = scan_delay(p);
        q = psbuf;  yp = -1;
        while( *p ) {
            if( *p=='%' ) {
                ++p;
                if(      *p == '.' ) {*q++ = 0;  ++p;}
                else if( *p == '+' ) {++p;  *q++ = *p++;}
                else break; /* ������� ������ cm */
                if( yp < 0 )  yp = q - psbuf - 1;
                else          xp = q - psbuf - 1;
            } else {
                *q++ = *p++;
            }
        }
        *q = '\0';
        if( *p ) { /* ����� �� �����,�� �������� cm */
            descr.sdpp = strget(pp, &s5); /* �����. ����� tgoto */
            descr.ldpp = 0; /* ������� �����. ����� tgoto */
        } else {
            descr.sdpp = strget(psbuf, &s5);
            descr.xdpp = xp;
            descr.ydpp = yp;
            descr.ldpp = q-psbuf;
        }
    } else {
        /* ��� ������� ���������������� */
        descr.sdpp = strget("",&s5);
    }

    descr.escape = escape;

/* ��������� ����� */

    if( descr.param.nf > 0 ) {  /* �������� ������� */
        int pcf, pcb;

        psbuf = &sbuf[0];
        pcf = ( p = tgetstr( "Cf", &psbuf )) ? 1 : 0;
        p = scan_delay( p );
        descr.scf = strget( p, &s5 );

        psbuf = &sbuf[0];
        pcb = ( p = tgetstr( "Cb", &psbuf )) ? 1 : 0;
        p = scan_delay( p );
        descr.scb = strget( p, &s5 );

        if( !( descr.param.tflag & a_c1) && !( descr.param.tflag & a_c2) ) {
            if( pcf && pcb ) descr.param.tflag |= a_c1;
            else if( pcf )   descr.param.tflag |= a_c2;
        }

        psbuf = &sbuf[0];
        if( p = tgetstr( "Mf", &psbuf ))
            descr.smf = strget( p, &s5 );
        else
            descr.smf = strget( "0123456789ABCDEF", &s5 );

        psbuf = &sbuf[0];
        if( p = tgetstr( "Mb", &psbuf ))
            descr.smb = strget( p, &s5 );
        else
            descr.smb = strget( "0123456789ABCDEF", &s5 );

        descr.color.cf = white;
        descr.color.cb = black;
    }

/* ������������� */

    psbuf = &sbuf[0];
    if(( p = tgetstr( "gs", &psbuf )) != NULL ) {
        static char *gg[] = { "g1", "g2", "g3", "g4", "g5", "g6" };

        descr.gr_beg = strget( p, &s5 );
        descr.param.tflag += a_gr;

        psbuf = &sbuf[0];
        p = tgetstr( "ge", &psbuf );
        descr.gr_end = strget( p, &s5 );

        for( i=0; i<6; i++ ) {
            psbuf = &sbuf[0];
            descr.s_g[i] =
              ((p=tgetstr(gg[i],&psbuf))!=NULL) ? strget( p, &s5 ) : NULL;
        }
    } else {
        static char gsngl[] = "-|---|+|---", gdbl[] = "=|===|+|===";

        descr.s_g[0] = descr.s_g[3] = strget( gsngl, &s5 );
        descr.s_g[1] = descr.s_g[2] = strget( gdbl, &s5 );
        descr.s_g[4] = descr.s_g[5] = s5-1;
    }

/* ���������� ���������� �����, �������� ��������� � buf5 */
    /* ������� ����� */
    if((psbuf = malloc((unsigned)(s5-&buf5[0])) ) == NULL ){
                wrerr("��� ����� ��� ��������� ���������\n");
                exit(1);
    }

    /* �������� */
    for(i=0; i<s5-&buf5[0]; ++i) psbuf[i] = buf5[i];
    /* ����������������� ������ */
    descr.beg += psbuf - &buf5[0];
    descr.end += psbuf - &buf5[0];
    descr.pref += psbuf - &buf5[0];
    descr.suff += psbuf - &buf5[0];
    descr.sdpp += psbuf - &buf5[0];
    descr.scf += psbuf - &buf5[0];
    descr.scb += psbuf - &buf5[0];
    descr.smf += psbuf - &buf5[0];
    descr.smb += psbuf - &buf5[0];
    descr.gr_beg += psbuf - &buf5[0];
    descr.gr_end += psbuf - &buf5[0];
    for( i=0; i<6; i++ ) descr.s_g[i] += psbuf - &buf5[0];
}


/* �������� �������� ������� ������ */

#if ('�'&037) == ('A'&037)
#   define koi8
#endif

int pack_code( sc )
    char      *sc;
{
    int  l, packed, pflag;
    char symb;

    sc = scan_delay(sc);
    l = strlen(sc);

    pflag = 0;
    if( l>2 ) {
        if( prefix[0]==0 )  strncpy(prefix,sc,l-1);
        if( strncmp(prefix,sc,l-1) )  return(-1);
        pflag = 1;
        goto smb;
    }

    if( *sc==escape && l==2 ) {
smb:    symb = sc[l-1];
        if( is_rus(symb) ) {
#ifndef koi8
            static char uc_to_as[] = {
                'A', 'B', 'W', 'G', 'D', 'E', 'V', 'Z',
                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                'R', 'S', 'T', 'U', 'F', 'H', 'C', '^',
                '[', ']', '"', 'Y', 'X', '\\','@', 'Q',
            };
            symb = uc_to_as[symb&037];
#else
            symb = (symb&037)+'@';
#endif
        }
        if( is_rus(symb) || (symb>='@' && symb<='~') )  symb &= ~040;

        if( is_control(symb) ) {
            if( !pflag )  packed = (symb & 037) + 040*esc_cnt;
            else          packed = (symb & 037) + 040*prf_cnt;
        } else if( '@'<=symb && symb<='_' ) {
            if( !pflag )  packed = (symb & 037) + 040*esc;
            else          packed = (symb & 037) + 040*prf;
        } else {
            packed = (symb & 037) + 040*esc_sgn;
        }
    } else if( is_control(*sc) && l==1 ) {
        packed = (*sc & 037) + 040*cnt;
    } else if( *sc == 0177 && l==1 ) {
        packed = 0177;
    } else {
        return(-1);                                   /* ��������� ��� */
    }
    return( packed );
}

/* ������� �������� */

char *scan_delay(  p )
    register char *p;
{
    while( (*p>='0' && *p<='9') || *p=='*' )  ++p;
    return( p );
}

/* ����������� ����� a la tgetstr() */
char * strget(from, ppoint)
char *from;
char ** ppoint;
{
    /* �������� ������ from � �����, �� ������� ���������
       *ppoint, ���������� *ppoint �� ������ �� ������ �����.
       ���������� ������ �������� ppoint
       from = NULL  ������������ from = "". */
    char *retval = *ppoint;
    if(from)
        while(*from){
            **ppoint = *from;
            (*ppoint)++; from++;
        }
    **ppoint = '\0'; ++(*ppoint);
    return retval;
}
