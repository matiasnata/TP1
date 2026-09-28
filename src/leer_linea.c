#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "leer_linea.h"

#define ERROR -1
#define TAMAÑO_EXTRA 10
#define TAMAÑO_INICIAL 10

/**
 * Lee caracteres por entrada estándar hasta encontrar el fin de línea o no
 * queden mas caracteres por leer. Los caracteres leídos se van insertando en el
 * bloque de memoria apuntada por *ptr de tamaño inicial *tamaño. Dicho bloque
 * debe haber sido reservado por el usuario previa invocación de la función
 * utilizando malloc.
 *
 * Si el tamaño del bloque no es suficiente, se utiliza realloc para agrandar el
 * bloque y se actualiza *tamaño y *ptr.
 *
 * Devuelve la cantidad de caracteres leidos (sin incluír el 0 final) o -1 en
 * caso de error.
 */
int leer_linea_ptr(char **ptr, size_t *tamaño, FILE *archivo)
{
	if ((!ptr) || (!*ptr) || (!archivo)) {
		return ERROR;
	}
	int caracter_leido = fgetc(archivo);
	if (caracter_leido == EOF) {
		return ERROR;
	}
	size_t tope = 0;
	while ((caracter_leido != EOF) && (caracter_leido != '\n')) {
		if ((tope + 1) >= *tamaño) {
			size_t nuevo_tamanio = *tamaño * 2;
			char *ptr_aux = realloc(*ptr, nuevo_tamanio);
			if (!ptr_aux) {
				return ERROR;
			}
			*ptr = ptr_aux;
			*tamaño = nuevo_tamanio;
		}
		(*ptr)[tope] = (char)caracter_leido;
		tope++;
		caracter_leido = fgetc(archivo);
	}
	(*ptr)[tope] = '\0';
	return (int)tope;
}

/**
 * Lee caracteres por entrada estándar hasta encontrar el fin de línea (\n) o no
 * queden mas caracteres para leer.
 *
 * Devuelve un puntero a un string reservado dinámicamente con malloc a liberar
 * por el usuario utilizando free o NULL en caso de error.
 */
char *leer_linea(FILE *archivo)
{
	if (!archivo) {
		return NULL;
	}
	size_t tamaño_inicial = TAMAÑO_INICIAL;
	char *ptr_string = malloc(TAMAÑO_INICIAL * sizeof(char));
	if (!ptr_string) {
		return NULL;
	}
	int resultado = leer_linea_ptr(&ptr_string, &tamaño_inicial, archivo);
	if (resultado == ERROR) {
		free(ptr_string);
		return NULL;
	}
	return ptr_string;
}
