/* ����� ������, ��������������� ��������:

   get_suit()      ������������ �� ������� 'select'
                   ���� 'only_x', �� �� �������� ��������� ������
*/

/*
 * $Log:        getsuit.c,v $
 * Revision 1.1  89/08/19  17:53:52  rygoff
 * Initial revision
 * 
 *
 */

#include "comint.h"

get_suit() {
    register char *n, *s;
    register int   code;
    int            i, skip;
    struct dir2   *pd;

    for( i=lind; --i>=0; ) {                    /* ������� '-' ������� */
        pd = (struct dir2*) (&cat[indcat[i]&0377]);
        if( pd->d_mark=='-' || pd->d_mark=='d' )  pd->d_mark=0;
    }

    nfiles = 0;
    if( ico_from )  indico[nfiles++] = maxcat+1;               /* <--( */
    skip = ico_from;

    for( i=0; i<lind; ++i ) {
        if( nfiles >= maxico ) {
            indico[nfiles-1] = maxcat;                         /* )--> */
            break;
        }
        pd = (struct dir2*) (&cat[indcat[i]&0377]);

        if( *select ) {                          /* �������� �� ����� */
            n = pd->d_name;  s = select;
            while( *s )  {
                if( *s == '^' )  {if( *n == 0 )  goto not_suit;}
                else             {if( *n != *s ) goto not_suit;}
                ++n;  ++s;
            }
        }

        if( selsuf ) {                           /* �������� �� �������� */
            register int l;
            n = pd->d_name;
            for( l=strlen(n); --l>=0; )  if( n[l]=='.' )  break;
            if( l < 0  ||  n[l+1] != selsuf )  goto not_suit;
        }

        if( only_x && pd->d_name[0]=='.') goto not_suit;  /* ��� .������ */

        if( only_x && nfiles ) {         /* �� ��������� ��������� ����� */
            s = cat[indico[nfiles-1]&0377].d_name; /* ��� ����. ����� */
            code = depend( s, pd->d_name );
            if( code==0 )  goto toico;          /* ����������� ���� */
            if( code <0 )  {                    /* ������ �����.�� ������*/
                indico[nfiles-1] = indcat[i];
            } else {                            /* ����� �����.�� �������*/
            }
        } else {
toico:      if( skip-- <= 0 )  indico[nfiles++] = indcat[i];
        }
not_suit: ;
    }
}


/* ���������� ��������� �� ��������� ���� ����� 'i' */

struct dir2 *item( i )
    int            i;
{
    return( (struct dir2*)(&cat[indico[i]&0377]) );
}
