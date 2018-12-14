<?php
  
  $servername = "localhost";
  $username = "battleship";
  $password = "12345678";
  $dbname = "BATTLESHIP";

  if ($_SERVER["REQUEST_METHOD"] == "POST") {
  $usr = test_input($_POST["Username"]);
  $pass = test_input($_POST["Password"]);
}
  
  // Create connection
  $conn = new mysqli($servername, $username, $password, $dbname);
  // Check connection
  if ($conn->connect_error) {
      die("Connection failed: " . $conn->connect_error);
  } 
  //echo "Connected successfully";
  $sql = "SELECT * FROM USER WHERE USERNAME ='$usr'";
  $result = $conn->query($sql);
  
  if ($result->num_rows > 0) {
    $message = "Username is existed";
    echo "<script type='text/javascript'>alert('$message');</script>";
  }
  else{
    $sql = "INSERT INTO USER (USERNAME, PASSWORD)
          VALUES ('$usr', '$pass');";
          
    if ($conn->query($sql) === TRUE) {
      $message = "Signup Successfully";
      echo "<script type='text/javascript'>alert('$message');</script>";
      header("Refresh: 3; url=../login.html");
    } else {
      echo "Error: " . $sql . "<br>" . $conn->error;
}
  }
  
  function test_input($data) {
  $data = trim($data);
  $data = stripslashes($data);
  $data = htmlspecialchars($data);
  return $data;
  }
?>

<html>

  <table>
    <?php while($row = $result->fetch_assoc()) {
        //echo $row["id"].$row["SHIPVALUE"]."*";
        #$rows[] = $row;
     ?>
     <tr>
      <td><?php //echo $row["id"] ?></td>
      <td><?php //echo $row["USERNAME"] ?></td>
      <td><?php //echo $row["PASSWORD"] ?></td>
     <tr>
    <?php } ?>
  </table>

</html>