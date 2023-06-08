cmake_minimum_required(VERSION 2.8)

project(tp-final)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_COMPILER_ID GNU)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -D_POSIX_C_SOURCE=200809L")

# Paquetes necesarios para la compilacion
# Threads
set(CMAKE_THREAD_PREFER_PTHREAD ON)                                                                                                                                                                                                           
set(THREADS_PREFER_PTHREAD_FLAG ON)                                                                                                                                                                                                           
find_package(Threads REQUIRED) 
# SDL2pp
find_package(SDL2pp REQUIRED)

# Find the QtWidgets library
find_package(Qt5Widgets COMPONENTS Widgets REQUIRED)

message("Agregando '${PROJECT_BINARY_DIR}'/include como directorio de includes")
include_directories(${PROJECT_SOURCE_DIR}/include)

# Agrego la carpeta con los headers autogenerados a los includes
message("Agregando '${PROJECT_BINARY_DIR}' como directorio de includes")
include_directories(${PROJECT_BINARY_DIR})

# Esta macro toma cada uno los parámetros y ejecuta *uic* sobre ellos
# La aplicacion uic genera headers con la definición de los widgets
# a partir de un archivo .ui
# Los nombres de los headers generados se guardan en la variable pasada
# como primer parámetro.
# No, no los genera con la carpeta como prefijo.
qt5_wrap_ui(UIS_HEADERS
  qt/Lobby.ui
  qt/gameoption.ui
  qt/joingame.ui
  qt/numberplayers.ui
  qt/mainwindow.ui
)

# Habilito el automoc para poder hacer autoconnect de señales
set(CMAKE_AUTOMOC on)
# Habilito autorcc para manejar automaticamente los QResources
set(CMAKE_AUTORCC ON)

file(GLOB SOURCES
    ./common_src/*.h
    ./common_src/*.cpp
    ./common_src/network/*.h
    ./common_src/network/*.cpp
    ./common_src/client/*.h
    ./common_src/client/*.cpp
    ./qt/*.h
    ./qt/*.cpp
)

# set(QT_SOURCES
#     qt/lobby.cpp
#     qt/Lobby.h
#     qt/gameoption.cpp
#     qt/GameOption.h
#     qt/numberplayers.cpp
#     qt/NumberPlayers.h
#     qt/joingame.cpp
#     qt/JoinGame.h
#     qt/mainwindow.cpp
#     qt/MainWindow.h
# )


add_executable (client
  ${SOURCES}
  ${UIS_HEADERS}
  # ${QT_SOURCES}
)

file(GLOB SOURCES
    ./common_src/*.h
    ./common_src/*.cpp
    ./common_src/network/*.h
    ./common_src/network/*.cpp
    ./common_src/server/*.h
    ./common_src/server/*.cpp
)
add_executable (server
  ${SOURCES}
)

target_link_libraries(client Threads::Threads SDL2pp::SDL2pp Qt5::Widgets)
target_link_libraries(server Threads::Threads SDL2pp::SDL2pp)