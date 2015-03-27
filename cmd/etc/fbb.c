/*
**      Поиск плохих блоков на диске.
**
**      fbb [ -sdl ] имя_устройства ...
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

#include <stdio.h>

int     ex_st = 0;              /* Exit status */
int     bs,
        ds = 4872,
        f_l = 0,
        f_s = 0,
        numb;
int     arr[16 * 512];

main (argc, argv)
int     argc;
char   *argv[];
{
    char    c;
    int     i,
            j,
            k,
            n_f = 0;

    for (i = 1; i < argc; ++i) {
        if (*argv[i] != '-')
            argv[n_f++] = argv[i];
        else {
            k = i;
            for (j = 1; (c = *(argv[k] + j)) != 0; ++j) {
                switch (c) {
                    case 'D': 
                    case 'd': 
                        ds = atoi (argv[++i]);
                        break;
                    case 'L': 
                    case 'l': 
                        f_l = 1;
                        break;
                    case 'S': 
                    case 's': 
                        f_s = 1;
                        break;
                    default: 
                        err ("Usage: fbb [-d devsize] [-f] [-l] [-s] [files...]");
                        break;
                }
            }
        }
    }

    if (f_l == f_s == 1)
        f_l = f_s = 0;
    if (f_l)
        bs = 1;
    else
        bs = 16;

    if (ds < bs)
        ds = 2 * bs;
    numb = ds / bs;
    if (n_f < 1)
        tst ("/dev/rrk0");
    else
        for (i = 0; i < n_f; ++i)
            tst (argv[i]);
    exit (ex_st);
}


tst (file)
char   *file;
{
    int     block,
            fd,
            st,
            e;

    fd = open (file, 0);
    if (!f_s)
        printf ("%s:", file);
    if (f_l)
        printf ("\n");

    e = 0;
    for (block = 0; block < numb; ++block) {
        st = read (fd, arr, bs * 512);
        if (st < 0) {
            ex_st = 1;
            e = 1;
            if (f_l)
                printf ("Bad block:%d\n", block);
            if (f_s)
                exit (1);
        }
    }
    if ((!f_s) && (!f_s)) {
        if (e)
            printf ("bad\n");
        else
            printf ("good\n");
    }
}


err (mess)                      /* Print error message & die  
                                */
char   *mess;
{
    fprintf (stderr, " ---");
    fprintf (stderr, mess);
    fprintf (stderr, "\n");
    exit (1);
}
