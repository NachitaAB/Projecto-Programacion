#ifndef FINANZAS_H
#define FINANZAS_H

#include "constante.h"

#define MAX_RUT          13   // Formato: "12.345.678-9"

typedef struct {
    char numeroCuenta[20];
    char banco[50];
    char fechaTransaccion[11];
    double ingresos;
    double egresos;
    double saldoActual;
    char rutAsociado[MAX_RUT];
} Finanzas;

extern Finanzas finanzas[MAX_REGISTROS];
extern int totalFinanzas;

void eliminarFinanzasPorRut(const char* rut);
int menuFinanzas(void);
void agregarFinanzas(void);
void listarFinanzasPorRut(const char* rut);
void editarFinanzas(void);
void eliminarFinanzas(void);
void mostrarMontoEnCLPyUSD(double monto); // Muestra en peso chileno y dólar

#endif