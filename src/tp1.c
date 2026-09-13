#include "tp1.h"
#include "leer_linea.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define ERROR -1
#define EXITO 0
#define FORMATO_ESCRITURA "%s,%d,%.1f,%c\n"

struct tp1 {
	struct pokemon *pokemones;
	size_t cantidad;
	size_t capacidad;
};

int comparar_nombres(const char *s1, const char *s2)
{
	unsigned char c1;
	unsigned char c2;

	while (*s1 != '\0' && *s2 != '\0') {
		c1 = (unsigned char)tolower((unsigned char)*s1);
		c2 = (unsigned char)tolower((unsigned char)*s2);
		if (c1 != c2) {
			return (int)c1 - (int)c2;
		}
		s1++;
		s2++;
	}
	c1 = (unsigned char)tolower((unsigned char)*s1);
	c2 = (unsigned char)tolower((unsigned char)*s2);
	return (int)c1 - (int)c2;
}

size_t buscar_posicion_orden(tp1_t *tp1, const char *nombre)
{
	size_t i = 0;
	while (i < tp1->cantidad &&
	       comparar_nombres(tp1->pokemones[i].nombre, nombre) < 0) {
		i++;
	}
	return i;
}

bool nombres_son_iguales(const char *s1, const char *s2)
{
	if (!s1 || !s2) {
		return false;
	}
	return comparar_nombres(s1, s2) == 0;
}

bool nombre_valido(char *nombre, tp1_t *tp1)
{
	if (strlen(nombre) == 0) {
		return false;
	}
	bool pokemon_repetido = false;
	size_t i = 0;
	while ((!pokemon_repetido) && (i < tp1->cantidad)) {
		if (nombres_son_iguales(tp1->pokemones[i].nombre, nombre)) {
			pokemon_repetido = true;
		}
		i++;
	}
	return !pokemon_repetido;
}

bool velocidad_valida(char *str, int *velocidad)
{
	char *fin;
	long valor;

	if (!str || *str == '\0') {
		return false;
	}
	valor = strtol(str, &fin, 10);
	if (*fin != '\0') {
		return false;
	}
	if (valor < 1 || valor > 99) {
		return false;
	}
	*velocidad = (int)valor;
	return true;
}

bool peso_valido(char *str, float *peso)
{
	char *fin;
	float valor;

	if (!str || *str == '\0') {
		return false;
	}
	valor = strtof(str, &fin);
	if (*fin != '\0') {
		return false;
	}
	if (valor <= 0.0f) {
		return false;
	}
	*peso = valor;
	return true;
}

bool rareza_valida(char *str, enum rareza_pokemon *rareza)
{
	if (!str || str[0] == '\0' || str[1] != '\0') {
		return false;
	}
	if (str[0] == 'C') {
		*rareza = RAREZA_COMUN;
	} else if (str[0] == 'R') {
		*rareza = RAREZA_RARO;
	} else if (str[0] == 'L') {
		*rareza = RAREZA_LEGENDARIO;
	} else {
		return false;
	}
	return true;
}

int agregar_pokemon(tp1_t *tp1, char *nombre, int velocidad, float peso,
		    enum rareza_pokemon rareza)
{
	if (tp1->cantidad == tp1->capacidad) {
		size_t nueva_capacidad;
		if (tp1->capacidad == 0) {
			nueva_capacidad = 2;
		} else {
			nueva_capacidad = tp1->capacidad * 2;
		}
		struct pokemon *aux =
			realloc(tp1->pokemones,
				nueva_capacidad * sizeof(struct pokemon));
		if (!aux) {
			return ERROR;
		}
		tp1->pokemones = aux;
		tp1->capacidad = nueva_capacidad;
	}
	char *nombre_copia = malloc(strlen(nombre) + 1);
	if (!nombre_copia) {
		return ERROR;
	}
	strcpy(nombre_copia, nombre);
	size_t posicion = buscar_posicion_orden(tp1, nombre);
	for (size_t i = tp1->cantidad; i > posicion; i--) {
		tp1->pokemones[i] = tp1->pokemones[i - 1];
	}
	tp1->pokemones[posicion].nombre = nombre_copia;
	tp1->pokemones[posicion].velocidad = velocidad;
	tp1->pokemones[posicion].peso = peso;
	tp1->pokemones[posicion].rareza = rareza;
	tp1->cantidad++;
	return EXITO;
}

