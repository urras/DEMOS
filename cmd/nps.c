/*
 * Упрощенный вариант команды ps.
 * Вызов:
 *      nps -D[kernel]
 *      nps -tty (где tty - имя терминала
 *                          без префикса /dev/tty);
 *      nps [Имя_пользователя]
 *
 * $Header: nps.c,v 1.11 86/04/05 00:07:27 asa Rel $
 * $Log:        nps.c,v $
 * Revision 1.11  86/04/05  00:07:27  asa
 * Добавлен режим инициализации: nps -D[namelist].
 * 
 * Revision 1.10  85/12/27  06:06:57  asa
 * Проведена верификация с помощью lint.
 * 
 * Revision 1.9  85/12/27  04:07:53  asa
 * *** empty log message ***
 * 
 * Revision 1.8  85/12/27  03:57:12  asa
 * Исправлены замеченные ошибки в формате выдачи.
 * 
 * Revision 1.6  85/12/12  19:49:31  asa
 * Законченный вариант команды nps.
 * Дальнейшее развитие остановлено до
 * получения отзывов и пожеланий.
 * 
 * Revision 1.5  85/12/11  02:42:56  asa
 * Почти законченная версия упрощенной команды ps.
 * Осталось тщательно прочистить и откоментировать текст.
 *      Преимущества данной версии:
 *              быстрая работа, малые размеры и высокая
 *              читабельность текста программы.
 *      Недостатки:
 *              выдается минимальная информация о процессах,
 *              указывается только имя команды без аргументов.
 * Вцелом команда по выдаваемой информации соответствует:
 *      ps aucx.
 * 
 * Revision 1.4  85/12/08  00:03:31  asa
 * Введены идентификаторы версии.
 * 
 */

#include <sys/param.h>
#include <sys/proc.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/stat.h>
#include <a.out.h>
#include <pwd.h>
#include <stdio.h>

#define NAMELEN 10              /* Длина имени терминала или
                                   имени пользователя */
#define NCTTY   64              /* Максимальное число
                                   терминалов */
#define NCUSERS 256             /* Максимальное число
                                   пользователей */
#define CREATMODE       0644    /* Режим создания файла */

/* Временные файлы */

char   *ss_nlist = "/tmp/ss.nlist";
 /* Адреса переменных в ядре */
char   *ss_names = "/tmp/ss.names";
 /* Имена пользователей */
char   *ss_devices = "/tmp/ss.devices";
 /* Имена терминалов */

/* Рабочие файлы */

char   *mem = "/dev/mem";       /* Память */
char   *unfile = "/demos";      /* Имя ядра ОС */
char   *devdir = "/dev";        /* Имя справочника устройств */
char    mkdatabase = 0;         /* Инициализация */

int     core;                   /* Дескриптор памяти */
int     pcore;                  /* */
int     swap;                   /* Дескриптор области подкачки 
                                */
daddr_t swplo;                  /* Начало области подкачки */
int     mf;                     /* Универсальный дескриптор
                                   обмена: указывает либо на
                                   область подкачки, либо на
                                   память */
int     ndev;                   /* Число терминалов */

#ifndef lint
static char rcsid[] =
            "$Header: nps.c,v 1.11 86/04/05 00:07:27 asa Rel $";
#endif

struct device {
    char    devc[NAMELEN];      /* Имя терминала */
    int     devl;               /* Номер устройства */
}               DEV[NCTTY];

struct user u;                  /* Управляющая структура
                                   процесса */
