#include <stdio.h>
#include <string.h>
#include "bancos.h"

const char* BANCOS_VALIDOS[NUM_BANCOS] = {
    "Banco de Chile",
    "Banco Santander Chile",
    "Banco de Credito e Inversiones",
    "Banco Estado",
    "Scotiabank Chile",
    "Banco Itau Chile",
    "Banco Security",
    "Banco Falabella",
    "Banco Ripley",
    "Banco Consorcio",
    "Banco Internacional"
};

int esBancoValido(const char* banco) {
    if (!banco) return 0;
    for (int i = 0; i < NUM_BANCOS; i++) {
        if (strcmp(banco, BANCOS_VALIDOS[i]) == 0)
            return 1;
    }
    return 0;
}

void seleccionarBanco(char destino[], int tam) {
    printf("\nSeleccione un banco:\n");
    for (int i = 0; i < NUM_BANCOS; i++) {
        printf("%2d. %s\n", i + 1, BANCOS_VALIDOS[i]);
    }
    printf("Opción (1-%d): ", NUM_BANCOS);

    int op;
    scanf("%d", &op);
    getchar(); // limpiar \n

    if (op >= 1 && op <= NUM_BANCOS) {
        strncpy(destino, BANCOS_VALIDOS[op - 1], tam - 1);
        destino[tam - 1] = '\0';
    } else {
        strncpy(destino, "Banco no válido", tam - 1);
        destino[tam - 1] = '\0';
    }
}