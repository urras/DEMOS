#include <stdio.h>
#include <signal.h>
#include <sys/lpio.h>
#include <ctype.h>
#include "chrtab.h"

/* Приведение координат */
#define mapx(x) ((DevRange*((x)-botx)/del)+centx)
#define mapy(y) ((DevRange*(del-1-(y)+boty)/del)-centy)
#define RASTER  "raster"

/* Тип линии */
#define SOLID           -1      /* Непрерывная */
#define DOTTED          014     /* Точечная */
#define SHORTDASHED     034     /* Короткий штрих */
#define DOTDASHED       054     /* Штрихпунктир */
#define LONGDASHED      074     /* Длинный штрих */

int     Raster;                 /* Дескриптор временного
                                   файла */

/* Параметры устройства */
int     DevRange = 792,         /* Размер поля в точках
                                   (квадрат) */
        Bits_per_byte = 6,      /* Число значащих бит в байте
                                   растра */
        Bytes_per_line = 132;   /* Число байт в строке растра 
                                */

/* Маски для установки битовых значений */
char    Bits[] = {
    001,
    002,
    004,
    010,
    020,
    040
};

char    Space[13068];           /* Буфер на часть листа. */
char    Part;                   /* Какая часть листа в памяти 
                                */

int     linmod = SOLID;         /* Тип линии по умолчанию */

/* Текущие координаты */
int     lastx;
int     lasty;

/* Координаты начала листа */
double  topx;
double  topy;

/* Координаты конца листа */
double  botx;
double  boty;

/* Координаты центра */
int     centx = 0;
int     centy = 0;

/* Разрешение по осям */
double  delx;
double  dely;
double  del;

int     warned = 0;             /* Предупредительная
                                   диагностика при появлении
                                   команд построения дуг */

char   *name,                   /* Имя пользователя для
                                   статистики */
       *acctfile;               /* Имя учетного файла */

main (ac, av)
int     ac;
char  **av;
{
    register char  *cp;
    register int    again;
    extern  abort ();

    while (*++av)
        if (**av == '-') {
            switch (av[0][1]) {
                case 'n': 
                    name = *++av;
                    break;
            }
        }
        else
            acctfile = *av;

 /* Начальная инициализация */
    topx = topy = DevRange;
    botx = boty = 0;
    delx = dely = del = DevRange;
    centx = (DevRange - mapx (topx)) / 2;
    centy = mapy (topy) / 2;

 /* Создание временного файла */
    if ((Raster = creat (RASTER, 0666)) < 0) {
        perror ("ptxplotf: creat");
        exit (1);
    }
    else {
    /* Файл бит-карты нужно открыть для чтения/записи */
        signal (SIGINT, abort);
        close (Raster);
        if ((Raster = open (RASTER, 2)) < 0) {
            perror ("ptxplotf: open");
            abort (SIGINT);
        }
    }

    do {
        for (cp = Space;
                cp < &Space[sizeof (Space)];
                *cp++ = 0300);
        for (Part = 0;
                Part < (DevRange
                    / (sizeof (Space)
                        / Bytes_per_line));
                Part++, put_part (Part));
        Part = 0;
        again = getpict ();
        putpict ();
    } while (again);

 /* Запись статистики, удаление временного файла */
    signal (SIGINT, SIG_IGN);
    close (Raster);
    unlink (RASTER);
    account (name, acctfile);
    exit (0);
}

/* Запись статистики */
account (who, acct)
char   *who,
       *acct;
{
    register    FILE * a;

    if (!who || !acct)
        return;
    if (access (acct, 02)
            || (a = fopen (acct, "a")) == NULL)
        return;
/*  fprintf (a, "t%6.2f\t", (lines / 200.0) / PAGE_LINES);  */
    fprintf (a, "%s\n", who);
    fclose (a);
}

/*
 * Построение картинки.
 * Простой интерпретатор команд библиотеки plot.
 */
