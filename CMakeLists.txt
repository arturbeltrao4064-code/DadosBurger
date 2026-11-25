
cmake_minimum_required(VERSION 3.16)

project(DadosBurger LANGUAGES CXX)

# Configuração do C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dependências do sistema
find_package(PkgConfig REQUIRED)
find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

pkg_check_modules(RAYLIB REQUIRED raylib)

# Executável
add_executable(dado_burger
    src/main.cpp
    src/partida.cpp
    src/avl.cpp
    src/cache.cpp
    src/fila.cpp
    src/historico.cpp
    src/montagem.cpp
    src/pereciveis.cpp
    src/receita.cpp
    src/timer.cpp
    src/arquivos.cpp
    src/interface.cpp
)

# Cabeçalhos
target_include_directories(dado_burger PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${RAYLIB_INCLUDE_DIRS}
)

# Localização da Raylib
target_link_directories(dado_burger PRIVATE
    ${RAYLIB_LIBRARY_DIRS}
)

# Bibliotecas: Raylib primeiro, dependências depois
target_link_libraries(dado_burger PRIVATE
    ${RAYLIB_LIBRARIES}
    X11::X11
    OpenGL::GL
    Threads::Threads
    ${CMAKE_DL_LIBS}
    m
    rt
)

# Avisos do compilador
if(MSVC)
    target_compile_options(dado_burger PRIVATE /W4)
else()
    target_compile_options(dado_burger PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# Executavel separado para testar a interface grafica sem alterar o fluxo
# principal do jogo enquanto a integracao e desenvolvida.
add_executable(dado_burger_interface
    src/interface_demo.cpp
    src/interface.cpp
)

target_include_directories(dado_burger_interface PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${RAYLIB_INCLUDE_DIRS}
)

target_link_directories(dado_burger_interface PRIVATE
    ${RAYLIB_LIBRARY_DIRS}
)

target_link_libraries(dado_burger_interface PRIVATE
    ${RAYLIB_LIBRARIES}
    X11::X11
    OpenGL::GL
    Threads::Threads
    ${CMAKE_DL_LIBS}
    m
    rt
)

if(MSVC)
    target_compile_options(dado_burger_interface PRIVATE /W4)
else()
    target_compile_options(dado_burger_interface PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()
