/*
 * $Log
 * Системные вызовы из 4.2 BSD
 * а также разные процедуры работы с каталогом.
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/inode.h>

#include <sys/user.h>
#include "../include/quota.h"


#define ENTRYSIZ sizeof( struct direct )

/* Сделано наоборот  по сравнению с 4.2 BSD
 * Соответствие следующее:
 *    Демос           4.2
 *     prele         iunlock
 *     irele         irele
 */

irele( ip ) struct inode *ip;
{
	if( ip->i_count <= 1 ){        /* ==1  only */
#ifdef DIAGNOSTIC
		printf("irele %d\n", ip->i_count );  /* нужен iput */
#endif
		iput( ip );       /* and unlock ip */
	}else{
		ip->i_count--;    /* and keep it locked */
	}
}

/* Записать информацию про I-узел ip в текущий вход каталога,
 * указываемого I-узлом dp.
 * Место в каталоге dp должно быть установлено заранее
 * при помощи вызова namei()
 */
dirrewrite( dp, ip )
register struct inode *ip, *dp;
{
#ifdef IPK_DIRECTORY
	register unsigned type;

	switch(ip->i_mode & IFMT) {
	    case IFDIR:
		type = DIR_IFDIR;
		break;
	    case IFLNK:
		type = DIR_IFLNK;
		break;
	    default:
		type = DIR_IFREG;
		break;
	}
	u.u_dent.d_class = type;
#endif IPK_DIRECTORY

	u.u_dent.d_ino = ip->i_number;
	u.u_count = ENTRYSIZ;
	u.u_segflg = 1;
	u.u_base = (caddr_t)&u.u_dent;  /* ЧТО записать */
	writei( dp );

#ifdef  UCB_QUOTAS
	/*
	 * Copy quota for new file
	 */
	if (!u.u_error)
		qcopy(dp, ip);
#endif
	iput(dp);
}

/*
 * Write a directory entry with
 * parameters left as side effects
 * to a call to namei.
 */
wdir(ip)
register struct inode *ip;
{
	if (u.u_pdir->i_nlink <= 0) {
		u.u_error = ENOTDIR;
		iput( u.u_pdir);
		return;
	}

	bcopy((caddr_t)u.u_dbuf, (caddr_t)u.u_dent.d_name, DIRSIZ);
	/* информация для записи */
	dirrewrite( u.u_pdir, ip );
}

/* Затереть текущий вход каталога (внутренний unlink) */
dirremove( ip ) register struct inode *ip;
{
	u.u_segflg = 1;                     /* из адресного пространства ядра */
	u.u_offset -= ENTRYSIZ;     /* смещение от начала катлога */
	u.u_base = (caddr_t)&u.u_dent;      /* адрес обмена */
	u.u_count = ENTRYSIZ;       /* сколько байт переслать */
	u.u_dent.d_ino = 0;
#ifdef IPK_DIRECTORY
	u.u_dent.d_class = 0;
	/* именно ноль ! Чтобы int, занимаемый под ino и class был нуль */
#endif
	writei(ip);
}

/* проверить, пуст ли каталог */
dirempty( ip )
struct inode *ip;
{
	off_t offset;
	int reply = 1;  /* пуст */
	struct direct d;
	off_t off;

	offset = u.u_offset;    /* save current offset */

	for( off = (off_t) 0; off < ip-> i_size ; off += ENTRYSIZ ){
		/* read next directory entry */

		/* set new parameters */
		u.u_offset = off;
		u.u_base = (caddr_t) &d;
		u.u_count = ENTRYSIZ;
		u.u_segflg = 1;         /* kernel mode */

		readi( ip );
		/* readi уменьшает count,
		 * увеличивает base и offset
		 */
		if( u.u_error ){
			reply = 0;
			break;
		}
		if( d.d_ino == (ino_t)0 )
			continue; /* free slot */

		/* В пустом каталоге есть только имена
		 * "." и ".."
		 */

		/* имя не с точки - явно лишнее */
		if( d.d_name [0] != '.' ){
			reply = 0;
			break;
		}
		/* имя с точки, но не . или .. */
		if( d.d_name[1] == '\0' )continue;  /* имя "." */
		if( d.d_name[1] != '.' || d.d_name[2] != '\0' ){
			/* .нечто (но не ..) */
			reply = 0;
			break;
		}
	}

