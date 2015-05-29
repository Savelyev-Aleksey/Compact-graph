<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ru_RU">
<context>
    <name>AboutForm</name>
    <message>
        <location filename="../../AboutForm.ui" line="14"/>
        <source>About program</source>
        <translation>О программе</translation>
    </message>
    <message>
        <location filename="../../AboutForm.ui" line="22"/>
        <source>Language:</source>
        <translation>Язык:</translation>
    </message>
    <message>
        <location filename="../../AboutForm.ui" line="63"/>
        <source>About Qt</source>
        <translation>О Qt</translation>
    </message>
    <message>
        <location filename="../../AboutForm.ui" line="113"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;
&lt;p&gt;This is simple program can do:
&lt;ul&gt;
&lt;li&gt;Read text file contain graph in following formats:
&lt;ul&gt;
&lt;li&gt;{NODE_NODE[_VALUE]} with basic weight value or with different weight values&lt;/li&gt;
&lt;li&gt;{BRACKETS_FLAT[_VALUE]} graph in brackets notation without nesting with basic weight or different weights&lt;/li&gt;
&lt;li&gt;{BRACKETS[_VALUE]} graph in brackets notation with nesting with basic weight or different weights&lt;/li&gt;
&lt;/ul&gt;
&lt;/li&gt;
&lt;li&gt;Save in files graph with same as read formats&lt;/li&gt;
&lt;li&gt;Save in files  generated short paths of graph with path weight&lt;/li&gt;
&lt;li&gt;Generate periodical graph structures: at this moment only hypercubes and torus&lt;/li&gt;
&lt;li&gt;Print graph statistic information:
&lt;ul&gt;
&lt;li&gt;Graph node, edge, generated short path count&lt;/li&gt;
&lt;li&gt;Graph diameter and radius&lt;/li&gt;
&lt;li&gt;Nodes indents statistic&lt;/li&gt;
&lt;li&gt;Nodes eccentricity sratistic&lt;/li&gt;
&lt;/ul&gt;
&lt;/li&gt;
&lt;/ul&gt;
&lt;/p&gt;
&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;
&lt;p&gt;Данная программа может делать следующее:
&lt;ul&gt;
&lt;li&gt;Читать текстовые файлы, содержащие граф в следующих форматах:
&lt;ul&gt;
&lt;li&gt;{NODE_NODE[_VALUE]} с базовым весом или с разным весом каждой вершины&lt;/li&gt;
&lt;li&gt;{BRACKETS_FLAT[_VALUE]} граф в скобочной записи без вложений с базовым веом или с разным весом рёбер&lt;/li&gt;
&lt;li&gt;{BRACKETS[_VALUE]} граф в скобочной записи с вложениями с базовым веом или с разным весом рёбер&lt;/li&gt;
&lt;/ul&gt;
&lt;/li&gt;
&lt;li&gt;Сохарнять в файлы граф в такие же форматы как форматы чтения&lt;/li&gt;
&lt;li&gt;Сохранять в файле созданные кратчайшие пути графа с весом пути&lt;/li&gt;
&lt;li&gt;Генерация периодических графовых структур: на данный момент только гиперкуб и тор&lt;/li&gt;
&lt;li&gt;Вывод статистики по графу:
&lt;ul&gt;
&lt;li&gt;Вершины графа, рёбра,количество созданных кратчайших путей&lt;/li&gt;
&lt;li&gt;Диаметр и радиус графа&lt;/li&gt;
&lt;li&gt;Статистика степеней вершин&lt;/li&gt;
&lt;li&gt;Стастистика эксцентриситетов вершин&lt;/li&gt;
&lt;/ul&gt;
&lt;/li&gt;
&lt;/ul&gt;
&lt;/p&gt;
&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
</context>
<context>
    <name>GraphGeneratorForm</name>
    <message>
        <location filename="../../GraphGeneratorForm.ui" line="14"/>
        <source>Graph generator</source>
        <translation>Генератор графа</translation>
    </message>
    <message>
        <location filename="../../GraphGeneratorForm.ui" line="20"/>
        <source>Torus</source>
        <translation>Тор</translation>
    </message>
    <message>
        <location filename="../../GraphGeneratorForm.ui" line="26"/>
        <source>Big radius</source>
        <translation>Большой радиус</translation>
    </message>
    <message>
        <location filename="../../GraphGeneratorForm.ui" line="40"/>
        <source>Small radius</source>
        <translation>Малый радиус</translation>
    </message>
    <message>
        <location filename="../../GraphGeneratorForm.ui" line="54"/>
        <location filename="../../GraphGeneratorForm.ui" line="107"/>
        <source>Edge weight</source>
        <translation>Вес ребра</translation>
    </message>
    <message>
        <location filename="../../GraphGeneratorForm.ui" line="71"/>
        <location filename="../../GraphGeneratorForm.ui" line="130"/>
        <source>Create</source>
        <translation>Создать</translation>
    </message>
    <message>
        <location filename="../../GraphGeneratorForm.ui" line="84"/>
        <source>Hypercube</source>
        <translation>Гиперкуб</translation>
    </message>
    <message>
        <location filename="../../GraphGeneratorForm.ui" line="93"/>
        <source>Dimention</source>
        <translation>Размерность</translation>
    </message>
    <message>
        <location filename="../../GraphGeneratorForm.cpp" line="71"/>
        <source>Hypercube dimention not a number</source>
        <translation>Размерность гиперкуба не число</translation>
    </message>
    <message>
        <location filename="../../GraphGeneratorForm.cpp" line="77"/>
        <source>Hypercube edge weight not a number</source>
        <translation>Вес ребра гиперкуба не является числом</translation>
    </message>
    <message>
        <location filename="../../GraphGeneratorForm.cpp" line="83"/>
        <location filename="../../GraphGeneratorForm.cpp" line="114"/>
        <source>Clearing graph...</source>
        <translation>Очистка графа...</translation>
    </message>
    <message>
        <location filename="../../GraphGeneratorForm.cpp" line="87"/>
        <source>Hypercube created</source>
        <translation>Гиперкуб создан</translation>
    </message>
    <message>
        <location filename="../../GraphGeneratorForm.cpp" line="96"/>
        <source>Torus big radius not a number</source>
        <translation>Тор большой радус не является числом</translation>
    </message>
    <message>
        <location filename="../../GraphGeneratorForm.cpp" line="102"/>
        <source>Torus small radius not a number</source>
        <translation>Тор малый радус не является числом</translation>
    </message>
    <message>
        <location filename="../../GraphGeneratorForm.cpp" line="108"/>
        <source>Torus edge weight not a number</source>
        <translation>Вес ребра тора не является числом</translation>
    </message>
    <message>
        <location filename="../../GraphGeneratorForm.cpp" line="118"/>
        <source>Torus created</source>
        <translation>Тор создан</translation>
    </message>
