#include "pa2m.h"
#include <stdio.h>
#include "tp1.h"
#include <string.h>

void crear_archivo(char *nombre, char *contenido_archivo)
{
	FILE *archivo = fopen(nombre, "w");
	fprintf(archivo, "%s", contenido_archivo);
	fclose(archivo);
}

void prueba_leer_archivo()
{
	tp1_t *inexistente = tp1_leer_archivo("no_existe.csv");
	pa2m_afirmar(inexistente == NULL,
		     "Leer un archivo inexistente devuelve NULL");
	crear_archivo("prueba.csv", "Magikarp,10,5.2,C\n"
				     "lineamala,sincomas\n"
				     "Suicune,70,80,L\n"
				     "pikachu,60,6,C\n"
				     "PIKACHU,99,6,L\n");
	tp1_t *tp1 = tp1_leer_archivo("prueba.csv");
	pa2m_afirmar(tp1 != NULL, "Leer un archivo válido no devuelve NULL");
	pa2m_afirmar(
		tp1_cantidad(tp1) == 3,
		"Se ignoran líneas inválidas y duplicados (case-insensitive) (esperado=3, obtenido=%zu)",
		tp1_cantidad(tp1));
	struct pokemon *primero = tp1_buscar_orden(tp1, 0);
	pa2m_afirmar(primero != NULL &&
			     strcmp(primero->nombre, "Magikarp") == 0,
		     "Los pokemon quedan ordenados alfabéticamente");
	tp1_destruir(tp1);
	remove("prueba.csv");
}

void prueba_buscar_pokemon()
{
	crear_archivo("prueba.csv", "Suicune,70,80,L\nMagikarp,10,5.2,C\n");
	tp1_t *tp1 = tp1_leer_archivo("prueba.csv");
	pa2m_afirmar(
		tp1_buscar_pokemon(tp1, "sUICUNE") != NULL,
		"Se encuentra un pokemon existente, sin importar mayúsculas/minúsculas");
	pa2m_afirmar(tp1_buscar_pokemon(tp1, "Pikachu") == NULL,
		     "Buscar un pokemon inexistente devuelve NULL");
	tp1_destruir(tp1);
	remove("prueba.csv");
}

void prueba_buscar_orden()
{
	crear_archivo("prueba.csv", "Suicune,70,80,L\nMagikarp,10,5.2,C\n");
	tp1_t *tp1 = tp1_leer_archivo("prueba.csv");
	struct pokemon *primero = tp1_buscar_orden(tp1, 0);
	pa2m_afirmar(primero != NULL &&
			     strcmp(primero->nombre, "Magikarp") == 0,
		     "tp1_buscar_orden(0) devuelve el primero alfabéticamente");
	pa2m_afirmar(tp1_buscar_orden(tp1, tp1_cantidad(tp1)) == NULL,
		     "Un índice fuera de rango devuelve NULL");
	tp1_destruir(tp1);
	remove("prueba.csv");
}

void prueba_iterar()
{
	crear_archivo("prueba.csv",
		      "Suicune,70,80,L\nMagikarp,10,5.2,C\nLapras,30,50.3,R\n");
	tp1_t *tp1 = tp1_leer_archivo("prueba.csv");
	int contador = 0;
	size_t invocaciones = tp1_iterar(tp1, contar, &contador);
	pa2m_afirmar(
		invocaciones == 2,
		"tp1_iterar corta apenas la función devuelve false (esperado=2, obtenido=%zu)",
		invocaciones);
	tp1_destruir(tp1);
	remove("prueba.csv");
}

void prueba_combinar()
{
	crear_archivo("a.csv", "Pikachu,50,6,C\nRaichu,60,10,C\n");
	crear_archivo("b.csv", "pikachu,99,99,L\nMew,10,4,L\n");
	tp1_t *a = tp1_leer_archivo("a.csv");
	tp1_t *b = tp1_leer_archivo("b.csv");
	tp1_t *combinado = tp1_combinar(a, b);
	pa2m_afirmar(
		tp1_cantidad(combinado) == 3,
		"Combinar suma los pokemon sin duplicar (esperado=3, obtenido=%zu)",
		tp1_cantidad(combinado));
	tp1_destruir(a);
	tp1_destruir(b);
	pa2m_afirmar(
		tp1_buscar_pokemon(combinado, "pikachu") != NULL,
		"El TP1 combinado es independiente y sigue accesible tras destruir los originales");
	tp1_destruir(combinado);
	remove("a.csv");
	remove("b.csv");
}

void prueba_escribir_archivo()
{
	crear_archivo("prueba.csv", "Suicune,70,80,L\nMagikarp,10,5.2,C\n");
	tp1_t *tp1 = tp1_leer_archivo("prueba.csv");
	tp1_escribir_archivo(tp1, "salida.csv");
	tp1_t *releido = tp1_leer_archivo("salida.csv");
	pa2m_afirmar(
		tp1_cantidad(releido) == 2,
		"Lo escrito se puede releer y conserva la cantidad de pokemon (esperado=2, obtenido=%zu)",
		tp1_cantidad(releido));
	tp1_destruir(tp1);
	tp1_destruir(releido);
	remove("prueba.csv");
	remove("salida.csv");
}

void prueba_destruir()
{
	tp1_destruir(NULL);
	pa2m_afirmar(1, "Destruir un TP1 NULL no produce errores");
}

int main()
{
	pa2m_nuevo_grupo("=========== PRUEBAS DEL TP1: POKEDEX ===========");
	pa2m_nuevo_grupo("tp1_leer_archivo");
	prueba_leer_archivo();
	pa2m_nuevo_grupo("tp1_buscar_pokemon");
	prueba_buscar_pokemon();
	pa2m_nuevo_grupo("tp1_buscar_orden");
	prueba_buscar_orden();
	pa2m_nuevo_grupo("tp1_iterar");
	prueba_iterar();
	pa2m_nuevo_grupo("tp1_combinar");
	prueba_combinar();
	pa2m_nuevo_grupo("tp1_escribir_archivo");
	prueba_escribir_archivo();
	pa2m_nuevo_grupo("tp1_destruir");
	prueba_destruir();
	return pa2m_mostrar_reporte();
}
