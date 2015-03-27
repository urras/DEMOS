#include <stdio.h>

/*
 * Выдача сообщений.
 */

message (from, what, about)
register char  *from,
               *what,
               *about;
{
    if (from)
        fprintf (stderr, "%s: ", from);
    if (what)
        fprintf (stderr, "%s ", what);
    if (about)
        fprintf (stderr, "\"%s\"", about);
    fprintf (stderr, "\n");
}