</context>
<context>
    <name>GraphInfoForm</name>
    <message>
        <location filename="../../GraphInfoForm.ui" line="223"/>
        <source>Nodes degree statistic</source>
        <translation>Статистика степеней вершин</translation>
    </message>
    <message>
        <location filename="../../GraphInfoForm.ui" line="272"/>
        <source>Degree</source>
        <translation>Степень</translation>
    </message>
    <message>
        <location filename="../../GraphInfoForm.ui" line="277"/>
        <location filename="../../GraphInfoForm.ui" line="419"/>
        <source>Count</source>
        <translation>Количество</translation>
    </message>
    <message>
        <location filename="../../GraphInfoForm.ui" line="26"/>
        <source>Graph parameters</source>
        <translation>Параметры графа</translation>
    </message>
    <message>
        <location filename="../../GraphInfoForm.ui" line="45"/>
        <source>Radius:</source>
        <translation>Радиус:</translation>
    </message>
    <message>
        <location filename="../../GraphInfoForm.ui" line="109"/>
        <source>Diameter:</source>
        <translation>Диаметр:</translation>
    </message>
    <message>
        <location filename="../../GraphInfoForm.ui" line="70"/>
        <source>Before search parameters will be created short paths for each node.
After will be found diameter, radius and eccentricity statistic.</source>
        <translation>Перед поиском параметров будут созданы кратчайшие пути для каждой вершины.
