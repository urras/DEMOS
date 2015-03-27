static  char    rcsid[] = "$Header: cc.c,v 1.14 88/09/24 19:48:23 root Exp $";
/*
 * $Log:	cc.c,v $
 * Revision 1.14  88/09/24  19:48:23  root
 * После великого переселения программ,
 * mm живет в новом месте.
 * 
 * Revision 1.13  88/09/22  11:59:57  korotaev
 * Теперь старые библиотеки ищутся в /usr/old/lib.
 *
 * Revision 1.12  88/05/07  14:56:48  andrew
 * -V зря передавался 1 и 2 проходам
 *
 * Revision 1.11  88/04/21  20:35:23  andrew
 * -Q последний - библиотека в оверлей, -L последний - в базу
 *
 * Revision 1.10  88/04/15  15:13:53  andrew
 * Ключик -v теперь передается ld.
 *
 * Revision 1.9  88/04/13  16:51:13  andrew
 * Увеличены размеры списков файлов, исправлена диагностика,
 * оверлейная библиотека подключается теперь через "-L",
 * по флагам -Q, -Z, -V, -L устанавливается -DC_OVERLAY
 *
 * Revision 1.8  87/08/17  17:15:04  dmitry
 * При вызове as ключик -V подставляется ВСЕГДА.
 *
 * Revision 1.7  87/03/04  14:30:20  alex
 * По ключам -Z и -L подключаем оверлейную библиотеку.
 *
 * Revision 1.6  87/03/04  14:26:02  alex
 * Убрали за ненадобностью обращение к libfpsim
 *
 * Revision 1.5  87/01/05  19:57:55  alex
 * Исправлена ошибка : ключ "-i" не передавался "ld".
 *
 * Revision 1.4  87/01/04  20:59:31  avg
 * Правлен setsuf - он должен оставлять только последнюю компоненту имени.
 *
 * Revision 1.3  86/12/30  19:18:24  avg
 * Сделан суффикс .cm и доведен до ума ключ -M.
 *
 * Revision 1.2  86/12/26  21:11:35  alex
 * Вставлены : минимакро, ключи "dyYmMT".
 * Изменены умолчания для "-t".
 *
 * Revision 1.1  86/12/25  18:18:19  alex
 * Initial revision
 *
 *
 */
# include <stdio.h>
# include <ctype.h>
# include <signal.h>
/**? # include <whoami.h> */
#ifdef sparc
# include "/home/mellorn/zaitcev/d22/include/ediag.h"
#else
# include <ediag.h>
#endif

/* cc command */

#ifndef VIRUS_VFORK
#define vfork fork
#endif VFORK

# define MAXINC 10
# define MAXFIL 256
# define MAXLIB 256
# define MAXOPT 100
char    *tmp0;
char    *tmp1;
char    *tmp2;
char    *tmp3;
char    *tmp4;
char    *tmp5;
char    *outfile;
# define CHSPACE 1000
char    ts[CHSPACE+50];
char    *tsa = ts;
char    *tsp = ts;
char    *av[MAXLIB+50];
char    *clist[MAXFIL];
char    *llist[MAXLIB];
int     pflag;
int     sflag;
int     cflag;
int     eflag;
int     iflag;
int     vflag;
int     exflag;
int     oflag;
int     proflag;
int     cdebflg;
int     yflag;
int     Yflag;
int     nocppflag;
int     Aflag;
int     Mflag;
int     mflag;


int     ovlyflag;
int     Qflag;
	/*
	 * Predefined constant always true for an overlay compilation.
	 * Useful for sorting through include files...
	 */
#define OVL_DEF "-DC_OVERLAY"
#define OVLIBC  "-lovc"
#define SPLITC  "-lsplitc"
#define OVBASEC "-lovbasec"
#  ifdef sparc
#    define LD      "/home/mellorn/zaitcev/d22/bin/ld"
#    define AS      "/home/mellorn/zaitcev/d22/bin/as"
#    define NEWAS   "/home/mellorn/zaitcev/bin/newas"  /* No such pest */
#    define MM      "/usr/bin/mm"
#  else
#    define LD      "/bin/ld"
#    define AS      "/bin/as"
#    define NEWAS   "/usr/bin/newas";
#    define MM      "/usr/ucb/bin/mm"
#  endif
#define LIBC    "-lc"
int     noflflag;
char *pld = LD;
char *pas = AS;
char *pmm = MM;
char    *chpass ;
char    *npassname ;
 /* P3: [40] is not enougth for /export/home/zaitcev/cross11/compl/c/c* */
