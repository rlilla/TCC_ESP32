#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <SSD1306.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "routes.h"

const char* ssid="Casa 17";
const char* pwd="caio1711";

int pulsoLitro = 5880;
int contador;
float litros=0;
String values="{\"Volume Atual\":321}";
AsyncWebServer server(80);

SSD1306 display(0x3c,21,22);

void IRAM_ATTR trataInterrupacao(){
  contador++;
} 

void setup() {
  pinMode(4,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(35, INPUT);
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,pwd);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println(WiFi.localIP());
  attachInterrupt(35,trataInterrupacao,RISING);  
  display.init();
  display.clear();
  display.drawString(0,0,"TESTE");
  display.display();
  server.on("/",HTTP_GET, handleMain);
  server.on("/sendPar",HTTP_POST,handleSendPar);
  server.on("/readPar",HTTP_GET,handleReadPar);
  server.on("/readActValues",HTTP_GET,handleReadActualValue);
  server.begin();
}

void loop() {
  litros = (float) contador / (float) pulsoLitro;
  values="{\"Volume Atual\":" + String(litros) + "}";
  if(Serial.read() > 0) contador=0;
  digitalWrite(2,HIGH);
  delay(1000);
  digitalWrite(2,LOW);
  delay(1000);
  Serial.println(litros);
}