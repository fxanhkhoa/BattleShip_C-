<?php
  session_start();
  
  $servername = "localhost";
  $username = "battleship";
  $password = "qwerty12345678";
  $dbname = "BATTLESHIP";

  // Create connection
  $conn = new mysqli($servername, $username, $password, $dbname);
  // Check connection
  if ($conn->connect_error) {
      die("Connection failed: " . $conn->connect_error);
  } 
  
  $pshand = test_input($_POST["select_pshand"]);
  $_SESSION["PSHAND"] = $pshand;
  
  //echo "Connected successfully";
  $sql = "UPDATE PSHAND SET STATUS = '1' WHERE id = '$pshand'";
  $result = $conn->query($sql);
  
  $message = "Room ".$room." selected";
  echo "<script type='text/javascript'>alert('$message');</script>";
  header("Location: ../game.php");
  
  function test_input($data) {
  $data = trim($data);
  $data = stripslashes($data);
  $data = htmlspecialchars($data);
  return $data;
  }
?>