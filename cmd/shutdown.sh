case $1 in
1) i="1";;
2) i="2 1";;
3) i="3 2 1";;
4) i="4 3 2 1";;
5) i="5 4 3 2 1";;
*) i="5 4 3 2 1";;
esac
for j in $i
do
echo "
 Система кончит работу через $j минут" | /etc/wall
sleep 60
done
echo "
Система кончит работу через 15 секунд"    | /etc/wall
sleep 15
echo "КОНЕЦ"  | /etc/wall
sync
/etc/reboot
be];
	} else {
		hdallop = emptyhd;
		hp = emptyslot;
	}
	if (htabinstall){
		*hp = taballoc();
		hdallop->h_nused++;
		for(len = 0, from = name, to = (*hp)->p.name; (len<NC); len++)
			if ((*to++ = *