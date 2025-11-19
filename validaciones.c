#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h> //INT_MIN: define el valor mínimo para un entero, INT_MAX: define el valor máximo para un entero
#include "validaciones.h"
#include "constante.h"

int leerEntero(const char* str, int* valor) {
    if (!str || *str == '\0') return 0;
    char* end;
    long val = strtol(str, &end, 10);
    // Ignorar espacios al final
    while (*end && isspace((unsigned char)*end)) end++;
    if (*end == '\0' && val >= INT_MIN && val <= INT_MAX) {
        *valor = (int)val;
        return 1;
    }
    return 0;
}

int leerDouble(const char* str, double* valor) {
    if (!str || *str == '\0') return 0;
    char* end;
    double val = strtod(str, &end);
    while (*end && isspace((unsigned char)*end)) end++;
    if (*end == '\0') {
        *valor = val;
        return 1;
    }
    return 0;
}

void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int sumaRecursiva(int rut, int multiplicador) {
    if (rut == 0){
        return 0;
    }
    int digito = rut % 10;
    int nuevo_mult = (multiplicador == 7) ? 2 : multiplicador + 1; //operador ternario 
    return digito * multiplicador + sumaRecursiva(rut / 10, nuevo_mult);
}

int calcularDV(int rut) {
    int suma = sumaRecursiva(rut, 2); //ejemplo 12.345.678-9 (8*2)+(7*3)+(6*4)+(5*5)+(4*6)+(3*7)+(2*2)+(1*3)
    int resto = suma % 11; //Resto de dividir la suma entre 11
    int resultado = 11 - resto; //Calcula el valor base  

    if (resultado == 11)
        return 0; //El rut termina en 0
    else if (resultado == 10)
        return -1; //El rut termina en k
    else
        return resultado;
}


// En validarRut() o en una función auxiliar
int esRutEnRangoValido(long rutNumerico) {
    // RUT de personas: 1.000.000 a 30.000.000
    // RUT de empresas: 50.000.000 a 99.999.999
    return (rutNumerico >= 1000000 && rutNumerico <= 30000000) ||
           (rutNumerico >= 50000000 && rutNumerico <= 99999999);
}

int validarRut(const char* rutCompleto) {
    if (!rutCompleto) return 0;

    char limpio[20] = {0};
    int j = 0;
    for (int i = 0; rutCompleto[i]; i++) {
        if (rutCompleto[i] != '.' && rutCompleto[i] != ' ')
            limpio[j++] = rutCompleto[i];
    }

    long rut;
    char dvIngresado;
    if (sscanf(limpio, "%ld-%c", &rut, &dvIngresado) != 2)
        return 0;

    if (rut <= 0) return 0;

    //Validación de rango realista
    if (!esRutEnRangoValido(rut)) {
        return 0; // RUT fuera de rango real
    }

    dvIngresado = toupper((unsigned char)dvIngresado);
    int dvCalculado = calcularDV((int)rut); // tu función recursiva
    char dvCorrecto = (dvCalculado == -1) ? 'K' : (char)('0' + dvCalculado);

    return dvIngresado == dvCorrecto;
}

int validarFecha(const char *fecha) {
    if (!fecha || strlen(fecha) != 10) // Formato dd/mm/aaaa
        return 0;

    int dia, mes, anio;

    // Escanea la fecha, debe tener formato exacto con /
    if (sscanf(fecha, "%2d/%2d/%4d", &dia, &mes, &anio) != 3)
        return 0;

    if (anio < 0 || anio > 2025) // Rango de año
        return 0;

    if (mes < 1 || mes > 12) // Rango de mes
        return 0;

    // Determinar la cantidad máxima de días en el mes
    int diasMax;
    switch (mes) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            diasMax = 31; break;
        case 4: case 6: case 9: case 11:
            diasMax = 30; break;
        case 2:
            // Año bisiesto
            if ((anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0))
                diasMax = 29;
            else
                diasMax = 28;
            break;
        default:
            return 0;
    }

    if (dia < 1 || dia > diasMax)
        return 0;

    return 1; // Fecha válida
}

int validarEmail(const char *email) {
    if (!email || strlen(email) == 0){
        return 0;
    }
    // No permitir espacios
    for (int i = 0; email[i]; i++) {
        if (isspace(email[i])){
            return 0;
        }
    }
    // Debe haber exactamente un '@'
    const char *arroba = strchr(email, '@');
    if (!arroba){
        return 0;
    }
    if (strchr(arroba + 1, '@')){// más de un '@'
        return 0;
    } 
    // Debe haber al menos un '.' después del '@' y al menos 2 caracteres al final
    const char *punto = strchr(arroba, '.');
    if (!punto || strlen(punto) < 3){
        return 0;
    }
    return 1; // email válido
}

// Valida formato MM-PP: dos dígitos, guion, uno o más dígitos
int validarRol(const char* rol) {
    if (!rol || strlen(rol) < 4 || strlen(rol) > 10) return 0;
    if (rol[2] != '-') return 0;
    
    // Verificar MM: dos dígitos
    if (!isdigit(rol[0]) || !isdigit(rol[1])) return 0;
    
    // Verificar PP: al menos un dígito, todos deben ser dígitos
    int i = 3;
    while (rol[i] != '\0') {
        if (!isdigit(rol[i])) return 0;
        i++;
    }
    return (i > 3); // al menos un dígito después del guion
}
