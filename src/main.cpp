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
#include "PubSubClient.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "banco.h"

// Instancia para conexao com o sensor de temperatura DS18B20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Configuracao para a conexao MQTT
WiFiClient espClient;
PubSubClient mqtt(espClient);
const char* broker="broker.mqtt-dashboard.com";

// Objeto JSON para enviar o valor atual
StaticJsonDocument<200> valAtual;
char buffer[1024];

// Usuario e senha da conexao WIFI
const char* ssid="Casa 17";
const char* pwd="caio1711";

// Configuração do medidor de vazão 
int pulsoLitro = 5880;

// Variável para contagem dos pulsos
int contadorEntrada = 0;
int contadorSaida = 0;

// Variável para contagem em litros
float litrosEntrada = 0;
float litrosSaida = 0;

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
void taskSimulaSensores(void *pvParameter);
void taskTemperatura(void *pvParameter);
void taskFirebase(void *pvParameter);

// Funcao para atualizar as saidas ciclicamente de acordo com o status atual da classe Operacao
void atualizaSaidas();

// Funcao para reconectar ao broker MQTT caso a conexao caia
void reconectaBroker();

// Funcao callback chamada ao receber mensagem nos topicos subscritos
void callback(char* topic, byte* message, unsigned int length){
  Serial.println(topic);
  String msgTemp;
  if(strcmp(topic,"smartlub/mqtt/comandos") == 0){
    for (int i=0; i < length; i++){
      msgTemp += (char) message[i];
    }
    DynamicJsonDocument recv(1024);
    deserializeJson(recv,msgTemp.c_str());
    op.stModo.modoAuto = (bool) recv["auto"];
    op.stModo.modoManual = (bool) recv["manual"];
    op.stModo.modoSemi = (bool) recv["semi"];
    op.stModo.semModo = (bool) recv["sem"];
    op.stOperacao.iniciaAuto = (bool) recv["iniAuto"];
    op.stOperacao.interrompeTroca = (bool) recv["interrompe"];
    op.stOperacao.iniciaSemiEntrada = (bool) recv["iniSemiEnt"];
    op.stOperacao.iniciaSemiSaida = (bool) recv["iniSemiSaida"];
    op.stOperacao.ligaValvulaEntrada = (bool) recv["ligaValvEnt"];
    op.stOperacao.ligaValvulaSaida = (bool) recv["ligaValvSaida"];
    op.stOperacao.simulaSensores = (bool) recv["simula"]; 
  }
  if(strcmp(topic,"smartlub/mqtt/parametros") == 0){
    for (int i=0; i < length; i++){
      msgTemp += (char) message[i];
    }
    DynamicJsonDocument doc(1024);
    deserializeJson(doc,msgTemp.c_str());
    Param::par.volumeEntrada = doc["volEntrada"];
    Param::par.volumeSaidaInicial = doc["volSaida"];
    Param::par.diferencaMaxima = doc["diferenca"];
    Param::par.temperatura = doc["temp"];
    Param::salvarParametros();
  }
}

