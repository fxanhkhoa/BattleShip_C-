<?php 
  if (!isset($_SESSION["USERNAME"])){
    header("Location: login.html");
  }
  else{
    header("Location: room_select.php");
  }
?>