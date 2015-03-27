: Эта программа предназначена для сборки библиотек разделов
: для загрузки через OVERLAY
: Обращение:
: 'ldover <список файлов и флагов резидента>
:          { <головной файл раздела> <ост.файлы и флаги> , ... } '
:   -V <имя> : имя создаваемой библиотеки разделов
:   -ОS <имя> : имя, под которым создается модуль связи.
echo "Сборщик перекрывающихся сегментов, (в.1. ИАЭ им. Курчатова)"
echo "Разбор параметров"
ld="ld"
MAIN="/lib"
if /bin/test "$1" "=" "-M"
then
MAIN=$2;echo MAIN= $2;PATH=:MAIN:$PATH;shift;shift
elif /bin/test -f /usr/lib/ogensw
then
MAIN="/usr/lib"
elif /bin/test -f /lib/ogensw
then
MAIN=/lib
else
echo "А откуда брать ogensw?? Укажите -М <каталог>"
exit 1
fi
flags=
libs=
carg="-X"
oarg="-Vo 1"
vfiles=
Vname="V.out.a"
crtf="/lib/crt0.o"
ovlib=$MAIN/libovl.a
oswitch="Oswitch"
for i
do
  case $1 in
    -OS)oswitch=`basename $2 .s`;shift;;
    -V) Vname=$2;shift;;
    -l*)libs="$libs $1";;
    -f)crtf=/lib/fcrt0.o;;
    "{")break;;
    "")echo "error: { no found"; exit 1;;
    *)carg="$carg $1";;
  esac
shift;
done
carg="$crtf $carg $oswitch.o $ovlib $libs -lc -lc"
shift
case $1 in
*.o);;
*) echo error "no segment name:$1?"; exit 1;;
esac
vfiles=$1
arfiles=$1v
oarg="-V 1 -o $1v $1"
shift
for i in $*
do
 case $1 in
   "}")break;;
   ",")shift;
           case $1 in
           *.o)vfiles="$vfiles $1"
               arfiles="$arfiles $1v"
               oarg="$oarg $libs -lc -lc -V 1 -o $1v $1";;
           *)echo "error:no segment name, $1 ?";exit;;
           esac
           ;;
    "*")oarg="$oarg $1";;
 esac
 shift
done
oarg="$oarg $libs -lc -lc"
echo Начало генерации переключателя
if $MAIN/ogensw $vfiles >${oswitch}.s && as - -o ${oswitch}.o ${oswitch}.s
then
:
else
echo error in switch
exit 1
fi
echo "Загрузчик"
echo $ld $carg $oarg
if $ld $carg $oarg
then
echo "Запись в библиотеку"
rm $Vname;ar r $Vname $arfiles
echo Готовы: основной файл и библиотека $Vname
else
echo Не удалось собрать объектные файлы
exit 1
fi
