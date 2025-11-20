#include <stdio.h> //Entrada/Salida estándar
#include <string.h> //Manipulación de cadena (strcpy, strcmp, etc....)
#include <stdlib.h> //strtol() : convierte una cadena en un número entero largo
#include <ctype.h> //Funciones para caracteres (toupper, etc.....)
#include "finanzas.h"
#include "miembros.h"         // para verificar que el RUT exista
#include "validaciones.h"    // para validar RUT y fecha
#include "bancos.h"          // para seleccionar banco válido

// Definición de variables globales
Finanzas finanzas[MAX_REGISTROS];
int totalFinanzas = 0; //Contador de registros reales 

// Tasa de conversión: 1 USD = 900 CLP
#define TASA_USD 900.0

// Elimina en cascada todas las finanzas asociadas a un RUT
void eliminarFinanzasPorRut(const char* rut) {

    if (!rut) return;
    for (int i = 0; i < totalFinanzas; ) {
        if (strcmp(finanzas[i].rutAsociado, rut) == 0) {
            // Desplazar todos los elementos siguientes una posición atrás
            for (int j = i; j < totalFinanzas - 1; j++) {
                finanzas[j] = finanzas[j + 1];
            }
            totalFinanzas--;
            // No incrementar 'i' aquí, porque el nuevo elemento en [i] debe ser revisado
        } else {
            i++;
        }
    }
}

//Crea un nuevo registro financiero vinculado a un rut existente 
void agregarFinanzas(void) {
    if (totalFinanzas >= MAX_REGISTROS) {
        printf("No se pueden registrar más finanzas (máx. %d).\n", MAX_REGISTROS);
        return;
    }

    char rut[13];
    while (1) {
        printf("Ingrese RUT del miembro (ej: 12.345.678-9) o '0' para cancelar: ");
        fgets(rut, sizeof(rut), stdin);
        rut[strcspn(rut, "\n")] = '\0';
        if (strcmp(rut, "0") == 0) {
            printf("Operación cancelada.\n");
            return;
        }
        if (!validarRut(rut)) {
            printf("RUT inválido. Intente nuevamente.\n");
            continue;
        }
        if (!existeMiembro(rut)) {
            printf("RUT no registrado. Intente nuevamente.\n");
            continue;
        }
        break;
    }

    Finanzas f;
    strncpy(f.rutAsociado, rut, sizeof(f.rutAsociado) - 1);
    f.rutAsociado[sizeof(f.rutAsociado) - 1] = '\0';

    printf("Número de cuenta: ");
    fgets(f.numeroCuenta, sizeof(f.numeroCuenta), stdin);
    f.numeroCuenta[strcspn(f.numeroCuenta, "\n")] = '\0';

    seleccionarBanco(f.banco, sizeof(f.banco));

    char fecha[11];
    while (1) {
        printf("Fecha de transacción (dd/mm/aaaa): ");
        fgets(fecha, sizeof(fecha), stdin);
        fecha[strcspn(fecha, "\n")] = '\0';
        if (validarFecha(fecha)) {
            strcpy(f.fechaTransaccion, fecha);
            break;
        } else {
            printf("Fecha inválida. Intente nuevamente.\n");
        }
    }

    // Ingresos
    char buffer[50];
    double ingresos;
    while (1) {
        printf("Ingresos (CLP): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (leerDouble(buffer, &ingresos) && ingresos >= 0) {
            f.ingresos = ingresos;
            break;
        } else {
            printf("Ingrese un número válido ≥ 0.\n");
        }
    }

    // Egresos
    double egresos;
    while (1) {
        printf("Egresos (CLP): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (leerDouble(buffer, &egresos) && egresos >= 0) {
            f.egresos = egresos;
            break;
        } else {
            printf("Ingrese un número válido ≥ 0.\n");
        }
    }

    f.saldoActual = f.ingresos - f.egresos;
    finanzas[totalFinanzas++] = f;
    printf("Finanzas registradas correctamente.\n");
}
//Muestra todas las finanzas de un miembro por su Rut
void listarFinanzasPorRut(const char* rut) {
    int encontrado = 0;
    printf("\n--- FINANZAS DEL RUT: %s ---\n", rut);
    for (int i = 0; i < totalFinanzas; i++) {
        if (strcmp(finanzas[i].rutAsociado, rut) == 0) {
            encontrado = 1;
            printf("\n[ID: %d]\n", i + 1);
            printf("Cuenta: %s\n", finanzas[i].numeroCuenta);
            printf("Banco: %s\n", finanzas[i].banco);
            printf("Fecha: %s\n", finanzas[i].fechaTransaccion);
            printf("Ingresos: ");
            mostrarMontoEnCLPyUSD(finanzas[i].ingresos);
            printf("Egresos: ");
            mostrarMontoEnCLPyUSD(finanzas[i].egresos);
            printf("Saldo actual: ");
            mostrarMontoEnCLPyUSD(finanzas[i].saldoActual);
            printf("------------------------\n");
        }
    }
    if (!encontrado) {
        printf("No hay registros financieros para este RUT.\n");
    }
}