struct nlist    nl[] = {
#define DPROC   0
    {
        "_proc"
    },
#define DDC11   1
    {
        "_dc11"
    },
#define DKL11   2
    {
        "_kl11"
    },
#define DDH11   3
    {
        "_dh11"
    },
#define DDZ11   4
    {
        "_dz11"
    },
#define DDJ11   5
    {
        "_dj11"
    },
#define DIPC    6
    {
        "_ipc"
    },
#define DBUF    7
    {
        "_buf"
    },
#define DBUFFERS        8
    {
        "_buffers"
    },
#define DBFREELI        9
    {
        "_bfreeli"
    },
#define DPC11   10
    {
        "_pc11"
    },
#define DSWBUF1 11
    {
        "_swbuf1"
    },
#define DSWBUF2 12
    {
        "_swbuf2"
    },
#define DRUNIN  13
    {
        "_runin"
    },
#define DRUNOUT 14
    {
        "_runout"
    },
#define DU      15
    {
        "_u"
    },
#define DLBOLT  16
    {
        "_lbolt"
    },
#define DTIME   17
    {
        "_time"
    },
#define DCALLOUT        18
    {
        "_callout"
    },
#define DRRKBUF 19
    {
        "_rrkbuf"
    },
#define DRRLBUF 20
    {
        "_rrlbuf"
    },
#define DRRPBUF 21
    {
        "_rrpbuf"
    },
#define DRHPBUF 22
    {
        "_rhpbuf"
    },
#define DRRMBUF 23
    {
        "_rrmbuf"
    },
#define DRSIBUF 24
    {
        "_rsibuf"
    },
#define DRRXBUF 25
    {
        "_rrxbuf"
    },
#define DRTMBUF 26
    {
        "_rtmbuf"
    },
#define DTEXT   27
    {
        "_text"
    },
#define DINODE  28
    {
        "_inode"
    },
#define DSWAPDEV        29
    {
        "_swapdev"
    },
#define DSWAPLO 30
    {
        "_swplo"
    },
#define DMAXPRO 31
    {
        "_max_pro"
    },
    {
        ""
    },
};

#define NSYMB   (sizeof(nl)/sizeof(struct nlist) - 2)

char    names[NCUSERS][NAMELEN];/* Список имен пользователей */

int     totalusage;
int     totalsize;

/* Информация о процессе */

struct INFO {
    struct proc p;              /* Описатель процесса */
    char   *ttynam;             /* Имя управляющего терминала 
                                */
    int     cptime;             /* Время загрузки ЦП */
    int     usage;              /* */
    char    cmnd[DIRSIZ];       /* Имя команды */
    int     uid,                /* Идентификатор пользователя 
                                */
            ruid;               /* */
}           info[NPROC], *lastinfo;

struct proc *max_proc;
int     nproc = NPROC;

main (argc, argv)
int     argc;
char   *argv[];
{
    register struct INFO   *ip;
    register int    j;
    extern  icomp ();

    if ((core = open (mem, 0)) < 0
            ||
            (pcore = open (mem, 0)) < 0) {
        perror (mem);
        exit (1);
    }
    if (argc == 2
        && argv[1][0] == '-'
        && argv[1][1] == 'D') {
        mkdatabase = 1;
        if (argv[1][2])
            unfile = &argv[1][2];
    }
    getnl ();
    getnames ();
    lseek (core, (long) nl[NSYMB - 2].n_value, 0);
    read (core, (char *) & nl[NSYMB - 2].n_value,
            sizeof (nl[NSYMB - 2].n_value));
    lseek (core, (long) nl[DSWAPLO].n_value, 0);
    read (core, (char *) & swplo, sizeof (swplo));
    getdev ();
    if (mkdatabase)
        exit (0);
    lseek (core, (long) nl[DPROC].n_value, 0);
    ip = &info[0];
    totalusage = 0;
    totalsize = 0;
    for (j = 0; j < nproc; j++) {
        read (core, (char *) & (ip -> p), sizeof (struct proc));
        if (ip -> p.p_stat == 0)
            continue;
        ip -> usage = ip -> p.p_cpu & 0377;
        totalusage += ip -> usage;
        totalsize += ip -> p.p_size >> 5;
        getusr (&ip -> p, ip);
        ip++;
    }
    lastinfo = ip - 1;
    qsort (info, lastinfo - info + 1, sizeof info[0], icomp);
    output (argc, argv);
}

/*
 * Упорядочивание выводной информации:
 *      по именам терминалов,
 *      группам процессов или их номерам.
 */

