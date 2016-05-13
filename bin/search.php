<?

	$input = $_GET['query'];
	$start = $_GET['start'];

	if (isset($_GET['rtype'])) 
		$selected_radio = $_GET['rtype'];
	else
		$selected_radio = "tf"; //Default type to TF-IDF

//echo "$selected_radio";

	if ($input) 
	{

		if ($selected_radio == "tf")
			$path = "./patricia -s -t ";
		else
			$path = "./patricia -s -b ";
		$q = $path . $input;
	
		$output = shell_exec($q);
		$result = preg_split('/\n+/',$output,-1,PREG_SPLIT_NO_EMPTY);
		$total = count($result);
	
		if($start >=0 )
			$begin = $start + 1;
		else 
		{
			$start = 0;
			$begin = $start + 1;
		}
		if($total<10)
			$end = $total;
		else
			$end = $start + 10;
	
	
	}


?>



<?php include("header_search.php");?>


<? 


if ($input) 
{
#609
	echo "<table width=\"90%\" cellspacing=\"0\" cellpadding=\"10\" border=\"0\">\n";
	echo "<tr class=\"row\"><td width=\"90%\" class=\"row\"></td></tr>\n";
  

	if ($total == 0)
	{
		echo "<tr><td class=\"alert\">Your search returned no results ..</td></tr>\n";
	}
  

	if ($total > 0) 
	{
		//$begin =1;
		if ( ($total - $end) >0)
			$c =10;
		else if ($total <10)
			$c = $total;
		else if($total - $end <10)
			$c =$total - $end +1;
		
		for($i=0;$i<$c;$i++)//for ($i = $start+1; $i <= $end ; $i++) //foreach ($result as &$value) 
		{
			$x = $i+$begin -1;
			$element = "element";
			$url = "/se/bin/". $result[$x];
			$title = $result[$x];
			
			$t1 = strrev ($title);
			$pos = strpos($t1, "/");
			
			$len = strlen ($title);
			$ap = $len - $pos;
			$oo = "";
			while($ap<$len)
			{
				$oo = $oo .  $title[$ap];
				$ap = $ap +1;
			}
			
			//$title = $oo;
			
			//echo "--$oo ---";

			//Start Snippet view
			//$snippet = "snippet view";
			$bodytag = str_replace(" ", "|", $input);
			//echo "$bodytag";
			$sq = "egrep -e ". "\"". $bodytag."\" " ." ". $title ;
			//echo "$sq<br>";
			$sv_op = shell_exec($sq);
			
			$sub = substr($sv_op, -1*(strlen($sv_op)-1) , 175);
			
			if(strlen($sv_op) < 175)
			$snippet = $sv_op;
			
			$snippet = $sub;#$sv_op;
			//End of Snippet View
			
			
			$title = $oo;
			if ($url != "") 
			{
				echo "<tr>\n"; 
	
				echo "<td><span class='list'>".($x+1).".</span> <a href='$url' class='link'>$title</a><br><span class='gray_small'>$snippet</span><br><a href='$url' class='link_small'>$url</a></td></tr>\n";
			}    
			
		}
	}
       

	echo "<tr><td><br>";
	if($begin > 1)
	{ 
		echo "<a href=\"search.php?query=$input&rtype=$selected_radio&start=".($start - 10)."\">previous 10 results</a> | "; 
	} 
	
	if($total != 0)
		echo "showing $begin to $end of $total results";
	else
		echo "showing 0 to $end of $total results";

	if ($end < $total) 
	{ 
		echo " | <a href=\"search.php?query=$input&rtype=$selected_radio&start=".($start + 10)."\">next 10 results</a>"; 
	}
	echo "</td></tr>\n";
  
	echo "</table>";
}


?>


<?php include("footer_search.php");?>
