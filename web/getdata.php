<?php
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
//echo "Connected successfully";
$sql = "SELECT * FROM GAMESTATUS";
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
?>