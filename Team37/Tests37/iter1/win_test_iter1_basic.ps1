# $autotester = "..\Code37\out\build\x64-Release\src\autotester\autotester.exe"
# $source = ".\iter1_basic_source.txt"
# $queryPrefix = ".\iter1_basic_"
# $querySuffix = "_queries.txt"
# $outputPrefix = ".\out\iter1_basic_"
# $outputSuffix = "_out.xml"

$autotester = "..\..\Code37\out\build\x64-Release\src\autotester\autotester.exe"
$source = ".\iter1_basic_source.txt"
$queryPrefix = ".\iter1_basic_"
$querySuffix = "_queries.txt"
$outputPrefix = "..\out\iter1_basic_"
$outputSuffix = "_out.xml"

$type = "modifies"
&$autotester $source $queryPrefix$type$querySuffix $outputPrefix$type$outputSuffix
$type = "uses"
&$autotester $source $queryPrefix$type$querySuffix $outputPrefix$type$outputSuffix
$type = "parent"
&$autotester $source $queryPrefix$type$querySuffix $outputPrefix$type$outputSuffix
$type = "follows"
&$autotester $source $queryPrefix$type$querySuffix $outputPrefix$type$outputSuffix
$type = "pattern"
&$autotester $source $queryPrefix$type$querySuffix $outputPrefix$type$outputSuffix
$type = "general"
&$autotester $source $queryPrefix$type$querySuffix $outputPrefix$type$outputSuffix
$type = "combine"
&$autotester $source $queryPrefix$type$querySuffix $outputPrefix$type$outputSuffix