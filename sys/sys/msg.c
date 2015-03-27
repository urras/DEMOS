/* @(#)msg.c    6.1 */
/*
**      Inter-Process Communication Message Facility.
**      $Log:	msg.c,v $
 * Revision 22.7  90/11/12  19:14:09  root
 * Новые вещи для СМ1425 и перемещение include.
 * 
 * Revision 22.6  89/05/11  14:10:53  abs
 * перестановки порядка includ-ов
 * 
 * Revision 22.5  89/05/11  13:46:54  abs
 * завел msginfo
 *
 * Revision 22.4  89/04/28  17:00:53  avg
 * Добавлен #include "h/space.h" для вытаскивания констант SEMMNS и MSGMNI.
 *
 * Revision 22.3  89/04/12  15:27:29  korotaev
 * "param.h" ==> <sys/param.h>
 *
 * Revision 22.2  89/04/06  18:31:06  abs
 * была вставлена не совсем так.
 *
 * Revision 22.1  89/04/06  15:26:16  abs
 * добавлена переменная msgtotal для нужд программы checksys
 *
 * Revision 22.0  89/03/30  15:50:50  korotaev
 * Begining DEMOS 2.2.
 *
 * Revision 1.3  89/03/13  21:06:45  abs
 * при попытке увеличить емкость очереди (msgctl) приходит ошибка EFBIG
 *
 * Revision 1.2  89/03/12  12:29:48  abs
 * msgrcv со флагом IPC_EXCL принимает сообщения любого типа,
 * КРОМЕ указанного в аргументе (например, кроме отправленных тем же процессом)
 *
 * Revision 1.1  89/03/10  15:50:45  abs
 * Initial revision
 *
*/

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>    /* только для user.h */
#include <sys/types.h>
#include <signal.h>
#include <sys/user.h>
#include <sys/proc.h>
#include "../include/buf.h"
#include <errno.h>
#include "../include/map.h"
#include "h/space.h"    /* Для MSGMNI */
#include <sys/ipc.h>
#include <sys/msg.h>


#if MSGMNI > 0

#define paddr_t  long    /* 32-х битовое целое: адрес в физпамяти ЭВМ */
#define u_rval1  u_r.r_val1
#define u_rval2  u_r.r_val2

#ifdef pdp11
# define MOVE   io5move
#else
# define MOVE   iomove
#endif

struct mapent _msgmap[ MSGMAP ];
struct map     msgmap[1] = {
	_msgmap, & _msgmap[MSGMAP],   "msgmap" };   /* msg allocation map */

struct msqid_ds  msgque[MSGMNI];             /* msg queue headers */
struct msg       msgh  [MSGTQL];             /* message headers */
struct msg              *msgfp;          /* ptr to head of free header list */
paddr_t                 msg;             /* base address of message buffer */
					 /* адрес в БАЙТАХ (no clicks !) */
long msgtotal = MSGSEG * MSGSSZ;         /* размер буферов в памяти */
extern time_t           time;            /* system idea of date */

extern struct msqid_ds  *ipcget(),
			*msgconv();

u_short want_msg;       /* сколько процессов спят,
			   ожидая память под текст сообщения */

struct msginfo msginfo = {
	MSGMAP,
	MSGMAX,
	MSGMNB,
	MSGMNI,
	MSGSSZ,
	MSGTQL,
	MSGSEG
};

/* Convert bytes to msg segments. */
/* Здесь для pdp11 можно было бы использовать btoc(),
 * ибо сегмент - это click
 */
#define btoq(X) ((X + MSGSSZ - 1) / MSGSSZ)


/*
**      msgconv - Convert a user supplied message queue id into a ptr to a
**              msqid_ds structure.
*/

struct msqid_ds *
msgconv(id)
register int    id;
{
	register struct msqid_ds        *qp;    /* ptr to associated q slot */

	qp = &msgque[id % MSGMNI];
	if((qp->msg_perm.mode & IPC_ALLOC) == 0 ||
		id / MSGMNI != qp->msg_perm.seq) {
		u.u_error = /* EINVAL; */ ENOENT;
		return(NULL);
	}
	return(qp);
}

/*
**      msgctl - Msgctl system call.
*/

