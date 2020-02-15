# Sistemas de Gestión de BD
 Base de datos :
  conjunto de datos comunes a un proyecto que son útiles a varias aplicaciones.

 Sistema de Gestión de BD :
  Conjunto de elementos software que se usan para definir, mantener y utilizar la BD.

CRUD :
 acciones básicas de una BD con respecto a los datos (Create, Read, Update, Delete)

## Elementos de una BD

- Hardware
 * BD normal
 * BD distribuida (varios pc's a un servidor)

- Software

- Usuarios
  * Usuario final
  * Programador de aplicaciones
  * Administrador

- Datos
  * Integrados (sin redundancia)
  * Compartidos

  Un dato operativo es aquella información que necesita una cierta organzación para su funcionamiento. Tipos:
   - Item básico: se puede pedir información sobre él
   - Atributos: Características
   - Relaciones: conexiones lógicas entre items.

## Objetivos de un SGBD
 - Independencia de los datos
 - Diseño y utilización orientada al usuario
 - Gestionar los datos de forma centralizada e independientemente de las aplicaciones
 - Los datos no deben estar duplicados a menos que sea necesario (por seguridad o por prevencion de fallos)
 - Consistencia. (El esquema lógico de la BD no debe tener fallos)
 - Datos protegidos contra fallos y mecanismos de recuperación (fiabilidad)
 - Seguridad (No todos los usuarios tienen acceso a todos los datos)

# Ventajas de usar un SGBD (No resumido)

# Concepto de Independencia
 Los datos se organizan independientemente de las aplicaciones que los usen y de los ficheros donde se almacenen.

###### Independencia física
El diseño lógico de la BD debe ser independiente del almacenamiento físico de los datos. Esto permite realizar cambios libremente en la estructura física y que los programadores no se tengan que preocupar de ella.


##### Independencia lógica

Cada aplicación debe tener su propio **esquema lógico general** y acceder sólo a sus datos.

Cada grupo de usuarios tiene su propia **vista de usuario**, que no debería verse modificada al cambiar el esquema lógico general o cambios en la estructura física.
