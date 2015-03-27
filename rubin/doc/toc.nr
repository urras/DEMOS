.th СОДЕРЖАНИЕ \*(RU 3/23/79
.s3
.i0
.in +4
.ti -4
EQUEL(QUEL) \- краткое описание языка EQUEL
.br
.s3
.i0
.in +4
.ti -4
MACROS(QUEL) \- описание языка макропроцессора монитора
.s3
.i0
.in +4
.ti -4
QUEL(QUEL) \- краткое описание языка QUEL
.s3
.i0
.in +4
.ti -4
APPEND(QUEL) \- добавить кортежи к отношению
.br
.bd append
[\c
.bd to\c
] имя_отн (список_объектов) [\c
.bd where
условие\c
]
.s3
.i0
.in +4
.ti -4
COPY(QUEL) \- скопировать данные в/из отношения из/в файл \*(UU.
.br
.bd copy
имя_отн
(имя_домена = формат {, имя_домена = формат })
.br
.ti +10
направление "имя файла"
.s3
.i0
.in +4
.ti -4
CREATE(QUEL) \- создать новое отношение
.br
.bd create
имя_отн (имя_домена\c
.it 1
= формат {, имя_домена\c
.it 2
= формат })
.s3
.i0
.in +4
.ti -4
DEFINE(QUEL) \- определить подсхему
.br
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
[ (список) ]
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
.i0
.s3
.i0
.in +4
.ti -4
DELETE(QUEL) \- удалить кортежи из отношения
.br
.bd delete
кортеж_переменная
[\c
.bd where
условие\c
]
.s3
.i0
.in +4
.ti -4
DESTROY(QUEL) \- уничтожить существующее отношение(я)
.br
.bd destroy
имя_отн {, имя_отн}
.br
.bd destroy
[\c
.bd permit
|
.bd integrity\c
] имя_отн
[\c
integer {, integer}\c
|
.bd all\c
]
.s3
.i0
.in +4
.ti -4
HELP(QUEL) \- получить информацию о том, как использовать \*(II или об отношениях в базе данных.
.br
.bd help
[\c
имя_отн\c
] [\c
"имя_статьи"\c
]
{, имя_отн}{, "имя_статьи"}
.br
.bd "help view"
имя_отн {, имя_отн}
.br
.bd "help permit"
имя_отн {, имя_отн}
.br
.bd "help integrity"
имя_отн {, имя_отн}
.s3
.i0
.in +4
.ti -4
INDEX(QUEL) \- создать вторичный индекс на существующем отношении.
.br
.bd "index on"
имя_отн
.bd is
имя_индекса
(домен\c
.it 1
{ ,домен\c
.it 2\c
})
.s3
.i0
.in +4
.ti -4
INTEGRITY(QUEL) \- определить ограничение целостности.
.br
.bd "define integrity on"
переменная
.bd is
условие
.s3
.i0
.in +4
.ti -4
MACROS(QUEL) \- макровозможности терминального монитора.
.br
.s3
.i0
.in +4
.ti -4
MODIFY(QUEL) \- преобразовать структуру памяти отношения.
.br
.in +5
.ti -5
.bd modify
имя_отн
.bd to
структура_памяти
[
.bd on
ключ1 [ :
.it порядок_сорт
] [ { , ключ2 [ :
.it порядок_сорт
] } ] ]
[
.bd where
[
.bd "fillfactor ="
.it n
] [
.bd ", minpages ="
.it n
] [
.bd ", maxpages ="
.it n
] ]
.i0
.s3
.i0
.in +4
.ti -4
MONITOR(QUEL) \- интерактивный терминальный монитор.
.br
.s3
.i0
.in +4
.ti -4
PERMIT(QUEL) \- добавить разрешение в отношение.
.br
.bd define
.bd permit
список_операций {
.bd on
|
.bd of
|
.bd to
} переменная [ (список) ]
.br
.if n .ti +5
.if t .ti +0.5i
.bd to
имя [
.bd at
терминал ]
[
.bd from
время
.bd to
время ]
.br
.if n .ti +5
.if t .ti +0.5i
[
.bd on
день_нед
.bd to
день_нед ] [
.bd where
условие ]
.s3
.i0
.in +4
.ti -4
PRINT(QUEL) \- печатать отношение(я).
.br
.bd print
имя_отн {, имя_отн}
.s3
.i0
.in +4
.ti -4
RANGE(QUEL) \- об'явить переменную для распространения на отношение.
.br
.bd "range of"
переменная
.bd is
имя_отн
.s3
.i0
.in +4
.ti -4
REPLACE(QUEL) \- заменить значения доменов в отношении.
.br
.bd replace
кортеж_переменная (список_объектов)
[\c
.bd where
условие]
.s3
.i0
.in +4
.ti -4
RETRIEVE(QUEL) \- выдать кортежи из отношения.
.br
.bd retrieve
[[\c
.bd into\c
] имя_отн] (список_объектов)
[\c
.bd where
условие]
.br
.bd "retrieve unique"
(список_объектов) [\c
.bd where
условие]
.s3
.i0
.in +4
.ti -4
SAVE(QUEL) \- хранить отношение вплоть до заданной даты.
.br
.bd save
имя_отн
.bd until
месяц день год
.s3
.i0
.in +4
.ti -4
VIEW(QUEL) \- определить виртуальное отношение.
.br
.bd define
.bd view
имя (список_объектов) [
.bd where
условие ]
.s3
.i0
.in +4
.ti -4
COPYDB(\*(DM) \- создание файлов-заданий, копирующих и восстанавливающих
базу данных.
.br
.bd copydb
[
.bd \-u\c
.it имя
[ база_данных полное_имя_каталога
[ отношение ... ]
.s3
.i0
.in +4
.ti -4
CREATDB(\*(DM) \- создать базу данных.
.br
.bd creatdb
[
.bd \-u\c
.it имя
] [
.bd \-e
] [
.bd \-m
] [
.bd \*(+-c
] [
.bd \*(+-q
] имя_бд
.s3
.i0
.in +4
.ti -4
DESTROYDB(\*(DM) \- уничтожить существующую базу данных.
.br
.bd destroydb
[
.bd \-s
] [
.bd \-m
] имя_бд
.s3
.i0
.in +4
.ti -4
EQUEL(\*(DM) \- встроенный в язык C интерфейс с QUEL.
.br
.bd equel
[
.bd \-d
] [
.bd \-f
] [
.bd \-r
] file.q ...
.s3
.i0
.in +4
.ti -4
HELPR(\*(DM) \- получить информацию о базе данных.
.br
.bd helpr
[
.bd \-u\c
.it имя
] [
.bd \*(+-w
] база_даных отношение ...
.s3
.i0
.in +4
.ti -4
\*(RU(\*(DM) \- система управления реляционной базой данных \*(II.
.br
.bd \*(rN
[
.it ключи
] имя_бд [ таблица_процессов ]
.s3
.i0
.in +4
.ti -4
PRINTR(\*(DM) \- печатать отношения.
.br
.bd printr
[
.it ключи
] база_данных отношение ...
.s3
.i0
.in +4
.ti -4
PURGE(\*(DM) \- уничтожить все промежуточные и временные отношения.
.br
.bd purge
[
.bd \-f
] [
.bd \-p
] [
.bd \-a
] [
.bd \-s
] [
.bd \*(+-w
] [ база_данных ... ]
.s3
.i0
.in +4
.ti -4
RESTORE(\*(DM) \- восстановление после аварии  \*(II или \*(UU .
.br
.bd restore
[
.bd \-a
] [
.bd \-s
] [
.bd \*(+-w
] [ база_данных ... ]
.s3
.i0
.in +4
.ti -4
SYSMOD(\*(DM) \- привести системные отношения к предопределенным структурам памяти.
.br
.bd sysmod
[
.bd \-s
] [
.bd \-w
] имя_бд
[
.bd relation
] [
.bd attribute
] [
.bd indexes
] [
.bd tree
] [
.bd protect
] [
.bd integrities
]
.s3
.i0
.in +4
.ti -4
USERSETUP(\*(DM) \- создать файл пользователей "users".
.br
.bd \&.../bin/usersetup
[ полное_имя_файла ]
.s3
.i0
.in +4
.ti -4
DAYFILE(FILES) \- регистрационное сообщение \*(II.
.br
.s3
.i0
.in +4
.ti -4
DBTMPLT(FILES) \- шаблон базы данных.
.br
.s3
.i0
.in +4
.ti -4
ERROR(FILES) \- файлы с ошибками \*(II.
.br
.s3
.i0
.in +4
.ti -4
LIBQ(FILES) \- библиотека функций EQUEL.
.br
.s3
.i0
.in +4
.ti -4
PROCTAB(FILES) \- информация о конфигурации \*(II во время работы.
.br
.s3
.i0
.in +4
.ti -4
STARTUP(FILES) \- файл инициализации \*(II.
.br
.s3
.i0
.in +4
.ti -4
USERS(FILES) \- коды и параметры пользователя \*(II.
.br
.s3
.i0
.in +4
.ti -4
INTRODUCTION(ERROR) \- введение по сообщениям об ошибках.
.br
.s3
.i0
.in +4
.ti -4
EQUEL(ERROR) \- сводка сообщений об ошибках \s-2EQUEL\s0.
.br
Номера ошибок 1000 \- 1999.
.s3
.i0
.in +4
.ti -4
PARSER(ERROR) \- сводка сообщений об ошибках лексического анализатора.
.br
Номера ошибок 2000 \- 2999.
.s3
.i0
.in +4
.ti -4
QRYMOD(ERROR) \- сводка сообщений об ошибках модификаций запросов.
.br
Номера ошибок 3000 \- 3999.
.s3
.i0
.in +4
.ti -4
OVQP(ERROR) \- сводка сообщений об ошибках процессора запросов с одной переменной.
.br
Номера ошибок 4000 \- 4499.
.s3
.i0
.in +4
.ti -4
DECOMP(ERROR) \- сводка сообщений об ошибках декомпозиции.
.br
Номера ошибок 4500 \- 4999.
.s3
.i0
.in +4
.ti -4
DBU(ERROR) \- сводка сообщений об ошибках обслуживающих программ базы данных.
.br
Номера ошибок 5000 \- 5999
