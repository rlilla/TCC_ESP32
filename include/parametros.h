#ifndef parammetros_h
#define parametros_h

#include <Arduino.h>
#include <Preferences.h>

struct parametros{
    float volumeEntrada;
    float volumeSaidaInicial;
    float diferencaMaxima;
    float temperatura;
};

bool saveParameters(parametros par);
parametros readParameters();

#endif