/*
 * ���������� ����� �����.
 *
 * $Log:	fcntl.c,v $
 * Revision 22.3  89/05/05  14:07:50  korotaev
 * �������� ����������� ������: ���� �� EINVAL ������� �� EMLINK.
 * 
 * Revision 22.2  89/04/12  15:18:02  korotaev
 * "param.h" ==> <sys/param.h>
 * 
 * Revision 22.0  89/03/30  13:39:42  korotaev
 * Begining DEMOS 2.2.
 * 
 */

#include <sys/param.h>
#include <errno.h>
#include <sys/inode.h>
#include <sys/fcntl.h>
#include <sys/seg.h>
#include <sys/systm.h>
#include <sys/user.h>
#include <sys/file.h>
#include <sys/proc.h>

/*
 * ��������� ����� fcntl �� System V.2 (VAX/UTS) ������ ����� ioctl.
 */
fcntl(fd, fp, cmd, arg)
int fd;
register struct file *fp;
register int    cmd;
caddr_t  arg;
{
	int i;

	u.u_r.r_val1 = 0;
	switch( cmd ){
	       /*
		* dup(), �� �� >= ������������� �����������
		*/
		case FIODUPFD:
			if( (int)arg < 0 || (int)arg > NOFILE ){
				u.u_error = EMFILE;
			} else if ((i = ufalloc((int)arg)) >= 0){
				u.u_ofile[i] = fp;
				fp->f_count++;
				u.u_r.r_val1 = i;
			}
			break;
	       /*
		* ��������� close-on-exec ����� (EXCLOSE)
		*/
		case FIOGETFD:
			u.u_r.r_val1 = u.u_pofile[fd];
			break;
	       /*
		* ���������� close-on-exec �����
		*/
		case FIOSETFD:
			u.u_pofile[fd] = (int)arg;
			break;
	       /*
		* ��������� ������� ��������� �����:
		* O_RDONLY, O_WRONLY, O_RDWR, O_NDELAY, O_APPEND, O_SYNC
		*/
		case FIOGETFL:
			u.u_r.r_val1 = fp->f_flag-1;
			break;
	       /*
		* ���������� ����� ������� ��������� �����, ������:
		*      O_NDELAY, O_APPEND, O_SYNC
		*/
		case FIOSETFL:
			fp->f_flag &= (FREAD|FWRITE);
			fp->f_flag |= ((++((int)arg)) & FMASK) & ~(FREAD|FWRITE);
			break;
	       /*
		* ��������� ������ ����������, ������� ������������
		* � ��������.
		*/
		case FIOGETLK:
	       /*
		* ���������� ��� ��������
		*/
		case FIOSETLK:
	       /*
		* ���������� � ���������
		*/
		case FIOSETLKW:
		{
			struct flock  fl;
			register struct inode  *ip = fp->f_inode;
			off_t  begin, e;
			segm    saveregs;
			int    pidd;

		       /*
			* ����������� ������ ������� ����� � ���������.
			*/
			switch( ip->i_mode & IFMT ){
				case IFREG:
				case IFBLK:
				case IFCHR:
					break;
				default:
					u.u_error = EINVAL;
					return;
			}
			if( copyin((caddr_t)arg,(caddr_t)&fl,sizeof(struct flock)) ){
				u.u_error = EFAULT;
				return;
			}
		       /*
			* �������� ���� ����������.
			*/
			switch(fl.l_type) {
				case F_RDLCK:
					if( (fp->f_flag & FREAD) == 0 )
						u.u_error = EINVAL;
					break;
				case F_WRLCK:
					if( (fp->f_flag & FWRITE) == 0 )
						u.u_error = EINVAL;
					break;
				case F_UNLCK:
					break;
				default:
					u.u_error = EINVAL;
					break;
			}
			if( u.u_error )
				return;
		       /*
			* ������ �������.
			*/
			switch(fl.l_whence) {
				case FSEEK_ABSOLUTE:
					begin = 0;
					break;
				case FSEEK_RELATIVE:
					begin = fp->f_un.f_offset;
					break;
				case FSEEK_EOF:
					begin = ip->i_size;
					break;
				default:
					u.u_error = EINVAL;
					return;
			}
			begin += fl.l_start;
			if( fl.l_len < 0 ){
				e = begin;
				begin += fl.l_len;
			} else if( fl.l_len == 0 )
				e = 0;
			else
				e = begin + fl.l_len;
			if( begin < 0 ){
				u.u_error = EINVAL;
				return;
			}
			pidd = (u.u_procp)->p_pid;
			saveseg5(saveregs);
			mapseg5(flock_base, (flock_size << 8) | RW);
			if( cmd == FIOGETLK ){
				getflock(ip,begin,e,&fl);
				if( copyout((caddr_t)&fl,(caddr_t)arg,sizeof(struct flock)) ){
					u.u_error = EFAULT;
					restorseg5(saveregs);
					return;
				}
			} else if( fl.l_type == F_UNLCK )
				unflock(ip,begin,e,pidd);
			else
				setflock(ip,fl.l_type,begin,e,(int)(cmd!=FIOSETLKW),pidd);
			restorseg5(saveregs);
			break;
		}
		default:
			u.u_error = EINVAL;
	}
}

