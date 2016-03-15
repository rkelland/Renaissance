<html>
<head>
   <title>Data of Sensor</title>
</head>
<body>
<h1>Data from the Sensor</h1>


<?php
   include("conec.php");
   $link=Conection();
   $temperatures=mysql_query("select * from enviro order by id desc",$link);
   $ftevents=mysql_query("select * from zones where doorname='front'; order by id desc",$link);
?>

<h2>Temp and Humidity</h2>
<table border="1" cellspacing="1" cellpadding="1">
      <tr>
         <td>&nbsp;Name &nbsp;</td>
         <td>&nbsp;Temperature &nbsp;</td>
         <td>&nbsp;Humidity &nbsp;</td>
         <td>&nbsp;Date &nbsp;</td>
       </tr>
<?php
   while($row = mysql_fetch_array($temperatures)) {
printf("<tr><td> &nbsp;%s </td><td>%s</td><td>%s</td><td> &nbsp;%s&nbsp; </td></tr>",$row["name"], $row["temp"], $row["humi"], $row["created"]);
   }
   mysql_free_result($result);
 ?>
</table>


<h2>Front Door</h2>
<table border="1" cellspacing="1" cellpadding="1">
      <tr>
         <td>&nbsp;Door &nbsp;</td>
         <td>&nbsp;Event &nbsp;</td>
         <td>&nbsp;Time &nbsp;</td>
         <td>&nbsp;Date &nbsp;</td>
       </tr>
<?php
   while($row = mysql_fetch_array($ftevents)) {
printf("<tr><td> &nbsp;%s </td><td>%s</td><td>%s</td><td> &nbsp;%s&nbsp; </td></tr>",$row["doorname"], $row["event"], $row["deltatime"], $row["created"]);
   }
   mysql_free_result($result);
 ?>
</table>
</body>
</html>

