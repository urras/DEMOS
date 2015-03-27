/*
 *  $Log:	ttynew.c,v $
 * Revision 22.2  90/11/12  19:22:46  root
 * Новые вещи для СМ1425 и перемещение include.
 * 
 * Revision 22.1  89/04/12  19:48:09  korotaev
 * "param.h" ==> <sys/param.h>
 *
 * Revision 22.0  89/03/28  16:17:22  korotaev
 * Begining ДЕМОС 2.2.
 *
 * Revision 1.21  89/03/28  16:15:44  korotaev
 * *** empty log message ***
 *
 * Revision 1.19  89/01/13  17:47:35  dvolodin
 * Исправлена ошибка в DO_ESC при работе в режиме CEL15.
 *
 * Revision 1.18  88/11/09  12:40:41  abs
 * Для ВТА-2000-2 заменен порядок перекодировки \n -> \r\n
 * на     \n -> \n\r, ибо \r в режиме LECSH на нем затирает ТЕКУЩУЮ
 * строку.
 *
 * Revision 1.17  88/10/18  20:34:18  korotaev
 * Убран лишний вызов процедуры.
 *
 * Revision 1.16  88/07/15  16:33:50  alex
 * Внесены правки для работы с КОИ-8/QWERTY терминалы
 *
 * Revision 1.15  88/06/06  14:33:26  korotaev
 * Вставлен IPK_SELECT (from TCP/IP).
 *
 * Revision 1.14  88/03/26  16:17:20  andrew
 * Во время прерывания выдачи (напр. по ^C) не восстанавливался
 * латинский регистр.
 *
 * Revision 1.13  88/03/17  12:34:16  korotaev
 * Версия после слияния с АЗЛК и Э-85 и перед слиянием с SCHED
 * (новым планировщиком).
 *
 * Revision 1.12  88/01/21  11:49:57  korotaev
 * Изменения с Э-85.
 *
 * Revision 1.11  87/02/23  19:12:09  alex
 * Символ ESCAPE теперь снимает номер колонки, чтобы не портить работу с
 * экраном.
 *
 * Revision 1.10  87/01/22  08:40:47  alex
 * Коды 0201 - 0277 интерпретируются как задержки.
 *
 * Revision 1.9  86/09/07  20:53:40  avg
 * *SYNTAX*
 *
 * Revision 1.8  86/09/07  20:06:05  avg
 * При lcase, siso и -ucase забой не затирал \ у \! (это |).
 *
 * Revision 1.7  86/06/09  10:27:27  avg
 * Исправлена ошибка - ? в русском регистре в случае CEL15I превращался в ^_.
 *
 * Revision 1.6  86/05/30  15:08:52  avg
 * Внесены изменения при работе с LITOUT-ом :
 *      1) Вычисляемые задержки выводятся как невычисляемые - т.к.
 *         счетчик тек. колонки может быть сбит.
 *      2) Маленькие латинские буквы на двухрегистровый терминал
 *         (LCASE) выводятся БЕЗ преобразования - for the sake of compatibility.
 *
 * Revision 1.5  86/05/13  17:22:56  avg
 * В режиме CBREAK+LCASE на вводе не должен работать \.
 * Кроме того, по LITOUT РАБОТАЮТ LZNAK и LTILDE, но
 * не работают width и отображение больших букв при UCASE и { как \(.
 *
 * Revision 1.4  86/05/12  13:11:03  avg
 * В режиме LITOUT теперь производится перекодировка в
 * смешанный код КОИ-7 для терминалов, имеющих только большие буквы.
 *
 * Revision 1.3  86/05/05  17:04:20  avg
 * Все-таки на РУС/ЛАТ всегда надо выдавать CRUS и CLAT !
 *
 * Revision 1.2  86/04/22  19:38:04  avg
 * Сделаны изменения под русские буквы.
 *
 * Revision 1.1  86/04/19  17:55:18  avg
 * Initial revision
 *
 */

/*
 * New version of tty driver, supported
 * as NTTYDISC.
 */
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/tty.h>
#include <sys/proc.h>
#include <sys/inode.h>
#include <sys/file.h>
#include "../include/reg.h"
#include <sys/conf.h>
#include "../include/buf.h"

#ifndef UCB_NET
#define ttsignal(tp, c) gsignal((tp)->t_pgrp, c)
#endif

extern  char    partab[];
extern char qwerty_in[];
#ifdef DO_ESCAPE
#define ESCTIME(t)      (ttitime[(t)->t_ispeed&017])

short ttitime[16] =
	/*  0  50  75 110 134 150 200 300 600 1.2 1.8 2.4 4.8 9.6  A  B */
	{  50, 50, 40, 30, 20, 15, 15, 10,  5,  4,  3,  3,  3,  2, 2, 2};

#endif  DO_ESCAPE
extern long timeclk;

/*
 * shorthand
 */
