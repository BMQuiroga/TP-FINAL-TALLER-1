# Manual del Proyecto

Bienvenidos a **Left4Dead**, El proyecto final de la materia Taller de Programacion, a continuacion habra una guia sobre el proyectio, orientado a correctores.

# Equipo
Germán Gonzalo Escandar Obadía - 105250
Mariana Galdo Martinez - 105658
Bruno Martin Quiroga - 107788

# Cronograma
16/5 -> 23/5 Objetivo: tener un servidor unipartida que permita que 2 jugadores se muevan.

No cumplido, faltaron gameloops de ambos lados.

23/5 -> 30/5 Objetivo: mismo de la semana pasada.

No cumplido, crashea en el 2do jugador, temas de protocolo.

30/5 -> 6/6 Objetivo: zombies con IA, disparos, daño, barra de vida, animaciones, lobby, partidas.

Cumplido.

6/6 -> 13/6 Entrega1: Lobby, 2 clientes en una partida con movimiento y zombies que atacan, disparos, readme de instalacion, mini documentacion.

Cumplido.

13/6 -> Entrega1: resolver bug en partidas multi-cliente, multiples eventos manejados por gameloop tanto en cliente como en servidor.

Cumplido, entregado el dia 15.

A partir de este momento, con las correcciones, se nos dio la tarea de que el juego tenga la mayor cantidad de features posibles el dia de la entrega.


# Tareas de cada miembro del grupo

**German**: implementacion de logica de servidor (protocolo, hilos, lectura de configuracion) y del juego (GameLoop, entidades, detección de colisiones), testing con GoogleTest

**Mariana**: Hilos del cliente de sender y receiver, protocolo de mensajes de lobby, vistas y diseño del lobby, encapsulan todo el uso de QT.

**Bruno**: Clases asset, Image, asset manager, client renderer, del lado del cliente, encapsulan todo el uso de SDL. Clases Zombie, Bullet, VenomProjectile, Arma, GrenadeHolder del lado del server, incluye la IA de los zombies y el funcionamiento de las armas.