char    pass0[100] = "/home/mellorn/zaitcev/d22/lib/c0";
char    pass1[100] = "/home/mellorn/zaitcev/d22/lib/c1";
char    pass2[100] = "/home/mellorn/zaitcev/d22/lib/c2";
char    passp[100] = "/home/mellorn/zaitcev/d22/lib/cpp";
char    *pref = "/home/mellorn/zaitcev/d22/lib/crt0.o";
char    *copy();
char    *setsuf();
char    *strcat();
char    *strcpy();
char    *mktemp();
char    *rindex();

main(argc, argv)
char *argv[];
{
	char *t;
	char *savetsp;
	char *assource;
	char **pv, *ptemp[MAXOPT], **pvt;
	int nc, nl, i, j, c, f20, nxo, na;
	int idexit();

	i = nc = nl = f20 = nxo = 0;
	setbuf(stdout, (char *)NULL);
	pv = ptemp;
	while(++i < argc) {
		if(*argv[i] == '-') switch (argv[i][1]) {
		default:
			goto passa;
		case 'S':
			sflag++;
			cflag++;
			break;
		case 'o':
			if (++i < argc) {
				outfile = argv[i];
				if ((c=getsuf(outfile))=='c'||c=='o') {
					error(ediag("Would overwrite %s",
						    "Файл %s будет испорчен"), outfile);
					exit(8);
				}
			}
			break;
		case 'O':
			oflag++;
			break;
		case 'a':
			nocppflag++;
			break;
		case 'M':
			mflag++;
			Mflag++;
			break;
		case 'm':
			mflag++;
			break;
		case 'y':
			yflag++;
			break;
		case 'Y':
			Yflag++;
			break;
		case 'A':
			Aflag++;
			break;
		case 'd':
			cdebflg++;
			break;
		case 'p':
			proflag++;
			break;
		case 'Q':
			Qflag++;
		case 'Z':
		case 'L':
		case 'V':
			if (!ovlyflag) {
			    *pv++ = OVL_DEF;
			    if (pv >= ptemp+MAXOPT) {
				error(ediag("Too many DIUC options",
					    "Слишком много ключей DIUC"), (char *)NULL);
				--pv;
			    }
			}
			ovlyflag++;
			if (argv[i][1] == 'V')
			    break;
			goto passa;

		case 'E':
			exflag++;
		case 'P':
			pflag++;
			*pv++ = argv[i];
		case 'c':
			cflag++;
			break;

		case 'f':
			noflflag++;
			if (npassname || chpass)
				error(ediag("-f overwrites earlier option",
					    "-t должен быть после -f"), (char *)NULL);
			npassname = "/lib/f";
			chpass = "1";
			break;

		case '2':
			if(argv[i][2] == '\0')
				pref = "/lib/crt2.o";
			else {
				pref = "/lib/crt20.o";
				f20 = 1;
			}
			break;

		case 'D':
		case 'I':
		case 'U':
		case 'C':
			*pv++ = argv[i];
			if (pv >= ptemp+MAXOPT) {
				error(ediag("Too many DIUC options",
					    "Слишком много ключей DIUC"), (char *)NULL);
				--pv;
			}
			break;
		case 't':
			if (chpass)
				error(ediag("-t overwrites earlier option",
					    "-t должен быть один раз"), (char *)NULL);
			chpass = argv[i]+2;
			if (chpass[0]==0)
				chpass = "012p";
			break;

		case 'B':
			if (npassname)
				error(ediag("-B overwrites earlier option",
					    "-t должен быть перед -f"), (char *)NULL);
			npassname = argv[i]+2;
			if (npassname[0]==0)
				npassname = "/usr/old/lib/";
			break;
		  case 'T': switch(argv[i][2]){
			    case 'l': pld = &(argv[i][3]); break;
			    case 'a': pas = &(argv[i][3]);
				      if ( !*pas ) pas = NEWAS;
					break;
			    case 'm': pmm =  &(argv[i][3]); break;
			    default: error( ediag("Bad 'T.' specifier.",
						  "Плохой код после -T.")); break;
				}
			   break;

		case 'v':
			vflag++;
			goto passa;

		case 'i':
			iflag++;
			goto passa;
		}
		else {
passa:
			t = argv[i];
			if((c=getsuf(t))=='c' || c=='s'|| c=='m' || exflag) {
				clist[nc++] = t;
				if (nc>=MAXFIL) {
					error(ediag("Too many source files",
						    "Слишком много исх. файлов"), (char *)NULL);
					exit(1);
				}
				t = setsuf(t, 'o');
			}
			if (nodup(llist, t)) {
				llist[nl++] = t;
				if (nl >= MAXLIB) {
					error(ediag("Too many object/library files",
						    "Слишком много библиотек/объектных файлов"), (char *)NULL);
					exit(1);
				}
				if (getsuf(t)=='o')
					nxo++;
			}
		}
	}
	if (npassname && chpass ==0)
		chpass = "012p";
	if (chpass && npassname==0)
		npassname = "/usr/old/lib/";
	if (chpass)
		for (t=chpass; *t; t++) {
			switch (*t) {
			case '0':
				strcpy (pass0, npassname);
				strcat (pass0, "c0");
				continue;
			case '1':
				strcpy (pass1, npassname);
				strcat (pass1, "c1");
				continue;
			case '2':
				strcpy (pass2, npassname);
				strcat (pass2, "c2");
				continue;
			case 'p':
				strcpy (passp, npassname);
				strcat (passp, "cpp");
				continue;
			}
		}
	if (noflflag)
		pref = proflag ? "/lib/fmcrt0.o" : "/lib/fcrt0.o";
	else if (proflag)
		pref = "/lib/mcrt0.o";
	if(nc==0)
		goto nocom;
	if (pflag==0) {
		char FD;

		tmp0 = copy("/tmp/ctm0XXXXX");
		mktemp (tmp0);
		FD = creat (tmp0,0);
		if (FD < 0)
		{
			error(ediag("cc: cannot create temp",
				    "cc: не могу создать временные файлы"), NULL);
			exit(1);
		}
		close (FD);
	}
	if (signal(SIGINT, SIG_IGN) != SIG_IGN)
		signal(SIGINT, idexit);
	if (signal(SIGTERM, SIG_IGN) != SIG_IGN)
		signal(SIGTERM, idexit);
	if (pflag==0) {           /* P3. Avoid coredump */
		(tmp1 = copy(tmp0))[8] = '1';
		(tmp2 = copy(tmp0))[8] = '2';
		(tmp3 = copy(tmp0))[8] = '3';
		if (oflag)
			(tmp5 = copy(tmp0))[8] = '5';
		(tmp4 = copy(tmp0))[8] = '4';
	}
	pvt = pv;
	for (i=0; i<nc; i++) {
		if (nc>1 || vflag)
			printf("%s:\n", clist[i]);
		if (getsuf(clist[i])=='s') {
			assource = clist[i];
			goto assemble;
		}
		else
			assource = tmp3;
		if (pflag)
			tmp4 = setsuf(clist[i], 'i');
		savetsp = tsp;
		av[1] = clist[i];
		if ( !nocppflag) {
		      av[0] = "cpp";
		      av[2] = exflag ? "-" : tmp4;
		      na = 3;
		      for(pv=ptemp; pv <pvt; pv++)
			      av[na++] = *pv;
		      av[na++]=0;
		      if(vflag)
			      doecho(passp, av);
		      if (callsys(passp, av)) {
			      cflag++;
			      eflag++;
			      continue;
		      }
		      av[1] = tmp4;
		      };
		if (pflag) {
			cflag++;
			continue;
		}
		if ( getsuf(clist[i]) == 'm' || mflag)
		{
		av[0] = "mm";
		av[2] =  Mflag?NULL:tmp3;
		av[3] =  NULL;
		if ( vflag ) doecho(pmm,av);
		if ( callisys(pmm,av)) {
			cflag++;
			eflag++;
			printf(ediag("Error of mm\n",
				     "Ошибка в mm\n"));
			continue;
		}
		if ( Mflag ) continue;
		av[1] = tmp3;
		}
		av[0]= "c0";
		na = 2;
		av[na++] = tmp1;
		av[na++] = tmp2;
		if (yflag)
			av[na++] = "-y";
		if (Yflag)
			av[na++] = "-Y";
		if (Aflag)
			av[na++] = "-A";
		if (cdebflg)
			av[na++] = "-d";
		if (proflag)
			av[na++] = "-P";
		/*if (ovlyflag)
			av[na++] = "-V";
		*/
		av[na++] = 0;
		if(vflag)
			doecho(pass0, av);
		if (callsys(pass0, av)) {
			cflag++;
			eflag++;
			continue;
		}
		av[0] = "c1";
		av[1] = tmp1;
		av[2] = tmp2;
		if (sflag)
			assource = tmp3 = setsuf(clist[i], 's');
		av[3] = tmp3;
		if (oflag)
			av[3] = tmp5;
		na = 4;
		/*if (ovlyflag)
			av[na++] = "-V";
		*/
		if (cdebflg)
			av[na++] = "-d";
		av[na++] = 0;
		if(vflag)
			doecho(pass1, av);
		if(callsys(pass1, av)) {
			cflag++;
			eflag++;
			continue;
		}
		if (oflag) {
			av[0] = "c2";
			av[1] = tmp5;
			av[2] = tmp3;
			av[3] = 0;
			if(vflag)
				doecho(pass2, av);
			if (callsys(pass2, av)) {
				unlink(tmp3);
				tmp3 = assource = tmp5;
			}
			else
				unlink(tmp5);
		}
		if (sflag)
			continue;
assemble:
		av[0] = "as";
		av[1] = "-u";
		na = 2;
/*                if (ovlyflag)         */
		/* P3: avg's assembler doesn't recognise this */
                /*	av[na++] = "-V"; */
		av[na++] = "-o";
		av[na++] = setsuf(clist[i], 'o');
		av[na++] = assource;
		av[na++] = 0;
		cunlink(tmp1);
		cunlink(tmp2);
		cunlink(tmp4);
		if(vflag)
			doecho(pas, av);
		if (callsys(pas, av) > 1) {
			cflag++;
			eflag++;
			continue;
		}
	}
nocom:
	if (cflag==0 && nl!=0 && Mflag == 0) {
		i = 0;
		av[0] = "ld";
		av[1] = "-X";
		av[2] = pref;
		j = 3;
	      /* -- Так как теперь libfpsim засунута в libc
		if (noflflag) {
			j = 4;
			if (iflag)
				av[3] = "-lfpsim_sep";
			else
				av[3] = "-lfpsim";
		}
	       */
		if (outfile) {
			av[j++] = "-o";
			av[j++] = outfile;
		}
		while(i<nl)
			av[j++] = llist[i++];
		if(f20)
			av[j++] = "-l2";
		else {
			if (ovlyflag && !Qflag)
				av[j++] = "-L";
			if (!Qflag)
				av[j++] = ovlyflag ? OVLIBC : LIBC;
			else {
				av[j++] = SPLITC;
				av[j++] = "-L";
				av[j++] = OVBASEC;
			}
		}
		av[j++] = 0;
		if(vflag)
			doecho(pld, av);
		eflag |= callsys(pld, av);
		if (nc==1 && nxo==1 && eflag==0)
			cunlink(setsuf(clist[0], 'o'));
	}
	dexit();
}

