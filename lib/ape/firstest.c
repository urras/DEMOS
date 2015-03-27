#include <ape.h>
main()
{
	PMINT a,b;
	long int i;

	new(&a);
	new(&b);
	for (i=100000; i < 2000000; i += 100000) {
		makemint(a,i);
		mout(a); putchar('\n');
		/*square(a,b);
		mout(b); putchar('\n');*/
		mult(a,a,b);
		mout(b); putchar('\n');
		putchar('\n');
		}
}
