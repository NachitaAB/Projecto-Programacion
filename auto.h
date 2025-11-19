#ifndef AUTOS_H
#define AUTOS_H

#define NUM_AUTOS 10
#define NUM_MODELOS 5
#define NUM_COLORES 5
#define NUM_ANIOS 5


// Estructura para guardar la información de un auto
typedef struct {
    const char* marca;
    const char* modelo;
    const char* color;
    int anio;
} Auto;

extern const char* MARCAS_VALIDAS[NUM_AUTOS];
extern const char* MODELOS_VALIDOS[NUM_AUTOS][NUM_MODELOS];
extern const char* COLORES_VALIDOS[NUM_COLORES];
extern const int   ANIOS_VALIDOS[NUM_ANIOS];

int esMarcaValida(const char* marca);
int esModeloValido(const char* marca, const char* modelo);
int esColorValido(const char* color);
int esAnioValido(int anio);
void seleccionarAuto(char marca[], char modelo[], char color[], int* anio, int tam);

#endif
