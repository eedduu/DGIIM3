# Autómatas Finitos Deterministas
Es una quintupla M=(Q,A,/delta,q,F) donde:
- __M__ es un conjunto finito llamado __conjunto de estados__
- __A__ es el __albafeto de entrada__
- __/delta__ es una aplicación llamada __función de transición__ de QxA->Q
- __q__ es un elemento de _Q_ llamado __estado inicial__
- __F__ es un subconjunto de _Q_ llamado __conjunto de estados finales__

### Proceso de cálculo
Dado un par (q,u) donde q es un estado y a una palabra, usando la función de transición nos devolverá otro estado. El autómata "lee" una traza de palabras junto con un estado inicial, y usando la función de transición llega a un estado que debería ser final (para que esté bien resuelto el problema).

- __Descripción instantánea o configuración__: un par (q,u) que define el estado del autómata
- __Configuración incial__: un par compuesto por el estado incial y una palabra
- __Relación paso de cálculo__: de una configuración sólo se puede pasar a una configuración como máximo
- __Relación de cálculo__: dos configuraciones tienen relación de cálculo si y solo si existe existe una sucesión de configuraciones que permitan llegar de la una a la otra.

# Autómatas Finitos No Deterministas

Es una quintupla M=(Q,A,/delta,q,F) donde:
- __M__ es un conjunto finito llamado __conjunto de estados__
- __A__ es el __albafeto de entrada__
- __/delta__ es una aplicación llamada __función de transición__ de __QxA->P(Q)__ (partes de Q)
- __q__ es un elemento de _Q_ llamado __estado inicial__
- __F__ es un subconjunto de _Q_ llamado __conjunto de estados finales__

### Proceso de cálculo
La diferencia entre los AFD y los no deterministas es que para los no deterministas puede haber estados que para __una entrada__ tengan __dos transiciones__ y puede haber estados que para __una entrada__ no tengan __ninguna transición__.

Se puede transformar un autómata finito no determinista en uno determinista añadiendo un __estado de error__ donde vayan las transiciones no definidad en el autómata anterior.

- __Descripción instantánea o configuración__: un par (q,u) que define el estado del autómata
- __Configuración incial__: un par compuesto por el estado incial y una palabra
- __Relación paso de cálculo__: de una configuración se puede pasar a varias configuraciones distintas en un paso de cálculo, e incluso a ninguna.
- __Relación de cálculo__: dos configuraciones tienen relación de cálculo si y solo si existe existe una sucesión de configuraciones que permitan llegar de la una a la otra.

## Equivalencia
### Determinista->No determinista
Todo lenguaje L aceptado por un autómata determinista es aceptado también por un no determinista.

### No determinista-> Determinista
Dado un AFND se asocia un __autómata determinista asociado__, en el que se completan todas las transiciones no definidas en el no-determinista.

# Automátas Finitos No Determinista con Transiciones Nulas

Es una quintupla M=(Q,A,/delta,q,F) donde:
- __M__ es un conjunto finito llamado __conjunto de estados__
- __A__ es el __albafeto de entrada__
- __/delta__ es una aplicación llamada __función de transición__ de __Qx(AU{ε})->P(Q)__ (partes de Q)
- __q__ es un elemento de _Q_ llamado __estado inicial__
- __F__ es un subconjunto de _Q_ llamado __conjunto de estados finales__

##TODO
- mirar ejemplos AFD
- lenguajes aceptados por los autómatas
- pag 31