#define q1      tp->t_rawq
#define q2      tp->t_canq
#define q3      (*(tp->t_optr))
#define q4      tp->t_un.t_ctlq

#define OBUFSIZ 100

/*
 * routine called on switching to NTTYDISC
 */
ntyopen(tp)
register struct tty *tp;
{
	if(tp->t_flags & RAW) tp->t_lstate |= L8BITS; /* Для "сырого" open */
	if (tp->t_line != NTTYDISC)
		wflushtty(tp);
}

/*
 * clean tp on exiting NTTYDISC
 */
ntyclose(tp)
struct tty *tp;
{
	wflushtty(tp);
}

/*
 * reinput pending characters after state switch
 * call at spl5().
 */
ttypend(tp)
register struct tty *tp;
{
	struct clist tq;
	register c;

	tp->t_local &= ~LPENDIN;
	tp->t_lstate |= LSTYPEN;
	tq = tp->t_rawq;
	tp->t_rawq.c_cc = 0;
	tp->t_rawq.c_cf = tp->t_rawq.c_cl = NULL;
	while ((c = getc(&tq)) >= 0)
		ntyinput(c, tp);
	tp->t_lstate &= ~LSTYPEN;
}

/*
 * Place a character on raw TTY input queue, putting in delimiters
 * and waking up top half as needed.
 * Also echo if required.
 * The arguments are the character and the appropriate
 * tty structure.
 */
