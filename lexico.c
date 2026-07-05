#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexico.h"

char *input;
int pos = 0;
int line = 1;

static void errorLexico(const char *msg) {
	printf("Error léxico (linea %d): %s\n", line, msg);
	exit(1);
}

Token getNextToken() {
	for(;;) {
		char c = input[pos];

		if(c == ' ' || c == '\t' || c == '\r') {
			pos++;
			continue;
		}
		if(c == '\n') {
			line++;
			pos++;
			continue;
		}
		break;
	}

	int lineaInicio = line;
	char c = input[pos];

	if(c == '\0')
		return (Token){TOKEN_EOF, "EOF", lineaInicio};

	if(isalpha((unsigned char)c) || c == '_') {
		int inicio = pos;
		
		while(isalnum((unsigned char)input[pos]) || input[pos] == '_')
			pos++;

		int len = pos - inicio;
		
		if(len >= MAX_LEXEMA)
			len = MAX_LEXEMA - 1;

		Token t;
		t.line = lineaInicio;
		strncpy(t.lexema, input + inicio, len);
		t.lexema[len] = '\0';

		if(strcmp(t.lexema, "begin") == 0)
			t.type = TOKEN_BEGIN;
		else {
			if(strcmp(t.lexema, "end") == 0)
				t.type = TOKEN_END;
			else {
				if(strcmp(t.lexema, "if") == 0)
					t.type = TOKEN_IF;
				else {
					if(strcmp(t.lexema, "else") == 0)
						t.type = TOKEN_ELSE;
					else {
						if(strcmp(t.lexema, "while") == 0)
							t.type = TOKEN_WHILE;
						else {
							if(strcmp(t.lexema, "print") == 0)
								t.type = TOKEN_PRINT;
							else {
								t.type = TOKEN_ID;
							}
						}
					}
				}
			}
		}
		return t;
	}

	if(isdigit((unsigned char)c)) {
		int inicio = pos;

		while(isdigit((unsigned char)input[pos]))
			pos++;

		int len = pos - inicio;
		
		if(len >= MAX_LEXEMA)
			len = MAX_LEXEMA - 1;

		Token t;
		t.type = TOKEN_NUMBER;
		t.line = lineaInicio;
		strncpy(t.lexema, input + inicio, len);
		t.lexema[len] = '\0';

		return t;
	}

	if(c == '"') {
		pos++;
		int inicio = pos;

		while(input[pos] != '"') {
			char cc = input[pos];

			if(cc == '\0' || cc == '\n')
				errorLexico("cadena de texto sin cerrar (falta comilla de cierre)");
			if(!(isalnum((unsigned char)cc) || cc == '_' || cc == ' '))
				errorLexico("caracter no permitido dentro de una cadena de texto");

			pos++;
		}

		int len = pos - inicio;
		
		if(len >= MAX_LEXEMA)
			len = MAX_LEXEMA - 1;

		Token t;
		t.type = TOKEN_STRING;
		t.line = lineaInicio;
		strncpy(t.lexema, input + inicio, len);
		t.lexema[len] = '\0';

		pos++;
		return t;
	}
	
	pos++;
	Token t;
	t.line = lineaInicio;

	switch(c) {
		case '+':
			t.type = TOKEN_PLUS;
			strcpy(t.lexema, "+");
			break;
		case '-':
			t.type = TOKEN_MINUS;
			strcpy(t.lexema, "-");
			break;
		case '*':
			t.type = TOKEN_MULT;
			strcpy(t.lexema, "*");
			break;
		case '/':
			t.type = TOKEN_DIV;
			strcpy(t.lexema, "/");
			break;
		case '(':
			t.type = TOKEN_LPAREN;
			strcpy(t.lexema, "(");
			break;
		case ')':
			t.type = TOKEN_RPAREN;
			strcpy(t.lexema, ")");
			break;
		case '{':
			t.type = TOKEN_LBRACE;
			strcpy(t.lexema, "{");
			break;
		case '}':
			t.type = TOKEN_RBRACE;
			strcpy(t.lexema, "}");
			break;
		case ';':
			t.type = TOKEN_SEMI;
			strcpy(t.lexema, ";");
			break;
		case '<':
			t.type = TOKEN_LT;
			strcpy(t.lexema, "<");
			break;
		case '>':
			t.type = TOKEN_GT;
			strcpy(t.lexema, ">");
			break;
		case '=':
			if(input[pos] == '=') {
				pos++;
				t.type = TOKEN_EQ;
				strcpy(t.lexema, "==");
			}
			else {
				t.type = TOKEN_ASSIGN;
				strcpy(t.lexema, "=");
			}
			break;
		default: {
			char msg[64];
			snprintf(msg, sizeof(msg), "caracter no reconocido '%c'", c);
			errorLexico(msg);
			break;
		}
	}
	return t;
}
