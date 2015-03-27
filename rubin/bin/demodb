:	"@(#)demodb.sh	7.1	2/6/81"
if test "$1" = "" ; then
	echo no database name specified
	exit
fi
echo creating database $1 -- please wait
if creatdb $2 $1 ; then echo loading relations ; else exit; fi
# if mc68
# then
# Sfalg=-S
# else
Sflag="";
# fi
rubin -s $Sflag $2 $1 <<\EOF
create товары (
	номер is i2,
	имя is c20,
	отд is i2,
	цена is i2,
	кол is i2,
	поставщ is i2)

create сбыт (
	номер is c6,
	дата is c8,
	срок is i2,
	отд is i2,
	товар is i2,
	колич is i2,
	служащ is i2,
	кредит is c8)

create служащие (
	номер is i2,
	имя is c20,
	оклад is i2,
	управл is i2,
	дата_рожд is i2,
	нач_раб is i2)

create отдел (
	номер is i2,
	имя is c20,
	запас is i2,
	этаж is i2,
	управл is i2)

create поставщики (
	номер is i2,
	имя is c15,
	город is c15,
	респуб is c6)

create снабжение (
	номер is i2,
	город is c15,
	респуб is c6)

create части (
	номер is i2,
	имя is c20,
	цвет is c8,
	вес is i2,
	колич is i2)

create поставки (
	Nпост is i2,
	Nчасти is i2,
	Nакта is i2,
	дата_пост is c8,
	размер is i2)

copy товары (
	номер is c0colon,
	имя is c0colon,
	отд is c0colon,
	цена is c0colon,
	кол is c0colon,
	поставщ is c0nl)
from "{pathname}/demo/товары"

copy сбыт (
	номер is c0colon,
	дата is c0colon,
	срок is c0colon,
	отд is c0colon,
	товар is c0colon,
	колич is c0colon,
	служащ is c0colon,
	кредит is c0nl)
from "{pathname}/demo/сбыт"

copy служащие (
	номер is c0colon,
	имя is c0colon,
	оклад is c0colon,
	управл is c0colon,
	дата_рожд is c0colon,
	нач_раб is c0nl)
from "{pathname}/demo/служащие"

copy отдел (
	номер is c0colon,
	имя is c0colon,
	запас is c0colon,
	этаж is c0colon,
	управл is c0nl)
from "{pathname}/demo/отдел"

copy поставщики (
	номер is c0colon,
	имя is c0colon,
	город is c0colon,
	респуб is c0nl)
from "{pathname}/demo/поставщики"

copy снабжение (
	номер is c0colon,
	город is c0colon,
	респуб is c0nl)
from "{pathname}/demo/служащие"

copy части (
	номер is c0colon,
	имя is c0colon,
	цвет is c0colon,
	вес is c0colon,
	колич is c0nl)
from "{pathname}/demo/части"

copy поставки (
	Nпост is c0colon,
	Nчасти is c0colon,
	Nакта is c0colon,
	дата_пост is c0colon,
	размер is c0nl)
from "{pathname}/demo/поставки"

     create работник(
		номер=i2,
		имя=c20,
		начальник=i2,
		заработок=i2,
		родился=i2,
		начал_работу=i2)
     create макс_номера(работник=i2)
     append to макс_номера(работник=1)

     range of p is служащие
     range of r is служащие
     define view work(фамилия=p.имя,заработок=p.оклад,начальник=r.имя,
		      возраст=(1988-p.дата_рожд),стаж=(1988-p.нач_раб))
	    where (p.управл=r.номер and p.номер != 0)

range of i is товары
define permit all on i to all
save товары until 12  31 1987

range of s is сбыт
define permit all on s to all
save сбыт until 12  31 1987

range of e is служащие
define permit all on e to all
save служащие until 12  31 1987

range of d is отдел
define permit all on d to all
save отдел until 12  31 1987

range of s is поставщики
define permit all on s to all
save поставщики until 12  31 1987

range of s is снабжение
define permit all on s to all
save снабжение until 12  31 1987

range of p is части
define permit all on p to all
save части until 12  31 1987

range of s is поставки
define permit all on s to all
save поставки until 12  31 1987
\g
EOF
echo database $1 created
