# TP-FINAL-TALLER-1

## YAML Parser a utilizar

[yaml-cpp](https://github.com/jbeder/yaml-cpp)

[Tutorial](https://github.com/jbeder/yaml-cpp/wiki/Tutorial)

## Sobre el juego

## Controles
Movimiento: ⬅ ➡ ⬆ ⬇
Disparar: S
Recargar: R

Despues de crear una partida hay que cerrar la ventana para que se ejecute el juego

## Software
libsdl2-image 2.6.3
libsdl2-mixer 2.6.3
libsdl2-ttf 2.20.2
libsdl2-dev 2.26.5
libSDL2pp 0.18.1
qt6-base
yaml-cpp
Compilador de C++

## Instalacion
Una vez en la carpeta de la instalacion
mkdir build
cd build
cmake ..
make -j4

## Ejecucion
./server config.yml
./client ip puerto

## Descripción del cliente

El cliente tiene tres threads, uno para realizar el render y el manejo de eventos, que será el main thread, uno para enviar los comandos que realiza el jugador al servidor y uno para recibir el estado de la partida del servidor. Además, en el cliente hay dos colas, `events_q` para los eventos y `updates_q` para los modelos a renderizar en pantalla.
El cliente primero inicia los dos hilos para la comunicación con el servidor. Cuando el jugador toque una tecla, el manejador de eventos agregará en la cola el evento que se realizó. El `ClientSender` recibe este evento al hacer un pop y se lo envía al servidor. El `ClientReceiver` va a estar esperando a recibir una actualización de la partida que llega desde el servidor, para poder pushearla a la cola `updates_q`. La actualización va a estar dada en una lista. En el hilo main, el render va intentar hacer un pop de la cola `updates_q` sin bloquearse, y si hay alguna actualización de la partida, lo va a renderizar en pantalla.

## Descripción del server

El servidor funciona con un thread accepter que esta permamentemente esperando nuevas conexiones, y que crea nuevos objetos `Client` a los cuales un objeto `GameHandler` los ubica en su partida. La partida `Game` tiene un thread Gameloop que popea eventos de `events_q`, calcula el proximo estado de la partida y convierte los elementos de la partida a `PlayerStateReference` ya que es el formato usado por el `Serializer`