После будут найдены: диаметр, радиус и статистика эксцентриситетов.</translation>
    </message>
    <message>
        <location filename="../../GraphInfoForm.ui" line="20"/>
        <location filename="../../GraphInfoForm.ui" line="293"/>
        <source>Graph info</source>
        <translation>Инормация о графе</translation>
    </message>
    <message>
        <location filename="../../GraphInfoForm.ui" line="74"/>
        <source>Create short paths</source>
        <translation>Создать кратчайшие пути</translation>
    </message>
    <message>
        <location filename="../../GraphInfoForm.ui" line="144"/>
        <source>Short paths:</source>
        <translation>Кратчайшие пути:</translation>
    </message>
    <message>
        <location filename="../../GraphInfoForm.ui" line="153"/>
        <source>Create projections</source>
        <translation>Создать проекции</translation>
    </message>
    <message>
        <location filename="../../GraphInfoForm.ui" line="130"/>
        <source>Projections:</source>
        <translation>Проекции:</translation>
    </message>
    <message>
        <location filename="../../GraphInfoForm.ui" line="192"/>
        <source>Graph degree:</source>
        <translation>Степень графа:</translation>
    </message>
    <message>
        <location filename="../../GraphInfoForm.ui" line="206"/>
        <source>Graph girth:</source>
        <translation>Обхват графа:</translation>
    </message>
    <message>
        <location filename="../../GraphInfoForm.ui" line="312"/>
        <source>File name:</source>
        <translation>Имя файла:</translation>
    </message>
    <message>
        <location filename="../../GraphInfoForm.ui" line="319"/>
        <source>Parameters:</source>
        <translation>Параметры:</translation>
    </message>
    <message>
        <location filename="../../GraphInfoForm.ui" line="340"/>
        <source>Node conut:</source>
        <translation>Количество вершин:</translation>
    </message>
    <message>
        <location filename="../../GraphInfoForm.ui" line="357"/>
        <source>Edge Count:</source>
        <translation>Количество рёбер:</translation>
    </message>
    <message>
        <location filename="../../GraphInfoForm.ui" line="380"/>
        <source>Nodes eccentricity statistic</source>
        <translation>Статистика экцентриситетов вершин</translation>
    </message>
    <message>
        <location filename="../../GraphInfoForm.ui" line="241"/>
        <location filename="../../GraphInfoForm.ui" line="398"/>
        <source>Detail info</source>
        <translation>Подробная информация</translation>
    </message>
    <message>
        <location filename="../../GraphInfoForm.ui" line="414"/>
        <source>Eccentricity</source>
        <translation>Экцентриситет</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../../MainWindow.ui" line="14"/>
        <source>Graph editor</source>
        <translation>Редактор графа</translation>
    </message>
    <message>
        <location filename="../../MainWindow.ui" line="105"/>
        <source>Open</source>
        <translation>Открыть</translation>
    </message>
    <message>
        <location filename="../../MainWindow.ui" line="152"/>
        <source>Save</source>
        <translation>Сохранить</translation>
    </message>
    <message>
        <location filename="../../MainWindow.ui" line="196"/>
        <source>Generator</source>
        <translation>Генератор</translation>
    </message>
    <message>
        <location filename="../../MainWindow.ui" line="240"/>
        <source>Statistic</source>
        <translation>Статистика</translation>
    </message>
    <message>
        <location filename="../../MainWindow.ui" line="297"/>
        <source>About</source>
        <translation>О программе</translation>
    </message>
</context>
<context>
    <name>NodeInfoForm</name>
    <message>
        <location filename="../../NodeInfoForm.ui" line="14"/>
        <source>Node info</source>
        <translation>Инормация о вершине</translation>
    </message>
    <message>
        <location filename="../../NodeInfoForm.ui" line="22"/>
        <source>Node number:</source>
        <translation>Номер вершины:</translation>
    </message>
    <message>
        <location filename="../../NodeInfoForm.ui" line="72"/>
        <source>Eccentricity:</source>
        <translation>Экцентриситет:</translation>
    </message>
    <message>
        <location filename="../../NodeInfoForm.ui" line="86"/>
        <source>Degree:</source>
        <translation>Степень:</translation>
    </message>
    <message>
        <location filename="../../NodeInfoForm.ui" line="100"/>
        <source>Shortest loop:</source>
        <translation>Кратчайший цикл:</translation>
    </message>
    <message>
        <location filename="../../NodeInfoForm.ui" line="146"/>
        <source>Projection</source>
        <translation>Проекция</translation>
    </message>
    <message>
        <location filename="../../NodeInfoForm.ui" line="157"/>
        <source>Create projection</source>
        <translation>Создать проекцию</translation>
    </message>
    <message>
        <location filename="../../NodeInfoForm.ui" line="201"/>
        <source>level</source>
        <translation>уровень</translation>
    </message>
    <message>
        <location filename="../../NodeInfoForm.ui" line="206"/>
        <source>original</source>
        <translation>оригинал</translation>
    </message>
    <message>
        <location filename="../../NodeInfoForm.ui" line="211"/>
        <source>replica</source>
        <translation>реплика</translation>
    </message>
    <message>
        <location filename="../../NodeInfoForm.ui" line="216"/>
        <source>total</source>
        <translation>всего</translation>
    </message>
    <message>
        <location filename="../../NodeInfoForm.ui" line="118"/>
        <source>Adjacent nodes</source>
        <translation>Смежные вершины</translation>
    </message>
    <message>
        <location filename="../../NodeInfoForm.cpp" line="30"/>
        <source>Graph is empty.</source>
        <translation>Граф пуст.</translation>
    </message>
    <message>
        <location filename="../../NodeInfoForm.cpp" line="93"/>
        <source>Start node is not number.</source>
        <translation>Начальная вершина не является числом.</translation>
    </message>
