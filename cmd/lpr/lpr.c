/*
 * Постановка задания в очередь на печать.
 */

#include "lpr_def.h"
#include <sys/stat.h>
#include <signal.h>
#include <a.out.h>
#include <ar.h>
#include <pwd.h>
#include <stdio.h>
#include <ctype.h>
#include "lpr.h"

char    _TABLE[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

char   *COM_TMP = Null,         /* Имя копии командного файла 
                                */
       *COM_FILE = Null,        /* Имя командного файла */
       *DAT_FILE = Null,        /* Имя файла данных */
       *DINAMIC = Null,         /* Переменная часть имени файла
                                   данных */
       *PROG_NAME = Null;       /* Имя программы */

struct passwd  *USER;           /* Информация о пользователе */

struct stat st;

main (ac, av)
int     ac;
register char **av;
{
    register char  *s;
    register int    fd_tmp;
    register int    entries;
    char    coping = FALSE,
           *printer = Null;
    struct j_header hdr;
    struct j_entry  ent;
    extern char *getenv (), *rindex (),
               *mk_tmp (), *basename ();
    extern struct passwd   *get_usr_id ();
    extern int  cleanup (), cpword ();

    PROG_NAME = *av++;

 /* Получение информации о пользователе */
    if (!(USER = get_usr_id ())) {
        message (PROG_NAME, M_USER, Null);
        exit ERR;
    }

 /* Разбор параметров командной строки */
    for (s = *av; s && *s++ == '-'; s = *++av)
        switch (*s) {
            case 'P': 
                if (!printer)
                    printer = ++s;
                break;
            case 'c': 
                coping = TRUE;
                break;
            case 'J': 
                if (!hdr.jh_name[0])
                    cpword (hdr.jh_name,
                            ++s,
                            sizeof (hdr.jh_name));
                break;
            case 'i': 
                if (!hdr.jh_indent)
                    hdr.jh_indent = (*++s) ? atoi (s) : 8;
                break;
            case 'g': 
                if (!hdr.jh_flags & F_FTYPE)
                    hdr.jh_flags |= F_GRAPH;
                break;
            case 'f': 
                if (!hdr.jh_flags & F_FTYPE)
                    hdr.jh_flags |= F_FTN;
                break;
            case 'm': 
                hdr.jh_flags |= F_MAIL;
                break;
            case 'h': 
                hdr.jh_flags |= F_NOHDR;
                break;
            default: 
                if (isdigit (*s)) {
                    if (!hdr.jh_copies)
                        hdr.jh_copies = atoi (s);
                }
                else {
                    message (PROG_NAME, M_USAGE, Null);
                    exit ERR;
                }
                break;
        }

 /* Определение имени печатающего устройства */
    if (!printer
            && (printer = getenv ("PRINTER")) == NULL)
        printer = DEF_PRINTER;

 /* Получение характеристик устройства */
    get_caps (printer);

 /* Контроль коррекности задания типа фильтра */
    switch (hdr.jh_flags & F_FTYPE) {
        case F_GRAPH: 
            if (!C_GRAPH) {
                message (PROG_NAME, M_GRAPH, printer);
                exit ERR;
            }
            break;
        case F_FTN: 
            if (!C_FTN) {
                message (PROG_NAME, M_FTN, printer);
                exit ERR;
            }
            break;
        default: 
            break;
    }

 /* Контроль задания числа копий задания */
    if (hdr.jh_copies && C_SCOPIES) {
        hdr.jh_copies = 0;
        message (PROG_NAME, M_SC, Null);
    }
    else
        if (hdr.jh_copies > MAX_COPIES)
            hdr.jh_copies = MAX_COPIES;

 /* Проверка на доступность очереди */
    enable ();

 /* Создание имен файлов */
    COM_TMP = mk_tmp (C_SPOOL, "tf");
    COM_FILE = mk_tmp (C_SPOOL, "cf");
    DAT_FILE = mk_tmp (C_SPOOL, "df");

 /* Для последующего изменения имени файла данных */
    DINAMIC = rindex (DAT_FILE, 'A');

 /* Перехват прерываний */
    signal (SIGINT, cleanup);
    signal (SIGQUIT, cleanup);
    signal (SIGHUP, cleanup);
    signal (SIGTERM, cleanup);

    umask (0);
 /* Создание временного командного файла */
    if ((fd_tmp = creat (COM_TMP, CRT_MODE)) < 0) {
        message (PROG_NAME, M_CREAT, COM_TMP);
        exit ERR;
    }

 /* Запись заголовка пакета */
    cpword (hdr.jh_person,      /* Имя владельца */
            USER -> pw_name,
            sizeof (hdr.jh_person));
    if (!hdr.jh_name[0])        /* Имя задания */
        cpword (hdr.jh_name,
                ((*av) ? basename (*av) : "<stdin>"),
                sizeof (hdr.jh_name));
    if (write (fd_tmp, (char *) & hdr, sizeof (hdr))
            != sizeof (hdr)) {
        message (PROG_NAME, M_WRITE, COM_TMP);
        cleanup ();
    }

    entries = 0;

 /* Копирование файлов в буферный справочник */
    do {
        if (*av) {
            if (non_print (*av) || !*DINAMIC)
                continue;
            if (!coping && link (*av, DAT_FILE) >= 0)
                goto next;
        }
        if (!cp_file (*av, DAT_FILE))
            continue;
next: 
        cpword (ent.je_file,
                basename (DAT_FILE),
                sizeof (ent.je_file));
        cpword (ent.je_name,
                ((*av)
                    ? basename (*av)
                    : "<stdin>"),
                sizeof (ent.je_name));
        if (write (fd_tmp, (char *) & ent, sizeof (ent)) !=
                sizeof (ent)) {
            message (PROG_NAME, M_WRITE, COM_TMP);
            cleanup ();
        }
        *DINAMIC = _TABLE[++entries];
    } while (*av && *++av);

    close (fd_tmp);

    if (entries) {
        if (link (COM_TMP, COM_FILE) < 0)
            message (PROG_NAME, M_LINK, COM_TMP);
        else
            if (unlink (COM_TMP) < 0)
                message (PROG_NAME, M_UNLINK, COM_TMP);
            else {
                execl (C_DEAMON,
                        basename (C_DEAMON),
                        printer,
                        Null);
                message (PROG_NAME, M_EXEC, C_DEAMON);
                exit OK;
            }
    }
    cleanup ();
}

/*
 * Создание уникального имени файла.
 */

char   *
        mk_tmp (dir, s)
register char  *dir;
register char  *s;
{
    register char  *line;
    register int    size;
    extern char *calloc ();

    size = strlen (dir) + strlen (s) + 16;
    if ((line = calloc ((unsigned) size, sizeof (char))))
        sprintf (line, "%s/%sA%d", dir, s, getpid ());
    return (line);
}

/*
 * Копирование файла с именем "in" в файл с именем "out".
 * Если файл "in" задан пустым именем, используется
 * стандартный файл ввода.
 * В случае ошибки, файл "out" уничтожается и возвращается
 * значение FALSE.
 */

cp_file (in, out)
char   *in,
       *out;
{
    register int    fd_in,
                    fd_out,
                    size;
    char    buf[BUF_SIZ],
            err = FALSE;

    if (!in)
        fd_in = fileno (stdin);
    else
        if ((fd_in = open (in, O_R)) < 0) {
            message (PROG_NAME, M_OPEN, in);
            return FALSE;
        }
    if ((fd_out = creat (out, CRT_MODE)) < 0) {
        message (PROG_NAME, M_CREAT, out);
        cleanup ();
    }
    while ((size = read (fd_in, (char *) buf, BUF_SIZ)) > 0)
        if (write (fd_out, (char *) buf, size) != size) {
            message (PROG_NAME, M_WRITE, out);
            err = TRUE;
            break;
        }

    close (fd_in);
    close (fd_out);

    if (!err && size < 0) {
        message (PROG_NAME, M_READ,
                ((in)
                    ? in
                    : "<stdin>"));
        err = TRUE;
    }
    if (err) {
        unlink (out);
        return FALSE;
    }
    else
        return TRUE;
}

/* Уничтожение временных файлов */

cleanup () {
    register int    i;

    signal (SIGINT, SIG_IGN);
    signal (SIGQUIT, SIG_IGN);
    signal (SIGHUP, SIG_IGN);
    signal (SIGTERM, SIG_IGN);
    if (COM_TMP && stat (COM_TMP, &st) >= 0)
        unlink (COM_TMP);
    if (COM_FILE && stat (COM_FILE, &st) >= 0)
        unlink (COM_FILE);
    for (i = 0; DINAMIC; i++) {
        *DINAMIC = _TABLE[i];
        if (!*DINAMIC || stat (DAT_FILE, &st) < 0)
            break;
        unlink (DAT_FILE);
    }
    exit ERR;
}

/*
 * Определение типа файла.
 * Возвращает значение TRUE, если файл не печатный,
 * иначе FALSE.
 */

non_print (file)
register char  *file;
{
    register int    fd;
    struct exec execb;
    int     answer = TRUE;

    if (access (file, A_R))
        message (PROG_NAME, M_READ, file);
    else
        if (stat (file, &st) < 0)
            message (PROG_NAME, M_STAT, file);
        else
            if ((st.st_mode & S_IFMT) == S_IFDIR)
                message (PROG_NAME, M_DIR, file);
            else
                if ((st.st_mode & S_IFMT) != S_IFREG)
                    message (PROG_NAME, M_SPEC, file);
                else
                    if (st.st_size == (long) 0)
                        message (PROG_NAME, M_EMPTY, file);
                    else
                        if ((fd = open (file, O_R)) < 0)
                            message (PROG_NAME, M_OPEN, file);
                        else
                            if (read (fd, (char *) & execb, sizeof (execb))
                                    == sizeof (execb))
                                switch (execb.a_magic) {
                                    case A_MAGIC1: 
                                    case A_MAGIC2: 
                                    case A_MAGIC3: 
                                    case A_MAGIC4: 
                                        message (PROG_NAME,
                                                M_EXECU,
                                                file);
                                        break;
                                    case ARMAG: 
                                        message (PROG_NAME,
                                                M_AR,
                                                file);
                                        break;
                                    default: 
                                        answer = FALSE;
                                        break;
                                }
                            else
                                message (PROG_NAME,
                                        M_READ,
                                        file);
    close (fd);
    return (answer);
}

get_caps (printer)
char   *printer;
{
    register struct c_strs *s;
    register struct c_flgs *f;
    char   *space,
           *tmp;
    extern char *calloc (), *pgetstr ();

    space = calloc ((unsigned) (BUF_SIZ / 4), sizeof (char));
    tmp = calloc ((unsigned) BUF_SIZ, sizeof (char));

    if (pgetent (tmp, printer) != TRUE) {
        message (PROG_NAME, M_UNKNOWN, printer);
        exit ERR;
    }

    for (s = STRS; s < END_STRS; s++)
        if (!(s -> cs_value = pgetstr (s -> cs_name, &space)))
            s -> cs_value = s -> cs_default;
    for (f = FLGS; f < END_FLGS; f++)
        f -> cf_value = pgetflag (f -> cf_name);

    cfree (tmp);
}

enable () {
    register char  *name;
    register int    size;
    extern char *calloc ();

    size = strlen (C_SPOOL) + strlen (DISABLE) + 2;
    name = calloc ((unsigned) size, sizeof (char));

    sprintf (name, "%s/%s", C_SPOOL, DISABLE);
    if (stat (name, &st) >= 0 && getuid ()) {
        message (PROG_NAME, M_DISABLE, Null);
        exit ERR;
    }
    cfree (name);
}
