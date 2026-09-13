#include "src/tp1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void imprimir(struct pokemon *p)
{
	char letra = 'C';
	if (p->rareza == RAREZA_RARO) {
		letra = 'R';
	} else if (p->rareza == RAREZA_LEGENDARIO) {
		letra = 'L';
	}
	printf("%s - Velocidad: %d - Peso: %.1f - Rareza: %c\n", p->nombre,
	       p->velocidad, p->peso, letra);
}

int main(int argc, char *argv[])
{
	tp1_t *tp1 = tp1_leer_archivo(argv[1]);
	const char *comando = argv[2];
	size_t i;

	if (strcmp(comando, "buscar-nombre") == 0) {
		imprimir(tp1_buscar_pokemon(tp1, argv[3]));

	} else if (strcmp(comando, "mostrar-uno") == 0) {
		srand((unsigned int)time(NULL));
		imprimir(tp1_buscar_orden(tp1,
					  (size_t)rand() % tp1_cantidad(tp1)));

	} else if (strcmp(comando, "mostrar-todo") == 0) {
		for (i = 0; i < tp1_cantidad(tp1); i++) {
			imprimir(tp1_buscar_orden(tp1, i));
		}

	} else if (strcmp(comando, "mostrar-rarezas") == 0) {
		enum rareza_pokemon r;
		for (r = RAREZA_COMUN; r <= RAREZA_LEGENDARIO; r++) {
			for (i = 0; i < tp1_cantidad(tp1); i++) {
				struct pokemon *p = tp1_buscar_orden(tp1, i);
				if (p->rareza == r) {
					imprimir(p);
				}
			}
		}
	}

	tp1_destruir(tp1);
	return 0;
}
