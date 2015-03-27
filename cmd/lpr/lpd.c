/*
 * lpd - обслуживающий процесс печати.
 */

#include "lpr_def.h"
#include <sys/stat.h>
#include <signal.h>
#include <setjmp.h>
#include <stdio.h>
#include "lpd.h"

#define WAIT_TIME       30      /* Частота опроса готовности
                                   устройства */
#define STDIN   0
#define STDOUT  1
#define STDERR  2

int     FD_LP;                  /* Дескриптор файла устройства 
                                */

static int  PID = 0;            /* Номер процесса фильтра */
static  jmp_buf ENV;            /* Для setjmp (упрятывание
                                   контекста) */

char   *CMD_FILE = Null,        /* Имя текущего командного
                                   файла */
       *PROG_NAME = Null;       /* Имя команды */

struct stat st;

main (ac, av)
int     ac;
char  **av;
{
    register int    i;
    register struct j_queue *job;
    char   *printer = Null;
    struct d_stat   status;
    extern struct j_queue  *get_queue ();
    extern  abort ();

 /* Игнорировать все возможные сигналы */
    signal (SIGHUP, SIG_IGN);
    signal (SIGINT, SIG_IGN);
    signal (SIGQUIT, SIG_IGN);
    signal (SIGTERM, SIG_IGN);

 /* Определение имени логического устройства */
    PROG_NAME = *av++;

    if (!*av)
        printer = DEF_PRINTER;
    else
        printer = *av;

 /* Получение описания логического устройства */
    get_caps (printer);

 /* Смена контекста открытых файлов */
    for (i = 3; i < MAX_FILES; close (i++));
    if (stat (C_LOGERRS, &st) >= 0
            && (i = open (C_LOGERRS, O_W)) > 0) {
        dup2 (i, STDERR);
        lseek (STDERR, 0L, 2);
        close (i);
    }
    else
        if ((i = open ("/dev/null", O_W)) > 0) {
            dup2 (i, STDERR);
            lseek (STDERR, 0L, 2);
            close (i);
        }

 /* Переход в буферный справочник */
    if (chdir (C_SPOOL) < 0) {
        message (PROG_NAME, M_CHDIR, C_SPOOL);
        exit ERR;
    }

 /* Проверка корректности семафорного файла */
    if (stat (LOCK, &st) >= 0
            || stat (STOP, &st) >= 0)
        exit OK;

 /* Освобождение команды lpr */
    if ((i = fork ())) {
        if (i < 0)
            message (PROG_NAME, M_FORK, Null);
        exit OK;
    }

    umask (0);
    if ((i = creat (LOCK, CRT_MODE)) < 0) {
        message (PROG_NAME, M_CREAT, LOCK);
        exit ERR;
    }

 /* Запись состояния демона */
    status.d_pid = getpid ();
    strncpy (status.d_status,
            "device not ready. waiting",
            sizeof (status.d_status));
    if (write (i, (char *) & status, sizeof (status))
            != sizeof (status)) {
        message (PROG_NAME, M_STATUS, Null);
        unlock ();
    }

    signal (SIGINT, abort);

 /* Ожидание готовности печатающего устройства */
    while ((FD_LP = open (C_DEVICE, O_W)) < 0) {
        if (stat (STOP, &st) >= 0)
            unlock ();
        sleep (WAIT_TIME);
    }

 /* Запись изменения состояния демона */
    lseek (i, 0L, 0);
    strncpy (status.d_status,
            "device ready. printing",
            sizeof (status.d_status));
    if (write (i, (char *) & status, sizeof (status))
            != sizeof (status)) {
        message (PROG_NAME, M_STATUS, Null);
        unlock ();
    }
    close (i);

 /* Прогон листа при первом открытии */
    if (C_FORMOPEN)
        write (FD_LP, C_FORMFEED, strlen (C_FORMFEED));

    while (TRUE) {
    /* Просмотр очереди и разрешения печати */
        if (!(job = get_queue ())
                || stat (STOP, &st) >= 0)
            unlock ();
    /* Печать задания */
        CMD_FILE = job -> jq_name;
        do_print ();
    }
}

