# Manual Tecnico

Bienvenidos a **Left4Dead**, El proyecto final de la materia Taller de Programacion, a continuacion habra una guia detallada sobre el funcionamiento del juego, orientado a otros desarrolladores, o personas con conocimientos de programacion.

# Sobre el juego
Left4Dead esta programado en C++, usa SDL para los modelos, mas especificamente el wrapper Sdl2pp y Qt5 para las ventanas del menu

# Estructura

Hay 3 carpetas principales:

 - **resources**: contiene todos los modelos y archivos de audio
 - **qt**: contiene el codigo de las ventanas del menú
 - **common_src**: contiene todo el codigo del proyecto, tiene sueltos los archivos de uso compartido, se divide en:
 - **client**: archivos del cliente
 - **network**: archivos de envio y recepcion de archivos, usados en el server
 - **server**: archivos del server

El ejecutable client excluye en su compilacion los archivos que esten en la carpeta server, de la misma forma que el ejecutable server excluye los que se encuentran el la capreta client

## Diseño

### Niveles:
Los niveles son sencillos, una caja de 1920x145 por donde los jugadores y los zombies pueden moverse libremente.

### Personajes:
Las verdaderas diferencias entre los personajes, son las diferencias entre sus atributos 'arma' el cual contiene a un objeto de tipo **Arma** el cual encapsula el arma de fuego y la granada en un objeto **GrenadeHolder** , las armas tienen atributos como los tiempos entre recargas, disparos o tamaño del cargador, pero mayormente se diferencian por el metodo polimorficos 'create_bullet' los cuales pushean a la cola del game distintos objetos **Bullet**. El arma se asigna en el constructor de PlayerState, y la granada, en el constructor de arma.

### Zombies e IA:
Nuevamente, los zombies son practicamente iguales, salvando las diferencias que hay en sus atributos constantes (velocidad, daño, rango de deteccion, etc. definidos en el game_config), la mayor diferencia que hay entre ellos son los metodos polimorficos **attack** y **calculate_next_movement**.
'attack' define el comportamiento al colisionar con un jugador y calculate_next_movement define el proximo movimiento/accion del zombie, encapsula toda la IA.

## Implementacion Tecnica

Los sprites, audios y demas son cargados antes del comienzo de la partida mediante el objeto **asset_manager** que ademas de las texturas, contiene la cantidad de fotogramas que tiene cada animacion, el largo y el ancho.

Las colisiones se manejan con un objeto **phisics manager** que contiene capas (como Jugador, Zombie o Proyectil aliado) y al detectar una colision entre entidades de capas distintas llama a el metodo **on_colission_detected** de ambas.

Los inputs, el dibujo de texturas y la reproduccion de audio son manejadas con Sdl2pp.


## Comunicacion

El protocolo es simples, con solo 4 metodos: envio y recibo de numeros y strings.
La verdadera magia ocurre en el objeto **Serializer** que usa estos 4 metodos para convertir structs complejos de comunicacion (definidos en **protocol_types.h**) a tira de bytes (tratados como uint8_t) y viceversa

## Dependencias
libsdl2-image 2.6.3
libsdl2-mixer 2.6.3
libsdl2-ttf 2.20.2
libsdl2-dev 2.26.5
libSDL2pp 0.18.1
qt6-base
yaml-cpp
Compilador de C++

## Funcionamiento

El cliente tiene tres threads, uno para realizar el render y el manejo de eventos, que será el main thread, uno para enviar los comandos que realiza el jugador al servidor y uno para recibir el estado de la partida del servidor. Además, en el cliente hay dos colas, `events_q` para los eventos y `updates_q` para los modelos a renderizar en pantalla. Primero inicia los dos hilos para la comunicación con el servidor. Cuando el jugador toque una tecla, el manejador de eventos agregará en la cola el evento que se realizó. El `ClientSender` recibe este evento al hacer un pop y se lo envía al servidor. El `ClientReceiver` va a estar esperando a recibir una actualización de la partida que llega desde el servidor, para poder pushearla a la cola `updates_q`. La actualización va a estar dada en una lista. En el hilo main, el render va intentar hacer un pop de la cola `updates_q` sin bloquearse, y si hay alguna actualización de la partida, lo va a renderizar en pantalla.

El servidor funciona con un thread accepter que esta permamentemente esperando nuevas conexiones, y que crea nuevos objetos `Client` a los cuales un objeto `GameHandler` los ubica en su partida. La partida `Game` tiene un thread Gameloop que popea eventos de `events_q`, calcula el proximo estado de la partida y convierte los elementos de la partida a `PlayerStateReference` y `ZombieStateReference`
