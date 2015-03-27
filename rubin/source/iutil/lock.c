# include       <local.h>
# include       <stdio.h>
#ifndef NOSOCKET
# include	<sys/types.h>
# include	<sys/socket.h>
# include	<netinet/in.h>
# include	<netdb.h>
# include	<signal.h>
# include	<setjmp.h>
# include	<sccs.h>

SCCSID(@(#)lock.c	1.3	9/12/83)

/*
** start_up_lock_driver
**	Attempt to start up a connection to the lock driver.
**	We connect to a know address (a socket server sits there).
**	If we get a connection on this location, than we are talking
**	to the lock driver. If we timeout, then we assume the driver
**	isn't there.
**
** Returns
**	File descriptor attached to the lock driver
**	-1 on any error.
**
** Trace Flags
**	28
*/
start_up_lock_driver()
{
	struct	sockaddr_in	addr;		/* address to attach to for server */
	register	int	to_driver;	/* we can talk to the lock driver on this one */
	auto		int	hostlen;	/* length of the hostname */
	register	char	*host;		/* name of this host */
	char			hname[BUFSIZ];
	struct		servent	*ing_ser;
	struct		hostent	*myhost;


	/*
	** Find out where the lock driver lives
	*/
	if ( (ing_ser = getservbyname("rubinlock",(char *)0)) == 0
	   && (ing_ser = getservbyname("rubinlock",(char *)0)) == 0 )
	{
			perror("Can't  use lock server: set_up_lock getservbyname");
		return ( -1 );
	}

	/*
	** Make our end of the socket
	*/
	if ( (to_driver = socket(AF_INET,SOCK_STREAM,0)) == -1 )
	{
			perror("Can't use lock server: set_up_lock socket");
		return ( -1 );
	}

	host = hname;
	hostlen = BUFSIZ;
	gethostname(hname,&hostlen);
	if ( (myhost = gethostbyname("localhost")) == 0 && (myhost = gethostbyname(host)) == 0 )
	{
			perror("Can't use lock server: set_up_lock gethostbyname");
		close(to_driver);
		return ( -1 );
	}
	bzero((char *) &addr,sizeof (addr));
	bcopy(myhost->h_addr,(char *)&addr.sin_addr,myhost->h_length);
	addr.sin_family = AF_INET;
	addr.sin_port = ing_ser->s_port;


	/*
	** Connect to the lock_driver
	*/
	if ( connect(to_driver,&addr,sizeof (addr)) == -1 )
	{
			perror("Can't use lock server: set_up_lock connect");
		close(to_driver);
		return ( -1 );
	}


	return ( to_driver );
}/* start_up_lock_driver */


struct servent *
getservbyname(name, proto)
	char *name, *proto;
{
	register struct servent *p;
	register char **cp;

	setservent(0);
	while (p = getservent()) {
		if (strcmp(name, p->s_name) == 0)
			goto gotname;
		for (cp = p->s_aliases; *cp; cp++)
			if (strcmp(name, *cp) == 0)
				goto gotname;
		continue;
gotname:
		if (proto == 0 || strcmp(p->s_proto, proto) == 0)
			break;
	}
	endservent();
	return (p);
}

#else NOSOCKET
#ifndef FCNTL
start_up_lock_driver()
{
	register int i;
	i = open("/dev/lock",2);
	if(i<0) perror("Warning: shared work not possible!!!\nLock driver `/dev/lock':");
	return(i);
}
#else
#include <lock.h>

/*
 * Принципы блокировки в SYSTEM_V
 *
 * 1. Для блокировок типа T_DB, T_CS, T_REL используется блокировка
 *    записей в файле Lockf
 *    Тип      Нач. адрес   Длина
 *    T_DB     0             1
 *    T_CS     CSSTART(tid)  1
 *    T_REL    RLSTART(tid)  1
 *
 *    CSSTART = 01000
 *    RLSTART = 010000
 *    Для страничной блокировки используется блокировка искомого
 *    листа искомого файла
 */
start_up_lock_driver()
{
	extern char *Dbpath;
	register int i;
	       i = open(ztack(Dbpath,"/Lockf"),O_RDWR|O_CREAT,0666);
	if(i<0) perror("Warning: shared work not possible!!!\n'Lockf'");
	return(i);
}

/*
 * set_lock(int fd, int mode,int  op,long  shift,int! len)
 * mode = F_SETLK, F_SETLKW - спать / не спать
 * op = F_RDLCK, F_WRLCK, F_UNLCK - операция
 * fd, shift, len - параметры
 */
set_lock(fd, mode, op, shift, len)
int	mode, op;
long	shift;
int	len;
{
    struct flock fl;
    register int	i;
    if (fd < 0) return(1);
    fl.l_type = op;
    fl.l_whence = 0;
    fl.l_start = shift;
    fl.l_len = len;
    i = fcntl(fd, mode, &fl);
    if (i == -1 ) 
        return(-1);
    return(0);
}
#endif
#endif
