#include <stdio.h>

/*
 * Печать информации о типе ЭВМ
 * который подразумевается в CPP
 */
main()
{
printf ("Creation SUBD FOR:");
#ifdef pdp11
printf("PDP11|Elektronica-79\n");
#endif
#ifdef vax
#ifdef ultrix
printf("VAX COMPATIBLE/ ULTRIX\n");
#else
printf("VAX COMPATIBLE\n");
#endif
#endif
#ifdef mc68
printf("Motorola 68000 COMPAT\n");
#endif

#ifdef M_XENIX
printf("Microsoft Xenix\n");
#endif
#if defined(M_XENIX) && defined(M_I386)
printf("Xenix/386: use CPU=x386\n");
#else
#if defined(M_XENIX) && defined(M_I86SM)
printf("Xenix/286: use CPU=xenix\n");
#endif
#endif

#ifdef i386
printf("386/ix OS Interactive Corporation\n");
#endif

exit(0);
}
