/*
 * Запись первой дистрибутивной ленты
 *
 * $Log:	maketape.c,v $
 * Revision 1.1  86/07/14  19:14:24  avg
 * Initial revision
 * 
 */
#include <stdio.h>
#include <ediag.h>

#define MAXB 30

int mt;
int fd;
char    buf[MAXB*512];
char    name[50];
int     blksz;
int     cnt,ii;

main(argc, argv)
int     argc;
char    *argv[];
{
	int i, j, k;
	FILE *mf;

	if (argc != 3) {
		fprintf(stderr, ediag("Usage: maketape tapedrive makefile\n",
				      "Вызов: maketape у-во ком_файл\n"));
		exit(0);
	}
	if ((mt = creat(argv[1], 0666)) < 0) {
		perror(argv[1]);
		exit(1);
	}
	if ((mf = fopen(argv[2], "r")) == NULL) {
		perror(argv[2]);
		exit(2);
	}

	j = 0;
	k = 0;
	for (;;) {
		if ((i = fscanf(mf, "%s %d", name, &blksz))== EOF)
			exit(0);
		if (i != 2) {
			fprintf(stderr, ediag("Help! Scanf didn't read 2 things (%d)\n",
					      "Scanf не может прочесть очередные два поля (%d)\n"), i);
			exit(1);
		}
		if (blksz <= 0 || blksz > MAXB) {
			fprintf(stderr, ediag("Block size %d is invalid\n",
					      "Плохой размер блока %d\n"), blksz);
			continue;
		}
		if (strcmp(name, "*") == 0) {
			close(mt);
			sleep(3);
			mt = open(argv[1], 2);
			j = 0;
			k++;
			continue;
		}
		fd = open(name, 0);
		if (fd < 0) {
			perror(name);
			continue;
		}
		printf(ediag("%s: block %d, file %d\n",
			     "%s: блок %d, файл %d\n"), name, j, k);

		/*
		 * Последняя неполная запись заполняется нулями.
		 * Таким образом в конце текстовых файлов нет мусора.
		 */

		while ( (cnt=read(fd, buf, 512*blksz)) == 512*blksz) {
			j++;
			write(mt, buf, 512*blksz);
		}
		if ( cnt>0 ){
			for(ii=cnt; ii < 512*blksz; ii++)
				buf[ii] = '\0';
			write(mt, buf, 512*blksz);
		}
	}
}
