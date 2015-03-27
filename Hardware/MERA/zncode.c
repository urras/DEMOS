#include <stdio.h>

FILE *inf, *outf;
char em[] = "Не могу открыть входной файл!";
char em1[] = "Не могу открыть выходной файл!";
char buf[80];

main( argc, argv )
int argc;
char *argv[]; {
int an, i, j;
char byte, bit;

  for( an = 1; an <= argc; an++ )
    if( *argv[an] != '-' )break;
  if( !(inf = fopen( argv[an], "r" )) ){ printf( em ); exit(); }
  if( !(outf = fopen( argv[an+1], "w" )) ){ printf( em1 ); exit(); }

  bit = 01;
  while( fgets( buf, 80, inf ) ){
    for( i = 1; i <= 8; i++ ){
      fgets( buf, 80, inf );
      byte = 0;
      for( j = 0; j <= 7; j++ ){
	if( buf[j] != '*' )byte |= bit;
	if( j != 7 )byte <<= 1;
      }
      putc( byte, outf );
      putc( 0377, outf );
    }
    fgets( buf, 80, inf );
  }
  fclose( outf );
}
