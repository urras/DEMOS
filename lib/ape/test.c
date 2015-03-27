#include <ape.h>
main()
{
	PMINT a,b;
	int i;

	a = itom(2);
	new(&b);
	for (i=1; i < 11; ++i) {
		square(a,b);
		mout(a); putchar('\t'); mout(b); putchar('\n');
		move(b,a); xfree(b);
		}
}
