/* ��������� �� ������ 'p' ��������� ������ */

/*
 * $Log:        cm_line.c,v $
 * Revision 2.3  89/11/12  10:33:03  rygoff
 * ��������� ����� ����� $F
 * 
 * Revision 2.2  89/11/07  12:45:08  rygoff
 * ������� ��������������� $! � $?, 
 * ������� ����������� ����������� ��������� _! � _?
 * (��. exefound.c)
 * 
 * Revision 2.1  89/10/30  19:19:07  rygoff
 * ����� �����.
 * ��� ������ ������ ��������������� � exefound()
 * (����� ������� show_type)
 * 
 * Revision 1.1  89/08/19  18:17:56  rygoff
 * Initial revision
 * 
 *
 */

#include "comint.h"

char *cvmacro();
extern char *fname, *fsuff, *ffull, *name_f, *name_g, *name_F;
extern char answ_symb, answ_line[], pattern[];


cm_line(         p )
  register char *p;
{
  register int     i;
  register char    addsym;
  register char   *s;

  addsym = 0;
  for( ; *p and *p!='\n'; ++p ) {
    if( *p == '$'  and  p[1]!='\n' ) {
      switch( *++p ) {      /* ��������� ����� ������: */

          case '!':                 /* ��������� ������ */
            if( answ_symb ) put_sh(answ_symb);
            break;

          case '*':                 /* ����� ���������� ������ */
          case ':':                 /* �.�. ��� ���� */
            for(i=0; i<nmarked; ++i) add_name(&cat[marked[i]&0377],0);
            for(i=0; i<nalt;    ++i) add_name(&altnam[i],1+(*p==':'));
            break;

          case '#':                 /* ������ ���������� ������ */
            for( i=0; i<nmarked; ++i ) {
              put_sh(' ');
              cpy_shbuf( conv(marked[i]&0377,5,10,0) );
            }
            break;

/* guba, 22.06.87  --->  */
          case '%':                 /* ����� �������� ����� */
            put_sh(' ');
            cpy_shbuf( conv(indico[current]&0377,5,10,0) );
            break;
/* guba, 22.06.87  <---  */

          case '-':                 /* ����� ���������� - ������ */
            for( i=0; i<nfiles; ++i )  {
              register struct dir2 *pd, *sd;
              register int          j;
              pd = item(i);
              if( pd->d_mark == '-' ) {
                add_name(pd,0);
                if( only_x ) {            /* ����� ��������� ����� */
                  for( j=lind; --j>=0; ) {
                    sd = &(cat[indcat[j]&0377]);
                    if( is_boss(pd->d_name,sd->d_name) )  add_name(sd,0);
                  }
                }
              }
            }
            break;

          case '$':                 /* $$ */
            goto pt;

          case 's': addsym='.';
          default:
            s = cvmacro(*p);
            if( s!=NULL and *s ) {
              if( addsym!=0 )  put_sh(addsym);
              mcpy_shbuf(s);
            }
            addsym = 0;
            break;
      }
    } else {
pt:   if( *p=='/' and p[1]=='$' ) {
        addsym = '/';
      } else {
        if( addsym )  {put_sh(addsym);  addsym = 0;}
        else           put_sh(*p);
      }
    }
  }
}


char *cvmacro(sym)  char sym; {
    register char *s;

    switch(sym) {
          case '?': s = answ_line;       break;   /* ��������� ������ */
          case '@': s = ffull;           break;   /* ������ ��� */
          case 'n': s = fname;           break;   /* ��� ��� �������� */
          case 's': s = fsuff;           break;   /* ������� */
          case 'c': s = hcat;            break;   /* ��� �������� */
          case 'a': s = arname;          break;   /* ��� ���������� */
          case 'p': s = arpath;          break;   /* ��� ���� � ����. */
          case 'C': s = altcat;          break;   /* ��� ��.�������� */
          case 'A': s = altarname;       break;   /* ��� ��.���������� */
          case 'P': s = altpath;         break;   /* ��� ���� � ��.����*/
          case '~': s = getenv("HOME");  break;   /* ���.��� */
          case 'f': s = name_f;          break;   /* ������ ��� */
          case 'g': s = name_g;          break;   /* ������ ��� */
          case 'F': { /*  rygoff 22.06.89 - ������ ��������� ���
                       *  �.�. ������ ������, ������ ��� �����������
                       *  �������� ����������� ����������� ���������
                       */
                       static char tmp_line[lnam1];
                       strcpy(tmp_line, altcat);
                       strcat(tmp_line,"/");
		       s = name_F = strcat(tmp_line, name_f);
                    }
                    break;

          case 'T': s = pattern;         break;   /* ���� �� ������   */
                                                  /* � �������������� */
          default:  s = NULL;            break;
     }
     return(s);
}

