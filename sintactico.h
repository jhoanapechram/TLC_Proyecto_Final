#ifndef SINTACTICO_H
#define SINTACTICO_H

/*

Gramática de MiniLang:

<programa>          ::= begin <lista_sentencias> end
<lista_sentencias>  ::= <sentencia> <lista_sentencias> | e
<sentencia>         ::= <asignacion> | <if> | <while> | <print>
<asignacion>        ::= id = <expresion> ;
<if>                ::= if ( <expresion> ) { <lista_sentencias> } [ else { <lista_sentencias> } ]
<while>             ::= while ( <expresion> ) { <lista_sentencias> }
<print>             ::= print ( <expresion_o_cadena> ) ;
<expresion_o_cadena> ::= <expresion> | cadena

Eliminación de recursividad izquierda en las expresiones matemáticas:

<expresion>  -> <termino> <expresion'>
<expresion'> -> (+ | -) <termino> <expresion'> | e

<termino>  -> <factor> <termino'>
<termino'> -> (* | /) <factor> <termino'> | e

<factor> -> id | numero | ( <expresion> )

NOTA: la gramática original también permite
   <factor> -> <expresión> (< | > | ==) <expresión>
pero esa alternativa hace que <factor> derive directamente en <expresión>, es decir, es
RECURSIVIDAD IZQUIERDA y provocaría un bucle infinito. Por eso, esto se resuelve dentro de <expresión>: primero se reconoce la cadena
completa (termino <expresion'>) y, solo si a continuación
aparece <, > o ==, se consume el operador y se reconoce una segunda
cadena. 

En el código, <expresion'> y <termino'> se implementan con un bucle
"while" dentro de expresionAritmetica()/termino() en vez de una función
separada; es la misma idea, solo que de forma iterativa.

*/

void parse();

#endif
