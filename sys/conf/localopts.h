#ifndef _sys_localopts_h_
#       define  _sys_localopts_h_
/*
 * ��������� ��������� ������� ( ������������ ����� whoami.h).
 * ������� ��������� ����������� � ������������:
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
 * ������������� ��������� ������ �� �������������
 */
/* #define UCB_LOGIN            /*- ���� ��������� ����� "login" */
/* #define UCB_QUOTAS           /*- �������� ������������ ������ */
/* #define TEXAS_AUTOBAUD       /*- �������������� ������ �������� tty */
/* #define UCB_VHANGUP          /*- ��� ������ user-a ���������� HANGUP */
#define UCB_RENICE              /*++ renice system call */
#define IPK_XOVLY               /*++ ������� 31 ���������������� �������
				 * ������� ��������� MENLO_OVLY (��. ����) */
/* #define VIRTUAL_SYSTEM       /*- @VG hacked VS facility. This is only @VG
				 *  debugging tool for DEMOS 3 project.
				 * Using for other needs aren't allowed. */
/* #define NOKA5                /*-? ������� ������ ��� 0120000 - KA5 �� ������������ ��� ������� � �������� */
/* #define UCB_FRCSWAP          /*-- Force swap on expand/fork */
/* #define UCB_CLIST            /*-  Clists ��������� �� ������� ���� */
/* #define BADSECT              /*-? Bad-sector forwarding */
/* #define DIAGNOSTIC           /*- �������������� �������� ������ ������� */
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
 * ������, ��������� �� ��� (��� �������)
 *      ��� ��� type set in whoami.h
 */
#define  MENLO_KOV
#if     PDP11 == GENERIC
#       define  KERN_NONSEP             /* kernel is not separate I/D */
#else
#   if  PDP11 <= 40 || PDP11 == 60
#       define  NONSEPARATE
#       define  KERN_NONSEP             /* kernel is not separate I/D */
#   endif
#endif

#if PDP11 == 40
#       define NONFP                /* if no floating point unit */
#endif

#if ((((PDP11)==(44))||((PDP11)==(70)))||((PDP11)==(24)))||((PDP11)==(GENERIC))
#       define  UNIBUS_MAP
#endif

/*
 *      �������������� �������� ������  �� ������� �������.
 *      ��� �������������� :splfix script ����� �������� ���, �����
 *      ������ spl6 ������������ spl7 (��. conf/:splfix.profile).
 */
/* #define PROFILE       /*-? �������������� ������� */

/*
 *  UCB_NET - ������� ����������� ���� ������, ������� ����� ���
 *  ����� �����������������, �� ��������!!!
 */
/* #define UCB_NET              /* TCP/IP Kernel */

/*
 *        ��������� ��������� ����� ��������:
 */
