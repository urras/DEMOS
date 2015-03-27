for i in $*
do
echo ".ip $i 10"
sed -n "32,40s/^ *\([^ ].*\)$/\1/P" $i
echo $i ready >&2
done
