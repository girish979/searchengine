</td>
<td background="images/blu_line.gif" width="10"></td>
    <td width="200" valign="top"><div align="center">Sponsored Links</div><br>
	
<script type="text/javascript"><!--
google_ad_client = "pub-6537087724904574";
/* 120x600, created 3/22/11 */
google_ad_slot = "0683526682";
google_ad_width = 120;
google_ad_height = 600;
//-->
</script>
<script type="text/javascript"
src="http://pagead2.googlesyndication.com/pagead/show_ads.js">
</script>
	
	</td>
  </tr>
</table>
<p>&nbsp;</p>
      <table width="100%"  border="0" cellspacing="0" cellpadding="10" id="table1">
        <tr>
          <td bgcolor="#e5ecf9">
			<p align="center">
            <td align="center" bgcolor="#e5ecf9"><form name="googleFrm" method="get" action="search.php">
      <table width="100%"  border="0" cellspacing="0" cellpadding="10">
        <tr>
          <td>
			<p align="center">
			<img border="0" src="/images/logo_small.gif"> <input name="query" type="text" id="query" value="<?php echo $_GET['query'];?>" size="40">
              <input type="submit" value="Search Again"><br> <br>
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
</body>
</table>
<p align="center">Designed By Girish</p>

</html>
