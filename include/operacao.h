#ifndef operacao_h
#define operacao_h

#include <Arduino.h>

#define SAIDA_VALV_ENTRADA 4
#define SAIDA_VAL_SAIDA 5

void modoManual();
void modoAuto();
void ciclico();


enum Estados{
    INICIO,
    AGUARDA_TEMPERATURA,
    AGUARDA_VOLUME_INICIAL,
    MONITORA_DIFERENCA,
    FINALIZA
};

struct operacao_t{
    bool ligaValvulaEntrada;
    bool ligaValvulaSaida;
    bool modoManAuto;
    bool iniciaAuto;
};

operacao_t operacao;



#endif
