#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "generador.h"

#define MAX_VARS   256
#define MAX_NOMBRE 256

/* Buffer dinamico donde se va acumulando el codigo C generado */
static struct {
	char  *datos;
	size_t longitud;
	size_t capacidad;
} salida;

static int nivel = 1; /* nivel de indentacion actual (1 = dentro de main) */

/* Tabla de variables ya declaradas en el codigo C generado */
static char variables[MAX_VARS][MAX_NOMBRE];
static int  numVariables = 0;

void generadorInit(void)
{
	salida.capacidad = 1024;
	salida.datos = malloc(salida.capacidad);
	if(!salida.datos)
	{
		fprintf(stderr, "Error de memoria\n");
		exit(1);
	}
	salida.datos[0] = '\0';
	salida.longitud = 0;
	nivel = 1;
	numVariables = 0;
}

void generadorAgregar(const char *texto)
{
	size_t len = strlen(texto);

	if(salida.longitud + len + 1 > salida.capacidad)
	{
		while(salida.longitud + len + 1 > salida.capacidad)
			salida.capacidad *= 2;

		char *tmp = realloc(salida.datos, salida.capacidad);
		if(!tmp)
		{
			fprintf(stderr, "Error de memoria\n");
			exit(1);
		}
		salida.datos = tmp;
	}

	memcpy(salida.datos + salida.longitud, texto, len + 1);
	salida.longitud += len;
}

void generadorIndentar(void)
{
	for(int i = 0; i < nivel; i++)
		generadorAgregar("    ");
}

void generadorSubirNivel(void)
{
	nivel++;
}

void generadorBajarNivel(void)
{
	nivel--;
}

int generadorVariableDeclarada(const char *nombre)
{
	for(int i = 0; i < numVariables; i++)
	{
		if(strcmp(variables[i], nombre) == 0)
			return 1;
	}
	return 0;
}

void generadorDeclararVariable(const char *nombre)
{
	if(generadorVariableDeclarada(nombre))
		return;

	if(numVariables >= MAX_VARS)
	{
		fprintf(stderr, "Se supero el limite de %d variables\n", MAX_VARS);
		exit(1);
	}

	strncpy(variables[numVariables], nombre, MAX_NOMBRE - 1);
	variables[numVariables][MAX_NOMBRE - 1] = '\0';
	numVariables++;
}

void generadorGuardar(const char *nombreArchivo)
{
	FILE *f = fopen(nombreArchivo, "w");
	if(!f)
	{
		fprintf(stderr, "No se pudo crear el archivo '%s'\n", nombreArchivo);
		exit(1);
	}
	fputs(salida.datos, f);
	fclose(f);
}

void generadorLiberar(void)
{
	free(salida.datos);
	salida.datos = NULL;
	salida.longitud = 0;
	salida.capacidad = 0;
}

char *formatoCadena(const char *fmt, ...)
{
	va_list args1, args2;
	va_start(args1, fmt);
	va_copy(args2, args1);

	int necesario = vsnprintf(NULL, 0, fmt, args1);
	va_end(args1);
	if(necesario < 0)
	{
		fprintf(stderr, "Error al formatear cadena\n");
		exit(1);
	}

	char *resultado = malloc((size_t)necesario + 1);
	if(!resultado)
	{
		fprintf(stderr, "Error de memoria\n");
		exit(1);
	}

	vsnprintf(resultado, (size_t)necesario + 1, fmt, args2);
	va_end(args2);

	return resultado;
}