int procesar_linea(char *linea, tp1_t *tp1)
{
	if ((!tp1) || (!linea)) {
		return ERROR;
	}
	char *coma1 = strchr(linea, ',');
	if (!coma1) {
		printf("Línea con formáto inválido. Se ignorara la misma y se seguirá con el resto.");
		return EXITO;
	}
	char *coma2 = strchr(coma1 + 1, ',');
	if (!coma2) {
		printf("Línea con formáto inválido. Se ignorara la misma y se seguirá con el resto.");
		return EXITO;
	}
	char *coma3 = strchr(coma2 + 1, ',');
	if (!coma3) {
		printf("Línea con formáto inválido. Se ignorara la misma y se seguirá con el resto.");
		return EXITO;
	}
	if (strchr(coma3 + 1, ',') != NULL) {
		return EXITO;
	}
	*coma1 = '\0';
	*coma2 = '\0';
	*coma3 = '\0';
	char *nombre = linea;
	char *velocidad = coma1 + 1;
	char *peso = coma2 + 1;
	char *rareza_str = coma3 + 1;
	int vel;
	float peso_total;
	enum rareza_pokemon rareza;

	if ((!nombre_valido(nombre, tp1)) ||
	    (!velocidad_valida(velocidad, &vel)) ||
	    (!peso_valido(peso, &peso_total)) ||
	    (!rareza_valida(rareza_str, &rareza))) {
		return EXITO;
	}
	if (agregar_pokemon(tp1, nombre, vel, peso_total, rareza) != EXITO) {
		return ERROR;
	}
	return EXITO;
}

/**
 * Lee un archivo de pokemon según especificado en el enunciado y devuelve un TP1 con dichos pokemon.
 *
 * En caso de error devuelve NULL
 */
tp1_t *tp1_leer_archivo(const char *nombre)
{
	if (!nombre) {
		return NULL;
	}
	FILE *archivo_pokemones = fopen(nombre, "r");
	if (!archivo_pokemones) {
		return NULL;
	}
	tp1_t *tp1 = malloc(sizeof(tp1_t));
	if (!tp1) {
		fclose(archivo_pokemones);
		return NULL;
	}
	tp1->pokemones = NULL;
	tp1->cantidad = 0;
	tp1->capacidad = 0;
	char *linea = leer_linea(archivo_pokemones);
	while (linea != NULL) {
		procesar_linea(linea, tp1);
		free(linea);
		linea = leer_linea(archivo_pokemones);
	}
	fclose(archivo_pokemones);
	return tp1;
}

/**
 * Devuelve la cantidad de pokemon leidos en este TP1
 *
 * En caso de error devuelve 0
 *
 */
size_t tp1_cantidad(tp1_t *tp1)
{
	if (!tp1) {
		return 0;
	}
	return tp1->cantidad;
}

/**
 * Crea un TP1 con la combinación de los pokemon de ambos TPs. En caso de
 * duplicados se toma el pokemon del primer TP1.
 *
 * Devuelve un nuevo TP1 con la combinación de ambos TP1 o NULL en caso de
 * error.
 *
 */
