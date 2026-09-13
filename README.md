## Información del estudiante

* Matias Armida
* Padrón: 114941
* armidamatias@gmail.com
* matiasnata

---

## Índice
* [1. Instrucciones](#1-Instrucciones)
  * [1.1. Compilar el proyecto](#11-Compilar-el-proyecto)
  * [1.2. Ejecutar las pruebas](#12-Ejecutar-las-pruebas)
  * [1.3. Ejecutar el programa con Valgrind](#13-Ejecutar-el-programa-con-Valgrind)
* [2. Funcionamiento](#2-Funcionamiento)
* [3. Estructura](#3-Estructura)
  * [3.1. Diagrama de memoria](#31-Diagrama-de-memoria)
  * [3.2. Análisis de complejidades](#32-Análisis-de-complejidades)
* [4. Decisiones de diseño y/o complejidades de implementación](#4-Decisiones-de-diseño-yo-complejidades-de-implementación)
* [5. Respuestas a las preguntas teóricas](#5-Respuestas-a-las-preguntas-teóricas)

## 1. Instrucciones

> [!TIP]
> Se recomienda fuertemente crear y usar un Makefile y colocar en esta sección los comandos Make.

### 1.1. Compilar el proyecto
```bash
gcc -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g src/*.c main.c -o tp1
```


### 1.2. Ejecutar las pruebas
```bash
gcc -std=c99 -Wall -Wextra -pedantic -g -Isrc -o pruebas_alumno pruebas/pruebas_alumno.c pruebas/pa2m.c src/tp1.c src/leer_linea.c
./pruebas_alumno
```

### 1.3. Ejecutar el programa con Valgrind
```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./tp1 pokedex.csv mostrar-todo
```

## 2. Funcionamiento

El programa recibe como parámetros la ruta de un archivo de texto con una pokédex. La pokédex consiste en una lista de pokemon, uno por línea y cada uno con su respectivo nombre, velocidad, peso y tipo de rareza. Por último recibe un comando que indica qué operación realizar con los datos (archivo y pokedex), mostrando el resultado por pantalla.

Al iniciar, el programa lee el archivo completo y valida de a una línea por vez. El primer caso es que la línea no cumpla con el formato esperado y se descarte para poder continuar con la lectura de las líneas restantes si es que las hay. El segundo caso es que la línea sea válida, la cual se agregará a un conjunto de pokemon.
En el caso que haya pokemon duplicado, es decir, que el nombre sea igual (sin distinguir mayúsculas y minúsculas) se conservara en el conjunto únicamente la primera aparición del pokemon en el archivo. 
Además, este conjunto se mantiene siempre ordenado alfabéticamente por nombre, sin importar el orden en que los pokemon fueron apareciendo en el archivo.

[Diagrama de flujo de lectura del archivo](img/diagrama_flujo_programa.svg)

Según el comando recibido, el programa realiza una de las siguientes acciones:

1) buscar-nombre: busca un pokemon por su nombre (sin distinguir mayúsculas de minúsculas). En el caso de ser encontrado, muestra sus datos, caso contrario informa que no se encuentra en la pokedex.

2) mostrar-uno: elige un pokemon al azar y muestra sus datos.

3) mostrar-todo: muestra todos los pokemon cargados, en orden alfabético.

4) mostrar-rarezas: muestra todos los pokemon agrupados por rareza (primero los comunes, luego los raros y por último los legendarios), respetando el orden alfabético dentro de cada grupo.

Además, el programa permite combinar los datos de dos pokédex en una nueva independiente (sin duplicar los pokemon repetidos entre ambas, priorizando siempre los datos de la primera) y guardar el contenido de una pokédex en un archivo de texto.

## 3. Estructura
Para implementar la estrcutura TDA (tp1_t), definí una estructura con 3 campos.
1) *pokemones: Un puntero a la dirección de un bloque de memoria dinámica (en el Heap) donde se almacenan contiguamente las estructuras pokemon. 

2) cantidad: Es de tipo size_t y representa el tamaño lógico del vector, es decir, el número de pok emonesválidos que ya fueron insertados en la estructura.

3) capacidad: Es de tipo size_t. Representa la memoria real asignada al vector dinámico. Indica el límite de elementos que el bloque actual puede contener sin realizar una redimensión (realloc).

Más allá de que lo pida el enunciado, el vector dinámico nos permite gestionar de forma mas eficiente el uso de la memoria. Al inicio del programa, la estructura puede no tener memoria reservada para el vector (capacidad 0). A medida que se procesa el archivo y se necesita almacenar un nuevo pokemon, se evalúa si se necesita realocar memoria. En el caso de precisar más memoria, se utiliza la función realloc para solicitar al sistema operativo un bloque más grande (duplicando la capacidad actual), copiando de forma automática los datos existentes al nuevo bloque si es necesario reubicarlo en el Heap (en el caso que falle no se pierde la información previa). Esto permite que el programa solo utilice la memoria necesaria.

### 3.1. Diagrama de memoria
Realizar un diagrama de memoria de la estructura de memoria durante la ejecución del programa, esto debe incluir el stack y el heap con las estructuras contenidas en ellos.

### 3.1 Diagrama de memoria (EJEMPLO)


### 3.2. Análisis de complejidades
Explicar las complejidades computacionales de las primitvas implementadas en el TP (las funciones del .h) en notación Big-O, justificando por qué cada una tiene esa determinada complejidad.

### 3.2. Análisis de complejidades (EJEMPLO 1)

* `fun1` tiene una complejidad de $O(1)$ ya que tiene como parámetro... y, al leer una línea....
* `fun2` tiene una complejidad de $O(n)$ ya que tiene como parámetro..., la cual....
* `fun3` tiene una complejidad de $O(n^2)$ ya que tiene como parámetro... y se encarga de....

### 3.2. Análisis de complejidades (EJEMPLO 2)

|      Función      |Complejidad|                 Justificación                  |
|:-----------------:|:---------:|:----------------------------------------------:|
|      `fun1`       |  $O(1)$   |Tiene como parámetro... y, al leer una línea....|
|      `fun2`       |  $O(n)$   |Tiene como parámetro..., la cual....            |
|      `fun3`       |  $O(n^2)$ |Tiene como parámetro... y se encarga de....     |

## 4. Decisiones de diseño y/o complejidades de implementación
Explicar las decisiones de diseño y/o las complejidades de implementación que hubo durante la resolución del TP.

## 4. Decisiones de diseño y/o complejidades de implementación (EJEMPLO)
La mayor complejidad en el TP se encuentra en la función `foo` que requiere hacer...; es por esto que decidí.... Además, decidí que el programa haga... para mejorar la implementación.

## 5. Respuestas a las preguntas teóricas
Deberás incluir en esta sección las respuestas a las preguntas teóricas indicadas en el [enunciado](./ENUNCIADO.md) del TP.

## 5. Respuestas a las preguntas teóricas (EJEMPLO)

### 5.1. ¿Porqué...?
Respondido en su respectiva sección.

### 5.2 ¿Cómo...?
Para implementar el....

### 5.3 ¿Cuál fue el...?
El motivo fue....
