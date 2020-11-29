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
}

void Operacao::modoNeutro(){
    this->stStatusOperacao.valvulaEntrada = false;
    this->stStatusOperacao.valvulaSaida = false;
}

void Operacao::modoSemi(){
    this->stStatusOperacao.valvulaEntrada = (this->stOperacao.iniciaSemiEntrada & (this->stDadosOperacao.volumeEntradaAtual <= this->stDadosOperacao.volumeEntradaProg));
    this->stStatusOperacao.valvulaSaida = (this->stOperacao.iniciaSemiSaida & (this->stDadosOperacao.volumeSaidaAtual <= this->stDadosOperacao.volumeSaidaProg));
}