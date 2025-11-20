#ifndef CAJA_H
#define CAJA_H

#define NUM_CAJA 5

extern const char* CAJAS_VALIDAS[NUM_CAJA];

int esCajaValida(const char* caja);
void seleccionarCaja(char destino[], int tam);

#endif