ntyinput(c, tp)
register c;
register struct tty *tp;
{
	register int t_flags;
	int i;

	if (tp->t_local&LPENDIN)
		ttypend(tp);
	tk_nin++;
	c &= 0377;
	t_flags = tp->t_flags;
	if (t_flags&TANDEM)
		ttyblock(tp);
	if ((t_flags&RAW)==0) {
		if ((tp->t_lstate&LSTYPEN) == 0)
			c &= 0177;
	/* check for literal nexting before any other processing */
		if (tp->t_lstate&LSLNCH) {
			if ( c < 0100 || c == 0177 )
				c |= 0200;
			tp->t_lstate &= ~LSLNCH;
		}
		if (t_flags & CYRILL) {
			if ((t_flags & (LCASE|SISO)) != LCASE) {
				if  (c == CRUS || c == CLAT) {
					i = LIREG;
					if (!(t_flags & LCASE) &&
					     (t_flags & ECHO)  &&
					     (t_flags & CSTYLE) != CBITS8 &&
					     (t_flags & CSTYLE) != CBITS8Q ) {
						if( !PutcOutq(c, tp) ) {
							i |= LOREG;
							ttstart(tp);
						}
					}
					if (c == CRUS)
						tp->t_lstate |= i;
					else
						tp->t_lstate &= ~i;
					return;
				}
			}
		}

 /* I MUST MENTION:
  *     Because c is an integer and t_????c are chars :
  *
  *             when c greater then 0177, c NOT sign extended,
  *             but t_????c if greater 0177 convert from chars to integers
  *             and EXTENDS from 03??? to 01777???
  *     Therefore: \^?  is not equal  undefined  t_????c (==0377).
  *                     ~~~~~~~~~~~~
  *
  *     THIS IS NONPORTABLE ! (And works only on PDP-11 computers)
  *                                     4/15/86    @VG
  */

		if (c==tlun.t_lnextc) {
			if (tp->t_flags&ECHO)
				ntyout("^\b", tp);
			tp->t_lstate |= LSLNCH;
	/* check for output control functions */
		} else if (c==tun.t_stopc) {
			if ((tp->t_state&TTSTOP)==0) {
				tp->t_state |= TTSTOP;
				(*cdevsw[major(tp->t_dev)].d_stop)(tp,0);
				return;
			}
			if (c==tun.t_startc)
				goto tSTART;    /* KLUDGE */
		} else if (c==tun.t_startc) {
tSTART:
			tp->t_state &= ~TTSTOP;
			tp->t_local &= ~LFLUSHO;
			ttstart(tp);
			return;
	/* check for input interrupts (and flushed output) */
		} else if (c==tlun.t_flushc) {
			if (tp->t_local & LFLUSHO)
				tp->t_local &= ~LFLUSHO;
			else {
				flushtty(tp, FWRITE);
				ntyecho(c, tp);
				if (tp->t_rawq.c_cc+tp->t_canq.c_cc)
					ntyretype(tp);
				tp->t_local |= LFLUSHO;
			}
			ttstart(tp);
			return;
		} else if (c==tlun.t_suspc || c==tun.t_intrc ||
			   c==tun.t_quitc) {
				if ((tp->t_local & LNOFLSH) == 0)
					flushtty(tp, c==tlun.t_suspc ? FREAD : FREAD|FWRITE);
			ntyecho(c, tp);
			c = c==tun.t_intrc ? SIGINT :
				((c==tun.t_quitc) ? SIGQUIT : SIGTSTP);
				ttsignal(tp, c);
	/* check for buffer editing functions - cooked mode */
		} else if ((t_flags&CBREAK) == 0) {
			if ((tp->t_lstate&LSQUOT) &&
			    (c==tp->t_erase||c==tp->t_kill)) {
				ntyrub(unputc(&tp->t_rawq), tp);
				if(c < 0100 || c == 0177)
					c |= 0200;
			}
			if (c==tp->t_erase) {
				if (tp->t_rawq.c_cc)
					ntyrub(unputc(&tp->t_rawq), tp);
			} else if (c==tp->t_kill) {
				if (tp->t_local&LCRTKIL &&
				    tp->t_rawq.c_cc == tp->t_rocount) {
					while (tp->t_rawq.c_cc)
						ntyrub(unputc(&tp->t_rawq), tp);
				} else {
					ntyecho(c, tp);
					ntyecho('\n', tp);
					while (getc(&tp->t_rawq) > 0)
						;
					tp->t_rocount = 0;
				}
				tp->t_lstate &= LIREG|LOREG|L8BITS;
			} else if (c==tlun.t_werasc) {
				if (tp->t_rawq.c_cc == 0)
					goto out;
				do {
					c = unputc(&tp->t_rawq);
					if (c != ' ' && c != '\t')
						goto erasenb;
					ntyrub(c, tp);
				} while (tp->t_rawq.c_cc);
				goto out;
			    erasenb:
				do {
					ntyrub(c, tp);
					if (tp->t_rawq.c_cc == 0)
						goto out;
					c = unputc(&tp->t_rawq);
				} while (c != ' ' && c != '\t');
				(void) putc(c, &tp->t_rawq);
			} else if (c==tlun.t_rprntc) {
				ntyretype(tp);
	/* check for cooked mode input buffer overflow */
			} else if (tp->t_rawq.c_cc + tp->t_canq.c_cc > TTYHOG) {
				/* we should start a timeout that flushes the
				   buffer if it stays full - same in CBREAK */
				if (tp->t_optr->c_cc < TTHIWAT(tp))
					(void) ntyoutput(CTRL(g), tp);
	/* put data char in q for user and wakeup if a break char */
			} else if (putc((c = Iconv(c, tp)), &tp->t_rawq) >= 0) {
				if (!ntbreakc(c, tp)) {
					if (tp->t_rocount++ == 0)
						tp->t_rocol = tp->t_col;
				} else {
					tp->t_rocount = 0;
					catq(&tp->t_rawq, &tp->t_canq);
						ttwakeup(tp);
					if (tp->t_local&LINTRUP)
						ttsignal(tp, SIGTINT);
				}
				tp->t_lstate &= ~LSQUOT;
				if (c == '\\')
					tp->t_lstate |= LSQUOT;
				if (tp->t_lstate&LSERASE) {
					tp->t_lstate &= ~LSERASE;
					(void) ntyoutput('/', tp);
				}
				i = tp->t_col;
				ntyecho(c, tp);
				if (c==tun.t_eofc && tp->t_flags&ECHO) {
					i = MIN(2, tp->t_col - i);
					while (i > 0) {
						(void) ntyoutput('\b', tp);
						i--;
					}
				}
			}
	/* CBREAK mode */
		} else if (tp->t_rawq.c_cc > TTYHOG) {
			if (tp->t_optr->c_cc < TTHIWAT(tp))
				(void) ntyoutput(CTRL(g), tp);
		} else if (putc((c = Iconv(c, tp)), &tp->t_rawq) >= 0) {
			if (tp->t_local&LINTRUP)
				ttsignal(tp, SIGTINT);
				ttwakeup(tp);
			ntyecho(c, tp);
		}
	/* RAW mode */
	} else if (tp->t_rawq.c_cc > TTYHOG)
		flushtty(tp, FREAD|FWRITE);
	else {
		if (putc(c, &tp->t_rawq) >= 0) {
			if (tp->t_local&LINTRUP)
				ttsignal(tp, SIGTINT);
				ttwakeup(tp);
		}
		ntyecho(c, tp);
	}
out:
	if (tp->t_local & LDECCTQ && tp->t_state & TTSTOP)
		return;
	tp->t_state &= ~TTSTOP;
	tp->t_local &= ~LFLUSHO;
	ttstart(tp);
}

/*
 * Convert character to KOI8 code from KOI7 external code
 * using the lstate registers (LIREG and LOREG).
 * This is too hard because a brain damage of SU ttys designers.
 * For some versions of autoregisters switching selecting implemented
 * by CSTYLE bits.  @VG
 */
