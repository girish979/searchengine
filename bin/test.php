<?php
$mystring = '../toIndex/0/5514.text';

$t1 = strrev ($mystring);
$pos = strpos($t1, "/");

$len = strlen ($mystring);
$ap = $len - $pos;

$oo = "";
while($ap<$len)
{
	$oo = $oo .  $mystring[$ap];
	$ap = $ap +1;
}

echo "$oo<br><br>";

$findme   = 'a';
$pos = strpos($mystring, $findme);

// Note our use of ===.  Simply == would not work as expected
// because the position of 'a' was the 0th (first) character.
if ($pos === false) {
    echo "The string '$findme' was not found in the string '$mystring'";
} else {
    echo "The string '$findme' was found in the string '$mystring'";
    echo " and exists at position $pos";
}
?>

