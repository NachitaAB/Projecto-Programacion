#ifndef VEHICULOS_H
#define VEHICULOS_H

#include "constante.h"  // Define MAX_REGISTROS = 100

// Límites para cadenas
#define MAX_TIPO       30   // Ej: "Sedán", "Camioneta", "SUV"
#define MAX_MARCA      30
#define MAX_MODELO     30
#define MAX_COLOR      20
#define MAX_PATENTE    10   // Formato chileno: "ABCD12" o "AB123CD"
#define MAX_RUT        13   // "12.345.678-9"

// Estructura Vehiculo
typedef struct {
    char tipo[MAX_TIPO];                
    int cc;                             
    char marca[MAX_MARCA];              
    char modelo[MAX_MODELO];            
    int anio;                           
    char color[MAX_COLOR];              
    char patente[MAX_PATENTE];          
    char rutAsociado[MAX_RUT];          
} Vehiculo;

// Variables globales del módulo
extern Vehiculo vehiculos[MAX_REGISTROS];
extern int totalVehiculos;

// Prototipos de funciones
int menuVehiculos(void);
void agregarVehiculos(void);
void listarVehiculosPorRut(const char* rut);
void editarVehiculos(void);
void eliminarVehiculos(void);
void eliminarVehiculosPorRut(const char* rut); // para eliminación en cascada

#endif // VEHICULOS_H