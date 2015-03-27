/* === CDEBUGGER === ЧТЕНИЕ СТРОКИ 'n' ИЗ ФАЙЛА */

/* точки входа:
        line_ini(файл)  инициализация работы с новым файлом
        line(n)         вводит строку номер 'n' в свой внутренний буфер
                        возврат: адрес строки или 0 (n неверное)         */

#include <stdio.h>
#define MAXLIN  256                     /* максимальная длина строки     */
#define NL      10                      /* помним адрес каждой NL строки */
#define LFILE   1000                    /* макс. длина файла в строках   */

FILE *lfd;                              /* дескриптор файла              */
char linebuf [MAXLIN];                  /* строка: ее тело               */
int  linenum;                           /*         номер                 */
long linepos;                           /*         позиция в файле       */

#define LTAB    (LFILE/NL)              /* длина таблицы адресов строк   */
int  ltab;                              /* таблица адресов каждой NL-ой  */
long tabpos[LTAB];                      /*         строки                */


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
        if( nnl == (ltab*NL)  &&  ltab<(LTAB-1) ) /* дополняем табл. */
            tabpos[ltab++] = ftell(lfd);
        if( fgets(linebuf, MAXLIN, lfd) == 0 )  goto endfile;

    } else {
        nnl = nline/NL;                 /* ищем адрес начала строки      */
        if( nnl >= ltab )  nnl=ltab-1;  /*    ==> linepos - адрес строки */
        linepos = tabpos[nnl];          /*                  nnl          */
        nnl *= NL;
        if( fseek(lfd, linepos, 0) == -1 )  goto endfile;
        if( fgets(linebuf, MAXLIN, lfd) == 0 )  goto endfile;

        while( nnl < nline ) {                     /* ищем нужную строку */
            ++nnl;
            if( nnl == (ltab*NL)  &&  ltab<(LTAB-1) ) /* дополняем табл. */
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