msgctl()
{
	register struct a {
		int             msgid,
				cmd;
		struct msqid_ds *buf;
	}               *uap = (struct a *)u.u_ap;
	struct msqid_ds                 ds;     /* queue work area */
	register struct msqid_ds        *qp;    /* ptr to associated q */

	if((qp = msgconv(uap->msgid)) == NULL)
		return;
	u.u_rval1 = 0;
	switch(uap->cmd) {
	case IPC_RMID:
		if(u.u_uid != qp->msg_perm.uid && u.u_uid != qp->msg_perm.cuid
			&& !suser())
			return;
		while(qp->msg_first)
			msgfree(qp, NULL, qp->msg_first);
		qp->msg_cbytes = 0;
		if(uap->msgid + MSGMNI < 0)
			qp->msg_perm.seq = 0;
		else
			qp->msg_perm.seq++;
		if(qp->msg_perm.mode & MSG_RWAIT)
			wakeup(&qp->msg_qnum);
		if(qp->msg_perm.mode & MSG_WWAIT)
			wakeup(qp);
		qp->msg_perm.mode = 0;
		return;
	case IPC_SET:
		if(u.u_uid != qp->msg_perm.uid && u.u_uid != qp->msg_perm.cuid
			 && !suser())
			return;
		if(copyin((caddr_t)uap->buf, (caddr_t)&ds, sizeof(ds))) {
			u.u_error = EFAULT;
			return;
		}
		if(ds.msg_qbytes > qp->msg_qbytes && !suser()){
			/* нельзя увеличивать емкость очереди */
			u.u_error = EFBIG;
			return;
		}
		qp->msg_perm.uid = ds.msg_perm.uid;
		qp->msg_perm.gid = ds.msg_perm.gid;
		qp->msg_perm.mode = (qp->msg_perm.mode & ~0777) |
			(ds.msg_perm.mode & 0777);
		qp->msg_qbytes = ds.msg_qbytes;
		qp->msg_ctime = time;
		return;
	case IPC_STAT:
		if(ipcaccess(&qp->msg_perm, MSG_R))
			return;
		if(copyout((caddr_t)qp, (caddr_t)uap->buf, sizeof(*qp))) {
			u.u_error = EFAULT;
			return;
		}
		return;
	default:
		u.u_error = EINVAL;
		return;
	}
}

/* очистить кусок памяти из msgmap,
 * разбудить процессы, ожидающие освобождения памяти
 */
mmapfree( size, addr )
	size_t size;
	memaddr addr;
{
	mfree( msgmap, size, addr );
	if( want_msg ){
		want_msg = 0;
		wakeup( (caddr_t) msgmap );
	}
}

/*
**      msgfree - Free up space and message header, relink pointers on q,
**      and wakeup anyone waiting for resources.
*/

msgfree(qp, pmp, mp)
register struct msqid_ds        *qp;    /* ptr to q of mesg being freed */
register struct msg             *mp,    /* ptr to msg being freed */
				*pmp;   /* ptr to mp's predecessor */
{
	/* Unlink message from the q. */
	if(pmp == NULL)
		qp->msg_first = mp->msg_next;
	else
		pmp->msg_next = mp->msg_next;
	if(mp->msg_next == NULL)
		qp->msg_last = pmp;
	qp->msg_qnum--;
	if(qp->msg_perm.mode & MSG_WWAIT) {
		qp->msg_perm.mode &= ~MSG_WWAIT;
		wakeup(qp);
	}

	/* Free up message text. */
	if(mp->msg_ts)
		mmapfree( btoq(mp->msg_ts), mp->msg_spot + 1);

	/* Free up header */
	mp->msg_next = msgfp;
	if(msgfp == NULL)
		wakeup(&msgfp);
	msgfp = mp;
}

/*
**      msgget - Msgget system call.
*/

msgget()
{
	register struct a {
		key_t   key;
		int     msgflg;
	}       *uap = (struct a *)u.u_ap;
	register struct msqid_ds        *qp;    /* ptr to associated q */
	int                             s;      /* ipcget status return */

	if((qp = ipcget(uap->key, uap->msgflg, msgque, MSGMNI, sizeof(*qp), &s))
		== NULL)
		return;

	if(s) {
		/* This is a new queue.  Finish initialization. */
		qp->msg_first = qp->msg_last = NULL;
		qp->msg_qnum = 0;
		qp->msg_qbytes = MSGMNB;
		qp->msg_lspid = qp->msg_lrpid = 0;
		qp->msg_stime = qp->msg_rtime = 0;
		qp->msg_ctime = time;
	}
	u.u_rval1 = qp->msg_perm.seq * MSGMNI + (qp - msgque);
}