static int Iconv(c, tp)
register c;
register struct tty *tp;
{
	register int cf;
	if ((tp->t_lstate & LSTYPEN) || !(tp->t_flags&CYRILL))
		return(c);
	c &= 0377;
	cf = tp->t_flags & ( LCASE | CSTYLE);
#ifdef  DO_ESCAPE
		if (c == CESCAPE && ((tp->t_lstate & LIREG) || cf == CEL15 )) {
			tp->t_esctime = timeclk + ESCTIME (tp);
			tp->t_escstate = 0;
		}
		else if (tp->t_esctime) {
			if (tp->t_esctime < timeclk || c < 040) {
			       tp->t_esctime = 0;
			} else {
				if (tp->t_escstate++ && c > 0100) {
					tp->t_esctime = 0;
				}
				return(c);
			}
		}
#endif  DO_ESCAPE
	if (cf != CBITS8Q && cf != CSISOQ && (c<=077 || c>=0177)|| c<040)
		return(c);

	if ((tp->t_flags & (LCASE|SISO)) == LCASE) {
		if (c>=0140 && c<0177)       /* Lower latin -> upper cyrillic */
			c = (c&037) | 0340;
		return(c);
	}

	switch (cf) {

	case CBMKBD:
	case CEL15:
		if ((tp->t_lstate & (LIREG|LOREG)) == LIREG ||
		    (tp->t_lstate & (LIREG|LOREG)) == LOREG )
			c ^= 040;
		if((tp->t_flags & CSTYLE) == CBMKBD) {
		/* LOST IN MUDDY WATERS OF HISTORY */
		   ;
		}
		break;
	case CBITS8Q:
	case CSISOQ:
		if ((tp->t_lstate & LIREG) )
			c = qwerty_in[c-040];
		return(c);
	case CEL15I:
	case CBITS8:
		if ((tp->t_lstate & LIREG) && c!=0137)
			c ^= 040;
		break;

	case CEL15K:
		break;

	case LCASE|SISO|UCASE:
	case LCASE|SISO:
		/*
		 * Для кириллицы перевернуть регистры Б <-> М
		 */
		if(tp->t_lstate & LIREG)
			c ^= 040;
	}
	if (tp->t_lstate & LIREG)
		c |= 0200;
	return(c);
}


/*
 * put character on TTY output queue, adding delays,
 * expanding tabs, and handling the CR/NL bit.
 * It is called both from the top half for output, and from
 * interrupt level for echoing.
 * The arguments are the character and the tty structure.
 * Returns < 0 if putc succeeds, otherwise returns char to resend
 * Must be recursive.
 */
