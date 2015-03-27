/*
 * Файл настроек расположения информации для системы электронной почты
 *
 * M - mailx
 * U - uued
 * R - uurtouter
 * S - sendmail
 * Еще есть настройки в конфигурациях sendmail
 */
#define XX_LOCALMAIL      "/usr/local/lib/relcom/localmail"
#define M_MAIL            XX_LOCALMAIL                       /* Name of mail sender */
#define M_HELPFILE        "/usr/local/lib/relcom/Mail.help"    /* Name of casual help file */
#define M_THELPFILE       "/usr/local/lib/relcom/Mail.help.~"  /* Name of casual tilde help */
#define M_RHELPFILE       "/usr/local/lib/relcom/Mail.rhelp"   /* Name of casual help file */
#define M_RTHELPFILE      "/usr/local/lib/relcom/Mail.rhelp.~" /* Name of casual tilde help */
#define M_MASTER          "/usr/local/lib/relcom/Mail.rc"
#define M_ALIASES         "/usr/local/lib/aliases"             /* Sendmail's aliases */
#define M_SENDMAIL        "/usr/local/lib/sendmail"
#define M_UUMAIL          "/usr/local/lib/relcom/uumail"
#ifdef DEMOS
#define M_VISUAL          "/bin/red"
#define M_MORE            "/usr/bin/more"
#else
#define M_VISUAL          "/usr/local/bin/red"
#define M_MORE            "/usr/local/bin/more"
#endif DEMOS
#define U_LOCAL           XX_LOCALMAIL
#define U_MAIL            "/usr/local/bin/mail"

#define R_UUMAIL          M_UUMAIL
#define R_SENDMAIL        M_SENDMAIL

#define R_ROUTES     "/usr/local/lib/relcom/uucp.routes" /* File containing MAN-mode table */
#define R_SYSTEMFILE "/usr/local/lib/relcom/Systems"    /* Systems file for AUTO mode */
#define R_SERVLIST   "/usr/local/lib/relcom/Services"   /* List of services */
#define R_SERVDIR    "/usr/local/lib/relcom"            /* Location of service utils */

#define R_SMAILER  "/usr/local/lib/sendmail -i"
#define R_SEMAILER "/usr/local/lib/sendmail -i"
#define R_NMAILER  XX_LOCALMAIL
#define R_ALIASES  M_ALIASES
#define R_BATCH    "/usr/local/lib/relcom/batchmail"
#define R_STAT     "/usr/local/adm/mailstat"

#define S_SMCF     "/usr/local/lib/sendmail.cf"
#define S_SMFC     "/usr/local/lib/sendmail.fc"
