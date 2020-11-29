#ifndef definicoes_h
#define definicoes_h

#define VALV_ENTRADA_OUTPUT 2
#define VALV_SAIDA_OUTPUT 6
#define SENSOR_ENTRADA_INPUT 34
#define SENSOR_SAIDA_INPUT 35
#define LIGA_SAIDA LOW
#define DESLIGA_SAIDA HIGH
#define ADDR_DISPLAY 0x3c
#define PINO_SDA 21
#define PINO_SCL 22

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