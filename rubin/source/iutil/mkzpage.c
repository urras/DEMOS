# include	<defines.h>
# include	<access.h>
# include	<sccs.h>

SCCSID(@(#)mkzpage.c   7.1     2/5/81)

/*
 * mk_zpage(b)
 * создать пустую страницу, которой физически не было в файле
 * Назначение: в режиме S_NOZERO при росписи файла пустые первичные
 * листы, у которых "mainpg = thispage+1" (то есть все, кроме
 * последнего), на диск не пишутся. При чтении они опознаюся
 * по "mainpg == 0" (при записи непустого листа 0 заменяется
 * на MAIN_ZERO).
 * Данная программа восстанавливает такой лист в нормальное состояние.
 */
mk_zpage(b)
register struct accbuf *b;
{
    b->mainpg = b->thispage + 1;
    b->linetab[0] = (int)b->firstup - (int)b;
    /*      b->nxtlino = 0;         И так 0 */
    /*      b->ovflopg = 0;         И так 0 */
}
