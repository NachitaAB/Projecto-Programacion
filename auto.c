#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "autos.h"
#include "validaciones.h" 

// Definiciones de listas (pueden ir aquí o en otro archivo)
const char* MARCAS_VALIDAS[NUM_AUTOS] = {
    "Toyota", "Hyundai", "Kia", "Chevrolet", "Nissan",
    "Suzuki", "Peugeot", "Ford", "Mazda", "Honda"
};

const char* MODELOS_VALIDOS[NUM_AUTOS][NUM_MODELOS] = {
    {"Yaris", "Corolla", "RAV4", "Hilux", "Prado"},
    {"Accent", "Tucson", "Elantra", "Santa Fe", "Creta"},
    {"Rio", "Sportage", "Sorento", "Picanto", "Cerato"},
    {"Sail", "Spark", "Tracker", "Onix", "Captiva"},
    {"Versa", "March", "Kicks", "Sentra", "X-Trail"},
    {"Swift", "Baleno", "Vitara", "S-Cross", "Jimny"},
    {"208", "2008", "3008", "Partner", "Expert"},
    {"Fiesta", "Focus", "EcoSport", "Ranger", "Territory"},
    {"Mazda2", "Mazda3", "CX-3", "CX-5", "BT-50"},
    {"Civic", "HR-V", "CR-V", "Fit", "Accord"}
};

const char* COLORES_VALIDOS[NUM_COLORES] = {
    "Blanco", "Negro", "Gris", "Rojo", "Azul"
};

const int ANIOS_VALIDOS[NUM_ANIOS] = {2021, 2022, 2023, 2024, 2025};

void seleccionarAuto(char marca[], char modelo[], char color[], int* anio, int tam) {
    char buffer[20];
    int opcion;

    //Seleccionar marca
    printf("\n--- Marcas disponibles ---\n");
    for (int i = 0; i < NUM_AUTOS; i++) {
        printf("%2d. %s\n", i + 1, MARCAS_VALIDAS[i]);
    }
    while (1) {
        printf("Seleccione una marca (1-%d): ", NUM_AUTOS);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (leerEntero(buffer, &opcion) && opcion >= 1 && opcion <= NUM_AUTOS) {
            strncpy(marca, MARCAS_VALIDAS[opcion - 1], tam - 1);
            marca[tam - 1] = '\0';
            break;
        } else {
            printf("Opción inválida. Intente nuevamente.\n");
        }
    }

    //Seleccionar modelo
    printf("\n--- Modelos de %s ---\n", marca);
    for (int j = 0; j < NUM_MODELOS; j++) {
        printf("%2d. %s\n", j + 1, MODELOS_VALIDOS[opcion - 1][j]);
    }
    while (1) {
        printf("Seleccione un modelo (1-%d): ", NUM_MODELOS);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        int op_modelo;
        if (leerEntero(buffer, &op_modelo) && op_modelo >= 1 && op_modelo <= NUM_MODELOS) {
            strncpy(modelo, MODELOS_VALIDOS[opcion - 1][op_modelo - 1], tam - 1);
            modelo[tam - 1] = '\0';
            break;
        } else {
            printf("Opción inválida. Intente nuevamente.\n");
        }
    }

    //Seleccionar color
    printf("\n--- Colores disponibles ---\n");
    for (int k = 0; k < NUM_COLORES; k++) {
        printf("%2d. %s\n", k + 1, COLORES_VALIDOS[k]);
    }
    while (1) {
        printf("Seleccione un color (1-%d): ", NUM_COLORES);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        int op_color;
        if (leerEntero(buffer, &op_color) && op_color >= 1 && op_color <= NUM_COLORES) {
            strncpy(color, COLORES_VALIDOS[op_color - 1], tam - 1);
            color[tam - 1] = '\0';
            break;
        } else {
            printf("Opción inválida. Intente nuevamente.\n");
        }
    }

    //Seleccionar año
    printf("\n--- Años disponibles ---\n");
    for (int a = 0; a < NUM_ANIOS; a++) {
        printf("%2d. %d\n", a + 1, ANIOS_VALIDOS[a]);
    }
    while (1) {
        printf("Seleccione un año (1-%d): ", NUM_ANIOS);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        int op_anio;
        if (leerEntero(buffer, &op_anio) && op_anio >= 1 && op_anio <= NUM_ANIOS) {
            *anio = ANIOS_VALIDOS[op_anio - 1];
            break;
        } else {
            printf("Opción inválida. Intente nuevamente.\n");
        }
    }

    printf("Selección completada: %s %s %s %d\n", marca, modelo, color, *anio);
}
