/*
 *                     RCS revision generation
 */

/* $Header: /usr/users/korotay/DEMOS_2.2/rcs/src/rcs/RCS/rcsgen.c,v 1.5 1986/03/13 10:17:30 dvolodin Exp $ */

/*********************************************************************************
 *********************************************************************************
 */

/* $Log: rcsgen.c,v $
 * Revision 1.5  1986/03/13  10:17:30  dvolodin
 * ���������� ������, ���������� ��� ���������� ���������.
 *
 * Revision 1.4  86/03/05  11:54:50  dvolodin
 * ��� ������� ��������, ���� ������ ������� �������� ������,
 * �������� �������� ����� �����������. ���������� ( �����! ).
 *
 * Revision 1.3  86/02/23  22:18:25  avg
 * ������� ��������� ��� ���� � ��� ������ � U-�����.
 *
 * Revision 1.2  85/12/29  15:04:26  fox
 * ����� rcsid
 *
 * Revision 1.1  85/12/26  22:08:35  fox
 * ������� �������� ������ ������
 *
 */

#include "rcsbase.h"

extern struct hshentry * getnum();
extern char * mktemp();
extern FILE * fopen();
extern savestring();
extern struct hshentry * genrevs();
extern editstring();

extern int nextc;          /* next character from lexical analyzer          */
extern char * RCSfilename, * workfilename;
extern struct hshentry * targetdelta; /* delta to be generated              */
extern char * Ktext;       /* keywords from syntax analyzer                 */
extern char * Klog;        /* Keyword "log"                                 */
extern char * Kdesc;       /* Keyword for description                       */
extern FILE * finptr;      /* RCS input file                                */
extern FILE * frewrite;    /* new RCS file                                  */
extern FILE * fcopy;       /* result file during editing                    */
extern FILE * fedit;       /* edit file                                     */
extern char * resultfile, *editfile;/* file names for fcopy and fedit       */
extern int    rewriteflag; /* indicates whether to rewrite the input file   */

char    curlogmsg[logsize] /* buffer for current log message                */
	='\0';

enum stringwork {copy, edit, expand, edit_expand };
/* parameter to scandeltatext() */


char * buildrevision(deltas, target, dir, expandflag)
struct hshentry ** deltas, * target;
char * dir; int expandflag;
/* Function: Generates the revision given by target
 * by retrieving all deltas given by parameter deltas and combining them.
 * If dir==nil, the revision is printed on the standard output,
 * otherwise written into a temporary file in directory dir.
 * if expandflag==true, keyword expansion is performed.
 * returns false on errors, the name of the file with the revision otherwise.
 *
 * Algorithm: Copy inital revision unchanged. Then edit all revisions but
 * the last one into it, alternating input and output files (resultfile and
 * editfile). The last revision is then edited in, performing simultaneous
 * keyword substitution (this saves one extra pass).
 * All this simplifies if only one revision needs to be generated,
 * or no keyword expansion is necessary, or if output goes to stdout.
 */
{
	int i;

	if (deltas[0]==target) {
		/* only latest revision to generate */
		if (dir==nil) {/* print directly to stdout */
			fcopy=stdout;
			scandeltatext(target,expand);
			return(char *) true;
		} else {
			initeditfiles(dir);
			scandeltatext(target,expandflag?expand:copy);
			ffclose(fcopy);
			return(resultfile);
		}
	} else {
		/* several revisions to generate */
		initeditfiles(dir?dir:"/tmp/");
		/* write initial revision into fcopy, no keyword expansion */
		scandeltatext(deltas[0],copy);
		i = 1;
		while (deltas[i+1] != nil) {
			/* do all deltas except last one */
			scandeltatext(deltas[i++],edit);
		}
		if (!expandflag) {
			/* no keyword expansion; only invoked from ci */
			scandeltatext(deltas[i],edit);
			finishedit(nil);
			ffclose(fcopy);
		} else {
			/* perform keyword expansion*/
			/* first, get to beginning of file*/
			finishedit(nil); swapeditfiles(dir==nil);
			scandeltatext(deltas[i],edit_expand);
			finishedit(deltas[i]);
			if (dir!=nil) ffclose(fcopy);
		}
		return(resultfile); /*doesn't matter for dir==nil*/
	}
}



