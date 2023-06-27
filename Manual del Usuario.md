# Manual de Usuario

Bienvenidos a **Left4Dead**, El proyecto final de la materia Taller de Programación. A continuación habrá una guía detallada sobre el funcionamiento del juego, orientado al usuario final.

## Sobre el juego

Este juego de disparos en 2D cuenta con 3 tipos de personajes, 5 especies de zombies y 2 modos de juego.

**Supervivencia**: Modo sin fin, sobrevive tanto tiempo como puedas ante una invación zombie que se vuelve gradualmente más poderosa, ¡lucha por conseguir la mayor puntuación posible!

**Limpiar la Zona**: Lucha contra una cantidad fija de enemigos, ¡limpia la zona lo más rápido posible para conseguir un mejor puntaje!

## Instalación

Este proyecto tiene las siguientes dependencias:

libsdl2-image 2.6.3
libsdl2-mixer 2.6.3
libsdl2-ttf 2.20.2
libsdl2-dev 2.26.5
libSDL2pp 0.18.1
qt6-base
yaml-cpp
Compilador de C++

Una vez instaladas todas y descargado el proyecto en formato .zip o tar.gz, descomprimirlo y adentro de la carpeta abrir el interprete de comandos.
En el interprete ejecutar los siguientes comandos:

```console
mkdir build
cd build
cmake ..
make -j4
```

## Ejecución

Hay 2 ejecutables: servidor y cliente, mientras el cliente es necesario que se ejecute en su computadora para jugar, el servidor puede ser abierto por cualquier usuario. Por defecto el puerto del server es 9091, definido en config.yml

Para levantar el servidor, dentro del intérprete de comandos levantado en la carpeta build, ejecute el siguiente comando:
./server config.yml

Para levantar el cliente, también en la carpeta build, ejecutar:
./client ip puerto

## Menu

Al ejecutar el juego, se da la opción de crear una partida nueva o unirse a una existente con un código de partida con una cantidad determinada de jugadores. Una vez seleccionada la opción deseada, entra a una pantalla de espera hasta que se llene la partida. Una vez que haya la cantidad necesaria de jugadores conectados en la partida, se cierra la ventana y empieza la partida.

## Configuración

Según la versión del juego que tenga, la configuración puede ser hecha modificando constantes en el archivo game_config.h o en el archivo game_config.yaml. Tenga en cuenta que si su version usa game_config.h deberá reinstalar el juego para poder ver los cambios.

## Jugabilidad

### Controles

Movimiento: ⬅ ➡ ⬆ ⬇

Disparar: S

Recargar: R

Granada 1: G

Granada 2: H

### Personajes

IDF: cuenta con un rifle de rafagas de corta distancia, granadas de humo que relentizan o aturden enemigos y granadas explosivas las cuales se pueden cargar para lanzarlas mas lejos, cuidado de no mantenerla en tu mano por demasiado tiempo!

![](./resources/user%20manual%20pics/soldier1.png)

P90: cuenta con una ametralladora multi-proposito y bombardero aereo cuyo poder es devastador

![](./resources/user%20manual%20pics/soldier2.png)

Scout: cuenta con un rifle de larga distancia con balas que atraviesan enemigos y un las mismas granadas que IDF

![](./resources/user%20manual%20pics/soldier3.png)

### Zombies:
Infectado: Zombie comun, corre hacia el jugador mas cercano e intenta hacerle daño.

![](./resources/user%20manual%20pics/zombie.png)

Spear: Zombie armado con una lanza, es mas lento pero hace mas daño.

![](./resources/user%20manual%20pics/spear.png)

Venom: Tiene la capacidad de lanzar proyectiles de larga distancia a traves del mapa y tambien tiene un ataque de corto rango.

![](./resources/user%20manual%20pics/venom.png)

Witch: Ocacionalmente se pone a gritar, atrayendo a mas Zombies, tiene muchisima vida, pero no se mueve ni ataca directamente.

![](./resources/user%20manual%20pics/witch.png)

Jumper: Tiene la capacidad de saltar, tiene baja vida, sin embargo hace mucho daño y es muy rapido.

![](./resources/user%20manual%20pics/jumper.png)


### Lobby

Antes de empezar a jugar, necesita ingresar un nombre y elegir qué personaje usar.

![Lobby](./resources/user%20manual%20pics/lobby.gif)

Una vez hecho este paso, puede crear una partida nueva, o unirse a una partida ya existente. 

Para crear una nueva partida, hay que elegir el modo de juego, darle un nombre a la partida e indicar 
la cantidad de jugadores que podrán conectarse a la partida. Una vez creada la partida, tendrá que esperar a que la partida se llene.

![Partida nueva](./resources/user%20manual%20pics/partida_nueva.gif)

Para unirse a una partida, le aparece una lista de partidas disponibles a unirse y deberá ingresar el código de la partida que quiera.

<img src="./resources/user manual pics/join_game.png" alt="Join Game" width="600px">

Si el código no es valido, aparecerá un mensaje de error.

<img src="./resources/user manual pics/codigo_no_valido.png" alt="Codigo no valido" width="600px">
