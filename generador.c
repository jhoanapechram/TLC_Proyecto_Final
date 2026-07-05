#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "generador.h"

#define MAX_VARS   256
#define MAX_NOMBRE 256

static struct {
	char  *datos;
	size_t longitud;
	size_t capacidad;
} salida;

static int nivel = 1; 
static char variables[MAX_VARS][MAX_NOMBRE];
static int  numVariables = 0;

// función que reserva el espacio inicial para el texto
void generadorInicializar(void) {
	salida.capacidad = 1024;
	salida.datos = malloc(salida.capacidad);
	if(salida.datos == NULL) {
		fprintf(stderr, "Error! Ya no hay más memoria RAM disponible\n");
		exit(1);
	}
	salida.datos[0] = '\0';
	salida.longitud = 0;
	nivel = 1;
	numVariables = 0;
}

// función para agregar nuevos fragmentos de texto después de lo escrito anterior
void generadorAgregar(const char *texto) {
	size_t len = strlen(texto);
	if(salida.longitud + len + 1 > salida.capacidad) {
		while(salida.longitud + len + 1 > salida.capacidad)
			salida.capacidad *= 2; // aquí se duplica la capacidad de la memoria
		char *tmp = realloc(salida.datos, salida.capacidad); // definimos un puntero temporal (tmp) para la función realloc
		if(tmp == NULL) {
			fprintf(stderr, "Error! El sistema ya no tiene memoria RAM para expandir el texto\n");
			exit(1);
		}
		salida.datos = tmp;
	}
	memcpy(salida.datos + salida.longitud, texto, len + 1);
	salida.longitud += len;
}

// función para agregar los espacios en blanco al inicio de una línea (sangría)
void generadorIndentar(void) {
	for(int i = 0; i < nivel; i++)
		generadorAgregar("    ");
}

// función para aumentar la sangría
void generadorSubirNivel(void) {
	nivel++;
}

// función para reducir la sangría
void generadorBajarNivel(void) {
	nivel--;
}

// función para checar si una variable ya fue registrada anteriormente, para evitar volverla a declarar 
int generadorVariableDeclarada(const char *nombre) {
	for(int i = 0; i < numVariables; i++) {
		if(strcmp(variables[i], nombre) == 0){
			return 1;
		}
	}
	return 0;
}

// función para declarar nuevas variables 
void generadorDeclararVariable(const char *nombre) {
	if(generadorVariableDeclarada(nombre) == 1) // la función termina en caso de que la variable ya exista
		return;
	if(numVariables >= MAX_VARS) {
		fprintf(stderr, "Se superó el límite de %d variables\n", MAX_VARS); // se imprime un error en caso de superar el límite de variables
		exit(1);
	}
	strncpy(variables[numVariables], nombre, MAX_NOMBRE - 1);
	variables[numVariables][MAX_NOMBRE - 1] = '\0';
	numVariables++;
}

// función para guardar el texto acumulado en la memoria en un archivo .c
void generadorGuardar(const char *nombreArchivo) {
	FILE *f = fopen(nombreArchivo, "w"); // abrir o crear el archivo para escribir en el ("w")
	if(f == NULL) {
		fprintf(stderr, "No se pudo crear el archivo '%s'\n", nombreArchivo);
		exit(1);
	}
	fputs(salida.datos, f); // pasa al archivo todo el texto guardado en la memoria
	fclose(f); // cierra el archivo
}

// función para liberar la memoria RAM al sistema
void generadorLiberar(void) {
	free(salida.datos);
	salida.datos = NULL;
	salida.longitud = 0;
	salida.capacidad = 0;
}

// crea una cadena con un formato de texto específico calculando el tamaño exacto que va a medir
char *formatoCadena(const char *fmt, ...) {
	va_list argumento1, argumento2;
	va_start(argumento1, fmt); // inicializa la lista argumento1
	va_copy(argumento2, argumento1); // copia la lista de argumento1 en argumento2

	int necesario = vsnprintf(NULL, 0, fmt, argumento1);
	va_end(argumento1); // limpiamos la memoria interna

	// con la condición, se detecta si hubo algún error al calcular el tamaño del texto
	if(necesario < 0) {
		fprintf(stderr, "Error al formatear cadena\n");
		exit(1);
	}
	// se usa la función malloc para apartar el espacio exacto de memoria RAM
	char *resultado = malloc((size_t)necesario + 1);
	if(resultado == NULL) {
		fprintf(stderr, "Error! Memoria RAM agotada\n"); // se imprime el error en caso de que no haya memoria RAM disponible
		exit(1);
	}

	// se escribe el texto final ya formateado en el espacio de memoria apartado
	vsnprintf(resultado, (size_t)necesario + 1, fmt, argumento2);
	va_end(argumento2);

	return resultado;
}
