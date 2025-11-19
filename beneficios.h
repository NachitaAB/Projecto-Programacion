#ifndef BENEFICIOS_H
#define BENEFICIOS_H

#include "constante.h"  // Define MAX_REGISTROS = 100

// Límites de cadenas
#define MAX_CAJA          50
#define MAX_DETALLE       200
#define MAX_CONDICIONES   200
#define MAX_RUT           13   // "12.345.678-9"

// Estructura Beneficio
typedef struct {
    int id;                                     
    char cajaCompensacion[MAX_CAJA];            
    char detalle[MAX_DETALLE];                  
    double porcentajeDescuento;                 
    char condiciones[MAX_CONDICIONES];          
    int cargaFamiliarMax;                       
    char fechaVigencia[11];                     
    char rutAsociado[MAX_RUT];                  
} Beneficio;

// Variables globales del módulo
extern Beneficio beneficios[MAX_REGISTROS];
extern int totalBeneficios;

// Prototipos de funciones
void menuBeneficios(void);
void agregarBeneficios(void);
void listarBeneficiosPorRut(const char* rut);
void editarBeneficios(void);
void eliminarBeneficios(void);
void eliminarBeneficiosPorRut(const char* rut); // para eliminación en cascada

#endif