icomp (ii1, ii2)
struct INFO *ii1,
           *ii2;
{
    register struct INFO   *i1,
                           *i2;

    i1 = ii1;
    i2 = ii2;
    if (i1 -> ttynam != i2 -> ttynam)
        return (i2 -> ttynam - i1 -> ttynam);
    if (i1 -> p.p_pgrp != i2 -> p.p_pgrp)
        return (i1 -> p.p_pgrp - i2 -> p.p_pgrp);
    return (i1 -> p.p_pid - i2 -> p.p_pid);
}

/*
 * Получение списка адресов переменных ядра.
 */

getnl () {
    register    file;
    register struct nlist *list;


    if (!mkdatabase && (file = open (ss_nlist, 0)) >= 0)
        read (file, (char *) nl, sizeof nl);
    else {
        if (nlist (unfile, nl) < 0) {
            perror (unfile);
            exit (1);
        }
        if (!nl[DU].n_value || !nl[DPROC].n_value
             || !nl[DTEXT].n_value || !nl[DINODE].n_value) {
             printf ("%s: bad namelist.\n", unfile);
             exit (1);
        }
        file = creat (ss_nlist, CREATMODE);
        write (file, (char *) nl, sizeof nl);
    }
    close (file);
    if (nl[DMAXPRO].n_type != 0) {
        lseek (core, (long) nl[DMAXPRO].n_value, 0);
        read (core, (char *) & max_proc, sizeof (max_proc));
        nproc = max_proc
            - (struct proc *) nl[DPROC].n_value + 1;
    }
}

/*
 * Получение управляющей структуры процесса.
 */

getusr (pp, ip)
register struct proc   *pp;
register struct INFO   *ip;
{
    register    j;
    long    addr,
            ltime;

    if (pp -> p_flag & SLOAD) {
        addr = ctob ((long) pp -> p_addr);
        mf = pcore;
    }
    else {
        addr = (pp -> p_addr + swplo) << BSHIFT;
        mf = swap;
    }
    lseek (mf, (long) addr, 0);
    read (mf, (char *) & u, sizeof u);
    ip -> uid = u.u_uid;
    ip -> ruid = u.u_ruid;
    ltime = (u.u_utime) + (u.u_stime) + 30;
    ip -> cptime = ltime / 60;
 /* Определение имени управляющего терминала */
    ip -> ttynam = "%";
    if (u.u_ttyp)
        for (j = 0; j < ndev; j++)
            if (DEV[j].devl == u.u_ttyd) {
                ip -> ttynam = DEV[j].devc;
                break;
            }
 /* Получение имени команды */
    if (pp -> p_stat == SIDL || pp -> p_stat == SZOMB) {
        ip -> cptime = 0;
        strncat ((caddr_t) ip -> cmnd, "    ", DIRSIZ);
    }
    else
        strncat ((caddr_t) ip -> cmnd,
                (caddr_t) u.u_comm, DIRSIZ);
    return;
}

/*
 * Вывод информации, проверка ключей командной строки.
 */

output (argc, argv)
char   *argv[];
{
    register int    i,
                    j;
    register struct INFO   *ip;

    printf ("tty  owner      pid status      size cptime     command\n");
    printf ("---  -----      --- ------      ---- ------ --- -------\n");
    if (argc == 1) {
    /* Без параметров */
        for (ip = lastinfo; ip >= &info[0]; ip--)
            if (ip -> p.p_pgrp == ip -> p.p_pid
                    ||
                    (ip -> p.p_ppid == 1
                        &&
                        ip -> p.p_pgrp != 0)
                )
                out1 (ip, 0);
    }
    else {
        if (argv[1][0] == '-') {
        /* 
         * Параметр начинается в символа '-', подразумевается
         * имя терминала без префикса /dev/tty.
         */
            for (ip = &info[0]; ip <= lastinfo; ip++) {
                if (strcmp (ip -> ttynam, &argv[1][1])
                        == NULL) {
                    out1 (ip, 0);
                    break;
                }
            }
            if (ip > lastinfo)
                printf ("tty%s not found\n", &argv[1][1]);
        }
        else
            for (i = 1; i < argc; i++) {
            /* 
             * В этом случае, считается что заданные
             * параметры являются именами пользователей.
             */
                j = 0;
                for (ip = &info[0]; ip <= lastinfo; ip++)
                    if (((ip -> p.p_pgrp == ip -> p.p_pid)
                                ||
                                (ip -> p.p_ppid == 1
                                    &&
                                    ip -> p.p_pgrp != 0))
                            &&
                            strcmp (argv[i], names[ip -> ruid])
                            == NULL) {
                        out1 (ip, 0);
                        j++;
                    }
                if (j == 0)
                    printf ("%s not logged in\n", argv[i]);
            }
    }
}

