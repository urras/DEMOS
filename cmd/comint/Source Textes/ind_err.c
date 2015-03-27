/* ����� ��������� �� ������ ����
   name: (���������)   +  ���� ������������� ���� ���� = 1 */

/*
 * $Log:        ind_err.c,v $
 * Revision 1.1  89/08/19  17:55:06  rygoff
 * Initial revision
 * 
 *
 */

#include <errno.h>
#include <tty_codes.h>
#include "comint.h"

int ind_err( name, flag )
    char    *name;
    int            flag;
{
    register char *p;

    if( errno != 0 ) {
        dpp(0,ysize-2);  dpo(el);
        dpp(0,ysize-1);  dpo(el);
        dpp(0,ysize-1);
        if( name )  {dps(name);  dps(" - ");}
        dpo(bl);  dps("������: ");
        switch( errno ) {
            case EPERM:   p="�� �����������������"; break;
            case ENOENT:  p="��� ������ ����� ��� ��������"; break;
            case ESRCH:   p="��� ��������"; break;
            case EIO:     p="������ �����/������"; break;
            case ENXIO:   p="��� ������ �-�� ��� ������"; break;
            case E2BIG:   p="������ ���������� ������� ������"; break;
            case EBADF:   p="������������ ����� �����"; break;
            case EAGAIN:  p="������ ��������� �������"; break;
            case ENOMEM:  p="��� ��������� ������"; break;
            case EACCES:  p="��������� ������"; break;
            case ENOTBLK: p="������ ���� ������� �-��"; break;
            case EBUSY:   p="����������� �-�� ������"; break;
            case EEXIST:  p="���� ��� ����������"; break;
            case EXDEV:   p="����� ����� ������������"; break;
            case ENODEV:  p="��� ����������"; break;
            case ENOTDIR: p="�� �������"; break;
            case EISDIR:  p="�������"; break;
            case EINVAL:  p="������������ ��������"; break;
            case ENFILE:  p="������� ������ �����������"; break;
            case EMFILE:  p="������� ����� �������� ������"; break;
            case ENOTTY:  p="�� ��������"; break;
            case ETXTBSY: p="����������� ���� �����"; break;
            case EFBIG:   p="���� ������� �����"; break;
            case ENOSPC:  p="��� ���������� ����� �� ����������"; break;
            case EROFS:   p="�������� ������� ������ ��� ������"; break;
            case EMLINK:  p="������� ����� ������"; break;
            case EFAULT:  p="������������ �����"; break;
            case 1000+1:  p="���������� ������ ��� ���������� ������";
                          break;
            default:      p=conv(errno,5,10,0); break;
        }
        dps(p);
        if( flag ) {dpi();  dpp(0,ysize-1);  dpo(el);}
    }
    return( errno );
}


ind_lerr(n,f)  {errno=1000+n; ind_err((char*)0,f);}