	u.u_offset = offset ;   /* restore */
	return reply;
}

/* макет пустого каталога */
#ifdef NO_BRAIN_DAMAGE
struct direct emptydir = {
	0,              /* i-node */
# ifdef IPK_DIRECTORY
	DIR_IFDIR,
# endif
	".."            /* name   */
};
#else
struct direct emptydir;
#endif NO_BRAIN_DAMAGE

/*
 * Mkdir system call
 */
mkdir()
{
	struct a {
		char    *name;
		int     dmode;
	} *uap;
	register struct inode *ip, *dp;
	struct direct empd[2];  /* макет пустого каталога */
	off_t offset;           /* для сохранения */

	uap = (struct a *)u.u_ap;
	ip = namei(uchar, CREATE, 0);
	if (u.u_error)
		return;
	if (ip != NULL) {
		iput(ip);
		u.u_error = EEXIST;
		return;
	}
	dp = u.u_pdir;
	uap->dmode &= 0777;
	uap->dmode |= IFDIR;
	/*
	 * Must simulate part of maknode here
	 * in order to acquire the inode, but
	 * not have it entered in the parent
	 * directory.  The entry is made later
	 * after writing "." and ".." entries out.
	 */
	ip = ialloc(dp->i_dev);
	if (ip == NULL) {
		iput(dp);
		return;
	}
#ifdef QUOTA
	if (ip->i_dquot != NODQUOT)
		panic("mkdir: dquot");
#endif
	ip->i_flag |= IACC|IUPD|ICHG;
	ip->i_mode = uap->dmode & ~u.u_cmask;
	ip->i_nlink = 2;
	ip->i_uid = u.u_uid;
	ip->i_gid = dp->i_gid;
#ifdef QUOTA
	ip->i_dquot = inoquota(ip);
#endif
	iupdat(ip, &time, &time, 1);

	/*
	 * Bump link count in parent directory
	 * to reflect work done below.  Should
	 * be done before reference is created
	 * so reparation is possible if we crash.
	 */
	dp->i_nlink++;
	dp->i_flag |= ICHG;
	iupdat(dp, &time, &time, 1);

	/*
	 * Initialize directory with "."
	 * and ".." from static template.
	 */

	/* три строчки, оказавшиеся здесь из-за тупости Ричевского
	 * компилятора, который не умеет инициализировать поля структур
	 */
	emptydir.d_name[0] = '.';
	emptydir.d_name[1] = '.';
#ifdef IPK_DIRECTORY
	emptydir.d_class = DIR_IFDIR;
#endif

	empd[0] = emptydir;
	empd[1] = emptydir;             /* ".." */
	empd[0].d_name[1] = '\0';       /* "."  */

	empd[0].d_ino = ip-> i_number;
	empd[1].d_ino = dp-> i_number;

	offset = u.u_offset;    /* save */

	/* new write */
	u.u_base = ( caddr_t) empd;
	u.u_offset = ( off_t) 0;
	u.u_segflg = 1;
	u.u_count = sizeof( empd );

	writei( ip );

	if (u.u_error) {
		dp->i_nlink--;
		dp->i_flag |= ICHG;
		goto bad;
	}
	/*
	 * Directory all set up, now
	 * install the entry for it in
	 * the parent directory.
	 */
	u.u_error = 0;
	u.u_offset = offset;    /* restore */
	/* прочие параметры принудительно выставляет wdir */
	wdir( ip );

	dp = NULL;
	if (u.u_error) {
		u.u_dirp = uap->name;
		dp = namei(uchar, LOOKUP, 0);
		if (dp) {
			dp->i_nlink--;
			dp->i_flag |= ICHG;
		}
	}
bad:
	/*
	 * No need to do an explicit itrunc here,
	 * prele will do this for us because we set
	 * the link count to 0.
	 */
	if (u.u_error) {
		ip->i_nlink = 0;
		ip->i_flag |= ICHG;
	}
	if (dp)
		iput(dp);
	iput(ip);
}


