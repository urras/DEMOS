/*
 * ��������� ��������� ������� ( ������������ ����� whoami.h).
 *
 * $Log:	localopts.h,v $
 * Revision 1.11  88/09/30  21:02:06  root
 * SMALL ��-ifdef-�� KERNEL-��
 * 
 * Revision 1.10  88/09/30  17:28:23  root
 * EXMOUNT ��-��������� ������ ����������.
 * 
 * Revision 1.9  88/09/01  14:45:33  korotaev
 * ��������� ���� define �� whoami.h (NONFP).
 * �������� ��������� � GENERIC'� (��������� ��������� ��� ����������).
 * 
 * Revision 1.8  88/06/26  19:17:47  korotaev
 * �������� ��������� ���������.
 * 
 * Revision 1.7  88/06/23  21:48:41  ache
 * �������� 31 ������� � ���� (IPK_XKOV)
 * 
 * Revision 1.6  88/06/13  16:09:03  korotaev
 * �������� IPK_SELECT (from TCP/IP).
 * 
 * Revision 1.5  88/04/02  17:29:37  korotaev
 * ������� ����� ����������� � �������� ��������� ���������.
 * 
 * Revision 1.4  87/12/09  16:49:03  andrew
 * ��� ������� ������ ����� ����� 31 �������, � �� ������ 7
 * 
 * Revision 1.3  87/12/08  12:01:04  root
 * ��������� ����������� ���������� IPK_FLOCK.
 * 
 * Revision 1.2  87/09/23  12:05:32  root
 * �������� IPK_TRUNCATE.
 *
 * Revision 1.1  86/04/19  15:02:28  avg
 * Initial revision
 *
 */

#ifndef _sys_localopts_h_
#       define  _sys_localopts_h_

/*
 *      ������� ��������� ����������� � ������������:
 *      *+ - ������ ��������
 *      *- - ������ ���������
 *      *++ - ��������� �� �������������
 *      *-- - �������� �� �������������
 *      *++? - ��������, ���������� �� ��������
 *      *--? - ���������, ��������� �� ��������
 *      *+? - ��������, ���������� �� �����������
 *      *-? - ���������, ��������� �� �����������
 */

/*
 * ��������� �������. ������, ����������� ����� �����������
 * ��� ������������
 */

/* #define CGL_RTP              /*- �������� 1 ����������������� ������� */
/* #define DISKMON              /*- �������� ���������� �� ������� ��� iostat */
/* #define UCB_GRPMAST          /*- �������������� "������������� ������" */
/* #define UCB_LOGIN            /*- ���� ��������� ����� "login" */
/* #define UCB_QUOTAS           /*- �������� ������������ ������ */
/* #define TEXAS_AUTOBAUD       /*- �������������� ������ �������� tty */
#define UCB_UPRINTF             /*+ ��������� �� ������� ���������� user-� */
/* #define UCB_VHANGUP          /*- ��� ������ user-a ���������� HANGUP */
#define UCB_LOAD                /*+ ���������� �� �������� ������� */
/* #define UCB_METER            /*- ���������� �� �������� */
/* #define  FXMETER             /*- ���������� �� ���������� �������. text */
#define UCB_RENICE              /*+ renice system call */
#define IPK_TRUNCATE            /*++ ������� ��������� ����� [f]truncate */
#define IPK_FLOCK               /*++ ������� ��������� ����� flock */
#define IPK_XOVLY               /*++ ������� 31 ���������������� �������
				 * ������� ��������� MENLO_OVLY (��. ����) */
/* #define IPK_SELECT           /*- ��������� ����� select ��� UCB_NET'� */
/* #define VIRTUAL_SYSTEM       /*- @VG hacked VS facility. This is only @VG
				 *  debugging tool for DEMOS 3 project.
				 * Using for other needs aren't allowed. */

/*
 * ���������� ����� �������
 */

