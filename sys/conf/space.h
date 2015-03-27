#ifndef _sys_space_h_
# define _sys_space_h_

/*
 *      ��� ���������������� � ������������������ ���������
 *      �������� � ���� ���� (���������� ������� ���������� param.c)
 */

#ifdef KERNEL
#include        "h/video.h"
#endif

#if    NVI > 0
#define HZ      64      /* � �����������-85 ����� ������� */
#else
#define HZ      50                 /* Ticks/second of the clock */
#endif NVI

#define TIMEZONE (%TIMEZONE% * 60) /* Minutes westward from Greenwich */
#define DSTFLAG  %DST%             /* Daylight Saving Time applies here */
#define MAXUSERS %MAXUSERS%

#define NBUF    %NBUF%             /* size of buffer cache, must be <=256*/
#define NMOUNT  %NMOUNT%           /* number of mountable file systems */

#ifdef  UCB_CLIST
#   ifdef UNIBUS_MAP
#       define NCLIST   500        /* number of clists, must be <= 512 */
#   else
#       define NCLIST   200        /* number of clists */
#   endif
#else   UCB_CLIST
#       define NCLIST   100        /* number of clists */
#endif  UCB_CLIST

#define NPROC   %NPROC%            /* max number of processes */
#define NTEXT   %NTEXT%            /* max number of pure texts */
#define NINODE  %NINODE%           /* number of in-core inodes */
#define NFILE   %NFILE%            /* number of file structures */
#define NCALL   %NCALL%            /* max simultaneous time callouts */
#define NDISK   3                  /* number of disks to monitor */

#define NFLOCK  %NFLOCK%                /* ���-�� ������ ������� flock */

/* --------- IPC semaphore facility (see sys/sem.h) ----------------- */
#define SEMMNS  %SEMMNS%      /* ����� ��������� � ������� (�������� 20) */
			      /* ���� �� �����, �� ���������� 0 */
#if SEMMNS > 0
#define SEMMAP  (SEMMNS/2+1)  /* worst case */
#define SEMMNI  %SEMMNI%      /* ����� ����� ��������� */
#define SEMMNU  %SEMMNU%      /* ����� ���������, ������� ����� �������
			       * �������� ��������� � ������ UNDO
			       */
#define SEMMSL   13  /* ���������� ���������, ��� �������� ������������
		      * ����� ��������� ��������
		      */
#define SEMOPM  (SEMUME+1) /* SEMOPM �������� ����� SEMUME */
#define SEMUME  %SEMUME%   /* ����. ����� ���������, ������� ����� �����
			    * �� ������ �������� � ������ UNDO
			    */
#define SEMVMX   32000  /* ����. �������� �������� */
#define SEMAEM   255    /* ����. ���������� �������� */
#endif SEMMNS

/* ---------- IPC message facility (see sys/msg.h) --------------------- */
#define MSGMNI  %MSGMNI%  /* ����� �������� ��������� (�������� 2 � ������) */
			  /* 0, ���� ��� �� ����� �������� messages */
#if MSGMNI > 0
/*
 ** local implementation constants
 **   PDP-11  dependent
 */

#define TOTAL    8192
/* ����� ����� ������ ��� ������ ���������, ������� 64 */

/* ������������ ������ ������ ������ ���������. �� pdp11 �� ������
 * ��������� ������� 1 ����� ������
 */
#if (TOTAL/2) > 8192
# define MSGMAX 8192
#else
# define MSGMAX (TOTAL/2)
#endif

#define MSGTQL %MSGTQL%     /* ����� ��������� � ������� */
#define MSGMAP (MSGTQL/3)
	/* ���� ��-�������� ���� �� (��� ������� ������) (MSGTQL/2+1) */
#define MSGSSZ   64
	/* ������ ������ �������� ������.
	 * �� PDP-11 ��� ������ ����� - 64 �����
	 */
#define MSGMNB   TOTAL
	/* ������������ ��������� ����� ������� ���������
	 * � ����� �������. ������� ���� �� (TOTAL/MSGMNI),
	 * �� � ������, ��� ���� ���� ������� ���� �������
	 * - �� �������� ��� ������
	 */

#define MSGSEG (TOTAL/MSGSSZ)
	/* ����� ��������� ������, ���������� ��� ������ */

#endif MSGMNI

/* ---------- UNIBUS MAP ------------------------------------------------- */
#ifndef UNIBUS_MAP
#   define CMAPSIZ (NPROC*2)              /* size of core allocation map */
#   define SMAPSIZ (NPROC*2+(5*NTEXT/10)) /* size of swap allocation map */
#else
#   define CMAPSIZ (NPROC+(8*NTEXT/10)) /* size of core allocation map */
#   define SMAPSIZ (NPROC+(8*NTEXT/10)) /* size of swap allocation map */
#endif

#endif _sys_space_h_

/*
 *      $Log:	space.h,v $
 * Revision 1.5  90/11/12  18:52:58  root
 * ����� ���� ��� �� 1425 � ����������� ��������� include'��.
 * 
 * Revision 1.4  89/05/13  15:32:10  abs
 * ����� ������� ����� ������ �������� � /usr/include/sys
 * 	����� ������ ��������� ����.
 * 
 * Revision 1.3  89/05/11  18:01:23  abs
 * �� msg.h � sem.h ��������� ���������� ����.
 *
 * Revision 1.2  89/05/05  19:52:00  korotaev
 * � ���� ���� �������� MAXUSERS.
 *
 * Revision 1.1  89/04/24  17:03:17  korotaev
 * Initial revision
 *
 */
