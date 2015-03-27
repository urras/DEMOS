/* The name of SENDMAIL delivery system */
#ifndef MSDOS
#ifdef DEMOS
char    SENDMAIL[] = "/usr/local/lib/sendmail";
#else
char    SENDMAIL[] = "/usr/lib/sendmail";
#endif
#else
char    SENDMAIL[] = "sendmail.exe";
#endif
