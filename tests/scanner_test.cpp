#include "gtest/gtest.h"

#include "lox/scanner.h"

namespace {
auto tokenize(std::string program) {
    lox::Scanner scanner(std::move(program));
    return scanner.scan_tokens();
}
}

TEST(ScannerTest, Identity) {
    ASSERT_EQ(1, 1);
}


TEST(ScannerTest, Bool) {
    const auto bool_test = R"(
true;  // Not false.
false; // Not *not* false.)";
    const auto tokens = tokenize(bool_test);
    ASSERT_NE(tokens.size(), 0);
}