idexit()
{
	eflag = 100;
	dexit();
}

dexit()
{
	if (!pflag) {
		cunlink(tmp1);
		cunlink(tmp2);
		if (sflag==0)
			cunlink(tmp3);
		cunlink(tmp4);
		cunlink(tmp5);
		cunlink(tmp0);
	}
	exit(eflag);
}

error(s, x)
char *s, *x;
{
	fprintf(exflag?stderr:stdout, s, x);
	putc('\n', exflag? stderr : stdout);
	cflag++;
	eflag++;
}

getsuf(as)
register char *as;
{
	register char *s;

	s = rindex( as, '.' );
	if( s == NULL )
		return(0);
	if( !strcmp( s, ".c" ) )
		return( 'c' );
	if( !strcmp( s, ".o" ) )
		return( 'o' );
	if( !strcmp( s, ".s" ) )
		return( 's' );
	if( !strcmp( s, ".a" ) )
		return( 'a' );
	if( !strcmp( s, ".cm" ) )
		return( 'm' );
	return(0);
}

char *
setsuf(as, ch)
char *as;
{
	register char *s, *s1;

	s = s1 = copy(as);
	s = rindex(s, '.');
	if( s != NULL ) {
		s[1] = ch;
		s[2] = '\0';
	}
	s = rindex(s1, '/');
	if( s == NULL )
		return(s1);
	return(s+1);
}

