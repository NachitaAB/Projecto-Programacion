#include <stdio.h>
#include <string.h>
#include "beneficios.h"
#include "validaciones.h"
#include "caja.h"

// Lista de cajas de compensación válidas
const char* CAJAS_VALIDAS[] = {
    "Caja Los Andes",
    "Caja de Compensación Los Héroes",
    "Mutual de Seguridad",
    "Cruz Blanca",
    "Caja de Compensación La Araucana"
};

int esCajaValida(const char* caja) {
    if (!caja) return 0;
    for (int i = 0; i < NUM_CAJA; i++) {
        if (strcmp(caja, CAJAS_VALIDAS[i]) == 0)
            return 1;
    }
    return 0;
}

void seleccionarCaja(char destino[], int tam) {
    printf("\n--- Cajas de Compensación ---\n");
    for (int i = 0; i < NUM_CAJA; i++) {
        printf("%2d. %s\n", i + 1, CAJAS_VALIDAS[i]);
    }

    char buffer[20];
    int opcion;
    while (1) {
        printf("Seleccione una caja (1-%d): ", NUM_CAJA);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) continue;
        buffer[strcspn(buffer, "\n")] = '\0';

        if (leerEntero(buffer, &opcion) && opcion >= 1 && opcion <= NUM_CAJA) {
            strncpy(destino, CAJAS_VALIDOS[opcion - 1], tam - 1);
            destino[tam - 1] = '\0';
            return;
        } else {
            printf("Opción inválida. Intente nuevamente.\n");
        }
    }
}
