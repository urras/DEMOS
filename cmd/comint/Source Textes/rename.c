/* �������������� ����� 'name' � ��������� ������ */

/*
 * $Log:        rename.c,v $
 * Revision 2.3  89/11/13  14:32:57  rygoff
 * ����� �� ����� '=' ��� �������������� ��������,
 * INSYMB �������� ��������
 *
 * Revision 2.2  89/11/11  22:57:08  rygoff
 * Wweden revim MS_mode
 * ������ ����� MS_mode ��� �������������� �����.
 *
 * Revision 2.1  89/10/30  19:50:39  rygoff
 * ����� �����.
 * ����� rspec() �������. ��. ���������� ������ � Mkdir.c
 *
 * Revision 1.1  89/08/19  18:10:46  rygoff
 * Initial revision
 *
 *
 */

#include "comint.h"
#include <sys/stat.h>
#include <tty_codes.h>

extern char ftype;
extern int INSYMB;

char *chname( name )
    char     *name;
{
    register int c, delta, i;
    char        *p;
    int          pos, lroot, er_flag;
    static char  new[D_NAME+1], dep[D_NAME+1];

    dpo('=');  strcpy(new,name);

    pos = 0;
inp:
    do {                                      /* ���� ������ ����� */
	for( i=strlen(new); i<D_NAME; ++i )  new[i] = ' ';
	c = dpr( new, D_NAME, &pos, on_control|MS_mode );
	for( i=D_NAME; --i>=0 and new[i]==' ';  new[i]=0 )  ;
	if( c == s_refuse )  goto ref;      /* ����� �� �������������� */
	if( c == s_finish )  goto ref;      /* rygoff 24.07.89         */
	if( c == s_help ) {help("comint",'=');  goto ref;}
    } while ( c!=rn && c!=s_erase && c!=f12 );
    er_flag = (c==s_erase || c==f12);

    if( !strcmp(new,name) ) {             /* ������� ����� �� ��� */
ref:    return(name);
    }

    if( *new==0                           /* ��������, ���� �� ��� ��� */
    or (!er_flag and exist(new)) )  {dpo(bl);  goto inp;}

    if( only_x ) {
	delta = strlen(new) - strlen(name);
	for( c=lind; --c>=0; ) {        /* �������������� ��������� */
	    p = cat[indcat[c]&0377].d_name;
	    if( (lroot = is_boss( name, p ))      /* ��� ���������     */
	    &&  strcmp("..",p)  &&  strcmp(".",p) /*   � �� ����-���   */
	    &&  lroot + delta <= D_NAME           /*   � ������� ��� ��*/
	    &&  !strcmp( name+lroot, new+lroot+delta )
	    &&  lroot+delta+strlen(p+lroot) <= D_NAME ) {
		strcpy( dep, new );
		strcpy( dep+lroot+delta, p+lroot );
		srename( p, dep, er_flag );
	    }
	}
    }

    srename( name, new, er_flag );
    return( new );
}

extern char *name_f, *name_g, *name_F;

static srename(    old, new, er_flag )
    register char *old,*new;
    int                      er_flag;
{
    char *p;
    errno = 0;
    if( er_flag and exist(new) )  srm(new);
    if( errno )  return;

    name_f=new;

    name_g=NULL; /* rygoff 30.10.89 ������ rspec() */
    if( p=lookcom('a'|0x0200,old) ) { INSYMB=' '; exefound(p); }
    else {
	errno = 0;
	if( link(old,new) != -1 )  unlink( old );
	ind_err(old,1);
    }
}
