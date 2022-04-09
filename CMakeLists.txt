cmake_minimum_required(VERSION 3.14)
project(Lox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(extern/fmt)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(lox_lib
    src/scanner.cpp
    src/parser.cpp
    src/interpreter.cpp
    src/environment.cpp
    src/native/clock_callable.cpp
    src/resolver.cpp)

target_compile_options(lox_lib PUBLIC -Wall -g)
include_directories(lox_lib PUBLIC include)
include_directories(lox_lib PUBLIC extern/fmt/include)
target_link_libraries(lox_lib fmt::fmt)

add_executable(lox src/main.cpp)
target_link_libraries(lox lox_lib)

add_subdirectory(tests)

