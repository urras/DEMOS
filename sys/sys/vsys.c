/*
 * ��������� ����������� �������
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

char    avlck[MAXP];    /* ������ ��������� ������ */
char    avact[MAXP];    /* ��������: 0 - done, 1 - R, 2 - W */
			/* ����� ���������� - ��������� (0 - ��) */
short   avinf[MAXP];    /* �������������� ����� */
caddr_t avadr[MAXP];    /* ����� �������� */

short   avlock;         /* ������: ���� �������� ������ */
short   avwant;         /* ������� ������� ���������� �� avlock �������� */
short   avwait;         /* �������-������� ���� */

int     avpid;          /* PID ������� */
int     avslot;         /* ���� ������� */
int     avcode;         /* ��� ������� */
caddr_t avargs;         /* ����� ���������� ������� */

#define AVLPRI  26      /* ���������, ����������� ������� */
#define AVHPRI  26 /*1*/ /* ���������, ����������� ������� */

 /*
  * ������-��������� �����
  * ���������:  code    - ��� ����. ������
  *             args    - ���� ����������
  * ���������� �����; ���� < 0, �� ������
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
	 * ���� ������� avlock ������, �����
	 */
	while( avlock ) {
		avwant = 1;
		sleep(&avlock, AVLPRI);
	}
	avlock = 1;

	/*
	 * ����� ��������� ����
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
	 * ���� �������-������� ����, ��������� ���
	 */
	if( avwait ) {
		avwait = 0;
		wakeup(&avwait);
	}

	/*
	 * ������������ � ��������� ������ ��������
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
	avlck[i] = 0;                   /* ���������� ���� */
	u.u_r.r_val1 = avinf[i];        /* ������� ��� �������� */
}

/*
 * �������� � ������� ���������� ������-���������� ������
 * ���������:   sba     - ����� ������ ��� ������ ��������� ���������
 *                      ��������� ���������: 4 �����
 *                          as_code - ��� ������-���������� ������
 *                          as_addr - ����� ����� ����������
 *                          as_slot - ���������� ����� ���������� ��������
 *                          as_pid  - ������������� (�����) - / -
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
	 * ���� ������� avlock ������, �����
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
	 * ����������� ���� ��������� � ���� ������
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
 * ���������� ���������� ��������
 * ���������:   slot    - ��������� ����� ���������� ��������
 *              retc    - ��� ��������
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
 * ������� ����� �� ������ �����.
 * ���������:
 *      slot - ����� ��������
 *      addr - �����
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
 * �������� ����� � ������ �����.
 * ���������:
 *      slot - ����� ��������
 *      addr - �����
 *      word - ���������� �����
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
