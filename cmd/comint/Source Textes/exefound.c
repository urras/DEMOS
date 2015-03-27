/*
 *      ���������� ��������� ������ �� ����� ���������.
 *      ��������� ��� ����, ���� ��� ����� �������� ���������
 *      ���������, �.�. lookcom ������� ��������� ������
 *      ��� ����� �/��� ������ �(��) �������(�).
 *      ( ��������� �������� ����� to_head() � to_tail() )
 *
 * $Log:        exefound.c,v $
 * Revision 2.8  89/11/15  16:44:40  rygoff
 * ������� ����������� ����� (����� nxfile:).
 * ����� �������� start() ��������� ���������� �� SIGCHLD
 * (������ SIG_IGN ������ �������� SIG_HOLD).
 * 
 * Revision 2.7  89/11/15  14:42:36  rygoff
 * *** empty log message ***
 * 
 * Revision 2.6  89/11/15  14:19:47  rygoff
 * 
 * Revision 2.5  89/11/11  22:47:37  rygoff
 * ������� �������� _! � _?
 * 
 * Revision 2.4  89/11/09  10:36:31  rygoff
 * ������ ������
 * 
 * Revision 2.3  89/11/07  12:43:48  rygoff
 * ������� ���������� ������� _! � _?
 * 
 * Revision 2.2  89/11/03  13:18:39  rygoff
 * ����� ���������� �������� � ������ p ������ ��� �����,
 * ����� p ��������� �� ������ ������ � ������.
 * 
 * Revision 2.1  89/10/30  19:29:24  rygoff
 * ������ � ���������������� � ������������� ���������
 * �������������� ������������ start(), ����������� ����� rspec().
 * ������� �� ����������� ������������� ���������� (����) ������
 * comint'�.
 * ������ ������ start() ���������� ����������� ��� ������������
 * (��������, ����� �������, ��������� � change.c
 * )
 * 
 */

#include <signal.h>
#include <tty_codes.h>
#include "comint.h"
#include "run.h"

extern char last_name[];
extern boolean noname;
extern boolean Paint;
#undef pd
extern struct dir2 *pd;
extern int INSYMB;
char answ_symb, answ_line[80];



exefound( pcurr ) register char *pcurr; {
#   define BufLen 256
    char buf[BufLen];  /* ����� ��� ����������� ��������� ����� */
    int pos, len;

    register char *p, *fin, *p2 /* Also used as integer */;
#   define ip2          (short)p2
#   define iip2(x)      (p2 = (char *)(x))

    p = (p2=buf) + BufLen - 1;
    while( (*p2++= *pcurr++)!='\n' ) if(p2>=p) goto err;
    *--p2='\0';
    p=buf;

    while(pcurr=p) {

      add:

        while( (p=index(++p,';')) && *(p-1) == '\\' );/* ���� ; ��� ������ */
        if(p) { p2=p++; while(*p++==' ') {;} p--;  } /* ���� ������ ����. */

        if( *pcurr != '_' ) {   /* ������� ������� - �������  */
            if(p) {             /* ���� ����������� ������ ����� ';' : */
               if(*p!='_') goto add;    /* - ������� ������ */
               *p2='\0';                /* - ����������� - ����� ������ */
            }
#ifdef BSD
            signal(SIGCHLD, SIG_IGN);
#endif
            start( show_type(pcurr), INSYMB, NO );
            last_mod = 0;
            if(nfiles) strcpy(last_name, item(current)->d_name);
            getcat();
            goto look_oname;

        } else {            /* ������� - �����������, ����� ��������  */

            register c = *++pcurr;

            pcurr++;
            *p2='\0';
            ini_shbuf();
            cm_line(pcurr);
            pcurr = get_shbuf();
            no_esc(pcurr);

            switch( c ) {

              case '!':                       /* ���� ������ ������� */
                  if(*pcurr) { dpp(0,ysize-1); dpo(el); dps(pcurr); }
                  INSYMB = answ_symb = dpi();
                  if( not is_print(answ_symb) ) answ_symb=0;
                  continue;

              case '?':                       /* ���� ������ */
                  pos=0; len=xsize-strlen(pcurr);
                  dpp(0,ysize-1); dpo(el); dps(pcurr);
                  fin= (p2=answ_line) + len;
                  for( ;p2<fin; ) *p2++=' ';
                  dpr(answ_line, len, &pos, on_control|MS_mode);
                  for( ;p2>answ_line && *--p2==' '; ) *p2='\0';
                  continue;

              case 'a':                       /* ���������� � ����� */
                  ls(pcurr);
                  current = (current+1)%nfiles;
                  continue;

              case 'b':                       /* ����� ����� */
                  catf(pcurr);
                  goto newsel;

              case 'c':                       /* ������� � ������� */
                  to_tail(pcurr, 0);
                  goto look_oname;

              case 'd':                             /* ����� ������ */
                  finish(); /*   OR   */ goto newsel;

              case 'e':                       /* ������ �� ������ ������ */
                  to_head();
                  goto look_oname;

              case 'f':                       /* ���� � ���� ��� � �����.*/
                  if( noname )  goto err;
                  to_tail(pd->d_name, 1);
                  goto look_oname;

              case 'g':                       /* �������� �� ������� */
                  fselect();
                  goto newsel;

              case 'i':                       /* �������� - � ��������� */
                  if( pd >= &cat[maxcat] ) goto nx_pg;
                  if( pd->d_mark != *pcurr and pd->d_mark != '-' ) {
                      dpo(bl);  dpo(cl);  dpo(pd->d_mark = *pcurr);
                      continue;
                  }

              case 'h':                       /* �������� - */
                 if( pd >= &cat[maxcat] )  goto nx_pg;
                 dpo('-');
                 pd->d_mark = '-';
                 current = (current+1)%nfiles;
                 continue;

              case 'j':                       /* ������� ����� ������ */
                  strcpy( last_name, Mkdir(pcurr) );
                  if( *last_name == 0 ) continue;
                  getcat();
                  goto look_oname;

              case 's':                       /* ��������� environ'� */
                  setenv(pcurr);
                  continue;

              case 'z':                       /* ��������� ������� */
                  dpo(bl);
                  if(*pcurr) { dpp(0,ysize-1); dpo(el); dps(pcurr); dpo(bl); }
                  continue; /* ��� �� ���������� */

              default:                        /* ��������� ������� */
              err:
                  dpo(bl);
                  return;

            } /* End switch */

        } /* End if */

    nx_pg:
        if( pd == &cat[maxcat] ) {                        /* )---> */
            ico_from += ico_from ? maxico-2 : maxico-1 ;
        } else {                                          /* <---( */
            ico_from -= maxico-1;
            if( ico_from ) ++ico_from;
        }
    newsel: /* ��������� �������� (�������� ��������� �������) */
        if(nfiles) strcpy(last_name, item(current)->d_name);
    look_oname:
        get_suit();
        current = nfiles;
        if( *last_name  &&  (iip2(search_ind(last_name))) >= 0 ) {
            iip2( indcat[(short)p2] & 0377 );
            for( current=nfiles;  --current>=0; ) {
                if( (indico[current] & 0377) == ip2 )  break;
            }
        }
        Paint=YES;

    } /* End while */
}
