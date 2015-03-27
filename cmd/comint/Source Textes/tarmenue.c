#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/dir.h>    /* for DIRSIZ only */

#define NAMLEN          100     /* From tar */
#define BUFLEN          1024*32
#define NLINES          256


char buf [ BUFLEN];     /* Dynamic in future ... */
char *lines[NLINES];
char name[NAMLEN-DIRSIZ]="";

int  nline=0;
char *compare();
char *fgetss();

main(argc, argv) char *argv[];
{
        register char *p, *n;
        register int   i;
        int pipdes[2], pid, rc, j;

        /*
         * Parse arguments
         *
         * ERROR messages will be shown by comint
         * as header in empty menue
         */
        switch(argc) {
        case 3:
                /*
           * Search path will be taken from tape (zero level)
           */
                if( strlen(argv[2]) >= sizeof name ) {
                        printf("/tumpn ERROR ! Too long patname  \n");
                        exit(1);
                }
                strcpy(name,argv[2]);
        case 2:
                /*
           * Search path will be taken from tape (zero level)
           */
                break;
        case 1:
                printf("/tumpn ERROR ! Tape name required\n");
                exit(1);
        default:
                printf("/tumpn ERROR ! Too many arguments\n");
                exit(1);
        }

        /*
         * Fork and start tar
         */
        pipe(pipdes);
        if( (pid=fork()) == 0 ) {
                close(1);
                dup(pipdes[1]);
                execl("/bin/tar", "tar", "tf", argv[1], 0);
                execl("/usr/bin/tar", "tar", "tf", argv[1], 0);
                perror("tar failed");
                exit(1);
        }
        close(0);
        dup(pipdes[0]);

        /*
         * Read contains of TapeImage
         * Save it into buffer
         */
        p = &buf[0];
        while(
            p+NAMLEN < &buf[BUFLEN]
            &&  nline < NLINES
            &&  fgetss(p,NAMLEN,stdin)
            ) {
                lines[nline++] = p;
                p += strlen(p)+1;
        }
        printf("Point 1 \n\r"); fflush(stdout);
        lines[nline] = (char*)0;
        close(pipdes[0]);
        close(pipdes[1]);

        /*
         * Build a menue
         * as a sqnce of lines on stdout
         */
        printf("Starting a\n"); fflush(stdout);

        for( i=0; (p=lines[i]); i++ ) {

                printf(" --> %s\n", p);
                fflush(stdout);
                continue;

                if( p=compare(&name[0],p) ) {
                        while( *p && *p!='/' )
                                putchar(*p++);
                        putchar('\n');
                }
        }
}

/*
 * This function advances the pointer
 * while the string corresponds to the pattern.
 * Returns NULL if incorrespondence
 * obtained not on the end of pattern
 * Else (success) returns pointer to the
 * first incorrespondent character in tested string
 * (if it isn't a slash) or to the next one.
 */
char *compare( pattern, pointer ) register char *pattern, *pointer; 
{

        while( *pattern++ == *pointer++ )
                ;
        if( *--pattern != '\0' )        /* No consistance */
                return( (char *)0 );
        /* else */                      /* Complete pattern searched */
        return( *--pointer=='/' ? ++pointer : pointer );

}


char *fgetss(buf,len,file)
register char *buf;
register int len;
FILE *file;
{
        char *buf0 = buf;
        register int i;

        do {;} while( write(1,buf,read(0,buf,len)) > 0 );
        exit(1);
        while( (a(),i=getc(file)) > 0 && i != '\n' && len--) *buf++ = i;
        if ( len--) *buf++ = '\0';
        printf("returning %s\n", buf0);
        return(i<0?(char *)0:buf0);
}

a() {
        printf(" a... ");
        fflush(stdout);
}
