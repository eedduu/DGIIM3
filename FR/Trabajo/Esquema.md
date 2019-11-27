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

## Historia

## Defi

SQL que significa lenguaje de consulta estructurada, es un lenguaje de dominio específico para sistemas de gestión de bases de datos que usan el modelo relacional. Se basa en álgebra y cálculo relacional para hacer consultas. Fue dieñado en 1974 ppor Dondald Chamberlin. Ya desde sus inicios tuvo gran aceptación pero fue en 1986-1987 cuando ANSI e ISO lo reconocieron como el lenguaje estándar para SGBD.

- Select: cuando hacemos una consulta.

  ## ¿Por qué SQL? -> más usado.

# Prevencion

Para prevenir los problemas, tenemos que tener claro como procesa SQL las entradas. Hay una sentencia escrita, a la que se le añade la respectiva entrada por teclado que haga el usuario, para añadirla a la sentencia y ejecutarla.

## Deteccion (filtro de spam)

Sencillo. Funciona basicamente como un filtro de spam, donde se leen los caracteres y se analiza si es una sentencia aceptable. Por ejemplo, la presencia de sentencias lógicas (OR, UNIÓN) o de punto y coma. Las sentencias no válidas o que no se consideren seguras no se ejecutarán (Puede dar falsos positivos).

## Escape de caracteres

Cuando se recibe una entrada por teclado (por ejemplo usuario, contraseña), esa entrada pasa por una función que elimina determinados caracteres como \n o \x00\. Por ejemplo cuando se recibe una sentencia con una sola comilla simple (') debe convertirse en doble comilla simple para formar una sentencia básica de SQL.

Este método es bastante eficaz pero dado a errores, ya que hay que usar el método que filtra en cada entrada de teclado.

## Chequeo de patrones en valores

Se programa la BD de manera que los valores tengan que cumplir unas determinadas propiedades(fecha, user, alfanumerico) y si no se cumplen no se comprueban. Esto evita, por ejemplo, que se lea una sentencia de código en un lugar donde se esperaba un valor numérico.

## Permisos

Se (debería) limita el uso de ciertas sentencias a solo un grupo de usuarios. Por ejemplo, en Microsoft SQL Server podemos usar esta sentencia para limitar quien puede usar el comando SELECT.

```
deny slect on sys.sysobjects to webdatabaselogon
deny select on sys.objects to webdatabaselogon
```

## Sentencias parametrizadas

En esta solución lo que hacemos es definir diferentes tipos en la base de datos, y en cada valor que introduce el usuario se espera uno de estos. `INSERT INTO products (name, price) VALUES (?, ?);` Primero se guarda la sentencia y se envía al SGBD, donde se pre-ejecuta sin guardar el resultado. Una vez se introducen los datos, se ejecuta la sentencia y se obtiene el resultado, se puede ejecutar tantas veces como se quiera. Si se introduce una inyección SQL, se ejecutará la operación tratando la sentencia como una cadena de strings, por lo que devolverá un error (si no era el valor que se esperaba) o se producirá un valor erroneo, o un tanto aleatorio. Muy segura pero requiere al desarrollador de tipificar toda la base de datos.

### Forzado a nivel de código

Esta solución consiste en utilizar un lenguaje dirigido a objetos para crear una especie de base de datos virtual. Donde a través de clases y sus métodos se representa las entidades y relaciones de la base de datos.
