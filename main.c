#include "bloomer.h"

//Funcion para entrar a nuestro menu principal del la empresa Bloomer.
void menuPrincipal(){

    int opcion;
    char buffer[20];    
    do{
        printf("\n------Sistema Bloomer------\n");
        printf("1. Miembros\n");
        printf("2. Finanzas\n");
        printf("3. Propiedades\n");
        printf("4. Vehículos\n");
        printf("5. Fondos mutuos\n");
        printf("6. Terrenos\n");
        printf("7. Agenda\n");
        printf("8. Beneficios\n");
        printf("0. Salir\n");
        printf("----------------------------\n");
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
        if (*end == '\0' && val >= 0 && val <= 8) {
            opcion = (int)val;
        } else {
            opcion = -1;
        }
        if (opcion == -1) {
            printf("Opción inválida. Ingrese un número del 1 al 5.\n");
            continue;
        }

        switch (opcion){

        case 1:
            menuMiembro();
            printf("Miembros y menu principal:");
            break;

        case 2:
            menuFinanzas();
            printf("Finanzas \n");
            break;

        case 3:
            menuPropiedades();
            printf("Propiedades");
            break;

        case 4:
            menuVehiculos();
            printf("Vehiculos");
            break;

        case 5:
            menuFondosMutuos();
            printf("Fondos Mutuos");
            break;

        case 6:
            menuTerrenos();
            printf("Terreno");
            break;

        case 7:
            menuAgenda();
            printf("Agenda");
            break;

        case 8:
            menuBeneficios();
            printf("Beneficios");
            break;

        case 0:
            printf("Saliendo del sistema. ¡Hasta pronto!\n");
            break;

        default:
            printf("Opcion invalida. Intente de nuevo.\n");
            break;
        }
    }while(opcion != 0);
}

int main(){
    menuPrincipal();
    return 0;
}
