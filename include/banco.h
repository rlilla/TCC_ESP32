#ifndef BANCO_H
#define BANCO_H

#include <Arduino.h>

// Caminho e credencial do banco de dados RealTime
#define BANCO_HOST "https://esp32-cb328-default-rtdb.firebaseio.com/"
#define BANCO_AUTH "yZi4MR8GwAdHaYU1uQfgD3dU0AtKIj00fjWkL5ZI"

// Prototipo de funcoes de escrita e leitura do banco de dados RealTime
void escreveLitroEntrada(float valor);
void escreveLitroSaida(float valor);
float lerLitroEntrada();
float lerLitroSaida();

#endif