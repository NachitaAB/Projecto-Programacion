#ifndef FECHAS_H
#define FECHAS_H

// Retorna 1 si el año es bisiesto
int esBisiesto(int anio);

// Devuelve días en un mes (considera bisiesto)
int diasEnMes(int mes, int anio);

// Menú interactivo para seleccionar fecha (dd/mm/aaaa)
void seleccionarFecha(char fechaSalida[11]);

// Menú interactivo para seleccionar hora (hh:mm)
void seleccionarHora(char horaSalida[6]);

#endif