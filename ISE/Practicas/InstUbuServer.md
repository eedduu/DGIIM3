# Instalar maquina

- Nueva, 1 GB ram, aprox 10 GB disco DVI, dinamico.

## Añadir imagen y crear otro disco para RAID1

1. Configuración, almacenamiento
2. En Controlador IDE selecciono imagen
3. Controlador SATA, agregar disco duro, crear nuevo y mismas opciones que el primero.
4. Instalar SO, particionado manual.
5. Seleccionar los dos discos duro para crear particiones
6. Crear RAID por software, crear MD, RAID1.
7. Nº disp=2 (los que conforman el RAID1), Nº disp libres=0 (de apoyo/recuperación)

## Crear particiones

1. Crear grupo de volumenes. (De donde cogen espacio las particiones/volumenes logicas)
2. Crear volumenes lógicos. Creamos swap (intercambio) 1GB (mismo tamaño ram), root lo que quede de espacio disponible, home aprox 2 GB y boot aprox 400 MB.

### Cifrar particiones

1. Configurar volumenes cifrados
2. Crear volumenes encriptados
3. Hay que cifrar los que contengan información personal sensible. Hay que cifrar /swap, ya que hay no controlamos lo que hay; /root, ya que contiene a /var, que contiene variables de los programas y puede tener información sensible. Y ciframos también /home pero solo por los loles. No podemos cifrar /boot ya que el sistema no arrancaria.
4. Formatear, seleccionando en menu principal la particion, como SF ext4 en todos menos swap-> area de intercambio. En los ext4 elegir el /... correspondiente. root en raiz ./
5. Finalizar y escribir cambios a disco.
