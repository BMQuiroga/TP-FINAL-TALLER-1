# Nota: 7

# Documentación (5/5)
* Al manual de usuario le vendría bien un mini apartado para ver como instalar las dependencias (recuerden que es un manual
para el usuario, capaz no sepan instalar estas cosas en linux), faltan algunas dependencias también como `CMake`
* Muy descriptivo manual de usuario, me gusta que se hayan tomado el tiempo de hacer unos videos para mostrarlo y hacer
mas gráfica la expliación.
* Al manual de usuario le vendría bien capturas explicando el HUD del juego
* Al manual tecnico le faltaría explicar de forma separada el diseño de hilos del cliente y del servidor, costó entender un poco esa parte.


# UX/Jugabilidad (3/5)
* Parece que la partida arranca desde antes cuando ocurre el conteo de ingreso, y los zombies ya se encuentran disparando al jugador.
Me ha pasado de entrar a la partida y morir al instante. Que el modelo tenga le mismo delay, o no spawneen los zombies inmediatamente.
* La animación se ve rara, muy acelerada. Faltaría controlar por archivo que la animación tarde un poco mas en ir cambiando de frames
* Está bueno como renderizan el cooldown para usar granadas o el ataque aereo
* El hitbox necesita un tuneo, tengo que estar mas abajo de los zombies para que el disparo sea efectivo. El jumper es el del hitbox mas raro, puedo estar pegado a el y no poder dispararle nunca.
* El layout de teclas es medio incómodo. Entiendo que los eligieron por ser las siglas de las acciones (shoot, recharge, granade, "humo" (spanglish detected)), pero es incómodo para el jugador tener ese layout de teclas. Podrían por ejemplo usar las teclas `Q`, `W`, `R`, `T`
al estilo League of legends para tener las teclas cerca y de facil acceso.
* El código de error de la ventana de lobby podrían pintarlo de rojo para enfatizar de que sucedió un error.
* El personaje avanza bastante lento y atenta negativamente contra la jugabilidad. Hay que tunear
la configuración del server para que el movimiento sea mas fluído y rápido.
* La configuración en el cliente de la animación del movimiento debería retocarse también para que se ajuste
al movimiento. Podrían agregar un delay entre los cambios de distintos frames para que se ajuste la animación al
movimiento que hace (es decir, que entre salto de frame, sucedan X segundos ajustable para modificarlo en runtime)

# Robustez / Valgrind(3/5)
* Pareciera no hay limpieza de partidas finalizadas. El cliente que se conecta puede ver una partida finalizada e intentar joinearse a esa partida.
* Un jugador puede joinearse a una partida terminada.
* El server tiene muchos mensajes de llamados a socket pasandolé bytes no inicializados.
```
==17986== Syscall param write(buf) points to uninitialised byte(s)
==17986==    at 0x506DA6F: __libc_write (write.c:26)
==17986==    by 0x506DA6F: write (write.c:24)
==17986==    by 0x4FE3F6C: _IO_file_write@@GLIBC_2.2.5 (fileops.c:1180)
==17986==    by 0x4FE5A60: new_do_write (fileops.c:448)
==17986==    by 0x4FE5A60: _IO_new_do_write (fileops.c:425)
==17986==    by 0x4FE5A60: _IO_do_write@@GLIBC_2.2.5 (fileops.c:422)
==17986==    by 0x4FE5F42: _IO_file_overflow@@GLIBC_2.2.5 (fileops.c:783)
==17986==    by 0x4FE0F49: putc (putc.c:31)
==17986==    by 0x4D62239: std::ostream::put(char) (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17986==    by 0x4D627E2: std::basic_ostream<char, std::char_traits<char> >& std::endl<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&) (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17986==    by 0x16833D: Serializer::serialize(LobbyGamesListsStateResponse const&) (serializer.cpp:194)
==17986==    by 0x16F7D9: Client::handle_request(ProtocolRequest&) (client.cpp:73)
==17986==    by 0x16F06D: Client::Client(Socket&&, ServerProtocol&, GameHandler&)::{lambda(ProtocolRequest&)#1}::operator()(ProtocolRequest&) const (client.cpp:24)
==17986==    by 0x16FFE6: void std::__invoke_impl<void, Client::Client(Socket&&, ServerProtocol&, GameHandler&)::{lambda(ProtocolRequest&)#1}&, ProtocolRequest&>(std::__invoke_other, Client::Client(Socket&&, ServerProtocol&, GameHandler&)::{lambda(ProtocolRequest&)#1}&, ProtocolRequest&) (invoke.h:61)
==17986==    by 0x16FEB1: std::enable_if<is_invocable_r_v<void, Client::Client(Socket&&, ServerProtocol&, GameHandler&)::{lambda(ProtocolRequest&)#1}&, ProtocolRequest&>, void>::type std::__invoke_r<void, Client::Client(Socket&&, ServerProtocol&, GameHandler&)::{lambda(ProtocolRequest&)#1}&, ProtocolRequest&>(Client::Client(Socket&&, ServerProtocol&, GameHandler&)::{lambda(ProtocolRequest&)#1}&, ProtocolRequest&) (invoke.h:111)

```
Y de conditional jumps que dependen de variables no inicializadas

