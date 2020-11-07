
#include "routes.h"
#include <ArduinoJson.h>
#include "parametros.h"

extern parametros valoresAtuais;
extern int numTela;
AsyncWebServer server(80);
StaticJsonDocument<200> doc;

void routesConfigure(){
    server.on("/",HTTP_GET, handleMain);
    server.on("/sendPar",HTTP_POST,handleSendPar);
    server.on("/readPar",HTTP_GET,handleReadPar);
    server.on("/readActValues",HTTP_GET,handleReadActualValue);
    server.on("/changeScreen",HTTP_POST,handleChangeScreen);
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
    request->send(200,"application/json",par);
}

void handleReadActualValue(AsyncWebServerRequest *request){
    doc["volEntrada"] = valoresAtuais.volumeEntrada;
    doc["volSaida"] = valoresAtuais.volumeSaidaInicial;
    doc["diferenca"] = valoresAtuais.diferencaMaxima;
    doc["temp"] = valoresAtuais.temperatura;
    String retorno;
    serializeJson(doc,retorno);
    request->send(200,"application/json",retorno);
}

void handleChangeScreen(AsyncWebServerRequest *request){
    numTela=request->getParam("tela",true)->value().toInt();
    request->send(200,"text/plain","OK");
}