</context>
<context>
    <name>OpenFileForm</name>
    <message>
        <location filename="../../OpenFileForm.ui" line="22"/>
        <source>Choose file</source>
        <translation>Выбрать файл</translation>
    </message>
    <message>
        <location filename="../../OpenFileForm.ui" line="35"/>
        <location filename="../../OpenFileForm.cpp" line="54"/>
        <source>No file</source>
        <translation>Нет файла</translation>
    </message>
    <message>
        <location filename="../../OpenFileForm.ui" line="57"/>
        <source>File info</source>
        <translation>Информация о файле</translation>
    </message>
    <message>
        <location filename="../../OpenFileForm.ui" line="14"/>
        <location filename="../../OpenFileForm.ui" line="111"/>
        <location filename="../../OpenFileForm.cpp" line="39"/>
        <source>Open file</source>
        <translation>Открыть файл</translation>
    </message>
    <message>
        <location filename="../../OpenFileForm.cpp" line="39"/>
        <source>Text files (*.txt)</source>
        <translation>Текстовые файлы (*.txt)</translation>
    </message>
    <message>
        <location filename="../../OpenFileForm.cpp" line="73"/>
        <source>Can&apos;t read file %1</source>
        <translation>Невозможно прочитать файл %1</translation>
    </message>
    <message>
        <location filename="../../OpenFileForm.cpp" line="75"/>
        <source>Can&apos;t read file. Please check that there is a right to read.</source>
        <translation>Невозможно прочитать файл. Пожалуйста проверьте права на чтение.</translation>
    </message>
    <message>
        <location filename="../../OpenFileForm.cpp" line="85"/>
        <source>Can&apos;t read file. File type is not recognised.</source>
        <translation>Невозможно прочитать файл. Тип файла не распознан.</translation>
    </message>
    <message>
        <location filename="../../OpenFileForm.cpp" line="90"/>
        <source>File type = %1
</source>
        <translation>Тип файла = %1
</translation>
    </message>
    <message>
        <location filename="../../OpenFileForm.cpp" line="118"/>
        <source>Lines in file: %1
</source>
        <translation>Строк в файле: %1
</translation>
    </message>
    <message>
        <location filename="../../OpenFileForm.cpp" line="119"/>
        <source>File size: %1 bytes</source>
        <translation>Размер файла: %1 байт</translation>
    </message>
    <message>
        <location filename="../../OpenFileForm.cpp" line="137"/>
        <source>Clearing graph...</source>
        <translation>Очистка графа...</translation>
    </message>
    <message>
        <location filename="../../OpenFileForm.cpp" line="146"/>
        <location filename="../../OpenFileForm.cpp" line="147"/>
        <source>File &quot;%1&quot; readed</source>
        <translation>Файл %1 прочитан</translation>
    </message>
    <message>
        <location filename="../../OpenFileForm.cpp" line="156"/>
        <source>Can&apos;t read file.</source>
        <translation>Невозможно прочитать файл.</translation>
    </message>
    <message>
        <location filename="../../OpenFileForm.cpp" line="159"/>
        <source>File have unknown type.</source>
        <translation>Тип файла неизвестен.</translation>
    </message>
    <message>
        <location filename="../../OpenFileForm.cpp" line="162"/>
        <source>File have syntax error.</source>
        <translation>В файле синтаксическая ошибка.</translation>
    </message>
    <message>
        <location filename="../../OpenFileForm.cpp" line="165"/>
        <source>Unknown error.</source>
        <translation>Неизвестная ошибка.</translation>
    </message>
    <message>
        <location filename="../../OpenFileForm.cpp" line="169"/>
        <source>
Check log file to get more information about error.
By default file graph.log located in same program directory.</source>
        <translation>
