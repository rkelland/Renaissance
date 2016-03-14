<?php
function Conection(){
   if (!($link=mysql_connect("localhost","arduino","arduino")))  {
      exit();
   }
   if (!mysql_select_db("whitburn",$link)){
      exit();
   }
   return $link;
}
?>
