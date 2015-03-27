/* === CDEBUGGER === ������ ������ 'n' �� ����� */

/* ����� �����:
        line_ini(����)  ������������� ������ � ����� ������
        line(n)         ������ ������ ����� 'n' � ���� ���������� �����
                        �������: ����� ������ ��� 0 (n ��������)         */

#include <stdio.h>
#define MAXLIN  256                     /* ������������ ����� ������     */
#define NL      10                      /* ������ ����� ������ NL ������ */
#define LFILE   1000                    /* ����. ����� ����� � �������   */

FILE *lfd;                              /* ���������� �����              */
char linebuf [MAXLIN];                  /* ������: �� ����               */
int  linenum;                           /*         �����                 */
long linepos;                           /*         ������� � �����       */

#define LTAB    (LFILE/NL)              /* ����� ������� ������� �����   */
int  ltab;                              /* ������� ������� ������ NL-��  */
long tabpos[LTAB];                      /*         ������                */


line_ini(fd)
    FILE *fd;
{
    lfd=fd;
    linenum = -10;
    ltab=1;  tabpos[0]=0;
}


line(nline)
    register int nline;
{
    register int nnl;
    long ftell();
    --nline;

    if( nline == linenum+1 ) {
        ++nnl;
        if( nnl == (ltab*NL)  &&  ltab<(LTAB-1) ) /* ��������� ����. */
            tabpos[ltab++] = ftell(lfd);
        if( fgets(linebuf, MAXLIN, lfd) == 0 )  goto endfile;

    } else {
        nnl = nline/NL;                 /* ���� ����� ������ ������      */
        if( nnl >= ltab )  nnl=ltab-1;  /*    ==> linepos - ����� ������ */
        linepos = tabpos[nnl];          /*                  nnl          */
        nnl *= NL;
        if( fseek(lfd, linepos, 0) == -1 )  goto endfile;
        if( fgets(linebuf, MAXLIN, lfd) == 0 )  goto endfile;

        while( nnl < nline ) {                     /* ���� ������ ������ */
            ++nnl;
            if( nnl == (ltab*NL)  &&  ltab<(LTAB-1) ) /* ��������� ����. */
                tabpos[ltab++] = ftell(lfd);
            if( fgets(linebuf, MAXLIN, lfd) == 0 )  goto endfile;
        }
    }

    linenum=nnl;
    nnl = strlen( linebuf );    linebuf[nnl-1] = 0;
    return( linebuf );

endfile:
    linenum = -10;   return(0);
}
