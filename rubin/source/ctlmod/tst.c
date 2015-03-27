# include	<defines.h>
# include	<aux.h>
# include	<tree.h>
# include	<symbol.h>
# include	<sccs.h>

main()
{ struct symbol ss;
  printf("%o %o %o %o\n",&ss, &ss.type, &ss.len, &ss.value);
}
