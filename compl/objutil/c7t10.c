/*
**      Перевод об'ектного файла из типа 0407 в тип 0410
**      (см. a.out(5)).
**
**      c7t10 файл
**
**      Стандартное местоположение в системе:  /etc
**
**      Метод сборки:  cc -O -n -s
**
**      $Header$
**      $Log$
**
*/

static  char Rcs_id[] = "$Header$";

#ifdef sparc
#include "/home/mellorn/zaitcev/d22/include/a.out.h"
#define GET16(v)   ((((v)&0377)<<8)+((v)>>8&0377))
#define PUT16(v)   ((((v)&0377)<<8)+((v)>>8&0377))
#else
#include <a.out.h>
#define GET16(v)   (v)
#define PUT16(v)   (v)
#endif
#include <stdio.h>

struct exec buf;
int     fd;

main (ac, av)
int     ac;
char   *av[];
{
    register unsigned uacc;
    if ((fd = open (av[1], 2)) <= 0) {
        fprintf (stderr, "Can not open %s\n", av[1]);
        exit (1);
    }
    if (read (fd, &buf, (sizeof buf)) != (sizeof buf)) {
        fprintf (stderr, "Can not read %s\n", av[1]);
        exit (1);
    }
    if (GET16(buf.a_magic) != A_MAGIC1) {
        fprintf (stderr, "Bad magic %o\n", buf.a_magic);
        exit (1);
    }
    buf.a_magic = PUT16(A_MAGIC2);
    uacc = GET16(buf.a_text) & 017777;
    uacc += GET16(buf.a_data);
    buf.a_data = PUT16(uacc);
    buf.a_text &= ~GET16(017777);
    lseek (fd, 0l, 0);
    if (write (fd, &buf, (sizeof buf)) != (sizeof buf)) {
        fprintf (stderr, "Write error\n");
        exit (1);
    }
    exit (0);
}
