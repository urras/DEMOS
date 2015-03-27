#include <stdio.h>
#include <ctype.h>

#define Хэш_константа 307
#define NPROD         300

#define BMAGIC  'b'
#define EMAGIC  'e'

struct   П_СПИСОК {
	struct   П_СПИСОК *след;
	char              *буф;
	int                строка;
	int                прод;
};

struct   П_СПИСОК *Хэш_таб[Хэш_константа];

int                сс_прод[NPROD];

int      н_прод;
int      н_стр;
char     прод_буф[BUFSIZ];
char     и_файла[100];
int      прод_длина;

char    *чт_буф();
char    *alloc();

main()
{
	register         int       i;
	register struct  П_СПИСОК *сп;

	for( i = 0; i < Хэш_константа; i++ ){
		Хэш_таб[i] = NULL;
	}
	for( i = 0; i < NPROD; i++ ){
		сс_прод[i] = -1;
	}
	while( чт_с_зан() )
		;
	for( i = 0; i < Хэш_константа; i++ ){
		if( ( сп = Хэш_таб[i] ) == NULL ){
			continue;
		}
		for( ; сп != NULL; сп = сп->след ){
			печать( сп );
		}
	}
}

чт_с_зан()
{
	register         int       у;
	register struct  П_СПИСОК *сп;

	if( ( у = чт_прод() ) == -1 ){
		return( 0 );
	}
	if( ( сп = Хэш_таб[у] ) != NULL ){
		for( ; strcmp( прод_буф, сп->буф ) != 0; сп = сп->след ){
			if( сп->след == NULL ){
				сп->след = (struct П_СПИСОК *)alloc( sizeof( struct П_СПИСОК ) );
				if( ( сп = сп->след ) == NULL || ( сп->буф = alloc( прод_длина ) ) == NULL ){
					if( сп != NULL ){
						/*free( сп );*/
						сп = NULL;
					}
					отказ( "Нет памяти 1" );
					exit( 1 );
				}
				strcpy( сп->буф, прод_буф );
				сп->строка = н_стр;
				сп->прод = н_прод;
				return( 1 );
			}
		}
	}else{
		Хэш_таб[у] = (struct П_СПИСОК *)alloc( sizeof( struct П_СПИСОК ) );
		if( ( сп = Хэш_таб[у] ) == NULL || ( сп->буф = alloc( прод_длина ) ) == NULL ){
			if( сп != NULL ){
				/*free( сп );*/
				сп = NULL;
			}
			отказ( "Нет памяти 2" );
			exit( 1 );
		}
		strcpy( сп->буф, прод_буф );
		сп->строка = н_стр;
		сп->прод = н_прод;
		return( 1 );
	}
	for( у = сп->прод; сс_прод[у] != -1; у = сс_прод[у] )
		;
	сс_прод[у] = н_прод;
}

чт_прод()
{
	register int     i;
	register int     s;
	register int    *p;
	register char   *к;

	if( ( к = чт_буф() ) == NULL ){
		return( -1 );
	}
	i = ( к - прод_буф + 1 )/2;
	p = (int*)прод_буф;
	s = 0;
	do{
		s += *p++;
	}while( --i > 0 );
	прод_длинна = к - прод_буф;

	return( s&Хэш_константа );
}

char *чт_буф()
{
	register char   *б_у = прод_буф;
	register char   *у;
		 char    буффер[BUFSIZ];
	register char   *буф = буффер;
		 int     в_строке = 0;
		 int     нп;

	do{
		if( fgets( буф, BUFSIZ-2, stdin ) == NULL ){
			if( ferror( stdin ) ){
				вв_отказ();
				exit( 1 );
			}
			return( NULL );
		}
	}while( *буф != BMAGIC );

	if( sscanf( &буф[1], "%d %d '%[^']'\n", &н_прод, &н_стр, и_файла ) == 0 ){
		вв_отказ();
		exit( 1 );
	}

	while( fgets( буф, sizeof буффер, stdin ) != NULL && *буф != EMAGIC ){
		у = &буф[1];
		while( isspace( *у ) && *у && !в_строке ){
			у++;
		}
		while( *у ){
			if( isspace( *у ) && !в_строке ){
				while( isspace( *у ) && *у != '\n' ){
					у++;
				}
				*б_у++ = ( *у == '\n' ? *у++ : ' ' );
				continue;
			}
			if( *у == '"' ){
				в_строке = !в_строке;
			}
			*б_у++ = *у++;
		}
	}
	*б_у   = '\0';
	б_у[1] = '\0';

	while( *буф != EMAGIC ){
		if( fgets( буф, BUFSIZ-2, stdin ) == NULL ){
			вв_отказ();
			exit( 1 );
		}
	};

	if( sscanf( &буф[1], "%d\n", &нп ) == 0 || нп != н_прод ){
		вв_отказ();
		exit( 1 );
	}
	return( б_у );
}

печать( п )
struct   П_СПИСОК *п;
{
	register int     i;
	register char   *p;

	for( i = п->прод; i != -1; i = сс_прод[i] ){
		printf( "case %d:\n", i );
	}
	printf( "# line %d \"%s\"\n", п->строка, и_файла );
	p = п->буф;
	while( *p ){
		putchar( *p );
		p++;
	}
	putchar( '\n' );
}

вв_отказ()
{
	отказ( "Ошибка ввода" );
}

отказ( с )
char    *с;
{
	fprintf( stderr, "ОТКАЗ : %s\n", с );
	exit( 1 );
}

char *alloc( n )
int      n;
{
	register char   *p;

	p = sbrk( n );
	return( n == -1 ? NULL : p );
}
