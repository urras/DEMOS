/*
 *                     RCS rcsdiff operation
 */
 static char rcsid[]=
 "$Header: /usr/users/korotay/DEMOS_2.2/rcs/src/rcs/RCS/rcsdiff.c,v 1.3 1986/02/23 22:16:12 avg Exp $";
/*****************************************************************************
 *                       generate difference between RCS revisions
 *****************************************************************************
 */


/* $Log: rcsdiff.c,v $
 * Revision 1.3  1986/02/23  22:16:12  avg
 * ������� ��������� ��� ���� � ��� ������ � U-�����.
 *
 * Revision 1.1  85/12/26  22:03:21  fox
 * ������� �������� ������ ������
 * 
 */
#include "rcsbase.h"

extern int    cleanup();            /* cleanup after signals                */
extern char * mktempfile();         /*temporary file name generator         */
extern struct hshentry * genrevs(); /*generate delta numbers                */
extern int    nerror;               /*counter for errors                    */
extern FILE * finptr;               /* RCS input file                       */

char *RCSfilename;
char *workfilename;
char * temp1file, * temp2file;

main (argc, argv)
int argc; char **argv;
{
        char * cmdusage;
        char command[NCPPN+revlength+40];
        int  revnums;                 /* counter for revision numbers given */
        char * rev1, * rev2;          /* revision numbers from command line */
        char numericrev[revlength];   /* holds expanded revision number     */
        char * xrev1, * xrev2;        /* expanded revision numbers          */
        struct hshentry * gendeltas[hshsize];/*stores deltas to be generated*/
        struct hshentry * target;
        char * boption, * otheroption;
        int  exit_stats;
        int  filecounter;

        catchints();
        boption=otheroption="";
        cmdid = "rcsdiff";
        cmdusage = "command format:\n    rcsdiff [-b] [-cefhn] [-rrev1] [-rrev2] file";
        filecounter=revnums=0;
        while (--argc,++argv, argc>=1 && ((*argv)[0] == '-')) {
                switch ((*argv)[1]) {
                case 'r':
                        if ((*argv)[2]!='\0') {
                            if (revnums==0) {
                                    rev1= *argv+2; revnums=1;
                            } elif (revnums==1) {
                                    rev2= *argv+2; revnums=2;
                            } else {
                                    faterror("too many revision numbers");
                            }
                        } /* do nothing for empty -r */
                        break;
                case 'b':
                        boption="-b ";
                        break;
                case 'c':
                case 'e':
                case 'f':
                case 'h':
                case 'n':
                        if (*otheroption=='\0') {
                                otheroption=(*argv);
                        } else {
                                faterror("Options c,e,f,h,n are mutually exclusive");
                        }
                        break;
                default:
                        faterror("unknown option: %s\n%s", *argv,cmdusage);
                };
        } /* end of option processing */

        if (argc<1) faterror("No input file\n%s",cmdusage);

        /* now handle all filenames */
        do {
                finptr=NULL;

                if (pairfilenames(argc,argv,true,false)!=1) continue;
                if (++filecounter>1)
                        diagnose("===================================================================");
                diagnose("RCS file: %s",RCSfilename);
                if (revnums<2 && !(access(workfilename,4)==0)) {
                        error("Can't open %s",workfilename);
                        continue;
                }
                if (!trysema(RCSfilename,false)) continue; /* give up */


                gettree(); /* reads in the delta tree */

                if (Head==nil) {
                        error("no revisions present");
                        continue;
                }
                if (revnums==0) rev1=Head->num; /* default rev1 */

                if (!expandsym(rev1,numericrev)) continue;
                if (!(target=genrevs(numericrev,nil,nil,nil,gendeltas))) continue;
                xrev1=target->num;

                if (revnums==2) {
                        if (!expandsym(rev2,numericrev)) continue;
                        if (!(target=genrevs(numericrev,nil,nil,nil,gendeltas))) continue;
                        xrev2=target->num;
                }


                temp1file=mktempfile("/tmp/",TMPFILE1);
                diagnose("retrieving revision %s",xrev1);
                sprintf(command,"%s/co -q -p%s %s > %s\n",
                        TARGETDIR,xrev1,RCSfilename,temp1file);
                if (system(command)){
                        error("co failed");
                        continue;
                }
                if (revnums<=1) {
                        temp2file=workfilename;
                        diagnose("diff %s%s -r%s %s",boption,otheroption,xrev1,workfilename);
                } else {
                        temp2file=mktempfile("/tmp/",TMPFILE2);
                        diagnose("retrieving revision %s",xrev2);
                        sprintf(command,"%s/co -q -p%s %s > %s\n",
                                TARGETDIR,xrev2,RCSfilename,temp2file);
                        if (system(command)){
                                error("co failed");
                                continue;
                        }
                        diagnose("diff %s%s -r%s -r%s",boption,otheroption,xrev1,xrev2);
                }
                sprintf(command,"%s %s %s %s %s\n",DIFF,boption,
                        otheroption, temp1file, temp2file);
                exit_stats = system (command);
                if (exit_stats != 0 && exit_stats != (1 << BYTESIZ)) {
                        error ("diff failed");
                        continue;
                }
        } while (cleanup(),
                 ++argv, --argc >=1);


        exit(nerror!=0);

}

