#include <stdio.h> //Entrada/Salida estándar
#include <string.h> //Manipulación de cadena (strcpy, strcmp, etc....)
#include <stdlib.h> //strtol() : convierte una cadena en un número entero largo
#include <ctype.h> //Funciones para caracteres (toupper, etc.....)
#include "fondosMutuos.h"
#include "miembros.h"         
#include "validaciones.h"
#include "constante.h"
#include "bancos.h"

// Depinición de variables globales
FondoMutuo fondosMutuos[MAX_REGISTROS];
int totalFondosMutuos = 0; //Contador de registros reales 

// Elimina en cascada todos los fondos mutuos asociadas a un RUT
void eliminarFondosMutuosPorRut(const char* rut) {
    if (!rut) return;
    for (int i = 0; i < totalFondosMutuos; ) {
        if (strcmp(fondosMutuos[i].rutAsociado, rut) == 0) {
            // Desplazar todos los elementos siguientes una posición atrás
            for (int j = i; j < totalFondosMutuos - 1; j++) {
                fondosMutuos[j] = fondosMutuos[j + 1];
            }
            totalFondosMutuos--;
            // No incrementar 'i' aquí, porque el nuevo elemento en [i] debe ser revisado
        } else {
            i++;
        }
    }
}

// Cálculo recursivo de proyección con interés compuesto
double calcularProyeccionRecursiva(double capital, double tasaMensual, int meses) {
    if (meses <= 0) {
        return capital;
    }
    return calcularProyeccionRecursiva(capital * (1 + tasaMensual), tasaMensual, meses - 1);
}

