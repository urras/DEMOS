.ds LF help bc bugs
.ds RF ape
.LP
.DS C
Bugs and Oddities in BC
.DE
.IP "1."
BC's ``freedom of format'' is somewhat limited.
In particular, it expects the predicates of
.B if ,
.B while ,
.B for ,
and
.B define
statements to begin on the same line as the keyword.
For example,
.DS
\fIif\fR  (n > 5)
{     t=1
      s=4
}
.DE
would be interpreted as
.DS
\fIif\fR  (n > 5)  ;
t=1
s=4
.DE
instead of the (presumably) intended
.DS
\fIif\fR  (n > 5) { 
      t=1
      s=4
}
.DE
.IP "2."
Output formatting options are essentially nonexistent.
.IP "3."
BC does not allow shell escapes, though DC does.
There are two ways to get around this.
One is to write an input processor to handle shell escapes;
another (if BC is not being used interactively) is to insert
the desired commands in the BC source as quoted strings,
translate (with
.I bc
.I -c )
the BC source into DC source,
``unquote'' the strings (of the form [!...]P)
in the DC source file, and run it.
.IP "4."
BC chokes on large input files, running out of
headers or space very quickly.
.IP "5."
BC does not seem to be able to read numbers more than 144
digits in length, though DC can.