/*
**      msginit - Called by main(main.c) to initialize message queues.
*/

msginit()
{
	register int            i;      /* loop control */
	register struct msg     *mp;    /* ptr to msg begin linked */
	register int            bs;     /* message buffer size (segments) */

	/* Allocate physical memory for message buffer. */
#ifdef pdp11
	if((msg = (paddr_t)ctob((long)(unsigned)malloc(coremap,
		bs=(int)btoc(msgtotal)))) == 0) {
	/* msg - адрес (байтовый) мессадж-буферов в памяти   */
	/* bs  - то же самое что MSGSEG (кликов по 64 байта) */
#endif
#ifdef vax
	if((msg = (paddr_t)sptalloc(bs=btoc(msgtotal),
		PG_V | PG_KW, 0)) == NULL) {
#endif
#ifdef u3b
	if((msg = (paddr_t)kseg(RW, btop(msgtotal))) ==
		NULL) {
#endif
		/* printf("Can't allocate message buffer.\n"); */
		panic( "msginit");
		/* MSGSEG = 0; */
	}
	/* mapinit(msgmap, MSGMAP); */
	mfree(msgmap, MSGSEG, 1);
	for(i = 0, mp = msgfp = msgh;++i < MSGTQL ; mp++)
		mp->msg_next = mp + 1;

	maxmem -= bs;           /* clicks */
	return(bs);
}

/*
**      msgrcv - Msgrcv system call.
*/

msgrcv()
{
	register struct a {
		int             msqid;
		struct msgbuf   *msgp;
		int             msgsz;
		long            msgtyp;
		int             msgflg;
	}       *uap = (struct a *)u.u_ap;
	register struct msg             *mp,    /* ptr to msg on q */
					*pmp,   /* ptr to mp's predecessor */
					*smp,   /* ptr to best msg on q */
					*spmp;  /* ptr to smp's predecessor */
	register struct msqid_ds        *qp;    /* ptr to associated q */
	int                             sz;     /* transfer byte count */

	if((qp = msgconv(uap->msqid)) == NULL)
		return;
	if(ipcaccess(&qp->msg_perm, MSG_R))
		return;
	if(uap->msgsz < 0) {
		u.u_error = EINVAL;
		return;
	}
	smp = spmp = NULL;
findmsg:
	/* поиск подходящего сообщения */
	pmp = NULL;
	mp = qp->msg_first;

	if(uap->msgtyp == 0)    /* first message in q. of any type */
		smp = mp;
	else
		for(;mp;pmp = mp, mp = mp->msg_next) {
			if(uap->msgtyp > 0) {
				if( uap->msgflg & IPC_EXCL ){
				      if(uap->msgtyp == mp->msg_type)
					    continue;
				}else if(uap->msgtyp != mp->msg_type)
					    continue;
				smp = mp;
				spmp = pmp;
				break;
			}
			if(mp->msg_type <= -uap->msgtyp) {
				if(smp && smp->msg_type <= mp->msg_type)
					continue;
				smp = mp;
				spmp = pmp;
				/* and continue searching */
			}
		}
	if(smp) {
		/* message found */
		if(uap->msgsz < smp->msg_ts)
			if(!(uap->msgflg & MSG_NOERROR)) {
				u.u_error = E2BIG;
				return;
			} else
				sz = uap->msgsz;
		else
			sz = smp->msg_ts;
		copyout((caddr_t) &smp->msg_type, (caddr_t) uap->msgp, sizeof(smp->msg_type));
		if(u.u_error)
			return;
		if(sz) {
			u.u_base = (caddr_t)uap->msgp + sizeof(smp->msg_type);
			u.u_segflg = 0;
			MOVE((paddr_t) msg + MSGSSZ * smp->msg_spot,
				sz, B_READ);
			if(u.u_error)
				return;
		}
		u.u_rval1 = sz;
		qp->msg_cbytes -= smp->msg_ts;
		qp->msg_lrpid = u.u_procp->p_pid;
		qp->msg_rtime = time;
		curpri = PMSG;
		msgfree(qp, spmp, smp);
		return ;
	}

	/* no applicable messages, wait */
	if(uap->msgflg & IPC_NOWAIT) {
		u.u_error = ENOMSG;
		return;
	}
	qp->msg_perm.mode |= MSG_RWAIT;
	if(sleep(&qp->msg_qnum, PMSG | PCATCH)) {
		u.u_error = EINTR;
		return;
	}
	if(msgconv(uap->msqid) == NULL) {
		u.u_error = EIDRM;
		return;
	}
	goto findmsg;
}

/*
**      msgsnd - Msgsnd system call.
*/

msgsnd()
{
	register struct a {
		int             msqid;
		struct msgbuf   *msgp;
		int             msgsz;
		int             msgflg;
	}       *uap = (struct a *)u.u_ap;
	register struct msqid_ds        *qp;    /* ptr to associated q */
	register struct msg             *mp;    /* ptr to allocated msg hdr */
	register int                    cnt,    /* byte count */
					spot;   /* msg pool allocation spot */
	long                            type;   /* msg type */

	if((qp = msgconv(uap->msqid)) == NULL)
		return;
	if(ipcaccess(&qp->msg_perm, MSG_W))
		return;
	if((cnt = uap->msgsz) < 0 || cnt > MSGMAX) {
		u.u_error = /* EINVAL; */ EFBIG;
		return;
	}
	copyin((caddr_t) uap->msgp, (caddr_t) &type, sizeof(type));
	if(u.u_error)
		return;
	if(type < 1l ) {
		u.u_error = EINVAL;
		return;
	}
getres:
	/* get resources for the message */

	/* Be sure that q has not been removed. */
	if(msgconv(uap->msqid) == NULL) {
		u.u_error = EIDRM;
		return;
	}

	/* Allocate space on q, message header, & buffer space. */
	if(cnt + qp->msg_cbytes > qp->msg_qbytes) {
		/* обязательно MSGMAX <= MSGMNB , иначе deadlock */

		if(uap->msgflg & IPC_NOWAIT) {
			u.u_error = EAGAIN;
			return;
		}
		qp->msg_perm.mode |= MSG_WWAIT; /* wait освобождения ресурса */
		if(sleep(qp, PMSG | PCATCH)) {
			u.u_error = EINTR;
			qp->msg_perm.mode &= ~MSG_WWAIT;
			wakeup(qp);
			return;
		}
		goto getres;
	}
	if(msgfp == NULL) {     /* no free msg structs */
		if(uap->msgflg & IPC_NOWAIT) {
			u.u_error = EAGAIN;
			return;
		}
		if(sleep(&msgfp, PMSG | PCATCH)) {
			u.u_error = EINTR;
			return;
		}
		goto getres;
	}
	if(cnt && (spot = malloc(msgmap, btoq(cnt))) == NULL) {
		if(uap->msgflg & IPC_NOWAIT) {
			u.u_error = EAGAIN;
			return;
		}
		want_msg ++;    /* хотим память */
		if(sleep(msgmap, PMSG | PCATCH)) {
			u.u_error = EINTR;
			return;
		}
		goto getres;
	}

	/* Everything is available, copy in text and put msg on q. */
	if(cnt) {
		u.u_base = (caddr_t)uap->msgp + sizeof(type);
		u.u_segflg = 0;
		MOVE((paddr_t) msg + MSGSSZ * --spot, cnt, B_WRITE);
		if(u.u_error) {
			mmapfree( btoq(cnt), spot + 1);
			return;
		}
	}
	qp->msg_qnum++;
	qp->msg_cbytes += cnt;
	qp->msg_lspid = u.u_procp->p_pid;
	qp->msg_stime = time;
	mp = msgfp;
	msgfp = mp->msg_next;
	mp->msg_next = NULL;
	mp->msg_type = type;
	mp->msg_ts = cnt;
	mp->msg_spot = cnt ? spot : -1;
	if(qp->msg_last == NULL)
		/* очередь была пуста */
		qp->msg_first = qp->msg_last = mp;
	else {
		if( uap->msgflg  & MSG_OOB ){
			/* добавляем в начало очереди */
			mp-> msg_next = qp -> msg_first;
			qp-> msg_first = mp;
		}else{
			/* добавляем в конец очереди */
			qp->msg_last->msg_next = mp;
			qp->msg_last = mp;
		}
	}
	if(qp->msg_perm.mode & MSG_RWAIT) {
		qp->msg_perm.mode &= ~MSG_RWAIT;
		curpri = PMSG;
		wakeup(&qp->msg_qnum);
	}
	u.u_rval1 = 0;
}

/*
**      msgsys - System entry point for msgctl, msgget, msgrcv, and msgsnd
**              system calls.
*/

msgsys()
{
	int             msgget(),
			msgctl(),
			msgrcv(),
			msgsnd();
	static int      (*calls[])() = { msgget, msgctl, msgrcv, msgsnd };
	register struct a {
		unsigned        id;     /* function code id */
		int             *ap;    /* arg pointer for recvmsg */
	}               *uap = (struct a *)u.u_ap;

	if(uap->id > 3) {
		u.u_error = EINVAL;
		return;
	}
	u.u_ap = &u.u_arg[1];
	(*calls[uap->id])();
}

#ifdef pdp11
/* МАШИННО-ЗАВИСИМАЯ ЧАСТЬ ДЛЯ PDP-11 */
#include <sys/seg.h>
#include "../include/buf.h"

/* Переместить n байт из/в пространства процесса по адресу u_base
 * в физическую память по адресу addr .
 * u.u_base, u.u_count, u.u_offset НЕ ИЗМЕНЯЮТСЯ (в отличие от iomove)
 */

io5move( addr, n, flag )
	paddr_t  addr;  /* core byte address */
	register n;     /* byte counter */
{
#ifdef UCB_NET
	extern seg Bmapsave;    /* see machdep.c */
#endif UCB_NET
	/* предполагаем, что всегда u.u_segflg == 0 */

	register caddr_t base = u.u_base; /* address in user space */
	register caddr_t offset;
	caddr_t          paddr;
	register t;

	if( n==0 ) return;

	offset = loint( addr ) & 077;   /* %64 где 64 - размер клика */
	paddr  =(loint( addr ) >> 6 ) & 01777;  /* btoc( addr ) - 1  */
	paddr  |= hiint( addr ) << 10;

	/* для messages offset == 0, т.к. они выравниваются на границу клика */

	if( n > 8192 - (int)offset ){
		/* нельзя копировать больше 1 листа физпамяти */
		u.u_error = EFBIG;
		return;
	}

#ifdef UCB_NET
	saveseg5( Bmapsave );
#endif
	mapseg5( paddr, (((n+MSGSSZ-1)<<2)&0177400) | RW );
	/*
	 * Подключили память машины по адресу addr
	 * в качестве 5-ого листа ядра длиной (n+63)/64 кликов.
	 * Здесь  paddr - кликовый адрес нужной области в памяти ЭВМ.
	 * Второй аргумент - регистр описания страницы:
	 *      левый полубайт - длина в кликах (по 64 байта)
	 *      правый         - коды доступа (биты 1,2 - RW)
	 */
	offset += SEG5; /* адрес в пространстве ядра,
		к которому подключился адрес addr (в 5-ом листе) */

	/* ПЕРЕСЫЛКА */
	if( ((n | (int)offset | (int)base ) & (NBPW-1)) == 0 ){
		/* пословно */
		if( flag == B_WRITE )
			t = copyin ( base, offset , n );
		else    t = copyout( offset, base,  n );
		if( t ) t = -1; /* error */
	}else{
		/* побайтно */
		if( flag == B_WRITE ){
			do{
				t = fubyte( base );
				if( t < 0 ) break;
				*offset++ = t & 0377;
				base ++;
			}while( --n );
		}else{
			do{
				if((t=subyte( base, *offset++ )) < 0 )
					break;
				base++;
			}while( --n );
		}
	}
out:
	/* Вернуться к старому 5-ому листу */
#ifdef UCB_NET
	restoreseg5( Bmapsave );
#else
	normalseg5();
#endif
	if( t<0 ) u.u_error = EFAULT;
}

#endif pdp11
#endif MSGMNI
