/*
**      Получение размера программы.
**
**      size [ файл ... ]
**
**      Стандартное местоположение в системе:  /bin
**
**      Метод сборки:  cc -O -n -s
**
**      $Header$
**      $Log$
**
*/

static  char Rcs_id[] = "$Header$";

#include        <stdio.h>
#ifndef sparc
#include        <a.out.h>
#define GET16(v)   (v)       /* Handle values in PDP-11 byte order */
#else
#include        "/home/mellorn/zaitcev/d22/include/a.out.h"
#define GET16(v)   ((((v)&0377)<<8)+((v)>>8&0377))
#endif

int     a_magic[] = {
    A_MAGIC1, A_MAGIC2, A_MAGIC3, A_MAGIC4, 0
};

main (argc, argv)
char  **argv;
{
    struct exec buf;
    long    sum;
    int     gorp,
            i;
    FILE * f;

    if (argc == 1) {
        *argv = "a.out";
        argc++;
        --argv;
    }
    gorp = argc;
    while (--argc) {
        ++argv;
        if ((f = fopen (*argv, "r")) == NULL) {
            printf ("size: %s not found\n", *argv);
            continue;
        }
        fread ((char *) & buf, sizeof (buf), 1, f);
#ifdef sparc
	buf.a_magic = GET16(buf.a_magic);
	buf.a_text = GET16(buf.a_text);
	buf.a_data = GET16(buf.a_data);
	buf.a_bss = GET16(buf.a_bss);
	buf.a_syms = GET16(buf.a_syms);
	buf.a_entry = GET16(buf.a_entry);
	buf.a_flag = GET16(buf.a_flag);
#endif
        for (i = 0; a_magic[i]; i++)
            if (a_magic[i] == buf.a_magic)
                break;
        if (a_magic[i] == 0) {
            printf ("size: %s not an object file\n", *argv);
            fclose (f);
            continue;
        }
        if (gorp > 2)
            printf ("%s: ", *argv);
        printf ("%u+%u+%u = ",
                buf.a_text, buf.a_data, buf.a_bss);
        sum = (long) buf.a_text
            + (long) buf.a_data
            + (long) buf.a_bss;
        printf ("%D = 0%O\n", sum, sum);
        fclose (f);
    }
}
