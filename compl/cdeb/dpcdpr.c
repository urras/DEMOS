/* === CDEBUGGER === ПРОГРАММЫ ВВОДА/ВЫВОДА НА ДИСПЛЕЙ */

/* весь ввод/вывод отладчика локализован в данном модуле
   для вывода используется буфер 'outline' и указатель на
   текущую позицию в нем 'pline'. позиция курсора в строке
   считается в переменной 'posit'.

   точки входа:
        dpc(адрес строки)       - вывод строки символов до 0
        dpo(символ)             - вывод одного символа
        dpr(адрес буфер,длина)  - ввод строки в буфер. в конец строки
                                  помещается 0, возвращает длину (без 0).
                                  строка отображается и после нее: '\n'  */

/* данная версия использует интерфейс UNIX'а - системные вызовы
   read(0,...,...)  и  write(1,...,...)                        */

#define MAXBUF  256
char outline[MAXBUF+1], *pline=outline; /* буфер вывода                  */
int  posit;                             /* позиция в выходной строке     */
extern int dsp_inp, dsp_out;            /* дисплей для ввода и вывода    */
static int oldtty[3],newtty[3];         /* состояние дисплея             */
int ttyflag;
extern int protocol;                    /* дескриптор файла протокола    */


dpo(letter)
    register char letter;
{
    if( !ttyflag ) {
        gtty(dsp_out,oldtty);
        ttyflag = 1;
    }
    *pline++ = letter;
    check_buf();
    if( letter == '\n' ) posit=0;  else  ++posit;
}


dpc(string)
    register char *string;
{
    while( *string ) dpo( *string++ );
}


check_buf()                      /* проверка: не переполнился ли буфер ? */
{
    if( pline >= &outline[MAXBUF] ) {
        gtty(dsp_out,newtty);
        stty(dsp_out,oldtty);
        write(dsp_out, outline, pline-outline);
        write(protocol,outline, pline-outline);
        stty(dsp_out,newtty);
        pline = outline;
    }
}


dpr(adr,length)
    register char *adr;
    register int  length;
{
    register int  lgt;

    gtty(dsp_out,newtty);
    stty(dsp_out,oldtty);
    write(dsp_out, outline, pline-outline);              /* вывод буфера */
    write(protocol,outline, pline-outline);
    pline = outline;

    lgt = read(dsp_inp, adr, length);                     /* ввод строки */
    write(protocol, adr, lgt);
    stty(dsp_inp,newtty);

    if( lgt<=0 ) {                                        /* конец работы*/
        write(dsp_out, "\n", 1);
        write(protocol,"\n", 1);
        exitdeb();
    }
    adr [ --lgt ] = 0;
    return( lgt );
}


exitdeb()
{
    write(dsp_out, outline, pline-outline);              /* вывод буфера */
    write(protocol,outline, pline-outline);
    stty(dsp_out,oldtty);
    exit(0);
}