getpict () {
    register    x1,
                y1,
                done1 = 0;
    int     radius,
            startx,
            starty,
            endx,
            endy;

    for (;;)
        switch (x1 = getc (stdin)) {

            case '\n':          /* Игнорировать лишние
                                   переводы строки и нулевые
                                   символы */
            case '\0': 
                continue;

            case 's': 
                botx = getw (stdin);
                boty = getw (stdin);
                topx = getw (stdin);
                topy = getw (stdin);
                delx = topx - botx;
                dely = topy - boty;
                if (dely / delx > 1.)
                    del = dely;
                else
                    del = delx;
                centx = 0;
                centx = (DevRange - mapx (topx)) / 2;
                centy = 0;
                centy = mapy (topy) / 2;
                if (done1)
                    return (1);
                continue;

            case 'b': 
                x1 = getc (stdin);
                continue;

            case 'l': 
                done1 |= 01;
                x1 = mapx (getw (stdin));
                y1 = mapy (getw (stdin));
                lastx = mapx (getw (stdin));
                lasty = mapy (getw (stdin));
                line (x1, y1, lastx, lasty);
                continue;

            case 'c': 
                done1 |= 01;
                x1 = mapx (getw (stdin));
                y1 = mapy (getw (stdin));
                radius = mapx (getw (stdin));
                circle (x1, y1, radius);
                continue;

            case 'a': 
                x1 = mapx (getw (stdin));
                y1 = mapy (getw (stdin));
                startx = mapx (getw (stdin));
                starty = mapy (getw (stdin));
                endx = mapx (getw (stdin));
                endy = mapy (getw (stdin));
                if (!warned) {
                    fprintf (stderr,
                            "ptxplotf: arcs are unimplemented\n");
                    warned++;
                }
                continue;

            case 'm': 
                lastx = mapx (getw (stdin));
                lasty = mapy (getw (stdin));
                continue;

            case 't': 
                done1 |= 01;
                while ((x1 = getc (stdin)) != '\n')
                    plotch (x1);
                continue;

            case 'e': 
                if (done1)
                    return (1);
                continue;

            case 'p': 
                done1 |= 01;
                lastx = mapx (getw (stdin));
                lasty = mapy (getw (stdin));
                point (lastx, lasty);
                point (lastx + 1, lasty);
                point (lastx, lasty + 1);
                point (lastx + 1, lasty + 1);
                continue;

            case 'n': 
                done1 |= 01;
                x1 = mapx (getw (stdin));
                y1 = mapy (getw (stdin));
                line (lastx, lasty, x1, y1);
                lastx = x1;
                lasty = y1;
                continue;

            case 'f': 
                getw (stdin);
                getc (stdin);
                switch (getc (stdin)) {
                    case 't': 
                        linmod = DOTTED;
                        break;
                    default: 
                    case 'i': 
                        linmod = SOLID;
                        break;
                    case 'g': 
                        linmod = LONGDASHED;
                        break;
                    case 'r': 
                        linmod = SHORTDASHED;
                        break;
                    case 'd': 
                        linmod = DOTDASHED;
                        break;
                }
                while ((x1 = getc (stdin)) != '\n')
                    if (x1 == EOF)
                        return (0);
                continue;

            case 'd': 
                getw (stdin);
                getw (stdin);
                getw (stdin);
                x1 = getw (stdin);
                while (--x1 >= 0)
                    getw (stdin);
                continue;

            case EOF: 
                put_part (Part);
                return (0);

            default: 
                fprintf (stderr,
                        "ptxplotf: input format error %c(%o)\n",
                        x1, x1);
                exit (2);
        }
}

plotch (ch)
register    ch;
{
    register char  *cp;
    register    j;
    int     i;

    if (isprint (ch)) {
        if (iscirill (ch))
            ch ^= 0100;
        cp = chrtab[ch - ' '];
        for (i = -8; i < 8; i++) {
            ch = *cp++;
            for (j = 7; j >= 0; j--)
                if ((ch >> j) & 01)
                    point (lastx + 4 - j, lasty + i);
        }
    }
    lastx += 8;
}

/* Построение линии */
line (x0, y0, x1, y1)
register    x0,
            y0;
{
    int     dx,
            dy;
    int     xinc,
            yinc;
    register    res1;
    int     res2;
    int     slope;

    xinc = 1;
    yinc = 1;
    if ((dx = x1 - x0) < 0) {
        xinc = -1;
        dx = -dx;
    }
    if ((dy = y1 - y0) < 0) {
        yinc = -1;
        dy = -dy;
    }
    slope = xinc * yinc;
    res1 = 0;
    res2 = 0;
    if (dx >= dy)
        while (x0 != x1) {
            if ((x0 + slope * y0) & linmod)
                point (x0, y0);
            if (res1 > res2) {
                res2 += dx - res1;
                res1 = 0;
                y0 += yinc;
            }
            res1 += dy;
            x0 += xinc;
        }
    else
        while (y0 != y1) {
            if ((x0 + slope * y0) & linmod)
                point (x0, y0);
            if (res1 > res2) {
                res2 += dy - res1;
                res1 = 0;
                x0 += xinc;
            }
            res1 += dx;
            y0 += yinc;
        }
    if ((x1 + slope * y1) & linmod)
        point (x1, y1);
}

