#include "tp1.h"
#include <stdio.h>

int main(void) {
    tp1_t *tp1 = tp1_leer_archivo("ejemplo.csv");
    if (!tp1) {
        printf("Error al cargar el archivo\n");
        return 1;
    }

    printf("Pokemones cargados.\n");

    tp1_destruir(tp1);
    return 0;
}