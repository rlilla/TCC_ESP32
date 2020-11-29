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

// Configuração do medidor de vazão 
int pulsoLitro = 5880;
// Variável para contagem dos pulsos
int contador;
// Variável para contagem em litros
float litros=0;

String values="{\"Volume Atual\":321}";
parametros par;
parametros valoresAtuais;

// Instância para a classe de operação
Operacao op;

telas t1;
int numTela;


void IRAM_ATTR trataInterrupacao(){
  contador++;
} 

void taskServer(void *pvParameter);
void taskScreens(void *pvParameter);
void taskOperation(void *pvParameter);
void taskCalculations(void *pvParamater);
void atualizaSaidas();

void setup() {
  pinMode(VALV_ENTRADA_OUTPUT,OUTPUT);
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
  int tela=0;
  while(true){
    vTaskDelay(pdMS_TO_TICKS(500));
    t1.volumeEntradaAtual = litros;
    t1.volumeEntradaProgramado = 2.12;
    if(op.stModo.modoAuto) tela=0;
    if(op.stModo.modoManual) tela=2;
    selecionaTela(tela, t1);  
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
    vTaskDelay(pdMS_TO_TICKS(1000));
    op.loopOperacao();
    op.stDadosOperacao.volumeEntradaAtual = litros;
    op.stDadosOperacao.volumeEntradaProg = 3;
    op.stOperacao.iniciaSemiEntrada = true;
    atualizaSaidas();
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
  delay(1000);
  Serial.println(numTela);
}

void atualizaSaidas(){
  digitalWrite(VALV_ENTRADA_OUTPUT,op.stStatusOperacao.valvulaEntrada);
  digitalWrite(VALV_SAIDA_OUTPUT,op.stStatusOperacao.valvulaSaida);
}