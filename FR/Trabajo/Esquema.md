# Introduccion

- Definicion( 2 lineas)
- Bases de datos relacional (caract, historia)
- SQL lenguaje (uso en BD) (Yo)

## Ejemplos prevencion ataques

- Implementacion
- Prevencion de riesgos. (Yo)

# Caso tocho

- Explicación ataque (Yo) (Ver si podemos ejecutar)
- Prevencion del caso

# Final

Prepararse ejemplos wikipedia.

// Usar epígrafes como elementos de presentaciones.




# SQL Lenguaje

SQL es un lenguaje de consulta estructurada para sistemas de gestión de bases de datos relacionales. Se basa en álgebra y cálculo relacional para hacer consultas. Es un lenguaje de definicion, manipulación y control de datos.

## ¿Por qué SQL? -> más usado.

SQL se trata del lenguaje por antonomasia de los sistemas de gestión de bases de datos. Se estandarizó en 1986 por ANSI.

Los ataques SQL Injection normalmente comienzan con el atacante introduciendo su código malicioso en un campo de formulario específico en una aplicación

//Poner diapositivas con sentencias básicas. (Esto entraría en ataques?)

Es una herramienta poderosa con la `SELECT data FROM table WHERE email = '$email';` (Incluir esto)

ejemplo@ejemplo.com'

En este caso hay un formulario donde se debe especificar el email.


# Prevencion

## Deteccion (filtro de spam)
  Sencillo. Funciona basicamente como un filtro de spam, donde se leen los caracteres y se analiza si es una sentencia aceptable. Por ejemplo, la presencia de sentencias lógicas (OR, UNIÓN) o de punto y coma. Las sentencias no válidas o que no se consideren seguras no se ejecutarán (Puede dar falsos positivos).

## Escape de caracteres
  Cuando se recibe una entrada por teclado (por ejemplo usuario, contraseña), esa entrada pasa por una función que elimina determinados caracteres como \n o \x00. Por ejemplo cuando se recibe una sentencia con una sola comilla simple (') debe convertirse en doble comilla simple para formar una sentencia básica de SQL.

  Este método es bastante eficaz pero dado a errores, ya que hay que usar el método que filtra en cada entrada de teclado. 
