#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <SSD1306.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include "screens.h"
#include "routes.h"
#include "parametros.h"
#include <ArduinoJson.h>
#include "operacao.h"
#include "definicoes.h"

// Usuario e senha da conexao WIFI
const char* ssid="Casa 17";
const char* pwd="caio1711";

// Configuração do medidor de vazão 
int pulsoLitro = 5880;

// Variável para contagem dos pulsos
int contadorEntrada = 0;
int contadorSaida = 0;

// Variável para contagem em litros
float litros=0;
float litrosEntrada = 0;
float litrosSaida = 0;

String values="{\"Volume Atual\":321}";
parametros par;
parametros valoresAtuais;

// Instância para a classe de operação
Operacao op;

telas t1;
int numTela;

// Rotina de interrupcao do sensor de vazao de entrada
void IRAM_ATTR trataIntEntrada(){
  contadorEntrada++;
}

// Rotina de interrupcao do sensor de vazao de saida
void IRAM_ATTR trataIntSaida(){
  contadorSaida++;
}

// Prototipo das funcoes RTOS
void taskServer(void *pvParameter);
void taskScreens(void *pvParameter);
void taskOperation(void *pvParameter);
void taskCalculations(void *pvParamater);
void taskSimulaSensores(void *pvParameters);
void atualizaSaidas();

void setup() {
  // Condigura pinos dos sensores e dos reles
  pinMode(VALV_ENTRADA_OUTPUT,OUTPUT);
  //pinMode(VALV_SAIDA_OUTPUT,OUTPUT);
  pinMode(SENSOR_ENTRADA_INPUT,INPUT);
  pinMode(SENSOR_SAIDA_INPUT,INPUT);
  Serial.begin(9600);
  // Inicia WIFI em modo STATION
  WiFi.mode(WIFI_STA);
  // Conecta ao roteador
  WiFi.begin(ssid,pwd);
  // Aguarda conexao WIFI
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  t1.ip=WiFi.localIP().toString();
  Serial.println(WiFi.localIP());
  // Habilita interrupcoes de borda de subida para os sensores de vazao
  attachInterrupt(SENSOR_ENTRADA_INPUT,trataIntEntrada,RISING);  
  attachInterrupt(SENSOR_SAIDA_INPUT,trataIntSaida,RISING);  
  // Cria tarefas RTOS
  xTaskCreatePinnedToCore(taskScreens,"Screen Task",configMINIMAL_STACK_SIZE + 1024,NULL,2,NULL,1);
  xTaskCreatePinnedToCore(taskServer,"Server Task",configMINIMAL_STACK_SIZE + 2048,NULL,15,NULL,0);
  xTaskCreatePinnedToCore(taskOperation,"Operation Task",configMINIMAL_STACK_SIZE + 1024,NULL,10,NULL,1);
  xTaskCreatePinnedToCore(taskCalculations,"Calculations Task",configMINIMAL_STACK_SIZE + 1024,NULL,15,NULL,1);
  xTaskCreatePinnedToCore(taskSimulaSensores,"Simulation Task",configMINIMAL_STACK_SIZE + 1024,NULL,1,NULL,1);
}

void taskScreens(void *pvParameter){
  configura();
  int tela=0;
  while(true){
    vTaskDelay(pdMS_TO_TICKS(500));
    t1.volumeEntradaAtual = litrosEntrada;
    t1.volumeSaidaAtual = litrosSaida;
    tela = 1;
    if(op.stModo.modoAuto) tela=0;
    if(op.stModo.modoSemi) tela=3;
    if(op.stModo.modoManual) tela=2;
    selecionaTela(tela, t1);  
  }  
}

void taskServer(void *pvParameter){
  MDNS.begin("smartlub");
  routesConfigure();
  MDNS.addService("http","tcp",80);
  while(true){
    vTaskDelay(pdMS_TO_TICKS(300));
  }
}

void taskOperation(void *pvParameter){
  while(true){
    op.stDadosOperacao.volumeEntradaAtual = litrosEntrada;
    op.stDadosOperacao.volumeSaidaAtual = litrosSaida;
    op.stDadosOperacao.volumeEntradaProg = Param::par.volumeEntrada;
    op.stDadosOperacao.volumeSaidaProg = Param::par.volumeSaidaInicial;
    op.stDadosOperacao.diferencaMaxProg = Param::par.diferencaMaxima;
    op.stDadosOperacao.diferencaMaxAtual = litrosEntrada - litrosSaida;
    op.loopOperacao();
    atualizaSaidas();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void taskCalculations(void *pvParameter){
  while(true){
    vTaskDelay(pdMS_TO_TICKS(50));  
    litrosEntrada = (float) contadorEntrada / (float) pulsoLitro;
    litrosSaida = (float) contadorSaida / (float) pulsoLitro;
  }
}

void loop() {
  if(Serial.read() > 0) contadorEntrada=0;
  delay(1000);
  Serial.print(op.stOperacao.simulaSensores);
  Serial.print("-");
  Serial.print(op.stStatusOperacao.valvulaSaida);
  Serial.print("-");
  Serial.println(op.stDadosOperacao.estadoAtual);
}

void taskSimulaSensores(void *pvParameters){
  contadorEntrada = 0;
  contadorSaida = 0;
  while(true){
    if (op.stOperacao.simulaSensores && op.stStatusOperacao.valvulaEntrada) {
      contadorEntrada += 10;
    }
    if (op.stOperacao.simulaSensores && op.stStatusOperacao.valvulaSaida) {
      contadorSaida += 10;
    }  
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void atualizaSaidas(){
  digitalWrite(VALV_ENTRADA_OUTPUT,op.stStatusOperacao.valvulaEntrada);
  digitalWrite(VALV_SAIDA_OUTPUT,op.stStatusOperacao.valvulaSaida);
}