struct Dialers {
#define Da_reset        D_arg[0]
#define Da_abort        D_arg[1]
#define Da_trans        D_arg[2]
#define Da_dialstart    D_arg[3]
#define Da_dialend      D_arg[4]
#define Da_CHAT         5
	int  Da_numargs;
	char *Da_arg[20];
	char Da_argbfr[300];
};
/*
 *      read and decode a line from dialers file
 *
 *	return code - FAIL at end-of file; 0 otherwise
 */

rddial(fp, dial)
register struct Dialers *dial;
FILE *fp;
{
	register int na;

	if (!cfgets(dial->Da_argbfr, sizeof(dev->Da_argbfr), fp))
		return FAIL;
	na = getargs(dev->Da_argbfr, dev->Da_arg, 20);
	ASSERT(na >= 4, "BAD DIAL ENTRY", dev->Da_argbfr, 0);
	dev->Da_numargs = na;
	return 0;
}

