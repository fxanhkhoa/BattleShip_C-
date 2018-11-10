<?php
  session_start();
  
  $servername = "localhost";
  $username = "root";
  $password = "root";
  $dbname = "BATTLESHIP";
  
  // Create connection
  $conn = new mysqli($servername, $username, $password, $dbname);
  // Check connection
  if ($conn->connect_error) {
      die("Connection failed: " . $conn->connect_error);
  }
  if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $usr = test_input($_POST["username"]);
    $pass = test_input($_POST["pass"]);
    //echo $username."-".$pass;
    
    $sql = "SELECT * FROM USER WHERE USERNAME ='$usr'";
    $result = $conn->query($sql);
    
    if ($result->num_rows > 0) {
      $_SESSION["USERNAME"] = $usr;
      $message = "Log in successful";
      echo "<script type='text/javascript'>alert('$message');</script>";
      header("Location: http://128.199.151.176/room_select.php");
    }
    else{
      $message = "Wrong username or password";
      echo "<script type='text/javascript'>alert('$message');</script>";
      header('Location: ' . $_SERVER['HTTP_REFERER']);
    }
  }
  
  function test_input($data) {
  $data = trim($data);
  $data = stripslashes($data);
  $data = htmlspecialchars($data);
  return $data;
  }
?>