setflock(ip,type,b,e,ndlflg,pidd)
register struct inode *ip;
	 short  type, ndlflg;
	 off_t  b, e;
	 int    pidd;
{
	register struct mflock *mlp, *bmlp;
		 int    need, maxneed;
		 char   flg;

repeat:
	maxneed = need = 0;
	flg = 0;
	for( mlp = ip->i_pflock ; mlp != NULL ; mlp = mlp->m_next ){
		if( e != 0 && e <= mlp->m_start )  /* �.�. ������ ������������ */
			break;
	       /*
		* ���� ���������� ���������� � ����� ������� � ����
		* ���������, �� ����� ���������� ��������� �����.
		*/
		if( mlp->m_end != 0 && b >= mlp->m_end )
			continue;
	       /*
		* ��������� �� ����������� �� ������ ���������,
		* ����� �� ������������� ������ �������.
		*/
		if( mlp->m_pid != pidd ){
			if( type == F_WRLCK || mlp->m_type == F_WRLCK ){
				if( ndlflg ){   /* if ������ ��� �������� */
					u.u_error = EAGAIN;
					return;
				}
				ip->i_flag |= IL1WAIT;
				normalseg5();
				sleep((caddr_t)ip->i_pflock,PLOCK);
				mapseg5(flock_base, (flock_size << 8) | RW);
				goto repeat;
			}
		} else {
	       /*
		* �� ����� �����������: ���� ������ ������� ���
		* �������������, �� ����������� ���-�� ������, �������
		* ������������ ��� ������������ �, ���� ����, ���
		* ��������������� ����� ����������������.
		*/
			if( mlp->m_type == type ){
		       /*
			* ���� ���� ���������, �� ������������ ����������
			* ������������ �����. ���� ����� ������������ �����
			* ����� �������, �� ���������� ���� ������� �� ����,
			* � �.�.
			*/
				if( b > mlp->m_start )
					b = mlp->m_start;
				if( e ){
					if( mlp->m_end == 0 )
						e = 0;
					else if( e < mlp->m_end )
						e = mlp->m_end;
				}
				need--;
			} else {
		       /*
			* ���� ���� ������, �� �������� ��� ������:
			* 1) �� ����� ������ �����. ��� ������ ���������
			*    ����� ������� �� ������ �������� �������������
			*    ������� � ������������� ����� ����� ��� ������.
			* 2) ����� ���� ����� ����. ��� ��������� ���������
			*    ����������.
			* 3) ����� ��� ����� �����. ��� ������ ���������
			*    ������ ������� �� �����. � ���� ������ ���� ����
			*    ����� ��� ��������������� ����������� �����
			*    ������ ������� � ���� ���� ��� ����� ����������.
			*/
				if( b <= mlp->m_start && (e == 0 || (mlp->m_end != 0 && e >= mlp->m_end)) )
					need--;
				else if( b >= mlp->m_start && (e != 0 && (mlp->m_end || e <= mlp->m_end)) )
					need += 2;
			}
			flg++;
			if( need > maxneed )
				maxneed = need;
		}
	}
       /*
	* ���� ��� ����� � ������� ����������, �� ������� �����. ������.
	*/
	if( nfslot < maxneed || nfslot - need < 1 ){
		u.u_error = EMLINK;
		return;
	}
	if( flg ){
		unflock(ip,b,e,pidd);
		if( u.u_error )
			return;
	}
	bmlp = NULL;
	for( mlp = ip->i_pflock ; mlp != NULL ; mlp = mlp->m_next ){
		if( mlp->m_start > b )
			break;
		bmlp = mlp;
	}
	if( nfslot < 1 ){
		u.u_error = EMLINK;
		return;
	}
	nfslot--;
	if( bmlp == NULL ){
		ip->i_pflock = pfslot;
		pfslot = pfslot->m_next;
		(ip->i_pflock)->m_next = mlp;
		mlp = ip->i_pflock;
	} else {
		bmlp->m_next = pfslot;
		bmlp = pfslot;
		pfslot = pfslot->m_next;
		bmlp->m_next = mlp;
		mlp = bmlp;
	}
	mlp->m_type = type;
	mlp->m_start = b;
	mlp->m_end = e;
	mlp->m_pid = pidd;
}

