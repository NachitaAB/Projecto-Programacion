#ifndef VALIDACIONES_H
#define VALIDACIONES_H
#include "constante.h"

typedef struct {
    int dia;
    int mes;
    int anio;
} Fecha;

// Funciones de validación requeridas por los módulos
int validarRut(const char* rut);
int validarFecha(const char* fecha);
int validarEmail(const char* email);
int validarRol(const char* rol);

// Función para limpiar el buffer de entrada (útil si mezclas fgets/scanf)
void limpiar_buffer(void);

// Conversión segura de cadenas a números
int leerEntero(const char* str, int* valor);
int leerDouble(const char* str, double* valor);

#endif