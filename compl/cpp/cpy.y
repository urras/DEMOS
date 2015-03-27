/*
 * $Log:	CPY.Y,V $
 * Revision 1.3  87/01/11  16:22:36  avg
 * *** empty log message ***
 * 
 * Revision 1.2  87/01/11  16:04:46  avg
 * Версия ид ДЕМОС 1.3 & МНОС РЛ 1.2
 *
 */

/*
 * C препроцессор
 *
 *  Адаптирован для МНОС 1.1 СМ-4
 *    Антоновым В.Г. и Леонтьевым С.Е. ( ИПК Минавтопрома ) 18.05.1984.
 *  Модифицирован Антоновым В.Г. для обработки русских букв 19.05.1984.
 */

%term number stop DEFINED
%term EQ NE LE GE LS RS
%term ANDAND OROR
%left ','
%right '='
%right '?' ':'
%left OROR
%left ANDAND
%left '|' '^'
%left '&'
%binary EQ NE
%binary '<' '>' LE GE
%left LS RS
%left '+' '-'
%left '*' '/' '%'
%right '!' '~' UMINUS
%left '(' '.'
%%
S:      e stop  ={return($1);}


e:        e '*' e
        ={$$ = $1 * $3;}
    | e '/' e
        ={$$ = $1 / $3;}
    | e '%' e
        ={$$ = $1 % $3;}
    | e '+' e
        ={$$ = $1 + $3;}
    | e '-' e
        ={$$ = $1 - $3;}
    | e LS e
        ={$$ = $1 << $3;}
    | e RS e
        ={$$ = $1 >> $3;}
    | e '<' e
        ={$$ = $1 < $3;}
    | e '>' e
        ={$$ = $1 > $3;}
    | e LE e
        ={$$ = $1 <= $3;}
    | e GE e
        ={$$ = $1 >= $3;}
    | e EQ e
        ={$$ = $1 == $3;}
    | e NE e
        ={$$ = $1 != $3;}
    | e '&' e
        ={$$ = $1 & $3;}
    | e '^' e
        ={$$ = $1 ^ $3;}
    | e '|' e
        ={$$ = $1 | $3;}
    | e ANDAND e
        ={$$ = $1 && $3;}
    | e OROR e
        ={$$ = $1 || $3;}
    | e '?' e ':' e
        ={$$ = $1 ? $3 : $5;}
    | e ',' e
        ={$$ = $3;}
    | term
        ={$$ = $1;}
term:
      '-' term %prec UMINUS
        ={$$ = -$2;}
    | '!' term
        ={$$ = !$2;}
    | '~' term
        ={$$ = ~$2;}
    | '(' e ')'
        ={$$ = $2;}
    | DEFINED '(' number ')'
        ={$$= $3;}
    | DEFINED number
        ={$$ = $2;}
    | number
        ={$$= $1;}
%%
# include "yylex.c"
