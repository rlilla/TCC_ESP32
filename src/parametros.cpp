#include "parametros.h"
#include "definicoes.h"
#include "operacao.h"


Preferences pref;
parametros Param::par;

bool Param::salvarParametros(){
    bool retorno = false;
    pref.begin("Par");
    pref.putFloat("volEnt", Param::par.volumeEntrada);
    pref.putFloat("volSai", Param::par.volumeSaidaInicial);
    pref.putFloat("dif", Param::par.diferencaMaxima);
    pref.putFloat("temp", Param::par.temperatura);
    pref.end();
    retorno = true;
    return retorno;
}

void Param::lerParametros(){
    pref.begin("Par");
    Param::par.volumeEntrada = pref.getFloat("volEnt",0);
    Param::par.volumeSaidaInicial = pref.getFloat("volSai", 0);
    Param::par.diferencaMaxima = pref.getFloat("dif", 0);
    Param::par.temperatura = pref.getFloat("temp", 0);
    pref.end();
}
