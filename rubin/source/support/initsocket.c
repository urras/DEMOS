# define DEBUG
# include	<stdio.h>
# include	<sys/types.h>
# include	<sys/socket.h>
# include	<sys/ioctl.h>
# include	<netinet/in.h>
# include	<netdb.h>
# include	<signal.h>

static	char	Sccsid[] = "%W% (RUBIN) %G%";

/*
** init_socket
** initilize the socket to the socket server
*/
init_socket()
{
	register	int	from_socket;	/* file descriptor to attach to socket server */
	int	to_ioctl = 1;			/* used in ioctl call */
	struct	sockaddr_in	addr;		/* address where socket server is */
	char	hostname[BUFSIZ];		/* hostname */
	struct	servent		*server;
	struct	hostent		*myhost;
	auto	int		len;
	extern	int		errno;

	if ( (len = fork()) != 0 )
	{
# ifdef	DEBUG
		printf("lock driver becomes %d\n",len);
# endif
		if ( len == -1 )
		{
			perror("rubin lock driver, fork");
			exit(errno);
		}
		exit(0);
	}
	if ( (from_socket = socket(AF_INET,SOCK_STREAM,0)) == -1 )
	{
# ifdef	DEBUG
		perror("INIT_S socket");
# endif
		exit(errno);
	}
	len = BUFSIZ;
	gethostname(hostname,&len);

	if ( (server = getservbyname("rubinlock",(char *)0)) == 0
	   &&(server = getservbyname("ingreslock",(char *)0)) == 0)
		exit(errno);

	if ( (myhost = gethostbyname("localhost")) == 0 && (myhost = gethostbyname(hostname)) == 0 )
		exit(errno);
	bzero((char *) &addr,sizeof (addr));
	bcopy(myhost->h_addr,(char *)&addr.sin_addr,myhost->h_length);
	addr.sin_family = AF_INET;
	addr.sin_port = server->s_port;
	len = sizeof (addr);
	if ( bind(from_socket,&addr,len) == -1 )
	{
# ifdef	DEBUG
		perror("INIT_S bind, assuming driver already running");
# endif
		exit(0);
	}

	if ( listen(from_socket,10) == -1 )
	{
		perror("Ingres lock, can't listen on port");
		exit(errno);
	}
	ioctl(from_socket,FIONBIO,&to_ioctl);
	return ( from_socket );
}/* init_socket */
