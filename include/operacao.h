#ifndef operacao_h
#define operacao_h

#include <Arduino.h>
#include "definicoes.h"

#define SAIDA_VALV_ENTRADA 4
#define SAIDA_VAL_SAIDA 5




enum EstadosAuto{
    INICIO,
    AGUARDA_TEMPERATURA,
    AGUARDA_VOLUME_INICIAL,
    MONITORA_DIFERENCA,
    FINALIZA
};

enum EstadosSemi{
    INICIO1,
    AGUARDA_VOL_ENTRADA,
    AGUARDA_VOL_SAIDA,
    FINALIZA1
};
struct operacao_t{
    bool ligaValvulaEntrada;
    bool ligaValvulaSaida;
    bool modoManAuto;
    bool iniciaAuto;
    bool iniciaSemiEntrada;
    bool iniciaSemiSaida;
};

struct statusOperacao_t{
    bool valvulaEntrada;
    bool valvulaSaida;
};

struct dadosOperacao_t{
    float volumeEntradaProg;
    float volumeEntradaAtual;
    float volumeSaidaProg;
    float volumeSaidaAtual;
    float diferencaMaxProg;
    float diferencaMaxAtual;
    float temperaturaProg;
    float temperaturaAtual;
};

struct selecionaModo_t{
    bool semModo;
    bool modoManual;
    bool modoSemi;
    bool modoAuto;
};

class Operacao{
public:
    void loopOperacao();
    void modoManual();
    void modoAuto();
    void modoSemi();
    void modoNeutro();
    static selecionaModo_t stModo;
    static dadosOperacao_t stDadosOperacao;
    static statusOperacao_t stStatusOperacao;
    static operacao_t stOperacao;
};

#endif
