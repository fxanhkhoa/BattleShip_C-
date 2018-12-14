$(document).ready(function(){
  $(".btn-2").click(function(){
    //alert("clicked");
      infinitive_loop()
    });
});

function infinitive_loop() {
    setInterval(function(){
      if (1){
        $.get("getdata.php", function(data){
          //alert("Data: " + data );
          get_game(data);
        });
      }
      /* else{
        if ($("#pshand").text() == 1){
          $.get("getdata_room_id1.php", function(data){
            //alert("Data: " + data );
            get_game(data);
          });
        }
        else{
          $.get("getdata_room_id2.php", function(data){
            //alert("Data: " + data );
            get_game(data);
          });
        }
      } */
      //var str = $("room").text();
      //str = str.replace(/ +/g, "");
      //alert("get_turn.php?room=" + str);
      $.get("get_turn.php?room=" + $("#room").text(), function(turn){
        $("#turn").text(turn);
        if (String(turn) == $("#pshand").text()){
        //if ($("#pshand").text().localeCompare(turn) == 0){
          $("#turn").text("your turn!");
        }
        else if (turn == 3){
          $("#turn").text("Team 1 WIN");
        }
        else if (turn == 4){
          $("#turn").text("Team 2 WIN");
        }
        else  $("#turn").text("please wait!");                
      });
      
      $.get("get_step.php?room=" + $("#room").text(), function(step){
        $("#step").text(step);
                   
      });
      
      $.get("get_time.php?room=" + $("#room").text(), function(time){
        //alert(time);
        $("#time").text(time);             
      });
    }, 300);
}

function get_game(data) {
    var jsonData = JSON.parse(data);
    console.log(jsonData.length);
    console.log("\"#"+jsonData[0].id+ "\"");
    for (var i = 0; i < jsonData.length; i++) {
    	console.log(jsonData[i].SHIPVALUE);
    	//alert(counter.firstName);
      // Remove all class 1st
      //$("#"+jsonData[i].id_square).removeClass("classX");
      //$("#"+jsonData[i].id_square).removeClass("fire");
      //$("#"+jsonData[i].id_square).removeClass("ship1");
      //$("#"+jsonData[i].id_square).removeClass("ship2");
      //$("#"+jsonData[i].id_square).removeClass("ship3");
      $("#"+jsonData[i].id_square).removeClass("selected");
      //$("td").removeClass("selected");
      
      if (jsonData[i].SHIPVALUE == 0){
        $("#"+jsonData[i].id_square).removeClass("classX");
        $("#"+jsonData[i].id_square).removeClass("fire");
        $("#"+jsonData[i].id_square).removeClass("ship1");
        $("#"+jsonData[i].id_square).removeClass("ship2");
        $("#"+jsonData[i].id_square).removeClass("ship3");
        $("#"+jsonData[i].id_square).removeClass("selected");
      }
      else if (jsonData[i].SHIPVALUE == 1){ // missed
        $("#"+jsonData[i].id_square).removeClass("ship1");
        $("#"+jsonData[i].id_square).removeClass("ship2");
        $("#"+jsonData[i].id_square).removeClass("ship3");
        $("#"+jsonData[i].id_square).addClass("classX");
      }
      else if (jsonData[i].SHIPVALUE == 2){ // hitted
        $("#"+jsonData[i].id_square).removeClass("ship1");
        $("#"+jsonData[i].id_square).removeClass("ship2");
        $("#"+jsonData[i].id_square).removeClass("ship3");
        $("#"+jsonData[i].id_square).addClass("fire");
      }
      else if ((jsonData[i].SHIPVALUE == 3)){ // ship3
        if ($("#pshand").text() == 1){ // team 1
          //alert("in here");
          if (jsonData[i].id_square < 200){
            $("#"+jsonData[i].id_square).addClass("ship1");
          }
        }
        else if ($("#pshand").text() == 2){
          if (jsonData[i].id_square > 199){
            $("#"+jsonData[i].id_square).addClass("ship1");
          }
        }
      }        
      else if ((jsonData[i].SHIPVALUE == 4)){ // ship4
        if ($("#pshand").text() == 1){ // team 1
          //alert("in here");
          if (jsonData[i].id_square < 200){
            $("#"+jsonData[i].id_square).addClass("ship2");
          }
        }
        else if ($("#pshand").text() == 2){
          if (jsonData[i].id_square > 199){
            $("#"+jsonData[i].id_square).addClass("ship2");
          }
        }
      }
      else if ((jsonData[i].SHIPVALUE == 5)){ // ship5
        if ($("#pshand").text() == 1){ // team 1
          //alert("in here");
          if (jsonData[i].id_square < 200){
            $("#"+jsonData[i].id_square).addClass("ship3");
          }
        }
        else if ($("#pshand").text() == 2){
          if (jsonData[i].id_square > 199){
            $("#"+jsonData[i].id_square).addClass("ship3");
          }
        }
      }
      else if (jsonData[i].SHIPVALUE >= 6){ // selected
        if ($("#step").text() != 3){
          if ($("#pshand").text() == 1){ // team 1
            //alert("in here");
            if (jsonData[i].id_square < 200){
              $("#"+jsonData[i].id_square).addClass("selected");
            }
          }
          else if ($("#pshand").text() == 2){
            if (jsonData[i].id_square > 199){
              $("#"+jsonData[i].id_square).addClass("selected");
            }
          }
        }
        else if ($("#step").text() == 3){
          $("#"+jsonData[i].id_square).addClass("selected");
        }
      }
    }
}