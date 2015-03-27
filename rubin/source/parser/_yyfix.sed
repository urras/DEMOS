/^.globl.*_yyexca/,/^.comm.*_yychar/s/^.globl.*_y.*$/.globl/
s/^_yyexca/.text\
&/
s/^_yychar/.data\
&/
