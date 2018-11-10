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
  
  $room = test_input($_POST["select_room"]);
  $_SESSION["ROOM"] = $room;
  
  //echo "Connected successfully";
  $sql = "SELECT * FROM GAMESTATUS WHERE ROOM = '$room'";
  $result = $conn->query($sql);
  
  if ($result->num_rows == 0){
    
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('100', '0', '$room');";
    $conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('101', '0', '$room');";
    $conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('102', '0', '$room');";
    $conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('103', '0', '$room');";
    $conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('104', '0', '$room');";
    $conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('105', '0', '$room');";
    $conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('106', '0', '$room');";
    $conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('107', '0', '$room');";
    $conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('110', '0', '$room');";
    $conn->multi_query($sql);
    
    
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('111', '0', '$room');";
    $conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('112', '0', '$room');";
    $conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('113', '0', '$room');";
    $conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('114', '0', '$room');";
    $conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('115', '0', '$room');";
    $conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('116', '0', '$room');";
    $conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('117', '0', '$room');";
    $conn->multi_query($sql);
            
    
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('120', '0', '$room');";
    $conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('121', '0', '$room');";
    $conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('122', '0', '$room');";
    $conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('123', '0', '$room');";
    $conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('124', '0', '$room');";
    $conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('125', '0', '$room');";
    $conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('126', '0', '$room');";
    $conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('127', '0', '$room');";
    $conn->multi_query($sql);
            
     
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('130', '0', '$room');";
    $conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('131', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('132', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('133', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('134', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('135', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('136', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('137', '0', '$room');";$conn->multi_query($sql);
            


    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('140', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('141', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('142', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('143', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('144', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('145', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('146', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('147', '0', '$room');";$conn->multi_query($sql);
            

    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('150', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('151', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('152', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('153', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('154', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('155', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('156', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('157', '0', '$room');";$conn->multi_query($sql);

            
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('160', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('161', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('162', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('163', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('164', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('165', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('166', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('167', '0', '$room');";$conn->multi_query($sql);
            
       
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('170', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('171', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('172', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('173', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('174', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('175', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('176', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('177', '0', '$room');";$conn->multi_query($sql);
    

      
    // TEAM 2 
    $sql = "";         
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('200', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('201', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('202', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('203', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('204', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('205', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('206', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('207', '0', '$room');";$conn->multi_query($sql);
            
            
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('210', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('211', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('212', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('213', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('214', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('215', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('216', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('217', '0', '$room');";$conn->multi_query($sql);
            
       
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('220', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('221', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('222', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('223', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('224', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('225', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('226', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('227', '0', '$room');";$conn->multi_query($sql);
            
       
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('230', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('231', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('232', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('233', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('234', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('235', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('236', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('237', '0', '$room');";$conn->multi_query($sql);
            

    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('240', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('241', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('242', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('243', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('244', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('245', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('246', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('247', '0', '$room');";$conn->multi_query($sql);
            
       
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('250', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('251', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('252', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('253', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('254', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('255', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('256', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('257', '0', '$room');";$conn->multi_query($sql);
            
        
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('260', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('261', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('262', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('263', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('264', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('265', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('266', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('267', '0', '$room');";$conn->multi_query($sql);
            

    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('270', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('271', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('272', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('273', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('274', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('275', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('276', '0', '$room');";$conn->multi_query($sql);
    $sql = "INSERT INTO GAMESTATUS (id_square, SHIPVALUE, ROOM)
            VALUES ('277', '0', '$room');";$conn->multi_query($sql);
            
  }
  
  
  $message = "Room ".$room." selected";
  echo "<script type='text/javascript'>alert('$message');</script>";
  header("Location: http://128.199.151.176/game.php");
  
  function test_input($data) {
  $data = trim($data);
  $data = stripslashes($data);
  $data = htmlspecialchars($data);
  return $data;
  }
?>