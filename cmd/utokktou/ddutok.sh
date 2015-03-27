for i in $*
do
utok <$i >t.m.p u
case $? in
	0) if mv t.m.p $i
	   then
		echo $i ready
	   else
		echo can not mov to $i
	   fi
	;;
	4) echo $i not changed;rm -f t.m.p;;
	2) echo $i is not text file;rm -f t.m.p;;
	*) echo $i : error utok;rm -f t.m.p;;
esac
done
