#include "lpr_def.h"
#include <stat.h>

static struct j_queue  *QUEUE = (struct j_queue *) 0;

/*
 * Получение очереди заданий на печать
 */

struct j_queue *
                get_queue () {
    register int    fd;
    register struct j_queue *job;
    struct dir  d_ent;
    struct stat st;
    extern char *calloc ();
    extern  add_queue ();

    if ((fd = open (".", O_R)) < 0)
        return ((struct j_queue *) 0);

 /* Пропускаем  "." и ".."  */
    lseek (fd, (long) (2 * sizeof (struct dir)), 0);

    if (!QUEUE)
        QUEUE = (struct j_queue *) calloc (1, sizeof (struct j_queue));
    else
        free_queue ();

    while (read (fd, (char *) & d_ent, sizeof (d_ent))
            == sizeof (d_ent)) {
        if (!d_ent.d_ino
                || strncmp (d_ent.d_name, "cf", 2)
                || stat (d_ent.d_name, &st) < 0)
            continue;

        if (!(job = (struct j_queue *)
            calloc (1, sizeof (struct j_queue))))
            break;
        strcpy (job -> jq_name, d_ent.d_name);
        job -> jq_ctime = st.st_ctime;
        add_queue (job);
    }
    close (fd);
    return (QUEUE -> jq_next);
}

add_queue (add)
register struct j_queue *add;
{
    register struct j_queue *job;

    for (job = QUEUE; job; job = job -> jq_next)
        if (!job -> jq_next
                || add -> jq_ctime
                < (job -> jq_next) -> jq_ctime) {
            add -> jq_next = job -> jq_next;
            job -> jq_next = add;
            break;
        }
}

free_queue () {
    register struct j_queue *job,
                           *tmp;

    job = QUEUE -> jq_next;
    while (job) {
        tmp = job;
        job = job -> jq_next;
        cfree (tmp);
    }
    QUEUE -> jq_next = (struct j_queue *) 0;
}
