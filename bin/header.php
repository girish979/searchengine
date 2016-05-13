<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<title>CS 570 Assignment</title>
<link href="css.css" rel="stylesheet" type="text/css">
</head>

<body>
<div align="center">
  <p>&nbsp;</p>
  <p><a href="<?php echo "http://172.16.26.31/se/bin/";?>"><img src="images/logo.jpeg" border=0></a><br>
      </p>
</div>
<form name="googleFrm" method="get" action="search.php">
  <div align="center">
	<span class="white_bold_small"><p align="center"><b>Web</b><font size="-1">&nbsp;&nbsp;&nbsp;&nbsp;</font><a href="#"><span style="text-decoration: none">Images</span></a><font size="-1">&nbsp;&nbsp;&nbsp;&nbsp;</font><a href="#"><span style="text-decoration: none">Video</span></a><font size="-1">&nbsp;&nbsp;&nbsp;&nbsp;</font><a href="#"><span style="text-decoration: none">Advanced Search</span></a><br>
  </span>

    <input name="query" onfocus="this.value='';" type="text" id="query" size="60" value="Start searching here, then press Enter"> 
        <br>
    &nbsp;<br>
      <font size=3><INPUT TYPE=RADIO NAME="rtype" VALUE="tf">TF IDF</font> &nbsp;&nbsp;&nbsp;&nbsp;
      <font size=3><INPUT TYPE=RADIO NAME="rtype" VALUE="bm">BM25</font>

    <br>
    <br><input type="submit" value="Search"></p></div>
</form>
<br><br><br><br><br><br><br><br>
