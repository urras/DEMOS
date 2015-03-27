/*
 * $Log:	sysent.c,v $
 * Revision 22.4  90/11/12  19:21:41  root
 * Новые вещи для СМ1425 и перемещение include.
 * 
 * Revision 22.3  89/05/23  14:11:10  abs
 * новый сисвызов ustat
 * 
 * Revision 22.2  89/04/28  17:03:48  avg
 * Добавлен #include "h/space.h" для вытаскивания констант SEMMNS и MSGMNI.
 *
 * Revision 22.1  89/04/12  15:40:42  korotaev
 * "param.h" ==> <sys/param.h>
 *
 * Revision 22.0  89/03/30  23:12:53  korotaev
 * Begining DEMOS 2.2.
 *
 * Revision 1.13  89/03/10  19:02:09  abs
 * msgsys получил теперь 7 аргументов вместо 6.
 * Это машинно-зависимый хак:
 *      один из 6 аргументов мог быть long и он не лез в слово!
 *
 * Revision 1.12  89/03/09  18:26:58  abs
 * опечатка
 *
 * Revision 1.11  89/03/09  17:25:04  abs
 * messages
 *
 * Revision 1.10  89/03/03  14:15:26  abs
 * semaphores
 *
 * Revision 1.9  89/01/31  21:07:21  abs
 * новые сисвызовы mkdir и rmdir.
 *
 * Revision 1.8  89/01/07  21:22:56  korotaev
 * Слияние с исправлениями насчет FIFO, +fcntl, + разные режимы открытия
 *
 * (from VAX/UTS SV.2)
 *
 * Revision 1.7  88/06/06  14:37:10  korotaev
 * Вставлен IPK_SELECT (from TCP/IP).
 *
 * Revision 1.6  88/01/23  21:16:46  korotaev
 * Слияние с AVG-Э-85.
 *
 * Revision 1.5  87/12/12  18:13:53  korotaev
 * Вставлен системный вызов flock.
 *
 * Revision 1.4  87/09/23  16:05:22  root
 * Изменения связанные с truncate
 *
 * Revision 1.3  87/01/22  08:36:12  alex
 * Если не задан VFORK, сист. вызов vfork работает как fork.
 * (для того, чтобы не перетранслировать половину системы).
 *
 * Revision 1.2  86/04/20  16:37:48  avg
 * Исправлена буржуйская плюха след. рода:
 *      когда не определен UCB_NET не работают системные вызовы
 *      symlink и readlink.
 *
 * Revision 1.1  86/04/19  15:52:58  avg
 * Initial revision
 *
 */

#include <sys/param.h>
#include <sys/systm.h>
#ifdef  UCB_QUOTAS
#include "../include/quota.h"
#endif

#include "h/space.h"    /* для SEMMNS и MSGMNI */

/*
 * This table is the switch used to transfer
 * to the appropriate routine for processing a system call.
 * Each row contains the number of arguments expected
 * and a pointer to the routine.
 */
int     alarm();
#define mpxchan         nosys
int     chdir();
int     chmod();
int     chown();
int     chroot();
int     close();
int     creat();
int     dup();
int     exec();
int     exece();
int     fork();
int     fstat();
int     getgid();
int     getpid();
int     getuid();
int     gtime();
int     gtty();
int     ioctl();
int     kill();
int     link();
int     mkdir();
int     mknod();
int     nice();
int     nosys();
int     nullsys();
int     open();
int     pause();
int     pipe();
int     profil();
int     ptrace();
int     read();
int     reboot();
int     rexit();
int     rmdir();
int     saccess();
int     sbreak();
int     seek();
int     setgid();
int     setpgrp();
int     setuid();
int     smount();
int     ssig();
int     stat();
int     stime();
int     stty();
int     sumount();
int     ftime();
#ifdef ACCT
int     sysacct();
#else
#define sysacct         nosys
#endif
int     syslock();
int     sysphys();
int     times();
int     umask();
int     unlink();
int     update();
int     utime();
int     wait();
int     write();
#ifdef CGL_RTP
int     rtp();
#else
#define rtp             nosys
#endif
#ifdef  VIRUS_VFORK
int     vfork();
#else
#define vfork           fork /* nosys */
#endif

