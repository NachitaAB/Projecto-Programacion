#include <stdio.h> //Entrada/Salida estándar
#include <string.h> //Manipulación de cadena (strcpy, strcmp, etc....)
#include <stdlib.h> //strtol() : convierte una cadena en un número entero largo
#include <ctype.h> //Funciones para caracteres (toupper, etc.....)
#include "beneficios.h"
#include "miembros.h"         
#include "validaciones.h"
#include "constante.h"
#include "caja.h"

// Depinición de variables globales
Beneficio beneficios[MAX_REGISTROS];
int totalBeneficios = 0; //Contador de registros reales 

// Elimina en cascada todo los Beneficios asociadas a un RUT
void eliminarBeneficiosPorRut(const char* rut) {
    if (!rut) return;
    for (int i = 0; i < totalBeneficios; ) {
        if (strcmp(beneficios[i].rutAsociado, rut) == 0) {
            // Desplazar todos los elementos siguientes una posición atrás
            for (int j = i; j < totalBeneficios - 1; j++) {
                beneficios[j] = beneficios[j + 1];
            }
            totalBeneficios--;
            // No incrementar 'i' aquí, porque el nuevo elemento en [i] debe ser revisado
        } else {
            i++;
        }
    }
}

//Crea un nuevo registro de beneficios vinculado a un rut existente
void agregarBeneficios(){

    if (totalBeneficios >= MAX_REGISTROS){
        printf("Límite de %d beneficiaros alcanzados.\n", MAX_REGISTROS);
        return 0;
    }

    //Validar RUT del cliente
    char rut[13];
    char buffer[50];
    while (1) {
        printf("RUT del cliente (ej: 12.345.678-9) o '0' para cancelar: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, "0") == 0) {
            printf("Operación cancelada.\n");
            return;
        }
        if (!validarRut(buffer)) {
            printf("RUT inválido.\n");
            continue;
        }
        if (!existeMiembro(buffer)) {
            printf("No existe un miembro con ese RUT.\n");
            continue;
        }
        strcpy(rut, buffer);
        break;
    }

    Beneficio b;
    strcpy(b.rutAsociado, rut);
    
    //ID
    printf("ID del beneficio (ej: BEN001): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) == 0) {
        snprintf(b.id, sizeof(b.id), "BEN%03d", totalBeneficios + 1);
    } else {
        strncpy(b.id, buffer, sizeof(b.id) - 1);
        b.id[sizeof(b.id) - 1] = '\0';
    }

    //Caja de compensación 
    seleccionarCaja(b.cajaCompensacion, sizeof(b.cajaCompensacion));

    //Detalle
    printf("Detalle del beneficio (ej: 15%% en salud dental): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) == 0) {
        strcpy(b.detalle, "Sin detalle");
    } else {
        strncpy(b.detalle, buffer, sizeof(b.detalle) - 1);
        b.detalle[sizeof(b.detalle) - 1] = '\0';
    }

    //Porcentaje de descuento
    b.porcentajeDescuento = -1; // -1 = no aplica
    printf("¿Aplica porcentaje de descuento? (s/n): ");
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] == 's' || buffer[0] == 'S') {
        while (1) {
            printf("Porcentaje de descuento (0–100): ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = '\0';
            int pct;
            if (leerEntero(buffer, &pct) && pct >= 0 && pct <= 100) {
                b.porcentajeDescuento = pct;
                break;
            } else {
                printf("Ingrese un valor entre 0 y 100.\n");
            }
        }
    }

    //Condiciones
    printf("Condiciones (ej: Vigente solo en clínicas asociadas): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) == 0) {
        strcpy(b.condiciones, "N/A");
    } else {
        strncpy(b.condiciones, buffer, sizeof(b.condiciones) - 1);
        b.condiciones[sizeof(b.condiciones) - 1] = '\0';
    }

    //Carga familiar máxima
    while (1) {
        printf("Carga familiar máxima (1–14): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        int carga;
        if (leerEntero(buffer, &carga) && carga >= 1 && carga <= 14) {
            b.cargaFamiliarMax = carga;
            break;
        } else {
            printf("Ingrese un número entre 1 y 14.\n");
        }
    }

    //Fecha de vigencia
    printf("Fecha de vigencia (dd/mm/aaaa, ej: 01/12/2024): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) == 0 || !validarFecha(buffer)) {
        strcpy(b.fechaVigencia, "Vigente indefinidamente");
    } else {
        strncpy(b.fechaVigencia, buffer, sizeof(b.fechaVigencia) - 1);
        b.fechaVigencia[sizeof(b.fechaVigencia) - 1] = '\0';
    }

    //Guardar
    beneficios[totalBeneficios] = b;
    totalBeneficios++;

    printf("\nBeneficio registrado para RUT %s\n", rut);
    printf("Caja: %s | ID: %s\n", b.cajaCompensacion, b.id);
}

//Menú interactivo del módulo beneficio
int menuBeneficios(void) {
    int opcion;
    do {
        char buffer[20];
        printf("\n-------- MÓDULO BENEFICIO --------\n");
        printf("1. Agregar Agenda\n");
        printf("2. Editar Agenda\n");
        printf("3. Eliminar Agenda\n");
        printf("4. Listar Agenda por RUT\n");
        printf("5. Volver al menú principal\n");
        printf("------------------------------------\n");
        printf("Opción: ");
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error de entrada. Intente de nuevo.\n");
            opcion = -1;
            continue;
        }
        // Eliminar el salto de línea
        buffer[strcspn(buffer, "\n")] = '\0';
        // Intentar convertir a entero con validación robusta
        char *end;
        long val = strtol(buffer, &end, 10);
        // Saltar espacios al final (si los hay)
        while (*end && isspace((unsigned char)*end)) {
            end++;
        }
        // Validar que toda la cadena fue consumida y está en rango
        if (*end == '\0' && val >= 1 && val <= 5) {
            opcion = (int)val;
        } else {
            opcion = -1;
        }
        if (opcion == -1) {
            printf("Opción inválida. Ingrese un número del 1 al 5.\n");
            continue;
        }

        switch (opcion) {
            case 1:    
                agregarBeneficios(); 
                break;
            case 2: 
                editarBeneficios(); 
                break;
            case 3: 
                eliminarBeneficios(); 
                break;
            case 4: {
                char rut[13];
                while (1) {
                    printf("\nIngrese RUT del miembro (ej: 12.345.678-9) o '0' para volver al menú: ");
                    if (fgets(rut, sizeof(rut), stdin) == NULL) continue;
                    rut[strcspn(rut, "\n")] = '\0';

                    if (strcmp(rut, "0") == 0) {
                        printf("Volviendo al menú de Agenda...\n");
                        break;
                    }
                    if (!validarRut(rut)) {
                        printf("RUT inválido. Intente nuevamente.\n");
                        continue;
                    }
                    if (!existeMiembro(rut)) {
                        printf("No existe un miembro con ese RUT. Intente nuevamente.\n");
                        continue;
                    }
                    //RUT válido y existente → listar todo lo asociado en Beneficios
                    listarBeneficiosPorRut(rut);
                    break;
                }
            }
            case 5: 
                //Vuelve al menú principal 
                break;
            default: 
                printf("Opción no válida.\n");
        }
    } while (opcion != 5);
}
