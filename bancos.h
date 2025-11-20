#ifndef BANCOS_H
#define BANCOS_H

#define NUM_BANCOS 11

extern const char* BANCOS_VALIDOS[NUM_BANCOS];

int esBancoValido(const char* banco);
void seleccionarBanco(char destino[], int tam);

#endif