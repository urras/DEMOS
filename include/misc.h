#ifndef _misc_h_
#       define  _misc_h_

/*
 * structure to access an
 * integer in bytes
 */
struct
{
	char    lobyte;
	char    hibyte;
};

/*
 * structure to access an integer
 */
struct
{
	int     integ;
};

/*
 * structure to access a long as integers
 */
struct {
	int     hiword;
	int     loword;
};

/*
 *      structure to access an unsigned
 */
struct {
	unsigned        unsignd;
};

#endif  _misc_h_
