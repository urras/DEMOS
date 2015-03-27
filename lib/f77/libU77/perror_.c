/*
char id_perror[] = "@(#)perror_.c	1.2";
 *
 * write a standard error message to the standard error output
 *
 * calling sequence:
 *	call perror(string)
 * where:
 *	string will be written preceeding the standard error message
 */

#include	"../libI77/fiodefs.h"
#include        <ediag.h>

extern char *sys_errlist[];
extern char *sys_rerrlist[];
extern int sys_nerr;
extern char *f_errlist[];
extern char *f_rerrlist[];
extern int f_nerr;
extern unit units[];

perror_(s, len)
char *s; ftnlen len;
{
	unit	*lu;
	char	buf[40];
	char	*mesg = s + len;

	while (len > 0 && *--mesg == ' ')
		len--;
	if (errno >=0 && errno < sys_nerr)
		mesg = ediag(sys_errlist[errno],sys_rerrlist[errno]);
	else if (errno >= F_ER && errno < (F_ER + f_nerr))
		mesg = ediag(f_errlist[errno - F_ER],f_errlist[errno - F_ER]);
	else
	{
		sprintf(buf, ediag("%d: unknown error number","%d: неизвестная ошибка"), errno);
		mesg = buf;
	}
	lu = &units[STDERR];
	if (!lu->uwrt)
		nowwriting(lu);
	while (len-- > 0)
		putc(*s++, lu->ufd);
	fprintf(lu->ufd, ": %s\n", mesg);
}
