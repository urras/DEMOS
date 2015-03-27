/*
 * Управление системной печатью.
 */

#include "lpr_def.h"
#include <sys/stat.h>
#include <signal.h>
#include <stdio.h>
#include <ctype.h>
#include "lpc.h"

char   *PROG_NAME = Null;
char    SUSER = FALSE;
char   *printer = Null;

struct stat st;

main (ac, av)
int     ac;
register char  **av;
{
    register struct _coms  *c;
    register    (*command) () = do_inform;
    extern char *getenv ();

    PROG_NAME = *av++;

    SUSER = (getuid () == 0);

    umask (0);

 /* Определение команды */
    if (*av) {
        for (c = COMS; c < ENDCOMS; c++)
            if (!strcmp (c -> cm_name, *av))
                break;
        if (c >= ENDCOMS) {
            message (PROG_NAME, M_COM, *av);
            exit ERR;
        }
        if (!SUSER && c -> cm_suser) {
            message (PROG_NAME, M_SUSER, *av);
            exit ERR;
        }
        command = c -> cm_func;
        av++;
    }

 /* 
  * Выполнение команды для заданного списка
  * печатающих устройств.
  */
    if (!*av) {
        if (!(printer = getenv ("PRINTER")))
            printer = DEF_PRINTER;
        if (!get_caps (printer)) {
            message (PROG_NAME, M_UNKNOWN, printer);
            exit ERR;
        }
        if (chdir (C_SPOOL) < 0)
            message (PROG_NAME, M_CHDIR, C_SPOOL);
        else {
            printf ("Printer: %s\n\n", printer);
            (*command) ();
            printf ("\n");
        }
    }
    else
        for (printer = *av++; printer; printer = *av++) {
            if (!get_caps (printer)) {
                message (PROG_NAME, M_UNKNOWN, printer);
                continue;
            }
            if (chdir (C_SPOOL) < 0)
                message (PROG_NAME, M_CHDIR, C_SPOOL);
            else {
                printf ("Printer: %s\n\n", printer);
                (*command) ();
                printf ("\n");
            }
            if (command == do_help)
                break;
        }
    exit OK;
}

/*
 * Вывод информации об описании усройства
 */
do_caps () {
    register struct c_strs *s;
    register struct c_nums *n;
    register struct c_flgs *f;
    extern char *convert ();

    for (s = STRS; s < ENDSTRS; s++)
        if (s -> cs_value)
            printf ("%s: %s\n",
                    str_name[s - STRS],
                    convert (s -> cs_value));
    for (n = NUMS; n < ENDNUMS; n++)
        if (n -> cn_value >= 0)
            printf ("%s = %d\n",
                    num_name[n - NUMS],
                    n -> cn_value);
    for (f = FLGS; f < ENDFLGS; f++)
        if (f -> cf_value)
            printf ("!!! %s\n", flg_name[f - FLGS]);
}

/*
 * Получение описания устройства из /etc/printcap
 */

get_caps (printer)
char   *printer;
{
    register struct c_strs *s;
    register struct c_nums *n;
    register struct c_flgs *f;
    char   *tmp,
           *space,
           *p_space;
    extern char *calloc (), *pgetstr ();

    tmp = calloc (BUF_SIZ, sizeof (char));
    space = calloc (BUF_SIZ / 4, sizeof (char));
    p_space = space;

    if (pgetent (tmp, printer) != TRUE)
        return FALSE;

    for (s = STRS; s < ENDSTRS; s++)
        if (!(s -> cs_value = pgetstr (s -> cs_name, &p_space)))
            s -> cs_value = s -> cs_default;
    for (n = NUMS; n < ENDNUMS; n++)
        if ((n -> cn_value = pgetnum (n -> cn_name)) < 0)
            n -> cn_value = n -> cn_default;
    for (f = FLGS; f < ENDFLGS; f++)
        f -> cf_value = pgetflag (f -> cf_name);
    cfree (tmp);
    cfree (space);
    return TRUE;
}

/*
 * Перекодировка строковых характеристик.
 */
char   *
        convert (str)
register char  *str;
{
    register char  *line;
    static char LINE[128];

    line = LINE;
    while (*str)
        if (isprint (*str) || *str == ' ')
            *line++ = *str++;
        else
            if (iscntrl (*str) || isspace (*str)) {
                *line++ = '^';
                *line++ = (*str++ | 0100);
            }
            else {
                register int    s,
                                i;
                char    n;

                *line++ = '\\';
                s = (*str++ & 0377);
                for (i = 2; i >= 0; i--) {
                    n = ((s >> (i * 3)) & 07) + '0';
                    *line++ = n;
                }
            }
    *line = '\0';
    return (LINE);
}

