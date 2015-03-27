#

/*
**  BUF.H -- buffer definitions
**
**	Version:
**		@(#)buf.h	7.1	2/5/81
*/

# define        BUFSIZE         128
# define        CHAR            short

struct buf
{
	struct buf	*nextb;
	CHAR            buffer[BUFSIZE];
	CHAR            *ptr;
};

# ifndef NULL
# define	NULL	0
# endif
