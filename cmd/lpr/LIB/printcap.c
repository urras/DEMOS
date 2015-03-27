#define MAXHOP  32              /* ����� ��������������� ����
                                   tc= */

#define CAPFILE "/etc/printcap"
#define O_R     0
#define BUFSIZ  512
#define STDERR  2

#include <ctype.h>
/*
 * printcap - ������������ ��� ������ � ������ ��������
 *            ������������ ������������ ��������� lpr.
 */

static char *tbuf;
static int  hopcount;           /* ������� ������
                                   ��������������� */

char   *pskip ();
char   *pgetstr ();
char   *pdecode ();
char   *getenv ();

/*
 * ��������� �������� ����������� ���������� � �����
 * bp.
 */
pgetent (bp, name)
char   *bp,
       *name;
{
    register char  *cp;
    register int    c;
    register int    i = 0,
                    cnt = 0;
    char    ibuf[BUFSIZ];
    int     tf = 0;

    tbuf = bp;
    if ((tf = open (CAPFILE, O_R)) < 0)
        return (-1);
    for (;;) {
        cp = bp;
        for (;;) {
            if (i == cnt) {
                if ((cnt = read (tf, ibuf, BUFSIZ)) <= 0) {
                    close (tf);
                    return (0);
                }
                i = 0;
            }
            c = ibuf[i++];
            if (c == '\n') {
                if (cp > bp && cp[-1] == '\\') {
                    cp--;
                    continue;
                }
                break;
            }
            if (cp >= bp + BUFSIZ) {
                write (STDERR,
                        "Printcap entry too long.\n",
                        25);
                break;
            }
            else
                *cp++ = c;
        }
        *cp = 0;

    /* 
     * ����� ������ ����������.
     */
        if (pnamatch (name)) {
            close (tf);
            return (pnchktc ());
        }
    }
}

/*
 * ��������� ���������� � �������� ����������
 * ����� ���� ����������� ���� tc=xxx. ��� ��������,
 * ��� ��� ��������� �������������� ��������� �������
 * �� �������� ���������� xxx.
 */
pnchktc () {
    register char  *p,
                   *q;
    char    tcname[16];         /* ��� ���������� */
    char    tcbuf[BUFSIZ];
    char   *holdtbuf = tbuf;
    int     l;

    p = tbuf + strlen (tbuf) - 2;/* ��������� �������� */
    while (*--p != ':')
        if (p < tbuf) {
            write (STDERR,
                    "Bad termcap entry\n", 18);
            return (0);
        }
    p++;
    if (p[0] != 't' || p[1] != 'c')
        return (1);
    strcpy (tcname, p + 3);
    q = tcname;
    while (q && *q != ':')
        q++;
    *q = 0;
    if (++hopcount > MAXHOP) {
        write (STDERR,
                "Infinite tc= loop\n", 18);
        return (0);
    }
    if (pgetent (tcbuf, tcname) != 1)
        return (0);
    for (q = tcbuf; *q != ':'; q++);
    l = p - holdtbuf + strlen (q);
    if (l > BUFSIZ) {
        write (STDERR,
                "Termcap entry too long\n", 23);
        q[BUFSIZ - (p - tbuf)] = 0;
    }
    strcpy (p, q + 1);
    tbuf = holdtbuf;
    return (1);
}

/*
 * �������� ���� �������������� ���� ����������.
 * ������ ���� �������� �������� ����� ����������
 * ����������� �������� '|'.
 */
pnamatch (np)
char   *np;
{
    register char  *Np,
                   *Bp;

    Bp = tbuf;
    if (*Bp == '#')
        return (0);
    for (;;) {
        for (Np = np; *Np && *Bp == *Np; Bp++, Np++)
            continue;
        if (*Np == 0
                && (*Bp == '|'
                    || *Bp == ':'
                    || *Bp == 0))
            return (1);
        while (*Bp && *Bp != ':' && *Bp != '|')
            Bp++;
        if (*Bp == 0 || *Bp == ':')
            return (0);
        Bp++;
    }
}

/*
 * ������� ���������� ����.
 * ��� ��� ���� ����������� ��������� ':', �� � ��������
 * ������ ':' ����� �������������� ������ � ���� ����.
 */
static char *
            pskip (bp)
register char  *bp;
{

    while (*bp && *bp != ':')
        bp++;
    if (*bp == ':')
        bp++;
    return (bp);
}

/*
 * ��������� ��������� �������� ����:
 *      li#80
 * ���� ��� ��������� ����� �������� �������� �����������
 * ������������ -1. ���� ����� � �������� ���������� � ����,
 * ��������� ��� ����� ������������.
 */
pgetnum (id)
char   *id;
{
    register int    i,
                    base;
    register char  *bp = tbuf;

    for (;;) {
        bp = pskip (bp);
        if (*bp == 0)
            return (-1);
        if (*bp++ != id[0] || *bp == 0 || *bp++ != id[1])
            continue;
        if (*bp == '@')
            return (-1);
        if (*bp != '#')
            continue;
        bp++;
        base = 10;
        if (*bp == '0')
            base = 8;
        i = 0;
        while (isdigit (*bp))
            i *= base, i += *bp++ - '0';
        return (i);
    }
}

/*
 * ��������� �������� ��������.
 */
pgetflag (id)
char   *id;
{
    register char  *bp = tbuf;

    for (;;) {
        bp = pskip (bp);
        if (!*bp)
            return (0);
        if (*bp++ == id[0] && *bp != 0 && *bp++ == id[1]) {
            if (!*bp || *bp == ':')
                return (1);
            else
                if (*bp == '@')
                    return (0);
        }
    }
}

/*
 * ��������� ��������� �������� ����:
 *      cl=^Z
 * ������ ������������ � ������ area.
 * ������� �������� �� ���������� ������ �� ��������.
 */
char   *
        pgetstr (id, area)
char   *id,
      **area;
{
    register char  *bp = tbuf;

    for (;;) {
        bp = pskip (bp);
        if (!*bp)
            return (0);
        if (*bp++ != id[0] || *bp == 0 || *bp++ != id[1])
            continue;
        if (*bp == '@')
            return (0);
        if (*bp != '=')
            continue;
        bp++;
        return (pdecode (bp, area));
    }
}

/*
 * �������������.
 */
static char *
            pdecode (str, area)
register char  *str;
char  **area;
{
    register char  *cp;
    register int    c;
    register char  *dp;
    int     i;

    cp = *area;
    while ((c = *str++) && c != ':') {
        switch (c) {

            case '^': 
                c = *str++ & 037;
                break;

            case '\\': 
                dp = "E\033^^\\\\::n\nr\rt\tb\bf\f";
                c = *str++;
        nextc: 
                if (*dp++ == c) {
                    c = *dp++;
                    break;
                }
                dp++;
                if (*dp)
                    goto nextc;
                if (isdigit (c)) {
                    c -= '0', i = 2;
                    do
                        c <<= 3, c |= *str++ - '0';
                    while (--i && isdigit (*str));
                }
                break;
        }
        *cp++ = c;
    }
    *cp++ = 0;
    str = *area;
    *area = cp;
    return (str);
}
