/*
  ESP8266 Blink by Simon Peter
  Blink the blue LED on the ESP-01 module
  This example code is in the public domain

  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/
#include <ESP8266WiFi.h>


uint8_t GPIO16 = 16,
        GPIO5 = 5,
        GPIO4 = 4,
        GPIO0 = 0,
        GPIO2 = 2,
        GPIO14 = 14,
        GPIO12 = 12,
        GPIO13 = 13,
        GPIO15 = 15,
        GPIO9 = 9,
        GPIO10 = 10;

void setup() {
  pinMode(GPIO4, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(GPIO5, OUTPUT);
  pinMode(GPIO12, OUTPUT);
  pinMode(GPIO14, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(GPIO4, LOW);   // Turn the LED on (Note that LOW is the voltage level
  digitalWrite(GPIO5, LOW);
  digitalWrite(GPIO12, LOW);
  digitalWrite(GPIO14, LOW);
  digitalWrite(GPIO9, LOW);
  digitalWrite(GPIO10, LOW);
  // but actually the LED is on; this is because
  // it is active low on the ESP-01)
  delay(1000);                      // Wait for a second
  digitalWrite(GPIO4, HIGH);  // Turn the LED off by making the voltage HIGH
  digitalWrite(GPIO5, HIGH);
  digitalWrite(GPIO12, HIGH);
  digitalWrite(GPIO14, HIGH);
  digitalWrite(GPIO9, HIGH);
  digitalWrite(GPIO10, HIGH);
  delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
}
