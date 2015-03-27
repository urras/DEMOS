/**
 ** MISS sublibrary archiver
 **/
#include <stdio.h>
#include <stdlib.h>         /* exit() */

/* Very machine-independent structure. I run it on Sparc. P3 */
struct item {
    char type;
    char prot;
    char diskaddr[2];
    char password[2];
    char info[8];
    char name[10];
    char fileproc[8];
};

int Usage();
void list( /* char* */ );
void extr( /* char* */ );
long fileoff( /* struct item * */ );
void extr1();
void makename();

int
main( argc, argv )
    int argc;
    char **argv;
{
    if( argc != 3 ) Usage();
    if( argv[1][0] == '-' ) argv[1]++;
    if( argv[1][0] == 't' ){
        list( argv[2] );
    }else if( argv[1][0] == 'x' ){
        extr( argv[2] );
    }else{
        Usage();
    }
    exit(0);
}

void list( arch )
    char *arch;
{
    FILE *af;
    struct item itemv[2];
    register struct item *curri = itemv, *previ = NULL;

    if( (af = fopen( arch, "r" )) == NULL ){
        fprintf( stderr, "Cannot open archive \"%s\"\n", arch );
        exit(1);
    }

    for(;;){
        if( fread( curri, sizeof(struct item), 1, af ) != 1 ){
            fprintf( stderr, "Corrupted archive\n" );
            exit(1);
        }
        if( previ != NULL ){
            previ->name[10] = 0;      /* Intervention into the fileproc */
            printf( "%8ld %c %10s\n", fileoff(curri) - fileoff(previ),
                previ->type & 0x7F, previ->name );
        }
        if( curri->type == 0 ) break;
        previ = curri;
        curri = (curri == itemv)? itemv+1: itemv;
    }

    fclose( af );
}

void extr( arch )
    char *arch;
{
    FILE *af, *df;
    struct item itemv[2];
    register struct item *curri = itemv, *previ = NULL;

    if( (af = fopen( arch, "r" )) == NULL ||
        (df = fopen( arch, "r" )) == NULL )
    {
        fprintf( stderr, "Cannot open archive \"%s\"\n", arch );
        exit(1);
    }

    for(;;){
        if( fread( curri, sizeof(struct item), 1, af ) != 1 ){
            fprintf( stderr, "Corrupted archive\n" );
            exit(1);
        }
        if( previ != NULL ){
            extr1( previ->name, df, fileoff( previ ), fileoff( curri ) );
        }
        if( curri->type == 0 ) break;
        previ = curri;
        curri = (curri == itemv)? itemv+1: itemv;
    }

    fclose( af );
    fclose( df );
}

long fileoff( ip )
    struct item *ip;
{
    register unsigned da, of;
    da = ip->diskaddr[1] & 0xFF;
    da |= ip->diskaddr[0] << 8;
    of = ip->password[1] & 0xFF;
    of |= ip->diskaddr[0] << 8;
    return (((long)da & 0xFFFF) << 8) + of;
}

void
extr1( miss_name, df, begin, end )
    char *miss_name;
    FILE *df;
    long begin, end;
{
    FILE *of;
    static char unix_name[11];
    static char buffer[ 0x2000 ];
    int len;
    long off = begin;

    makename( unix_name, miss_name );
    if( (of = fopen( unix_name, "w" )) == NULL ){
        fprintf( stderr, "Cannot create \"%s\"\n", unix_name );
        exit(1);
    }

    if( fseek( df, begin, 0 ) == -1 ){
        fprintf( stderr, "I/O out of archive\n" );
        exit(1);
    }
    for(;;){
        len = sizeof(buffer);
        if( end - off < sizeof(buffer) ) len = (int) (end - off);
        if( len <= 0 ) break;
        len = fread( buffer, 1, len, df );
        if( len == 0 ) break;
        if( fwrite( buffer, 1, len, of ) != len ){
            fprintf( stderr, "Write error\n" );
            exit(1);
        }
        off += len;
    }
    fclose( of );
}

void makename( un, mn )
    char *un, *mn;
{
    int i;
    register char c;
    for( i = 0; i < 10; i++ ){
        c = mn[i];
        if( c == ' ' ){
            c = '-';
        }else if( c == ':' ){
            c = '.';
        }else if( c == '!' ){
            c = '_';
        }else if( c & 0x80 ){
            c = c & 0x7F ^ 0x20;
        }
        un[i] = c;
    }
    un[10] = 0;
    for( i = 10; i != 0; ){
        --i;
        if( un[i] == '-' ) un[i] = 0;
    }
}

int Usage()
{
    fprintf( stderr, "Usage: uar [-]{tx} archive\n" );
    exit(1);
}
