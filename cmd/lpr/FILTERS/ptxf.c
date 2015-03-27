/*
 * ������ ��� ��������� ������ � ����������� ������
 * �� PRINTRONIX-P300.
 */

#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#include <sys/lpio.h>
#include "chrtab.h"

#define BYTES_PER_LINE  132
#define BITS_PER_BYTE   6
#define MAXWIDTH        99


struct lpio lp_old,             /* ����������� ��������� ���
                                   ����������� ���������� */
            lp_new;

char    line_buf[16 * BYTES_PER_LINE];

int     lineno;                 /* ����� ������� ������ */
int     width = MAXWIDTH;       /* ����� ������ */
int     length = 49;            /* ����� �������� */
int     indent = 0;             /* ������ */
int     npages = 0;             /* ����� ������� */

char   *name;                   /* ��� ������������ */
char   *acctfile;               /* ������� ���� */

/* ����� ��� ��������� ������� �������� */
char    Bits[] = {
    001,
    002,
    004,
    010,
    020,
    040
};

main (argc, argv)
int     argc;
char   *argv[];
{
    register int    i,          /* ��������� ���������� */
                    ch,         /* ������� ������ */
                    col;        /* ����� ������� ������� */
    char    j,
           *cp;                 /* ��������� ���������� */
    int     done,               /* ����� ����� */
            linedone;           /* ����� ������ */
    extern int  abort ();


 /* ������ ���������� */
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

 /* 
  * ����� ������ �������� ������� ������ SIGINT
  * ���� ���������� �������� ������.
  */
    signal (SIGINT, abort);

 /* ��������� ������� �������� */
    lp_get ();

    done = 0;

    while (!done) {
        col = indent;
        linedone = 0;

    /* ��������� ������ */
        for (cp = line_buf;
                cp < &line_buf[sizeof (line_buf)];
                *cp++ = 0);

        while (!linedone) {
            switch (ch = getchar ()) {
                case EOF: 
                    linedone = done = 1;
                    break;

                case '\f': 
                    lineno = length;
                case '\n': 
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
                    if (col >= width
                            || !isprint (ch)) {
                        col++;
                        break;
                    }
                    if (iscirill (ch))
                        ch ^= 0100;
                    cp = chrtab[ch - ' '];
                    for (j = 0; j < 16; j++) {
                        ch = *cp++;
                        for (i = 7; i >= 0; i--)
                            if ((ch >> i) & 01)
                                line_buf[(j * BYTES_PER_LINE)
                                        + ((col << 3) + 7 - i)
                                        / BITS_PER_BYTE]
                                    |= Bits[((col << 3) + 7 - i)
                                        % BITS_PER_BYTE];
                    }
                    col++;
                    break;
            }
        }
    /* ����� ������ */
        for (cp = line_buf;
                cp < &line_buf[sizeof (line_buf)];
                cp += BYTES_PER_LINE) {
            register char  *c;
            char    f;

            f = 0;
            putchar ('\05');
            for (c = cp;
                    c < (char *) (cp + BYTES_PER_LINE);
                    c++)
                if (!*c)
                    f++;
                else {
                    while (f--)
                        putchar ('\300');
                    f = 0;
                    putchar ((*c | 0300));
                }
            putchar ('\n');
        }
    }

    reset ();
    account ();
    exit (0);
}

/*
 * ���� ��� �������� - �� ������� ���������� ����
 * � ��������� ���� ����� ������� ����.
 */

abort (sig) {
    signal (sig, SIG_IGN);
    reset ();
    account ();
    exit (1);
}

/* ���������� �������� �����. */

account () {
    if (name && acctfile && access (acctfile, 02) >= 0 &&
            freopen (acctfile, "a", stdout) != NULL)
        printf ("%d\t%s\n", npages, name);
}

reset () {
    if (lineno) {
        putchar ('\f');
        npages++;
    }
    fflush (stdout);
    ioctl (fileno (stdout), LPIOCPUT, &lp_old);
}

lp_get () {
    ioctl (fileno (stdout), LPIOCGET, &lp_old);

    lp_new = lp_old;
    lp_new.l_flag |= LP_RAW;
    lp_new.l_col = (width << 3) / 6;
    lp_new.l_line = ((length + 1) << 4);

    ioctl (fileno (stdout), LPIOCPUT, &lp_new);
}
