#ifndef definicoes_h
#define definicoes_h

#define VALV_ENTRADA_OUTPUT 2
#define VALV_SAIDA_OUTPUT 4
#define RELE_OUTPUT 14
#define SENSOR_ENTRADA_INPUT 35
#define SENSOR_SAIDA_INPUT 34
#define LIGA_SAIDA LOW
#define DESLIGA_SAIDA HIGH
#define ADDR_DISPLAY 0x3c
#define PINO_SDA 21
#define PINO_SCL 22
#define ONE_WIRE_BUS 18

struct valoresAtuais_t{
    float volumeEntrada;
    float volumeSaida;
    float temperatura;
};

struct valoresProgramados_t{
    float volumeEntrada;
    float volumeSaidaInicial;
    float diferencaMaxima;
    float temperatura;
    float volumeFlushing;
};



#endif