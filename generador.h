#ifndef GENERADOR_H
#define GENERADOR_H

/* Modulo encargado de acumular el codigo C equivalente mientras el parser
 * reconoce la gramatica, y de escribirlo al final en un archivo .c */

void  generadorInicializar(void);
void  generadorAgregar(const char *texto);
void  generadorIndentar(void);
void  generadorSubirNivel(void);
void  generadorBajarNivel(void);
int   generadorVariableDeclarada(const char *nombre);
void  generadorDeclararVariable(const char *nombre);
void  generadorGuardar(const char *nombreArchivo);
void  generadorLiberar(void);

/* Utilidad tipo "sprintf" que reserva memoria automaticamente.
 * El puntero devuelto debe liberarse con free(). */
char *formatoCadena(const char *fmt, ...);

#endif
