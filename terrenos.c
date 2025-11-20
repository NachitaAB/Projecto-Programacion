#include <stdio.h> //Entrada/Salida estándar
#include <string.h> //Manipulación de cadena (strcpy, strcmp, etc....)
#include <stdlib.h> //strtol() : convierte una cadena en un número entero largo
#include <ctype.h> //Funciones para caracteres (toupper, etc.....)
#include "terrenos.h"
#include "miembros.h"         // para verificar que el RUT exista
#include "validaciones.h"
#include "constante.h"

// Depinición de variables globales
Terreno terrenos[MAX_REGISTROS];
int totalTerrenos = 0; //Contador de registros reales 

// Elimina en cascada todo los Terrenos asociadas a un RUT
void eliminarTerrenosPorRut(const char* rut) {
    if (!rut) return;
    for (int i = 0; i < totalTerrenos; ) {
        if (strcmp(terrenos[i].rutAsociado, rut) == 0) {
            // Desplazar todos los elementos siguientes una posición atrás
            for (int j = i; j < totalTerrenos - 1; j++) {
                terrenos[j] = terrenos[j + 1];
            }
            totalTerrenos--;
            // No incrementar 'i' aquí, porque el nuevo elemento en [i] debe ser revisado
        } else {
            i++;
        }
    }
}
void agregarTerrenos(void){

    if (totalTerrenos >= MAX_REGISTROS){
        printf("Límite de %d terrenos alcanzados.\n", MAX_REGISTROS);
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
    Terreno t;
    strcpy(t.rutAsociado, rut);

    //Rol terreno
    printf("Rol del terreno (ej: 9876-5432): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) == 0) {
        strcpy(t.rolTerreno, "Sin Rol");
    } else {
        strncpy(t.rolTerreno, buffer, sizeof(t.rolTerreno) - 1);
        t.rolTerreno[sizeof(t.rolTerreno) - 1] = '\0';
    }

    //Metros cuadrados totales
    int m2Totales;
    while (1) {
        printf("Metros cuadrados totales (≥1): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (leerEntero(buffer, &m2Totales) && m2Totales >= 1) {
            t.metrosTotales = m2Totales;
            break;
        } else {
            printf("Ingrese un número entero ≥ 1.\n");
        }
    }

    //Metros cuadrados para construcción ---
    int m2Construccion;
    while (1) {
        printf("Metros cuadrados para construcción (≤ %d): ", m2Totales);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (leerEntero(buffer, &m2Construccion) && m2Construccion >= 0 && m2Construccion <= m2Totales) {
            t.metrosConstruccion = m2Construccion;
            break;
        } else {
            printf("Debe ser ≥ 0 y ≤ %d.\n", m2Totales);
        }
    }

    //Dirección
    printf("Dirección del terreno: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) == 0) {
        strcpy(t.direccion, "Sin dirección");
    } else {
        strncpy(t.direccion, buffer, sizeof(t.direccion) - 1);
        t.direccion[sizeof(t.direccion) - 1] = '\0';
    }

    //Tasación (valor monetario)
    double tasacion;
    while (1) {
        printf("Tasación (CLP, ≥ 0): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (leerDouble(buffer, &tasacion) && tasacion >= 0) {
            t.tasacion = tasacion;
            break;
        } else {
            printf("Ingrese un monto válido ≥ 0.\n");
        }
    }

    //¿Apto para edificio?
    printf("¿Apto para edificio? (s/n): ");
    fgets(buffer, sizeof(buffer), stdin);
    t.aptoEdificio = (buffer[0] == 's' || buffer[0] == 'S');

    //Estado (validar opciones)
    printf("Estado del terreno:\n");
    printf("1. en venta\n2. en arriendo\n3. activo fijo\n");
    while (1) {
        printf("Opción (1-3): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        int estado;
        if (leerEntero(buffer, &estado) && estado >= 1 && estado <= 3) {
            if (estado == 1) strcpy(t.estado, "en venta");
            else if (estado == 2) strcpy(t.estado, "en arriendo");
            else strcpy(t.estado, "activo fijo");
            break;
        } else {
            printf("Ingrese 1, 2 o 3.\n");
        }
    }

    //---
    terrenos[totalTerrenos] = t;
    totalTerrenos++;

    printf("\nTerreno registrado para RUT %s\n", rut);
    printf("Rol: %s | Tasación: $%.0f CLP | Estado: %s\n", 
           t.rolTerreno, t.tasacion, t.estado);
}

//Muestra todos los terrenos de un miembro por su rut
void listarTerrenosPorRut(const char* rut) {
    if (!rut) {
        printf("RUT inválido.\n");
        return;
    }

    int encontrado = 0;
    printf("\n--- TERRENOS DEL RUT: %s ---\n", rut);

    for (int i = 0; i < totalTerrenos; i++) {
        if (strcmp(terrenos[i].rutAsociado, rut) == 0) {
            encontrado = 1;
            printf("Rol: %s\n", terrenos[i].rolTerreno);
            printf("Metros cuadrados: %.2f\n", terrenos[i].metrosTotales);
            printf("Metros cuadrados construidos: %.2f\n", terrenos[i].metrosConstruccion);
            printf("Dirección: %s\n", terrenos[i].direccion);
            printf("¿Apto para edificio?: %s\n", terrenos[i].aptoEdificio ? "Sí" : "No");
            printf("Tasación: $%.0f CLP\n", terrenos[i].tasacion);
            printf("Estado: %s\n", terrenos[i].estado);
            printf("------------------------\n");
        }
    }

    if (!encontrado) {
        printf("No hay terrenos registrados para este cliente.\n");
    }
}
//Edita los Terrenos
void editarTerrenos(void) {
    if (totalTerrenos == 0) {
        printf("No hay terrenos registrados para editar.\n");
        return;
    }

    char rolIngresado[20];
    printf("Ingrese el Rol del terreno a editar (ej: 9876-5432): ");
    char buffer[100];
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(rolIngresado, buffer, sizeof(rolIngresado) - 1);
    rolIngresado[sizeof(rolIngresado) - 1] = '\0';

    // Buscar por Rol (identificador único)
    int idx = -1;
    for (int i = 0; i < totalTerrenos; i++) {
        if (strcmp(terrenos[i].rolTerreno, rolIngresado) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("No se encontró un terreno con ese Rol.\n");
        return;
    }

    Terreno* t = &terrenos[idx];
    
    printf("\nTerreno encontrado:\n");
    printf("Rol: %s | RUT: %s\n", t->rolTerreno, t->rutAsociado);
    printf("Ubicación: %s | Estado: %s\n", t->direccion, t->estado);
    printf("M²: total=%2f, construcción=%2f | Apto edificio: %s\n",
           t->metrosTotales, t->metrosConstruccion,
           t->aptoEdificio ? "Sí" : "No");

    // --- Editar cada campo (solo si el usuario ingresa algo) ---

    // Rol
    printf("\nNuevo Rol (actual: %s): ", t->rolTerreno);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        strncpy(t->rolTerreno, buffer, sizeof(t->rolTerreno) - 1);
        t->rolTerreno[sizeof(t->rolTerreno) - 1] = '\0';
    }

    // M² totales
    printf("Nuevos m² totales (actual: %2f): ", t->metrosTotales);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        int m2;
        if (leerEntero(buffer, &m2) && m2 >= 1) {
            t->metrosTotales = m2;
        } else {
            printf("M² totales inválidos. No se actualizó.\n");
        }
    }

    // M² para construcción (debe ser ≤ m² totales)
    printf("Nuevos m² para construcción (actual: %2f, ≤ %2f): ", t->metrosConstruccion, t->metrosTotales);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        int m2;
        if (leerEntero(buffer, &m2) && m2 >= 0 && m2 <= t->metrosTotales) {
            t->metrosConstruccion = m2;
        } else {
            printf("Debe ser ≥ 0 y ≤ %2f. No se actualizó.\n", t->metrosTotales);
        }
    }

    // Dirección
    printf("Nueva dirección (actual: %s): ", t->direccion);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        strncpy(t->direccion, buffer, sizeof(t->direccion) - 1);
        t->direccion[sizeof(t->direccion) - 1] = '\0';
    }

    // Tasación
    printf("Nueva tasación (actual: $%.0f): ", t->tasacion);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        double valor;
        if (leerDouble(buffer, &valor) && valor >= 0) {
            t->tasacion = valor;
        } else {
            printf("Tasación inválida. No se actualizó.\n");
        }
    }

    // ¿Apto para edificio?
    printf("¿Apto para edificio? (s/n, actual: %s): ", t->aptoEdificio ? "Sí" : "No");
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] == 's' || buffer[0] == 'S') {
        t->aptoEdificio = 1;
    } else if (buffer[0] == 'n' || buffer[0] == 'N') {
        t->aptoEdificio = 0;
    }

    // Estado (con switch explícito)
    printf("Estado actual: %s\n", t->estado);
    printf("Nuevo estado:\n1. en venta\n2. en arriendo\n3. activo fijo\nOpción (1-3, o Enter para omitir): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        int estado;
        if (leerEntero(buffer, &estado) && estado >= 1 && estado <= 3) {
            switch (estado) {
                case 1: strcpy(t->estado, "en venta"); break;
                case 2: strcpy(t->estado, "en arriendo"); break;
                case 3: strcpy(t->estado, "activo fijo"); break;
            }
        } else {
            printf("Estado inválido. No se actualizó.\n");
        }
    }

    printf("Terreno actualizado correctamente.\n");
}

