#include <stdio.h>
#include <stdlib.h>
#include "lexico.h"
#include "sintactico.h"
#include "generador.h"

int main(int argc, char *argv[]) {
	if(argc != 2) {
		fprintf(stderr, "Uso: %s <archivo.mini>\n", argv[0]);
		return 1;
	}

	FILE *f = fopen(argv[1], "rb");
	if(f == NULL) {
		fprintf(stderr, "No se pudo abrir el archivo '%s'\n", argv[1]);
		return 1;
	}

	fseek(f, 0, SEEK_END);
	long tam = ftell(f);
	rewind(f);

	if(tam < 0) {
		fprintf(stderr, "No se pudo determinar el tamaño del archivo\n");
		fclose(f);
		return 1;
	}

	char *buffer = malloc((size_t)tam + 1);
	if(buffer == NULL) {
		fprintf(stderr, "Error de memoria\n");
		fclose(f);
		return 1;
	}

	size_t leidos = fread(buffer, 1, (size_t)tam, f);
	fclose(f);
	buffer[leidos] = '\0';

	// conectar el buffer leído con las variables globales del lexer
	input = buffer;
	pos = 0;
	line = 1;

	generadorInicializar();

	parse(); // si hay un error léxico o sintáctico, el programa termina aquí

	generadorGuardar("salida.c");
	printf("Código C generado exitosamente en 'salida.c'\n");
	printf("Puedes compilarlo con: gcc salida.c -o salida\n");

	generadorLiberar();
	free(buffer);

	return 0;
}
