# MiniLang - Compilador (Lexer + Parser + Generador de codigo C)

Analizador lexico y sintactico para el lenguaje **MiniLang**, implementado en C
mediante un parser de descenso recursivo. Si el programa fuente es correcto,
ademas genera automaticamente su traduccion equivalente en lenguaje C.

## Estructura del proyecto

```
.
├── lexico.h        # Tipos de token y prototipo del lexer
├── lexico.c        # Analizador léxico (getNextToken)
├── sintactico.h     # Gramática documentada y prototipo del parser
├── sintactico.c     # Parser de descenso recursivo (análisis + traducción)
├── generador.h      # Prototipos del generador de código C
├── generador.c       # Buffer de salida y tabla de variables
├── main.c             # Punto de entrada: lee el archivo .mini y orquesta todo
├── ejemplo.mini       # Programa de ejemplo en MiniLang
└── README.md
```

## Requisitos

- Un compilador de C compatible con C11 (`gcc` o `clang`).
- En Windows: [MSYS2](https://www.msys2.org/) con el paquete `mingw-w64-ucrt-x86_64-gcc`
  (ver seccion "Instalar gcc en Windows" mas abajo si aun no lo tienes).

Verifica que tu compilador este disponible desde la terminal:

```bash
gcc --version
```

## Compilar

Desde la carpeta del proyecto, compila todos los archivos fuente en un solo
ejecutable:

```bash
gcc -Wall -Wextra -std=c11 -o minilang main.c lexico.c sintactico.c generador.c
```

En Windows (CMD o PowerShell) el resultado es el mismo comando, solo cambia el
nombre del ejecutable si quieres la extension explicita:

```powershell
gcc -Wall -Wextra -std=c11 -o minilang.exe main.c lexico.c sintactico.c generador.c
```

Este comando debe terminar sin imprimir ningun mensaje (compilacion limpia,
sin advertencias).

## Ejecutar

El programa recibe como argumento la ruta al archivo fuente de MiniLang:

```bash
./minilang ejemplo.mini        # Linux / macOS
.\minilang.exe ejemplo.mini    # Windows
```

### Si el programa es correcto

Se imprime en consola:

```
El programa es sintacticamente CORRECTO.
Codigo C generado exitosamente en 'salida.c'
Puedes compilarlo con: gcc salida.c -o salida
```

y se crea el archivo **`salida.c`** en la misma carpeta, con el equivalente
en C del programa MiniLang. Puedes compilarlo y ejecutarlo para comprobar el
resultado:

```bash
gcc -Wall -Wextra -o salida salida.c
./salida
```

### Si hay un error lexico o sintactico

El programa imprime la linea y la causa del error, y termina con codigo de
salida distinto de cero, sin generar `salida.c`. Ejemplo:

```
Error sintactico (linea 3): token inesperado 'print'
```

## Compilar y ejecutar en un solo paso (opcional)

Si prefieres no escribir el comando largo cada vez, puedes crear un
`Makefile` con este contenido:

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
OBJS = main.c lexico.c sintactico.c generador.c

minilang: $(OBJS)
	$(CC) $(CFLAGS) -o minilang $(OBJS)

clean:
	rm -f minilang salida salida.c
```

Y luego simplemente:

```bash
make
./minilang ejemplo.mini
```

## Instalar gcc en Windows (si aun no lo tienes)

1. Descarga e instala [MSYS2](https://www.msys2.org/).
2. Abre la terminal de MSYS2 y actualiza el sistema:
   ```bash
   pacman -Syu
   ```
   (puede pedirte reabrir la terminal a mitad de proceso; repite el comando).
3. Instala el compilador:
   ```bash
   pacman -S mingw-w64-ucrt-x86_64-gcc
   ```
4. Agrega `C:\msys64\ucrt64\bin` a la variable de entorno `PATH` de Windows.
5. Abre una terminal nueva y verifica:
   ```powershell
   gcc --version
   ```

## Gramatica de MiniLang

```
<programa>           ::= begin <lista_sentencias> end
<lista_sentencias>   ::= <sentencia> <lista_sentencias> | e
<sentencia>          ::= <asignacion> | <if> | <while> | <print>
<asignacion>         ::= id = <expresion> ;
<if>                 ::= if ( <expresion> ) { <lista_sentencias> } [ else { <lista_sentencias> } ]
<while>              ::= while ( <expresion> ) { <lista_sentencias> }
<print>              ::= print ( <expresion_o_cadena> ) ;
<expresion_o_cadena> ::= <expresion> | cadena

<expresion>  ::= <termino> { (+ | -) <termino> }
<termino>    ::= <factor> { (* | /) <factor> }
<factor>     ::= id | numero | ( <expresion> )
```

La comparacion relacional (`<`, `>`, `==`) se evalua un nivel por encima de
`<factor>`, dentro de `<expresion>`, para evitar la recursividad izquierda
que tendria la alternativa `<factor> ::= <expresion> (< | > | ==) <expresion>`
tal como aparece en la especificacion original. El detalle de esta decision
esta documentado en el comentario de `sintactico.h`.
