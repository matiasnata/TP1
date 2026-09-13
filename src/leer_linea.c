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
	if ((!ptr) || (!*ptr) ||
	    (!archivo)) { //ptr (que exista la direccion del puntero), *ptr (que apunte a un bloque valido), y que archivo no sea NULL.
		return ERROR;
	}
	int caracter_leido = fgetc(
		archivo); //Intenta leer el primer caracter donde se encuentra parado el cursor.
	if (caracter_leido == EOF) {
		return ERROR;
	}
	size_t tope = 0;
	while ((caracter_leido != EOF) && (caracter_leido != '\n')) {
		if ((tope + 1) >=
		    *tamaño) { // + 1 ya que hay que dejar un lugar a lo ultimo para poder agregar el '\0' y que sea realmente un string
			size_t nuevo_tamanio = *tamaño * 2;
			char *ptr_aux = realloc(
				*ptr,
				nuevo_tamanio); // Creo un ptr auxiliar por si realloc falla y no perder el acceso al contenido (memory leak).
			if (!ptr_aux) {
				return ERROR;
			}
			*ptr = ptr_aux; //hago que ptr original apunte al nuevo bloque otorgado por realloc (ya comprobamos que no sea NULL)
			*tamaño = nuevo_tamanio; // Actualizo el tamaño
		}
		(*ptr)[tope] = (char)
			caracter_leido; //Hago el casteo ya que sino estaria insertado int en un string (char)
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
	if (!archivo) { //Chequeo que el archivo no apunte a NULL.
		return NULL;
	}
	size_t tamaño_inicial =
		TAMAÑO_INICIAL; //Lo creo de tipo size_t porque la funcion que llamo despues recibe un size_t
	char *ptr_string = malloc(
		TAMAÑO_INICIAL *
		sizeof(char)); // ptr_string apunta a la direccion devuelta por malloc en el HEAP.
	if (!ptr_string) { //Chequeo que no haya fallado y que no apunte a NULL para no hacer ninguna operacion peligrosa.
		return NULL;
	}
	int resultado = leer_linea_ptr(
		&ptr_string, &tamaño_inicial,
		archivo); //arhcivo ya es un puntero, tamaño se lo paso con direccion por si se modifica y le paso la direccion donde se almacena el ptr_string.
	if (resultado == ERROR) { //Si el resultado es -1 (error)
		free(ptr_string); // Libero la memoria reservada previamente con malloc
		return NULL; //devuelvo un puntero nulo.
	}
	return ptr_string; // Caso exitoso devuelvo el puntero
}