//Modifica un registro financiero existente 
void editarFinanzas(void) {
    if (totalFinanzas == 0) {
        printf("No hay registros para editar.\n");
        return;
    }

    int id;
    printf("Ingrese ID de la finanza a editar (1 a %d): ", totalFinanzas);
    if (scanf("%d", &id) != 1 || id < 1 || id > totalFinanzas) {
        printf("ID inválido.\n");
        limpiar_buffer();
        return;
    }
    limpiar_buffer();
    id--; //Convierte a índice base 0

    printf("Editando finanza para RUT: %s\n", finanzas[id].rutAsociado);

    //Editar número de cuenta 
    printf("Nuevo número de cuenta (actual: %s): ", finanzas[id].numeroCuenta);
    char buffer[20];
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        strncpy(finanzas[id].numeroCuenta, buffer, sizeof(finanzas[id].numeroCuenta) - 1);
        finanzas[id].numeroCuenta[sizeof(finanzas[id].numeroCuenta) - 1] = '\0';
    }

    //Editar banco (solo si el usuario lo confirma)
    printf("¿Cambiar banco? (s/n): ");
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] == 's' || buffer[0] == 'S') {
        seleccionarBanco(finanzas[id].banco, sizeof(finanzas[id].banco));
    }


    //Editar fecha con validación
    printf("Nueva fecha (actual: %s): ", finanzas[id].fechaTransaccion);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0 && validarFecha(buffer)) {
        strncpy(finanzas[id].fechaTransaccion, buffer, sizeof(finanzas[id].fechaTransaccion) - 1);
        finanzas[id].fechaTransaccion[sizeof(finanzas[id].fechaTransaccion) - 1] = '\0';
    } else if (strlen(buffer) > 0) {
        printf("Fecha inválida. No se actualizó.\n");
    }

    //Editar ingresos 
    printf("Nuevos ingresos (actual: %.0f): ", finanzas[id].ingresos);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        double nuevo;
        if (leerDouble(buffer, &nuevo)) {
            finanzas[id].ingresos = nuevo;
        } else {
            printf("Ingresos inválidos. No se actualizó.\n");
        }
    }

    //Editar egresos 
    printf("Nuevos egresos (actual: %.0f): ", finanzas[id].egresos);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        double nuevo;
        if (leerDouble(buffer, &nuevo)) {
            finanzas[id].egresos = nuevo;
        } else {
            printf("Egresos inválidos. No se actualizó.\n");
        }
    }

    //Recalcular saldo 
    finanzas[id].saldoActual = finanzas[id].ingresos - finanzas[id].egresos;
    printf("Finanza actualizada.\n");
}

//Elimina un registro financiero por ID
void eliminarFinanzas(void) {
    if (totalFinanzas == 0) {
        printf("No hay registros para eliminar.\n");
        return;
    }

    int id;
    char buffer[50];
    while (1) {
        printf("ID a eliminar (1 a %d): ", totalFinanzas);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (leerEntero(buffer, &id) && id >= 1 && id <= totalFinanzas) {
            id--; // a índice 0
            break;
        } else {
            printf("ID inválido. Intente nuevamente.\n");
        }
    }

    printf("¿Está seguro? (s/n): ");
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] == 's' || buffer[0] == 'S') {
        for (int i = id; i < totalFinanzas - 1; i++) {
            finanzas[i] = finanzas[i + 1];
        }
        totalFinanzas--;
        printf("Registro eliminado.\n");
    } else {
        printf("Operación cancelada.\n");
    }
}

//Muestra un monto en ambos formatos: pesos chilenos y dólares
void mostrarMontoEnCLPyUSD(double monto) {
    printf("$%.0f CLP | $%.2f USD\n", monto, monto / TASA_USD);
}

//Menú interactivo del módulo finanzas 
int menuFinanzas() {
    int opcion;
    do {
        char buffer[20];
        printf("\n-------- MÓDULO FINANZAS --------\n");
        printf("1. Agregar finanzas\n");
        printf("2. Editar finanzas\n");
        printf("3. Eliminar finanzas\n");
        printf("4. Listar finanzas por RUT\n");
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
                agregarFinanzas(); 
                break;
            case 2: 
                editarFinanzas(); 
                break;
            case 3: 
                eliminarFinanzas(); 
                break;
            case 4: {
                char rut[13];
                while (1) {
                    printf("\nIngrese RUT del miembro (ej: 12.345.678-9) o '0' para volver al menú: ");
                    if (fgets(rut, sizeof(rut), stdin) == NULL) continue;
                    rut[strcspn(rut, "\n")] = '\0';

                    if (strcmp(rut, "0") == 0) {
                        printf("Volviendo al menú de Finanzas...\n");
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

                    //RUT válido y existente → listar TODO lo asociado en FINANZAS
                    listarFinanzasPorRut(rut);
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