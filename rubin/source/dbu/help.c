# include	<pv.h>
# include	<defines.h>
# include	<aux.h>
# include	<catalog.h>
# include	<access.h>
# include	<func.h>
# include	<signal.h>
# include	<sccs.h>

SCCSID(@(#)help.c	7.4	9/26/83)

extern short 	tTdbu[100];
extern int	help();
extern int	null_fn();

struct fn_def HelpFn =
{
	"HELP",
	help,
	null_fn,		/* initialization function */
	null_fn,
	NULL,
	0,
	tTdbu,
	100,
	'Z',
	0
};


/*
**  HELP - Provide Information to User
**
**	Arguments:
**		pv[i] - code
**			0 - print relation information
**			1 - print manual section
**			2 - print relation list
**			3 - print relation info for all accessible relations
**		pv[i+1] - name of entity for modes 0 or 1
**              KEY/WORDS:
**      short - short out, my - only MY relations, real - only REGULAR
**      full  - full information
**
**	Trace Flags:
**		44
*/

short h_flags;
#define H_SHORT 001
#define H_FULL  002
#define H_MY    004
#define H_REAL  010

char *h_keys[] =  { "short", "full", "my", "real", NULL};
static chkflags(), pts2();
#define LLINE 132
static short lline = 256;

help(parmc, parmv)
int	parmc;
PARM	parmv[];
{
	DESC		des;
	int		mode;
	register PARM	*pv;
	register int	ret;
	short  mode1 = 2;
	h_flags = 0;
	lline =80;
# ifdef	xZTR1
	if (tTf(44, -1))
	{
		printf(">>help\n");
		if (tTf(44, 0))
			prvect(parmc, parmv);
	}
# endif

	ret = 0;
	pv = parmv;
	getuser(-1);	/* init getuser for modes 1 & 2 */
	while (pv->pv_type != PV_EOF)
	{
		mode = atoi((pv++)->pv_val.pv_str);
		if (mode < 2 && pv->pv_type == PV_EOF)
			syserr("help: mode %d no val", mode);

#		ifdef xZTR1
		if (tTf(44, -1))
		{
			printf("help %d", mode);
			if (mode != 2)
				printf(" %s", pv->pv_val.pv_str);
			putchar('\n');
		}
#		endif
		switch (mode)
		{

		  case 0:	/* help relation */
			if (!openr(&des, -6, pv->pv_val.pv_str))
			{
				mode1 = 0;
				rel_fmt(&des);
				pv->pv_val.pv_str = NULL;
			}
			else if(chkflags(pv->pv_val.pv_str))
				pv->pv_val.pv_str = NULL;
			else mode1 = 0;
			pv++;
			break;

		  case 1:	/* help manual section */
			if(chkflags(pv->pv_val.pv_str))
				pv->pv_val.pv_str = NULL;
			else if (man(pv->pv_val.pv_str))
				{
				pv->pv_val.pv_str = NULL;
				mode1 = 0;
				}
			else mode1 = 0;
			pv++;
			break;

		  case 2:
		  case 3:
			mode1 = mode;
			break;

		  default:
			syserr("HELP: mode %d", mode);
		}
	}
	if (mode1)
		relpr(mode1);
	getuser(0);	/* close getuser in case mode 1 or 2 */
	/* now rescan for error messages */
	pv = parmv;
	while (pv->pv_type != PV_EOF)
	{
		mode = atoi((pv++)->pv_val.pv_str);
		if (mode < 2)
		{
			if (pv->pv_val.pv_str != NULL)
				ret = nferror(5401 + mode, pv->pv_val.pv_str, 0);
			pv++;
		}
	}
	return (ret);
}


/*
**  Nroff Manual Section
**
**	The manual section given by 'name' is nroff'ed.  Returns one
**	on success, zero if the manual section is not found.
**
**	Uses trace flag 11
*/

int
man(name)
char	*name;
{
	char		manual[100];
	register int	i;
	int		stat;
	char		name_nr[18];
	register char	*naa;
	extern char	*ztack();

	if (length(name) > 14)
		return (0);

	/* a null manual name gives table of contents */
	if (name[0] == 0)
		smove("../toc.nr", name_nr);
	else
		concat(name, ".nr", name_nr);

	concat(ztack(Pathname, "/doc/quel/"), name_nr, manual);
	if ((i = open(manual, 0)) < 0)
	{
		/* try a demos command instead */
		concat(ztack(Pathname, "/doc/cmds/"), name_nr, manual);
		if ((i = open(manual, 0)) < 0)
			return (0);
	}
	if (close(i))
		syserr("cannot close %s", manual);
	ruboff(0);	/* wait for child's death if rubout occures */
	i = fork();
	if (i == 0)
	{
		signal(SIGINT, SIG_DFL);	/* die on rubout */
		setuid(getuid());
#		ifndef xB_UNIX
		setgid(getgid());
#		endif
		naa = ztack(Pathname, "/doc/iaa");
		execl("/bin/nroff", "nroff", naa, manual, 0);
		execl("/usr/bin/nroff", "nroff", naa, manual, 0);
		syserr("help: exec: nroff");
	}
	/* wait for nroff if fork succeeded */
	if (i > 0)
		fullwait(i, "help: nroff");
	rubon();
	return (1);
}


/*
**  PRINT DATABASE INFORMATION
**
**	Prints a list of all the relations in the database, together
**	with their owner.
**
**	Uses trace flag 12
*/

relpr(mode)
int	mode;
{
	extern DESC	Reldes;
	register DESC	*d;
	register int j;
	int                    i;
	register char			*cp;
	struct tup_id			limtid, tid;
	char				buf[MAXLINE + 1];
	char				lastuser[2];
	struct relation			rel;
	short s;
	j = 0;
	opencatalog("relation", 0);
	d = &Reldes;
	if (i = find(d, NOKEY, &tid, &limtid))
		syserr("help: relpr: find %d", i);
	lastuser[0] = '\0';
	if (mode == 2 )
	{
		if(!(h_flags&H_SHORT))
		printf("\n relation name     relation owner\n\n");
		else
		printf("== /xy, x= I-index, V-view,B-binary. y=* - DBA Rels ==\n");
	}
	while ((i = get(d, &tid, &limtid, &rel, 1)) == 0)
	{
	    s = rel.relstat;
	    if ((h_flags&H_MY) && (!bequal(rel.relowner,Usercode,2)) ||
		(h_flags&H_REAL)&&(s&(S_CATALOG|S_VIEW|S_INDEX|S_SERIAL)) ||
		(h_flags&H_SHORT)&&((s&(S_CATALOG))
				  ||(bequal("_Sys",rel.relid,4)))
	       )
		    continue;
		if (mode == 2 && (h_flags & H_SHORT))
		{
			cp = buf;
			strncpy(cp,rel.relid,12);
			while(cp-buf<12 && *cp!=' ') cp++;
			strcpy(cp,"/");
			if(s&S_SERIAL) strcat(cp,"%");
			if(s&S_VIEW) strcat(cp,"V");
			if(s&S_INDEX) strcat(cp,"I");
			if(s&S_BINARY) strcat(cp,"B");
			if(!bequal(Usercode,rel.relowner,2))
			{
				if(bequal(rel.relowner,Admin.adhdr.adowner,2))
					strcat(cp,"*");
				else continue;
			}
			printf("%-15s%c",buf,++j%5?' ':'\n');
		}
		else if (mode == 2)
		{
			if (!bequal(lastuser, rel.relowner, 2))
			{
				if (getuser(rel.relowner, buf))
				{
					/* cant find user code */
					bmove("  ", buf, 2);
					cp = &buf[2];
					bmove(rel.relowner, cp, 2);
					cp = &cp[2];
					*cp = '\0';
				}
				else
				{
					for (cp = buf; *cp != ':'; cp++)
						;
					*cp = '\0';
				}
				bmove(rel.relowner, lastuser, 2);
			}
		/* Внимание! в этих строках табуляции */
			if (rel.relstat &S_SERIAL)
				printf(" %.12s  %s(var)  =  %ld\n",rel.relid,buf,rel.reltups);
			else
				printf(" %.12s  %s (tups=%ld)\n", rel.relid, buf,rel.reltups);
		}
		else
		{
			if ((rel.relstat & S_CATALOG) || bequal("_SYS", rel.relid, 4))
				continue;
			if (bequal(Usercode, rel.relowner, 2) || bequal(Admin.adhdr.adowner, rel.relowner, 2))
				rel_fmt(&rel);
		}
	}

	if (i < 0)
		syserr("help: relpr: get %d", i);
	if (mode == 2 && j%5)
		printf("\n");
	return (0);
}



/*
**  Print Relation Information
**
**	Prints detailed information regarding the relation.
**
**	Uses trace flag 13
*/

rel_fmt(r)
register struct relation	*r;
{
	struct tup_id		limtid, tid;
	char			buf[MAXLINE + 1];
	struct attribute	att;
	struct index		indkey, ind;
	register int		i;
	int			j;
	extern DESC		Attdes, Inddes;
	char			*trim_relname();
	short s_flag;
#define SF(s,f) (s_flag?s:f)
	s_flag = (h_flags&H_SHORT) != 0;
	if (r->relstat & S_SERIAL)
	{
		printf(" %s (VAR) = %ld\n",trim_relname(r->relid), r->reltups);
		return;
	}
	printf(SF("\042%s\042","\nRelation:\t\t%s\n"), trim_relname(r->relid));
	i = getuser(r->relowner, buf);
	if (i)
	{
		smove("(xx)", buf);
		bmove(r->relowner, &buf[1], 2);
	}
	else
	{
		for (i = 0; buf[i] != ':'; i++)
			continue;
		buf[i] = 0;
	}
	printf(SF("(%s)","Owner:\t\t\t%s\n"), buf);
	printf(SF(" w=%d","Tuple width:\t\t%d\n"), r->relwid);
	if (!s_flag && r->relsave != 0)
	{
		printf("Saved until:\t\t%s", ctime(&r->relsave));
	}
	if ((r->relstat & S_VIEW) == 0)
	{
		printf(SF(" tups=%ld","Number of tuples:\t%ld\n"), r->reltups);
		printf(SF(" /","Storage structure:\t"));
		i = r->relspec;
		if (i < 0)
		{
			printf(SF("C","compressed "));
			i = -i;
		}
		switch (i)
		{
	
		  case M_HEAP:
			printf(SF("HEAP/","paged heap\n"));
			break;
	
		  case M_ISAM:
			printf(SF("ISAM/","ISAM file\n"));
			break;
	
		  case M_HASH:
			printf(SF("HASH/","random hash\n"));
			break;
	
		  default:
			printf(SF("%d/","unknown structure %d\n"), i);
			break;
	
		}
	}
	if(!s_flag)
		printf("Relation type:\t\t");
	if (r->relstat & S_CATALOG)
		printf(SF(" SYS","system catalog\n"));
	else if (r->relstat & S_VIEW)
		printf(SF(" VIEW","view\n"));
	else
		if (r->relindxd < 0)
		{
			printf(SF(" INDEX(","secondary index on "));
			opencatalog("indexes", 0);
			setkey(&Inddes, &indkey, r->relowner, IOWNERP);
			setkey(&Inddes, &indkey, r->relid, IRELIDI);
			if (!getequal(&Inddes, &indkey, &ind, &tid))
				printf(SF("%s)","%s\n"), trim_relname(ind.irelidp));
			else
				printf(SF("?)","unknown relation\n"));
		}
		else
		{
			if (r->relstat & S_DISTRIBUTED)
				printf(SF(" DST","distributed "));
			printf(SF(" USER","user relation"));
		}
	if (r->relindxd > 0)
	{
		printf("\nSecondary Indices:\t");
		opencatalog("indexes", 0);
		setkey(&Inddes, &indkey, r->relid, IRELIDP);
		setkey(&Inddes, &indkey, r->relowner, IOWNERP);
		if (i = find(&Inddes, EXACTKEY, &tid, &limtid, &indkey))
			syserr("help: find %d indexes", i);
		j = FALSE;
		while ((i = get(&Inddes, &tid, &limtid, &ind, 1)) == 0)
		{
			if (!bequal(&indkey, &ind, MAXNAME + 2))
				continue;
			if (j)
				printf(", ");
			j =TRUE;
			printf("%s", trim_relname(ind.irelidi));
		}
		if (i < 0)
			syserr("help:get indexes %d", i);
		if (!j)
			printf("unknown");
	}
	printf("\n");
	opencatalog("attribute", 0);
	if (!s_flag)
		printf("\n attribute name    type  length  keyno.\n\n");
	seq_init(&Attdes, r);
	while (seq_attributes(&Attdes, r, &att))
	{
		if (!s_flag)
		{
			printf(" %.12s      %c%8d",
				att.attname, att.attfrmt, att.attfrml & 0377);
			if (att.attxtra)
				printf("%7d", att.attxtra);
			printf("\n");
		}
		else
		{
			char b0[20];
			if(att.attxtra)
				sprintf(b0,"%c%d/%d ",att.attfrmt,att.attfrml & 0377, att.attxtra);
			else
				sprintf(b0,"%c%d ",att.attfrmt,att.attfrml & 0377);
			att.attname[MAXNAME] = 0;/* !! */
			pts2(att.attname,b0);
		}
	}
	if (!s_flag)
		printf("\n");
	else
		pts2(NULL,NULL);
	return (0);
}

/*
 * chkflags (name)
 * Установка ключа, если name - ключевое слово
 * Ответ: 0 - не ключевое слово, 1 - клюевое слово
 */
static chkflags(name)
register char *name;
{
	register char **p;
	register short flg;
	p = h_keys;
	flg = 01;
	if ( bequal(name,"width",5))
	{
		lline = atoi(name+5);
		if (lline < 20) lline = 20;
		if (lline > LLINE) lline = LLINE;
		return(1);
	}
	while( *p)
	{
		if(sequal(*p,name))
		{
			h_flags |= flg;
			return(1);
		}
		flg <<= 1;
		p++;
	}
	return(0);
}

/*
 * pts2(s0,s1)
 * Печать в 2-х строчной таблицы
 * s0, s1 - очередные элементы
 * s0 = NULL - сброс
 * Возвращается число реально напечатанных строк
 * Внимание: s0 и s1 портятся!!
 */
static pts2(s0, s1)
register char	*s0, *s1;
{
    static short	l;
    static char	buf0[LLINE+1], buf1[LLINE+1];
    register short	j;
    if (s0 == NULL) 
        goto out;
    if ( l + strlen(s0) >= lline - 3 || l + strlen(s1) >= lline - 3)
	pts2(NULL, NULL);
    if (l == 0)
    {
	buf0[0] = buf1[0] = '|';
	l = 1;
    }
    j = l;
    buf0[j] = buf1[j] = ' ';
    j++;
    while (*s0 || *s1) {
	buf0[j] = *s0 ? *s0++ : ' ';
	buf1[j] = *s1 ? *s1++ : ' ';
	if (*s0 == ' ')
	    *s0 = 0;
	if (*s1 == ' ')
	    *s1 = 0;
	j++;
    }
    buf0[j] = buf1[j] = ' ';
    j++;
    buf0[j] = buf1[j] = '|';
    j++;
    l = j;
    if (l < lline - 4)
	return(0);
out:
    if (l == 0)
	return(0);
/*
    j = l - 2;
    putchar('+');
    while (j--)
	putchar('-');
    putchar('+');
    putchar(012);
 */
    buf0[l] = buf1[l] = 0;
    printf("%s\n%s\n", buf0, buf1);
    j = l - 2;
    putchar('+');
    while (j--)
	putchar('-');
    putchar('+');
    putchar(012);
    l = 0;
    return(4); /* Число строк */
}


