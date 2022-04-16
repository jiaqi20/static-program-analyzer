# $autotester = "..\Code37\out\build\x64-Release\src\autotester\autotester.exe"
# $source = ".\simple_source_code.txt"
# $queryPrefix = ".\simple_"
# $querySuffix = "_query.txt"
# $outputPrefix = ".\out\simple_"
# $outputSuffix = "_out.xml"

$autotester = "..\..\Code37\out\build\x64-Release\src\autotester\autotester.exe"
$source = ".\simple_source_code.txt"
$queryPrefix = ".\simple_"
$querySuffix = "_query.txt"
$outputPrefix = "..\out\simple_"
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
$type = "no_condition"
&$autotester $source $queryPrefix$type$querySuffix $outputPrefix$type$outputSuffix
$type = "multiconditions"
&$autotester $source $queryPrefix$type$querySuffix $outputPrefix$type$outputSuffix