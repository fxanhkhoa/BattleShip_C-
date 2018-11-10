<?php 
  if (!isset($_SESSION["USERNAME"])){
    header("Location: http://128.199.151.176/login.html");
  }
  else{
    header("Location: http://128.199.151.176/game.php");
  }
?>