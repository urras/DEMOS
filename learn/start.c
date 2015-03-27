#include <stdio.h>
#include <sys/types.h>
#include <sys/dir.h>
#include "lrnref"
#define	ND	64

start(lesson)
register char *lesson;
{
register struct direct dv[ND], *dm, *dp;
register int f, c, n;
	char where [100];

	f = open(".", 0);
	n = read(f, dv, ND*sizeof(*dp));
	n /= sizeof(*dp);
	if (n==ND)  {
		if(flag) fprintf(stderr,"урок слишком длинный\n");
		else fprintf(stderr, "lesson too long\n"); 
	}
	dm = dv+n;
	for(dp=dv; dp<dm; dp++)
		if (dp->d_ino) {
			n = strlen(dp->d_name);
			if (dp->d_name[n-2] == '.' && dp->d_name[n-1] == 'c')
				continue;
			c = dp->d_name[0];
			if ((c>='a' && c<= 'z')|| (c>='A' && c<='Z') || (c>='а' && c<='я') || (c>='А' && c<='Я'))
				unlink(dp->d_name);
		}
	close(f);
	if (ask)
		return;
	sprintf(where, "../../%s/L%s", sname, lesson);
	if (access(where, 04)==0)	/* there is a file */
		return;
	if (flag) fprintf (stderr , "Нет урока %s\n",lesson);
	else fprintf(stderr, "No lesson %s\n",lesson);
	wrapup(1);
}

fcopy(new,old)
register char *new, *old;
{
	char b[BUFSIZ];
register int n, fn, fo;
	fn = creat(new, 0666);
	fo = open(old,0);
	if (fo<0) return;
	if (fn<0) return;
	while ( (n=read(fo, b, BUFSIZ)) > 0)
		write(fn, b, n);
	close(fn);
	close(fo);
}









