#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <SSD1306.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "screens.h"
#include "routes.h"
#include "parametros.h"
#include <ArduinoJson.h>
#include "operacao.h"
#include "definicoes.h"

const char* ssid="Casa 17";
const char* pwd="caio1711";



int pulsoLitro = 5880;
int contador;
float litros=0;
String values="{\"Volume Atual\":321}";
parametros par;
parametros valoresAtuais;

telas t1;
int numTela;


void IRAM_ATTR trataInterrupacao(){
  contador++;
} 

void taskServer(void *pvParameter);
void taskScreens(void *pvParameter);
void taskOperation(void *pvParameter);
void taskCalculations(void *pvParamater);


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
  t1.ip=WiFi.localIP().toString();
  Serial.println(WiFi.localIP());
  attachInterrupt(35,trataInterrupacao,RISING);  
  
  xTaskCreatePinnedToCore(taskScreens,"Screen Task",configMINIMAL_STACK_SIZE + 1024,NULL,2,NULL,0);
  xTaskCreatePinnedToCore(taskServer,"Server Task",configMINIMAL_STACK_SIZE + 1024,NULL,4,NULL,0);
  xTaskCreatePinnedToCore(taskOperation,"Operation Task",configMINIMAL_STACK_SIZE + 1024,NULL,10,NULL,1);
  xTaskCreatePinnedToCore(taskCalculations,"Calculations Task",configMINIMAL_STACK_SIZE + 1024,NULL,15,NULL,1);
}

void taskScreens(void *pvParameter){
  configura();
  while(true){
    vTaskDelay(pdMS_TO_TICKS(500));
    t1.volumeEntradaAtual = litros;
    t1.volumeEntradaProgramado = 2.12;
    selecionaTela(numTela, t1);  
  }  
}

void taskServer(void *pvParameter){
  routesConfigure();
  while(true){
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void taskOperation(void *pvParameter){
  while(true){
    vTaskDelay(pdMS_TO_TICKS(100));
    ciclico();
  }
}

void taskCalculations(void *pvParameter){
  while(true){
    vTaskDelay(pdMS_TO_TICKS(50));  
    litros = (float) contador / (float) pulsoLitro;
    valoresAtuais.volumeEntrada = litros;
    valoresAtuais.diferencaMaxima = valoresAtuais.volumeEntrada - valoresAtuais.volumeSaidaInicial;
  }
}

void loop() {
  
  
  
  
  
  
  if(Serial.read() > 0) contador=0;
  digitalWrite(2,HIGH);
  delay(1000);
  digitalWrite(2,LOW);
  delay(1000);
  Serial.println(numTela);
}