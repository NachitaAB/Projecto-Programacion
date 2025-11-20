#ifndef PROPIEDADES_H
#define PROPIEDADES_H

#include "constante.h"  // Contiene #define MAX_REGISTROS 100

// Límites de cadenas
#define MAX_ROL          20   // Ej: "12345-67"
#define MAX_RUT          13   // Formato: "12.345.678-9"

// Estructura Propiedad
typedef struct {
    char rol[12];           // Formato: "MM-PP" → máximo 10 chars + '\0'
    int m2Construidos;
    int ano;
    char torre;             // 'A' a 'H'
    char departamento[15];
    int numPiezas;
    int numBanos;
    char rutAsociado[13];
} Propiedad;

// Variables globales del módulo
extern Propiedad propiedades[MAX_REGISTROS];
extern int totalPropiedades;

// Prototipos de funciones
int menuPropiedades(void);
void agregarPropiedades(void);
void listarPropiedadesPorRut(const char* rut);
void editarPropiedades(void);
void eliminarPropiedades(void);
void eliminarPropiedadesPorRut(const char* rut); // para eliminación en cascada

#endif