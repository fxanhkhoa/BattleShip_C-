<?php
  session_start();

  $room = $_SESSION["ROOM"];
  
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
  //echo "Connected successfully";
  $sql = "SELECT * FROM GAMESTATUS WHERE ROOM ='$room' AND id_square < 300 AND id_square > 199";
  $result = $conn->query($sql);
  
  if ($result->num_rows > 0) {
    // output data of each row
    $rows = array();
    while($row = $result->fetch_assoc()) {
        //echo $row["id"].$row["SHIPVALUE"]."*";
      $rows[] = $row;
    }
  print json_encode($rows);
  } else {
    echo "0 results";
  }
  $conn->close();

  function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
  }
?>