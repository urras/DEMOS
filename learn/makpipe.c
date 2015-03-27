#include <stdio.h>
#include "lrnref"
makpipe()
{
register int f[2];

	pipe(f);
	if (fork()==0) {
		close(f[1]);
		close(0);
		dup(f[0]);
		close(f[0]);
		execl ("/bin/sh", "sh", "-i", 0);
		execl ("/usr/bin/sh", "sh", "-i", 0);
		if (flag)  write(2,"Ошибка при исполнении sh\n",28);
		else  write(2,"Exec error\n",11);
	}
	close(f[0]);
	/* для того, чтобы shell смогла все пережевать */
	sleep(2);
	return(f[1]);
}