scandeltatext(delta,func)
struct hshentry * delta; enum stringwork func;
/* Function: Scans delta text nodes up to and including the one given
 * by delta. For the one given by delta, the log message is saved into
 * curlogmsg and the text is processed according to parameter func.
 * Assumes the initial lexeme must be read in first.
 * Does not advance nexttok after it is finished.
 */
{       struct hshentry * nextdelta;

	do {
		nextlex();
		if (!(nextdelta=getnum())) {
			fatserror("Can't find delta for revision %s", delta->num);
		}
		if (!getkey(Klog) || nexttok!=STRING)
			serror("Missing log entry");
		elsif (delta==nextdelta) {
			savestring(curlogmsg,logsize);
			delta->log=curlogmsg;
		} else {readstring();
			delta->log= "";
		}
		nextlex();
		if (!getkey(Ktext) || nexttok!=STRING)
			fatserror("Missing delta text");

		if(delta==nextdelta)
			/* got the one we're looking for */
			switch (func) {
			case copy:      copystring();
					break;
			case expand:    xpandstring(delta);
					break;
			case edit:      editstring(nil);
					break;
			case edit_expand: editstring(delta);
					break;
			}
		else    readstring(); /* skip over it */

	} while (delta!=nextdelta);
}


int putdesc(initflag,textflag,textfile,quietflag)
int initflag,textflag; char * textfile; int quietflag;
/* Function: puts the descriptive text into file frewrite.
 * if !initflag && !textflag, the text is simply copied from finptr.
 * Otherwise, if the textfile!=nil, the text is read from that
 * file, or from stdin, if textfile==nil.
 * if initflag&&quietflag&&!textflag, an empty text is inserted.
 * if !initflag, the old descriptive text is discarded.
 * Returns true is successful, false otherwise.
 */
{       FILE * txt; register int c, old1, old2;

	if (!initflag && !textflag) {
		/* copy old description */
		fprintf(frewrite,"\n\n%s%c",Kdesc,nextc);
		rewriteflag=true; getdesc(false);
		return true;
	} else {
		/* get new description */
	       if (!initflag) {
			/*skip old description*/
			rewriteflag=false; getdesc(false);
		}
		fprintf(frewrite,"\n\n%s\n%c",Kdesc,SDELIM);
		if (textfile) {
			old1='\n';
			/* copy textfile */
			if ((txt=fopen(textfile,"r"))!=NULL) {
				while ((c=getc(txt))!=EOF) {
					if (c==SDELIM) putc(c,frewrite); /*double up*/
					putc(c,frewrite);
					old1=c;
				}
				if (old1!='\n') putc('\n',frewrite);
				fclose(txt);
				putc(SDELIM,frewrite);fputs("\n\n", frewrite);
				return true;
			} else {
				error("Can't open file with description%s",textfile);
			}
		}
		if (initflag&&quietflag) {
			warn("empty descriptive text");
			putc(SDELIM,frewrite);fputs("\n\n", frewrite);
			return true;
		}
		/* read text from stdin */
		if (isatty(fileno(stdin))) {
		    fputs("enter description, terminated with ^D or '.':\n",stdout);
		    fputs("NOTE: This is NOT the log message!\n>> ",stdout);
		}
		old1 = '\n'; old2= '\n';
		for (;;) {
			c=getchar();
			if (c==EOF) {
				if (isatty(fileno(stdin))) putc('\n',stdout);
				putc(old1,frewrite);
				if (old1!='\n') putc('\n',frewrite);
				break;
			}
			if (c=='\n' && old1=='.' && old2=='\n') {
				break;
			}
			if (c=='\n' && isatty(fileno(stdin))) fputs(">> ",stdout);
			if(old1==SDELIM) putc(old1,frewrite); /* double up*/
			putc(old1,frewrite);
			old2=old1;
			old1=c;
		} /* end for */
		putc(SDELIM,frewrite);fputs("\n\n",frewrite);
		return true;
	}
}