out1 (p, level)
struct INFO *p;
{
    register struct INFO   *rp,
                           *ip;

    ip = p;
    out2 (ip, level);
    for (rp = &info[0]; rp <= lastinfo; rp++)
        if (rp -> p.p_pid       /* учитываем процесс 0 */
                &&
                rp -> p.p_ppid == ip -> p.p_pid
                &&
                rp -> p.p_pgrp == ip -> p.p_pgrp
            )
            out1 (rp, level + 1);
}

out2 (ip, level)
register struct INFO   *ip;
{
    register    j;
    static char *lasttty;

    if (lasttty != ip -> ttynam) {
        lasttty = ip -> ttynam;
        printf ("%-3.3s ", lasttty);
    }
    else
        printf ("    ");
    if (level == 0) {
        printf ("- ");
        puid (ip -> p.p_uid);
    }
    else {
        printf ("-");
        j = level;
        while (j--)
            putchar ('>');
        j = level;
        while (j++ <= 8)
            putchar (' ');
    }
    printf ("%5d ", ip -> p.p_pid);
    pstatus (&ip -> p);
    printf ("%2dk ", ip -> p.p_size >> 5);
    printf ("%3d:", ip -> cptime / 60);
    ip -> cptime %= 60;
    printf (ip -> cptime < 10 ? "0%d " : "%d ", ip -> cptime);
    if (ip -> usage * 100 / totalusage)
        printf ("%2d%% ", ip -> usage * 100 / totalusage);
    else
        printf ("    ");
    if (ip -> p.p_pid == 0)
        printf ("Demos (KIAE)");
    else
        printf ("%-.30s ", ip -> cmnd);
    printf ("\n");
}

gwchan (wchan)
register char  *wchan;
{
    register struct nlist  *trial,
                           *guess;

    trial = &nl[NSYMB];
    for (guess = &nl[0]; guess < &nl[NSYMB - 2]; guess++)
        if (guess -> n_value > trial -> n_value
                &&
                guess -> n_value <= wchan)
            trial = guess;
    return (trial);
}

/*
 * Получение имен пользователей системы
 */

getnames () {
    register    file;
    register struct passwd *pass;

    if (!mkdatabase && (file = open (ss_names, 0)) >= 0) {
        read (file, (char *) names, sizeof names);
    }
    else {
        while ((pass = getpwent ()) != NULL)
            if ((int) (pass -> pw_uid) < NCUSERS
                    && names[pass -> pw_uid][0] == 0)
                strncat (names[pass -> pw_uid],
                        pass -> pw_name, (NAMELEN - 2));
        endpwent ();
        file = creat (ss_names, CREATMODE);
        write (file, (char *) names, sizeof names);
    }
    close (file);
}

/* Вывод имени или номера пользователя */

puid (uid)
register    uid;
{
    if (names[uid][0])
        printf ("%-8.8s", names[uid]);
    else
        printf ("%-8d", uid);
}

/*
 * Вывод информации о состоянии процесса
 */

