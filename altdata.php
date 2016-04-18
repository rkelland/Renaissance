<?php
header("content-type: application/json");

$con = mysql_connect("localhost","arduino","arduino");

if (!$con) {
  die('Could not connect: ' . mysql_error());
}
if ($_GET['doorname'] == 'back') {
  $doorname = "back";
  $doorname = mysql_real_escape_string($doorname);
} else {
  $doorname = "front";
  $doorname = mysql_real_escape_string($doorname);
}

mysql_select_db("whitburn", $con);
$result = mysql_query("SELECT created, deltatime, doorname FROM zones WHERE doorname ='".$doorname."' AND event = 0");
//echo $result;
$array=[];
while($row = mysql_fetch_array($result)) {

//echo(strtotime($row['created']). ",". $row['deltatime']);
	$entry["x"] = intval(date("N",strtotime($row['created']))-1);
	$entry["y"] = intval(date(G,strtotime($row['created'])))*60*60*1000 + intval(date(i,strtotime($row['created'])))*60*1000-$row['deltatime']/2;
        $entry["z"] = $row['deltatime']/1000;
	$entry["door"] = $row['doorname'];
	$entry["fulldate"] = strtotime($row['created']);
	array_push($array,$entry);
	}

mysql_close($con);

echo $_GET["callback"].'('. json_encode($array).')';
?>
