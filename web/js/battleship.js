$(document).ready(function(){
  $(".btn-2").click(function(){
    //alert("clicked");
      infinitive_loop()
    });
});

function infinitive_loop() {
    setInterval(function(){ 
      $.get("http://128.199.151.176/getdata_room.php", function(data){
        //alert("Data: " + data );
        get_game(data);
      });
    }, 1000);
}

function get_game(data) {
    var jsonData = JSON.parse(data);
    console.log(jsonData.length);
    console.log("\"#"+jsonData[0].id+ "\"");
    for (var i = 0; i < jsonData.length; i++) {
    	console.log(jsonData[i].SHIPVALUE);
    	//alert(counter.firstName);
      // Remove all class 1st
      $("#"+jsonData[i].id_square).removeClass("classX");
      $("#"+jsonData[i].id_square).removeClass("fire");
      $("#"+jsonData[i].id_square).removeClass("ship1");
      $("#"+jsonData[i].id_square).removeClass("ship2");
      $("#"+jsonData[i].id_square).removeClass("ship3");
      $("#"+jsonData[i].id_square).removeClass("selected");
      
      if (jsonData[i].SHIPVALUE == 0){
        
      }
      else if (jsonData[i].SHIPVALUE == 1){ // missed
        $("#"+jsonData[i].id_square).addClass("classX");
      }
      else if (jsonData[i].SHIPVALUE == 2){ // hitted
        $("#"+jsonData[i].id_square).addClass("fire");
      }
      else if (jsonData[i].SHIPVALUE == 3){ // ship3
        $("#"+jsonData[i].id_square).addClass("ship1");
      }
      else if (jsonData[i].SHIPVALUE == 4){ // ship4
        $("#"+jsonData[i].id_square).addClass("ship2");
      }
      else if (jsonData[i].SHIPVALUE == 5){ // ship5
        $("#"+jsonData[i].id_square).addClass("ship3");
      }
      else if (jsonData[i].SHIPVALUE == 6){ // selected
        $("#"+jsonData[i].id_square).addClass("selected");
      }
    }
}