#ifndef screens_h
#define screens_h

#include <Arduino.h>
#include <SSD1306.h>

struct telas{
    float volumeEntradaAtual;
    float volumeSaidaAtual;
    float volumeEntradaProgramado;
    float volumeSaidaProgramado;
    float temperaturaAtual;
    float temperaturaProgramada;
    String ip;
};

void configura();
void selecionaTela(int numTela, telas t1);
void telaPrincipal(telas t1);
void telaParametros(telas t1);


#endif