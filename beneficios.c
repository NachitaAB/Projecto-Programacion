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
        return;
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

//Lista todos los beneficios asociados a un RUT dado
void listarBeneficiosPorRut(const char* rut) {
    if (!rut) {
        printf("RUT inválido.\n");
        return;
    }

    int encontrado = 0;
    printf("\n--- BENEFICIOS DEL RUT: %s ---\n", rut);

    for (int i = 0; i < totalBeneficios; i++) {
        if (strcmp(beneficios[i].rutAsociado, rut) == 0) {
            encontrado = 1;
            printf("\n[ID: %s]\n", beneficios[i].id);
            printf("Caja de compensación: %s\n", beneficios[i].cajaCompensacion);
            printf("Detalle: %s\n", beneficios[i].detalle);
            
            if (beneficios[i].porcentajeDescuento >= 0) {
                printf("Porcentaje de descuento: %.0f%%\n", beneficios[i].porcentajeDescuento);
            } else {
                printf("Porcentaje de descuento: No aplica\n");
            }
            
            printf("Condiciones: %s\n", beneficios[i].condiciones);
            printf("Carga familiar máxima: %d\n", beneficios[i].cargaFamiliarMax);
            printf("Fecha de vigencia: %s\n", beneficios[i].fechaVigencia);
            printf("------------------------\n");
        }
    }

    if (!encontrado) {
        printf("No hay beneficios registrados para este cliente.\n");
    }
}

//Edita los Beneficios
void editarBeneficios(void){
    if (totalBeneficios == 0) {
        printf("No hay beneficios registrados para editar.\n");
        return;
    }

    char idIngresado[MAX_ID];
    printf("Ingrese el ID del beneficio a editar (ej: BEN001): ");
    char buffer[100];
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(idIngresado, buffer, sizeof(idIngresado) - 1);
    idIngresado[sizeof(idIngresado) - 1] = '\0';

    // Buscar el beneficio por ID
    int idx = -1;
    for (int i = 0; i < totalBeneficios; i++) {
        if (strcmp(beneficios[i].id, idIngresado) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("No se encontró un beneficio con ese ID.\n");
        return;
    }

    Beneficio* b = &beneficios[idx];
    printf("\nBeneficio encontrado:\n");
    printf("ID: %s | RUT asociado: %s\n", b->id, b->rutAsociado);
    printf("Caja: %s | Descuento: %.0f%%\n", b->cajaCompensacion, b->porcentajeDescuento);
    printf("Detalle: %s\n", b->detalle);

    //Editar Caja
    printf("\n¿Desea cambiar la caja de compensación? (s/n): ");
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] == 's' || buffer[0] == 'S') {
        seleccionarCaja(b->cajaCompensacion, sizeof(b->cajaCompensacion));
    }

    //Editar Detalle
    printf("Nuevo detalle (actual: %s): ", b->detalle);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        strncpy(b->detalle, buffer, sizeof(b->detalle) - 1);
        b->detalle[sizeof(b->detalle) - 1] = '\0';
    }

    //Editar Porcentaje de descuento
    printf("¿Aplica descuento? Actual: %.0f%% (0 si no aplica). Nuevo (0-100 o -1 para 'no aplica'): ", b->porcentajeDescuento);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        double nuevoPct;
        if (leerDouble(buffer, &nuevoPct)) {
            if (nuevoPct >= -1 && nuevoPct <= 100) {
                if (nuevoPct == -1) {
                    b->porcentajeDescuento = -1; // no aplica
                } else if (nuevoPct >= 0) {
                    b->porcentajeDescuento = nuevoPct;
                }
            } else {
                printf("Valor fuera de rango. No se actualizó.\n");
            }
        } else {
            printf("Entrada inválida. No se actualizó.\n");
        }
    }

    //Editar Condiciones
    printf("Nuevas condiciones (actual: %s): ", b->condiciones);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        strncpy(b->condiciones, buffer, sizeof(b->condiciones) - 1);
        b->condiciones[sizeof(b->condiciones) - 1] = '\0';
    }

    //Editar Carga familiar máxima
    printf("Nueva carga familiar máxima (actual: %d): ", b->cargaFamiliarMax);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        int carga;
        if (leerEntero(buffer, &carga) && carga >= 1 && carga <= 14) {
            b->cargaFamiliarMax = carga;
        } else {
            printf("Carga inválida (1-14). No se actualizó.\n");
        }
    }

    //Editar Fecha de vigencia
    printf("Nueva fecha de vigencia (actual: %s, dd/mm/aaaa): ", b->fechaVigencia);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        if (validarFecha(buffer)) {
            strncpy(b->fechaVigencia, buffer, sizeof(b->fechaVigencia) - 1);
            b->fechaVigencia[sizeof(b->fechaVigencia) - 1] = '\0';
        } else {
            printf("Fecha inválida. No se actualizó.\n");
        }
    }

    printf("Beneficio actualizado correctamente.\n");
}
void eliminarBeneficios(void){

    if (totalBeneficios == 0) {
        printf("No hay beneficios registrados para eliminar.\n");
        return;
    }

    char idIngresado[MAX_ID];
    printf("Ingrese el ID del beneficio a eliminar (ej: BEN001): ");
    char buffer[100];
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(idIngresado, buffer, sizeof(idIngresado) - 1);
    idIngresado[sizeof(idIngresado) - 1] = '\0';

    // Buscar el beneficio por ID
    int idx = -1;
    for (int i = 0; i < totalBeneficios; i++) {
        if (strcmp(beneficios[i].id, idIngresado) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("No se encontró un beneficio con ese ID.\n");
        return;
    }

    // Mostrar datos del beneficio a eliminar
    printf("\n¿Eliminar este beneficio?\n");
    printf("ID: %s\n", beneficios[idx].id);
    printf("Caja: %s\n", beneficios[idx].cajaCompensacion);
    printf("Detalle: %s\n", beneficios[idx].detalle);
    printf("RUT asociado: %s\n", beneficios[idx].rutAsociado);
    printf("¿Está seguro? (s/n): ");

    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] == 's' || buffer[0] == 'S') {
        // Eliminar desplazando elementos
        for (int i = idx; i < totalBeneficios - 1; i++) {
            beneficios[i] = beneficios[i + 1];
        }
        totalBeneficios--;
        printf("Beneficio con ID %s eliminado correctamente.\n", idIngresado);
    } else {
        printf("Eliminación cancelada.\n");
    }
}
//Menú interactivo del módulo beneficio
int menuBeneficios(void) {
    int opcion;
    do {
        char buffer[20];
        printf("\n--MÓDULO BENEFICIO--\n");
        printf("1. Agregar Beneficio\n");
        printf("2. Editar Beneficio\n");
        printf("3. Eliminar Beneficio\n");
        printf("4. Listar Beneficio por RUT\n");
        printf("5. Volver al menú principal\n");
        printf("\n");
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
