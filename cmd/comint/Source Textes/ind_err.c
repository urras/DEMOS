/* ПОКАЗ СООБЩЕНИЯ ОБ ОШИБКЕ ВИДА
   name: (сообщение)   +  ждет прикосновения если флаг = 1 */

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
        dpo(bl);  dps("ошибка: ");
        switch( errno ) {
            case EPERM:   p="не суперпользователь"; break;
            case ENOENT:  p="нет такого файла или каталога"; break;
            case ESRCH:   p="нет процесса"; break;
            case EIO:     p="ошибка ввода/вывода"; break;
            case ENXIO:   p="нет такого у-ва или адреса"; break;
            case E2BIG:   p="список аргументов слишком длинен"; break;
            case EBADF:   p="неправильный номер файла"; break;
            case EAGAIN:  p="нельзя запустить процесс"; break;
            case ENOMEM:  p="нет свободной памяти"; break;
            case EACCES:  p="нарушение защиты"; break;
            case ENOTBLK: p="должно быть блочное у-во"; break;
            case EBUSY:   p="монтируемое у-во занято"; break;
            case EEXIST:  p="файл уже существует"; break;
            case EXDEV:   p="связь между устройствами"; break;
            case ENODEV:  p="нет устройства"; break;
            case ENOTDIR: p="не каталог"; break;
            case EISDIR:  p="каталог"; break;
            case EINVAL:  p="неправильный аргумент"; break;
            case ENFILE:  p="таблица файлов переполнена"; break;
            case EMFILE:  p="слишком много открытых файлов"; break;
            case ENOTTY:  p="не терминал"; break;
            case ETXTBSY: p="разделяемый файл занят"; break;
            case EFBIG:   p="файл слишком велик"; break;
            case ENOSPC:  p="нет свободного места на устройстве"; break;
            case EROFS:   p="файловая система только для чтения"; break;
            case EMLINK:  p="слишком много связей"; break;
            case EFAULT:  p="неправильный адрес"; break;
            case 1000+1:  p="неизвестно полное имя помеченных файлов";
                          break;
            default:      p=conv(errno,5,10,0); break;
        }
        dps(p);
        if( flag ) {dpi();  dpp(0,ysize-1);  dpo(el);}
    }
    return( errno );
}


ind_lerr(n,f)  {errno=1000+n; ind_err((char*)0,f);}
