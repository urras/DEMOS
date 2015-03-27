/*
 * $Log:	sys1.c,v $
 * Revision 22.5  90/11/12  19:19:10  root
 * Новые вещи для СМ1425 и перемещение include.
 * 
 * Revision 22.4  89/07/01  20:13:46  avg
 * Увеличен размер области свопинга, для которой начнут выполняться 
 * проверки.
 * 
 * Revision 22.3  89/04/28  17:02:44  avg
 * Добавлен #include "h/space.h" для вытаскивания констант SEMMNS и MSGMNI.
 *
 * Revision 22.2  89/04/12  15:36:09  korotaev
 * "param.h" ==> <sys/param.h>
 *
 * Revision 22.1  89/04/02  13:33:09  abs
 * заменил один anic на printf
 *
 * Revision 22.0  89/03/31  17:59:54  korotaev
 * Begining DEMOS 2.2.
 *
 * Revision 1.12  89/03/03  14:16:12  abs
 * semaphores
 *
 * Revision 1.11  89/02/09  16:36:59  abs
 * механизм копирования аргументов при ШеллСкрипте переписан вообще на фиг !
 *
 * Revision 1.10  89/02/07  15:51:28  abs
 * UCB_SHELLSCRIPT теперь имеет аргументы.
 *
 * Revision 1.9  89/02/04  18:51:33  avg
 * Ускорено переключение пользовательских оверлеев.
 *
 * Revision 1.8  89/01/26  15:55:42  korotaev
 * Уменьшены диагностики.
 *
 * Revision 1.7  89/01/07  20:56:57  korotaev
 * Слияние с исправлениями насчет FIFO, +fcntl, + разные режимы открытия
 *
 * (from VAX/UTS SV.2)
 *
 * Revision 1.5  88/03/23  13:37:36  korotaev
 * Состояние после слияния с АЗЛК, Э-85 и Бурковским планировщиком
 *
 * Revision 1.4  88/01/23  19:53:30  korotaev
 * Состояние после слияния с AVG-Э-85.
 *
 * Revision 1.3  87/12/09  16:31:52  andrew
 * 31 оверлей
 *
 * Revision 1.2  87/04/18  14:42:44  avg
 * Правки в vfork by alex.
 *
 * Revision 1.1  86/04/19  15:52:28  avg
 * Initial revision
 *
 */

/* Структура стека сразу после exec-а :
     ----------------
     |   na-ne= argc
     |    a0
     |    ...
     |    an
     |   NULL
     |    e0
     |    ...
     |    em
     |   NULL
  a0:|    "argv0\0"
     |    ...
  an:|    "argvn\0"
  e0:|    "envp0\0"
     |    ...
  em:|    "envpm\0"
     |     0
   0:| пустой стек
 -----

*/
#include <sys/param.h>
#include <sys/systm.h>
#include "../include/map.h"
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/proc.h>
#include "../include/buf.h"
#include "../include/reg.h"
#include "../include/psw.h"
#include <sys/inode.h>
#include <sys/seg.h>
#include <sys/acct.h>
#include <sys/file.h>
#include <sys/wait.h>

#include "h/space.h"    /* для SEMMNS */

extern panicreboot;

/*
 * exec system call, with and without environments.
 */
struct execa {
	char    *fname;
	char    **argp;
	char    **envp;
};

exec()
{
	((struct execa *)u.u_ap)->envp = NULL;
	exece();
}

#define byteaddress int

