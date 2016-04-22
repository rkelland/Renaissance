<?php
header("content-type: application/json");

$con = mysql_connect("localhost","arduino","arduino");

if (!$con) {
  die('Could not connect: ' . mysql_error());
}

if ($_GET['type'] == 'humi') {
  $type = "humi";
  $type = mysql_real_escape_string($type);
} else {
  $type = "temp";
  $type = mysql_real_escape_string($type);
}

if ($_GET['name'] == 'DHT_Basement') {
  $name = "DHT_Basement";
  $name = mysql_real_escape_string($name);
} else {
  $name = "basement";
  $name = mysql_real_escape_string($name);
}

mysql_select_db("whitburn", $con);
$result = mysql_query("SELECT ".$type.", created FROM enviro WHERE name ='".$name."' ORDER BY id DESC LIMIT 300");
//echo $result;

$array=[];
while($row = mysql_fetch_array($result)) {
//	$entry["name"] = $row['name'];
	$entry[0] = strtotime($row['created'])*1000;
//	$entry[1] = floatval($row['temp']);
	$entry[1] = floatval($row[$type]);
//	$entry[1] = date("l, F j y H:i:s",strtotime($row['created']));
	array_push($array,$entry);
	}

mysql_close($con);

echo $_GET["callback"].'('. json_encode($array).')';
?>
