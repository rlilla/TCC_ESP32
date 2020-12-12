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
    request->send(200,"text/plain","OK");
}

void handleSendPar(AsyncWebServerRequest *request){
    String par1=request->getParam("parametros",true)->value(); 
    deserializeJson(doc,par1);
    Param::par.volumeEntrada = doc["volEntrada"];
    Param::par.volumeSaidaInicial = doc["volSaida"];
    Param::par.diferencaMaxima = doc["diferenca"];
    Param::par.temperatura = doc["temp"];
    bool saved = Param::salvarParametros();
    String retorno = "OK";
    if(!saved) retorno = "NOK"; 
    AsyncWebServerResponse *response = request -> beginResponse(200,"text/plain",retorno);
    response->addHeader("Access-Control-Allow-Origin","*");
    request->send(response);
}

void handleReadPar(AsyncWebServerRequest *request){
    Param::lerParametros();
    doc["volEntrada"] = Param::par.volumeEntrada;
    doc["volSaida"] = Param::par.volumeSaidaInicial;
    doc["diferenca"] = Param::par.diferencaMaxima;
    doc["temp"] = Param::par.temperatura;
    String retornoPar;
    serializeJson(doc,retornoPar);
    AsyncWebServerResponse *response = request -> beginResponse(200,"application/json",retornoPar);
    response->addHeader("Access-Control-Allow-Origin","*");
    request->send(response);
}

void handleReadActualValue(AsyncWebServerRequest *request){
    doc["volEntrada"] = valoresAtuais.volumeEntrada;
    doc["volSaida"] = valoresAtuais.volumeSaidaInicial;
    doc["diferenca"] = valoresAtuais.diferencaMaxima;
    doc["temp"] = valoresAtuais.temperatura;
    doc["modo"] = "SEM MODO";
    if (Operacao::stModo.modoAuto) doc["modo"] = "AUTOMATICO";
    if (Operacao::stModo.modoManual) doc["modo"] = "MANUAL";
    if (Operacao::stModo.modoSemi) doc["modo"] = "SEMI AUTOMATICO";
    if (Operacao::stModo.semModo) doc["modo"] = "SEM MODO";
    String retorno;
    serializeJson(doc,retorno);
    AsyncWebServerResponse *response = request -> beginResponse(200,"application/json",retorno);
    response->addHeader("Access-Control-Allow-Origin","*");
    request->send(response);
}

void handleChangeScreen(AsyncWebServerRequest *request){
    numTela=request->getParam("tela",true)->value().toInt();
    AsyncWebServerResponse *response = request -> beginResponse(200,"text/plain","OK");
    response->addHeader("Access-Control-Allow-Origin","*");
    request->send(response);
}

void handleModoManual(AsyncWebServerRequest *request){
    Operacao::stModo.modoManual = true;
    Operacao::stModo.modoAuto = false;
    Operacao::stModo.modoSemi = false;
    AsyncWebServerResponse *response = request -> beginResponse(200,"text/plain","OK");
    response->addHeader("Access-Control-Allow-Origin","*");
    request->send(response);
}

void handleModoAuto(AsyncWebServerRequest *request){
    Operacao::stModo.modoManual = false;
    Operacao::stModo.modoAuto = true;
    Operacao::stModo.modoSemi = false;
    AsyncWebServerResponse *response = request -> beginResponse(200,"text/plain","OK");
    response->addHeader("Access-Control-Allow-Origin","*");
    request->send(response);
}

void handleModoSemi(AsyncWebServerRequest *request){
    Operacao::stModo.modoManual = false;
    Operacao::stModo.modoAuto = false;
    Operacao::stModo.modoSemi = true;
    AsyncWebServerResponse *response = request -> beginResponse(200,"text/plain","OK");
    response->addHeader("Access-Control-Allow-Origin","*");
    request->send(response);
}

void handleModoNeutro(AsyncWebServerRequest *request){
    Operacao::stModo.modoManual = false;
    Operacao::stModo.modoAuto = false;
    Operacao::stModo.modoSemi = false;
    AsyncWebServerResponse *response = request -> beginResponse(200,"text/plain","OK");
    response->addHeader("Access-Control-Allow-Origin","*");
    request->send(response);
}

void handleLigaEntrada(AsyncWebServerRequest *request){
    Operacao::stOperacao.ligaValvulaEntrada = true;
    AsyncWebServerResponse *response = request -> beginResponse(200,"text/plain","OK");
    response->addHeader("Access-Control-Allow-Origin","*");
    request->send(response);
}

void handleLigaSaida(AsyncWebServerRequest *request){
    Operacao::stOperacao.ligaValvulaSaida = true;
    AsyncWebServerResponse *response = request -> beginResponse(200,"text/plain","OK");
    response->addHeader("Access-Control-Allow-Origin","*");
    request->send(response);
}

void handleDesligaEntrada(AsyncWebServerRequest *request){
    Operacao::stOperacao.ligaValvulaEntrada = false;
    AsyncWebServerResponse *response = request -> beginResponse(200,"text/plain","OK");
    response->addHeader("Access-Control-Allow-Origin","*");
    request->send(response);
}

void handleDesligaSaida(AsyncWebServerRequest *request){
    Operacao::stOperacao.ligaValvulaSaida = false;
    AsyncWebServerResponse *response = request -> beginResponse(200,"text/plain","OK");
    response->addHeader("Access-Control-Allow-Origin","*");
    request->send(response);
}