Проверьте файл логов для получения большей информации об ошибке.
По умолчанию файл graph.log расположен в директории программы.</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../../GraphWorker.cpp" line="32"/>
        <source>Cancel</source>
        <translation>Отмена</translation>
    </message>
    <message>
        <location filename="../../GraphWorker.cpp" line="66"/>
        <source>Creating short path</source>
        <translation>Создание кратчайшего пути</translation>
    </message>
    <message>
        <location filename="../../GraphWorker.cpp" line="84"/>
        <source>Creating projections</source>
        <translation>Создание проекций</translation>
    </message>
    <message>
        <location filename="../../GraphWorker.cpp" line="99"/>
        <source>Writing edges of node</source>
        <translation>Запись рёбер вершины</translation>
    </message>
    <message>
        <location filename="../../GraphWorker.cpp" line="115"/>
        <location filename="../../GraphWorker.cpp" line="136"/>
        <source>Writing adjacency list</source>
        <translation>Запись списка смежности</translation>
    </message>
    <message>
        <location filename="../../GraphWorker.cpp" line="158"/>
        <source>Writing projections</source>
        <translation>Запись проекций</translation>
    </message>
    <message>
        <location filename="../../GraphWorker.cpp" line="184"/>
        <source>Writing projection</source>
        <translation>Запись проекции</translation>
    </message>
    <message>
        <location filename="../../GraphWorker.cpp" line="209"/>
        <source>Node %1 not found.</source>
        <translation>Вершина %1 не найдена.</translation>
    </message>
    <message>
        <location filename="../../GraphWorker.cpp" line="214"/>
        <source>Nearest:</source>
        <translation>Ближайшие:</translation>
    </message>
</context>
<context>
    <name>SaveForm</name>
    <message>
        <location filename="../../SaveForm.ui" line="14"/>
        <source>Save to file</source>
        <translation>Сохранить в файл</translation>
    </message>
    <message>
        <location filename="../../SaveForm.ui" line="102"/>
        <source>Graph</source>
        <translation>Граф</translation>
    </message>
    <message>
        <location filename="../../SaveForm.ui" line="139"/>
        <source>Short path</source>
        <translation>Кратчайший
путь</translation>
    </message>
</context>
<context>
    <name>SaveGraphForm</name>
    <message>
        <location filename="../../SaveGraphForm.ui" line="28"/>
        <source>File type</source>
        <translation>Тип файла</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.ui" line="34"/>
        <source>Edge list</source>
        <translation>Список рёбер</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.ui" line="44"/>
        <source>Adjacency list</source>
        <translation>Список смежности</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.ui" line="51"/>
        <source>Projections</source>
        <translation>Проекции</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.ui" line="58"/>
        <source>Brackets style, all paths</source>
        <translation>Скобочная запись, все пути</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.ui" line="87"/>
        <source>Options</source>
        <translation>Настройки</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.ui" line="93"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;

Print value used to print value for each edge. &lt;br&gt;
It useful if graph has different weight volumes. &lt;br&gt;
It useful turned off if graph has one weight.&lt;br&gt;
&lt;table&gt;
&lt;tr&gt;
&lt;td&gt;
If turned on:&lt;br&gt;
1 2 5&lt;br&gt;
1 3 1&lt;br&gt;
2 3 2&lt;br&gt;
or 1(2[5] 3[7] 4[9])
&lt;/td&gt;
&lt;td&gt;
If turned off:&lt;br&gt;
1 2&lt;br&gt;
1 3&lt;br&gt;
2 3&lt;br&gt;
or 1(2 3 4)&lt;br&gt;
&lt;/td&gt;
&lt;/tr&gt;
&lt;/table&gt;
&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;

Печать веса используется для печати веса каждого ребра. &lt;br&gt;
Это используется, если в графе веса имеют разные значения. &lt;br&gt;
Полезно выключить, если граф имеет один вес всех ребер.&lt;br&gt;
&lt;table&gt;
&lt;tr&gt;
&lt;td&gt;
Если включено:&lt;br&gt;
1 2 5&lt;br&gt;
1 3 1&lt;br&gt;
2 3 2&lt;br&gt;
or 1(2[5] 3[7] 4[9])
&lt;/td&gt;
&lt;td&gt;
Если выключено:&lt;br&gt;
1 2&lt;br&gt;
1 3&lt;br&gt;
2 3&lt;br&gt;
or 1(2 3 4)&lt;br&gt;
&lt;/td&gt;
&lt;/tr&gt;
&lt;/table&gt;
&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.ui" line="119"/>
        <source>Print value</source>
        <translation>Печать веса</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.ui" line="129"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Prints graph additional information. &lt;/p&gt;&lt;p&gt;For example: &lt;br/&gt;name of generated type (hypercube torus); &lt;br/&gt;parameters, used by generator (dimention, big raidus, small radius);&lt;br/&gt;basic weight of all edges;&lt;/p&gt;&lt;p&gt;Example in file:&lt;/p&gt;&lt;p&gt;{TYPE=HYPERCUBE}&lt;br/&gt;{DIMENTION=6}&lt;br/&gt;{WEIGHT=1}&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Печать дополнительной информации о графе. &lt;/p&gt;&lt;p&gt;Например: &lt;br/&gt;название сгенерированного типа графа (гиперкуб тор); &lt;br/&gt;параметры, введенные в генератор (размерность, большой радиус, малый радиус);&lt;br/&gt;базовый вес всех рёбер;&lt;/p&gt;&lt;p&gt;Пример в файле:&lt;/p&gt;&lt;p&gt;{TYPE=HYPERCUBE}&lt;br/&gt;{DIMENTION=6}&lt;br/&gt;{WEIGHT=1}&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.ui" line="132"/>
        <source>Print graph info</source>
        <translation>Печать информации о графе</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.ui" line="142"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;
