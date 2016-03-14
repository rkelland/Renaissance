<?php
   include("conec.php");
   $link=Conection();

	if(isset($_GET['db'])) {
		if ($_GET['db'] == 'enviro'){$Sql="insert into enviro (name,temp,humi)  values ('".$_GET["name"]."','".$_GET["temp"]."','".$_GET["humi"]."')";}
		if ($_GET['db'] == 'zones'){$Sql="insert into zones (doorname,event,deltatime)  values ('".$_GET["doorname"]."','".$_GET["event"]."','".$_GET["deltatime"]."')";}
	}
	else{
		  $Sql="insert into enviro (name,temp,humi)  values ('".$_GET["name"]."','".$_GET["temp"]."','".$_GET["humi"]."')";
	}
   mysql_query($Sql,$link);
   header("Location: index.php");
?>
