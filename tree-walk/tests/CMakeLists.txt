enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# add_executable(parser_test parser_test.cpp)
add_executable(scanner_test scanner_test.cpp)

# target_link_libraries(parser_test lox_lib gtest_main ast_yaml_utils)
target_link_libraries(scanner_test lox_lib gtest_main)

include(GoogleTest)
gtest_discover_tests(scanner_test)
#gtest_discover_tests(parser_test)
