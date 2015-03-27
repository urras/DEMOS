#include <a.out.h>
#include <stdio.h>
/*  Программа для конвертирования об'ектного файла, сформированного в
	режиме "407", в режим "410".
*/

struct exec  buf;
int fd;

main(ac, av)
int ac; 
char *av[];
{
	if( (fd = open(av[1], 2)) <=0){
		fprintf(stderr, "Can not open %s\n", av[1]) ;
		exit(1);
	}
	if(read(fd, &buf, (sizeof buf)) != (sizeof buf))
	    {
		fprintf(stderr, "Can not read %s\n", av[1]) ; 
		exit(1);
	}
	if (buf.a_magic != A_MAGIC1) {
		fprintf(stderr, "Bad magic %o\n", buf.a_magic ) ;
		exit(1);
	}
	buf.a_magic = A_MAGIC2;
	buf.a_data += (buf.a_text&017777);
	buf.a_text &= ~(unsigned)017777;
	lseek(fd,0l,0);
	if(write(fd,&buf, (sizeof buf)) != (sizeof buf)) {
		fprintf(stderr, "Write error\n" ) ; 
		exit(1);
	}
	exit(0);
}
