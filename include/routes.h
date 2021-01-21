#ifndef routes_h
#define routes_h

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

void routesConfigure();
void handleHTML(AsyncWebServerRequest *request);
void handleMain1(AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total);
void handleSendPar(AsyncWebServerRequest *request);
void handleReadPar(AsyncWebServerRequest *request);
void handleReadActualValue(AsyncWebServerRequest *request);
void handleChangeScreen(AsyncWebServerRequest *request);
void handleModoManual(AsyncWebServerRequest *request);
void handleModoAuto(AsyncWebServerRequest *request);
void handleModoSemi(AsyncWebServerRequest *request);
void handleModoNeutro(AsyncWebServerRequest *request);
void handleLigaEntrada(AsyncWebServerRequest *request);
void handleLigaSaida(AsyncWebServerRequest *request);
void handleDesligaEntrada(AsyncWebServerRequest *request);
void handleDesligaSaida(AsyncWebServerRequest *request);
void handleInterrompeTroca(AsyncWebServerRequest *request);
void handleIniciaTrocaAuto(AsyncWebServerRequest *request);
void handleIniciaEntradaSemi(AsyncWebServerRequest *request);
void handleFinalizaEntradaSemi(AsyncWebServerRequest *request);
void handleIniciaSaidaSemi(AsyncWebServerRequest *request);
void handleFinalizaSaidaSemi(AsyncWebServerRequest *request);
void handleHabilitaSimulacao(AsyncWebServerRequest *request);
void handleDesabilitaSimulacao(AsyncWebServerRequest *request);
#endif