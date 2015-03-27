/*
 * Филтьр для печатающего устройства.
 * Если вызывается с именем lpf - отрабатывает наложение
 * прописных букв.
 */

#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#include <sys/lpio.h>

#define MAXWIDTH  256           /* Максимальная длина строки 
                                */
#define MAXREP    8             /* Максимальное число
                                   наложений */

struct lpio lp_old,             /* Управляющая структура для
                                   печатающего устройства */
            lp_new;

char    buf[MAXREP][MAXWIDTH];
int     maxcol[MAXREP] = {
    -1
};
int     lineno;
int     width = 132;            /* Длина строки */
int     length = 66;            /* Длина страницы */
int     indent;                 /* Отступ */
int     npages = 1;

char   *name;                   /* Имя пользователя */
char   *acctfile;               /* Учетный файл */

main (argc, argv)
int     argc;
char   *argv[];
{
    register int    i,
                    col;
    register char  *cp;
    int     done,               /* Коней файла */
            linedone,           /* Конец строки */
            lcase = 0,          /* Выдавать прописные буквы
                                   наложением строчных */
            over = 0,           /* Отработка наложений */
            maxrep;             /* Число наложений */
    char    ch,
           *limit;
    extern int  intr ();

    if (!strcmp (*argv, "lpf"))
        lcase = 1;
    while (--argc) {
        if (*(cp = *++argv) == '-') {
            switch (cp[1]) {
                case 'n':       /* Имя пользователя */
                    argc--;
                    name = *++argv;
                    break;

                case 'w':       /* Ширина листа */
                    if ((i = atoi (&cp[2])) > 0
                            && i <= MAXWIDTH)
                        width = i;
                    break;

                case 'l':       /* Длина листа */
                    length = atoi (&cp[2]);
                    break;

                case 'i':       /* Отступ */
                    indent = atoi (&cp[2]);
                    break;

                default: 
                    fprintf (stderr,
                            "Unknown option `%s'.\n",
                            cp);
                    exit (1);
            }
        }
        else
            acctfile = cp;      /* Имя учетного файла */
    }

    if (indent >= width)
        indent = 0;

    signal (SIGINT, intr);
    ioctl (fileno (stdout), LPIOCGET, &lp_old);
    lp_new = lp_old;
    lp_new.l_col = width;
    lp_new.l_line = length;
    ioctl (fileno (stdout), LPIOCPUT, &lp_new);
    for (cp = buf[0], limit = buf[MAXREP]; cp < limit;
            *cp++ = ' ');
    done = 0;

    while (!done) {
        col = indent;
        maxrep = -1;
        linedone = 0;
        while (!linedone) {
            switch (ch = getchar ()) {
                case EOF: 
                    linedone = done = 1;
                    ch = '\n';
                    break;

                case '\f': 
                    lineno = length;
                case '\n': 
                    if (maxrep < 0)
                        maxrep = 0;
                    linedone = 1;
                    break;

                case '\b': 
                    if (--col < indent)
                        col = indent;
                    break;

                case ' ': 
                    col++;
                    break;

                case '\r': 
                    col = indent;
                    break;

                case '\t': 
                    col = ((col - indent) | 07) + indent + 1;
                    break;

                default: 
                    if (col >= width || !isprint (ch)) {
                        col++;
                        break;
                    }
                    if (lcase) {
                        if (isupper (ch)) {
                            over = 1;
                            ch = tolower (ch);
                        }
                        else {
                            register char  *bp;
                            for (bp = "{(})`'~^|!";
                                    *bp && *bp != ch;
                                    bp += 2);
                            if (*bp) {
                                over = 2;
                                ch = *(++bp);
                            }
                        }
                    }
            overflow: 
                    cp = &buf[0][col];
                    for (i = 0; i < MAXREP; i++) {
                        if (i > maxrep)
                            maxrep = i;
                        if (*cp == ' ') {
                            *cp = ch;
                            if (col > maxcol[i])
                                maxcol[i] = col;
                            break;
                        }
                        cp += MAXWIDTH;
                    }
                    if (over) {
                        if (over == 2)
                            ch = '-';
                        over = 0;
                        goto overflow;
                    }
                    col++;
                    break;
            }
        }

    /* Вывод строки */
        for (i = 0; i <= maxrep; i++) {
            for (cp = buf[i], limit = cp + maxcol[i];
                    cp <= limit;) {
                putchar (*cp);
                *cp++ = ' ';
            }
            if (i < maxrep)
                putchar ('\r');
            else
                putchar (ch);
            if (++lineno >= length) {
                npages++;
                lineno = 0;
            }
            maxcol[i] = -1;
        }
    }
    if (lineno) {
        putchar ('\f');
        npages++;
    }
    ioctl (fileno (stdout), LPIOCPUT, &lp_old);
    if (name && acctfile && access (acctfile, 02) >= 0 &&
            freopen (acctfile, "a", stdout) != NULL) {
        printf ("%d\t%s\n", npages, name);
    }
    exit (0);
}

/*
 * Если нас прервали - не забудем продернуть лист
 * и заполнить если нужно учетный файл.
 */

intr (sig) {
    signal (sig, SIG_IGN);
    if (lineno) {
        putchar ('\f');
        npages++;
    }
    ioctl (fileno (stdout), LPIOCPUT, &lp_old);
    if (name && acctfile && access (acctfile, 02) >= 0 &&
            freopen (acctfile, "a", stdout) != NULL) {
        printf ("%d\t%s\n", npages, name);
    }
    exit (1);
}