ntyoutput(c, tp)
register c;
register struct tty *tp;
{
	register char *colp;
	register ctype;
	int      c1;

	c &= 0377;

	if (tp->t_flags & RAW) {
		if (tp->t_local&LFLUSHO)
			return (-1);
		if (PutcOutq(c, tp))
			return(c);
		tk_nout++;
		return (-1);
	}

	/* Avoid quoting for control chars  @VG @AR его забил */
	if ((c&0300) == 0200 && c != 0200 )   goto DELAY;
	/*      c &= ~0200;             */

	c1 = c;
#ifdef Ucode
	c = conv_out(c);
#endif
	if ((tp->t_local & LLITOUT) == 0) {

		/*
		 * Turn tabs to spaces as required
		 */
		if (c=='\t' && (tp->t_flags&XTABS)) {
			register int s;

			c = 8 - (tp->t_col&7);
			if ((tp->t_local&LFLUSHO) == 0) {
				s = spl5();  /* don't interrupt tabs */
				if( tp->t_tproc == NULL )
					c -= b_to_q("        ", c, tp->t_optr);
				else {
					register int i = c;

					while(PutcOutq(' ', tp) == 0 && --i ) ;
					c -= i;
				}
				tk_nout += c;
				splx(s);
			}
			tp->t_col += c;
			return (c ? -1 : '\t');
		}
	}

	if(tp->t_flags&LCASE) {
	/*
	 * Перекодировать буквы в смешанный код КОИ-7 для
	 * терминалов, имеющих только большие буквы.
	 */
		if ((tp->t_local & LLITOUT) == 0 &&
		     'a'<=c && c<='z') {
			c += 'A' - 'a';
			goto ABL;
		}
		if ( c>=0300 && c<0340) {
			c += 040;       /* Cyrillic A - a */
			goto ABL;
		}

	/*
	 * Генерировать составные символы \x для терминалов, имеющих
	 * только большие буквы.
	 */
		if((tp->t_local&LLITOUT) == 0) {
		    colp = "({)}!|^~'`";
		    while(*colp++)
			if(c == *colp++) {
				if (ntyoutput('\\', tp) >= 0)
					return (c);
				c = colp[-2];
				break;
			}
		    if ( (tp->t_flags & UCASE) &&
			((c&0300) == 0300 || ('A'<=c && c<='Z')) ) {
			  if( ntyoutput('\\', tp) >= 0 )
				return(c1);
		    }
		}
	}
    ABL:

	/*
	 * Конвертировать <nl> в <cr><lf> при необходимости.
	 */
	if (c=='\n' && (tp->t_flags&CRMOD) && (tp->t_local&LLITOUT) == 0){
		if (tp->t_local & LESCH ){
		/* На ВТА 2000-2 \r в виде ESC-M затирает текущую строку,
		 * поэтому следует преобразовывать не \n -> \r\n
		 * а \n -> \n\r
		 */
			c = '\r';
			tp -> t_flags &= ~CRMOD;
			if( ntyoutput( '\n', tp ) >= 0 ){
				tp-> t_flags |= CRMOD;
				return c1;
			}
			tp-> t_flags |= CRMOD;
			c1 = '\r';
			/* and later output c == '\r' */
		}else
		if (ntyoutput('\r', tp) >= 0)
			return (c1);
	}
	/*
	 * Обработка LTILDE и LZNAK
	 */
	switch(tp->t_flags & (CYRILL|LCASE)) {

	case CYRILL:
#ifdef Ucode
		/*
		 * Если символ - 'большой твердый знак', то
		 * вывести его как 'маленький твердый знак'
		 */
		if(c==0377 && !(tp->t_local & LZNAK))
			c = 0337;
#endif Ucode
		break;

	case CYRILL|LCASE:
		/*
		 * Если символ - 'твердый знак', то выводить его
		 * как 'мягкий знак'
		 */
		if(c==0377 && !(tp->t_local & LZNAK))
			c = 'x'|0200;   /* Мягкий знак в КОИ-8 */
		break;

	default:
		if (c=='~' && tp->t_local&LTILDE)
			c = '`';
	}

	/*
	 * Turn \b to ESC-H and \r to ESC-M for stupid BTA2000-2  @VG
	 */
	if ((c=='\b'||c=='\r') && (tp->t_local&(LESCH|LLITOUT)) == LESCH) {
		if(PutcOutq('\033', tp))
			return(c1);
		c = (c=='\b')? 'H': 'M';
		goto SKIR;
	}

	/*
	 * Установить регистры РУС/ЛАТ
	 */
	if (tp->t_flags & CYRILL)
		switch (tp->t_flags & (CSTYLE|LCASE)) {
		case CBITS8:
		case CBITS8Q:
			goto out_8;
		case CEL15:
		case CEL15I:
		case CEL15K:
		case CBMKBD:
		case CSISOQ:
			if (c == CRUS) {
				tp->t_lstate |= LOREG;
				break;
			}
			if (c == CLAT) {
				tp->t_lstate &= ~LOREG;
				break;
			}
			if ((c&0300) == 0300 && !(tp->t_lstate&LOREG)) {
				if(PutcOutq(CRUS, tp))
					return(c1);
				tp->t_lstate |= LOREG;
				ttstart(tp);
				break;
			}
			if (((c&0300) == 0100 ||
			      (partab[c&0377]&0177) == CONTROL) &&
			      (tp->t_lstate&LOREG)) {
				if(PutcOutq(CLAT, tp))
					return(c1);
				tp->t_lstate &= ~LOREG;
				ttstart(tp);
			}
		}

    SKIR:
	c &= 0177;
   out_8:
	tk_nout++;
    DELAY:
	if ((tp->t_local&LFLUSHO) == 0 && PutcOutq(c, tp))
		return (c1);

	/*
	 * Вычислить задержки.
	 * Задержки обозначаются символами с кодом больше 0200.
	 ******************************************
	 * В режиме raw задержки не вычисляются и *
	 * передаются полные 8-битные символы.    *
	 ******************************************
	 */
	colp = &tp->t_col;
	ctype = partab[c];
	c = 0;
	switch (ctype & 0177) {

	default:
		(*colp)++;
		break;

	case CONTROL:
		tp->t_lstate |= LNOWID;
		break;

	case BACKSPACE:
		if (*colp)
			(*colp)--;
		break;

	case NEWLINE:
		c = tp->t_nldly & 0177;
		if ( (tp->t_nldly & 0200) &&
		    !(tp->t_local & LLITOUT) ) {       /* Вычисляемая задержка */
			ctype = c;
			ctype = ((unsigned)(*colp) * ctype)/32;
			if (ctype > 0177)
				ctype = 0177;
			c = ctype;
		}
		tp->t_lstate &= ~LNOWID;
		*colp = 0;
		break;

	case TAB:
		c = tp->t_htdly & 0177;
		ctype = (unsigned)*colp;
		*colp |= 07;
		(*colp)++;
		if ( (tp->t_htdly & 0200) &&
		    !(tp->t_local & LLITOUT) ) {       /* Вычисляемая задержка */
			ctype = (unsigned)(*colp) - ctype+1;
			ctype = (c * ctype)/32;
			if (ctype > 0177)
				ctype = 0177;
			c = ctype;
		}
		break;

	case VTAB:
		c = tp->t_vtdly & 0177;
		break;

	case RETURN:
		c = tp->t_crdly & 0177;
		if ( (tp->t_crdly & 0200) &&
		    !(tp->t_local & LLITOUT) ) {       /* Вычисляемая задержка */
			ctype = c;
			ctype = ((unsigned)(*colp) * ctype)/32;
			if (ctype > 0177)
				ctype = 0177;
			c = ctype;
		}
		tp->t_lstate &= ~LNOWID;
		*colp = 0;
	}
	if(c && (tp->t_local&LFLUSHO) == 0)
		(void) PutcOutq(c|0200, tp);
	if((tp->t_local & LLITOUT) == 0 && tp->t_width != 0 &&
	   !(tp->t_lstate & LNOWID) ) {
		if ( (unsigned)(tp->t_col) >= (unsigned)(tp->t_width) )
			(void)ntyoutput('\n', tp);
	}
	return (-1);
}

