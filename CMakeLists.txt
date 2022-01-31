cmake_minimum_required(VERSION 3.13)
project(Lox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")

add_subdirectory(extern/fmt)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(extern/fmt/include)
add_executable(lox
    src/main.cpp
    src/scanner.cpp
    src/ast_pretty_printer.cpp
    src/parser.cpp
    src/interpreter.cpp
    src/environment.cpp
    src/native/clock_callable.cpp)
target_link_libraries(lox fmt::fmt)

add_subdirectory(tests)