```
==17986== Conditional jump or move depends on uninitialised value(s)
==17986==    at 0x4FE47B7: _IO_new_file_xsputn (fileops.c:1218)
==17986==    by 0x4FE47B7: _IO_file_xsputn@@GLIBC_2.2.5 (fileops.c:1196)
==17986==    by 0x4FD9056: fwrite (iofwrite.c:39)
==17986==    by 0x4D62B34: std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long) (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17986==    by 0x16832B: Serializer::serialize(LobbyGamesListsStateResponse const&) (serializer.cpp:194)
==17986==    by 0x16F7D9: Client::handle_request(ProtocolRequest&) (client.cpp:73)
==17986==    by 0x16F06D: Client::Client(Socket&&, ServerProtocol&, GameHandler&)::{lambda(ProtocolRequest&)#1}::operator()(ProtocolRequest&) const (client.cpp:24)
==17986==    by 0x16FFE6: void std::__invoke_impl<void, Client::Client(Socket&&, ServerProtocol&, GameHandler&)::{lambda(ProtocolRequest&)#1}&, ProtocolRequest&>(std::__invoke_other, Client::Client(Socket&&, ServerProtocol&, GameHandler&)::{lambda(ProtocolRequest&)#1}&, ProtocolRequest&) (invoke.h:61)
==17986==    by 0x16FEB1: std::enable_if<is_invocable_r_v<void, Client::Client(Socket&&, ServerProtocol&, GameHandler&)::{lambda(ProtocolRequest&)#1}&, ProtocolRequest&>, void>::type std::__invoke_r<void, Client::Client(Socket&&, ServerProtocol&, GameHandler&)::{lambda(ProtocolRequest&)#1}&, ProtocolRequest&>(Client::Client(Socket&&, ServerProtocol&, GameHandler&)::{lambda(ProtocolRequest&)#1}&, ProtocolRequest&) (invoke.h:111)
==17986==    by 0x16FD84: std::_Function_handler<void (ProtocolRequest&), Client::Client(Socket&&, ServerProtocol&, GameHandler&)::{lambda(ProtocolRequest&)#1}>::_M_invoke(std::_Any_data const&, ProtocolRequest&) (std_function.h:290)
==17986==    by 0x175C12: std::function<void (ProtocolRequest&)>::operator()(ProtocolRequest&) const (std_function.h:590)
==17986==    by 0x175769: Receiver<ProtocolResponse, ProtocolRequest>::run() (receiver.h:60)
==17986==    by 0x170537: Thread::main() (thread.h:26)
==17986==  Uninitialised value was created by a stack allocation
==17986==    at 0x183640: GameHandler::get_refs()::{lambda(std::vector<Game, std::allocator<Game> >&)#1}::operator()(std::vector<Game, std::allocator<Game> >&) const (gamehandler.cpp:31)
==17986==
```



# Performance (5/5)

# Instalación (3/5)
* No hay instalador pero no hubo ningún problema al compilar el juego
* El logo del lobby no se muestra porque están haciendo un shadowing de la variable `CLIENT_SOURCES`, primero la usan para cargar el 
archivo `qrc` de QT, y luego pisan el valor con la función `FILE(GLOB ...)` posterior. Cambiando el nombre de la variable en `qt5_add_resources`
se soluciona.

```
qt5_add_resources(QRC_FILES qt/logo.qrc)

file(GLOB CLIENT_SOURCES
    common_src/*.h
    common_src/*.cpp
    common_src/network/*.h
    common_src/network/*.cpp
    common_src/client/*.h
    common_src/client/*.cpp
    qt/*.h
    qt/*.cpp
)

add_executable (client
  ${CLIENT_SOURCES}
  ${QRC_FILES}
  ${UIS_HEADERS}
  ${QT_SOURCES}
)
```

# Features (4/5)
## Zombies
### Infectados
- [x] Caminan o permanecen quietos si no son molestados
- [x] Cuando el jugador esté en el rango de vision los persigue 
- [x] golpe con mordidas o golpes

### Jumper
- [x] Salta sobre sus victimas
- [x] produce daño cuando cae sobre las victimas
- [ ] salto tiene animación previa de aviso

### Witch
- [x] ocasionalmente grita para atraer infectados

### Spear
- [x] usan armas como lanzas

### Venom
- [x] ataque a distancia
- [x] daña a todo jugador que esté en contacto con el ataque

## Jugador
- [x] Munición infinita
- [x] Tiempo de recarga 
- [x] Todos salvo P90 pueden lanzar granadas explosivas y de humo


