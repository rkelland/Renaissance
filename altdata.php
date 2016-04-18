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
$result = mysql_query("SELECT created, deltatime FROM zones WHERE doorname ='".$doorname."' AND event = 0");
//echo $result;
$array=[];
while($row = mysql_fetch_array($result)) {

//echo(strtotime($row['created']). ",". $row['deltatime']);
	$entry[0] = intval(date("N",strtotime($row['created']))-1);
	$entry[1] = intval(date(G,strtotime($row['created'])))*60*60*1000 + intval(date(i,strtotime($row['created'])))*60*1000+$row['deltatime']/2;
        $entry[2] = $row['deltatime']/1000;
	array_push($array,$entry);
	}

mysql_close($con);

echo $_GET["callback"].'('. json_encode($array).')';
?>
