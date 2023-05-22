# TP-FINAL-TALLER-1

## YAML Parser a utilizar

[yaml-cpp](https://github.com/jbeder/yaml-cpp)

[Tutorial](https://github.com/jbeder/yaml-cpp/wiki/Tutorial)

## Descripción del cliente

El cliente tiene tres threads, uno para realizar el render y el manejo de eventos, que será el main thread, uno para enviar los comandos que realiza el jugador al servidor y uno para recibir el estado de la partida del servidor. Además, en el cliente hay dos colas, `events_q` para los eventos y `updates_q` para los modelos a renderizar en pantalla.
El cliente primero inicia los dos hilos para la comunicación con el servidor. Cuando el jugador toque una tecla, el manejador de eventos agregará en la cola el evento que se realizó. El `ClientSender` recibe este evento al hacer un pop y se lo envía al servidor. El `ClientReceiver` va a estar esperando a recibir una actualización de la partida que llega desde el servidor, para poder pushearla a la cola `updates_q`. La actualización va a estar dada en una lista. En el hilo main, el render va intentar hacer un pop de la cola `updates_q` sin bloquearse, y si hay alguna actualización de la partida, lo va a renderizar en pantalla.
