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

class Param{
public:
    static parametros par;
    static void lerParametros();
    static bool salvarParametros();
};



bool saveParameters(parametros par);
parametros readParameters();

#endif