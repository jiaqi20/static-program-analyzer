# $autotester = "..\Code37\out\build\x64-Release\src\autotester\autotester.exe"
# $source = ".\iter1_nested_while_source.txt"
# $queryPrefix = ".\iter1_nested_while_"
# $querySuffix = "_queries.txt"
# $outputPrefix = ".\out\iter1_nested_while_"
# $outputSuffix = "_out.xml"

$autotester = "..\..\Code37\out\build\x64-Release\src\autotester\autotester.exe"
$source = ".\iter1_nested_while_source.txt"
$queryPrefix = ".\iter1_nested_while_"
$querySuffix = "_queries.txt"
$outputPrefix = "..\out\iter1_nested_while_"
$outputSuffix = "_out.xml"

$type = "modifies"
&$autotester $source $queryPrefix$type$querySuffix $outputPrefix$type$outputSuffix
$type = "uses"
&$autotester $source $queryPrefix$type$querySuffix $outputPrefix$type$outputSuffix
$type = "parents"
&$autotester $source $queryPrefix$type$querySuffix $outputPrefix$type$outputSuffix
$type = "follows"
&$autotester $source $queryPrefix$type$querySuffix $outputPrefix$type$outputSuffix
$type = "pattern"
&$autotester $source $queryPrefix$type$querySuffix $outputPrefix$type$outputSuffix
$type = "general"
&$autotester $source $queryPrefix$type$querySuffix $outputPrefix$type$outputSuffix