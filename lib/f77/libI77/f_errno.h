/*
char id_f_errno[] = "@(#)f_errno.h	1.5";
 *
 * f77 I/O error definitions
 */

#include	<errno.h>

extern int errno;
extern int f_nerr;

#define F_ER		100	/* base offset of f77 error numbers */

#define F_ERFMT		100	/* error in format */
#define F_ERUNIT	101	/* illegal unit number */
#define F_ERNOFIO	102	/* formatted io not allowed */
#define F_ERNOUIO	103	/* unformatted io not allowed */
#define F_ERNODIO	104	/* direct io not allowed */
#define F_ERNOSIO	105	/* sequential io not allowed */
#define F_ERNOBKSP	106	/* can't backspace file */
#define F_ENULLN        107     /* off beginning of record */
#define F_ERSTAT	108	/* can't stat file */
#define F_ERNOPEN1      109     /* no * after repeat count */
#define F_EREREC	110	/* off end of record */
#define F_ERTRUNC	111	/* truncation failed */
#define F_ERLIO		112	/* incomprehensible list input */
#define F_ERSPACE	113	/* out of free space */
#define F_ERNOPEN	114	/* unit not connected */
#define F_ERRDCHR	115	/* read unexpected character */
#define F_ERLOGIF	116	/* blank logical input field */
#define F_ERSYS         117     /* unknown system error */
#define F_ERSEEK        120     /* requires seek ability */
#define F_ERARG         121     /* illegal argument */
#define F_ERILLOP       122     /* illegal operation */

#define F_MAXERR	(f_nerr + F_ER)
