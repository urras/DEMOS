#include <stdio.h>
#include <ctype.h>

#define ���_��������� 307
#define NPROD         300

#define BMAGIC  'b'
#define EMAGIC  'e'

struct   �_������ {
	struct   �_������ *����;
	char              *���;
	int                ������;
	int                ����;
};

struct   �_������ *���_���[���_���������];

int                ��_����[NPROD];

int      �_����;
int      �_���;
char     ����_���[BUFSIZ];
char     �_�����[100];
int      ����_�����;

char    *��_���();
char    *alloc();

main()
{
	register         int       i;
	register struct  �_������ *��;

	for( i = 0; i < ���_���������; i++ ){
		���_���[i] = NULL;
	}
	for( i = 0; i < NPROD; i++ ){
		��_����[i] = -1;
	}
	while( ��_�_���() )
		;
	for( i = 0; i < ���_���������; i++ ){
		if( ( �� = ���_���[i] ) == NULL ){
			continue;
		}
		for( ; �� != NULL; �� = ��->���� ){
			������( �� );
		}
	}
}

��_�_���()
{
	register         int       �;
	register struct  �_������ *��;

	if( ( � = ��_����() ) == -1 ){
		return( 0 );
	}
	if( ( �� = ���_���[�] ) != NULL ){
		for( ; strcmp( ����_���, ��->��� ) != 0; �� = ��->���� ){
			if( ��->���� == NULL ){
				��->���� = (struct �_������ *)alloc( sizeof( struct �_������ ) );
				if( ( �� = ��->���� ) == NULL || ( ��->��� = alloc( ����_����� ) ) == NULL ){
					if( �� != NULL ){
						/*free( �� );*/
						�� = NULL;
					}
					�����( "��� ������ 1" );
					exit( 1 );
				}
				strcpy( ��->���, ����_��� );
				��->������ = �_���;
				��->���� = �_����;
				return( 1 );
			}
		}
	}else{
		���_���[�] = (struct �_������ *)alloc( sizeof( struct �_������ ) );
		if( ( �� = ���_���[�] ) == NULL || ( ��->��� = alloc( ����_����� ) ) == NULL ){
			if( �� != NULL ){
				/*free( �� );*/
				�� = NULL;
			}
			�����( "��� ������ 2" );
			exit( 1 );
		}
		strcpy( ��->���, ����_��� );
		��->������ = �_���;
		��->���� = �_����;
		return( 1 );
	}
	for( � = ��->����; ��_����[�] != -1; � = ��_����[�] )
		;
	��_����[�] = �_����;
}

��_����()
{
	register int     i;
	register int     s;
	register int    *p;
	register char   *�;

	if( ( � = ��_���() ) == NULL ){
		return( -1 );
	}
	i = ( � - ����_��� + 1 )/2;
	p = (int*)����_���;
	s = 0;
	do{
		s += *p++;
	}while( --i > 0 );
	����_������ = � - ����_���;

	return( s&���_��������� );
}

char *��_���()
{
	register char   *�_� = ����_���;
	register char   *�;
		 char    ������[BUFSIZ];
	register char   *��� = ������;
		 int     �_������ = 0;
		 int     ��;

	do{
		if( fgets( ���, BUFSIZ-2, stdin ) == NULL ){
			if( ferror( stdin ) ){
				��_�����();
				exit( 1 );
			}
			return( NULL );
		}
	}while( *��� != BMAGIC );

	if( sscanf( &���[1], "%d %d '%[^']'\n", &�_����, &�_���, �_����� ) == 0 ){
		��_�����();
		exit( 1 );
	}

	while( fgets( ���, sizeof ������, stdin ) != NULL && *��� != EMAGIC ){
		� = &���[1];
		while( isspace( *� ) && *� && !�_������ ){
			�++;
		}
		while( *� ){
			if( isspace( *� ) && !�_������ ){
				while( isspace( *� ) && *� != '\n' ){
					�++;
				}
				*�_�++ = ( *� == '\n' ? *�++ : ' ' );
				continue;
			}
			if( *� == '"' ){
				�_������ = !�_������;
			}
			*�_�++ = *�++;
		}
	}
	*�_�   = '\0';
	�_�[1] = '\0';

	while( *��� != EMAGIC ){
		if( fgets( ���, BUFSIZ-2, stdin ) == NULL ){
			��_�����();
			exit( 1 );
		}
	};

	if( sscanf( &���[1], "%d\n", &�� ) == 0 || �� != �_���� ){
		��_�����();
		exit( 1 );
	}
	return( �_� );
}

������( � )
struct   �_������ *�;
{
	register int     i;
	register char   *p;

	for( i = �->����; i != -1; i = ��_����[i] ){
		printf( "case %d:\n", i );
	}
	printf( "# line %d \"%s\"\n", �->������, �_����� );
	p = �->���;
	while( *p ){
		putchar( *p );
		p++;
	}
	putchar( '\n' );
}

��_�����()
{
	�����( "������ �����" );
}

�����( � )
char    *�;
{
	fprintf( stderr, "����� : %s\n", � );
	exit( 1 );
}

char *alloc( n )
int      n;
{
	register char   *p;

	p = sbrk( n );
	return( n == -1 ? NULL : p );
}
