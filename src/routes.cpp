#include "routes.h"
#include <ArduinoJson.h>
#include "parametros.h"
#include "operacao.h"

extern parametros valoresAtuais;
extern int numTela;
AsyncWebServer server(80);
StaticJsonDocument<200> doc;

// Função para configurar as rota da API
void routesConfigure(){
    server.on("/",HTTP_GET, handleMain);
    server.on("/sendPar",HTTP_POST,handleSendPar);
    server.on("/readPar",HTTP_GET,handleReadPar);
    server.on("/readActValues",HTTP_GET,handleReadActualValue);
    server.on("/changeScreen",HTTP_POST,handleChangeScreen);
    server.on("/modoManual",HTTP_GET,handleModoManual);
    server.on("/modoAuto",HTTP_GET,handleModoAuto);
    server.on("/modoSemi",HTTP_GET,handleModoSemi);
    server.on("/modoNeutro",HTTP_GET,handleModoNeutro);
    server.on("/ligaEntrada",HTTP_GET,handleLigaEntrada);
    server.on("/desligaEntrada",HTTP_GET,handleDesligaEntrada);
    server.on("/ligaSaida",HTTP_GET,handleLigaSaida);
    server.on("/desligaSaida",HTTP_GET,handleDesligaSaida);
    server.begin();
}

void handleMain(AsyncWebServerRequest *request){
    digitalWrite(4,!digitalRead(4));
    String teste=request->getParam("par1",true)->value();
    Serial.println(teste);
    request->send(200,"text/plain","OK");
}

void handleSendPar(AsyncWebServerRequest *request){
    String par1=request->getParam("parametros",true)->value(); 
    deserializeJson(doc,par1);
    parametros par;
    par.volumeEntrada = doc["volEntrada"];
    par.volumeSaidaInicial = doc["volSaida"];
    par.diferencaMaxima = doc["diferenca"];
    par.temperatura = doc["temp"];
    bool saved = saveParameters(par);
    String retorno = "OK";
    if(!saved) retorno = "NOK"; 
    request->send(200,"text/plain","OK"); 
}

void handleReadPar(AsyncWebServerRequest *request){
    String par="{\"teste\":123}";
    AsyncWebServerResponse *response = request -> beginResponse(200,"application/json",par);
    response->addHeader("Access-Control-Allow-Origin","*");
    request->send(response);
}

void handleReadActualValue(AsyncWebServerRequest *request){
    doc["volEntrada"] = valoresAtuais.volumeEntrada;
    doc["volSaida"] = valoresAtuais.volumeSaidaInicial;
    doc["diferenca"] = valoresAtuais.diferencaMaxima;
    doc["temp"] = valoresAtuais.temperatura;
    String retorno;
    serializeJson(doc,retorno);
    AsyncWebServerResponse *response = request -> beginResponse(200,"application/json",retorno);
    response->addHeader("Access-Control-Allow-Origin","*");
    request->send(response);
}

void handleChangeScreen(AsyncWebServerRequest *request){
    numTela=request->getParam("tela",true)->value().toInt();
    request->send(200,"text/plain","OK");
}

void handleModoManual(AsyncWebServerRequest *request){
    Operacao::stModo.modoManual = true;
    Operacao::stModo.modoAuto = false;
    Operacao::stModo.modoSemi = false;
    request->send(200,"text/plain","OK");
}

void handleModoAuto(AsyncWebServerRequest *request){
    Operacao::stModo.modoManual = false;
    Operacao::stModo.modoAuto = true;
    Operacao::stModo.modoSemi = false;
    request->send(200,"text/plain","OK");
}

void handleModoSemi(AsyncWebServerRequest *request){
    Operacao::stModo.modoManual = false;
    Operacao::stModo.modoAuto = false;
    Operacao::stModo.modoSemi = true;
    request->send(200,"text/plain","OK");
}

void handleModoNeutro(AsyncWebServerRequest *request){
    Operacao::stModo.modoManual = false;
    Operacao::stModo.modoAuto = false;
    Operacao::stModo.modoSemi = false;
    request->send(200,"text/plain","OK");
}

void handleLigaEntrada(AsyncWebServerRequest *request){
    Operacao::stOperacao.ligaValvulaEntrada = true;
    request->send(200,"text/plain","OK");
}

void handleLigaSaida(AsyncWebServerRequest *request){
    Operacao::stOperacao.ligaValvulaSaida = true;
    request->send(200,"text/plain","OK");
}

void handleDesligaEntrada(AsyncWebServerRequest *request){
    Operacao::stOperacao.ligaValvulaEntrada = false;
    request->send(200,"text/plain","OK");
}

void handleDesligaSaida(AsyncWebServerRequest *request){
    Operacao::stOperacao.ligaValvulaSaida = false;
    request->send(200,"text/plain","OK");
}