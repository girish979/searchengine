<?php


	$input = $_GET['query'];
	$path = "./patricia -s -t ";
	$q = $path . $input;

#print "input $q";

//system('./a.out', $retval);


/* $out = array();
 exec = ('ls', $out);
 print_r($out);*/


$output = shell_exec($q);
//echo "$output";

$op = preg_split('/\n+/',$output,-1,PREG_SPLIT_NO_EMPTY);

//echo "$op[0]";

foreach ($op as &$value) 
{
    echo "$value<br>";
}



#print "<br><br> $retval";


?>
