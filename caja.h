#ifndef CAJA_H
#define CAJA_H

#define NUM_CAJA 5

extern const char* CAJA_VALIDOS[NUM_CAJA];

int esCajaValida(const char* caja);
void seleccionarCaja(char destino[], int tam);

#endif
