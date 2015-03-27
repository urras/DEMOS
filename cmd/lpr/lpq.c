/*
 * ��������� ���������� � ��������, ���������
 * ������ �� ���������� ����������.
 */

#include "lpr_def.h"
#include <sys/stat.h>
#include <signal.h>
#include <stdio.h>
#include <ctype.h>
#include "lpq.h"

/* ������ �������� �� ������� ������� */
int     J_NUMS[MAX_JOBS],
       *LST_J_NUM = J_NUMS;

/* ������ �������� �� ������ ������������� */
char   *U_NAMES[MAX_USERS],
      **LST_U_NAME = U_NAMES;

char    FIRST = TRUE,           /* ��� ������ ��������� */
        LONG_FORM = FALSE,      /* ��������� ������ */
       *PROG_NAME = Null,       /* ��� ��������� */
       *SPOOL_DIR = Null;       /* ��� ��������� ����������� */

struct stat st;

main (ac, av)
int     ac;
char  **av;
{
    register char  *s;
    register struct j_queue *job,
                           *queue;
    struct d_stat   status;
    int     rank = 0,
            fd;
    char   *printer = Null;
    extern struct j_queue  *get_queue ();
    extern char *getenv ();

    PROG_NAME = *av++;

 /* ������ �������� ���������� ��������� ������ */
    for (s = *av; s; s = *++av)
        if (*s == '-')
            switch (*++s) {
                case 'P': 
                    if (!printer)
                        printer = ++s;
                    break;
                case 'l': 
                    LONG_FORM = TRUE;
                    break;
                default: 
                    message (PROG_NAME, M_USAGE, Null);
                    exit ERR;
            }
        else
            if (isdigit (*s)) {
                if (LST_J_NUM < &J_NUMS[MAX_JOBS])
                    *LST_J_NUM++ = atoi (s);
            }
            else
                if (LST_U_NAME < &U_NAMES[MAX_USERS])
                    *LST_U_NAME++ = s;

 /* ����������� ����� ����������� ���������� */
    if (printer == NULL
            && (printer = getenv ("PRINTER")) == NULL)
        printer = DEF_PRINTER;

 /* ��������� ����� ��������� ����������� */
    get_spool_name (printer);

 /* ������� � �������� ���������� */
    if (chdir (SPOOL_DIR) < 0) {
        message (PROG_NAME, M_CHDIR, SPOOL_DIR);
        exit ERR;
    }

 /* ��������� ������� ������� */
    if (!(queue = get_queue ())) {
        message (PROG_NAME, M_EMPTY, printer);
        exit OK;
    }

 /* ��������� ���������� � ��������� ������ ���� ����������
    ���������� ����, ������ ���������� ������ SIGTERM, �������
    ������ ��������������. �������� ���������� ����������
    ������ kill ������� � ���, ��� ����� ���������� �
    ���������� � ��� ��������� �������. */
    if (stat (LOCK, &st) < 0)
        message (Null, M_NODEAMON, Null);
    else
        if ((fd = open (LOCK, O_R)) < 0)
            message (PROG_NAME, M_OPEN, LOCK);
        else
            if (read (fd, (char *) & status, sizeof (status))
                    != sizeof (status))
                message (PROG_NAME, M_READ, LOCK);
            else
                if (status.d_pid <= 0
                        || kill (status.d_pid, SIGTERM) < 0)
                    message (PROG_NAME, M_BADLOCK, Null);
                else
                    message (Null, status.d_status, Null);

 /* ����� ���������� � ������� */
    for (job = queue; job; job = job -> jq_next)
        do_inform (++rank, job -> jq_name);
    exit OK;
}

/*
 * ����� ���������� � �������.
 */

do_inform (rank, job)
int     rank;
char   *job;
{
    register int    job_num;    /* ����� ������� */
    register int   *num;        /* ������ ������� */
    register char **name;       /* ������ ������������� */
    int     fd;                 /* ���������� ���������� ����� 
                                */
    off_t total = (long) 0;     /* ����� ������ ������� */
    struct j_header hdr;        /* ��������� */
    struct j_entry  ent;        /* ���� ������ */

 /* �������� ���������� ����� */
    if ((fd = open (job, O_R)) < 0) {
        message (PROG_NAME, M_OPEN, job);
        return;
    }
 /* ������ ��������� ������� */
    if (read (fd, (char *) & hdr, sizeof (hdr))
            != sizeof (hdr)) {
        message (PROG_NAME, M_READ, job);
        goto end;
    }

    job_num = atoi (&job[3]);

 /* �������� ������� */
    for (num = J_NUMS; num < LST_J_NUM; num++)
        if (*num == job_num)
            break;
    if (num >= LST_J_NUM) {
        for (name = U_NAMES; name < LST_U_NAME; name++)
            if (!strcmp (*name, hdr.jh_person))
                break;
        if (name >= LST_U_NAME
                && (LST_J_NUM != J_NUMS
                    || LST_U_NAME != U_NAMES))
            goto end;
    }

 /* ����� ���������� */
    if (LONG_FORM) {
        printf ("%4.4d: %-14.14s [job #%d %s]",
                rank,
                hdr.jh_person,
                job_num,
                hdr.jh_name);
        if (hdr.jh_copies)
            printf (" %d cop%s",
                    hdr.jh_copies,
                    ((hdr.jh_copies == 1)
                        ? "y"
                        : "ies"));
        printf ("\n");
    }
    else {
        if (FIRST) {
            printf ("Rank Owner          job#     Total size\n");
            FIRST = FALSE;
        }
        printf ("%-4.4d %-14.14s %-8.8d ",
                rank,
                hdr.jh_person,
                job_num);
    }

 /* 
  * �������� ������ ������ ������.
  * ���� ������� ��������� ������ ������
  * ����� ������ � �� �������, � ��������� ������,
  * ������ ������������ ����� ������.
  */
    while (read (fd, (char *) & ent, sizeof (ent))
            == sizeof (ent)) {
        if (stat (ent.je_file, &st) < 0) {
            message (PROG_NAME, M_STAT, ent.je_file);
            goto end;
        }
        if (LONG_FORM) {
            printf ("      %-14.14s %D bytes\n",
                    ent.je_name,
                    st.st_size);
        }
        else
            total += st.st_size;
    }
    if (!LONG_FORM)
        printf ("%D\n", total);
end: 
    close (fd);
}

/*
 * ��������� ����� ��������� �����������
 * �� ����� �������� /etc/printcap.
 */

get_spool_name (printer)
register char  *printer;
{
    char   *space,
           *tmp;
    extern char *SPOOL_DIR;
    extern char *calloc (), *pgetstr ();

    space = calloc (BUF_SIZ / 16, sizeof (char));
    tmp = calloc (BUF_SIZ, sizeof (char));

    if (pgetent (tmp, printer) != TRUE) {
        message (PROG_NAME, M_UNKNOWN, printer);
        exit ERR;
    }

    if (!(SPOOL_DIR = pgetstr (D_SD, &space)))
        SPOOL_DIR = DEF_SD;
    cfree (tmp);
}
