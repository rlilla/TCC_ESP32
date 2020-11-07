#include "operacao.h"
#include "parametros.h"

Estados estado;

extern parametros valoresAtuais;
extern parametros par;

void modoManual(){
    

}

void modoAuto(){

    switch(estado){
        case INICIO:
            if(operacao.iniciaAuto){
                estado = AGUARDA_TEMPERATURA;
            }
            break;
        case AGUARDA_TEMPERATURA:
            if(valoresAtuais.temperatura >= par.temperatura){
                estado = AGUARDA_VOLUME_INICIAL;
            }
            break;
        case AGUARDA_VOLUME_INICIAL:

            break;
        case MONITORA_DIFERENCA:

            break;
        
        case FINALIZA:

            break;

        default:

            break;

    }

}

void ciclico(){
    if(operacao.modoManAuto){
        modoAuto();
    }else{
        estado = INICIO;
        modoManual();
    }
}