/* #define NOKA5                /*-? ������� ������ ��� 0120000 - KA5 �� ������������ ��� ������� � �������� */
/* #define UCB_FRCSWAP          /*-- Force swap on expand/fork */
#define UCB_BHASH               /*+  ��� ��� ������� � ������� */
/* #define UCB_CLIST            /*-  Clists ��������� �� ������� ���� */
#define UCB_DEVERR              /*+ ����������� ��������� �� ������� ��������� */
#define UCB_ECC                 /*+ ��������� ���������� ������ �� ������ */
/* #define BADSECT              /*-? Bad-sector forwarding */
#define UCB_FSFIX               /*+ ���������� ������� ������ ������� �� ����*/
#define UCB_IHASH               /*+? hashed inode table */
#define UCB_ISRCH               /*+? circular inode search */
#define EXMOUNT                 /*+ ������� mount �������� �� ������ ���� */
/* #define UNFAST               /*++? ������������ ������� ������ ����� (��� �������� �����) */
#ifdef KERNEL
#define SMALL                   /*+ ����� ������� (����� ���-�������) */
#endif KERNEL
#define IPK_SDADDR              /*+ 2-byte ����� �� ����� ��� ����� ������ */
/* #define NOACCI               /*- �� ���������� ����� ��������� � ����� */
/* #define ACCT                 /*- ���������� �� �������� */
/* #define INSECURE             /*- �� ������� setuid, setgid ���� ��� ������ � ���� */
/* #define DIAGNOSTIC           /*- �������������� �������� ������ ������� */
/* #define SCHED                /*- ����� ����������� ��������� ������� (��� ������ ������� >= 1M�) */
/* #define IPK_XKOV             /*- ������� 31 ������� � ����
				* ������� ��������� MENLO_KOV (��. ����) */

/*
 * ������, ��������� �� ��� (��� �������)
 *      ��� ��� type set in whoami.h
 */
#if     PDP11 == GENERIC
#       define  MENLO_KOV
#       define  KERN_NONSEP             /* kernel is not separate I/D */
#else
#   if  PDP11 <= 40 || PDP11 == 60
#       define  MENLO_KOV
#       define  NONSEPARATE
#       define  KERN_NONSEP             /* kernel is not separate I/D */
#   endif
#endif

#if PDP11 == 40
#       define NONFP                /* if no floating point unit */
#endif

#if     PDP11 == 44 || PDP11 == 70 || PDP11 == 24 || PDP11 == GENERIC || defined(ENABLE34)
#       define  UNIBUS_MAP
#endif

/*
 *      �������������� �������� ������  �� ������� �������.
 *      ��� �������������� :splfix script ����� �������� ���, �����
 *      ������ spl6 ������������ spl7 (��. conf/:splfix.profile).
 */
/* #define PROFILE       /*-? �������������� ������� */

/* ���������, ��������� ������� �� �������������,
 * ��� ��� ��������� �������������� ���� ��������
 * ��� �������� � ��������������� ��������
 */

/* #define UCB_PGRP             /*-- ��������� �������� �� ������� */
#define UCB_SCRIPT              /*++ ��� ������� ���.������ ���������� #!shell */
/* #define UCB_SUBM             /*--? "submit", �� ����� ������ ��� UCB_JCL */
#define UCB_SYMLINKS            /*++ "����������" ����� ������ */
#define UCB_AUTOBOOT            /*++ ��������� ����� "reboot" */
/* #define OLDTTY               /*--? ������ ����� ������ � ���������� */
#define UCB_NTTY                /*++? ������ � ����������� �� ������   */
#define MENLO_JCL               /*++? Job Control (�� ���������!!!) */
#define MENLO_OVLY              /*++ ���������������� ������� */
#define VIRUS_VFORK             /*++ vfork system call */
#define UCB_NKB         1       /*++ "n" KB ������ ����� �� ����� (���� /*, �� 512)*/
/* #define DISPLAY              /*-? 11/70 or 45 display routine */
/* #define MPX_FILS             /*--? �������������� ����� */
/*
 *  UCB_NET - ������� ����������� ���� ������, ������� ����� ���
 *  ����� �����������������, �� ��������!!!
 */
/* #define UCB_NET              /* TCP/IP Kernel */

#ifdef SCHED
#       undef   CGL_RTP
#endif SCHED

#ifdef UCB_NET
#       define  IPK_SELECT
#endif UCB_NET

#ifdef KERNEL
#   include "pty.h"
#   if NPTY > 0 && !defined(IPK_SELECT)
#       define  IPK_SELECT
#   endif
#endif KERNEL

#if defined(IPK_XKOV) && !defined(IPK_XOVLY)
#       define MENLO_OVLY
#endif

#endif  _sys_localopts_h_
