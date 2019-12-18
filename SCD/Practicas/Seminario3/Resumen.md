# Sobre MPI
Es un estándar que define una API para programación paralela mediante paso de mensajes.

- Hay que hacer #include <mpi.h>
- Las funciones devuelven un codigo de error, si no hay error devuelven MPI_SUCCES (cte).
- MPI_Status es una estructura que contiene:
    - status.MPI_SOURCE proceso fuente
    - status.MPI_TAG etiqueta del mensaje
- Usa **constantes** para representar los tipos de datos básicos (MPI_CHAR, MPI_INT, MPI_UNSIGNED_LONG...)

# Compilación y ejecución OpenMPI

`mpicxx -std=c++11 -c ejemplo.cpp`

Se usa **mpicxx** para compilar los archivos, acepta las mismas opciones que el compilador de C++.

`mpirun -np 4 -machinefile maquinas prog1`

Para ejecutar un programa compilado usamos **mpirun**, la opción -np es para seleccionar el número de procesos, y el comando -machinefile es para ejecución distribuida, donde maquinas es el archivo con la configuración.

# Funciones MPI
