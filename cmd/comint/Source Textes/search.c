/* ������ �� ������� �������� ������

   ����� �����:

      search_ind( name )      ����� ������� ����� 'name' � �������������
                              ������ ������ 'indcat'.
                              ���������� ������ ���� ������ �
                              ~������ ������� ���� ���� �������� ���� ���

      insert_ind( ind, to )   ������� ������ ������� � 'indcat' �
                              ������� 'to';
*/

/*
 * $Log:        search.c,v $
 * Revision 1.1  89/08/19  18:11:44  rygoff
 * Initial revision
 * 
 *
 */

#include "comint.h"

int search_ind( name )
    char       *name;
{
    register int code, from, to, middle;

    from = 0;  to = lind;                            /* �������� ����� */
    while( from != to ) {
        middle = (from + to) / 2;
        code   = strcmp( name, cat[indcat[middle]&0377].d_name );
        if( code == 0 )  return( middle );           /* ������ */
        if( code >  0 )  from = middle+1;
        else             to   = middle;
    }
    return( ~from );                                 /* �� ������ */
}


insert_ind( ind, to )
    int     ind, to;
{
    register char *bmove, *emove;

    bmove = &indcat[to];
    emove = &indcat[lind];
    while( bmove < emove ) {
        --emove;
        *(emove+1) = *emove;
    }
    *bmove = ind;
    ++lind;
}
