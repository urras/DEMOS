/* ����� �������� */

/*
 * $Log:        indcat.c,v $
 * Revision 2.5  89/11/15  16:47:42  rygoff
 * ������� ���� ��������� ����� Paint.
 * 
 * Revision 2.4  89/11/15  15:08:38  rygoff
 * ����� <signal.h>
 * 
 * Revision 2.3  89/11/15  14:21:33  rygoff
 * ����� ������ �������� ��������� ����������
 * �� ���������� ������� ���������.
 * ��������� � ����� ������ ��������� �������� ����������
 * � ���������� ������, ������������� ��������� �������
 * ������� �����, � �� � main().
 * 
 * Revision 2.2  89/11/03  13:21:07  rygoff
 * ����� ������������ setpos() � indname(),
 * ������ ����� ������� � ������. ��� ��������.
 * 
 * Revision 2.1  89/10/30  19:44:35  rygoff
 * ����� �����.
 * ����������� ������ �� ��������.
 * ������ ������������� ������ (��� ������� ������)
 * 
 * Revision 1.2  89/08/19  16:14:05  rygoff
 * ����� ���� �����.
 * ������� ������ �������� ������ ����, � �� ����� �������.
 * �� ������������ �����������.
 * ��������� (� ��� ����) ���������� ����� ���������� �����.
 * ���� ��� �� ������� �� �����, ���������� ������� 
 * � ����������� ...
 * 
 * 
 * 
 */

#include <sys/stat.h>
#include <signal.h>
#include <tty_codes.h>
#include "comint.h"

extern int   mhead, thead, uhead, phead;
extern char *nhead;
extern int   is_shell;
extern boolean Paint;
extern char *geteuid(), *getuname();
extern short largeflag;                         /* Being set in getcat() */

extern onbreak();
#ifdef BSD
extern onchild();
#else
#   define onchild onbreak
#endif
#undef pd

ind_cat() {
    int c;
    register char        *i /* Also used as integer */, *p;
    register struct dir2 *pd;
    char                 *pe;
    int j;
#define ii (int)i
#define iii(x)  (i=(char*)(x))

    dpp(0,0);  dpo(es);                            /* ������ ��������� */

    if( !mhead and nalt!=0 )  {dpp(0,y_head); dpo('*');}

    if( !thead ) {                                 /* ���� */
        dpp( xsize-18, y_head );
        ind_date( time(0) );
#ifdef BSD
        {                                          /* �������� ������� */
            int svec[3];
            dpp( xsize-5, y_head+1 );
            gldav(svec);
            dps( conv(svec[0]>>8,2,10,' ') );  dpo('.');
            dps10((svec[0]&0xff)/26,0);
        }
#endif
    }

    if( !mhead ) {                                 /* �������� */
        dpp( 2, y_head );
        dps( only_x ? "+ " : "  " );
        if( c=strlen(select) )  {dps(select); dpn(D_NAME-c,'^');}
        if( selsuf )  {dpo('.');  dpo(selsuf);}
    }

    if( !uhead ) {                                 /* ��� ������������ */
        char *u;                                   /* ��� �������      */
        u = (u=getenv("USER")) ? u : "Unknown";
        p = (p=getenv("SYSTEM")) ? p : "Localhost";
        i = geteuid();
        dpp( (xsize-strlen(p)-strlen(u)-1-8) / 2, y_head );
        dps( i ? "===" : "<<<" );  dpo( is_shell ? ' ' : '(' );
        dps( p ); dpo( '.' ); dps( u );
        dpo( is_shell ? ' ' : ')' );  dps( i ? "===" : ">>>" );
    }

    p = hcat;                                      /* ��� �������� */
    if( nhead ) {
        pe = index(nhead,'\n');
        ini_shbuf();
        *pe = 0;  cm_line(nhead);  *pe = '\n';
        p = get_shbuf();
        iii(strlen(p));
    } else {    /* rygoff 19.08.89 - ������, ���������� ����� �������. */

        if( (int)iii(strlen(p)) > (xsize-20) ) { /* ��������� ���
                                               �� ������� �� ����� */
            static char hcat1[70];  /* lname � comint.h ��������  */
            char *p_old;            /* � �� ������ ���� ������    */
            p+=1; i-=1;
            while( ii>(xsize-20) ) { p_old=p; p=index(p,'/')+1; i-=p-p_old; }
            strcpy(hcat1,"...");
            strcat(hcat1,p);
            p=hcat1;
            i+=3;
        }
    }
    dpp( (xsize-ii) / 2, y_name );  dps( p );

    if( largeflag ) {           /* God, too many files again ! */
        largeflag=0;
        dpp(0,0);
        dpo(bl);
        dps("Warning: Very large directory. Some files may be not shown.");
        dpo(bl);
    }

    if( !phead )  ind_run();                       /* �������.�������� */


    for( j=0; j<lines; ++j ) {                      /* ����� */
        for( iii(j); ii<nfiles; i+=lines ) {
            struct stat st;
            dpp( x0 + (ii/lines)*width - 1 , y_table + ii%lines );
            pd = item( ii );
            c = pd->d_mark;
            if( c=='1' )  c = '*';
            if( c>='1' && c<='9' )  {dpo(c);    dpo('*');}
            else                    {dpo(' ');  dpo( c ? c : ' ' );}
            p=pd->d_name;
            for(p=pd->d_name;*p;p++) dpo( is_print(*p)? *p : '^');
            if( stat(pd->d_name,&st) != -1
                && (st.st_mode & S_IFMT)==S_IFDIR )
                dpo('/');
        }
    }

    Paint=NO;

#ifdef BSD
    if( !phead ) sigsys(SIGCHLD,onchild);/* ��������� �� ���������� ������� */
#endif

}

