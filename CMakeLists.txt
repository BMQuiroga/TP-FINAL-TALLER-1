cmake_minimum_required(VERSION 2.8)

project(tp-final)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_COMPILER_ID GNU)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -D_POSIX_C_SOURCE=200809L")

# configuration file
add_custom_target(copy-files ALL
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_SOURCE_DIR}/config.yml
  ${CMAKE_CURRENT_BINARY_DIR}/config.yml
)

# Paquetes necesarios para la compilacion
# Threads
set(CMAKE_THREAD_PREFER_PTHREAD ON)                                                                                                                                                                                                           
set(THREADS_PREFER_PTHREAD_FLAG ON)                                                                                                                                                                                                           
find_package(Threads REQUIRED) 
# SDL2pp
find_package(SDL2pp REQUIRED)
# yaml-cpp
find_package(yaml-cpp REQUIRED)

# Find the QtWidgets library
find_package(Qt5Widgets COMPONENTS Widgets REQUIRED)

include_directories(${YAML_CPP_INCLUDE_DIRS})

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
  qt/waitingroom.ui
)

# Habilito el automoc para poder hacer autoconnect de señales
set(CMAKE_AUTOMOC on)
# Habilito autorcc para manejar automaticamente los QResources
set(CMAKE_AUTORCC ON)

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

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
  ${CLIENT_SOURCES}
  ${UIS_HEADERS}
  # ${QT_SOURCES}
)

file(GLOB SERVER_SOURCES
    common_src/*.h
    common_src/*.cpp
    common_src/network/*.h
    common_src/network/*.cpp
    common_src/server/*.h
    common_src/server/*.cpp
)
add_executable (server
  ${SERVER_SOURCES}
)

target_link_libraries(client Threads::Threads SDL2pp::SDL2pp ${YAML_CPP_LIBRARIES} Qt5::Widgets)
target_link_libraries(server Threads::Threads SDL2pp::SDL2pp ${YAML_CPP_LIBRARIES})

# testing
enable_testing()

list(REMOVE_ITEM SERVER_SOURCES "${PROJECT_SOURCE_DIR}/common_src/server/server_main.cpp")
file(GLOB TEST_SOURCES
    tests/*.cpp
)

add_executable(
  test
  ${TEST_SOURCES}
  ${SERVER_SOURCES}
)
target_link_libraries(
  test
  GTest::gtest_main Threads::Threads SDL2pp::SDL2pp ${YAML_CPP_LIBRARIES}
)

include(GoogleTest)
gtest_discover_tests(test)
add_custom_command(
     TARGET test
     COMMENT "Run tests"
     POST_BUILD 
     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
     COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIGURATION> --verbose --output-on-failures
)