/*
 * Called from device's read routine after it has
 * calculated the tty-structure given as argument.
 */
ntread(tp)
register struct tty *tp;
{
	register struct clist *qp;
	register c, first;

	if ((tp->t_state&CARR_ON)==0)
		return(0);
loop:
#ifdef SCHED
	/* mark process as one that does tty input */
	u.u_procp->p_sflag |= STTYIN;
#endif SCHED
	(void) _spl5();
	if (tp->t_local&LPENDIN)
		ttypend(tp);
	(void) _spl0();
	while (tp == u.u_ttyp && u.u_procp->p_pgrp != tp->t_pgrp) {
		if (u.u_signal[SIGTTIN] == SIG_IGN ||
		    u.u_signal[SIGTTIN] == SIG_HOLD ||
		    (u.u_procp->p_flag&SDETACH))
			return (0);
		gsignal(u.u_procp->p_pgrp, SIGTTIN);
		sleep((caddr_t)&lbolt, TTIPRI);
	}
	if (tp->t_flags&RAW) {
		(void) _spl5();
		if (tp->t_rawq.c_cc <= 0) {
			if ((tp->t_state&CARR_ON)==0
			    || (tp->t_state&TS_NBIO)
			   )
			{
				(void) _spl0();
				return (0);
			}
			if (tp->t_local&LINTRUP &&
			    u.u_signal[SIGTINT] != SIG_DFL) {
				u.u_error = EWOULDBLOCK;
				(void) _spl0();
				return (0);
			}
			sleep((caddr_t)&tp->t_rawq, TTIPRI);
			(void) _spl0();
			goto loop;
		}
		(void) _spl0();
		while (tp->t_rawq.c_cc && passc(getc(&tp->t_rawq))>=0)
			;
	} else {
		qp = tp->t_flags & CBREAK ? &tp->t_rawq : &tp->t_canq;
		(void) _spl5();
		if (qp->c_cc <= 0) {
			if ((tp->t_state&CARR_ON)==0
			   || (tp->t_state&TS_NBIO)
			   )
			{
				(void) _spl0();
				return (0);
			}
			if (tp->t_local&LINTRUP &&
			    u.u_signal[SIGTINT] != SIG_DFL) {
				u.u_error = EWOULDBLOCK;
				(void) _spl0();
				return (0);
			}
			sleep((caddr_t)&tp->t_rawq, TTIPRI);
			(void) _spl0();
			goto loop;
		}
		(void) _spl0();
		first = 1;
		while ((c = getc(qp)) >= 0) {
			c &= 0377;
			if ((c&0300) == 0200 || c==0377) {
				c &= ~0200;
				goto NCV;
			}
			if (tp->t_flags&CRMOD && c == '\r')
				c = '\n';
			if ((tp->t_flags&(LCASE|SISO)) == LCASE) {
				if (tp->t_lstate&LSBKSL) {
					if ((partab[c]&0177) >= 040)
						c = (partab[c]&0177)|(c&0200);
					tp->t_lstate &= ~LSBKSL;
				} else if (c >= 'A' && c <= 'Z')
					c += 'a' - 'A';
				else if (c >= 0340) /* Upper cyrillic -> lower */
					c -= 040;
				else if ((tp->t_flags&CBREAK)==0 && c == '\\') {
					tp->t_lstate |= LSBKSL;
					continue;
				}
			}
		    NCV:

			if (c == tlun.t_dsuspc) {
				ttsignal(tp, SIGTSTP);
				if (first) {
					sleep((caddr_t)&lbolt, TTIPRI);
					goto loop;
				}
				break;
			}
			if (c == tun.t_eofc && (tp->t_flags&CBREAK)==0)
				break;
#ifdef Ucode
			if (passc(conv_in(c)) < 0)
#else
			if (passc(c) < 0)
#endif Ucode
				break;
			if ((tp->t_flags&CBREAK)==0 && ntbreakc(c, tp))
				break;
			first = 0;
		}
		tp->t_lstate &= ~LSBKSL;
	}

	/*
	 * Resume output if:
	 * is blocked
	 * AND (input < threshold OR (cooked mode AND delim_count == 0))
	 * This makes tandem mode usable for line-mode input.
	 */
	if (tp->t_state&TBLOCK && ((tp->t_rawq.c_cc < TTYHOG/5) ||
	   (tp->t_delct==0 && ((tp->t_flags&(CBREAK|RAW)) == 0)))) {
		if (PutcOutq(tun.t_startc, tp)==0) {
			tp->t_state &= ~TBLOCK;
			ttstart(tp);
		}
	}

	return (tp->t_rawq.c_cc + tp->t_canq.c_cc);
}