&lt;p&gt;Print indents for more readable data. Used in bracket style.&lt;br/&gt;
Each new nesting increase indent lewel.&lt;/p&gt;
&lt;p&gt;
Example:
&lt;table&gt;
&lt;tr&gt;
&lt;td&gt;
Turned off:&lt;br&gt;
1(2(3(4 5) 7))
&lt;/td&gt;
&lt;td&gt;
Turned on:&lt;br&gt;
1(&lt;br&gt;
&amp;middot; &amp;middot; 2(&lt;br&gt;
&amp;middot; &amp;middot; &amp;middot; &amp;middot; 3(&lt;br&gt;
&amp;middot; &amp;middot; &amp;middot; &amp;middot; &amp;middot; &amp;middot; 4&lt;br&gt; 
&amp;middot; &amp;middot; &amp;middot; &amp;middot; &amp;middot; &amp;middot; 5&lt;br&gt;
&amp;middot; &amp;middot; &amp;middot; &amp;middot; ) &lt;br&gt;
&amp;middot; &amp;middot; &amp;middot; &amp;middot; 7&lt;br&gt;
&amp;middot; &amp;middot; )&lt;br&gt;
)
&lt;/td&gt;
&lt;/tr&gt;
&lt;/table&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;
&lt;p&gt;Печать отступов используется для более читаемого вида данных. Используется в скобочной записи.&lt;br/&gt;
Каждое новое вложение увеличивает уровень отступа.&lt;/p&gt;
&lt;p&gt;
Пример:
&lt;table&gt;
&lt;tr&gt;
&lt;td&gt;
Выключено:&lt;br&gt;
1(2(3(4 5) 7))
&lt;/td&gt;
&lt;td&gt;
Включено:&lt;br&gt;
1(&lt;br&gt;
&amp;middot; &amp;middot; 2(&lt;br&gt;
&amp;middot; &amp;middot; &amp;middot; &amp;middot; 3(&lt;br&gt;
&amp;middot; &amp;middot; &amp;middot; &amp;middot; &amp;middot; &amp;middot; 4&lt;br&gt; 
&amp;middot; &amp;middot; &amp;middot; &amp;middot; &amp;middot; &amp;middot; 5&lt;br&gt;
&amp;middot; &amp;middot; &amp;middot; &amp;middot; ) &lt;br&gt;
&amp;middot; &amp;middot; &amp;middot; &amp;middot; 7&lt;br&gt;
&amp;middot; &amp;middot; )&lt;br&gt;
)
&lt;/td&gt;
&lt;/tr&gt;
&lt;/table&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.ui" line="169"/>
        <source>Print indents</source>
        <translation>Печать отступов</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.ui" line="176"/>
        <source>Print all nodes</source>
        <translation>Печать всех вершин</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.ui" line="185"/>
        <source>Start node</source>
        <translation>Начальная вершина</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.ui" line="198"/>
        <source>Node id for start writing bracket style paths.</source>
        <translation>Номер вершины для начала пути в скобочной записи.</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.ui" line="211"/>
        <source>Path Limit</source>
        <translation>Ограничение пути</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.ui" line="224"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Path limit. It&apos;s nesting level limiter. &lt;/p&gt;&lt;p&gt;Useful if need crop graph or give some graph part that accessible in not more than &amp;quot;path limit&amp;quot; edges.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Ограничение пути - это граничение уровня вложения. &lt;/p&gt;&lt;p&gt;Ипользуется, если нужно обрезать граф или получить часть пути которая достижима не более чем в &amp;quot;ограничение пути&amp;quot; ребер.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.ui" line="270"/>
        <source>Example</source>
        <translation>Пример</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.ui" line="14"/>
        <location filename="../../SaveGraphForm.ui" line="322"/>
        <location filename="../../SaveGraphForm.cpp" line="454"/>
        <source>Save graph</source>
        <translation>Сохранить граф</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.cpp" line="409"/>
        <source>File type is not recognised. Please check save settings</source>
        <translation>Тип файла не распознан. Пожалуйста проверьте настройки сохранения</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.cpp" line="423"/>
        <source>Graph is empty. Nothing to save.</source>
        <translation>Граф пуст. Сохранять нечего.</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.cpp" line="168"/>
        <location filename="../../SaveGraphForm.cpp" line="212"/>
        <location filename="../../SaveGraphForm.cpp" line="261"/>
        <source>Print value and print info are turned off.
