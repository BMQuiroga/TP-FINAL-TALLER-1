cmake_minimum_required(VERSION 2.8)

project(tp-final)

set(CMAKE_CXX_COMPILER_ID GNU)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -D_POSIX_C_SOURCE=200809L")

# Paquetes necesarios para la compilacion
# Threads
set(CMAKE_THREAD_PREFER_PTHREAD ON)                                                                                                                                                                                                           
set(THREADS_PREFER_PTHREAD_FLAG ON)                                                                                                                                                                                                           
find_package(Threads REQUIRED) 
# SDL2pp
find_package(SDL2pp REQUIRED)

# Agrego la carpeta con los headers autogenerados a los includes
message("Agregando '${PROJECT_BINARY_DIR}' como directorio de includes")
include_directories(${PROJECT_BINARY_DIR})

file(GLOB SOURCES
    ./common_src/*.h
    ./common_src/*.cpp
    ./common_src/network/*.h
    ./common_src/network/*.cpp
    ./common_src/client/*.h
    ./common_src/client/*.cpp
)

add_executable (client
  ${SOURCES}
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

target_link_libraries(client Threads::Threads SDL2pp::SDL2pp)
target_link_libraries(server Threads::Threads SDL2pp::SDL2pp)