//Crea un nuevo registro de fondos mutuos vinculado a un rut existente 
void agregarFondosMutuos(void) {

    if (totalFondosMutuos >= MAX_REGISTROS) {
        printf("Límite de %d fondos mutuos alcanzado.\n", MAX_REGISTROS);
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

    FondoMutuo f;
    strcpy(f.rutAsociado, rut);

    //ID único (puede ser ingresado o generado)
    printf("ID del fondo (ej: FM001): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) == 0) {
        snprintf(f.id, sizeof(f.id), "FM%03d", totalFondosMutuos + 1);
    } else {
        strncpy(f.id, buffer, sizeof(f.id) - 1);
        f.id[sizeof(f.id) - 1] = '\0';
    }

    //Ahorros a la fecha
    while (1) {
        printf("Ahorros a la fecha (CLP, ≥ 0): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        double ahorros;
        if (leerDouble(buffer, &ahorros) && ahorros >= 0) {
            f.ahorros = ahorros;
            break;
        } else {
            printf("Ingrese un monto válido ≥ 0.\n");
        }
    }

    // --- Paso 4: Tasa de interés (en decimal, ej: 0.035 = 3.5%) ---
    while (1) {
        printf("Tasa de interés anual (ej: 0.035 para 3.5%%): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        double tasa;
        if (leerDouble(buffer, &tasa) && tasa >= 0 && tasa <= 1.0) {
            f.tasaInteres = tasa;
            break;
        } else {
            printf("Ingrese una tasa entre 0 y 1 (ej: 0.05 para 5%%).\n");
        }
    }

    //Interés total generado (calculado o ingresado) ---
    f.interesTotal = f.ahorros * f.tasaInteres;

    //Banco
    printf("Banco (ej: Santander, BCI, Estado): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) == 0) {
        strcpy(f.banco, "N/A");
    } else {
        strncpy(f.banco, buffer, sizeof(f.banco) - 1);
        f.banco[sizeof(f.banco) - 1] = '\0';
    }

    // Proyección (usando recursividad) ---
    // Proyección mensual = 1 mes
    f.proyeccionMensual = calcularProyeccionRecursiva(f.ahorros, f.tasaInteres / 12, 1);
    // Proyección anual = 12 meses
    f.proyeccionAnual = calcularProyeccionRecursiva(f.ahorros, f.tasaInteres / 12, 12);

    // --- Guardar ---
    fondosMutuos[totalFondosMutuos] = f;
    totalFondosMutuos++;

    printf("\nFondo mutuo registrado para RUT %s\n", rut);
    printf("ID: %s | Ahorros: $%.0f CLP\n", f.id, f.ahorros);
    printf("Tasa: %.2f%% | Proyección anual: $%.0f CLP\n", f.tasaInteres * 100, f.proyeccionAnual);
}

//Muestra todos los fondos mutuos de un miembro por su rut 
void listarFondosMutuosPorRut(const char* rut){
    if (!rut) {
        printf("RUT inválido.\n");
        return;
    }

    int encontrado = 0;
    printf("\n--- FONDOS MUTUOS DEL RUT: %s ---\n", rut);

    for (int i = 0; i < totalFondosMutuos; i++) {
        if (strcmp(fondosMutuos[i].rutAsociado, rut) == 0) {
            encontrado = 1;
            printf("\n[ID: %s]\n", fondosMutuos[i].id);
            printf("Ahorros: $%.0f CLP\n", fondosMutuos[i].ahorros);
            printf("Interés total generado: $%.0f CLP\n", fondosMutuos[i].interesTotal);
            printf("Tasa de interés: %.2f%%\n", fondosMutuos[i].tasaInteres * 100);
            printf("Banco: %s\n", fondosMutuos[i].banco);
            printf("Proyección mensual: $%.0f CLP\n", fondosMutuos[i].proyeccionMensual);
            printf("Proyección anual: $%.0f CLP\n", fondosMutuos[i].proyeccionAnual);
            printf("------------------------\n");
        }
    }

    if (!encontrado) {
        printf("No hay fondos mutuos registrados para este cliente.\n");
    }
}

//Edita los Fondos Mutuos
void editarFondosMutuos(){
    if (totalFondosMutuos == 0) {
        printf("No hay fondos mutuos registrados para editar.\n");
        return;
    }

    char idIngresado[15];
    printf("Ingrese el ID del fondo a editar (ej: FM001): ");
    char buffer[50];
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(idIngresado, buffer, sizeof(idIngresado) - 1);
    idIngresado[sizeof(idIngresado) - 1] = '\0';

    // Buscar por ID
    int idx = -1;
    for (int i = 0; i < totalFondosMutuos; i++) {
        if (strcmp(fondosMutuos[i].id, idIngresado) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("No se encontró un fondo con ese ID.\n");
        return;
    }

    FondoMutuo* f = &fondosMutuos[idx];
    printf("\nFondo encontrado:\n");
    printf("ID: %s | RUT: %s\n", f->id, f->rutAsociado);
    printf("Ahorros: $%.0f | Tasa: %.2f%% | Banco: %s\n", 
           f->ahorros, f->tasaInteres * 100, f->banco);

    //Editar Ahorros
    printf("\nNuevos ahorros (actual: $%.0f): ", f->ahorros);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        double ahorros;
        if (leerDouble(buffer, &ahorros) && ahorros >= 0) {
            f->ahorros = ahorros;
        } else {
            printf("Ahorros inválidos. No se actualizó.\n");
        }
    }

    //Editar Tasa de interés
    printf("Nueva tasa de interés (actual: %.2f%%, ej: 0.035): ", f->tasaInteres * 100);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        double tasa;
        if (leerDouble(buffer, &tasa) && tasa >= 0 && tasa <= 1.0) {
            f->tasaInteres = tasa;
        } else {
            printf("Tasa inválida (debe estar entre 0 y 1). No se actualizó.\n");
        }
    }

    //Editar Banco
    printf("Nuevo banco (actual: %s): ", f->banco);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        strncpy(f->banco, buffer, sizeof(f->banco) - 1);
        f->banco[sizeof(f->banco) - 1] = '\0';
    }

    //Recalcular campos derivados (con recursividad)
    f->interesTotal = f->ahorros * f->tasaInteres;
    f->proyeccionMensual = calcularProyeccionRecursiva(f->ahorros, f->tasaInteres / 12, 1);
    f->proyeccionAnual = calcularProyeccionRecursiva(f->ahorros, f->tasaInteres / 12, 12);

    printf("Fondo actualizado correctamente.\n");
}

//Elimina un registro de los vehiculos por patente
void eliminarFondosMutuos(){
        if (totalFondosMutuos == 0) {
        printf("No hay fondos mutuos registrados para eliminar.\n");
        return;
    }

    char idIngresado[15];
    printf("Ingrese el ID del fondo a eliminar (ej: FM001): ");
    char buffer[50];
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(idIngresado, buffer, sizeof(idIngresado) - 1);
    idIngresado[sizeof(idIngresado) - 1] = '\0';

    // Buscar el fondo por ID
    int idx = -1;
    for (int i = 0; i < totalFondosMutuos; i++) {
        if (strcmp(fondosMutuos[i].id, idIngresado) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("No se encontró un fondo con ese ID.\n");
        return;
    }

    // Mostrar datos del fondo a eliminar
    printf("\n¿Eliminar este fondo mutuo?\n");
    printf("ID: %s\n", fondosMutuos[idx].id);
    printf("RUT asociado: %s\n", fondosMutuos[idx].rutAsociado);
    printf("Ahorros: $%.0f CLP | Banco: %s\n",fondosMutuos[idx].ahorros, fondosMutuos[idx].banco);
    printf("¿Está seguro? (s/n): ");

    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] == 's' || buffer[0] == 'S') {
        // Eliminar desplazando elementos
        for (int i = idx; i < totalFondosMutuos - 1; i++) {
            fondosMutuos[i] = fondosMutuos[i + 1];
        }
        totalFondosMutuos--;
        printf("Fondo con ID %s eliminado correctamente.\n", idIngresado);
    } else {
        printf("Eliminación cancelada.\n");
    }
}

//Menú interactivo del módulo fondos mutuos
int menuFondosMutuos(void) {
    int opcion;
    do {
        char buffer[20];
        
        printf("\n-------- MÓDULO FONDOS MUTUOS --------\n");
        printf("1. Agregar Fondos Mutuos\n");
        printf("2. Editar Fondos Mutuos\n");
        printf("3. Eliminar Fondos Mutuos\n");
        printf("4. Listar Fondos Mutuos por RUT\n");
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
                agregarFondosMutuos(); 
                break;
            case 2: 
                editarFondosMutuos(); 
                break;
            case 3: 
                eliminarFondosMutuos(); 
                break;
            case 4: {
                char rut[13];
                while (1) {
                    printf("\nIngrese RUT del miembro (ej: 12.345.678-9) o '0' para volver al menú: ");
                    if (fgets(rut, sizeof(rut), stdin) == NULL) continue;
                    rut[strcspn(rut, "\n")] = '\0';

                    if (strcmp(rut, "0") == 0) {
                        printf("Volviendo al menú de fondos mutuos...\n");
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
                    //RUT válido y existente → listar todo lo asociado en FONDOS MUTUOS
                    listarFondosMutuosPorRut(rut);
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
