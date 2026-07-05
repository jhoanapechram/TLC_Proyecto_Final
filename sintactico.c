#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexico.h"
#include "sintactico.h"
#include "generador.h"

Token current;

void error() {
	printf("Error sintáctico (línea %d): token inesperado '%s'\n",
	       current.line, current.lexema);
	exit(1);
}

void match(TokenType expected) {
	if(current.type == expected)
		current = getNextToken();
	else
		error();
}

char *factor();
char *termino();
char *expresionAritmetica();
char *expresion();

void asignacion();
void sentenciaIf();
void sentenciaWhile();
void sentenciaPrint();
void sentencia();
void listaSentencias();

// Expresiones //
// <factor> -> id | numero | ( <expresion> )

char *factor() {
	char *resultado = NULL;

	if(current.type == TOKEN_ID) {
		resultado = formatoCadena("%s", current.lexema);
		match(TOKEN_ID);
	}
	else {
		if(current.type == TOKEN_NUMBER) {
		resultado = formatoCadena("%s", current.lexema);
		match(TOKEN_NUMBER);
		}
		else {
			if(current.type == TOKEN_LPAREN) {
			match(TOKEN_LPAREN);
			char *interior = expresion();
			match(TOKEN_RPAREN);
			resultado = formatoCadena("(%s)", interior);
			free(interior);
			}
			else {
				error();
			}
		}
	}
	return resultado;
}

// <termino> -> <factor> <termino'>
// <termino'> -> (* | /) <factor> <termino'> | e   (implementado con while)

char *termino() {
	char *izq = factor();
	
	while(current.type == TOKEN_MULT || current.type == TOKEN_DIV) {
		TokenType opType = current.type;
		char op = (opType == TOKEN_MULT) ? '*' : '/';
		match(opType);

		char *der = factor();
		char *nuevo = formatoCadena("%s %c %s", izq, op, der);
		free(izq);
		free(der);
		izq = nuevo;
	}
	return izq;
}

// <expresion> -> <termino> <expresion'>
// <expresion'> -> (+ | -) <termino> <expresion'> | e   (implementado con while)

char *expresionAritmetica() {
	char *izq = termino();

	while(current.type == TOKEN_PLUS || current.type == TOKEN_MINUS) {
		TokenType opType = current.type;
		char op = (opType == TOKEN_PLUS) ? '+' : '-';
		match(opType);

		char *der = termino();
		char *nuevo = formatoCadena("%s %c %s", izq, op, der);
		free(izq);
		free(der);
		izq = nuevo;
	}

	return izq;
}

char *expresion() {
	char *izq = expresionAritmetica();

	if(current.type == TOKEN_LT || current.type == TOKEN_GT || current.type == TOKEN_EQ) {
		const char *op = (current.type == TOKEN_LT) ? "<" :
		                 (current.type == TOKEN_GT) ? ">" : "==";
		match(current.type);

		char *der = expresionAritmetica();
		char *nuevo = formatoCadena("%s %s %s", izq, op, der);
		free(izq);
		free(der);
		izq = nuevo;
	}
	return izq;
}

// Sentencias //
// <asignacion> -> id = <expresion> ;

void asignacion() {
	char nombre[MAX_LEXEMA];
	strncpy(nombre, current.lexema, MAX_LEXEMA - 1);
	nombre[MAX_LEXEMA - 1] = '\0';

	match(TOKEN_ID);
	match(TOKEN_ASSIGN);
	char *expr = expresion();
	match(TOKEN_SEMI);

	generadorIndentar();

	//Se declaran variables automáticamente
	
	if(generadorVariableDeclarada(nombre) == NULL) 	{
		generadorDeclararVariable(nombre);
		generadorAgregar("int ");
	}

	generadorAgregar(nombre);
	generadorAgregar(" = ");
	generadorAgregar(expr);
	generadorAgregar(";\n");

	free(expr);
}

// <if> -> if ( <expresion> ) { <lista_sentencias> } [ else { <lista_sentencias> } ]

void sentenciaIf() {
	match(TOKEN_IF);
	match(TOKEN_LPAREN);
	char *cond = expresion();
	match(TOKEN_RPAREN);
	match(TOKEN_LBRACE);

	generadorIndentar();
	generadorAgregar("if (");
	generadorAgregar(cond);
	generadorAgregar(") {\n");
	free(cond);

	generadorSubirNivel();
	listaSentencias();
	generadorBajarNivel();

	match(TOKEN_RBRACE);
	generadorIndentar();
	generadorAgregar("}\n");

	if(current.type == TOKEN_ELSE) {
		match(TOKEN_ELSE);
		match(TOKEN_LBRACE);

		generadorIndentar();
		generadorAgregar("else {\n");

		generadorSubirNivel();
		listaSentencias();
		generadorBajarNivel();

		match(TOKEN_RBRACE);
		generadorIndentar();
		generadorAgregar("}\n");
	}
}

// <while> -> while ( <expresion> ) { <lista_sentencias> }

void sentenciaWhile() {
	match(TOKEN_WHILE);
	match(TOKEN_LPAREN);
	char *cond = expresion();
	match(TOKEN_RPAREN);
	match(TOKEN_LBRACE);

	generadorIndentar();
	generadorAgregar("while (");
	generadorAgregar(cond);
	generadorAgregar(") {\n");
	free(cond);

	generadorSubirNivel();
	listaSentencias();
	generadorBajarNivel();

	match(TOKEN_RBRACE);
	generadorIndentar();
	generadorAgregar("}\n");
}

// <print> -> print ( <expresion_o_cadena> ) ;

void sentenciaPrint() {
	match(TOKEN_PRINT);
	match(TOKEN_LPAREN);

	generadorIndentar();

	if(current.type == TOKEN_STRING) {
		char texto[MAX_LEXEMA];
		strncpy(texto, current.lexema, MAX_LEXEMA - 1);
		texto[MAX_LEXEMA - 1] = '\0';
		match(TOKEN_STRING);

		generadorAgregar("printf(\"");
		generadorAgregar(texto);
		generadorAgregar("\\n\");\n");
	}
	else {
		char *expr = expresion();
		generadorAgregar("printf(\"%d\\n\", ");
		generadorAgregar(expr);
		generadorAgregar(");\n");
		free(expr);
	}

	match(TOKEN_RPAREN);
	match(TOKEN_SEMI);
}

// <sentencia> -> <asignacion> | <if> | <while> | <print>

void sentencia() {
	switch(current.type) {
		case TOKEN_ID:    asignacion();     break;
		case TOKEN_IF:    sentenciaIf();    break;
		case TOKEN_WHILE: sentenciaWhile(); break;
		case TOKEN_PRINT: sentenciaPrint(); break;
		default:
			error();
	}
}

// <lista_sentencias> -> <sentencia> <lista_sentencias> | e

void listaSentencias() {
	while(current.type == TOKEN_ID    ||
	      current.type == TOKEN_IF    ||
	      current.type == TOKEN_WHILE ||
	      current.type == TOKEN_PRINT)
	{
		sentencia();
	}
}

// Programa //
// <programa> -> begin <lista_sentencias> end

void parse() {
	current = getNextToken();

	match(TOKEN_BEGIN);
	generadorAgregar("#include <stdio.h>\n\nint main(void) {\n");
	listaSentencias();
	match(TOKEN_END);
	generadorIndentar();
	generadorAgregar("return 0;\n}\n");

	if(current.type == TOKEN_EOF)
		printf("El programa es sintácticamente CORRECTO\n");
	else
		error();
}
