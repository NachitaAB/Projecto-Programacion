#include <stdio.h> //Entrada/Salida estándar
#include <string.h> //Manipulación de cadena (strcpy, strcmp, etc....)
#include <stdlib.h> //strtol() : convierte una cadena en un número entero largo
#include <ctype.h> //punciones para caracteres (touffer, etc.....)
#include "propiedades.h"
#include "miembros.h"        
#include "validaciones.h"    // para validar RUT y pecha

// Depinición de variables globales
Propiedad propiedades[MAX_REGISTROS];
int totalPropiedades = 0; //Contador de registros reales 

// Elimina en cascada todas las propiedades asociadas a un RUT
void eliminarPropiedadesPorRut(const char* rut) {

    if (!rut) return;
    for (int i = 0; i < totalPropiedades; ) {
        if (strcmp(propiedades[i].rutAsociado, rut) == 0) {
            // Desplazar todos los elementos siguientes una posición atrás
            for (int j = i; j < totalPropiedades - 1; j++) {
                propiedades[j] = propiedades[j + 1];
            }
            totalPropiedades--;
            // No incrementar 'i' aquí, porque el nuevo elemento en [i] debe ser revisado
        } else {
            i++;
        }
    }
}
//Crea un nuevo registro de propiedades vinculado a un rut existente 
void agregarPropiedades(void) {
    if (totalPropiedades >= MAX_REGISTROS) {
        printf("Límite de %d propiedades alcanzado.\n", MAX_REGISTROS);
        return;
    }

    char rut[13];
    while (1) {
        printf("RUT del cliente (ej: 12.345.678-9) o '0' para cancelar: ");
        char buffer[30];
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
            printf("Cliente no registrado.\n");
            continue;
        }
        strcpy(rut, buffer);
        break;
    }

    Propiedad p;
    strcpy(p.rutAsociado, rut);

    // Rol (MM-ff)
    char buffer[30];
    while (1) {
        printf("Rol (formato MM-ff, ej: 12-345): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (validarRol(buffer)) {
            strcpy(p.rol, buffer);
            break;
        } else {
            printf("Formato de Rol inválido. Use MM-ff (ej: 05-123).\n");
        }
    }

    // M² construidos
    while (1) {
        printf("M² construidos (≥1): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        int m2;
        if (leerEntero(buffer, &m2) && m2 >= 1) {
            p.m2Construidos = m2;
            break;
        } else {
            printf("Ingrese un número entero ≥ 1.\n");
        }
    }

    // Año (1900–2025)
    while (1) {
        printf("Año de construcción (1900–2025): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        int anio;
        if (leerEntero(buffer, &anio) && anio >= 1900 && anio <= 2025) {
            p.ano = anio;
            break;
        } else {
            printf("Año fuera de rango.\n");
        }
    }

    // Torre (A-H)
    while (1) {
        printf("Torre (A a H): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) == 1) {
            char t = touffer(buffer[0]);
            if (t >= 'A' && t <= 'H') {
                p.torre = t;
                break;
            }
        }
        printf("Ingrese una letra de la A a la H.\n");
    }

    // Departamento
    printf("Departamento (ej: 302, B5): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) == 0) {
        strcpy(p.departamento, "N/A");
    } else {
        strncpy(p.departamento, buffer, sizeof(p.departamento) - 1);
        p.departamento[sizeof(p.departamento) - 1] = '\0';
    }

    // Piezas
    while (1) {
        printf("Número de piezas (1–10): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        int piezas;
        if (leerEntero(buffer, &piezas) && piezas >= 1 && piezas <= 10) {
            p.numPiezas = piezas;
            break;
        } else {
            printf("Entre 1 y 10 piezas.\n");
        }
    }

    // Baños
    while (1) {
        printf("Número de baños (1–5): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        int banos;
        if (leerEntero(buffer, &banos) && banos >= 1 && banos <= 5) {
            p.numBanos = banos;
            break;
        } else {
            printf("Entre 1 y 5 baños.\n");
        }
    }

    propiedades[totalPropiedades++] = p;
    printf("Propiedad registrada para RUT %s.\n", rut);
}
//Muentra todas las propiedades de un miembro por su rut 
void listarPropiedadesPorRut(const char* rut) {
    int encontrado = 0;
    printf("\n--- PROPIEDADES DEL RUT: %s ---\n", rut);
    for (int i = 0; i < totalPropiedades; i++) {
        if (strcmp(propiedades[i].rutAsociado, rut) == 0) {
            encontrado = 1;
            printf("\nRol: %s\n", propiedades[i].rol);
            printf("M²: %d | Año: %d | Torre: %c | Depto: %s\n",
                   propiedades[i].m2Construidos,
                   propiedades[i].ano,
                   propiedades[i].torre,
                   propiedades[i].departamento);
            printf("Piezas: %d | Baños: %d\n", 
                   propiedades[i].numPiezas, 
                   propiedades[i].numBanos);
            printf("------------------------\n");
        }
    }
    if (!encontrado) {
        printf("No hay propiedades registradas.\n");
    }
}
//Edita las propiedades 
void editarPropiedades(void) {
    if (totalPropiedades == 0) {
        printf("No hay propiedades para editar.\n");
        return;
    }

    char rol[12];
    printf("Ingrese el Rol de la propiedad a editar (ej: 12-345): ");
    char buffer[30];
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(rol, buffer, sizeof(rol) - 1);
    rol[sizeof(rol) - 1] = '\0';

    int idx = -1;
    for (int i = 0; i < totalPropiedades; i++) {
        if (strcmp(propiedades[i].rol, rol) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("Propiedad con Rol %s no encontrada.\n", rol);
        return;
    }

    Propiedad* p = &propiedades[idx];

    // Editar cada campo (solo si el usuario ingresa algo)
    printf("Nuevo Rol (actual: %s): ", p->rol);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0 && validarRol(buffer)) {
        strcpy(p->rol, buffer);
    }

    printf("Nuevos M² (actual: %d): ", p->m2Construidos);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        int val;
        if (leerEntero(buffer, &val) && val >= 1) p->m2Construidos = val;
    }

    printf("Nuevo año (actual: %d): ", p->ano);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        int val;
        if (leerEntero(buffer, &val) && val >= 1900 && val <= 2025) p->ano = val;
    }

    printf("Nueva torre (actual: %c): ", p->torre);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) == 1) {
        char t = toupper(buffer[0]);
        if (t >= 'A' && t <= 'H') p->torre = t;
    }

    printf("Nuevo departamento (actual: %s): ", p->departamento);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        strncpy(p->departamento, buffer, sizeof(p->departamento) - 1);
        p->departamento[sizeof(p->departamento) - 1] = '\0';
    }

    printf("Nuevas piezas (actual: %d): ", p->numPiezas);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        int val;
        if (leerEntero(buffer, &val) && val >= 1 && val <= 10) p->numPiezas = val;
    }

    printf("Nuevos baños (actual: %d): ", p->numBanos);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        int val;
        if (leerEntero(buffer, &val) && val >= 1 && val <= 5) p->numBanos = val;
    }

    printf("Propiedad actualizada.\n");
}

