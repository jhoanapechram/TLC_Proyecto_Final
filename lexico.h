#ifndef LEXICO_H
#define LEXICO_H

#define MAX_LEXEMA 256

typedef enum {
	// Palabras reservadas
	TOKEN_BEGIN, TOKEN_END, TOKEN_IF, TOKEN_ELSE, TOKEN_WHILE, TOKEN_PRINT,
	// Categorías con valor variable
	TOKEN_ID, TOKEN_NUMBER, TOKEN_STRING,
	// Operadores aritméticos
	TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULT, TOKEN_DIV, TOKEN_ASSIGN,
	// Operadores relacionales
	TOKEN_LT, TOKEN_GT, TOKEN_EQ,
	// Delimitadores
	TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_LBRACE, TOKEN_RBRACE, TOKEN_SEMI,
	// Fin de archivo
	TOKEN_EOF
} TokenType;

typedef struct {
	TokenType type;
	char lexema[MAX_LEXEMA];
	int line;
} Token;

extern char *input;
extern int pos;
extern int line;

Token getNextToken();

#endif
