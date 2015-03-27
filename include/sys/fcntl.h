#ifndef _sys_fcntl_h_
#       define  _sys_fcntl_h_

#ifndef _sys_types_h_
#include <sys/types.h>
#endif _sys_types_h_

/* flags supplied to access call */
#define FACCESS_EXISTS  0x0     /* does file exist? */
#define FACCESS_EXECUTE 0x1     /* is it executable by caller? */
#define FACCESS_WRITE   0x2     /* is it writable by caller? */
#define FACCESS_READ    0x4     /* is it readable by caller? */
#define F_OK            FACCESS_EXISTS
#define X_OK            FACCESS_EXECUTE
#define W_OK            FACCESS_WRITE
#define R_OK            FACCESS_READ

/* flags supplies to lseek call */
#define FSEEK_ABSOLUTE  0x0     /* absolute offset */
#define FSEEK_RELATIVE  0x1     /* relative to current offset */
#define FSEEK_EOF       0x2     /* relative to end of file */
#define L_SET           FSEEK_ABSOLUTE
#define L_INCR          FSEEK_RELATIVE
#define L_XTND          FSEEK_EOF

/* flags supplied to open call */
#define FATT_RDONLY     0       /* open for reading only */
#define FATT_WRONLY     01      /* open for writing only */
#define FATT_RDWR       02      /* open for reading and writing */
#define FATT_NDELAY     04      /* open no delay i/o */
#define FATT_APPEND     010     /* open only append */
#define FATT_SYNC       020     /* синхронный обмен */
     /* Следующие режимы д.б. в старшем байте слова */
#define FATT_CREAT      0x100   /* open with creat */
#define FATT_TRUNC      0x200   /* open with truncate */
#define FATT_EXCL       0x400   /* exclusive open */
#define O_RDONLY        FATT_RDONLY
#define O_WRONLY        FATT_WRONLY
#define O_RDWR          FATT_RDWR
#define O_NDELAY        FATT_NDELAY
#define O_APPEND        FATT_APPEND
#define O_SYNC          FATT_SYNC
#define O_CREAT         FATT_CREAT
#define O_TRUNC         FATT_TRUNC
#define O_EXCL          FATT_EXCL

/* Здесь были LOCK_XX для flock. Теперь в sys/file.h --P3 */

/* for fcntl sV.2 */

#define FCNTL           'F'
#define FIODUPFD        (('F'<<8)|3)
#define FIOGETFD        (('F'<<8)|4)
#define FIOSETFD        (('F'<<8)|5)
#define FIOGETFL        (('F'<<8)|6)
#define FIOSETFL        (('F'<<8)|7)
#define FIOGETLK        (('F'<<8)|8)
#define FIOSETLK        (('F'<<8)|9)
#define FIOSETLKW       (('F'<<8)|10)
#define F_DUPFD FIODUPFD
#define F_GETFD FIOGETFD
#define F_SETFD FIOSETFD
#define F_GETFL FIOGETFL
#define F_SETFL FIOSETFL
#define F_GETLK FIOGETLK
#define F_SETLK FIOSETLK
#define F_SETLKW FIOSETLKW

#define F_RDLCK    1
#define F_WRLCK    2
#define F_UNLCK    3

struct flock {
	short l_type;    /* Тип блокирования:
			      F_RDLCK - блокирование на чтение
			      F_WRLCK - блокирование на запись
			      F_UNLCK - разблокирование */
	short l_whence;  /* Флаг начала сдвига:
			      L_SET  - от начала файла
			      L_INCR - от текущего положения
			      L_XTND - от конца файла */
	off_t l_start;   /* Сдвиг области от l_whence */
	off_t l_len;     /* Длина области блокирования */
	short l_pid;     /* Номер блокирующего процесса */
};

struct mflock {         /* то, что хранится в памяти о блокировке */
	short m_type;    /* Тип блокирования */
	off_t m_start;   /* Начало области блокирования */
	off_t m_end;     /* Конец области */
	short m_pid;     /* Номер блокирующего процесса */
	struct mflock *m_next;     /* Ссылка для организации списка */
};

#ifdef KERNEL

struct mflock *pfslot;
memaddr        flock_base;
memaddr        flock_size;

#endif /* KERNEL */

#endif  _sys_fcntl_h_
