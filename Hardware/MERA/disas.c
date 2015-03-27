typedef struct {
  char *opcode;
  char o1, o2;
  char fl;
} ins_elem;

#include "disas.h"

#include <stdio.h>

struct {
	int lolb:4;
	int holb:4;
};
static int imop;
int ltbl[1000];
int tbl_free =0;
char ltype[1000];
FILE *inf;


main( argc, argv )
int argc;
char *argv[]; {
int as_form = 0;
int an;

for( an = 1; an <= argc; an++ ){
  if( *argv[an] == '-' )
    as_form = (argv[an][1] == 'a');
  else break;
}

if( !(inf = fopen( argv[an], "r" )) ){
  printf(" �� ���� ������� ����!\n");
  exit();
}
pass(0, as_form);
fseek(inf, 0l, 0);
pass(1, as_form);
}


pass( pn, as_form )
int pn, as_form; {
int c, l, adr, an;
ins_elem *p;

adr = 0;
while( (c = getc( inf )) != EOF ){
  p = & ins_tbl[c.holb][c.lolb];
  if( !as_form && pn ) printf("%04x    %02x ", adr, c);
  l = ((p->o1 == '#' || p->o2 == '#') ? 1 :
      (p->o1 == '&' || p->o1 == '*' || p->o2 == '&' || p->o2 == '*' ) ? 2 : 0);
  switch( l ) {
  case 1: imop = getc( inf );
	  if( !as_form && pn)printf("%02x    ", imop);
	  break;
  case 2: imop = getc( inf );
	  imop |= getc( inf )<<8;
	  if( !as_form && pn )printf( "%04x  ", imop );
	  break;
  case 0: if( !as_form && pn )printf( "      " );
  }
  /* ����� ��������� */
  if( pn ){
    prlab( adr );
    printf( "      %s  ",p->opcode );
  }
  put_ope( pn, p->o1 ,p->fl );
  if( p->o1 && p->o2 && pn )printf( "," );
  put_ope( pn, p->o2, p->fl );
  if( l == 1 && pn )
    if( imop >= 040 && imop < 0177 )printf( "  ; %c",imop );
    else printf( "  ; %03o",imop );
  if( pn )printf( "\n" );
  adr += (l+1);
  if( p->fl == 1 )put_tbl( adr, 'X' );
}
}

put_ope( pn, ope, flag )
int ope, pn, flag; {
int i;


if( pn )
  switch( ope ) {
  case '&':
  case '*': if((i = prlab1(imop)) == EOF)
	      printf( "%05xh", imop );
	    break;
  case '#': printf( "%03xh", imop ); break;
  case 's': printf( "sp" ); break;
  case 'p': printf( "pc" ); break;
  case 'w': printf( "psw" ); break;
  case   0: break;
  default:  putchar( ope );
  }
else
 if( ope == '*' && flag )put_tbl( imop, 'L' );
 else
   if( (ope == '&' || ( !flag && ope == '*' )) && imop < 04000 && imop > 0 )
     put_tbl( imop, 'D' );
}

put_tbl( im, type )
int im; char type; {
register int i;

  for ( i = 0; i < tbl_free; i++)
    if( ltbl[i] == im )
      if( ltype[i] != 'X' )return;
      else { ltype[ i ] = type; ltbl[ i ] = im; }
  ltype[ tbl_free ] = type; ltbl[tbl_free++] = im;
}

prlab1(im)
int im; {
register int i;

for ( i = 0; i < tbl_free; i++)
  if( ltbl[i] == im ){
    printf("%c%04d",ltype[i], i);
    return( i );
  }
return(EOF);
}

prlab(ad)
int ad; {
static unsigned i = 32767;
static int j = 0;
register int ij;
if(ad < i && i != 32767) { printf("      "); return; }
if (ad == i) printf("%c%04d:", ltype[j], j); else printf("      ");
for((i = 32767, ij = 0); ij < tbl_free; ij++)
  if(ltbl[ij] > ad && ltbl[ij] < i) { i = ltbl[ij]; j = ij; }
}
