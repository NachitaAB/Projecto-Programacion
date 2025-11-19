#ifndef FONDOS_MUTUOS
#define FONDOS_MUTUOS

#include "constante.h"  // Define MAX_REGISTROS = 100

// Límites para cadenas
#define MAX_ID         40  
#define MAX_BANCO      50
#define MAX_RUT        13   // "12.345.678-9"

// Estructura FondosMutuos
typedef struct {
    char id[MAX_ID];             
    double ahorros;   
    double interesTotales;                       
    double tasaInteres;
    char banco[MAX_BANCO];
    double proyeccionMensual;
    double proyeccionAnual;
    char rutAsociado[MAX_RUT];       
} FondoMutuo;

// Variables globales del módulo
extern FondoMutuo fondosMutuos[MAX_REGISTROS];
extern int totalFondosMutuos;

// Prototipos de funciones
void menuFondosMutuos(void);
void agregarFondosMutuos(void);
void listarFondosMutuosPorRut(const char* rut);
void editarFondosMutuos(void);
void eliminarFondosMutuos(void);
void eliminarFondosMutuosPorRut(const char* rut); // para eliminación en cascada

// Calcula la proyección de ahorros a N periodos usando interés compuesto
double calcularProyeccionRecursiva(double capital, double tasa, int periodos);

#endif
