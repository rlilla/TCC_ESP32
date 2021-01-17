#include "operacao.h"
#include "parametros.h"

EstadosAuto estadoAuto;

selecionaModo_t Operacao::stModo;
dadosOperacao_t Operacao::stDadosOperacao;
statusOperacao_t Operacao::stStatusOperacao;
operacao_t Operacao::stOperacao;

void Operacao::loopOperacao(){
    if (this->stModo.modoAuto){
        this->modoAuto();
    }else if (this->stModo.modoManual){
        this->modoManual();
    }else if (this->stModo.modoSemi){
        this->modoSemi();
    }else{
        this->modoNeutro();
    }  
}

void Operacao::modoManual(){
    this->stStatusOperacao.valvulaEntrada = this->stOperacao.ligaValvulaEntrada;
    this->stStatusOperacao.valvulaSaida = this->stOperacao.ligaValvulaSaida;
}

void Operacao::modoAuto(){
    this->stStatusOperacao.valvulaEntrada = true;
    switch(estadoAuto){
        // Estado inicial. Aguarda o sinal de start
        case INICIO:
            this->stOperacao.ligaValvulaEntrada = false;
            this->stOperacao.ligaValvulaSaida = false;
            this->stOperacao.ligaValvulaByPass = false;
            if(this->stOperacao.iniciaAuto){
                estadoAuto = AGUARDA_TEMPERATURA;
            }
            break;
        // Abre a valvula de bypass e aguarda a temperatura programada ser atingida
        case AGUARDA_TEMPERATURA:
            this->stOperacao.ligaValvulaByPass = true;
            if(this->stDadosOperacao.temperaturaAtual >= this->stDadosOperacao.temperaturaProg){
                this->stOperacao.ligaValvulaByPass = false;
                estadoAuto = AGUARDA_VOLUME_INICIAL;
            }
            break;
        // Abre a válvula de saída e fecha e de entrada e aguarda atingir o volume inicial de saída
        case AGUARDA_VOLUME_INICIAL:
            this->stOperacao.ligaValvulaSaida = true;
            if(this->stDadosOperacao.volumeSaidaAtual >= this->stDadosOperacao.volumeSaidaProg){
                this->stOperacao.ligaValvulaSaida = false;
                estadoAuto = MONITORA_DIFERENCA;
            }
            break;
        // Monitora a diferença entra volume de entrada e de saída
        case MONITORA_DIFERENCA:
            // Se a diferença for maior que a programada, liga saída e desliga entrada 
            if(this->stDadosOperacao.diferencaMaxAtual > this->stDadosOperacao.diferencaMaxProg){
                this->stOperacao.ligaValvulaEntrada = false;
                this->stOperacao.ligaValvulaSaida = true;
            // Se a diferença for menor que a programada, liaga entrada e desliga saída
            }else if(this->stDadosOperacao.diferencaMaxAtual < -this->stDadosOperacao.diferencaMaxProg){
                this->stOperacao.ligaValvulaEntrada = true;
                this->stOperacao.ligaValvulaSaida = false;
            // Se a diferença estiver dentro do limite, liga as duas válvulas
            }else{
                this->stOperacao.ligaValvulaEntrada = true;
                this->stOperacao.ligaValvulaSaida = true;
            }
            // Aguarda que o volume de entrada programado seja atingido ou que a troca seja interrompida
            if(this->stOperacao.interrompeTroca || this->stDadosOperacao.volumeEntradaAtual >= this->stDadosOperacao.volumeEntradaProg){
                estadoAuto = FINALIZA;
            }
            break;
        case FINALIZA:
            this->stOperacao.interrompeTroca = false;
            this->stOperacao.ligaValvulaEntrada = false;
            this->stOperacao.ligaValvulaSaida = false;
            break; 
    }
}

void Operacao::modoNeutro(){
    this->stStatusOperacao.valvulaEntrada = false;
    this->stStatusOperacao.valvulaSaida = false;
    this->stOperacao.iniciaSemiEntrada = false;
    this->stOperacao.iniciaSemiSaida = false;
    this->stOperacao.iniciaAuto = false;
    this->stOperacao.interrompeTroca = false;
}

void Operacao::modoSemi(){
    this->stStatusOperacao.valvulaEntrada = (this->stOperacao.iniciaSemiEntrada & (this->stDadosOperacao.volumeEntradaAtual <= this->stDadosOperacao.volumeEntradaProg));
    this->stStatusOperacao.valvulaSaida = (this->stOperacao.iniciaSemiSaida & (this->stDadosOperacao.volumeSaidaAtual <= this->stDadosOperacao.volumeSaidaProg));
}