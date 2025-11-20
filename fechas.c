#include <stdio.h>
#include <string.h>
#include "fechas.h"
#include "validaciones.h" // para limpiar_buffer()

int esBisiesto(int anio) {
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}

int diasEnMes(int mes, int anio) {
    if (mes < 1 || mes > 12) return 0;
    const int dias[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int d = dias[mes - 1];
    if (mes == 2 && esBisiesto(anio)) d = 29;
    return d;
}

void seleccionarFecha(char fechaSalida[11]) {
    int anio, mes, dia;
    char buffer[20];

    // Años permitidos (2025–2026 según tu idea)
    const int ANIOS[] = {2025, 2026};
    const int N_ANIOS = 2;

    printf("\n--- Seleccione AÑO ---\n");
    for (int i = 0; i < N_ANIOS; i++) {
        printf("%d. %d\n", i + 1, ANIOS[i]);
    }
    while (1) {
        printf("Opción (1-%d): ", N_ANIOS);
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &anio) == 1 && anio >= 1 && anio <= N_ANIOS) {
            anio = ANIOS[anio - 1];
            break;
        }
        printf("Opción inválida.\n");
    }

    const char* MESES[12] = {
        "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
        "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"
    };

    printf("\n--- Seleccione MES ---\n");
    for (int i = 0; i < 12; i++) {
        printf("%2d. %s\n", i + 1, MESES[i]);
    }
    while (1) {
        printf("Opción (1-12): ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &mes) == 1 && mes >= 1 && mes <= 12) {
            break;
        }
        printf("Opción inválida.\n");
    }

    int maxDias = diasEnMes(mes, anio);
    printf("\n--- Seleccione DÍA (1-%d) ---\n", maxDias);
    while (1) {
        printf("Día: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &dia) == 1 && dia >= 1 && dia <= maxDias) {
            break;
        }
        printf("Día inválido (máx: %d).\n", maxDias);
    }

    // Formato dd/mm/aaaa con ceros a la izquierda
    snprintf(fechaSalida, 11, "%02d/%02d/%04d", dia, mes, anio);
}

void seleccionarHora(char horaSalida[6]) {
    int hora, minuto;
    char buffer[20];

    // Rango típico: 08:00 a 20:00
    printf("\n--- Seleccione HORA (8-20) ---\n");
    while (1) {
        printf("Hora (8-20): ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &hora) == 1 && hora >= 8 && hora <= 20) {
            break;
        }
        printf("Hora inválida (8-20).\n");
    }

    printf("Minutos (00, 15, 30, 45): ");
    const int MINUTOS[] = {0, 15, 30, 45};
    for (int i = 0; i < 4; i++) {
        printf("%d. %02d\n", i + 1, MINUTOS[i]);
    }
    while (1) {
        printf("Opción (1-4): ");
        fgets(buffer, sizeof(buffer), stdin);
        int op;
        if (sscanf(buffer, "%d", &op) == 1 && op >= 1 && op <= 4) {
            minuto = MINUTOS[op - 1];
            break;
        }
        printf("Opción inválida.\n");
    }

    snprintf(horaSalida, 6, "%02d:%02d", hora, minuto);
}