void setup() {
  // Condigura pinos dos sensores e dos reles
  pinMode(VALV_ENTRADA_OUTPUT,OUTPUT);
  pinMode(VALV_SAIDA_OUTPUT,OUTPUT);
  pinMode(RELE_OUTPUT,OUTPUT);
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

  // Ler IP atual da conexão WIFI e envia para a serial para debug
  t1.ip=WiFi.localIP().toString();
  Serial.println(WiFi.localIP());
  
  // Habilita interrupcoes de borda de subida para os sensores de vazao
  attachInterrupt(SENSOR_ENTRADA_INPUT,trataIntEntrada,RISING);  
  attachInterrupt(SENSOR_SAIDA_INPUT,trataIntSaida,RISING);  
  
  // Cria tarefas RTOS
  xTaskCreatePinnedToCore(taskScreens,"Screen Task",configMINIMAL_STACK_SIZE + 1024,NULL,2,NULL,1);
  xTaskCreatePinnedToCore(taskServer,"Server Task",configMINIMAL_STACK_SIZE + 4096,NULL,15,NULL,0);
  xTaskCreatePinnedToCore(taskOperation,"Operation Task",configMINIMAL_STACK_SIZE + 4096,NULL,10,NULL,1);
  xTaskCreatePinnedToCore(taskCalculations,"Calculations Task",configMINIMAL_STACK_SIZE + 1024,NULL,15,NULL,1);
  xTaskCreatePinnedToCore(taskSimulaSensores,"Simulation Task",configMINIMAL_STACK_SIZE + 1024,NULL,1,NULL,1);
  xTaskCreatePinnedToCore(taskTemperatura,"Temperature Task",configMINIMAL_STACK_SIZE + 1024,NULL,5,NULL,1);
  xTaskCreatePinnedToCore(taskFirebase,"Firebase Task",configMINIMAL_STACK_SIZE + 4096,NULL,3,NULL,1);

    // Configura Servidor e Callback MQTT
  mqtt.setServer(broker,1883);
  mqtt.setCallback(callback);

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
  //MDNS.begin("smartlub");
  routesConfigure();
  //MDNS.addService("http","tcp",80);
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
    op.stDadosOperacao.temperaturaProg = Param::par.temperatura;
    op.stDadosOperacao.diferencaMaxAtual = litrosEntrada - litrosSaida;
    op.loopOperacao();
    atualizaSaidas();
    vTaskDelay(pdMS_TO_TICKS(500));
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
  if(!mqtt.connected()){
    reconectaBroker();
  }
  valAtual["volEntrada"] = op.stDadosOperacao.volumeEntradaAtual;
  valAtual["volSaida"] = op.stDadosOperacao.volumeSaidaAtual;
  valAtual["temp"] = op.stDadosOperacao.temperaturaAtual;
  valAtual["diferenca"] = op.stDadosOperacao.diferencaMaxAtual;
  valAtual["statusValvEntrada"] = op.stStatusOperacao.valvulaEntrada;
  valAtual["statusValvSaida"] = op.stStatusOperacao.valvulaSaida;
  valAtual["modo"] = "SEM MODO";
  if (Operacao::stModo.modoAuto) valAtual["modo"] = "AUTOMATICO";
  if (Operacao::stModo.modoManual) valAtual["modo"] = "MANUAL";
  if (Operacao::stModo.modoSemi) valAtual["modo"] = "SEMI AUTOMATICO";
  if (Operacao::stModo.semModo) valAtual["modo"] = "SEM MODO";
  valAtual["parVolEntrada"] = Param::par.volumeEntrada;
  valAtual["parVolSaida"] = Param::par.volumeSaidaInicial;
  valAtual["parDiferenca"] = Param::par.diferencaMaxima;
  valAtual["parTemp"] = Param::par.temperatura;
  serializeJson(valAtual,buffer);
  mqtt.publish_P("smartlub/mqtt/atual",buffer,0);
  mqtt.loop();
  delay(2000);
}

void taskSimulaSensores(void *pvParameter){
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

// Funcao para medicao de temperatura
void taskTemperatura(void *pvParameter){
  sensors.begin();
  while(true){
    sensors.requestTemperatures();
    op.stDadosOperacao.temperaturaAtual = sensors.getTempCByIndex(0);
    vTaskDelay(pdMS_TO_TICKS(3000));
  }
}

// Funcao para receber e enviar dados para o Firebase 
void taskFirebase(void *pvParameter){
  float oldLitrosEntrada = 0;
  float oldLitrosSaida = 0;
  float litrosEntradaAcum = lerLitroEntrada();
  float litrosSaidaAcum = lerLitroSaida();
  while(true){
    if((op.stDadosOperacao.volumeEntradaAtual != oldLitrosEntrada) || (op.stDadosOperacao.volumeSaidaAtual != oldLitrosSaida)){
      escreveLitroSaida(op.stDadosOperacao.volumeSaidaAtual + litrosSaidaAcum);
      escreveLitroEntrada(op.stDadosOperacao.volumeEntradaAtual + litrosEntradaAcum);
      oldLitrosEntrada = op.stDadosOperacao.volumeEntradaAtual;
      oldLitrosSaida = op.stDadosOperacao.volumeSaidaAtual;
    }
    vTaskDelay(pdMS_TO_TICKS(5000));
  }

}

// Função para atualizar as saídas das válvulas
void atualizaSaidas(){
  // Monitora o comando para ligar cada uma das valvulas
  // Como o rele aciona com 0V na saida, utilizamos o comando negado
  digitalWrite(VALV_ENTRADA_OUTPUT,!op.stStatusOperacao.valvulaEntrada);
  digitalWrite(VALV_SAIDA_OUTPUT,!op.stStatusOperacao.valvulaSaida);
  // Saida utilizada para alimentar o circuito do opto acoplador da placa de rele
  // Dessa forma, garantimos que o rele não aciona durante o boot do sistem
  digitalWrite(RELE_OUTPUT,1);
}

// Funcao para reconectar o broker caso a conexão caia
void reconectaBroker(){
  while(!mqtt.connected()){
    if(mqtt.connect("SmartLub-Unisal")){
      mqtt.subscribe("smartlub/mqtt/comandos");
      mqtt.subscribe("smartlub/mqtt/parametros");
    }else{
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }
}

