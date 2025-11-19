#include <stdio.h> //Entrada/Salida estándar
#include <string.h> //Manipulación de cadena (strcpy, strcmp, etc....)
#include <stdlib.h> //strtol() : convierte una cadena en un número entero largo
#include <ctype.h> //Funciones para caracteres (toupper, etc.....)
#include "miembros.h" //incluimos la cabecera para acceder a la struct y prototipos
#include "finanzas.h"
#include "vehiculos.h"
#include "fondosmutuos.h"
#include "validaciones.h"
#include "constante.h"

// Variables globales (accesibles desde otros módulos)
Miembro miembros[MAX_REGISTROS];
int totalMiembros = 0;

// Función para verificar si un RUT ya existe (usada por otros módulos)
int existeMiembro(const char* rut) {
    for (int i = 0; i < totalMiembros; i++) {
        if (strcmp(miembros[i].rut, rut) == 0) {
            return 1;
        }
    }
    return 0;
}
//Funcion para crear un nuevo miembro
int crearMiembro() {

    if (totalMiembros >= MAX_REGISTROS) {
        printf("Límite de 100 miembros alcanzado.\n");
        return 0;
    }

    Miembro nuevo;
    char buffer[100];
    
    do {
        printf("Nombre (máx. 19 caracteres): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        if (strlen(buffer) == 0) {
            printf("El nombre no puede estar vacío.\n");
        } else if (strlen(buffer) >= sizeof(nuevo.nombre)) {
            printf("Nombre demasiado largo.\n");
        } else {
            strcpy(nuevo.nombre, buffer);
            break;
        }
    } while (1);

    do {
        printf("Apellido (máx. 19 caracteres): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        if (strlen(buffer) == 0) {
            printf("El apellido no puede estar vacío.\n");
        } else if (strlen(buffer) >= sizeof(nuevo.apellido)) {
            printf("Apellido demasiado largo.\n");
        } else {
            strcpy(nuevo.apellido, buffer);
            break;
        }
    } while (1);

    do {
        printf("RUT (ej: 12.345.678-9): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        if (strlen(buffer) == 0) {
            printf("El RUT no puede estar vacío.\n");
        } else if (!validarRut(buffer)) {
            printf("RUT inválido (dígito verificador incorrecto).\n");
        } else if (existeMiembro(buffer)) {
            printf("Ya existe un miembro con ese RUT.\n");
        } else {
            strcpy(nuevo.rut, buffer);
            break;
        }
    } while (1);

    do {
        printf("Dirección (máx. 59 caracteres): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        if (strlen(buffer) == 0) {
            printf("La dirección no puede estar vacía.\n");
        } else if (strlen(buffer) >= sizeof(nuevo.direccion)) {
            printf("Dirección demasiado larga.\n");
        } else {
            strcpy(nuevo.direccion, buffer);
            break;
        }
    } while (1);

    do {
        printf("Edad (1-120): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        int edad;
        if (sscanf(buffer, "%d", &edad) == 1 && edad >= 1 && edad <= 120) {
            nuevo.edad = edad;
            break;
        }
        printf("Ingrese una edad válida entre 1 y 120.\n");
    } while (1);
    
    do {
        printf("Fecha de nacimiento (dd/mm/aaaa): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        if (strlen(buffer) == 0) {
            printf("La fecha no puede estar vacía.\n");
        } else if (!validarFecha(buffer)) {
            printf("Fecha inválida.\n");
        } else {
            strcpy(nuevo.fechaNacimiento, buffer);
            break;
        }
    } while (1);

    do {
        printf("Sexo (M/F): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        if (strlen(buffer) == 1) {
            char s = toupper(buffer[0]);
            if (s == 'M' || s == 'F') {
                nuevo.sexo = s;
                break;
            }
        }
        printf("Ingrese solo M o F.\n");
    } while (1);

    do {
        printf("¿Casado? (1=Sí / 0=No): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        int casado;
        if (sscanf(buffer, "%d", &casado) == 1 && (casado == 0 || casado == 1)) {
            nuevo.casado = casado;
            break;
        }
        printf("Ingrese 1 para Sí o 0 para No.\n");
    } while (1);

    do {
        printf("Email (máx. 39 caracteres): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        if (strlen(buffer) == 0) {
            printf("El email no puede estar vacío.\n");
        } else if (strlen(buffer) >= sizeof(nuevo.email)) {
            printf("Email demasiado largo.\n");
        } else if (!validarEmail(buffer)) {
            printf("Email inválido (debe contener '@' y un dominio con '.').\n");
        } else {
            strcpy(nuevo.email, buffer);
            break;
        }
    } while (1);

    miembros[totalMiembros++] = nuevo;
    printf("Miembro registrado con éxito.\n");
    return 1;
}

//Funcion para mostrar la lista de los miembros 
void listarMiembros() {
    if (totalMiembros == 0) {
        printf("No hay miembros registrados.\n");
        return;
    }

    printf("\n------ LISTA DE MIEMBROS ------\n");
    for (int i = 0; i < totalMiembros; i++) {
        printf("\n[ID: %d]\n", i + 1);
        printf("Nombre: %s %s\n", miembros[i].nombre, miembros[i].apellido);
        printf("RUT: %s\n", miembros[i].rut);
        printf("Dirección: %s\n", miembros[i].direccion);
        printf("Edad: %d\n", miembros[i].edad);
        printf("Fecha nac.: %s\n", miembros[i].fechaNacimiento);
        printf("Sexo: %c\n", miembros[i].sexo);
        printf("Casado: %s\n", miembros[i].casado ? "Sí" : "No");
        printf("Email: %s\n", miembros[i].email);
        printf("------------------------\n");
    }
}

//Funcion para editar los datos de los miembros
void editarMiembro() {
    //Comprobacion de que si hay miembros ingresados 
    if (totalMiembros == 0) {
        printf("No hay miembros para editar.\n");
        return;
    }

    //editar rut
    char rut[13];
    printf("Ingrese RUT del miembro a editar: ");
    fgets(rut, sizeof(rut), stdin);
    rut[strcspn(rut, "\n")] = 0;

    if (!validarRut(rut)) {
        printf("RUT inválido.\n");
        return;
    }

    int idx = -1;
    for (int i = 0; i < totalMiembros; i++) {
        if (strcmp(miembros[i].rut, rut) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("No se encontró un miembro con ese RUT.\n");
        return;
    }

    printf("Editando miembro: %s %s\n", miembros[idx].nombre, miembros[idx].apellido);
    char buffer[100];

    // Editar nombre
    printf("Nuevo nombre (actual: %s): ", miembros[idx].nombre);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) > 0 && strlen(buffer) < sizeof(miembros[idx].nombre)) {
        strcpy(miembros[idx].nombre, buffer);
    }

    // Editar apellido
    printf("Nuevo apellido (actual: %s): ", miembros[idx].apellido);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) > 0 && strlen(buffer) < sizeof(miembros[idx].apellido)) {
        strcpy(miembros[idx].apellido, buffer);
    }

    // Editar dirección
    printf("Nueva dirección (actual: %s): ", miembros[idx].direccion);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) > 0 && strlen(buffer) < sizeof(miembros[idx].direccion)) {
        strcpy(miembros[idx].direccion, buffer);
    }

    // Editar edad
    printf("Nueva edad (actual: %d): ", miembros[idx].edad);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) > 0) {
        int edad;
        if (sscanf(buffer, "%d", &edad) == 1 && edad >= 1 && edad <= 120) {
            miembros[idx].edad = edad;
        } else {
            printf("Edad inválida. No se actualizó.\n");
        }
    }

    // Editar fecha de nacimiento
    printf("Nueva fecha nac. (actual: %s): ", miembros[idx].fechaNacimiento);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) > 0) {
        if (validarFecha(buffer)) {
            strcpy(miembros[idx].fechaNacimiento, buffer);
        } else {
            printf("Fecha inválida. No se actualizó.\n");
        }
    }

    // Editar sexo
    printf("Nuevo sexo (actual: %c): ", miembros[idx].sexo);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) == 1) {
        char s = toupper(buffer[0]);
        if (s == 'M' || s == 'F') {
            miembros[idx].sexo = s;
        }
    }

    // Editar casado
    printf("¿Casado? (1=Sí / 0=No, actual: %s): ", miembros[idx].casado ? "Sí" : "No");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) > 0) {
        int casado;
        if (sscanf(buffer, "%d", &casado) == 1 && (casado == 0 || casado == 1)) {
            miembros[idx].casado = casado;
        }
    }

    // Editar email
    printf("Nuevo email (actual: %s): ", miembros[idx].email);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) > 0) {
        if (strlen(buffer) < sizeof(miembros[idx].email) && validarEmail(buffer)) {
            strcpy(miembros[idx].email, buffer);
        } else {
            printf("Email inválido o muy largo. No se actualizó.\n");
        }
    }

    printf("Miembro actualizado.\n");
}