#define labs(a) (a >= 0 ? a : -a)

/* Построение окружности */
circle (x, y, c) {
    register    dx,
                dy;
    long    ep;
    int     de;

    dx = 0;
    ep = 0;
    for (dy = c; dy >= dx; dy--) {
        for (;;) {
            point (x + dx, y + dy);
            point (x - dx, y + dy);
            point (x + dx, y - dy);
            point (x - dx, y - dy);
            point (x + dy, y + dx);
            point (x - dy, y + dx);
            point (x + dy, y - dx);
            point (x - dy, y - dx);
            ep += 2 * dx + 1;
            de = -2 * dy + 1;
            dx++;
            if (labs (ep) >= labs (ep + de)) {
                ep += de;
                break;
            }
        }
    }
}

/*
 * Построение точки.
 * Координаты точек должны быть в диапазоне
 * 0 <= x (or y) < DevRange.
 */
point (x, y)
register int    x,
                y;
{
    register char  *byte;

    if (x >= 0 && x < DevRange
            && y >= 0 && y < DevRange) {
        y = get_list (y);
        byte = Space
            + (y * Bytes_per_line
                + x / Bits_per_byte);
        *byte |= Bits[x % Bits_per_byte];
    }
}

get_list (y)
register int    y;
{
    if (y >= ((Part + 1)
                * (sizeof (Space)
                    / Bytes_per_line))
            || y < (Part
                * (sizeof (Space)
                    / Bytes_per_line))) {
    /* Смена листа */
        put_part (Part);
        Part = y
            / (sizeof (Space)
                / Bytes_per_line);
        get_part (Part);
    }
    return (y
            - (Part
                * (sizeof (Space)
                    / Bytes_per_line)));
}

/* Чтение части растра с диска */
get_part (part)
register int    part;
{
    if (lseek (Raster,
                (long) ((long) (Part) * sizeof (Space)), 0)
            < 0) {
        perror ("ptxplotf: lseek");
        abort (SIGINT);
    }
    if (read (Raster, Space, sizeof (Space))
            != sizeof (Space)) {
        perror ("ptxplotf: read");
        abort (SIGINT);
    }
}

/* Сброс части растра на диск */
put_part (part)
register int    part;
{
    if (lseek (Raster,
                (long) ((long) (part) * sizeof (Space)), 0)
            < 0) {
        perror ("ptxplotf: lseek");
        abort (SIGINT);
    }
    if (write (Raster, Space, sizeof (Space))
            != sizeof (Space)) {
        perror ("ptxplotf: write");
        abort (SIGINT);
    }
}

putpict () {
    register char  *line,
                   *c;
    register int    f;
    struct lpio lpold,
                lpnew;

 /* Установка режима графического вывода */
    ioctl (fileno (stdout), LPIOCGET, &lpold);
    lpnew = lpold;
    lpnew.l_flag |= LP_RAW;
    lpnew.l_ind = 0;
    lpnew.l_line = 1024;
    lpnew.l_col = 136;
    ioctl (fileno (stdout), LPIOCPUT, &lpnew);

 /* Печать картинки */
    for (Part = 0;
            Part < (DevRange
                / (sizeof (Space)
                    / Bytes_per_line));
            Part++) {
        get_part (Part);
        for (line = Space;
                line < &Space[sizeof (Space)];
                line += Bytes_per_line) {
            f = 0;
            putchar ('\05');
            for (c = line;
                    c < (char *) (line + Bytes_per_line);
                    c++)
                if ((*c & 0377) == 0300)
                    f++;
                else {
                    while (f--)
                        putchar ('\300');
                    f = 0;
                    putchar (*c);
                }
            putchar ('\n');
        }
    }
    putchar ('\f');
    fflush (stdout);
    ioctl (fileno (stdout), LPIOCPUT, &lpold);
    return;
}

/* Обработка прерывания */
abort (sig) {
    signal (sig, SIG_IGN);
    close (Raster);
    unlink (RASTER);
    exit (1);
};
