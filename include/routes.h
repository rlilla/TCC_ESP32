#ifndef routes_h
#define routes_h

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

void handleMain(AsyncWebServerRequest *request);
void handleMain1(AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total);
void handleSendPar(AsyncWebServerRequest *request);
void handleReadPar(AsyncWebServerRequest *request);
void handleReadActualValue(AsyncWebServerRequest *request);


#endif