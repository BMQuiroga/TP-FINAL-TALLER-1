# Código (2/5)
## Cliente
* **(P2)** Hay paths hardcodeados de archivos. Revisar sección instalación donde se sugiere el uso de macros
definidas en CMake para evitar este hardcode
* **(P3)** Los assests a agregar podrian venir del YAML, para no tener que recompilar todo si querés agregar o cambiar
un asset
* **(P3)** El main del cliente tiene mucho codigo suelto, faltan clases RAII para asegurarte que los hilos se joineen
debidamente sin importar excepciones que se lancen
* **(P4)** Hay mucho número mágico en el cliente que no se entiende que significan.

## Server
* **(P2)** La clase Client no respeta RAII, si algo lanza excepción los hilos que tiene no los va a joinear nadie
* **(P4)** ProtectedMatchCounter podria haber sido un simple `std::atomic<int>`
* **(P2)** En vez de una lista de partidas, podrías tener un `std::map<string, Game>` y usar el nombre de la partida como clave de la busqueda. No tendrías que hacer una busqueda lineal de la partida (O(n)) y con el diseño actual de loxby no 
tendrías que mostrar ningún código, sino que indexás directamente por el nombre de  partida.
* **(P0)** Falta una clase de configuración del juego para tunearlo sin tener que recompilar

* **(P1)** Imagenes a la docu

# UX/Jugabilidad (2/5)
* **(P0)** El personaje avanza bastante lento y atenta negativamente contra la jugabilidad. Hay que tunear
la configuración del server para que el movimiento sea mas fluído y rápido.

* **(P1)** La configuración en el cliente de la animación del movimiento debería retocarse también para que se ajuste
al movimiento. Podrían agregar un delay entre los cambios de distintos frames para que se ajuste la animación al
movimiento que hace (es decir, que entre salto de frame, sucedan X segundos ajustable para modificarlo en runtime)

* **(P0)** Hay que brindar información en el lobby para poder joinearse a una sala existente, ya sea que el que crea partida 
reciba el codigo para pasarselós a sus amigos, o que puedas listar partidas creadas. El cierre de la ventana debería ser automático. 

# Robustez / Valgrind (todo prioridad P0) (2/5)
* Tanto cliente como servidor siempre cierran con un sigsegv por excepciones no catcheadas
* El server tiene muchas variables sin inicializar, y memory leaks (se adjuntas logs de valgrind)
* En el cliente se corrió valgrind usando QT, y los errores de memoria encontrados parecieran ser todos de agentes externos (falsos positivos de SDL y/o QT). Esto no quiere decir de que no haya leaks, probablemente cuando arreglen el SIGSEGV se detecten leaks otros leaks de memoria.
* Si cierro el lobby del cliente sin haberme joineado o creado partida, abre una ventana de juego que es injugable. Hay que chequear si el jugador se joineó/creó una partida para saber si tenés que abrir la ventana de SDL o no


# Instalación (3/5)
* El juego se puede compilar sin problemas.
* **(P3)** No hay instalador. Los paths a los assets debería ser variable en función de si estás ejecutandoló en modo desarrollo (paths locales) o si estás en producción (`/usr/bin/` para los ejecutables, `/var/left4dead/` para las configuraciones). Pueden usar CMake Para definir una macro de compilación (ver `add_compile_definitions`) y en donde quieran
cargar assets (o archivo de configuración) hacer concatenar strings

```cpp
std::string filepath = CONFIG_BASE_PATH "/server/config.yaml"
```


# Features (2/5)
Faltan mostrar muchos feature, ponganlé pilas a los prioritarios
## Zombies

### Jumper

- [ ] **(P0)** salto tiene animación previa de aviso


## Jugador
- [x] Munición infinita
- [x] Tiempo de recarga 
- [ ] **(P0)** Todos salvo P90 pueden lanzar granadas explosivas y de humo


## Granadas
- [ ] **(P0)** Daña a jugadores aliados también
- [ ] **(P2)** humo: no producen daño. Los infectados quedan aturdidos y dejan de correr y saltar, siguen caminando

## Bombardeo aereo
- [ ] **(P2)** Solo P90 puede invocarlo
- [ ] **(P2)** caen granadas por todo el escenario salvo la zona cercana al jugador
- [ ] **(P2)** tiempo de espera entre bombardeos mayor

## Vida
- [x] Los jugadores tienen vida.
- [x] Los zombies tienen vida.
- [ ] **(P3)** Cuando llega a cero son derribados
- [ ] **(P3)** Los jugadores derribados pueden recibir asistencia por otro jugador y seguir jugando
- [ ] **(P3)** el jugador recuperado se levanta con mitad de vida
- [ ] **(P3)** a los 3 derribos muere inmediatamente.
- [ ] Si jugador derribado no es asistido, muere

## Ataque y colisiones
- [x] soldados y zombies pueden colisionar
- [ ] **(P0)** hay un hitbox para detectar zombies en el eje y
	* No pareciera haber un hitbox, intenté dispararle a un zombie que estaba en el maximo del eje y, yo estando casi en la zona mas baja del eje y, y le hice daño.


## Escenario
- [ ] **(P4)** los crateres son zona por la que no podés caminar
- [ ] **(P1)** Te podés mover a los costados
- [ ] **(P3)** Hay una ilusion de paralaje

## Modos de juego
### Clear the zone
- [x] **(P0)** cantidad fija de zombies. Jugador gana cuando elimina a todos

### Survival
- [ ] Cantidad infinita de zombies.
- [ ] **(P3)** cada vez se hacen mas fuertes, resistentes y veloces
- [ ] **(P0)** termina cuando jugadores mueren

## Fin de partida
- [ ] **(P0)** se muestra estadisticas de juego (cantidad de eliminados, balas disparadas, tiempo de partida)
- [ ] **(P4)** En modo survival las estadisticas se guardan y se muestra un ranking historico (1 ranking por cada estadistica) que se muestra al final del modo

## Interfaz gráfica
- [x] Se muestra la vida de cada soldado
- [x] Se muestra las rafagas restantes antes de recargar
- [ ] **(P1)**Se muestra tiempo restante para lanzar granada
- [ ] **(P3)** Se muestra balas disparadas
- [ ] **(P1)** Se muestra infectados eliminados
- [ ] **(P1)** Se muestra tiempo transcurrido
## Controles
- [x] se controla via teclado
- [x] Recarga manual (mediante botón de recarga)
- [ ] **(P1)** distancia de lanzado de granada depende de cuanto tiempo se mantiene presionado la tecla
- [ ] **(P1)** Si mantenés mucho tiempo presionado la tecla de lanzar granada, explota en la mano


## sonidos
- [x] Cuando hay disparo
- [ ] **(P1)** Cuando hay expĺosion
- [ ] **(P1)** Cuando hay una recarga

## Musica
- **(P0)** No hay
## Animaciones
- [x] Hay animaciones, tal vez estaría bueno ajustar el delay entre saltos de frame de cada animación para que parezca mas natural el movimiento.

## Configuraciones
- [ ] **(P0)** No veo configuraciones en el server, lo que va a dificultar mucho el tuneo del movimiento.

## Cheat
- [ ] **(P4)** No hay cheats

## Tests
- [ ] **(P1)** No hay tests

# Cliente - Servidor (1/5)
* **(P0)** No se puede ver una partida con varios jugadores. No tengo forma de saber con qué codigo se creó una partida para poder joinearme y tener una partida de dos jugadores.

# Extras
No hay