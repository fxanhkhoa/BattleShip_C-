<html>
<?php 
  session_start();
  if (!isset($_SESSION["USERNAME"])){
    header("Location: login.html");
  }
  if (!isset($_SESSION["ROOM"])){
    header("Location: room_select.php");
  }
  if (!isset($_SESSION["PSHAND"])){
    header("Location: ps_hand_select.php");
  }
?>
<head>
  <link rel="stylesheet" href="css/battleship.css">
  <link href="https://maxcdn.bootstrapcdn.com/font-awesome/4.1.0/css/font-awesome.min.css" rel="stylesheet">
  <script src="https://cdnjs.cloudflare.com/ajax/libs/socket.io/2.1.1/socket.io.dev.js"></script>
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
  <script src="js/battleship.js"></script>
</head>
<body>
<div align="center">
  <div> step </div>
  <h2 id="step"></h2>
  <div> room </div>
  <h2 id="room"><?php echo $_SESSION["ROOM"];?></h2>
  <div> ps hand </div>
  <h2 id="pshand"><?php echo $_SESSION["PSHAND"];  ?></h2>
  <div>
    <div> turn </div>
    <h2 id="turn"></h2>
    <div> time </div>
    <h2 id="time"></h2>
  </div>
    <table class="container">
      <thead>
          <tr>
              <th><h1>1</h1></th>
              <th><h1>2</h1></th>
              <th><h1>3</h1></th>
              <th><h1>4</h1></th>
              <th><h1>5</h1></th>
              <th><h1>6</h1></th>
              <th><h1>7</h1></th>
              <th><h1>8</h1></th>
          </tr>
      </thead>
      <tbody>
          <tr>
              <td id="100"></td>
              <td id="101"></td>
              <td id="102"></td>
              <td id="103"></td>
              <td id="104"></td>
              <td id="105"></td>
              <td id="106"></td>
              <td id="107"></td>
          </tr>
          <tr>
              <td id="110"></td>
              <td id="111"></td>
              <td id="112"></td>
              <td id="113"></td>
              <td id="114"></td>
              <td id="115"></td>
              <td id="116"></td>
              <td id="117"></td>
          <tr>
              <td id="120"></td>
              <td id="121"></td>
              <td id="122"></td>
              <td id="123"></td>
              <td id="124"></td>
              <td id="125"></td>
              <td id="126"></td>
              <td id="127"></td>
          </tr>
          <tr>
              <td id="130"></td>
              <td id="131"></td>
              <td id="132"></td>
              <td id="133"></td>
              <td id="134"></td>
              <td id="135"></td>
              <td id="136"></td>
              <td id="137"></td>
          </tr>
          <tr>
              <td id="140"></td>
              <td id="141"></td>
              <td id="142"></td>
              <td id="143"></td>
              <td id="144"></td>
              <td id="145"></td>
              <td id="146"></td>
              <td id="147"></td>
          </tr>
          <tr>
              <td id="150"></td>
              <td id="151"></td>
              <td id="152"></td>
              <td id="153"></td>
              <td id="154"></td>
              <td id="155"></td>
              <td id="156"></td>
              <td id="157"></td>
          </tr>
          <tr>
              <td id="160"></td>
              <td id="161"></td>
              <td id="162"></td>
              <td id="163"></td>
              <td id="164"></td>
              <td id="165"></td>
              <td id="166"></td>
              <td id="167"></td>
          </tr>
          <tr>
              <td id="170"></td>
              <td id="171"></td>
              <td id="172"></td>
              <td id="173"></td>
              <td id="174"></td>
              <td id="175"></td>
              <td id="176"></td>
              <td id="177"></td>
          </tr>
      </tbody>
  </table>
  
  <table class="container">
      <thead>
          <tr>
              <th><h1>1</h1></th>
              <th><h1>2</h1></th>
              <th><h1>3</h1></th>
              <th><h1>4</h1></th>
              <th><h1>5</h1></th>
              <th><h1>6</h1></th>
              <th><h1>7</h1></th>
              <th><h1>8</h1></th>
          </tr>
      </thead>
      <tbody>
          <tr>
              <td id="200"></td>
              <td id="201"></td>
              <td id="202"></td>
              <td id="203"></td>
              <td id="204"></td>
              <td id="205"></td>
              <td id="206"></td>
              <td id="207"></td>
          </tr>
          <tr>
              <td id="210"></td>
              <td id="211"></td>
              <td id="212"></td>
              <td id="213"></td>
              <td id="214"></td>
              <td id="215"></td>
              <td id="216"></td>
              <td id="217"></td>
          <tr>
              <td id="220"></td>
              <td id="221"></td>
              <td id="222"></td>
              <td id="223"></td>
              <td id="224"></td>
              <td id="225"></td>
              <td id="226"></td>
              <td id="227"></td>
          </tr>
          <tr>
              <td id="230"></td>
              <td id="231"></td>
              <td id="232"></td>
              <td id="233"></td>
              <td id="234"></td>
              <td id="235"></td>
              <td id="236"></td>
              <td id="237"></td>
          </tr>
          <tr>
              <td id="240"></td>
              <td id="241"></td>
              <td id="242"></td>
              <td id="243"></td>
              <td id="244"></td>
              <td id="245"></td>
              <td id="246"></td>
              <td id="247"></td>
          </tr>
          <tr>
              <td id="250"></td>
              <td id="251"></td>
              <td id="252"></td>
              <td id="253"></td>
              <td id="254"></td>
              <td id="255"></td>
              <td id="256"></td>
              <td id="257"></td>
          </tr>
          <tr>
              <td id="260"></td>
              <td id="261"></td>
              <td id="262"></td>
              <td id="263"></td>
              <td id="264"></td>
              <td id="265"></td>
              <td id="266"></td>
              <td id="267"></td>
          </tr>
          <tr>
              <td id="270"></td>
              <td id="271"></td>
              <td id="272"></td>
              <td id="273"></td>
              <td id="274"></td>
              <td id="275"></td>
              <td id="276"></td>
              <td id="277"></td>
          </tr>
      </tbody>
  </table>
  </div>
  <div class="type-3" style="clear:both;padding-left:28%;">
    <div>
      <a href="#" onclick="" class="btn btn-2" id="button_get">
        <span class="txt">Click to Start</span>
        <span class="round"><i class="fa fa-chevron-right"></i></span>
      </a>
    </div>
  </div>
</body>

</html>