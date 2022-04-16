$autotester = "..\..\Code37\out\build\x64-Release\src\autotester\autotester.exe"
$source1 = ".\stress_source1.txt"
$source2 = ".\stress_source2.txt"

$queryPrefix1 = ".\stress1"
$queryPrefix2 = ".\stress2"
$querySuffix = "_queries.txt"

$outPrefix1 = "..\out\stress1"
$outPrefix2 = "..\out\stress2"
$outSuffix = "_out.xml"

&$autotester $source1 $queryPrefix1$querySuffix $outPrefix1$outSuffix

&$autotester $source2 $queryPrefix2$querySuffix $outPrefix2$outSuffix
