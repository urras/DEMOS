/* Программа для генерации модуля/переключателя для реализации
   overlay.
   Параметры команды - список об'ектных файлов,
   команды генерирует в стандартный вывод программу
   на ассемблере, которая содержит переключатель входов
   и начальные данные для таблиц сегментов и входов.  */
#define MSEG 20
#define MENTRY 300
#include <stdio.h>
#include <a.out.h>

int ns=0, ne=0;
char *segname[MSEG];
struct stentry {
        int nseg; 
        unsigned shift;
} 
tentry[MENTRY];
struct exec bufe;
struct nlist bufs;

main (ac,av) int ac; 
char **av;
{
        register int i,nsym;
        char *namee;
        FILE *tf;
#define err(s) {namee = s; goto error;}
        prolog();
        while((av++, --ac))
        {
                if(!(tf = fopen(*av, "r"))) err("не могу открыть");
                if(fread(&bufe, (sizeof(bufe)),1,tf) != 1) err("плохой формат файла");
                if(bufe.a_magic != A_MAGIC1 || bufe.a_flag) err("плохой формат файла");
                fseek(tf,((long)(bufe.a_text + bufe.a_data))*2l,1);
                segname[ns++] = *av;
                if(ns > MSEG) err("Таблица сегментов переполнена");
                nsym = (bufe.a_syms)/(sizeof(bufs));
                for(i=0; i<nsym; i++)
                {
                        if(fread(&bufs, (sizeof(bufs)),1,tf) != 1) err("ошибка при чтении описателя");
                        if(bufs.n_type == N_EXT + N_TEXT)
                        {
                                if(ne > MENTRY) err("Переполнена таблица входов");
                                tentry[ne].nseg = ns;
                                tentry[ne++].shift = bufs.n_value;
                                putname();
                        }
                }
                fclose(tf);
        }
        finish();
        exit(0);
error: 
        fprintf(stderr,"Генератор переключателя: %8s : %8s\n", *av, namee);
        exit(1);
}

prolog()
{
        printf(".globl _O_switch\n_O_switch:\n");
}
putname()
{ 
        register char *s = &(bufs.n_name);
        printf(".globl %.8s\n%.8s:mov $%o,-(sp)\njmp _O_ldstart\n",s,s,ne);
}

finish()
{
        int i,j;
        printf(".globl _O_swend\n_O_swend:\n.data\n");
        for(i=0; i<ns; i++)
                printf("L%d:<%s>\n.byte 0\n",i+1,segname[i]);
        printf(".even\n.globl _O_sname\n_O_sname:\n");
        for(i=0;i<ns;i++)
                printf("L%d;",i+1);
        printf("0\n.even\n.globl _O_tentry\n_O_tentry:\n");
        for(i=0;i<ne;i++)
                printf("%o;%o\n",tentry[i].nseg,tentry[i].shift);
        printf("0;0\n");
}