do_print () {
    int     fd_cmd,
            fd_file;
    struct j_header hdr;
    struct j_entry  ent;
    char   *arg_vec[8],
          **av = arg_vec,
           *filter;
    char    indent[8],
            length[8],
            width[8],
            name[8];
    extern  attantion ();

 /* Открытие командного файла */
    if ((fd_cmd = open (CMD_FILE, O_R)) < 0) {
        message (PROG_NAME, M_OPEN, CMD_FILE);
        unlock ();
    }

 /* Чтение заголовка */
    if (read (fd_cmd, (char *) & hdr, sizeof (hdr))
            != sizeof (hdr)) {
        message (PROG_NAME, M_READ, CMD_FILE);
        close (fd_cmd);
        unlock ();
    }

 /* Определение типа фильтра */
    switch (hdr.jh_flags & F_FTYPE) {
        case F_GRAPH: 
            filter = C_GRAPH;
            *av++ = basename (filter);
            sprintf (length, "-y%d", C_YRANGE);
            sprintf (width, "-x%d", C_XRANGE);
            goto args1;
        case F_FTN: 
            filter = C_FTN;
            *av++ = basename (filter);
            goto args;
        default: 
            filter = C_FILTER;
            *av++ = basename (filter);
    args: 
            sprintf (indent, "-i%d", hdr.jh_indent);
            sprintf (length, "-l%d", C_LENGTH);
            sprintf (width, "-w%d", C_WIDTH);
            *av++ = indent;
    args1: 
            *av++ = length;
            *av++ = width;
            if (C_ACCTFILE) {
                sprintf (name, "-n%d", hdr.jh_person);
                *av++ = name;
                *av++ = C_ACCTFILE;
            }
            *av = Null;
            break;
    }

 /* Вывод заданного числа копий */
    do {
    /* Выход в начало списка файлов задания */
        lseek (fd_cmd, (long) sizeof (hdr), 0);

    /* Прогон листа */
        if (!C_SFORM)
            write (FD_LP, C_FORMFEED, strlen (C_FORMFEED));

    /* Печать титульного листа задания */
        if (!C_SHEADER && !(hdr.jh_flags & F_NOHDR))
            pr_header (hdr.jh_person, hdr.jh_name);

    /* Вывод файлов */
        while (read (fd_cmd, (char *) & ent, sizeof (ent))
                == sizeof (ent)) {
        /* Открытие файла данных */
            if ((fd_file = open (ent.je_file, O_R)) < 0) {
                message (PROG_NAME, M_OPEN, ent.je_file);
                continue;
            }
            if (!(PID = fork ())) {
            /* Новый процесс! Запускаем фильтр. */
                register int    i;

                dup2 (fd_file, STDIN);
                dup2 (FD_LP, STDOUT);
                for (i = 3; i < MAX_FILES; close (i++));
                execv (filter, arg_vec);
                message (PROG_NAME, M_EXEC, filter);
                exit ERR;
            }
            else
                if (PID > 0) {
                    static int  w,
                                status;

                    setjmp (ENV);
                    signal (SIGHUP, attantion);
                    while ((w = wait (&status)) != PID
                            && w > 0);
                    signal (SIGHUP, SIG_IGN);
                    if (status) {
                        close (fd_file);
                        return;
                    }
                }
                else {
                    message (PROG_NAME, M_FORK, Null);
                    close (fd_file);
                    unlock ();
                }
            close (fd_file);
        }
    } while (hdr.jh_copies--);

 /* Отправление сообщения о завершении печати */
    if (hdr.jh_flags & F_MAIL)
        send_mail (hdr.jh_person);

 /* Удаление файлов */
    lseek (fd_cmd, (long) sizeof (hdr), 0);
    while (read (fd_cmd, (char *) & ent, sizeof (ent))
            == sizeof (ent))
        unlink (ent.je_file);
    close (fd_cmd);
    unlink (CMD_FILE);
}

/* Отправление сообщения о завершении работы */

send_mail (user)
char   *user;
{
    register int    pid;
    int     p[2];

    pipe (p);

    if (!(pid = fork ())) {
        register int    i;

        dup2 (p[0], STDIN);
        for (i = 3; i < MAX_FILES; close (i++));
        execl ("/bin/mail", "mail", user, Null);
        message (PROG_NAME, M_EXEC, "/bin/mail");
        exit ERR;
    }
    else
        if (pid > 0) {
            register int    w;
            int     status;

            close (p[0]);
            write (p[1], "Your printer job is done\n", 25);
            close (p[1]);
            while ((w = wait (&status)) != pid && w > 0);
        }
        else {
            close (p[0]);
            close (p[1]);
        }
}

/* Печать титульного листа */

pr_header (user, job_name)
char   *user,
       *job_name;
{
    register int    pid;
    int     p[2];

    pipe (p);

    if (!(pid = fork ())) {
        register int    i;

        dup2 (p[0], STDIN);
        dup2 (FD_LP, STDOUT);
        for (i = 3; i < MAX_FILES; close (i++));
        execl ("/usr/bin/banner", "banner", Null);
        message (PROG_NAME, M_EXEC, "/usr/bin/banner");
        exit ERR;
    }
    else
        if (pid > 0) {
            register int    w;
            int     status;
            char    line[32],
                   *s;
            long    tm;
            extern long time ();
            extern char *ctime ();

            close (p[0]);
            sprintf (line, "\n%s\n\n%s\n\n", user, job_name);
            write (p[1], line, strlen (line));
            close (p[1]);
            while ((w = wait (&status)) != pid && w > 0);
            tm = time ((long *) 0);
            s = ctime (&tm);
            write (FD_LP, s, strlen (s));
            write (FD_LP, C_FORMFEED, strlen (C_FORMFEED));
        }
        else {
            close (p[0]);
            close (p[1]);
        }
}

abort () {
    signal (SIGINT, SIG_IGN);
    signal (SIGHUP, SIG_IGN);
    if (PID > 0)
        kill (PID, SIGINT);
    unlock ();
}

unlock () {
    unlink (LOCK);
    exit OK;
}

attantion (sig) {
    signal (sig, SIG_IGN);
    if (stat (CMD_FILE, &st) < 0)
        kill (PID, SIGINT);
    longjmp (ENV, TRUE);
}

get_caps (printer)
char   *printer;
{
    register struct c_strs *s;
    register struct c_nums *n;
    register struct c_flgs *f;
    char   *space,
           *tmp;
    extern char *calloc (), *pgetstr ();

    space = calloc (BUF_SIZ / 4, sizeof (char));
    tmp = calloc (BUF_SIZ, sizeof (char));

    if (pgetent (tmp, printer) != TRUE) {
        message (PROG_NAME, M_UNKNOWN, printer);
        exit ERR;
    }

    for (s = STRS; s < END_STRS; s++)
        if (!(s -> cs_value = pgetstr (s -> cs_name, &space)))
            s -> cs_value = s -> cs_default;
    for (n = NUMS; n < END_NUMS; n++)
        if ((n -> cn_value = pgetnum (n -> cn_name)) < 0)
            n -> cn_value = n -> cn_default;
    for (f = FLGS; f < END_FLGS; f++)
        f -> cf_value = pgetflag (f -> cf_name);

    cfree (tmp);
}
