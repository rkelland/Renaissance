<?php
   include("conec.php");
   $link=Conection();
   $Sql="insert into enviro (name,temp,humi)  values ('".$_GET["name"]."','".$_GET["temp"]."','".$_GET["humi"]."')";
   mysql_query($Sql,$link);
   header("Location: index.php");
?>
