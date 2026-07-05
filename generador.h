#ifndef GENERADOR_H
#define GENERADOR_H

void  generadorInicializar(void);
void  generadorAgregar(const char *texto);
void  generadorIndentar(void);
void  generadorSubirNivel(void);
void  generadorBajarNivel(void);
int   generadorVariableDeclarada(const char *nombre);
void  generadorDeclararVariable(const char *nombre);
void  generadorGuardar(const char *nombreArchivo);
void  generadorLiberar(void);

char *formatoCadena(const char *fmt, ...);

#endif
