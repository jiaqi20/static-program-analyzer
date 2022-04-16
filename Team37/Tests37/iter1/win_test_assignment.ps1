# For no subfolder
# $autotester = "..\Code37\out\build\x64-Release\src\autotester\autotester.exe"
# $source = ".\assignment.txt"
# $query = ".\assignment_query.txt"
# $out = ".\out\assignment_test_out.xml"
# &$autotester $source $query $out

$autotester = "..\..\Code37\out\build\x64-Debug\src\autotester\autotester.exe"
$source = ".\assignment.txt"
$query = ".\assignment_query.txt"
$out = "..\out\assignment_test_out.xml"
&$autotester $source $query $out