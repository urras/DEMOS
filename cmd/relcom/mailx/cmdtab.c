#include "def.h"
#include "local.h"

/*
 * Mail -- a mail program
 *
 * Define all of the command names and bindings.
 *
 * $Log: cmdtab.c,v $
 * Revision 1.10  1991/07/19  20:00:14  ache
 * dOBAWLENA RASSYLKA SOOB]ENIJ W TELEKONFERENCII
 *
 * Revision 1.9  1990/12/20  20:30:40  ache
 * top PERENESENA WY[E (NUVNEE, ^EM type)
 *
 * Revision 1.8  90/10/04  03:40:01  ache
 * list -- first 'l' command.
 * 
 * Revision 1.7  90/09/29  17:23:10  ache
 * Added lpr,Lpr...
 * 
 * Revision 1.6  90/09/21  21:58:12  ache
 * MS-DOS extends + some new stuff
 * 
 * Revision 1.5  90/08/16  17:31:46  avg
 * dOBAWLENA KOMANDA forward.
 * 
 * Revision 1.4  90/06/10  00:42:13  avg
 * dOBAWLENY KOMANDY @Reply @reply @from @headers.
 * 
 * Revision 1.3  90/05/31  19:45:40  avg
 * pOMENQLISX MESTAMI KOMANDY reply I Reply. dOBAWLENA KOMANDA headline.
 * 
 * Revision 1.2  88/01/11  12:23:36  avg
 * dOBAWLENY KUSKI DLQ RABOTY S PMCS.
 * u rcsid POSTAWLENY KOMMENTARII NOXSTR.
 * 
 * Revision 1.1  87/12/25  15:58:43  avg
 * Initial revision
 *
 */

/*NOXSTR
static char rcsid[] = "$Header: /usr/src/Relcom/relcom/mailx/RCS/cmdtab.c,v 1.10 1991/07/19 20:00:14 ache Exp $";
YESXSTR*/

extern int type(), preserve(), delete(), undelete(), next(), shell(), schdir();
extern int save(), help(), headers(), pdot(), strace(), respond(), editor();
extern int edstop(), rexit(), pcmdlist(), sendmail(), from(), copycmd();
extern int messize(), psalloc(), deltype(), unset(), set(), source();
extern int group(), top(), core(), null(), stouch(), visual(), lpr(), Lpr();
extern int swrite(), dosh(), file(), echo(), Respond(), scroll(), ifcmd();
extern int elsecmd(), endifcmd(), mboxit(), clobber(), alternates();
extern int local(), folders(), igfield(), Type(), headline(), decode();
extern int XRespond(), Xrespond(), Xheaders(), Xfrom(), Forward(), submit();
#ifdef PMCS
extern int projects();
#endif

