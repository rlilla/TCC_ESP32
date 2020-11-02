#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <SSD1306.h>



void setup() {
  pinMode(4,OUTPUT);
  pinMode(2,OUTPUT);
  
}

void loop() {
  digitalWrite(2,HIGH);
  digitalWrite(4,HIGH);
  delay(1000);
  digitalWrite(2,LOW);
  digitalWrite(4,LOW);
  delay(1000);

}