unflock(ip,b,e,pidd)
register struct inode *ip;
off_t  b, e;
int pidd;
{
	register struct mflock *mlp, *bmlp;
		 off_t          e1;

	bmlp = NULL;
	mlp = ip->i_pflock;
	while( mlp != NULL ){
		if( e != 0 && e <= mlp->m_start )  /* �.�. ������ ������������ */
			break;
		if( (mlp->m_end == 0 || b < mlp->m_end) && mlp->m_pid == pidd ){
			if( b <= mlp->m_start ){
				if( e == 0 || (mlp->m_end != 0 && e >= mlp->m_end) ){
				/* unlock ���� ������� */
					nfslot++;
					if( bmlp == NULL )
						ip->i_pflock = mlp->m_next;
					else
						bmlp->m_next = mlp->m_next;
					mlp->m_next = pfslot;
					pfslot = mlp;
					if( bmlp == NULL ){
						mlp = ip->i_pflock;
						continue;
					}
					mlp = bmlp;
				} else          /* unlock ����� ������ ������� */
					mlp->m_start = e;
			} else if( e == 0 || (mlp->m_end != 0 && e >= mlp->m_end) ){
			/* unlock ����� ����� ������� */
				mlp->m_end = b;
			} else {        /* unlock � ������ ������� */
				struct mflock *wmlp;

				if( nfslot < 1 ){       /* ��� ����� � ������� */
					u.u_error = EMLINK;
					return;
				}
			       /*
				* ������ ������� ���������� �� ������ �������
				* ����� ��������� ����� �������.
				*/
				e1 = mlp->m_end;
				mlp->m_end = b;
			       /*
				* ������ ����� ���� ���������� � ����� �����.
				* �.�. ������ ������������ �� ������� ��������,
				* �� ��� ���� ���������� �� ������� ����������.
				*/
				nfslot--;
				pfslot->m_type = mlp->m_type;
				pfslot->m_start = e + 1;
				pfslot->m_end = e1;
				pfslot->m_pid = mlp->m_pid;
				wmlp = mlp;
				bmlp = mlp;
				mlp = mlp->m_next;
				while( mlp != NULL ){
					if( mlp->m_start > e )
						break;
					bmlp = mlp;
					mlp = mlp->m_next;
				}
				bmlp->m_next = pfslot;
				bmlp = bmlp->m_next;
				pfslot = pfslot->m_next;
				bmlp->m_next = mlp;
				mlp = wmlp;
			}
		}
		bmlp = mlp;
		mlp = mlp->m_next;
	}
	if( ip->i_flag & IL1WAIT ){
		ip->i_flag &= ~IL1WAIT;
		normalseg5();
		wakeup((caddr_t)ip->i_pflock);
		mapseg5(flock_base, (flock_size << 8) | RW);
	}
}

getflock(ip,b,e,flp)
register struct inode *ip;
off_t b, e;
register struct flock *flp;
{
	register struct mflock *mlp;

	for( mlp = ip->i_pflock ; mlp != NULL ; mlp = mlp->m_next ){
		if( e != 0 && e < mlp->m_start )  /* �.�. ������ ������������ */
			break;
		if( mlp->m_end != 0 && b >= mlp->m_end )
			continue;
		flp->l_type = mlp->m_type;
		flp->l_whence = FSEEK_ABSOLUTE;
		flp->l_start = mlp->m_start;
		flp->l_len = (mlp->m_end) ? mlp->m_end - mlp->m_start : 0;
		flp->l_pid = mlp->m_pid;
		return;
	}
	flp->l_type = F_UNLCK;
}
