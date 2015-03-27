#ifndef _strings_h_
#       define _strings_h_

/*
 * Внешние определения функций для
 * работы со строками см. string(3).
 */
char	*strcat();
char	*strncat();
int	strcmp();
int	strncmp();
int     strlcmp();
int     strnlcmp();
char	*strcpy();
char	*strncpy();
int	strlen();
char	*index();
char	*rindex();

#endif _strings_h_
