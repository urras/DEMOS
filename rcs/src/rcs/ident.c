/*
 *                     RCS identification operation
 */
 static char rcsid[] =
 "$Header: /usr/users/korotay/DEMOS_2.2/rcs/src/rcs/RCS/ident.c,v 1.3 1986/02/23 22:12:46 avg Exp $";
/*****************************************************************************
 *****************************************************************************
 */

/* $Log: ident.c,v $
 * Revision 1.3  1986/02/23  22:12:46  avg
 * ������� ��������� ��� ���� � ��� ������ � U-�����.
 *
 * Revision 1.1  85/12/26  21:55:42  fox
 * ������� �������� ������ ������
 * 
*/

#include  "rcsbase.h"
#define fflsbuf _flsbuf
/* redefinition of _flsbuf in putc not needed */

main(argc, argv)
int  argc; char  *argv[];
/*  Ident searches the named files for all occurrences
 *  of the pattern $keyword:...$, where the keywords are
 *  Author, Date, Header, Log, Revision, Source, and State.
 */

{
   FILE *fp, *fopen();
   register int c;
   register int matchcount;

   if (argc<2) {
        fprintf(stderr,  "ident error: no input file\n");
        exit(1);
   }
   while ( --argc > 0 ) {
      if ( (fp = fopen(*++argv, "r") ) == NULL ) {
         fprintf(stderr,  "ident error: can't open %s\n", *argv);
         continue;
      } else {
         matchcount = 0;
         printf( "%s:\n", *argv);   /*  print file name  */
         while( (c=getc(fp)) != EOF) {
            if ( (char)c==KDELIM)
               matchcount += match(fp);
         }
         if (matchcount == 0)
            fprintf(stderr, "ident warning: no id keywords in %s\n", *argv);
         if (argc>1) putchar('\n');
      }
      fclose(fp);
   }
}

match(fp)   /* group substring between two KDELIM's; then do pattern match */
FILE *fp;

{
   char line[keyvallength];
   register int c;
   register char * tp;

   tp = line;
   while( (c = getc(fp)) != KDELIM ) {
      *tp++ = c;
      if ( c==EOF || c=='\n' || tp>= line+keyvallength-2)
         return(0);
      }
   *tp++=c;     /*append trailing KDELIM*/
   *tp='\0';
   if (isprefix(line, AUTHOR))
       ;
   else if ( isprefix(line,DATE) )
       ;
   else if ( isprefix(line, HEADER) )
       ;
   else if ( isprefix(line, LOCKER) )
       ;
   else if ( isprefix(line, LOG) )
       ;
   else if (isprefix(line, REVISION) )
       ;
   else if (isprefix(line, SOURCE) )
       ;
   else if ( isprefix(line, STATE) )
       ;
   else {
      /* no match; put trailing KDELIM back into input */
      ungetc(c,fp );
      return(0);
   }
   fprintf(stdout,"     $%s\n",line);
   return(1);
}


isprefix(s,t) /*  return true if t is prefix of s; false otherwise  */
char s[], t[];
{
   int  j, k;

   for (j=0, k=0; t[k] != '\0' && s[j] != '\0' && s[j] == t[k]; ++j,++k)
      ;
   if ( t[k] != '\0' )
      return(false);
   if (s[j] == VDELIM)
      return(true);
   return(false);
}