struct sysent sysent[64] =
{
	1, 0, nullsys,                  /*  0 = indir */
	1, 1, rexit,                    /*  1 = exit */
	0, 0, fork,                     /*  2 = fork */
	3, 1, read,                     /*  3 = read */
	3, 1, write,                    /*  4 = write */
	3, 0, open,                     /*  5 = open */
	1, 1, close,                    /*  6 = close */
	0, 0, wait,                     /*  7 = wait */
	2, 0, creat,                    /*  8 = creat */
	2, 0, link,                     /*  9 = link */
	1, 0, unlink,                   /* 10 = unlink */
	2, 0, exec,                     /* 11 = exec */
	1, 0, chdir,                    /* 12 = chdir */
	0, 0, gtime,                    /* 13 = time */
	3, 0, mknod,                    /* 14 = mknod */
	2, 0, chmod,                    /* 15 = chmod */
	3, 0, chown,                    /* 16 = chown; now 3 args */
	1, 0, sbreak,                   /* 17 = break */
	2, 0, stat,                     /* 18 = stat */
	4, 1, seek,                     /* 19 = seek; now 3 args */
	0, 0, getpid,                   /* 20 = getpid */
	3, 0, smount,                   /* 21 = mount */
	1, 0, sumount,                  /* 22 = umount */
	1, 1, setuid,                   /* 23 = setuid */
	0, 0, getuid,                   /* 24 = getuid */
	2, 2, stime,                    /* 25 = stime */
	4, 1, ptrace,                   /* 26 = ptrace */
	1, 1, alarm,                    /* 27 = alarm */
	2, 1, fstat,                    /* 28 = fstat */
	0, 0, pause,                    /* 29 = pause */
	2, 0, utime,                    /* 30 = utime */
	2, 1, stty,                     /* 31 = stty */
	2, 1, gtty,                     /* 32 = gtty */
	2, 0, saccess,                  /* 33 = access */
	1, 1, nice,                     /* 34 = nice */
	1, 0, ftime,                    /* 35 = ftime; formerly sleep */
	0, 0, update,                   /* 36 = sync */
	2, 1, kill,                     /* 37 = kill */
	0, 0, nullsys,                  /* 38 = switch; inoperative */
	2, 1, setpgrp,                  /* 39 = setpgrp */
	1, 1, nosys,                    /* 40 = tell (obsolete) */
	2, 2, dup,                      /* 41 = dup */
	0, 0, pipe,                     /* 42 = pipe */
	1, 0, times,                    /* 43 = times */
	4, 0, profil,                   /* 44 = prof */
	0, 0, nosys,                    /* 45 = unused */
	1, 1, setgid,                   /* 46 = setgid */
	0, 0, getgid,                   /* 47 = getgid */
	2, 0, ssig,                     /* 48 = sig */
	1, 1, rtp,                      /* 49 = real time process */
	0, 0, nosys,                    /* 50 = reserved for USG */
	1, 0, sysacct,                  /* 51 = turn acct off/on */
	3, 0, sysphys,                  /* 52 = set user physical addresses */
	1, 0, syslock,                  /* 53 = lock user in core */
	3, 0, ioctl,                    /* 54 = ioctl */
	2, 0, reboot,                   /* 55 = reboot */
	4, 0, mpxchan,                  /* 56 = creat mpx comm channel */
	0, 0, vfork,                    /* 57 = vfork */
	1, 0, nosys,                    /* 58 = local system calls */
	3, 0, exece,                    /* 59 = exece */
	1, 0, umask,                    /* 60 = umask */
	1, 0, chroot,                   /* 61 = chroot */
	0, 0, nosys,                    /* 62 = x */
	0, 0, nosys                     /* 63 = used internally */
};

#ifdef  UCB_LOGIN
int     login();
#else
#define login           nosys
#endif
#define submit          nosys
#define killbkg         nosys
int     nostk();
#define killpg          nosys
#undef  setpgrp
int     setpgrp();
#if     !defined(NONSEPARATE) && defined(NONFP)
int     fetchi();
#else
#define fetchi          nosys
#endif
#ifdef  UCB_QUOTAS
int     quota();
int     qfstat();
int     qstat();
#else
#define quota           nosys
#define qfstat          nosys
#define qstat           nosys
#endif
#ifdef  UCB_LOAD
int     gldav();
#else
#define gldav           nosys
#endif
#ifndef NONFP
int     fperr();
#else
#define fperr           nosys
#endif
#ifdef  UCB_VHANGUP
int     vhangup();
#else
#define vhangup         nosys
#endif
int     renice();
int     ucall();
#if SEMMNS > 0
int     semsys();
# else
# define semsys nosys
#endif
#if MSGMNI > 0
int     msgsys();
# else
# define msgsys  nosys
#endif
#ifdef  UCB_NET
int     gethostid();
int     gethostname();
int     saccept();
int     sconnect();
int     sethostid();
int     sethostname();
int     setregid();
int     setreuid();
int     sreceive();
int     ssend();
int     ssocket();
int     ssocketaddr();
#else
#define gethostid       nosys
#define gethostname     nosys
#define saccept         nosys
#define sconnect        nosys
#define sethostid       nosys
#define sethostname     nosys
#define setregid        nosys
#define setreuid        nosys
#define sreceive        nosys
#define ssend           nosys
#define ssocket         nosys
#define ssocketaddr     nosys
#endif
int     select();
int     lstat();
int     readlink();
int     symlink();

