#ifndef _sys_sem_h_
# define _sys_sem_h_

# ifndef _sys_ipc_h_
#include <sys/ipc.h>
# endif  _sys_ipc_h_

# ifndef _sys_space_h_
#  ifdef KERNEL
#include "h/space.h"
#  else
#   include <sys/space.h>
#  endif KERNEL
# endif _sys_space_h_

#if SEMMNS > 0

/*
**      IPC Semaphore Facility.
*/

/*
**      Implementation Constants.
*/

#define PSEMN   (PZERO + 3)     /* sleep priority waiting for greater value */
#define PSEMZ   (PZERO + 2)     /* sleep priority waiting for zero */
/* interruptable by signals */

/*      Local size parameters */
/*      if you need no semaphores, define SEMMNS == 0 */

#define SEMUSZ sizeof(struct sem_undo)

/*
** semaphore information structure
*/
struct  seminfo {
	int     semmap,         /* # of entries in semaphore map */
		semmni,         /* # of semaphore identifiers */
		semmns,         /* # of semaphores in system */
		semmnu,         /* # of undo structures in system */
		semmsl,         /* max # of semaphores per id */
		semopm,         /* max # of operations per semop call */
		semume,         /* max # of undo entries per process */
		semusz,         /* size in bytes of sem_undo structure */
		semvmx,         /* semaphore maximum value */
		semaem;         /* adjust on exit max value */
};

/*
**      Permission Definitions.
*/

#define SEM_A   0200    /* alter permission */
#define SEM_R   0400    /* read permission */

/*
**      Semaphore Operation Flags.
*/

#define SEM_UNDO        010000  /* set up adjust on exit entry */

/*
**      Semctl Command Definitions.
*/

#define GETNCNT 3       /* get semncnt */
#define GETPID  4       /* get sempid */
#define GETVAL  5       /* get semval */
#define GETALL  6       /* get all semval's */
#define GETZCNT 7       /* get semzcnt */
#define SETVAL  8       /* set semval */
#define SETALL  9       /* set all semval's */

/*
**      Structure Definitions.
*/

/*
**      There is one semaphore id data structure for each set of semaphores
**              in the system.
*/

struct semid_ds {
	struct ipc_perm sem_perm;       /* operation permission struct */
	struct sem      *sem_base;      /* ptr to first semaphore in set */
	u_short         sem_nsems;      /* # of semaphores in set */
	time_t          sem_otime;      /* last semop time */
	time_t          sem_ctime;      /* last change time */
};

/*
**      There is one semaphore structure for each semaphore in the system.
*/

struct sem {
	u_short  semval;         /* semaphore text map address */
	short    sempid;         /* pid of last operation */
	/* u_short */ char  semncnt;        /* # awaiting semval > cval */
	/* u_short */ char  semzcnt;        /* # awaiting semval = 0 */
};

/*
**      There is one undo structure per process in the system.
*/

struct sem_undo {
	struct sem_undo *un_np; /* ptr to next active undo structure */
	short  un_cnt;          /* # of active entries */
	struct undo {
		short   un_aoe; /* adjust on exit values */
		short   un_num; /* semaphore # */
		int     un_id;  /* semid */
	}      un_ent[SEMUME];      /* undo entries (one minimum) */
};

/*
**      User semaphore template for semop system calls.
*/

struct sembuf {
	u_short  sem_num;        /* semaphore # */
	short   sem_op;         /* semaphore operation */
	short   sem_flg;        /* operation flags */
};

#endif SEMMNS

#endif _sys_sem_h_
