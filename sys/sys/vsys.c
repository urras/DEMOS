/*
 * Интерфейс виртуальной системы
 *
 * $Log:	vsys.c,v $
 * Revision 22.1  89/04/12  15:44:25  korotaev
 * "param.h" ==> <sys/param.h>
 * 
 * Revision 22.0  89/03/30  17:10:55  korotaev
 * Begining DEMOS 2.2.
 * 
 * Revision 1.1  88/01/23  16:47:55  korotaev
 * Initial revision
 * 
 */
#include <sys/param.h>
#ifdef VIRTUAL_SYSTEM
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/proc.h>

#define MAXP    30

char    avlck[MAXP];    /* флажки занятости слотов */
char    avact[MAXP];    /* действие: 0 - done, 1 - R, 2 - W */
			/* после выполнения - результат (0 - ОК) */
short   avinf[MAXP];    /* информационное слово */
caddr_t avadr[MAXP];    /* адрес операнда */

short   avlock;         /* флажок: есть активный запрос */
short   avwant;         /* признак наличия ожидающего по avlock процесса */
short   avwait;         /* процесс-система ждет */

int     avpid;          /* PID запроса */
int     avslot;         /* слот запроса */
int     avcode;         /* код запроса */
caddr_t avargs;         /* адрес аргументов запроса */

#define AVLPRI  26      /* приоритет, разрешающий свопинг */
#define AVHPRI  26 /*1*/ /* приоритет, запрещающий свопинг */

 /*
  * Псевдо-системный вызов
  * Аргументы:  code    - код сист. вызова
  *             args    - блок аргументов
  * Возвращает целое; если < 0, то ошибка
  */
avcall()
{
	register struct a {
		short   code;
		caddr_t args;
	} *uap;
	register i;

	uap = (struct a *)u.u_ap;

	/*
	 * Если семафор avlock поднят, ждать
	 */
	while( avlock ) {
		avwant = 1;
		sleep(&avlock, AVLPRI);
	}
	avlock = 1;

	/*
	 * Найти свободный слот
	 */
	for( i = 0; i < MAXP ; i++ )
		if( avlck[i] == 0 )
			goto Found;
	u.u_error = EMFILE;      /* no more slots ! */
	return;
Found:
	avlck[i] = 1;
	avslot = i;
	avpid = u.u_procp->p_pid;
	avcode = uap->code;
	avargs = uap->args;

	/*
	 * Если процесс-система ждет, разбудить его
	 */
	if( avwait ) {
		avwait = 0;
		wakeup(&avwait);
	}

	/*
	 * Остановиться и выполнять разные действия
	 */
Next_IO:
	sleep(&avlck[i], AVHPRI);
	switch(avact[i]) {
	    case 0:     /* DONE */
		break;

	    case 1:     /* READ */
		avact[i] = 0;
		if( fubyte(avadr[i]) == -1 )
			avact[i] = 1;
		else
			avinf[i] = fuword(avadr[i]);
		wakeup(&avact[i]);
		goto Next_IO;

	    case 2:     /* WRITE */
		avact[i] = 0;
		if( suword(avadr[i], 0) < 0 )
			avact[i] = 1;
		else
			suword(avadr[i], avinf[i]);
		wakeup(&avact[i]);
		goto Next_IO;
	}
	avlck[i] = 0;                   /* освободить слот */
	u.u_r.r_val1 = avinf[i];        /* вернуть код возврата */
}

/*
 * Ожидание и выборка параметров псевдо-системного вызова
 * Аргументы:   sba     - адрес памяти для записи структуры состояния
 *                      Структура состояния: 4 слова
 *                          as_code - код псевдо-системного вызова
 *                          as_addr - адрес блока аргументов
 *                          as_slot - внутренний номер ожидающего процесса
 *                          as_pid  - идентификатор (ДЕМОС) - / -
 */
 avaccept()
 {
	struct a {
		caddr_t sba;
	} *uap;
	struct SB {
		short   as_code;
		caddr_t as_addr;
		short   as_slot;
		int     as_pid;
	} sblock;

	uap = (struct a *)u.u_ap;

	/*
	 * Если семафор avlock опущен, ждать
	 */
	if( !avlock ) {
		avwait = 1;
		if( avwant ) {
			avwant = 0;
			wakeup(&avlock);
		}
		sleep(&avwait, AVLPRI);
		avwait = avlock = 0;
	}

	/*
	 * Скопировать блок состояния и блок данных
	 */
	sblock.as_code = avcode;
	sblock.as_pid  = avpid;
	sblock.as_slot = avslot;
	sblock.as_addr = avargs;
	if( copyout((caddr_t)&sblock, uap->sba, sizeof sblock) ) {
		u.u_error = EFAULT;
		return;
	}
}

/*
 * Продолжить исполнение процесса
 * Аргументы:   slot    - внутрений номер ожидающего процесса
 *              retc    - код возврата
 */
avcont()
{
	struct a {
		short   slot;
		short   retc;
	} *uap;
	register s;

	uap = (struct a *)u.u_ap;
	s = uap->slot;
	if( s < 0 || s >= MAXP || !avlck[s] ) {
		u.u_error = EINVAL;
		return;
	}
	avinf[s] = uap->retc;
	avact[s] = 0;   /* Done */
	wakeup(&avlck[s]);
}

/*
 * Выбрать слово из памяти польз.
 * Аргументы:
 *      slot - номер процесса
 *      addr - адрес
 */
avget()
{
	struct a {
		short   slot;
		caddr_t addr;
	} *uap;
	register s;

	uap = (struct a *)u.u_ap;
	s = uap->slot;
	if( s < 0 || s >= MAXP || !avlck[s] ) {
		u.u_error = EINVAL;
		return;
	}
	avadr[s] = uap->addr;
	avact[s] = 1;   /* Read */
	wakeup(&avlck[s]);
	sleep(&avact[s], AVHPRI);
	if(avact[s])
		u.u_error = EFAULT;
	u.u_r.r_val1 = avinf[s];
}

/*
 * Записать слово в память польз.
 * Аргументы:
 *      slot - номер процесса
 *      addr - адрес
 *      word - собственно слово
 */
avput()
{
	struct a {
		short   slot;
		caddr_t addr;
		short   word;
	} *uap;
	register s;

	uap = (struct a *)u.u_ap;
	s = uap->slot;
	if( s < 0 || s >= MAXP || !avlck[s] ) {
		u.u_error = EINVAL;
		return;
	}
	avinf[s] = uap->word;
	avadr[s] = uap->addr;
	avact[s] = 2;   /* Write */
	wakeup(&avlck[s]);
	sleep(&avact[s], AVHPRI);
	if(avact[s])
		u.u_error = EFAULT;
}
#endif VIRTUAL_SYSTEM
