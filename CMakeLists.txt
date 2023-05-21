cmake_minimum_required(VERSION 2.8)

project(tp-final)

set(CMAKE_CXX_COMPILER_ID GNU)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -D_POSIX_C_SOURCE=200809L")
set(EXECUTABLE_OUTPUT_PATH "../")

find_package(SDL2pp REQUIRED)

# Agrego la carpeta con los headers autogenerados a los includes
message("Agregando '${PROJECT_BINARY_DIR}' como directorio de includes")
include_directories(${PROJECT_BINARY_DIR})

file(GLOB_RECURSE SOURCES
    ./*.h
    ./*.cpp
)

add_executable (game
  ${SOURCES}
)

target_link_libraries(game SDL2 SDL2_image SDL2pp::SDL2pp)

install(FILES assets/cat.gif DESTINATION ${CMAKE_BINARY_DIR})