/*
 * Called from the device's write routine after it has
 * calculated the tty-structure given as argument.
 */
caddr_t
ntwrite(tp)
register struct tty *tp;
{
	register char *cp;
	register int cc, ce;
	register i;
	char obuf[OBUFSIZ];
	register c;
	int hiwat = TTHIWAT(tp);
	int cnt = u.u_count;

	if ((tp->t_state&CARR_ON)==0)
		return (NULL);
loop:
#ifdef SCHED
	u.u_procp->p_sflag |= STTYOUT;
#endif

	while (u.u_procp->p_pgrp != tp->t_pgrp && tp == u.u_ttyp &&
	    (tp->t_local&LTOSTOP) &&
	    u.u_signal[SIGTTOU] != SIG_IGN &&
	    u.u_signal[SIGTTOU] != SIG_HOLD &&
	    (u.u_procp->p_flag&SDETACH)==0) {
		gsignal(u.u_procp->p_pgrp, SIGTTOU);
		sleep((caddr_t)&lbolt, TTIPRI);
	}
	while (u.u_count) {
		cc = MIN(u.u_count, OBUFSIZ);
		cp = obuf;
		iomove(cp, (unsigned)cc, B_WRITE);
		if (u.u_error)
			break;
		if (tp->t_optr->c_cc > hiwat)
			goto ovhiwat;
		if (tp->t_local&LFLUSHO)
			continue;

		while (cc--) {
			c = *cp++;
			tp->t_rocount = 0;
			while((c = ntyoutput(c, tp)) >= 0) {
				/* out of clists, wait a bit */
				ttstart(tp);
				sleep((caddr_t)&lbolt, TTOPRI);
				tp->t_rocount = 0;
			}
			if (tp->t_optr->c_cc > hiwat)
				goto ovhiwat;
		}
	}
	ttstart(tp);
	return(NULL);

ovhiwat:
	(void) _spl5();
	u.u_base -= cc;
	u.u_offset -= cc;
	u.u_count += cc;
	if (tp->t_optr->c_cc <= hiwat) {
		(void) _spl0();
		goto loop;
	}
	ttstart(tp);
	if (tp->t_state & TS_NBIO) {
		if (u.u_count == cnt)
			u.u_error = EWOULDBLOCK;
		return(NULL);
	}
	tp->t_state |= ASLEEP;
	if(tp->t_optr->c_cc > 0)
		sleep((caddr_t)(tp->t_optr), TTOPRI);
	(void) _spl0();
	goto loop;
}

/*
 * Rubout one character from the rawq of tp
 * as cleanly as possible.
 */
static ntyrub(c, tp)
register c;
register struct tty *tp;
{
	register char *cp;
	register int savecol;
	int s;
	char *nextc();

	if ((tp->t_flags&ECHO)==0)
		return;
	tp->t_local &= ~LFLUSHO;
	c &= 0377;
	if (tp->t_local&LCRTBS) {
		if (tp->t_rocount == 0) {
			/*
			 * Screwed by ttwrite; retype
			 */
			ntyretype(tp);
			return;
		}
		if (c==('\t'|0200) || c==('\n'|0200))
			ntyrubo(tp, 2);
		else {
			if( (c&0300)==0200 || c==0377 )
				c &= ~0200;
			switch(partab[c] & 0177) {

			default:
				if( (tp->t_flags&(LCASE|SISO))==(LCASE|SISO) &&
				    (c=='|' || c=='{' || c=='}' ||
				     c=='`' || c=='~' ||
				     ((tp->t_flags&UCASE) &&
					(('A'<=c&&c<='Z')||(c&0340)==0340)
				      )                                   ) ) {
					ntyrubo(tp, 2);
					break;
				}
				ntyrubo(tp, 1);
				break;

			case VTAB:
			case BACKSPACE:
			case CONTROL:
			case RETURN:
				if (tp->t_local & LCTLECH)
					ntyrubo(tp, 2);
				break;

			case TAB:
				if (tp->t_rocount < tp->t_rawq.c_cc) {
					ntyretype(tp);
					return;
				}
				s = spl5();
				savecol = tp->t_col;
				tp->t_lstate |= LSCNTTB;
				tp->t_local |= LFLUSHO;
				tp->t_col = tp->t_rocol;
				for (cp = tp->t_rawq.c_cf; cp; cp = nextc(&tp->t_rawq, cp))
					ntyecho(lookc(cp), tp);
				tp->t_local &= ~LFLUSHO;
				tp->t_lstate &= ~LSCNTTB;
				splx(s);
				/*
				 * savecol will now be length of the tab
				 */
				savecol -= tp->t_col;
				tp->t_col += savecol;
				if (savecol > 8)
					savecol = 8;            /* overflow screw */
				while (--savecol >= 0)
					(void) ntyoutput('\b', tp);
				break;
			}
		}
	} else if (tp->t_local&LPRTERA) {
		if ((tp->t_lstate&LSERASE) == 0) {
			(void) ntyoutput('\\', tp);
			tp->t_lstate |= LSERASE;
		}
		ntyecho(c, tp);
	} else
		ntyecho(tp->t_erase, tp);
	tp->t_rocount--;
}