struct cmd cmdtab[] = {
	"next",         next,           NDMLIST,        0,      MMNDEL,
	"alias",        group,          M|RAWLIST,      0,      1000,
	"print",        type,           MSGLIST,        0,      MMNDEL,
	"Print",        Type,           MSGLIST,        0,      MMNDEL,
	"top",          top,            MSGLIST,        0,      MMNDEL,
	"type",         type,           MSGLIST,        0,      MMNDEL,
	"Type",         Type,           MSGLIST,        0,      MMNDEL,
	"list",         pcmdlist,       M|NOLIST,       0,      0,
	"lpr",          lpr,            MSGLIST,        0,      MMNDEL,
	"Lpr",          Lpr,            MSGLIST,        0,      MMNDEL,
	"visual",       visual,         I|MSGLIST,      0,      MMNORM,
	"touch",        stouch,         W|MSGLIST,      0,      MMNDEL,
	"preserve",     preserve,       W|MSGLIST,      0,      MMNDEL,
	"delete",       delete,         W|P|MSGLIST,    0,      MMNDEL,
	"dp",           deltype,        W|MSGLIST,      0,      MMNDEL,
	"dt",           deltype,        W|MSGLIST,      0,      MMNDEL,
	"undelete",     undelete,       P|MSGLIST,      MDELETED,MMNDEL,
	"unset",        unset,          M|RAWLIST,      1,      1000,
	"mail",         sendmail,       R|M|I|STRLIST,  0,      0,
	"mbox",         mboxit,         W|MSGLIST,      0,      0,
	"!",            shell,          I|STRLIST,      0,      0,
	"copy",         copycmd,        M|STRLIST,      0,      0,
	"chdir",        schdir,         M|STRLIST,      0,      0,
	"cd",           schdir,         M|STRLIST,      0,      0,
	"save",         save,           STRLIST,        0,      0,
	"source",       source,         M|STRLIST,      0,      0,
	"set",          set,            M|RAWLIST,      0,      1000,
	"shell",        dosh,           I|NOLIST,       0,      0,
	"group",        group,          M|RAWLIST,      0,      1000,
	"write",        swrite,         STRLIST,        0,      0,
	"forward",      Forward,        R|M|I|STRLIST,  0,      0,
	"from",         from,           MSGLIST,        0,      MMNORM,
	"@from",        Xfrom,          MSGLIST,        0,      MMNORM,
	"file",         file,           T|M|RAWLIST,    0,      1,
	"folder",       file,           T|M|RAWLIST,    0,      1,
	"folders",      folders,        T|M|RAWLIST,    0,      1,
#ifdef PMCS
	"projects",     projects,       T|M|RAWLIST,    0,      1,
#endif
	"?",            help,           M|NOLIST,       0,      0,
	"z",            scroll,         M|STRLIST,      0,      0,
	"headers",      headers,        MSGLIST,        0,      MMNDEL,
	"@headers",     Xheaders,       MSGLIST,        0,      MMNDEL,
	"help",         help,           M|NOLIST,       0,      0,
	"=",            pdot,           NOLIST,         0,      0,
	"Reply",        Respond,        R|I|MSGLIST,    0,      MMNDEL,
	"Respond",      Respond,        R|I|MSGLIST,    0,      MMNDEL,
	"reply",        respond,        R|I|MSGLIST,    0,      MMNDEL,
	"respond",      respond,        R|I|MSGLIST,    0,      MMNDEL,
	"@Reply",       XRespond,       R|I|MSGLIST,    0,      MMNDEL,
	"@Respond",     XRespond,       R|I|MSGLIST,    0,      MMNDEL,
	"@reply",       Xrespond,       R|I|MSGLIST,    0,      MMNDEL,
	"@respond",     Xrespond,       R|I|MSGLIST,    0,      MMNDEL,
	"edit",         editor,         I|MSGLIST,      0,      MMNORM,
	"echo",         echo,           M|RAWLIST,      0,      1000,
	"quit",         edstop,         NOLIST,         0,      0,
	"local",        local,          M|RAWLIST,      0,      1000,
	"xit",          rexit,          M|NOLIST,       0,      0,
	"exit",         rexit,          M|NOLIST,       0,      0,
	"size",         messize,        MSGLIST,        0,      MMNDEL,
	"hold",         preserve,       W|MSGLIST,      0,      MMNDEL,
	"if",           ifcmd,          F|M|RAWLIST,    1,      1,
	"else",         elsecmd,        F|M|RAWLIST,    0,      0,
	"endif",        endifcmd,       F|M|RAWLIST,    0,      0,
	"alternates",   alternates,     M|RAWLIST,      0,      1000,
	"ignore",       igfield,        M|RAWLIST,      0,      1000,
	"discard",      igfield,        M|RAWLIST,      0,      1000,
	"core",         core,           M|NOLIST,       0,      0,
	"#",            null,           M|NOLIST,       0,      0,
	"clobber",      clobber,        M|RAWLIST,      0,      1,
	"headline", 	headline,	M|RAWLIST,	0,	1000,
	"uudecode",	decode,		MSGLIST,	0,	MMNDEL,
	"submit",       submit,         M|STRLIST,      0,      0,
	0,              0,              0,              0,      0
};
