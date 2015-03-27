/*
 *      Данная программа предназначена для получения
 *      из оперативного руководства по системе томов
 *      со сквозной нумерацией.
 *      Реализован частный случай, исключительно для
 *      подготовки  типографского  варианта описания
 *      по системе.
 *      Вызов:
 *              fmt [filename ... ]
 *      Если не указано ни одного имени файла, исполь-
 *      зуется стандартный файл ввода. Результат ра-
 *      боты выдается в стандартный файл вывода.
 */

#include <stdio.h>
#include <ctype.h>

#define FALSE 0
#define TRUE  1
#define MAXSIZ 512
#define NPAGE 1

main (ac, av)
int     ac;
char  **av;
{
    register int    c,
                    page = NPAGE - 1,
                    i,
                    str;
    char    s[MAXSIZ];
    if (ac == 1) goto alone;
    while (*(++av)) {
        if (freopen (*av, "r", stdin) == NULL)
            fprintf (stderr, "!!! file: %s can't open.\n", *av);
        else {
    alone:
            str = FALSE;
            i = 0;
            while ((c = getchar ()) != EOF) {
                if (c != '\n') {
                    s[i++] = c;
                    if (!isdigit (c) && c != ' ' && c != '\t')
                        str = TRUE;
                    if (i >= MAXSIZ)
                        fprintf (stderr, "Line length so much.\n"), exit (1);
                }
                else {
                    if (!str && isdigit (s[i - 1]) && (i == 43 || i <= 2)) {
                        page++;
                        if (page & 01)
                            printf ("%43d\n", page);
                        else
                            printf ("%-43d\n", page);
                    }
                    else {
                        s[i] = '\0';
                        puts (s);
                    }
                    str = FALSE;
                    i = 0;
                }
            }
        }
    }
}
