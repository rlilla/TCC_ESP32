#include "banco.h"
#include <FirebaseESP32.h>

// Instancia do FirebaseData
FirebaseData fbdo;

// Funcao para escrever o valor de litros de entrada
void escreveLitroEntrada(float valor){
    Firebase.begin(BANCO_HOST, BANCO_AUTH);
    Firebase.reconnectWiFi(true);
    Firebase.setReadTimeout(fbdo, 1000 * 60);
    Firebase.setwriteSizeLimit(fbdo, "tiny");
    Firebase.setFloatDigits(2);
    Firebase.setDoubleDigits(6);
    String path = "/Sensores";
    bool resFB = Firebase.setFloat(fbdo,path+"/Entrada",valor);
}

// Funcao para escrever o valor de litros de saida
void escreveLitroSaida(float valor){
    Firebase.begin(BANCO_HOST, BANCO_AUTH);
    Firebase.reconnectWiFi(true);
    Firebase.setReadTimeout(fbdo, 1000 * 60);
    Firebase.setwriteSizeLimit(fbdo, "tiny");
    Firebase.setFloatDigits(2);
    Firebase.setDoubleDigits(6);
    String path = "/Sensores";
    bool resFB = Firebase.setFloat(fbdo,path+"/Saida",valor);
}

// Funcao para ler o valor de litros de entrada
float lerLitroEntrada(){
    Firebase.begin(BANCO_HOST, BANCO_AUTH);
    Firebase.reconnectWiFi(true);
    Firebase.setReadTimeout(fbdo, 1000 * 60);
    Firebase.setwriteSizeLimit(fbdo, "tiny");
    Firebase.setFloatDigits(2);
    Firebase.setDoubleDigits(6);
    String path = "/Sensores";
    Firebase.getFloat(fbdo,path+"/Entrada");
    return fbdo.floatData();
}

// Funcao para ler o valor de litros de saida
float lerLitroSaida(){
    Firebase.begin(BANCO_HOST, BANCO_AUTH);
    Firebase.reconnectWiFi(true);
    Firebase.setReadTimeout(fbdo, 1000 * 60);
    Firebase.setwriteSizeLimit(fbdo, "tiny");
    Firebase.setFloatDigits(2);
    Firebase.setDoubleDigits(6);
    String path = "/Sensores";
    Firebase.getFloat(fbdo,path+"/Saida");
    return fbdo.floatData();
}