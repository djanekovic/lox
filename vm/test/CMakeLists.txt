# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(test_chunk test_chunk.cpp)
target_link_libraries(test_chunk GTest::gtest_main clox_lib)

add_executable(test_scanner test_scanner.cpp)
target_link_libraries(test_scanner GTest::gtest_main clox_lib)

include(GoogleTest)
gtest_discover_tests(test_chunk)
