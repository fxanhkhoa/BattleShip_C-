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
  $sql = "SELECT * FROM USER";
  $result = $conn->query($sql);
  
?>

<html>

  <table>
    <?php while($row = $result->fetch_assoc()) {
        //echo $row["id"].$row["SHIPVALUE"]."*";
        #$rows[] = $row;
     ?>
     <tr>
      <td><?php echo $row["id"] ?></td>
      <td><?php echo $row["USERNAME"] ?></td>
      <td><?php echo $row["PASSWORD"] ?></td>
     <tr>
    <?php } ?>
  </table>

</html>