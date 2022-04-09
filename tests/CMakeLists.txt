enable_testing()

add_executable(parser_test parser_test.cpp)
add_executable(scanner_test scanner_test.cpp)

target_link_libraries(parser_test lox_lib gtest_main)
target_link_libraries(scanner_test lox_lib gtest_main)

include(GoogleTest)
gtest_discover_tests(scanner_test)
gtest_discover_tests(parser_test)
