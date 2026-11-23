

cmake_minimum_required(VERSION 3.16)

project(DadosBurger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(dado_burger
    src/main.cpp
    src/partida.cpp
    src/carregar_partida.cpp
    src/salvar_partida.cpp
    src/avl.cpp
    src/cache.cpp
    src/fila.cpp
    src/historico.cpp
    src/montagem.cpp
    src/pereciveis.cpp
    src/receita.cpp
    src/timer.cpp
    src/arquivos.cpp
)

target_include_directories(dado_burger PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


if(MSVC)
    target_compile_options(dado_burger PRIVATE /W4)
else()
    target_compile_options(dado_burger PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()