//Elimina un registro de propiedades por rol 
void eliminarPropiedades(void) {
    if (totalPropiedades == 0) {
        printf("No hay propiedades para eliminar.\n");
        return;
    }

    char rol[12];
    printf("Rol de la propiedad a eliminar (ej: 12-345): ");
    char buffer[30];
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(rol, buffer, sizeof(rol) - 1);
    rol[sizeof(rol) - 1] = '\0';

    int idx = -1;
    for (int i = 0; i < totalPropiedades; i++) {
        if (strcmp(propiedades[i].rol, rol) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("No se encontró una propiedad con ese Rol.\n");
        return;
    }

    printf("¿Eliminar propiedad Rol %s del RUT %s? (s/n): ", 
           propiedades[idx].rol, propiedades[idx].rutAsociado);
    fgets(buffer, sizeof(buffer), stdin);

    if (buffer[0] == 's' || buffer[0] == 'S') {
        for (int i = idx; i < totalPropiedades - 1; i++) {
            propiedades[i] = propiedades[i + 1];
        }
        totalPropiedades--;
        printf("Propiedad eliminada.\n");
    } else {
        printf("Cancelado.\n");
    }
}

//Menú interactivo del módulo propiedades 
int menuPropiedades(void) {
    int opcion;
    do {
        char buffer[20];
        printf("\n-------- MÓDULO PROPIEDADES --------\n");
        printf("1. Agregar Propiedades\n");
        printf("2. Editar Propiedades\n");
        printf("3. Eliminar Propiedades\n");
        printf("4. Listar Propiedades por RUT\n");
        printf("5. Volver al menú principal\n");
        printf("-----------------------------------\n");
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
                agregarPropiedades(); 
                break;
            case 2: 
                editarPropiedades(); 
                break;
            case 3: 
                eliminarPropiedades(); 
                break;
            case 4: {
                char rut[13];
                while (1) {
                    printf("\nIngrese RUT del miembro (ej: 12.345.678-9) o '0' para volver al menú: ");
                    if (fgets(rut, sizeof(rut), stdin) == NULL) continue;
                    rut[strcspn(rut, "\n")] = '\0';

                    if (strcmp(rut, "0") == 0) {
                        printf("Volviendo al menú de propiedad...\n");
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

                    //RUT válido y existente → listar TODO lo asociado en propiedad
                    listarPropiedadesPorRut(rut);
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
