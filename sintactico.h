#ifndef SINTACTICO_H
#define SINTACTICO_H

/*

Gramatica de MiniLang:

<programa>          ::= begin <lista_sentencias> end
<lista_sentencias>  ::= <sentencia> <lista_sentencias> | e
<sentencia>         ::= <asignacion> | <if> | <while> | <print>
<asignacion>        ::= id = <expresion> ;
<if>                ::= if ( <expresion> ) { <lista_sentencias> } [ else { <lista_sentencias> } ]
<while>             ::= while ( <expresion> ) { <lista_sentencias> }
<print>             ::= print ( <expresion_o_cadena> ) ;
<expresion_o_cadena> ::= <expresion> | cadena

Eliminacion de recursividad izquierda en las expresiones matematicas
(mismo procedimiento E/E', T/T' visto en clase, aplicado aqui):

<expresion>  -> <termino> <expresion'>
<expresion'> -> (+ | -) <termino> <expresion'> | e

<termino>  -> <factor> <termino'>
<termino'> -> (* | /) <factor> <termino'> | e

<factor> -> id | numero | ( <expresion> )

NOTA: la gramatica original tambien permite
   <factor> -> <expresion> (< | > | ==) <expresion>
pero esa alternativa hace que <factor> derive directamente en <expresion>
(factor -> expresion -> termino -> factor -> ...), es decir, es
RECURSIVIDAD IZQUIERDA y provocaria un bucle infinito en un parser de
descenso recursivo. Por eso, la comparacion relacional se resuelve un
nivel mas arriba, dentro de <expresion>: primero se reconoce la cadena
aritmetica completa (termino <expresion'>) y, solo si a continuacion
aparece <, > o ==, se consume el operador y se reconoce una segunda
cadena aritmetica. Esto conserva el mismo uso que le da la gramatica
(comparaciones en if/while) sin la recursividad izquierda.

En el codigo, <expresion'> y <termino'> se implementan con un bucle
"while" dentro de expresionAritmetica()/termino() en vez de una funcion
separada; es la misma idea, solo que de forma iterativa.

*/

void parse();

#endif
