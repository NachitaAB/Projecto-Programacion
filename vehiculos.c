#include <stdio.h> //Entrada/Salida estándar
#include <string.h> //Manipulación de cadena (strcpy, strcmp, etc....)
#include <stdlib.h> //strtol() : convierte una cadena en un número entero largo
#include <ctype.h> //Funciones para caracteres (toupper, etc.....)
#include "vehiculos.h"
#include "miembros.h"         // para veripicar que el RUT exista
#include "validaciones.h"
#include "constante.h"
#include "autos.h"

// Definición de variables globales
Vehiculo vehiculos[MAX_REGISTROS];
int totalVehiculos = 0;

// Elimina en cascada todas las vehiculos asociadas a un RUT
void eliminarVehiculosPorRut(const char* rut) {
    if (!rut) return;
    for (int i = 0; i < totalVehiculos; ) {
        if (strcmp(vehiculos[i].rutAsociado, rut) == 0) {
            // Desplazar todos los elementos siguientes una posición atrás
            for (int j = i; j < totalVehiculos - 1; j++) {
                vehiculos[j] = vehiculos[j + 1];
            }
            totalVehiculos--;
            // No incrementar 'i' aquí, porque el nuevo elemento en [i] debe ser revisado
        } else {
            i++;
        }
    }
}

//Crea un nuevo registro de propiedades vinculado a un rut existente 
void agregarVehiculos(void) {
    if (totalVehiculos >= MAX_REGISTROS) {
        printf("Límite de %d vehículos alcanzado.\n", MAX_REGISTROS);
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

    Vehiculo v;
    strcpy(v.rutAsociado, rut);

    //Tipo de vehículo
    printf("Tipo de vehículo (auto, camioneta, moto, SUV, etc.): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) == 0) {
        strcpy(v.tipo, "No especificado");
    } else {
        strncpy(v.tipo, buffer, sizeof(v.tipo) - 1);
        v.tipo[sizeof(v.tipo) - 1] = '\0';
    }

    //Cilindrada (CC)
    while (1) {
        printf("Cilindrada (CC, ej: 1600): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        int cc;
        if (leerEntero(buffer, &cc) && cc >= 50 && cc <= 10000) {
            v.cc = cc;
            break;
        } else {
            printf("Ingrese una cilindrada válida (50-10000 CC).\n");
        }
    }

    seleccionarAuto(v.marca, v.modelo, v.color, &v.anio, sizeof(v.marca));

    //Patente (validación chilena)
    while (1) {
        printf("Patente (ej: ABCD12 o AB-AB-12): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        // Normalizar: convertir a mayúsculas y eliminar guiones opcionales para validación
        char patenteTemp[15] = {0};
        int j = 0;
        for (int i = 0; buffer[i] != '\0' && j < 14; i++) {
            if (buffer[i] != '-' && buffer[i] != ' ') {
                patenteTemp[j++] = toupper((unsigned char)buffer[i]);
            }
        }
        patenteTemp[j] = '\0';

        // Validar formatos: AAAA11 (nuevo) o AABB11 (antiguo sin guiones)
        if (strlen(patenteTemp) == 6) {
            int valido = 1;
            for (int i = 0; i < 4; i++) {
                if (!isalpha(patenteTemp[i])) { valido = 0; break; }
            }
            for (int i = 4; i < 6; i++) {
                if (!isdigit(patenteTemp[i])) { valido = 0; break; }
            }
            if (valido) {
                // Guardar en formato original (con guiones si los ingresó)
                strncpy(v.patente, buffer, sizeof(v.patente) - 1);
                v.patente[sizeof(v.patente) - 1] = '\0';
                break;
            }
        }

        printf("Formato de patente inválido. Use ABCD12 o AB-AB-12.\n");
    }

    // --- Guardar el vehículo ---
    vehiculos[totalVehiculos] = v;
    totalVehiculos++;
    printf("Vehículo registrado correctamente para RUT %s.\n", rut);
}

//Muestra todos los vehiculos de un miembro por su rut 
void listarVehiculosPorRut(const char* rut){

    if (!rut) {
        printf("RUT inválido.\n");
        return;
    }
    int encontrado = 0;
    printf("\n--- VEHÍCULOS DEL RUT: %s ---\n", rut);

    for (int i = 0; i < totalVehiculos; i++) {
        if (strcmp(vehiculos[i].rutAsociado, rut) == 0) {
            encontrado = 1;
            printf("\nPatente: %s\n", vehiculos[i].patente);
            printf("Tipo: %s\n", vehiculos[i].tipo);
            printf("Marca: %s\n", vehiculos[i].marca);
            printf("Modelo: %s\n", vehiculos[i].modelo);
            printf("Año: %d\n", vehiculos[i].anio);
            printf("Color: %s\n", vehiculos[i].color);
            printf("CC: %d\n", vehiculos[i].cc);
            printf("------------------------\n");
        }
    }

    if (!encontrado) {
        printf("No hay vehículos registrados para este cliente.\n");
    }
}
//Edita los Vehiculos
void editarVehiculos(void){
        if (totalVehiculos == 0) {
        printf("No hay vehículos registrados para editar.\n");
        return;
    }

    char patenteIngresada[15];
    printf("Ingrese la patente del vehículo a editar (ej: ABCD12 o AB-AB-12): ");
    char buffer[50];
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(patenteIngresada, buffer, sizeof(patenteIngresada) - 1);
    patenteIngresada[sizeof(patenteIngresada) - 1] = '\0';

    // Buscar por patente (debe ser única)
    int idx = -1;
    for (int i = 0; i < totalVehiculos; i++) {
        if (strcmp(vehiculos[i].patente, patenteIngresada) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("No se encontró un vehículo con esa patente.\n");
        return;
    }

    Vehiculo* v = &vehiculos[idx];
    printf("\nVehículo encontrado:\n");
    printf("Patente: %s | RUT dueño: %s\n", v->patente, v->rutAsociado);
    printf("Tipo: %s | Marca: %s | Modelo: %s | Año: %d | CC: %d | Color: %s\n",
           v->tipo, v->marca, v->modelo, v->anio, v->cc, v->color);

    // --- Editar campos (solo si el usuario ingresa algo) ---

    // Tipo
    printf("\nNuevo tipo (actual: %s): ", v->tipo);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        strncpy(v->tipo, buffer, sizeof(v->tipo) - 1);
        v->tipo[sizeof(v->tipo) - 1] = '\0';
    }

    // CC
    printf("Nueva cilindrada (CC) (actual: %d): ", v->cc);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        int cc;
        if (leerEntero(buffer, &cc) && cc >= 50 && cc <= 10000) {
            v->cc = cc;
        } else {
            printf("CC inválida. No se actualizó.\n");
        }
    }

    // Marca, modelo, color, año → usar selección guiada
    printf("¿Desea cambiar marca/modelo/color/año? (s/n): ");
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] == 's' || buffer[0] == 'S') {
        seleccionarAuto(v->marca, v->modelo, v->color, &v->anio, sizeof(v->marca));
    }

    // Patente (opcional: permitir cambiarla, pero debe ser válida)
    printf("Nueva patente (actual: %s): ", v->patente);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        // Validar patente (puedes reutilizar lógica de agregarVehiculo)
        char temp[15] = {0};
        int j = 0;
        for (int i = 0; buffer[i] && j < 14; i++) {
            if (buffer[i] != '-' && buffer[i] != ' ') {
                temp[j++] = toupper((unsigned char)buffer[i]);
            }
        }
        if (strlen(temp) == 6) {
            int valido = 1;
            for (int i = 0; i < 4; i++) if (!isalpha(temp[i])) valido = 0;
            for (int i = 4; i < 6; i++) if (!isdigit(temp[i])) valido = 0;
            if (valido) {
                strncpy(v->patente, buffer, sizeof(v->patente) - 1);
                v->patente[sizeof(v->patente) - 1] = '\0';
            } else {
                printf("Patente inválida. No se actualizó.\n");
            }
        } else {
            printf("Patente inválida. No se actualizó.\n");
        }
    }

    printf("Vehículo actualizado correctamente.\n");
}