void eliminarMiembro() {

    if (totalMiembros == 0) {
        printf("No hay miembros para eliminar.\n");
        return;
    }

    char rut[13];
    printf("Ingrese RUT del miembro a eliminar: ");
    fgets(rut, sizeof(rut), stdin);
    rut[strcspn(rut, "\n")] = 0;

    if (!validarRut(rut)) {
        printf("RUT inválido.\n");
        return;
    }

    int idx = -1;
    for (int i = 0; i < totalMiembros; i++) {
        if (strcmp(miembros[i].rut, rut) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("No se encontró un miembro con ese RUT.\n");
        return;
    }

    printf("¿Eliminar a %s %s y todos sus datos asociados? (s/n): ",
           miembros[idx].nombre, miembros[idx].apellido);
    char buffer[10];
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;

    if (buffer[0] == 's' || buffer[0] == 'S'){
        //ELIMINACIÓN EN CASCADA
        eliminarFinanzasPorRut(rut);
        eliminarPropiedadesPorRut(rut);
        eliminarVehiculosPorRut(rut);
        eliminarFondosMutuosPorRut(rut);
        //eliminarTerrenosPorRut(rut);
        //eliminarAgendaPorRut(rut);
        //eliminarBeneficiosPorRut(rut);

        // Ahora eliminar al miembro
        for (int i = idx; i < totalMiembros - 1; i++) {
            miembros[i] = miembros[i + 1];
        }
        totalMiembros--;

        printf("Miembro y todos sus datos asociados eliminados.\n");
    } else {
        printf("Eliminación cancelada.\n");
    }
}

int menuMiembro() {
    int opcion, c;
    do {
        char buffer[20];
        printf("\n--------- MÓDULO MIEMBROS ---------\n");
        printf("1. Crear Miembro\n");
        printf("2. Editar Miembro\n");
        printf("3. Eliminar Miembro\n");
        printf("4. Listar Miembros\n");
        printf("5. Volver al menú principal\n");
        printf("-------------------------------------\n");
        printf("Seleccione una opcion: ");
        
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
                crearMiembro(); 
                break;
            case 2: 
                editarMiembro(); 
                break;
            case 3: 
                eliminarMiembro(); 
                break;
            case 4: 
                listarMiembros(); 
                break;
            case 5: 
                return 0;
            default: 
                printf("Opción inválida.\n"); 
                break;
        }

    } while (opcion != 5);
    return 0;
}
