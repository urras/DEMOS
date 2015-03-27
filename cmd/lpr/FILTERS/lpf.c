/*
 * ������ ��� ����������� ����������.
 * ���� ���������� � ������ lpf - ������������ ���������
 * ��������� ����.
 */

#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#include <sys/lpio.h>

#define MAXWIDTH  256           /* ������������ ����� ������ 
                                */
#define MAXREP    8             /* ������������ �����
                                   ��������� */

struct lpio lp_old,             /* ����������� ��������� ���
                                   ����������� ���������� */
            lp_new;

char    buf[MAXREP][MAXWIDTH];
int     maxcol[MAXREP] = {
    -1
};
int     lineno;
int     width = 132;            /* ����� ������ */
int     length = 66;            /* ����� �������� */
int     indent;                 /* ������ */
int     npages = 1;

char   *name;                   /* ��� ������������ */
char   *acctfile;               /* ������� ���� */

main (argc, argv)
int     argc;
char   *argv[];
{
    register int    i,
                    col;
    register char  *cp;
    int     done,               /* ����� ����� */
            linedone,           /* ����� ������ */
            lcase = 0,          /* �������� ��������� �����
                                   ���������� �������� */
            over = 0,           /* ��������� ��������� */
            maxrep;             /* ����� ��������� */
    char    ch,
           *limit;
    extern int  intr ();

    if (!strcmp (*argv, "lpf"))
        lcase = 1;
    while (--argc) {
        if (*(cp = *++argv) == '-') {
            switch (cp[1]) {
                case 'n':       /* ��� ������������ */
                    argc--;
                    name = *++argv;
                    break;

                case 'w':       /* ������ ����� */
                    if ((i = atoi (&cp[2])) > 0
                            && i <= MAXWIDTH)
                        width = i;
                    break;

                case 'l':       /* ����� ����� */
                    length = atoi (&cp[2]);
                    break;

                case 'i':       /* ������ */
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
            acctfile = cp;      /* ��� �������� ����� */
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

    /* ����� ������ */
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
 * ���� ��� �������� - �� ������� ���������� ����
 * � ��������� ���� ����� ������� ����.
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