callsys(f, v)
char f[], *v[];
{
	int t, status;

	if ((t=vfork())==0) {
		execv(f, v);
		printf(ediag("Can't find %s\n",
			     "Нет %s\n"), f);
		_exit(100);
	} else
		if (t == -1) {
			printf(ediag("Try again\n",
				     "Попробуйте еще раз\n"));
			return(100);
		}
	while(t!=wait(&status))
		;
	if (t = status&0377) {
		if (t!=SIGINT) {
			printf(ediag("Fatal error in %s\n",
				     "Ошибка или сбой в %s\n"), f);
			eflag = 8;
		}
		dexit();
	}
	return((status>>8) & 0377);
}

char *
copy(as)
char *as;
{
	char *malloc();
	register char *otsp, *s;

	otsp = tsp;
	s = as;
	while (*tsp++ = *s++)
		;
	if (tsp > tsa+CHSPACE) {
		tsp = tsa = malloc(CHSPACE+50);
		if (tsp==NULL) {
			error(ediag("no space for file names",
				    "нет памяти для имен файлов"), (char *)NULL);
			dexit();
		}
	}
	return(otsp);
}

nodup(l, os)
char **l, *os;
{
	register char *t, *s;
	register int c;

	s = os;
	if (getsuf(s) != 'o')
		return(1);
	while(t = *l++) {
		while(c = *s++)
			if (c != *t++)
				break;
		if (*t=='\0' && c=='\0')
			return(0);
		s = os;
	}
	return(1);
}

