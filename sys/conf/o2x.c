/* Преобразование одного magic'а в другой.
 * $Header: o2x.c,v 1.2 88/09/26 12:03:17 korotaev Exp $
 * $Log:	o2x.c,v $
 * Revision 1.2  88/09/26  12:03:17  korotaev
 * Теперь она правильно работает и при уже имеющемся нужном
 * MAGIC_NUMBER'e.
 * 
 * Revision 1.1  88/09/24  21:16:00  ache
 * Initial revision
 * 
 */

#include <stdio.h>
#include <a.out.h>

struct exec exec;
struct xovlhdr  xovlhdr;	/* расписана 0 */

main () {
    register    c, flag;

    fread (&exec, sizeof (exec), 1, stdin);
    if (exec.a_magic != A_MAGIC5 && exec.a_magic != A_MAGIC7) {
	fprintf (stderr, "Not overlaid\n");
	exit (1);
    }
    flag = exec.a_magic == A_MAGIC5;
    exec.a_magic = A_MAGIC7;
    fwrite (&exec, sizeof (exec), 1, stdout);
    if (flag) {
	fread (&xovlhdr, sizeof (struct ovlhdr), 1, stdin);
	fwrite (&xovlhdr, sizeof (xovlhdr), 1, stdout);
    }
    while ((c = getchar ()) != EOF)
	putchar (c);
    exit (0);
}