tp1_t *tp1_combinar(tp1_t *tp1_a, tp1_t *tp1_b)
{
	if ((!tp1_a) || (!tp1_b)) {
		return NULL;
	}
	tp1_t *nuevo = malloc(sizeof(tp1_t));
	if (!nuevo) {
		return NULL;
	}
	nuevo->pokemones = NULL;
	nuevo->cantidad = 0;
	nuevo->capacidad = 0;
	bool hubo_error = false;
	size_t i = 0;
	for (i = 0; !hubo_error && i < tp1_a->cantidad; i++) {
		struct pokemon *p = &tp1_a->pokemones[i];
		if (agregar_pokemon(nuevo, p->nombre, p->velocidad, p->peso,
				    p->rareza) != EXITO) {
			hubo_error = true;
		}
	}
	for (i = 0; !hubo_error && i < tp1_b->cantidad; i++) {
		struct pokemon *p = &tp1_b->pokemones[i];
		if (nombre_valido(p->nombre, nuevo)) {
			if (agregar_pokemon(nuevo, p->nombre, p->velocidad,
					    p->peso, p->rareza) != EXITO) {
				hubo_error = true;
			}
		}
	}

	if (hubo_error) {
		tp1_destruir(nuevo);
		nuevo = NULL;
	}
	return nuevo;
}

/**
 * Busca el n-esimo pokemon del TP1 (ordenados por orden alfabético)
 *
 * En caso de error devuelve NULL
 */
struct pokemon *tp1_buscar_orden(tp1_t *tp1, size_t n)
{
	if (!tp1 || n >= tp1->cantidad) {
		return NULL;
	}
	return &tp1->pokemones[n];
}
/**
 * Guarda el TP1 a un archivo de texto.
 *
 * Devuelve el mismo TP1 o NULL en caso de error. Si el archivo ya existe se
 * sobreescribe.
 *
 */
tp1_t *tp1_escribir_archivo(tp1_t *tp1, const char *nombre)
{
	if ((!tp1) || (!nombre)) {
		return NULL;
	}
	FILE *archivo = fopen(nombre, "w");
	if (!archivo) {
		return NULL;
	}
	for (size_t i = 0; i < tp1->cantidad; i++) {
		char letra_rareza = 'C';
		if (tp1->pokemones[i].rareza == RAREZA_RARO) {
			letra_rareza = 'R';
		} else if (tp1->pokemones[i].rareza == RAREZA_LEGENDARIO) {
			letra_rareza = 'L';
		}
		fprintf(archivo, FORMATO_ESCRITURA, tp1->pokemones[i].nombre,
			tp1->pokemones[i].velocidad, tp1->pokemones[i].peso,
			letra_rareza);
	}
	fclose(archivo);
	return tp1;
}

/**
 * Busca el n-esimo pokemon del TP1 (ordenados por orden alfabético)
 *
 * En caso de error devuelve NULL
 */
struct pokemon *tp1_buscar_pokemon(tp1_t *tp1, const char *nombre)
{
	if ((!tp1) || (!nombre)) {
		return NULL;
	}
	struct pokemon *pokemon_buscado = NULL;
	bool encontrado = false;
	size_t i = 0;
	while ((!encontrado) && (i < tp1->cantidad)) {
		if (nombres_son_iguales(tp1->pokemones[i].nombre, nombre)) {
			encontrado = true;
			pokemon_buscado = &tp1->pokemones[i];
		}
		i++;
	}
	return pokemon_buscado;
}

/**
 * Recorre los pokemon del TP1 aplicando la función f a cada uno. Si f devuelve
 * true, sigue iterando.
 *
 * Devuelve la cantidad de veces que se invocó f.
 */
size_t tp1_iterar(tp1_t *tp1, bool (*f)(struct pokemon *, void *), void *extra)
{
	if ((!tp1) || (!f)) {
		return 0;
	}
	size_t contador = 0;
	bool seguir_iterando = true;
	while ((seguir_iterando) && (contador < tp1->cantidad)) {
		seguir_iterando = f(&tp1->pokemones[contador], extra);
		contador++;
	}
	return contador;
}

/**
 * Destruye el TP1 y toda la memoria asociada
 *
 */
void *tp1_destruir(tp1_t *tp1)
{
	if (!tp1) {
		return NULL;
	}
	for (size_t i = 0; i < tp1->cantidad; i++) {
		free(tp1->pokemones[i].nombre);
	}
	free(tp1->pokemones);
	free(tp1);
	return NULL;
}
