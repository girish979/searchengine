<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<title>IR 570</title>
<link href="css.css" rel="stylesheet" type="text/css">
</head>

<body>

<table width="100%"  border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td width="250"><a href="<?php echo "http://172.16.26.31/se/bin/";?>"><img src="images/logo.jpeg" border="0" WIDTH=300 HEIGHT=100></a></td>
    <td><form name="googleFrm" method="get" action="search.php" ">
      <table width="100%"  border="0" cellspacing="0" cellpadding="10">
        <tr>
          <td><span class="white_bold_small"><p align="center"><b>Web</b><font size="-1">&nbsp;&nbsp;&nbsp;&nbsp;</font><a href="#"><span style="text-decoration: none">Images</span></a><font size="-1">&nbsp;&nbsp;&nbsp;&nbsp;</font><a href="#"><span style="text-decoration: none">Video</span></a><font size="-1">&nbsp;&nbsp;&nbsp;&nbsp;</font><a href="#"><span style="text-decoration: none">Advanced Search</span></a><br>
  </span><br>
            <input name="query" type="text" id="query" value="<?php echo $_GET['query'];?>" size="40">
              <input type="submit" value="Search Again">
              <?
              	$selected_radio = $_GET['rtype'];
              	if($selected_radio =="tf")
              	{
              		$str1 = "checked";
             		$str2 = "unchecked";
              	}
              	else if($selected_radio =="bm")
              	{
              		$str1 = "unchecked";
             		$str2 = "checked";

              	}
              	else{$str1 = "unchecked"; $str2 = "unchecked";}
              ?>
              
              
              	<font size=3><INPUT TYPE=RADIO NAME="rtype" VALUE="tf" <? print $str1; ?> >TF IDF</font> &nbsp;&nbsp;&nbsp;&nbsp;
      		<font size=3><INPUT TYPE=RADIO NAME="rtype" VALUE="bm" <? print $str2; ?> >BM25</font>
              </td>
        </tr>
      </table>
    </form></td>
  </tr>
</table>
<table width="100%"  border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td height="1" colspan="4" class="td_search_color"></td>
  </tr>
  <tr class="td_search_color">
    <td width="10" bgcolor="#e5ecf9">&nbsp;</td>
    <td height="25" bgcolor="#e5ecf9"><strong>Search Results</strong></td>
    <td height="25" bgcolor="#e5ecf9"><div align="right">Results <?php if($total != 0 )echo "$begin"; else echo "0";?> 
		- <?php echo "$end";?> of about <?php echo "$total";?> results</i> for <i><?php echo $input;?></i></div></td>
    <td width="10" bgcolor="#e5ecf9">&nbsp;</td>
  </tr>
</table><br>
<table width="100%"  border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td>