int     truncate();
int     ftruncate();
int     flock();
int     ustat();

#ifdef VIRTUAL_SYSTEM   /* @VG virtual system feature */
int avcall(), avaccept(), avcont(), avget(), avput();
#else
#define avcall   nosys
#define avaccept nosys
#define avcont   nosys
#endif

#ifdef AVG_CALLS        /* @VG server calls feature */
int mkcall();
#else
#define mkcall nosys
#endif


struct  sysent  syslocal[] = {
	0, 0, nosys,                    /*  0 = illegal local call */
	3, 0, login,                    /*  1 = login */
	2, 0, lstat,                    /*  2 = like stat, don't follow links */
	1, 1, submit,                   /*  3 = submit - allow after logout */
	0, 0, nostk,                    /*  4 = nostk */
	2, 0, killbkg,                  /*  5 = killbkg - kill background */
	2, 0, killpg,                   /*  6 = killpg - kill process group */
	2, 0, renice,                   /*  7 = renice - change a nice value */
	1, 1, fetchi,                   /*  8 = fetchi */
	4, 0, ucall,                    /*  9 = ucall - call sub from user */
	5, 0, quota,                    /* 10 = quota - set quota parameters */
	2, 1, qfstat,                   /* 11 = qfstat- long fstat for quotas */
	2, 0, qstat,                    /* 12 = qstat - long stat for quotas */
	0, 0, setpgrp,                  /* 13 = setpgrp - set process group */
	1, 1, gldav,                    /* 14 = gldav */
	0, 0, fperr,                    /* 15 = fperr - get FP error regs */
	0, 0, vhangup,                  /* 16 = vhangup - close tty files */
	0, 0, nosys,                    /* 17 = unused */
	5, 1, select,                   /* 18 = select active file descr */
	2, 0, gethostname,              /* 19 = get host name */
	2, 0, sethostname,              /* 20 = set host name */
	4, 0, ssocket,                  /* 21 = get socket fd */
	2, 0, sconnect,                 /* 22 = connect socket */
	2, 0, saccept,                  /* 23 = accept socket connection */
	4, 0, ssend,                    /* 24 = send datagram */
	4, 0, sreceive,                 /* 25 = receive datagram */
	2, 0, ssocketaddr,              /* 26 = get socket address  */
	2, 2, setreuid,                 /* 27 = set real user id */
	2, 2, setregid,                 /* 28 = set real group id */
	2, 1, symlink,                  /* 29 = symlink */
	3, 1, readlink,                 /* 30 = readlink */
	0, 0, gethostid,                /* 31 = gethostid */
	2, 0, sethostid,                /* 32 = sethostid */
	3, 0, truncate,                 /* 33 = truncate */
	3, 1, ftruncate,                /* 34 = ftruncate */
	2, 2, flock,                    /* 35 = file lock */
	2, 1, mkcall,                   /* 36 = make call for server (@VG) */
	2, 0, mkdir ,                   /* 37 = create new directory */
	1, 0, rmdir ,                   /* 38 = remove directory     */
	5, 0, semsys,                   /* 39 = semaphore control    */
	7, 0, msgsys,                   /* 40 = message IPC control  */
	2, 1, ustat,                    /* 41 = FS usage statistics  */
#ifdef VIRTUAL_SYSTEM
	2, 2, avcall,   /* 41 = call virtusal system */
	1, 1, avaccept, /* 42 = accept virtual sys call */
	2, 2, avcont,   /* 43 = continue vs caller execution */
	2, 2, avget,    /* 44 = get word from user */
	3, 2, avput,    /* 45 = put word to user */
#endif
};

int     nlocalsys = sizeof(syslocal) / sizeof(syslocal[0]);
