#ifndef AGENDA
#define AGENDA

#include "constante.h"  // Define MAX_REGISTROS = 100

// Límites para cadenas
#define MAX_ID         40
#define MAX_TITULO      50
#define MAX_DETALLE     200
#define MAX_INVITADOS   200  // Lista separada por comas: "Juan,Pedro,Ana"
#define MAX_RUT         13   // "12.345.678-9"

// Estructura Agenda
typedef struct {
    char id[MAX_ID];       //Considerando email como ID único             
    char fechaCita[11];                  
    char horaCita[6];                    
    char titulo[MAX_TITULO];             
    char detalle[MAX_DETALLE];           
    char invitados[MAX_INVITADOS];       
    int esCiclica;                       
    char fechaRepeticion[11];            
    char rutAsociado[MAX_RUT];             
} Agenda;


// Variables globales del módulo
extern Agenda agenda[MAX_REGISTROS];
extern int totalAgenda;

// Prototipos de funciones
int menuAgenda(void);
void agregarAgenda(void);
void listarAgendaPorRut(const char* rut);
void editarAgenda(void);
void eliminarAgenda(void);
void eliminarAgendaPorRut(const char* rut); // para eliminación en cascada

#endif
