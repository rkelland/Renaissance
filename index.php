<html>
<head>
   <title>Data of Sensor</title>
</head>
<body>
<h1>Data from the Sensor</h1>

<!-- Manually add data to the table
<form action="add.php" method="get">
<table>
<tr>
   <td>Name </td>
   <td><input type="text" name="name" size="20" maxlength="30"></td>
</tr>
<tr>
   <td>Temperature </td>
   <td><input type="text" name="temp" size="20" maxlength="30"></td>
</tr>
<tr>
   <td>Moisture </td>
   <td><input type="text" name="humi" size="20" maxlength="30"></td>
</tr>
</table>
<input type="submit" name="action" value="Submit">
</form>
<hr>
--->
<?php
   include("conec.php");
   $link=Conection();
   $result=mysql_query("select * from enviro order by id desc",$link);
?>
<table border="1" cellspacing="1" cellpadding="1">
      <tr>
         <td>&nbsp;Name &nbsp;</td>
         <td>&nbsp;Temperature &nbsp;</td>
         <td>&nbsp;Humidity &nbsp;</td>
         <td>&nbsp;Date &nbsp;</td>
       </tr>
<?php

   while($row = mysql_fetch_array($result)) {
printf("<tr><td> &nbsp;%s </td><td>%s</td><td>%s</td><td> &nbsp;%s&nbsp; </td></tr>",$row["name"], $row["temp"], $row["humi"], $row["created"]);
   }
   mysql_free_result($result);
 ?>
</table>
</body>
</html>

