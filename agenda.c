#include <stdio.h> //Entrada/Salida estándar
#include <string.h> //Manipulación de cadena (strcpy, strcmp, etc....)
#include <stdlib.h> //strtol() : convierte una cadena en un número entero largo
#include <ctype.h> //Funciones para caracteres (toupper, etc.....)
#include "agenda.h"
#include "miembros.h"
#include "fechas.h"
#include "validaciones.h"
#include "constante.h"

// Depinición de variables globales
Agenda agenda[MAX_REGISTROS];
int totalAgenda = 0; //Contador de registros reales 

// Elimina en cascada toda la Agenda asociadas a un RUT
void eliminarAgendaPorRut(const char* rut) {
    if (!rut) return;
    for (int i = 0; i < totalAgenda; ) {
        if (strcmp(agenda[i].rutAsociado, rut) == 0) {
            // Desplazar todos los elementos siguientes una posición atrás
            for (int j = i; j < totalAgenda - 1; j++) {
                agenda[j] = agenda[j + 1];
            }
            totalAgenda--;
            // No incrementar 'i' aquí, porque el nuevo elemento en [i] debe ser revisado
        } else {
            i++;
        }
    }
}
//Crea un nuevo registro de agenda vinculado a un rut existente
void agregarAgenda(void) {
    if (totalAgenda >= MAX_REGISTROS) {
        printf("Límite de %d citas alcanzado.\n", MAX_REGISTROS);
        return;
    }

    // Validar RUT
    char rut[13];
    char buffer[100];
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
            printf("Cliente no registrado.\n");
            continue;
        }
        strcpy(rut, buffer);
        break;
    }

    //Obtener el email del cliente usando su RUT
    char emailCliente[40] = "email_no_encontrado";
    for (int i = 0; i < totalMiembros; i++) {
        if (strcmp(miembros[i].rut, rut) == 0) {
            strcpy(emailCliente, miembros[i].email);
            break;
        }
    }

    Agenda a;
    strcpy(a.rutAsociado, rut);

    //ID = email del cliente (automático)
    strncpy(a.id, emailCliente, sizeof(a.id) - 1);
    a.id[sizeof(a.id) - 1] = '\0';

    // Fecha de cita (con menú interactivo)
    printf("Seleccione la fecha de la cita:\n");
    seleccionarFecha(a.fechaCita);

    // Hora de cita (con menú interactivo)
    printf("Seleccione la hora de la cita:\n");
    seleccionarHora(a.horaCita);

    // Título
    printf("Título de la cita: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(a.titulo, buffer, sizeof(a.titulo) - 1);
    a.titulo[sizeof(a.titulo) - 1] = '\0';

    // Detalle
    printf("Detalle: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(a.detalle, buffer, sizeof(a.detalle) - 1);
    a.detalle[sizeof(a.detalle) - 1] = '\0';

    // Invitados
    printf("Invitados (separados por comas): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(a.invitados, buffer, sizeof(a.invitados) - 1);
    a.invitados[sizeof(a.invitados) - 1] = '\0';

    // ¿Cíclico?
    printf("¿Es una cita cíclica? (s/n): ");
    fgets(buffer, sizeof(buffer), stdin);
    a.esCiclica = (buffer[0] == 's' || buffer[0] == 'S');
    if (a.esCiclica) {
        printf("Seleccione la fecha de repetición:\n");
        seleccionarFecha(a.fechaRepeticion);
    } else {
        strcpy(a.fechaRepeticion, ""); // o "N/A"
    } 

    // Guardar
    agenda[totalAgenda] = a;
    totalAgenda++;

    printf("\nCita registrada para %s\n", emailCliente);
    printf("ID (email): %s\n", a.id);
}

//Lista todas las agendas asociadas a un RUT dado
void listarAgendaPorRut(const char* rut) {
    int encontrados = 0;
    printf("\n------ AGENDA para RUT %s ------\n", rut);
    for (int i = 0; i < totalAgenda; i++) {
        if (strcmp(agenda[i].rutAsociado, rut) == 0) {
            encontrados++;
            printf("\n[ID: %s]\n", agenda[i].id);
            printf("Fecha: %s | Hora: %s\n", agenda[i].fechaCita, agenda[i].horaCita);
            printf("Título: %s\n", agenda[i].titulo);
            printf("Detalle: %s\n", agenda[i].detalle);
            printf("Invitados: %s\n", agenda[i].invitados);
            printf("Cíclico: %s\n", agenda[i].esCiclica ? "Sí" : "No");
            printf("------------------------\n");
        }
    }
    if (encontrados == 0) {
        printf("No hay citas registradas para este RUT.\n");
    }
}

//Edita una cita existente en la agenda
void editarAgenda(void) {
    if (totalAgenda == 0) {
        printf("No hay citas registradas para editar.\n");
        return;
    }

    char idIngresado[40]; // ID = email
    printf("Ingrese el email (ID) de la cita a editar: ");
    char buffer[100];
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(idIngresado, buffer, sizeof(idIngresado) - 1);
    idIngresado[sizeof(idIngresado) - 1] = '\0';

    // Buscar por ID (email)
    int idx = -1;
    for (int i = 0; i < totalAgenda; i++) {
        if (strcmp(agenda[i].id, idIngresado) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("No se encontró una cita con ese email (ID).\n");
        return;
    }

    Agenda* a = &agenda[idx];
    printf("\nCita encontrada:\n");
    printf("ID (email): %s\n", a->id);
    printf("Fecha: %s | Hora: %s\n", a->fechaCita, a->horaCita);
    printf("Título: %s\n", a->titulo);
    printf("RUT asociado: %s\n", a->rutAsociado);
    printf("Cíclica: %s\n", a->esCiclica ? "Sí" : "No");

    //Editar campos (solo si el usuario ingresa algo)

    // Fecha
    printf("\n¿Desea cambiar la fecha? (s/n): ");
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] == 's' || buffer[0] == 'S') {
        printf("Seleccione nueva fecha:\n");
        seleccionarFecha(a->fechaCita);
    }

    // Hora
    printf("¿Desea cambiar la hora? (s/n): ");
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] == 's' || buffer[0] == 'S') {
        printf("Seleccione nueva hora:\n");
        seleccionarHora(a->horaCita);
    }

    // Título
    printf("Nuevo título (actual: %s): ", a->titulo);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        strncpy(a->titulo, buffer, sizeof(a->titulo) - 1);
        a->titulo[sizeof(a->titulo) - 1] = '\0';
    }

    // Detalle
    printf("Nuevo detalle (actual: %s): ", a->detalle);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        strncpy(a->detalle, buffer, sizeof(a->detalle) - 1);
        a->detalle[sizeof(a->detalle) - 1] = '\0';
    }

    // Invitados
    printf("Nuevos invitados (actual: %s): ", a->invitados);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        strncpy(a->invitados, buffer, sizeof(a->invitados) - 1);
        a->invitados[sizeof(a->invitados) - 1] = '\0';
    }

    // ¿Cíclico?
    printf("Cíclico: %s\n", a->esCiclica ? "Sí" : "No");
    if (a->esCiclica) {
        printf("Fecha de repetición: %s\n", a->fechaRepeticion);
    }

    printf("Cita actualizada correctamente.\n");
}
//Elimina una cita de la agenda
void eliminarAgenda(void) {
    if (totalAgenda == 0) {
        printf("No hay citas para eliminar.\n");
        return;
    }

    char idIngresado[40];
    printf("Ingrese el email (ID) de la cita a eliminar: ");
    char buffer[100];
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(idIngresado, buffer, sizeof(idIngresado) - 1);
    idIngresado[sizeof(idIngresado) - 1] = '\0';

    int idx = -1;
    for (int i = 0; i < totalAgenda; i++) {
        if (strcmp(agenda[i].id, idIngresado) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("No se encontró una cita con ese ID.\n");
        return;
    }

    printf("¿Eliminar la cita de %s? (s/n): ", agenda[idx].id);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] == 's' || buffer[0] == 'S') {
        for (int i = idx; i < totalAgenda - 1; i++) {
            agenda[i] = agenda[i + 1];
        }
        totalAgenda--;
        printf("Cita eliminada.\n");
    } else {
        printf("Eliminación cancelada.\n");
    }
}
//Menú interactivo del módulo agenda
int menuAgenda(void) {
    int opcion;
    do {
        char buffer[20];
        
        printf("\n-------- MÓDULO AGENDA --------\n");
        printf("1. Agregar Agenda\n");
        printf("2. Editar Agenda\n");
        printf("3. Eliminar Agenda\n");
        printf("4. Listar Agenda por RUT\n");
        printf("5. Volver al menú principal\n");
        printf("---------------------------------\n");
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
                agregarAgenda(); 
                break;
            case 2: 
                editarAgenda(); 
                break;
            case 3: 
                eliminarAgenda(); 
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
                    //RUT válido y existente → listar todo lo asociado en Agenda
                    listarAgendaPorRut(rut);
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