pstatus (pp)
register struct proc   *pp;
{
    register struct nlist  *nlp;
    register char  *mess;
    register    i;

    switch (pp -> p_stat) {
        case SSLEEP: 
            mess = pp -> p_pri > 0 ? "slp" : "SLP";
            break;
        case SRUN: 
            mess = "RUN";
            break;
        case SIDL: 
            mess = "idl";
            break;
        case SZOMB: 
            mess = "zmb";
            break;
        case SSTOP: 
            mess = "stp";
            break;
    }
    printf ("%s%c", mess, (pp -> p_flag & SLOAD) ? '*' : ' ');
    mess = " ";
    if (pp -> p_stat == SSLEEP) {
        nlp = gwchan (pp -> p_wchan);
        switch (nlp - &nl[DPROC]) {
            case DPROC: 
                i = (int) ((int) (pp -> p_wchan)
                        - (int) (nl[DPROC].n_value))
                    /
                    sizeof (*pp);
                printf (i < 10 ?
                        "proc[0%d] " : "proc[%2d] ",
                        i);
                return;
            case DDC11: 
                mess = "dc11";
                break;
            case DKL11: 
                mess = "kl11";
                break;
            case DDH11: 
                mess = "dh11";
                break;
            case DDZ11: 
                mess = "dz11";
                break;
            case DDJ11: 
                mess = "dj11";
                break;
            case DIPC: 
                mess = "trace";
                break;
            case DBUF: 
                mess = "I/O";
                break;
            case DBUFFERS: 
                mess = "filsys";
                break;
            case DBFREELI: 
                mess = "freebuf";
                break;
            case DPC11: 
                mess = "pc11";
                break;
            case DSWBUF1: 
            case DSWBUF2: 
                mess = "swap";
                break;
            case DRUNIN: 
                mess = "runin";
                break;
            case DRUNOUT: 
                mess = "runout";
                break;
            case DU: 
                mess = "pause";
                break;
            case DLBOLT: 
            case DTIME: 
            case DCALLOUT: 
                mess = "clock";
                break;
            case DRRKBUF: 
            case DRRLBUF: 
            case DRRPBUF: 
            case DRHPBUF: 
            case DRRMBUF: 
            case DRSIBUF: 
            case DRRXBUF: 
            case DRTMBUF: 
                mess = "raw I/O";
                break;
            case DTEXT: 
                mess = "text";
                break;
            case DINODE: 
                mess = "inode";
                break;
            default: 
                printf ("%-8d ", pp -> p_wchan);
                return;
        }
    }
    printf ("%-8.8s ", mess);
}

/*
 * Получение имен терминалов
 */

getdev () {
    register struct direct *p;
    register    i,
                c;
    int     f;
    char    dbuf[512];
    struct stat sbuf;

    if (!mkdatabase && (f = open (ss_devices, 0)) >= 0) {
        read (f, (char *) DEV, sizeof DEV);
        close (f);
    }
    else {
        if (chdir (devdir) < 0) {
            perror (devdir);
            exit (1);
        }
        if ((f = open (devdir, 0)) < 0) {
            perror (devdir);
            exit (1);
        }
        c = 0;
        while ((i = read (f, (char *) dbuf, 512)) > 0) {
            while (i < 512)
                dbuf[i++] = 0;
            for (p = dbuf; p < dbuf + 512; p++) {
                if (p -> d_ino == 0)
                    continue;
                if (strcmp ((char *) p -> d_name,
                            (char *) "console")
                        == NULL) {
                    if (stat (p -> d_name, &sbuf) < 0)
                        continue;
                    strcat (DEV[c].devc, "C");
                    DEV[c].devl = sbuf.st_rdev;
                    c++;
                    continue;
                }
                if (p -> d_name[0] == 't'
                        &&
                        p -> d_name[1] == 't'
                        &&
                        p -> d_name[2] == 'y'
                        &&
                        p -> d_name[3] != 0) {
                    if (stat (p -> d_name, &sbuf) < 0
                            || (sbuf.st_mode & S_IFMT)
                            != S_IFCHR)
                        continue;
                    strncat (DEV[c].devc,
                            &p -> d_name[3], (NAMELEN - 2));
                    DEV[c].devl = sbuf.st_rdev;
                    c++;
                    continue;
                }
            }
        }
        close (f);
        f = creat (ss_devices, CREATMODE);
        write (f, (char *) DEV, sizeof DEV);
        close (f);
    }
    if ((swap = open ("/dev/swap", 0)) < 0) {
        perror ("/dev/swap");
        exit (1);
    }
    c = 0;
    while (DEV[c].devc[0])
        c++;
    ndev = c;
    return;
}
