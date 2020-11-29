#include "parametros.h"
#include "definicoes.h"
#include "operacao.h"

Preferences pref;

bool saveParameters(parametros par){
    bool retorno = false;
    pref.begin("Par");
    pref.putFloat("volEnt", par.volumeEntrada);
    pref.putFloat("volSai", par.volumeSaidaInicial);
    pref.putFloat("dif", par.diferencaMaxima);
    pref.putFloat("temp", par.temperatura);
    pref.end();
    retorno = true;
    return retorno;
}

parametros readParameters(){
    parametros par;
    pref.begin("Par");
    par.volumeEntrada = pref.getFloat("volEnt",0);
    par.volumeSaidaInicial = pref.getFloat("volSai", 0);
    par.diferencaMaxima = pref.getFloat("dif", 0);
    par.temperatura = pref.getFloat("temp", 0);
    pref.end();
    return par;
}

