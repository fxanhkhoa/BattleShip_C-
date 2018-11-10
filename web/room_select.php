<?php
  session_start();
  
  $servername = "localhost";
  $username = "root";
  $password = "root";
  $dbname = "BATTLESHIP";
  
  if (!isset($_SESSION["USERNAME"])){
    header("Location: http://128.199.151.176/login.html");
  }
  
  // Create connection
  $conn = new mysqli($servername, $username, $password, $dbname);
  // Check connection
  if ($conn->connect_error) {
      die("Connection failed: " . $conn->connect_error);
  }
  
  $sql = "SELECT (id) FROM ROOM WHERE STATUS = '0'";
  $result = $conn->query($sql);

  if ($result->num_rows > 0) {
    
  }
?>

<!DOCTYPE html>
<html lang="en">
<head>
  <title>Select Room</title>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
  <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
</head>

<body>
  <div class="container">
    <form action="user/room_process.php" method ="POST">
      <div class="form-group">
        <label for="select_room">Select list (select one):</label>
        <select class="form-control" id="select_room" name="select_room">
          <?php 
            while($row = $result->fetch_assoc()) {
          ?>
          <option value="<?php echo $row["id"]; ?>"><?php echo $row["id"] ?></option>
          <?php } ?>
        </select>
      </div>
      <input type="submit" class="btn btn-success" value="Select">
    </form>
  </div>
</body>
</html>