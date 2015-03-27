#include <stdio.h>
#include <sgtty.h>
#include "lrnref"

selsub(argc,argv)
register char *argv[];
register int  argc;
{
	char ans1[100], *cp;
	static char ans2[30];
	static char dirname [20];
	static char subname[20];
	static char russian[20];
	struct sgttyb strterm;
	char fd;
	int wrong=0;
	int i,j;
	int n= 4; /* ���� ������ ������� learn ����� ����������, ��������� */
	/* ���� �������� */
	fd = fileno (stdout);
	gtty (fd, &strterm);            /* �������� ����������� ������ */
	if (strterm.sg_flags&CYRILL){       /* ��������� � ������� ������� */
		flag=1;
	}

	/* ������ ���������� �������*/
	if (argc >1 && argv[1][0] =='-') {           /* learn -DIR */
		direct = argv[1]+1 ;                       /* ������ ������� ���������� */
		argc--;
		argv ++;
	}
	chknam(direct);
	if(chdir(direct)!=0){
		if (flag) fprintf(stderr,"�� ����� ����� � ���������� %s ��� ���������� ����� \n",direct);
		else fprintf(stderr,"can't cd to directory %s to exercise a script.\n",direct);
		fflush(stderr);
		exit(1);
	}

	if (flag) lang();

	/* ������ �� ��� ����� ?*/
	russian[0] = 'r'; 
	russian[1] = NULL;
	if(flag) sname = argc > 1 ? strcat(russian,argv[1]) : NULL;
	else sname = argc > 1 ? argv[1] : NULL;
	if (argc>2)                     /* ����� �� N �����? */
		strcpy (level=ans2,argv[2]);
	else level=0;
	if (argc>3)                     /* ������ �� ������? */
		speed=atoi(argv[3]);

       if (!sname) {
		if (!flag) {
			printf("These are the available courses - \n ");
			list ("Linfo");
			list("info");
		}
		else {
			printf("��������� ����� : \n");
			list("rLinfo");
			list("rinfo");
			printf("\n$");
		}
		fflush(stdout);
ag: 
		gets(subname);
		fflush(stdout);
		russian[0] = 'r'; 
		russian[1] = NULL;
		if (flag){ 
			sname = strcat(russian, subname);
		} 
		else sname = subname;
		fflush(stdout);
		if( (sname[0]=='\0')||((sname[0]=='r')&& (sname[1]=='\0'))){
			if (!wrong) {
				if (flag) list("rXinfo");
				else list("Xinfo"); 
			}       
			fflush(stdout);
			do { 
				if (flag) printf("\n� ����� ������ �� ������ ��������?\n$ ");
				else printf("\n Which subject?");
				fflush(stdout);
				gets(subname);
				russian[0] = 'r'; 
				russian[1] = NULL;
				if (flag){ 
					sname = strcat(russian, subname);
				}
			} 
			while( (sname[0]=='\0')||((sname[0]=='r')&&(sname[1]=='\0')));
		}
	}
	if(access(sname,05)<0){
		if(flag){
			printf("\n��������, �� �������� � �������� %s �� \n",subname);
			printf("����������. ���������� ��� �����.\n");
			printf("��������� ����� : \n");
			list("rLinfo");
			printf("\n$");
		}
		else {
			printf("Sorry, but there is no subject named %s .\n",sname);
			printf("Try again.\n");
		}
		fflush(stdout);
		wrong++;
		if(wrong <= n){
			for(i=0; subname[i] != '\0'; i++) subname[i]='\0';
			for(j=0; sname[j] != '\0'; j++) sname[j]='\0';
			goto ag;
		}
		else chknam (sname);
	}
	if (!level) { 
		if (!flag)   list("begin");
		else    list("���");
		printf("\n$");
		fflush(stdout);
		gets(ans2);
		if (ans2[0]==0)
			strcpy(ans2,"0");
		for (cp=ans2;*cp;cp++)
			if ( *cp == '(' || *cp ==' ')
				*cp=0;
		level=ans2;
	}
	/* �������� ������ �������� ����������� ������������*/
	if(chdir("play")!=0){
		if (flag) fprintf( stderr,"�� ����� ������� � ������� ���������� .\n");
		else fprintf(stderr,"can't cd to playpen.\n");
		exit(1);
	}
	sprintf(dir=dirname,"pl%da",getpid());
	sprintf(ans1,"mkdir %s",dir);
	system(ans1);
	if (chdir(dir)<0){
		if(flag) fprintf(stderr,"�� ������ ������� ������� ����������.\n����.\n");
		else fprintf(stderr,"couldn't create working directory.\nBye.\n");
		exit(1); 
	}
	/* � ���� ����� � ��� ���� ������� ���������� */
	/* �������� wrapup , ����� ��������� ���*/
	if (access (sprintf(ans1, "%s/%s/init", direct, sname), 04)==0)
		if(system(sprintf(ans1, "%s/%s/init %s", direct, sname, level))!=0){
			if (flag) printf("������� �� 'learn'.\n����.\n");
			else printf("Leaving 'learn'.\nBYE.\n");
			fflush(stdout);
			wrapup(1);
		}
	if(level[0]=='-') /* ��������� � ����� ������� */
		ask=1;
	start(level);
}
chknam(name)
register char *name;
{
	if (access(name,05)<0)
	{
		if (flag) printf("���������, �� ����� ��� ����� �� ����� %s ���.\n����.\n", name);
		else printf("Sorry, there is no subject or lesson named %s.\nBye.\n", name);
		exit(1); 
	}
}
lang()
{ 
	char ans3[30];
	list("Pref");
	printf ("\n$");
	fflush (stdout);
	gets (ans3);
	if (ans3[1] =='\0') flag=1; 
	else {
		if( (ans3 [0] == '�')||(ans3[0]=='�')||(ans3[0]=='d')||(ans3[0]='D') ) flag=0;
	}             
}
