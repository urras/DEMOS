#define NUM 32
#include <stdio.h>
struct {
  int lhb:4;
  int hhb:4;
};

FILE *inf, *outf;
char *p;
char buf[80];

main( argc, argv )
int argc;
char ** argv; {
int an, i;

  for( an = 1; an < argc; an++ )
    if( *argv[an] != '-' )break;

  if( !(inf = fopen( argv[an], "r" )) )
    printf( "Не могу открыть входной файл!\n" );
  if( !(outf = fopen( argv[an+1], "w" )) )
    printf( "Не могу открыть выходной файл!\n" );
  while ( fgets( buf, 80, inf ) ){
    printf( "%s", buf );
    p = buf + 5;
    for( i = 1; i <= NUM; i++){
	int ii;
      ii =  conv_byte( p ) & 0377;
      putc( ii, outf );
      putc( 0377, outf );
      p+=2;
    }
  }
  fclose( outf );
}

conv_byte( p )
char *p; {
int r;
int j;
  j = *p;
  r.hhb = ((j >= '0' && j <= '9') ? (j - '0') :
	  (j >= 'a' && j <= 'f') ? (j - 'a' + 10) : 0);
  j  = *(p+1);
  r.lhb = ((j >= '0' && j <= '9') ? (j - '0') :
	  (j >= 'a' && j <= 'f') ? (j - 'a' + 10) : 0);
  return( r & 0377 );
}
