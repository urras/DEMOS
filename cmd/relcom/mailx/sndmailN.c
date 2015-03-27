/* The name of SENDMAIL delivery system */
#ifndef MSDOS
char    SENDMAIL[] = "/usr/lib/sendmail";
#else
char    SENDMAIL[] = "sendmail.exe";
#endif