//Elimina un registro de los terrenos por patente
void eliminarTerrenos(void) {
    if (totalTerrenos == 0) {
        printf("No hay terrenos registrados para eliminar.\n");
        return;
    }

    char rolIngresado[20];
    printf("Ingrese el Rol del terreno a eliminar (ej: 9876-5432): ");
    char buffer[100];
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(rolIngresado, buffer, sizeof(rolIngresado) - 1);
    rolIngresado[sizeof(rolIngresado) - 1] = '\0';

    // Buscar el terreno por Rol
    int idx = -1;
    for (int i = 0; i < totalTerrenos; i++) {
        if (strcmp(terrenos[i].rolTerreno, rolIngresado) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("No se encontró un terreno con ese Rol.\n");
        return;
    }

    // Mostrar datos del terreno a eliminar
    printf("\n¿Eliminar este terreno?\n");
    printf("ID: %d\n", terrenos[idx].id);
    printf("RUT asociado: %s\n", terrenos[idx].rutAsociado);
    printf("M² totales: %.2f | M² construcción: %.2f\n", terrenos[idx].metrosTotales, terrenos[idx].metrosConstruccion);
    printf("Tasación: $%.0f CLP\n", terrenos[idx].tasacion);
    printf("¿Está seguro? (s/n): ");

    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] == 's' || buffer[0] == 'S') {
        // Eliminar desplazando elementos
        for (int i = idx; i < totalTerrenos - 1; i++){
            terrenos[i] = terrenos[i + 1];
        }
        totalTerrenos--;
        printf("Terreno con Rol %s eliminado correctamente.\n", rolIngresado);
    } else {
        printf("Eliminación cancelada.\n");
    }
}
//Menú interactivo del módulo terrenos
int menuTerrenos(void) {
    int opcion;
    do {
        char buffer[20];
        
        printf("\n----ERRENOS --------\n");
        printf("1. Agregar Terrenos\n");
        printf("2. Editar Terrenos\n");
        printf("3. Eliminar Terrenos\n");
        printf("4. Listar Terrenos por RUT\n");
        printf("5. Volver al menú principal\n");
        printf("----------------------------------------\n");
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
                agregarTerrenos(); 
                break;
            case 2: 
                editarTerrenos(); 
                break;
            case 3: 
                eliminarTerrenos(); 
                break;
            case 4: {
                char rut[13];
                while (1) {
                    printf("\nIngrese RUT del miembro (ej: 12.345.678-9) o '0' para volver al menú: ");
                    if (fgets(rut, sizeof(rut), stdin) == NULL) continue;
                    rut[strcspn(rut, "\n")] = '\0';

                    if (strcmp(rut, "0") == 0) {
                        printf("Volviendo al menú de Terrenos...\n");
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
                    //RUT válido y existente → listar todo lo asociado en Terrenos
                    listarTerrenosPorRut(rut);
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