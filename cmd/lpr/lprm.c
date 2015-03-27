/*
 * �������� ������� �� ������� �� ������.
 */

#include "lpr_def.h"
#include <sys/stat.h>
#include <signal.h>
#include <pwd.h>
#include <stdio.h>
#include <ctype.h>
#include "lprm.h"

/* ������ �������� �� ������� ������� */
int     J_NUMS[MAX_JOBS],
       *LST_J_NUM = J_NUMS;

/* ������ �������� �� ������ ������������� */
char   *U_NAMES[MAX_USERS],
      **LST_U_NAME = U_NAMES;

char    ALL_REMOVE = FALSE,
       *PROG_NAME = Null,       /* ��� ��������� */
       *SPOOL_DIR = Null,       /* ��� ��������� ����������� */
        SUPER_USER = FALSE;     /* ������� ���������
                                   �����������������
                                   ������������ */

struct passwd  *USER;           /* ���������� � ������������ */

struct stat st;

main (ac, av)
int     ac;
char  **av;
{
    register char  *s;
    register struct j_queue *job,
                           *queue;
    char   *printer = Null;
    int     fd;
    struct d_stat   status;
    extern struct j_queue  *get_queue ();
    extern char *getenv ();
    extern struct passwd   *get_usr_id ();

    PROG_NAME = *av++;

 /* ����������� ����� ������������ */
    if (!(USER = get_usr_id ())) {
        message (PROG_NAME, M_USER, Null);
        exit ERR;
    }

    SUPER_USER = (getuid () == 0);

 /* ������ �������� ���������� ��������� ������ */
    for (s = *av; s; s = *++av)
        if (*s == '-')
            switch (*++s) {
                case 'P': 
                    if (!printer)
                        printer = ++s;
                    break;
                case '\0': 
                    ALL_REMOVE = TRUE;
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
                if (LST_U_NAME < &U_NAMES[MAX_USERS]
                        && (SUPER_USER
                            || !strcmp (s, USER -> pw_name)))
                    *LST_U_NAME++ = s;

 /* �������� ������������ ������� */
    if (!ALL_REMOVE
            && LST_J_NUM == J_NUMS
            && LST_U_NAME == U_NAMES) {
        message (PROG_NAME, M_REQUEST, Null);
        exit ERR;
    }

 /* ��������� ����� ����������� ���������� */
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

 /* ��������� ������� ������ */
    if (!(queue = get_queue ())) {
        message (PROG_NAME, M_EMPTY, printer);
        exit OK;
    }

 /* ����� ���������� � ������� */
    for (job = queue; job; job = job -> jq_next)
        do_remove (job -> jq_name);

 /* ����������� ����� � ������������� ����������. ����
    ���������� ����������� ����, �� �������� � �������� � �����
    ������� (������) ���������� ������ SIGHUP, �� ��������
    ����� ������ ������������� ������� ������� � ����
    ���������� ��������� ������� ������. */
    if (stat (LOCK, &st) >= 0
            && (fd = open (LOCK, O_R)) >= 0
            && read (fd, (char *) & status, sizeof (status))
            == sizeof (status)
            && status.d_pid > 0)
        kill (status.d_pid, SIGHUP);
    exit OK;
}

/*
 * �������� �������.
 */

do_remove (job)
char   *job;
{
    register int    job_num;    /* ����� ������� */
    register int   *num;        /* ������ ���������� ������� */
    register char **name;       /* ������ ������������� */
    int     fd;                 /* ���������� ���������� ����� 
                                */
    struct j_header hdr;        /* ��������� */
    struct j_entry  ent;        /* ���� ������ */

 /* �������� ���������� ����� */
    if ((fd = open (job, O_R)) < 0) {
        message (PROG_NAME, M_OPEN, job);
        return;
    }
 /* ������ ��������� */
    if (read (fd, (char *) & hdr, sizeof (hdr))
            != sizeof (hdr)) {
        message (PROG_NAME, M_READ, job);
        close (fd);
        return;
    }

 /* ������� ������������ ����� ������� ������ ���� ������� */
    if (!SUPER_USER
            && strcmp (hdr.jh_person, USER -> pw_name)) {
        close (fd);
        return;
    }

    job_num = atoi (&job[3]);

 /* �������� ������ ������� */
    if (!ALL_REMOVE) {
        for (num = J_NUMS; num < LST_J_NUM; num++)
            if (*num == job_num)
                break;
        if (num >= LST_J_NUM) {
            for (name = U_NAMES; name < LST_U_NAME; name++)
                if (!strcmp (*name, hdr.jh_person))
                    break;
            if (name >= LST_U_NAME
                    && (LST_U_NAME != U_NAMES
                        || LST_J_NUM != J_NUMS)) {
                close (fd);
                return;
            }
        }
    }

 /* �������� ������ ������ */
    while (read (fd, (char *) & ent, sizeof (ent))
            == sizeof (ent))
        if (unlink (ent.je_file) < 0) {
            message (PROG_NAME, M_UNLINK, ent.je_file);
            exit ERR;
        }
    close (fd);

 /* �������� ���������� ����� */
    if (unlink (job) < 0) {
        message (PROG_NAME, M_UNLINK, job);
        exit ERR;
    }
 /* ����������� ������������ �� �������� */
    message (Null, M_MSGS, &job[3]);
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
    free (tmp);
}
