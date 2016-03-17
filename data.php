<?php
$con = mysql_connect("localhost","arduino","arduino");

if (!$con) {
  die('Could not connect: ' . mysql_error());
}

mysql_select_db("whitburn", $con);

$result = mysql_query("SELECT created, temp FROM enviro ORDER BY id DESC LIMIT 200");

while($row = mysql_fetch_array($result)) {

printf(date("l, F j y H:i:s",strtotime($row['created'])). "\t". $row['temp']. "\n");
}

mysql_close($con);
?>
