
#include "routes.h"

extern String values;

void handleMain(AsyncWebServerRequest *request){
    digitalWrite(4,!digitalRead(4));
    String teste=request->getParam("par1",true)->value();
    Serial.println(teste);
    request->send(200,"text/plain","OK");
}

void handleSendPar(AsyncWebServerRequest *request){
    String par=request->getParam("parametros",true)->value(); 
    Serial.println(par);
    request->send(200,"text/plain","OK"); 
}

void handleReadPar(AsyncWebServerRequest *request){
    String par="{\"teste\":123}";
    request->send(200,"application/json",par);
}

void handleReadActualValue(AsyncWebServerRequest *request){
    request->send(200,"application/json",values);
}