cunlink(f)
char *f;
{
	if (f==NULL)
		return;
	unlink(f);
}

doecho(s, v)
char *s;
register char **v;
{
	printf ("%s", s);
	while (*v)
		printf (" %s", *v++);
	putchar ('\n');
	fflush (stdout);
}


callisys(f, v)
char f[], *v[];
{
	char *fin, *fout;
	int t, status;
	if ((t=fork())==0) {
		fin = v[1]; fout=v[2]; v[2] = v[0]; v += 2;
		if ( fin )
		    { close(0);
		      if ( open(fin,0) != 0 )
			{ printf(stderr, ediag("Can not open %s\n",
					       "Не могу открыть %s\n"), fin);
			  exit(100);
			}
		    }
		if ( fout )
		    { close(1);
		      if ( creat(fout,0600) != 1 )
			{ printf(stderr, ediag("Can not open %s\n",
					       "Не могу открыть %s\n"), fin);
			  exit(100);
			}
		    }
		execv(f, v);
		printf(ediag("Can't find %s\n",
			     "Нет %s\n"), f);
		exit(100);
	} else
		if (t == -1) {
			printf(ediag("Try again\n",
				     "Попробуйте еще раз\n"));
			return(100);
		}
	while(t!=wait(&status))
		;
	if (t = status&0377) {
		if (t!=SIGINT) {
			printf(ediag("Fatal error in %s\n",
				     "Ошибка или сбой в %s\n"), f);
			eflag = 8;
		}
		dexit();
	}
	return((status>>8) & 0377);
}
