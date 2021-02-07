#include "screens.h"
#include "parametros.h"
#include "definicoes.h"
#include "operacao.h"

// Inicializa o display SSD1306
SSD1306 display(ADDR_DISPLAY,PINO_SDA,PINO_SCL);

const unsigned char wifi1 [] = {
0x18, 0x1C, 0x7C, 0x7C, 0xFE, 0xFE, 0xFE, 0x7C, 0x7C, 0x1C, 0x00, 0x00, 
};

// Configura display
void configura(){
    display.init();
    display.clear();
    display.display();
}

// Funcao para selecionar a tela a ser mostrada no display
void selecionaTela(int numTela, telas t1){
    switch(numTela){
        case 0:
            telaPrincipal(t1);
            break;
        case 1:
            telaParametros(t1);
            break;
        case 2:
            telaModoManual(t1);
            break;
        case 3:
            telaModoSemi(t1);
            break;
        default:
            telaPrincipal(t1);
    }
}

// Tela modo Auto
void telaPrincipal(telas t1){
    display.clear();
    display.drawString(0,0,"PRINCIPAL");
    display.drawString(0,15,t1.ip);
    display.drawString(0,25,"Vol. Ent.: " + String(Operacao::stDadosOperacao.volumeEntradaAtual));
    display.drawString(0,35,"Vol. Saida: " + String(Operacao::stDadosOperacao.volumeSaidaAtual));
    display.drawString(0,45,"Temp: " + String(Operacao::stDadosOperacao.temperaturaAtual));
    display.display();
}
// Tela modo Manual
void telaModoManual(telas t1){
    String statusValvEntrada, statusValvSaida;
    display.clear();
    display.drawString(0,0,"MANUAL");
    statusValvEntrada=Operacao::stStatusOperacao.valvulaEntrada?"ON":"OFF";
    statusValvSaida=Operacao::stStatusOperacao.valvulaSaida?"ON":"OFF";
    display.drawString(0,15,"Entrada:" + statusValvEntrada);
    display.drawString(0,25,"Saida:" + statusValvSaida);
    display.drawString(0,35,"Vol. Ent.: " + String(Operacao::stDadosOperacao.volumeEntradaAtual));
    display.drawString(0,45,"Vol. Saida: " + String(Operacao::stDadosOperacao.volumeSaidaAtual));
    display.display();
}
// Tela ao iniciar sem modo selecionado
void telaParametros(telas t1){
    display.clear();
    display.drawString(0,0,"PARAMETROS");
    Param::lerParametros();
    display.drawString(0,10,"Vol. Ent.: " + String(Param::par.volumeEntrada));
    display.drawString(0,20,"Vol. Saida.: " + String(Param::par.volumeSaidaInicial));
    display.drawString(0,30,"Diferenca: " + String(Param::par.diferencaMaxima));
    display.drawString(0,40,"Temperatura: " + String(Param::par.temperatura));
    //display.drawFastImage(0,0,12,8,wifi1);
    display.display();
}
// Tela modo semi
void telaModoSemi(telas t1){
    display.clear();
    display.drawString(0,0,"SEMI AUTO");
    display.drawString(0,15,t1.ip);
    display.drawString(0,25,"Vol. Ent.: " + String(Operacao::stDadosOperacao.volumeEntradaAtual));
    display.drawString(0,35,"Vol. Saida: " + String(Operacao::stDadosOperacao.volumeSaidaAtual));
    display.display();
}