Weight and other information will be lost.</source>
        <translation>Печать веса и печать информации отключены.
Вес и другая информация будут потеряны.</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.cpp" line="318"/>
        <source>Not all projections are created yet. Will be created on save. It&apos;s take a time.</source>
        <translation>Не все проекции ещё созданы. Будут созданы при сохранениию Это займёт время.</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.cpp" line="331"/>
        <source>Projection not exist yet. Will be created on save.</source>
        <translation>Проекция ещё не создана. Будет создана при сохранении.</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.cpp" line="372"/>
        <location filename="../../SaveGraphForm.cpp" line="390"/>
        <source>Start node is not number.</source>
        <translation>Начальная вершина не является числом.</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.cpp" line="455"/>
        <source>Text files (*.txt)</source>
        <translation>Текстовые файлы (*.txt)</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.cpp" line="537"/>
        <source>File type is unknown. Check settings.</source>
        <translation>Тип файла неизвестен. Проверьте настройки.</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.cpp" line="543"/>
        <location filename="../../SaveGraphForm.cpp" line="544"/>
        <source>Saving interrupted.</source>
        <translation>Сохранение прервано.</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.cpp" line="550"/>
        <source>Graph saved in file %1</source>
        <translation>Граф сохранён в файле %1</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.cpp" line="555"/>
        <source>Graph not saved.File not accessed to write.</source>
        <translation>Граф не сохранён. Нет прав на запись в файл.</translation>
    </message>
    <message>
        <location filename="../../SaveGraphForm.cpp" line="557"/>
        <source>Graph not saved.</source>
        <translation>Граф не сохранён.</translation>
    </message>
</context>
<context>
    <name>SaveShortPathForm</name>
    <message>
        <location filename="../../SaveShortPathForm.ui" line="14"/>
        <source>Save short path</source>
        <translation>Сохранить кратчайший путь</translation>
    </message>
    <message>
        <location filename="../../SaveShortPathForm.ui" line="22"/>
        <source>Short path</source>
        <translation>Кратчайший путь</translation>
    </message>
    <message>
        <location filename="../../SaveShortPathForm.ui" line="30"/>
        <source>Start node:</source>
        <translation>Начальная вершина:</translation>
    </message>
    <message>
        <location filename="../../SaveShortPathForm.ui" line="37"/>
        <source>Node id for start writing bracket style paths.</source>
        <translation>Номер вершины для начала пути в скобочной записи.</translation>
    </message>
    <message>
        <location filename="../../SaveShortPathForm.ui" line="47"/>
        <source>Path weight limit:</source>
        <translation>Ограничение веса пути:</translation>
    </message>
    <message>
        <location filename="../../SaveShortPathForm.ui" line="54"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Path weight limit. It&apos;s nesting weight limiter. &lt;/p&gt;&lt;p&gt;Useful if need crop graph or give some graph part that &lt;br/&gt;accessible in not larger than &amp;quot;path weight limit&amp;quot; edges weight sum.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Ограничение веса пути - это граничение уровня вложения. &lt;/p&gt;&lt;p&gt;Ипользуется, если нужно обрезать граф или получить часть пути которая по весу не более чем в &amp;quot;ограничение веса пути&amp;quot; суммы весов ребер.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../../SaveShortPathForm.ui" line="66"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;