/*
 * Rmdir system call.
 */
rmdir()
{
	struct a {
		char    *name;
	};
	register struct inode *ip, *dp;

	ip = namei(uchar, DELETE | LOCKPARENT, 0);
	if (ip == NULL)
		return;
	dp = u.u_pdir;
	/*
	 * No rmdir "." please.
	 */
	if (dp == ip ) {
		/* here dp is locked and dp->i_count >= 2 */
		irele(dp);      /* see above */
		iput(ip);
		u.u_error = EINVAL;
		return;
	}
	/* do not remove current directory */
	if ( ip == u.u_cdir) {
		iput(dp);   /*   ..    */
		iput(ip);   /*    .    */
		u.u_error = EINVAL;
		return;
	}
	if ((ip->i_mode&IFMT) != IFDIR) {
		u.u_error = ENOTDIR;
		goto out;
	}
	/*
	 * Don't remove a mounted on directory.
	 */
	if (ip->i_dev != dp->i_dev) {
		u.u_error = EBUSY;
		goto out;
	}
	/*
	 * Verify the directory is empty (and valid).
	 * (Rmdir ".." won't be valid since
	 *  ".." will contain a reference to
	 *  the current directory and thus be
	 *  non-empty.)
	 */
	if (ip->i_nlink != 2 || !dirempty(ip)) {
		u.u_error = ENOTEMPTY;
		goto out;
	}
	/*
	 * Delete reference to directory before purging
	 * inode.  If we crash in between, the directory
	 * will be reattached to lost+found,
	 */
	dirremove(dp);
	if( u.u_error )
		goto out;

	dp->i_nlink--;
	dp->i_flag |= ICHG;
	iput(dp);
	dp = NULL;

	/*
	 * Truncate inode.  The only stuff left
	 * in the directory is "." and "..".  The
	 * "." reference is inconsequential since
	 * we're quashing it.  The ".." reference
	 * has already been adjusted above.  We've
	 * removed the "." reference and the reference
	 * in the parent directory, but there may be
	 * other hard links so decrement by 2 and
	 * worry about them later.
	 */

	ip->i_nlink -= 2;       /* := 0 */
	itrunc(ip, (size_t)0);

	if( ip-> i_nlink )
	    printf( "rmdir:I=%d ln=%d\n", ip-> i_number, ip->i_nlink );
out:
	if (dp)
		iput(dp);
	iput(ip);
}

#ifdef UNUSED
/* проверить, содержится ли каталог source в пути к каталогу target */
/* target - locked, source - unlocked */
/* target is always iput before returning */

checkpath( source, target )
	struct inode *source, *target;
{
	struct direct dots[2];  /*  .    and   ..    */
	register struct inode *ip;
	int error = 0;

	ip = target;    /* дно пути поиска */
	if( ip-> i_number == source -> i_number ){
		/* source == target */
		error = EEXIST;
		goto out;
	}
	if( ip->i_number == ROOTINO ) goto out;

	/* цикл подьема вверх до корня диска */
	for(;;){
		if( (ip->i_mode & IFMT ) != IFDIR ){
			error = ENOTDIR;
			break;
		}
		u.u_base = (caddr_t) dots;
		u.u_count = sizeof( dots );
		u.u_segflg = 1;
		u.u_offset = (off_t) 0;
		/* . and .. must be first ones in directory */
		readi( ip );
		if( u.u_error ) break;
		if( bcmp( dots[1].d_name , ".." , 3 )){
			error = ENOTDIR;
			break;
		}
		if( dots[1].d_ino == source -> i_number ){
			error = EINVAL;
			/* да, source  сдержится в пути к target */
			break;
		}
		if( dots[1].d_ino == ROOTINO )
			break;   /* root of disk */
		iput( ip );
		ip = iget( ip-> i_dev, dots[1].d_ino );
		if( ip == NULL ){
			error = u.u_error;
			break;
		}
	} /* end for */
out:
	if( error == ENOTDIR ){
		printf( "checkpath: .. isn't dir\n");
	}
	if( ip != NULL )
		iput(ip);
	return error;
}
#endif UNUSED
