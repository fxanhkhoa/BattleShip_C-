#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

SoftwareSerial ESPserial(2, 3); // RX | TX

const char* ssid = "UIT Public";
const char* password = "";

const uint16_t port = 8000;
char host[] = "14.186.70.185";

uint8_t buffer[1024] = "";
uint8_t uart_buffer[1024] = "";
uint8_t length, cl_length;
WiFiClient client;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(13, OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(19200);

  // Start the software serial for communication with the ESP8266
  ESPserial.begin(9600);  
  Serial.println(WiFi.macAddress());
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
 
    delay(1000);
    Serial.print("Connecting..");
 
  }
  
  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());


  if (!client.connect(host, port)) {
        Serial.println(F("Ket noi den socket server that bai!"));
        return;
    }
 
    //Khi đã kết nối thành công
    if (client.connected()) {
        Serial.print("Host connected ");
        //Thì gửi sự kiện ("connection") đến Socket server ahihi.
        //client.send("connection", "message", "Connected !!!!");
    }

    length = 0;
    cl_length = 0;
}

void loop()
{

    //Serial.println("Connected to server successful!");

    //client.print("Hello from ESP32!");

    //Serial.println("Disconnecting...");
    //client.stop();
    //buf[0] = 1;
    //buf[1] = 2;
    //buf[3] = 2;
    //buf[4] = 2;
    //buf[5] = 1;
    //client.write((const uint8_t *)buf, 20);
    if (client.available()) 
    {
      uart_buffer[cl_length] = client.read();
      cl_length ++;
      if (cl_length >= 1024)
      {
        uart_buffer[9] = 42; // dau *
        Serial.write(uart_buffer, 10);
      }
    }
    
    if ( Serial.available() )   
    {  
      digitalWrite(LED_BUILTIN, HIGH);
      //Serial.println(Serial.read());
      buffer[length] = Serial.read();
      //Serial.write(buffer[length]);
      length++;
      if (buffer[length - 1] == 42) // dau *
      {
        //Serial.write(buffer, length - 1);
        client.write((const uint8_t *)buffer, 1024);
        length = 0;
      }
      //Serial.write(buffer[0] + 70);
      //Serial.write(length);
      //Serial.write( buffer, length); 
      //client.write((const uint8_t *)buffer, length);
    }
    //delay(10000);
}
