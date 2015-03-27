/*
 * Постановка значений макропеременных "устройство загрузки"
 * в исходную строку параметров
 *
 *      %n      - hk            имя типа у-ва
 *      %d      - 1             номер накопителя
 *      %p      - a             буква имени раздела
 *      %f      - hk1a          полное имя файловой системы ( == %n%d%p )
 *      %v      - hk1           имя тома ( == %n%d )
 *
 */

#include        <sys/param.h>
#include        <sys/inode.h>
#include        "../saio.h"

extern int bootopts, bootdev, checkword, rbootdev;
extern int fileondev;

editboot(line)
char *line;
{
	int   dev;
	char *devname;
	int   devminor;
	int   devmajor;
	int   partition;
	register char *p, *q, *r;
	static  char   Worka[110];

	/*
	 * Проверить корректность параметров boot
	 */
	if( bootopts != ~checkword ) {
		dev = rbootdev;         /* Real boot device */
	} else {
		dev = bootdev;
	}
	devmajor = (dev>>8) & 0377;
	devminor = dev & 0377;

	/*
	 * Если на устройстве открыт файл, то использовать его
	 * устройство в качестве у-ва загрузки
	 */
	if( fileondev > 0 ) {
		devname  = devsw[iob[fileondev-3].i_ino.i_dev].dv_name;
		devminor = iob[fileondev-3].i_unit;
	}

	/*
	 * Проверить - есть ли %%?
	 */
	p = line;
	while( *p != '\0' ) {
		if( *p++ == '%' )
			goto Cont;
	}
	return(-1);
Cont:

	/*
	 * Искать имя устройства в таблице мажоров
	 */
	if( fileondev <= 0 ) {
		extern struct tdevs { char *devt; int ndevt;} tdev[];
		struct tdevs *tp = tdev;

		while( tp->devt != 0 ) {
			if( tp->ndevt == devmajor )
				goto Found;
			tp++;
		}
		printf( "Unknown device major: %d\n", devmajor );
		return(0);
	    Found:
		devname = tp->devt;
	}

	/*
	 * Для rk, tm и ts and my - миноры считаются через 1, иначе -
	 * младшие три бита миноров - номер раздела.
	 */
	partition = -1;
	if( strcmp(devname, "rk") &&
	    strcmp(devname, "tm") &&
	    strcmp(devname, "ts") &&
	    strcmp(devname, "my") &&
	    fileondev < 0            ) {
		partition = devminor & 07;
		devminor >>= 3;
	}

       /*
	* Для my может быть загрузка с односторонней и двухсторонней
	* дискет (fd0, fd0.d)
	*/
	if( (strcmp(devname, "my") == 0) && (devminor & 0200) ){
		partition = 8;
		devminor &= 0177;
	}

	/*
	 * Собственно подстановка параметров
	 */
	p = line;
	q = Worka;
	while( *p != '\0' ) {
		if( *p != '%' )
			*q++ = *p;
		else switch( *++p ) {
		    case 'n':
			r = devname;
			while( *r ) *q++ = *r++;
			break;

		    case 'd':
			*q++ = devminor + '0';
			break;

		    case 'f':
		    case 'v':
			r = devname;
			while( *r ) *q++ = *r++;
			*q++ = devminor + '0';
			if( *p == 'v' )
				break;

		    case 'p':
			if( partition >= 0 ){
				if( partition != 8 )
					*q++ = 'a' + partition;
				else {
					*q++ = '.';
					*q++ = 'd';
				}
			}
			break;
		    case '%':
		    case '\0':
			*q++ = '%';
			break;
		    default:
			printf( "Unknown format ptr: %%%c\n", *p );
			return(0);
		}
		p++;
	}
	*q = '\0';

	/*
	 * Копирование Worka обратно в line
	 */
	strcpy(line, Worka);
	return(1);
}