&lt;p&gt;Print indents for more readable data. Used in bracket style.&lt;br/&gt;
Each new nesting increase indent lewel.&lt;/p&gt;
&lt;p&gt;
Example:
&lt;table&gt;
&lt;tr&gt;
&lt;td&gt;
Turned off:&lt;br&gt;
1(2[3](3[5](4[9] 5[6]) 7[4]))
&lt;/td&gt;
&lt;td&gt;
Turned on:&lt;br&gt;
1(&lt;br&gt;
&amp;middot; &amp;middot; 2[3](&lt;br&gt;
&amp;middot; &amp;middot; &amp;middot; &amp;middot; 3[5](&lt;br&gt;
&amp;middot; &amp;middot; &amp;middot; &amp;middot; &amp;middot; &amp;middot; 4[9]&lt;br&gt; 
&amp;middot; &amp;middot; &amp;middot; &amp;middot; &amp;middot; &amp;middot; 5[6]&lt;br&gt;
&amp;middot; &amp;middot; &amp;middot; &amp;middot; ) &lt;br&gt;
&amp;middot; &amp;middot; &amp;middot; &amp;middot; 7[4]&lt;br&gt;
&amp;middot; &amp;middot; )&lt;br&gt;
)
&lt;/td&gt;
&lt;/tr&gt;
&lt;/table&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;
&lt;p&gt;Печать отступов используется для более читаемого вида данных. Используется в скобочной записи.&lt;br/&gt;
Каждое новое вложение увеличивает уровень отступа.&lt;/p&gt;
&lt;p&gt;
Пример:
&lt;table&gt;
&lt;tr&gt;
&lt;td&gt;
Выключено:&lt;br&gt;
1(2[3](3[5](4[9] 5[6]) 7[4]))
&lt;/td&gt;
&lt;td&gt;
Включено:&lt;br&gt;
1(&lt;br&gt;
&amp;middot; &amp;middot; 2[3](&lt;br&gt;
&amp;middot; &amp;middot; &amp;middot; &amp;middot; 3[5](&lt;br&gt;
&amp;middot; &amp;middot; &amp;middot; &amp;middot; &amp;middot; &amp;middot; 4[9]&lt;br&gt; 
&amp;middot; &amp;middot; &amp;middot; &amp;middot; &amp;middot; &amp;middot; 5[6]&lt;br&gt;
&amp;middot; &amp;middot; &amp;middot; &amp;middot; ) &lt;br&gt;
&amp;middot; &amp;middot; &amp;middot; &amp;middot; 7[4]&lt;br&gt;
&amp;middot; &amp;middot; )&lt;br&gt;
)
&lt;/td&gt;
&lt;/tr&gt;
&lt;/table&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../../SaveShortPathForm.ui" line="93"/>
        <source>Print indents</source>
        <translation>Печать отступов</translation>
    </message>
    <message>
        <location filename="../../SaveShortPathForm.ui" line="116"/>
        <source>Example</source>
        <translation>Пример</translation>
    </message>
    <message>
        <location filename="../../SaveShortPathForm.ui" line="165"/>
        <source>Save</source>
        <translation>Сохранить</translation>
    </message>
    <message>
        <location filename="../../SaveShortPathForm.cpp" line="56"/>
        <source>Graph is empty. Nothing to save.</source>
        <translation>Граф пуст. Сохранять нечего.</translation>
    </message>
    <message>
        <location filename="../../SaveShortPathForm.cpp" line="71"/>
        <location filename="../../SaveShortPathForm.cpp" line="128"/>
        <source>Start node is not number.</source>
        <translation>Начальная вершина не является числом.</translation>
    </message>
    <message>
        <location filename="../../SaveShortPathForm.cpp" line="78"/>
        <location filename="../../SaveShortPathForm.cpp" line="135"/>
        <source>Path limit is not number.</source>
        <translation>Ограничение пути не является числом.</translation>
    </message>
    <message>
        <location filename="../../SaveShortPathForm.cpp" line="92"/>
        <source>Path not created. It will be created before save.</source>
        <translation>Путь не создан. Будет создан перед сохранением.</translation>
    </message>
    <message>
        <location filename="../../SaveShortPathForm.cpp" line="139"/>
        <source>Save graph</source>
        <translation>Сохранить граф</translation>
    </message>
    <message>
        <location filename="../../SaveShortPathForm.cpp" line="140"/>
        <source>Text files (*.txt)</source>
        <translation>Текстовые файлы (*.txt)</translation>
    </message>
    <message>
        <location filename="../../SaveShortPathForm.cpp" line="169"/>
        <source>Short path saved in file %1</source>
        <translation>Кратчайший путь сохранён в файле %1</translation>
    </message>
    <message>
        <location filename="../../SaveShortPathForm.cpp" line="174"/>
        <source>Short path not saved.File not accessed to write.</source>
        <translation>Кратчайший путь не сохранён. Нет прав на запись в файл.</translation>
    </message>
    <message>
        <location filename="../../SaveShortPathForm.cpp" line="176"/>
        <source>Short path not saved.</source>
        <translation>Кратчайший путь не сохранён.</translation>
    </message>
</context>
<context>
    <name>StatisticMenuForm</name>
    <message>
        <location filename="../../StatisticMenuForm.ui" line="14"/>
        <source>Statistic</source>
        <translation>Статистика</translation>
    </message>
    <message>
        <location filename="../../StatisticMenuForm.ui" line="102"/>
        <source>Graph</source>
        <translation>Граф</translation>
    </message>
    <message>
        <location filename="../../StatisticMenuForm.ui" line="139"/>
        <source>Node</source>
        <translation>Вершина</translation>
    </message>
</context>
</TS>
