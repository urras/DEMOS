/*
**      Пауза.
**
**      sleep число_секунд
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

main (argc, argv)
char  **argv;
{
    int     c,
            n;
    char   *s;

    n = 0;
    if (argc < 2) {
        printf ("arg count\n");
        exit (0);
    }
    s = argv[1];
    while (c = *s++) {
        if (c < '0' || c > '9') {
            printf ("bad character\n");
            exit (0);
        }
        n = n * 10 + c - '0';
    }
    sleep (n);
}
