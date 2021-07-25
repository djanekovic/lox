cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(extern/fmt)

include_directories(extern/fmt/include)
add_executable(lox src/main.cpp src/scanner.cpp src/pretty_printer.cpp)
target_link_libraries(lox fmt::fmt)
