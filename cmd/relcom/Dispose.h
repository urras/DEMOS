/*
 *
 * M - mailx
 * U - uued
 * R - uurtouter
 * S - sendmail
 */
#define XX_LOCALMAIL      "/usr/lib/relcom/localmail"
#define M_MAIL            XX_LOCALMAIL                /* Name of mail sender */
#define M_HELPFILE  "/usr/lib/relcom/Mail.help"   /* Name of casual help file */
#define M_THELPFILE "/usr/lib/relcom/Mail.help.~" /* Name of casual tilde help*/
#define M_RHELPFILE "/usr/lib/relcom/Mail.rhelp"  /* Name of casual help file */
#define M_RTHELPFILE "/usr/lib/relcom/Mail.rhelp.~" /*Name of ca-l tilde help */
#define M_MASTER          "/usr/lib/relcom/Mail.rc"
#define M_ALIASES         "/usr/lib/aliases"        /* Sendmail's aliases */
#define M_SENDMAIL        "/usr/lib/sendmail"
#define M_UUMAIL          "/usr/lib/relcom/uumail"
#define M_VISUAL          "/usr/local/bin/re"
#define M_MORE            "/usr/local/bin/more"
#define U_LOCAL           XX_LOCALMAIL
#define U_MAIL            "/usr/bin/mail"

#define R_UUMAIL          M_UUMAIL
#define R_SENDMAIL        M_SENDMAIL

#define R_ROUTES "/usr/lib/relcom/uucp.routes" /*File containing MAN-mode tbl */
#define R_SYSTEMFILE "/usr/lib/relcom/Systems"  /* Systems file for AUTO mode */
#define R_SERVLIST   "/usr/lib/relcom/Services" /* List of services */
#define R_SERVDIR    "/usr/lib/relcom"          /* Location of service utils */

#define R_SMAILER  "/usr/lib/sendmail -i"
#define R_SEMAILER "/usr/lib/sendmail -i"
#define R_NMAILER  XX_LOCALMAIL
#define R_ALIASES  M_ALIASES
#define R_BATCH    "/usr/lib/relcom/batchmail"
#define R_STAT     "/usr/adm/mailstat"

#define S_SMCF     "/usr/lib/sendmail.cf"
#define S_SMFC     "/usr/lib/sendmail.fc"