/*
 * Crt back over cnt chars perhaps
 * erasing them.
 */
static ntyrubo(tp, cnt)
register struct tty *tp;
register cnt;
{

	while (--cnt >= 0)
		ntyout(tp->t_local&LCRTERA ? "\b \b" : "\b", tp);
}

/*
 * Reprint the rawq line.
 * We assume c_cc has already been checked.
 */
static ntyretype(tp)
register struct tty *tp;
{
	register char *cp;
	char *nextc();
	register s;

	if (tlun.t_rprntc != (-1))
		ntyecho(tlun.t_rprntc, tp);
	(void) ntyoutput('\n', tp);
	s = spl5();
	for (cp = tp->t_canq.c_cf; cp; cp = nextc(&tp->t_canq, cp))
		ntyecho(lookc(cp), tp);
	for (cp = tp->t_rawq.c_cf; cp; cp = nextc(&tp->t_rawq, cp))
		ntyecho(lookc(cp), tp);
	tp->t_lstate &= ~LSERASE;
	splx(s);
	tp->t_rocount = tp->t_rawq.c_cc;
	tp->t_rocol = 0;
}

/*
 * Echo a typed character to the terminal
 */
static ntyecho(c, tp)
register c;
register struct tty *tp;
{

	if ((tp->t_lstate & LSCNTTB) == 0)
		tp->t_local &= ~LFLUSHO;
	if ((tp->t_flags&ECHO) == 0)
		return;
	c &= 0377;
	if (tp->t_flags&RAW) {
		(void) ntyoutput(c, tp);
		return;
	}
	if (c == '\r' && tp->t_flags&CRMOD)
		c = '\n';
	if (tp->t_local&LCTLECH) {
		if ((c&0177) <= 037 && c!='\t' && c!='\n' || c==0177 || c==0377) {
			tp->t_lstate |= LOREG;
			(void) ntyoutput('^', tp);
			c &= 0177;
			if (c == 0177)
				c = '?';
			else if (tp->t_flags&LCASE &&
				(('a'&037)<=c && c<=('z'&037)))
				c += 'a' - 1;
			else
				c += 'A' - 1;
		}
	}
	/* Convert upper case to lower case if input style with \ */
	if((tp->t_flags&(LCASE|UCASE|SISO))==(LCASE|UCASE)) {
		if (('A'<=c && c<='Z') || (c&0340)==0340)
			c ^= 040;
	}
	if ( (c&0300) == 0200) c &= 0177;
#ifdef Ucode
	(void) ntyoutput(conv_in(c), tp);
#else
	(void) ntyoutput(c, tp);
#endif Ucode
}

/*
 * Is c a break char for tp?
 */
static ntbreakc(c, tp)
register c;
register struct tty *tp;
{
	return (c == '\n' || c == tun.t_eofc || c == tun.t_brkc ||
		c == '\r' && (tp->t_flags&CRMOD));
}

/*
 * send string cp to tp
 */
static ntyout(cp, tp)
register char *cp;
register struct tty *tp;
{
	register char c;

	while (c = *cp++)
#ifdef Ucode
		(void) ntyoutput(conv_in(c), tp);
#else
		(void) ntyoutput(c, tp);
#endif
}

ttwakeup(tp)
register struct tty *tp;
{
	if (tp->t_rsel) {
		selwakeup(tp->t_rsel, tp->t_state&TS_RCOLL);
		tp->t_state &= ~TS_RCOLL;
		tp->t_rsel = 0;
	}
	wakeup((caddr_t)&tp->t_rawq);
}

#ifdef UCB_NET
ttsignal(tp, signo)
	struct tty *tp;
	int signo;
{

	gsignal(tp->t_pgrp, signo);
}
#endif  UCB_NET
