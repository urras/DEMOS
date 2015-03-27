.th DEFINE QUEL "СУБД \*(RU. ОПЕРАТОРЫ QUEL"
.sh ИМЯ
define \- определить подсхему на отношении
.sh ФОРМАТ
.in +5
.ti -5
.bd define
.bd view
имя (список_объектов) [
.bd where
условие ]
.ti -5
.bd "define permit"
список_операций
{
.bd on
|
.bd of
|
.bd to
}
переменная
[ (список_атрибутов) ]
.bd to
имя
[
.bd at
терминал ]
[
.bd from
время
.bd to
время ]
[
.bd on
день_нед
.bd to
день_нед ] [
.bd where
условие ]
.ti -5
.bd define
.bd integrity
.bd on
переменная
.bd is
условие
.ti -5
.bd define
имя_переменной
.bd =
значение
.i0
.sh ОПИСАНИЕ
Оператор
.it define
определяет заданные
подсхемы на указанном отношении.
Полное описание этих команд смотри в перечисленных ниже
секциях руководства.
.sh "ДОПОЛНИТЕЛЬНЫЕ ССЫЛКИ"
integrity(quel),
permit(quel),
view(quel)
serial(quel)