//Elimina un registro de los vehiculos por patente
void eliminarVehiculos(){
    if (totalVehiculos == 0){
        printf("No hay Vehiculos para eliminar.\n");
        return;
    }

    char patenteIngresada[15];
    printf("Ingrese la patente del vehículo a eliminar (ej: ABCD12 o AB-AB-12): ");
    char buffer[50];
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(patenteIngresada, buffer, sizeof(patenteIngresada) - 1);
    patenteIngresada[sizeof(patenteIngresada) - 1] = '\0';

    // Buscar el vehículo por patente (debe ser único)
    int idx = -1;
    for (int i = 0; i < totalVehiculos; i++) {
        if (strcmp(vehiculos[i].patente, patenteIngresada) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("No se encontró un vehículo con esa patente.\n");
        return;
    }

    // Mostrar datos del vehículo a eliminar
    printf("\n¿Eliminar este vehículo?\n");
    printf("Patente: %s\n", vehiculos[idx].patente);
    printf("Marca: %s | Modelo: %s | Año: %d\n", 
           vehiculos[idx].marca, 
           vehiculos[idx].modelo, 
           vehiculos[idx].anio);
    printf("RUT dueño: %s\n", vehiculos[idx].rutAsociado);
    printf("¿Está seguro? (s/n): ");

    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] == 's' || buffer[0] == 'S') {
        // Eliminar desplazando elementos
        for (int i = idx; i < totalVehiculos - 1; i++) {
            vehiculos[i] = vehiculos[i + 1];
        }
        totalVehiculos--;
        printf("Vehículo con patente %s eliminado correctamente.\n", patenteIngresada);
    } else {
        printf("Eliminación cancelada.\n");
    }
}

//Menú interactivo del módulo vehiculos
int menuVehiculos() {
    int opcion;
    do {
        char buffer[20];
        printf("\n-------- MÓDULO VEHCULOS --------\n");
        printf("1. Agregar Vehiculos\n");
        printf("2. Editar Vehiculos\n");
        printf("3. Eliminar Vehiculos\n");
        printf("4. Listar Vehiculos por RUT\n");
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
                agregarVehiculos(); 
                break;
            case 2: 
                editarVehiculos(); 
                break;
            case 3: 
                eliminarVehiculos(); 
                break;
            case 4: {
                char rut[13];
                while (1) {
                    printf("\nIngrese RUT del miembro (ej: 12.345.678-9) o '0' para volver al menú: ");
                    if (fgets(rut, sizeof(rut), stdin) == NULL) continue;
                    rut[strcspn(rut, "\n")] = '\0';

                    if (strcmp(rut, "0") == 0) {
                        printf("Volviendo al menú de Vehiculos...\n");
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

                    //RUT válido y existente → listar todo lo asociado en VEHICULOS
                    listarVehiculosPorRut(rut);
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