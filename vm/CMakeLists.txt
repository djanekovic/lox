cmake_minimum_required(VERSION 3.14)
project(clox)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

add_library(clox_lib src/chunk.c src/memory.c src/debug.c)
include_directories(lox_lib PUBLIC include)
target_compile_options(clox_lib PUBLIC -Wall -Wextra --pedantic-errors -g)

add_executable(clox src/main.c)
target_link_libraries(clox clox_lib)

add_subdirectory(test)