### IDF
- [x] rafaga de 20 balas 
- [x] daño a corta distancia alto
- [x] daño menor a larga distancia
- [x] Recarga cada 50 rafagas

### P90

- [x] rafagas de 10 balas
- [ ] daño no reducido a largas distancias
- [x] recarga cada 30 rafagas

### Scout
- [x] Rifle de 1 bala
- [x] Atraviesa infectados
- [x] Daño se reduce por cada impacto entre infectados
- [x] recarga cada 20 bbalas


## Granadas
- [x] Son infinitas
- [x] hay un tiempo de espera entre lanzamientos
- [x] explosiva: daña enemigos en la zona. Daña a jugadores aliados también
- [x] humo: no producen daño. Los infectados quedan aturdidos y dejan de correr y saltar, siguen caminando

## Bombardeo aereo
- [x] Solo P90 puede invocarlo
- [x] caen granadas por todo el escenario salvo la zona cercana al jugador
- [x] tiempo de espera entre bombardeos mayor

## Vida
- [x] Los jugadores tienen vida.
- [x] Los zombies tienen vida.
- [x] Cuando llega a cero son derribados
- [x] Los jugadores derribados pueden recibir asistencia por otro jugador y seguir jugando
- [x] el jugador recuperado se levanta con mitad de vida
- [ ] a los 3 derribos muere inmediatamente.
- [ ] Si jugador derribado no es asistido, muere

## Ataque y colisiones
- [x] soldados y zombies pueden colisionar
- [x] hay un hitbox para detectar zombies en el eje y

## Escenario
- [ ] los crateres son zona por la que no podés caminar
- [x] hay una camara que sigue al jugador
- [x] Te podés mover a los costados
- [x] Hay una ilusion de paralaje

## Modos de juego
### Clear the zone
- [x] cantidad fija de zombies. Jugador gana cuando elimina a todos

### Survival
- [x] Cantidad infinita de zombies.
- [x] cada vez se hacen mas fuertes, resistentes y veloces
- [x] termina cuando jugadores mueren

## Fin de partida
- [ ] se muestra estadisticas de juego (cantidad de eliminados, balas disparadas, tiempo de partida)
- [ ] En modo survival las estadisticas se guardan y se muestra un ranking historico (1 ranking por cada estadistica) que se muestra al final del modo

## Interfaz gráfica
- [x] Se muestra la vida de cada soldado
- [x] Se muestra las rafagas restantes antes de recargar
- [x] Se muestra tiempo restante para lanzar granada
- [x] Se muestra balas disparadas
- [x] Se muestra infectados eliminados
- [x] Se muestra tiempo transcurrido
## Controles
- [x] se controla via teclado
- [x] Recarga automatica (cuando te quedás sin balas)
- [x] Recarga manual (mediante botón de recarga)
- [x] distancia de lanzado de granada depende de cuanto tiempo se mantiene presionado la tecla
- [x] Si mantenés mucho tiempo presionado la tecla de lanzar granada, explota en la mano


## Sonidos
- [x] Cuando hay disparo
- [x] Cuando hay expĺosion
- [x] Cuando hay una recarga
- [ ] Musica

## Animaciones
- [x] Hay animaciones pero no se ven fluidas.

## Configuraciones
- [x] Hay configuraciones

## Cheat
- [ ] No hay cheats

## Tests
- [x] podria haber una mayor cobertura de tests, pero hay.

# Cliente - Servidor (3/5)
* Arrancás una partida, luego abrís un cliente para partida de 2, desde el momento en el que el cliente 2 se encuentra en el lobby
esperando a que se sume gente, el cliente 1 que ya estaba en partida no puede seguir jugando. Adjunto video
en carpeta correcciones, junto con el archivo de yml usado (se modificó el daño de los zombies
y la vida del jugador para evitar me maten antes de arrancar la partida)
* Si armás una partida de 2 jugadores, un cliente no puede mover a su jugador.

# Código (3/5)
## Cliente
* Los paths a los assets son relativos a una supuesta carpeta build, si usara los paths default de QT (que definen la carpeta build
en otro lado) este codigo no anda. Se podría haber definido un path que varie segun quien compila y para qué (producción o desarrollo)
usando macros de cmake.
* Los assests a agregar podrian venir del YAML, para no tener que recompilar todo si querés agregar o cambiar
un asset
* El main del cliente tiene mucho codigo suelto, faltan clases RAII para asegurarte que los hilos se joineen
debidamente sin importar excepciones que se lancen
* El join de los hilos receivers y sender se deberían hacer en el destructor de la clase para respetar RAII

## Server
* En vez de una lista de partidas, podrías tener un `std::map<string, Game>` y usar el nombre de la partida como clave de la busqueda. No tendrías que hacer una busqueda lineal de la partida (O(n)) y con el diseño actual de loxby no 
tendrías que mostrar ningún código, sino que indexás directamente por el nombre de  partida.
