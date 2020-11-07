#include "screens.h"
#include "parametros.h"
#include "definicoes.h"

SSD1306 display(ADDR_DISPLAY,PINO_SDA,PINO_SCL);

const unsigned char wifi1 [] = {
0x18, 0x1C, 0x7C, 0x7C, 0xFE, 0xFE, 0xFE, 0x7C, 0x7C, 0x1C, 0x00, 0x00, 
};


void configura(){
    display.init();
    display.clear();
    display.display();
}

void selecionaTela(int numTela, telas t1){
    switch(numTela){
        case 0:
            telaPrincipal(t1);
            break;
        case 1:
            telaParametros(t1);
            break;
        default:
            telaPrincipal(t1);
    }
}

void telaPrincipal(telas t1){
    display.clear();
    display.drawString(0,0,"PRINCIPAL");
    display.drawString(0,15,t1.ip);
    display.drawString(0,25,"Vol. Ent.: " + String(t1.volumeEntradaAtual));
    display.display();
}

void telaParametros(telas t1){
    display.clear();
    display.drawString(0,0,"PARAMETROS");
    parametros par=readParameters();
    display.drawString(0,10,"Vol. Ent.: " + String(par.volumeEntrada));
    display.drawString(0,20,"Vol. Saida.: " + String(par.volumeSaidaInicial));
    display.drawString(0,30,"Diferenca: " + String(par.diferencaMaxima));
    display.drawString(0,40,"Temperatura: " + String(par.temperatura));
    //display.drawFastImage(0,0,12,8,wifi1);
    display.display();
}