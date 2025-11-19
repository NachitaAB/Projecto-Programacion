#ifndef MIEMBRO_H
#define MIEMBRO_H

#include "constante.h"  // Contiene #define MAX_REGISTROS 100

// Definición de límites para campos de texto
#define MAX_NOMBRE      20   // suficiente para 19 caracteres + '\0'
#define MAX_RUT         13   // "12.345.678-9" + '\0'
#define MAX_DIRECCION   60
#define MAX_EMAIL       40
#define MAX_FECHA       11   // "dd/mm/aaaa" + '\0'

// Estructura del miembro (cliente)
typedef struct {
    char nombre[MAX_NOMBRE];
    char apellido[MAX_NOMBRE];
    char rut[MAX_RUT];
    char direccion[MAX_DIRECCION];
    int edad;
    char fechaNacimiento[MAX_FECHA];
    char sexo;          // 'M' o 'F'
    int casado;         // 0 = false, 1 = true
    char email[MAX_EMAIL];
} Miembro;

// Variables globales (visibles desde otros módulos)
extern Miembro miembros[MAX_REGISTROS];
extern int totalMiembros;

// Prototipos de funciones del módulo
int menuMiembro(void);
int crearMiembro(void);
void listarMiembros(void);
void editarMiembro(void);
void eliminarMiembro(void);
int existeMiembro(const char* rut);  // usada por finanzas.c, propiedades.c, etc.

#endif // MIEMBRO_H