exece()
{
	register nc;    /* общее число байт в argp и envp */
	int na,         /* число строк в argp и envp вместе */
	    ne,         /* число строк в envp */
	    ucp,c;
	byteaddress ap;
		/* грязное место. В дальнейшем время от времени
		 * ap используется по разному: как   char *
		 * как char **     и как    int
		 */
	register char *cp;
	register struct buf *bp;
	register struct execa *uap;
	memaddr bno;
	struct inode *ip;
	char *next;
#define  AP (uap->argp)
#define  EP (uap->envp)

# define SCRMAG  "#!"
# define SCRMAG2 "/*#!"
# define ARGPLACE "$*"
# define SHSIZE  70
# define ARGV    11
	char **AV;

	extern int schar();
	int uid, gid;
	char indir, local;
	off_t  off;
	char shellcmd[ SHSIZE ];        /* прочитанная команда */
	char *argv[ ARGV ];             /* разобранная команда */

	if ((ip = namei(uchar, LOOKUP, 1)) == NULL)
		return;
#ifdef SCHED
	u.u_procp->p_sflag |= SEXEC;
#endif SCHED
	bno = 0;
	bp = (struct buf *) NULL;
	indir = 0;
	uid = u.u_uid;
	gid = u.u_gid;
	if (ip->i_mode&ISUID)
	    uid = ip->i_uid;
	if (ip->i_mode&ISGID)
	    gid = ip->i_gid;
again:
	if (access(ip, IEXEC))
		goto bad;
	if ((ip->i_mode & IFMT) != IFREG ||
	   (ip->i_mode & (IEXEC | (IEXEC >> 3) | (IEXEC >> 6))) == 0) {
		u.u_error = EACCES;
		goto bad;
	}

	/* moved from getxfile() */
	u.u_base = (caddr_t) &u.u_exdata;
	u.u_count = sizeof u.u_exdata;
	u.u_offset = 0;
	u.u_segflg = 1;

	readi(ip);

	u.u_segflg = 0;
	if (u.u_error)
	    goto bad;

	/* check if script. */
	if (indir < NSYMLNK && u.u_offset > (off_t)2 ){
	    cp = (char *) &u.u_exdata;
	    if( !bcmp(cp, SCRMAG, 2 ))
		off = (off_t) 2;
	    else if( u.u_offset > (off_t)4 && !bcmp( cp, SCRMAG2, 4))
		off = (off_t) 4;
	    else goto NoScript;

	    /* да, имеется shell script */
	    indir++;

	    /* зачитать в буфер первые SHSIZE байт файла */
	    u.u_count = SHSIZE;
	    u.u_base  = shellcmd;
	    u.u_offset= off;
	    u.u_segflg = 1;

	    readi( ip );
	    u.u_segflg = 0;

	    if( u.u_error )
		goto bad;

	    iput( ip );
	    off = u.u_offset - off;
	    /* столько байт на самом деле прочлось внутрь */

	    /* обработка */
	    /* пока nc используется как временная переменная */

	    for( nc=0; nc < (int) off; nc++ )
		if( shellcmd[ nc ] == '\n' ){
			shellcmd[ nc] = '\0';
			goto Parse;
		}
Big:
	    /* строка слишком длинная */
	    u.u_error = E2BIG;
Ret:
# ifdef SCHED
	    u.u_procp->p_sflag &= ~SEXEC;
# endif SCHED
	    return;
Parse:

	    nc = 0;
	    cp = shellcmd;
	    for(;;){
		/* пропуск пробелов */
		while( *cp == ' ' ) cp++;
		if( ! *cp ) break;      /* конец строки */

		if( nc == ARGV-1 ) goto Big;
		argv[ nc++ ] = cp;

		/* пропуск аргумента */
		while( *cp && *cp != ' ' ) cp++;
		if( ! *cp ) break;

		*cp ++ = '\0';
	    }
	    argv[ nc ] = NULL;

	    if( !nc ){
		u.u_error = EINVAL;
		goto Ret;
	    }

	    u.u_dirp = argv[0];

	    /* смотрим, что лежит в указуемом файле */
	    if ((ip = namei(schar, LOOKUP, 1)) == NULL) {
# ifdef SCHED
		u.u_procp->p_sflag &= ~SEXEC;
# endif SCHED
		return;
	    }
	    goto again;
	}
	if( indir == NSYMLNK ){
		u.u_error = ELOOP;
		goto bad;
	}

NoScript:
	/*other magic numbers are described in getxfile()*/

	/*
	 * Collect arguments on "file" in swap space.
	 * ip - locked ino of executable core image file.
	 */
	uap = (struct execa *)u.u_ap;
	ap = na = ne = nc = 0;

	/* заказать в своппинге место для размещения
	 * аргументов и окружения новой программы
	 */
#ifndef UCB_NKB
	if ((bno = malloc(swapmap, (NCARGS + BSIZE - 1) / BSIZE)) == 0) {
#else   UCB_NKB
	if ((bno = malloc(swapmap, ctod((int) btoc(NCARGS + BSIZE)))) == 0) {
#endif  UCB_NKB
		/* panicreboot++;
		 * panic("out of swap");
		 */
		printf( "misswap\n");
		u.u_error = ENOSPC;
		goto bad;
	}

     /* формат:
	 пусть есть файл   /.../.../x
	 со следующим содержимым:
	 -------------------------------
	 |#!CMD A1 A2 A3
	 | .......

	Вызываем команду:
		x B1 B2

	Запустится на самом деле такая команда:
		CMD A1 A2 A3 /.../.../x B1 B2

	Один из аргументов Ai может иметь вид $*
	Тогда аргументы запуска подставятся на это место, а не в конец:
	 -------------------------------
	 |#!CMD A1 $* A2 A3
	 | .......

	Вызываем:
		x B1 B2

	Запустится:
		CMD A1 /.../.../x B1 B2 A2 A3
     */

	if( indir ) AV = argv;
	else        AV = NULL;

	/* цикл чтения сначала argp, затем envp в своппинг */
	if ( AP ) for (;;) {

GetNext:

		/* здесь ap используется как флаг, что можно
		 * брать аргументы из argv
		 */
		if( AV  && ! ap ){
			next = *AV++;
			if( next == NULL ){
				AV = NULL;
				goto GetNext;
			}
			if( !bcmp( next, ARGPLACE, 2 ))
				goto Argp;
			local = 1;
		}else

Argp:           if( AP ){
			next = fuword ( (caddr_t) AP++ );
			if( next == NULL ){
				AP = NULL;
				ap = 0;
				/* можно дочитывать argv,
				 * если еще осталось
				 */
				goto GetNext;
			}
			/* полное имя косвенного файла задаем как
			 * argp0 скриптовой программе.
			 */
			if( ap == 0 && indir )
				next = (caddr_t) (uap->fname);
			local = 0;
			ap++;   /* не ноль */
		}else
		if( EP ){
			next = fuword( (caddr_t) EP++ );
			if( next == NULL ) break;
			local = 0;
			ne++;
			ap++;
		}else
			break;

		/* число пересланных строк */
		na++;
		if (next == (char *)-1)
			u.u_error = EFAULT;

		/* Адрес строки сейчас хранится в next.
		 * Начинаем цикл пересылки строки в "файл" в своппинге
		 */
		do {
			if (nc >= NCARGS - 1)
				u.u_error = E2BIG;
			if( local ){
			     /* строка - в пространстве ядра */
			     c = *next++;
			     c &= 0377;
			} else
			     /* строка - в пространстве юзера */
			     if ((c = fubyte((caddr_t) next++)) < 0)
				   u.u_error = EFAULT;

			if (u.u_error)
				goto bad;
			if ((nc & BMASK) == 0) {
				if (bp) {
					mapout(bp);
					bdwrite(bp);
				}
			/* заказываем кэш-буфер для записи в
			 * "своппинговый файл"
			 */
#ifndef UCB_NKB
				bp = getblk(swapdev, swplo + bno + (nc >> BSHIFT));
#else
				bp = getblk(swapdev,
				  dbtofsb(clrnd(swplo + bno)) + (nc >> BSHIFT));
#endif
				cp = mapin(bp);
			}

			/* число засланных байтов ++ */
			nc++;

			/* заносим байт в кэш-буфер */
			*cp++ = c;

		} while (c > 0);        /* пока не конец строки */
	}

	if (bp) {
		mapout(bp);
		bdwrite(bp);
	}

	/* выделяем память для новой программы,
	 * зачитываем программу из файла в эту память
	 */
	bp = 0;
	nc = (nc + NBPW - 1) & ~(NBPW - 1);
	if (getxfile(ip, (na * NBPW) + nc, uid, gid) || u.u_error)
		goto bad;

	/*
	 * copy back arglist
	 * формируем стек нового процесса (для вызова main)
	 */

	ucp = -nc - NBPW;
	ap = ucp - na * NBPW - 3 * NBPW;
	u.u_ar0[R6] = ap;
	suword((caddr_t)ap, na - ne);
	nc = 0;
	for (;;) {
		ap += NBPW;
		if (na == ne) {
			suword((caddr_t)ap, 0);
			ap += NBPW;
		}
		if (--na < 0)
			break;
		suword((caddr_t)ap, ucp);
		do {
			if ((nc & BMASK) == 0) {
				if (bp) {
					mapout(bp);
					bp->b_flags |= B_AGE;
					brelse(bp);
				}
#ifndef UCB_NKB
				bp = bread(swapdev, swplo + bno + (nc>>BSHIFT));
#else
				bp = bread(swapdev,
				  dbtofsb(clrnd(swplo + bno)) + (nc >> BSHIFT));
#endif
				bp->b_flags &= ~B_DELWRI;
				cp = mapin(bp);
				/* stick in interpreter name for accounting */
				if (indir && nc == 0)
					bcopy(cp, (caddr_t)u.u_dbuf, DIRSIZ);
			}
			subyte((caddr_t)ucp++, (c = *cp++));
			nc++;
		} while(c & 0377);
	}
	suword((caddr_t) ap, 0);
	suword((caddr_t) (-NBPW), 0);
	/* завершено формирование стека */

	if (bp) {
		mapout(bp);
		bp->b_flags |= B_AGE;
		brelse(bp);
		bp = 0;
	}
	setregs();

bad:
#ifdef SCHED
	u.u_procp->p_sflag &= ~SEXEC;
#endif SCHED
	if (bp) {
		mapout(bp);
		bp->b_flags |= B_AGE;
		brelse(bp);
	}
	if (bno)
#ifndef UCB_NKB
		mfree(swapmap, (NCARGS + BSIZE - 1) / BSIZE, bno);
#else
		mfree(swapmap, ctod((int) btoc(NCARGS + BSIZE)), bno);
#endif
	iput(ip);
}

/*
 * Read in and set up memory for executed file.
 * Zero return is normal;
 * non-zero means only the text is being replaced
 */
getxfile(ip, nargc, uid, gid)
int nargc, uid, gid;
register struct inode *ip;
{
	register unsigned ds;
	register sep;
	register unsigned ts, ss;
	register i, overlay;
	register ovflag,ovmax;
	struct u_ovd sovdata;
	register xovflag;
	unsigned ovhead[1 + NXOVL];
	long lsize;

	sep = 0;
	overlay = 0;
	ovflag = 0;
	xovflag = 0;
	if (u.u_exdata.ux_mag == A_MAGIC1) {
		lsize = (long) u.u_exdata.ux_dsize + u.u_exdata.ux_tsize;
		u.u_exdata.ux_dsize = lsize;
		if (lsize != u.u_exdata.ux_dsize) {     /* check overflow */
			u.u_error = ENOMEM;
			goto bad;
		}
		u.u_exdata.ux_tsize = 0;
	} else if (u.u_exdata.ux_mag == A_MAGIC3)
		sep++;
	else if (u.u_exdata.ux_mag == A_MAGIC4)
		overlay++;
	else if (u.u_exdata.ux_mag == A_MAGIC5)
		ovflag++;
	else if (u.u_exdata.ux_mag == A_MAGIC7) {
		ovflag++;
		xovflag++;
	}
	else if (u.u_exdata.ux_mag == A_MAGIC6) {
		sep++;
		ovflag++;
	}
	else if (u.u_exdata.ux_mag != A_MAGIC2) {
		u.u_error = ENOEXEC;
		goto bad;
	}
	if (u.u_exdata.ux_tsize!=0 && (ip->i_flag&ITEXT)==0 && ip->i_count!=1) {
		u.u_error = ETXTBSY;
		goto bad;
	}

	/*
	 * find text and data sizes
	 * try them out for possible
	 * overflow of max sizes
	 */
	ts = btoc(u.u_exdata.ux_tsize);
	lsize = (long) u.u_exdata.ux_dsize + u.u_exdata.ux_bsize;
	if (lsize != (unsigned) lsize) {
		u.u_error = ENOMEM;
		goto bad;
	}
	ds = btoc(lsize);
	ss = SSIZE + btoc(nargc);

	/*
	 * if auto overlay get second header
	 */

	sovdata = u.u_ovdata;
	u.u_ovdata.uo_ovbase = 0;
	u.u_ovdata.uo_curov  = 0;
	u.u_ovdata.uo_maxov  = 0;       /* used for checks in EMT handler */
	if (ovflag) {
		register n_ovl1 = (xovflag?NXOVL:NOVL);

		u.u_ovdata.uo_maxov = n_ovl1++;
		u.u_count = sizeof(unsigned)*n_ovl1;
		u.u_base = (caddr_t) ovhead;
		u.u_offset = sizeof(u.u_exdata);
		u.u_segflg = 1;
		readi(ip);
		u.u_segflg = 0;
		if (u.u_count != 0)
			u.u_error = ENOEXEC;
		if (u.u_error) {
			u.u_ovdata = sovdata;
			goto bad;
		}
		/* set beginning of overlay segment */
		u.u_ovdata.uo_ovbase = ctos(ts);

		/* 0th entry is max size of the overlays */
		ovmax = btoc(ovhead[0]);

		/* set max number of segm. registers to be used */
		u.u_ovdata.uo_nseg = ctos(ovmax);

		/* set base of data space */
		u.u_ovdata.uo_dbase = stoc(u.u_ovdata.uo_ovbase + u.u_ovdata.uo_nseg);

		/*
		 * Set up a table of offsets to each of the
		 * overlay segements. The ith overlay runs
		 * from ov_offst[i-1] to ov_offst[i].
		 */
		u.u_ovdata.uo_ov_offst[0] = ts;
		for (i = 1; i < n_ovl1; i++) {
			register t;
			/* check if any overlay is larger than ovmax */
			if ((t=btoc(ovhead[i])) > ovmax) {
				u.u_error = ENOEXEC;
				u.u_ovdata = sovdata;
				goto bad;
			}
			u.u_ovdata.uo_ov_offst[i] =
				t + u.u_ovdata.uo_ov_offst[i - 1];
		}
	}

	if (overlay) {
		if (u.u_sep == 0 && ctos(ts) != ctos(u.u_tsize) || nargc) {
			u.u_error = ENOMEM;
			goto bad;
		}
		ds = u.u_dsize;
		ss = u.u_ssize;
		sep = u.u_sep;
		xfree();
		xalloc(ip);
		u.u_ar0[PC] = u.u_exdata.ux_entloc & ~01;
	} else {
		if (estabur(ts, ds, ss, sep, RO)) {
			u.u_ovdata = sovdata;
			goto bad;
		}

		/*
		 * allocate and clear core
		 * at this point, committed
		 * to the new image
		 */

		u.u_prof.pr_scale = 0;
#ifdef  VIRUS_VFORK
		if (u.u_procp->p_flag & SVFORK)
			endvfork();
		else
			xfree();
		expand(ds, S_DATA);
		clear(u.u_procp->p_daddr, ds);
		expand(ss,S_STACK);
		clear(u.u_procp->p_saddr, ss);
#else
		xfree();
		i = USIZE + ds + ss;
		expand(i);
		clear(u.u_procp->p_addr + USIZE, i - USIZE);
#endif
		xalloc(ip);

		/*
		 * read in data segment
		 */
		estabur((unsigned)0, ds, (unsigned)0, 0, RO);
		u.u_base = 0;
		u.u_offset = sizeof(u.u_exdata);
		if (ovflag) {
			u.u_offset += sizeof(unsigned)*((xovflag?NXOVL:NOVL)+1);
			u.u_offset +=
		(((long)u.u_ovdata.uo_ov_offst[xovflag?NXOVL:NOVL]) << 6);
		}
		else
			u.u_offset += u.u_exdata.ux_tsize;
		u.u_count = u.u_exdata.ux_dsize;
		readi(ip);

		/*
		 * set SUID/SGID protections, if no tracing
		 */
		if ((u.u_procp->p_flag & STRC) == 0) {
			u.u_uid = uid;
			u.u_procp->p_uid = uid;
			u.u_gid = gid;
		} else
			psignal(u.u_procp, SIGTRAP);
	}
	u.u_tsize = ts;
	u.u_dsize = ds;
	u.u_ssize = ss;
	u.u_sep = sep;
	estabur(ts, ds, ss, sep, RO);
bad:
	return(overlay);
}

/*
 * Clear registers on exec
 */
setregs()
{
	register int (**rp)();
	long sigmask;
	register char *cp;
	register i;

	u.u_procp->p_flag &= ~SNUSIG;
	for(rp = &u.u_signal[1], sigmask = 1L; rp < &u.u_signal[NSIG];
	    sigmask <<= 1, rp++) {
		switch (*rp) {

		case SIG_HOLD:
			u.u_procp->p_flag |= SNUSIG;
			continue;
		case SIG_IGN:
		case SIG_DFL:
			continue;

		default:
			/*
			 * Normal or deferring catch; revert to default.
			 */
			(void) _spl6();
			*rp = SIG_DFL;
			if ((int)SIG_DFL & 1)
				u.u_procp->p_siga0 |= sigmask;
			else
				u.u_procp->p_siga0 &= ~sigmask;
			if ((int)SIG_DFL & 2)
				u.u_procp->p_siga1 |= sigmask;
			else
				u.u_procp->p_siga1 &= ~sigmask;
			(void) _spl0();
			continue;
		}
	}
	for(cp = &regloc[0]; cp < &regloc[6];)
		u.u_ar0[*cp++] = 0;
	u.u_ar0[PC] = u.u_exdata.ux_entloc & ~01;
#ifndef NONFP
	for(rp = (int *)&u.u_fps; rp < (int *)&u.u_fps.u_fpregs[6];)
		*rp++ = 0;
#endif
	for(i=0; i<NOFILE; i++) {
		if (u.u_pofile[i]&EXCLOSE) {
#ifndef UCB_NET
			closef(u.u_ofile[i]);
#else
			closef(u.u_ofile[i],1);
#endif
			u.u_ofile[i] = NULL;
			u.u_pofile[i] &= ~EXCLOSE;
		}
	}
#ifdef  ACCT
	u.u_acflag &= ~AFORK;
#endif
	/*
	 * Remember file name.
	 */
	bcopy((caddr_t)u.u_dbuf, (caddr_t)u.u_comm, DIRSIZ);
}

/*
 * exit system call:
 * pass back caller's arg
 */
rexit()
{
	register struct a {
		int     rval;
	} *uap;

	uap = (struct a *)u.u_ap;
	exit((uap->rval & 0377) << 8);
}

/*
 * Release resources.
 * Save u. area for parent to look at.
 * Enter zombie state.
 * Wake up parent and init processes,
 * and dispose of children.
 */
exit(rv)
{
	register int i;
	register struct proc *p, *q;
	register struct file *f;

	p = u.u_procp;
	p->p_flag &= ~(STRC|SULOCK);
	p->p_clktim = 0;
#ifdef  CGL_RTP
	/*
	 * if this a "real time" process that is dying
	 * remove the rtpp flag.
	 */
	if (rtpp != NULL && rtpp == p)
		rtpp = NULL;
#endif
	(void) _spl6();
	if ((int)SIG_IGN & 1)
		p->p_siga0 = ~0L;
	else
		p->p_siga0 = 0L;
	if ((int)SIG_IGN & 2)
		p->p_siga1 = ~0L;
	else
		p->p_siga1 = 0L;
	(void) _spl0();
	for(i=0; i<NSIG; i++)
		u.u_signal[i] = SIG_IGN;
	for(i=0; i<NOFILE; i++) {
		f = u.u_ofile[i];
		u.u_ofile[i] = NULL;
#ifndef UCB_NET
		closef(f);
#else
		closef(f,1);
#endif
	}
	plock(u.u_cdir);
	iput(u.u_cdir);
	if (u.u_rdir) {
		plock(u.u_rdir);
		iput(u.u_rdir);
	}
#if SEMMNS > 0
	semexit();
#endif
#ifdef  ACCT
	acct();
#endif  ACCT
#ifdef  VIRUS_VFORK
	if (p->p_flag & SVFORK) {
		endvfork();
	} else {
		xfree();
		mfree(coremap, p->p_dsize, p->p_daddr);
		mfree(coremap, p->p_ssize, p->p_saddr);
	}
	mfree(coremap, USIZE, p->p_addr);
#else   VIRUS_VFORK
	xfree();
	mfree(coremap, p->p_size, p->p_addr);
#endif  VIRUS_VFORK
	p->p_stat = SZOMB;
#ifdef RUBIN
	ilrma(p->p_pid);
#endif RUBIN
	if (p->p_pid == 1) {
		/*
		 * If /etc/init is not found by the icode,
		 * the stack size will still be zero when it exits.
		 * Don't panic: we're unlikely to find init after a reboot,
		 * either.
		 */
		if (u.u_ssize == 0) {
			printf("can't exec init\n");
			for (;;)
				idle();
		} else {
			panicreboot++;
			panic("init died");
		}
	}
	p->p_un.xp_xstat = rv;
	p->p_un.xp_utime = u.u_cutime + u.u_utime;
	p->p_un.xp_stime = u.u_cstime + u.u_stime;
#ifdef  UCB_LOGIN
	p->p_un.xp_login = u.u_login;
#endif
	for(q = &proc[0]; q <= maxproc; q++)
		if (q->p_pptr == p) {
			q->p_pptr = &proc[1];
			q->p_ppid = 1;
			wakeup((caddr_t)&proc[1]);
			/*
			 * Traced processes are killed
			 * since their existence means someone is screwing up.
			 * Stopped processes are sent a hangup and a continue.
			 * This is designed to be ``safe'' for setuid
			 * processes since they must be willing to tolerate
			 * hangups anyways.
			 */
			if (q->p_flag&STRC) {
				q->p_flag &= ~STRC;
				psignal(q, SIGKILL);
			} else if (q->p_stat == SSTOP) {
				psignal(q, SIGHUP);
				psignal(q, SIGCONT);
			}
			/*
			 * Protect this process from future
			 * tty signals, clear TSTP/TTIN/TTOU if pending,
			 * and set SDETACH bit on procs.
			 */
			spgrp(q, -1);
		}
	wakeup((caddr_t)p->p_pptr);
	psignal(p->p_pptr, SIGCHLD);
	swtch();
}

/*
 * Wait system call.
 * Search for a terminated (zombie) child,
 * finally lay it to rest, and collect its status.
 * Look also for stopped (traced) children,
 * and pass back status from them.
 */
wait()
{
	register f;
	register struct proc *p;
	register options;

	options = (u.u_ar0[RPS] & PS_ALLCC) == PS_ALLCC ?  u.u_ar0[R0] : 0;
	f = 0;

loop:
	for(p = &proc[0]; p <= maxproc; p++)
	if (p->p_pptr == u.u_procp)
		{
		f++;
		if (p->p_stat == SZOMB) {
			u.u_r.r_val1 = p->p_pid;
			u.u_r.r_val2 = p->p_un.xp_xstat;
			p->p_un.xp_xstat = 0;
			p->p_pptr = 0;
			p->p_siga0 = 0L;
			p->p_siga1 = 0L;
			p->p_cursig = 0;
			u.u_cutime += p->p_un.xp_utime;
			u.u_cstime += p->p_un.xp_stime;
			p->p_pid = 0;
			p->p_ppid = 0;
			p->p_pgrp = 0;
			p->p_sig = 0;
			p->p_flag = 0;
			p->p_wchan = 0;
			p->p_stat = NULL;
			if (p == maxproc)
				while (maxproc->p_stat == NULL)
					maxproc--;
			return;
		}
		if (p->p_stat == SSTOP && (p->p_flag & SWTED) == 0 &&
		   (p->p_flag & STRC || options & WUNTRACED )){
			p->p_flag |= SWTED;
			u.u_r.r_val1 = p->p_pid;
			u.u_r.r_val2 = (p->p_cursig << 8) | 0177;
			return;
		}
	}
	if (f) {
		if (options & WNOHANG) {
			u.u_r.r_val1 = 0;
			return;
		} else {
			if ((u.u_procp->p_flag & SNUSIG) && save(u.u_qsav)) {
				u.u_eosys = RESTARTSYS;
				return;
			}
			sleep((caddr_t)u.u_procp, PWAIT);
			goto loop;
		}
	}
	u.u_error = ECHILD;
}

/*
 * fork system call.
 */
#ifdef  VIRUS_VFORK
fork()
{
	fork1(0);
}

/*
 * vfork system call
 */
vfork()
{
	fork1(1);
}

fork1(isvfork)
#else   VIRUS_VFORK
fork()
#endif
{
#ifdef  UCB_PGRP
	register struct proc *p1;
	struct proc *p2;
	register a, pg;
#else
	register struct proc *p1, *p2;
	register a;
#endif

	/*
	 * Make sure there's enough swap space for max
	 * core image, thus reducing chances of running out
	 * If swapping area size < 1.5 Mb, avoid this test,
	 * for the sake of PC.
	 */
	if( nswap >= 3000 ) {
		if ((a = malloc(swapmap, ctod(maxmem))) == 0) {
		       u.u_error = ENOMEM;
		       goto out;
		}
		mfree(swapmap, ctod(maxmem), a);
	}
	a = 0;
#ifdef  UCB_PGRP
	pg = u.u_procp->p_pgrp; /* process group number */
#endif
	p2 = NULL;
	for(p1 = proc; p1 < procNPROC; p1++) {
		if (p1->p_stat==NULL && p2==NULL)
			p2 = p1;
		else {
			/*
			 * Exempt low positive uids (0-15) for users like uucp
			 * and network, which shouldn't lose limits.
			 */
#ifdef  UCB_PGRP
			if (p1->p_pgrp==pg && (unsigned) u.u_uid>=16
			    && p1->p_stat!=NULL)
				a++;
#else
			if ((p1->p_uid==u.u_uid && p1->p_stat!=NULL)
			    && ((unsigned) u.u_uid >= 16))
				a++;
#endif
		}
	}
	/*
	 * Disallow if
	 *  No processes at all;
	 *  not su and too many procs owned (or in pgrp, if UCB_PGRP set);
	 *  or not su and would take last slot.
	 */
	if (p2 == NULL)
		tablefull("proc");
	if (p2 == NULL || (u.u_uid != 0 && (p2 == procNPROC-1 || a > MAXUPRC))){
		u.u_error = EAGAIN;
		goto out;
	}
	p1 = u.u_procp;
#ifdef  VIRUS_VFORK
	if (newproc(isvfork))
#else
	if (newproc())
#endif
		{
		u.u_r.r_val1 = p1->p_pid;
		u.u_start = time;
		u.u_cstime = 0;
		u.u_stime = 0;
		u.u_cutime = 0;
		u.u_utime = 0;
#ifdef  UCB_LOGIN
		u.u_login = 0;
#endif
#ifdef  ACCT
		u.u_acflag = AFORK;
#endif
		return;
	}
	u.u_r.r_val1 = p2->p_pid;

out:
	u.u_ar0[R7] += NBPW;
}

/*
 * break system call.
 *  -- bad planning: "break" is a dirty word in C.
 */
sbreak()
{
	struct a {
		char    *nsiz;
	};
	register a, n, d;
	int i;

	/*
	 * set n to new data size
	 * set d to new-old
	 * set n to new total size
	 */

	n = btoc((int) ((struct a *) u.u_ap)->nsiz);
	if (!u.u_sep)
		if (u.u_ovdata.uo_ovbase)
			n -= u.u_ovdata.uo_dbase;
		else
			n -= ctos(u.u_tsize) * stoc(1);
	if (n < 0)
		n = 0;
#ifdef  VIRUS_VFORK
	d = n - u.u_dsize;
	if (estabur(u.u_tsize, n, u.u_ssize, u.u_sep, RO))
		return;
	expand(n,S_DATA);
	if (d > 0)
		clear(u.u_procp->p_daddr + u.u_dsize, d);
	u.u_dsize = n;

#else   VIRUS_VFORK
	d = n - u.u_dsize;
	n += USIZE+u.u_ssize;
	if (estabur(u.u_tsize, u.u_dsize+d, u.u_ssize, u.u_sep, RO))
		return;
	u.u_dsize += d;
	if (d > 0)
		goto bigger;
	a = u.u_procp->p_addr + n - u.u_ssize;
	copy(a-d, a, u.u_ssize);        /* d is negative */
	expand(n);
	return;

bigger:
	expand(n);
	a = u.u_procp->p_addr + n - u.u_ssize - d;
	n = u.u_ssize;
	while (n >= d) {
		n -= d;
		copy(a+n, a+n+d, d);
	}
	copy(a, a+d, n);
	clear(a, d);
#endif  VIRUS_VFORK
}
