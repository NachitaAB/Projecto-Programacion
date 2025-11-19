#ifndef TERRENOS
#define TERRENOS

#include "constante.h"  // Define MAX_REGISTROS = 100

// Límites para cadenas
#define MAX_ROL_TERRENO     24   // Ej: "12345-67"
#define MAX_DIRECCION       100
#define MAX_ESTADO          20   // "en venta", "arriendo", "activo fijo"
#define MAX_RUT             13   // "12.345.678-9"

// Estructura Terreno
typedef struct {
    int id;
    char rolTerreno[MAX_ROL_TERRENO];    
    double metrosTotales;                
    double metrosConstruccion;           
    char direccion[MAX_DIRECCION];       
    double tasacion;                     
    int aptoEdificio;                   
    char estado[MAX_ESTADO];             
    char rutAsociado[MAX_RUT];           
} Terreno;


// Variables globales del módulo
extern Terreno terrenos[MAX_REGISTROS];
extern int totalTerrenos;

// Prototipos de funciones
int menuTerrenos(void);
void agregarTerrenos(void);
void listarTerrenosPorRut(const char* rut);
void editarTerrenos(void);
void eliminarTerrenos(void);
void eliminarTerrenosPorRut(const char* rut); // para eliminación en cascada

#endif