/*
 * Разрешение печати.
 */
do_start () {
    if (stat (STOP, &st) >= 0
            && unlink (STOP) < 0)
        message (PROG_NAME, M_UNLINK, STOP);
    else
        do_restart ();
}

/*
 * Запрет печати
 */
do_stop () {
    register int    fd;

    if (stat (STOP, &st) >= 0)
        message (PROG_NAME, M_RSTOP, Null);
    else
        if ((fd = creat (STOP, CRT_MODE)) < 0)
            message (PROG_NAME, M_CREAT, STOP);
        else {
            printf ("deamon are stopped\n");
            close (fd);
        }
}

/*
 * Открытие очереди
 */
do_enable () {
    if (stat (DISABLE, &st) >= 0) {
        if (unlink (DISABLE) < 0)
            message (PROG_NAME, M_ENABLE, Null);
        else
            printf ("spooling queue enable\n");
    }
    else
        message (PROG_NAME, M_RENABLE, Null);
}

/*
 * Закрытие очереди
 */
do_disable () {
    register int    fd;

    if (stat (DISABLE, &st) >= 0)
        message (PROG_NAME, M_RDISABLE, Null);
    else
        if ((fd = creat (DISABLE, CRT_MODE)) < 0)
            message (PROG_NAME, M_CREAT, DISABLE);
        else {
            printf ("spooling queue disable\n");
            close (fd);
        }
}

/*
 * Прерывание текущей печати
 */
do_abort () {
    register int    fd;
    struct d_stat   status;

    if (stat (LOCK, &st) < 0)
        message (PROG_NAME, M_NODEAMON, Null);
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
                    if (kill (status.d_pid, SIGINT) < 0)
                        message (PROG_NAME, M_ABORT, Null);
                    else
                        printf ("deamon aborted\n");
}

/*
 * Перезапуск демона.
 */
do_restart () {
    register int    fd,
                    pid;
    struct d_stat   status;

    if (stat (LOCK, &st) < 0)
        goto run;
    else
        if ((fd = open (LOCK, O_R)) < 0)
            message (PROG_NAME, M_OPEN, LOCK);
        else
            if (read (fd, (char *) & status, sizeof (status))
                    != sizeof (status))
                message (PROG_NAME, M_READ, LOCK);
            else
                if (status.d_pid <= 0
                        || kill (status.d_pid, SIGTERM) < 0) {
                    if (unlink (LOCK) < 0)
                        message (PROG_NAME, M_UNLINK, LOCK);
                    else {
                run: 
                        if (!(pid = fork ())) {
                            execl (C_DEAMON,
                                    basename (C_DEAMON),
                                    printer,
                                    Null);
                            message (PROG_NAME, M_EXEC, C_DEAMON);
                            exit ERR;
                        }
                        else
                            if (pid < 0)
                                message (PROG_NAME, M_FORK, Null);
                            else
                                printf ("deamon restarted\n");
                    }
                }
                else
                    printf ("deamon exist\n");
}

/*
 * Получение подпробной информации о состоянии
 * очереди
 */
do_inform () {
    register int    fd;
    register struct j_queue *job,
                           *queue;
    struct d_stat   status;
    extern struct j_queue  *get_queue ();

 /* Определение состояния демона */
    if (stat (LOCK, &st) < 0)
        printf ("deamon not exist\n");
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
                    printf ("bad lock file, deamon was die\n");
                else
                    printf ("%s\n", status.d_status);
    close (fd);

 /* Определение доступности печати */
    printf ("printer %sable\n",
            (stat (STOP, &st) >= 0)
            ? "en"
            : "dis");

 /* Определение доступности очереди */
    printf ("spooling queue %sable\n",
            (stat (DISABLE, &st) >= 0)
            ? "en"
            : "dis");
    if (!(queue = get_queue ()))
        printf ("no entries");
    else {
        for (fd = 0, job = queue; job; job = job -> jq_next)
            fd++;
        printf ("%d entr%s", fd, ((fd == 1) ? "y" : "ies"));
    }
    printf (" in spooling queue\n");
}

/*
 * Печать списка команд.
 */
do_help () {
    register struct _coms  *c;

    for (c = COMS; c < ENDCOMS; c++)
        if (c -> cm_suser && !SUSER)
            continue;
        else
            message (c -> cm_name, com_help[c - COMS], Null);
}
