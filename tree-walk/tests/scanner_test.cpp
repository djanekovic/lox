#include "gtest/gtest.h"
#include "lox/scanner.h"

namespace lox {
inline bool operator==(const Token& lhs, const Token& rhs) {
    return rhs.type_ == lhs.type_ && rhs.line_ == lhs.line_;
}
}

namespace {
void compare_tokens(std::vector<lox::Token>&& output_tokens, std::vector<lox::Token>&& correct_tokens) {
    EXPECT_EQ(output_tokens.size(), correct_tokens.size());
    for (auto i = 0u; i < output_tokens.size(); i++) {
        EXPECT_EQ(output_tokens[i], correct_tokens[i]);
#if 0
        if (output_tokens[i] != correct_tokens[i]) {
            std::cout << output_tokens[i].to_string() << ", " << correct_tokens[i].to_string() << std::endl;
        }
#endif
    }
    EXPECT_EQ(output_tokens, correct_tokens);
}
}

/*
 * Boring tests
 */
TEST(ScannerTest, SingleCharacterTokens) {
    auto test_str = R"(( ) {   }  ,   .
-   +
;
    /
*
)";

    std::vector<lox::Token> correct_tokens{
        {lox::TokenType::LEFT_PAREN, 1},
        {lox::TokenType::RIGHT_PAREN, 1},
        {lox::TokenType::LEFT_BRACE, 1},
        {lox::TokenType::RIGHT_BRACE, 1},
        {lox::TokenType::COMMA, 1},
        {lox::TokenType::DOT, 1},
        {lox::TokenType::MINUS, 2},
        {lox::TokenType::PLUS, 2},
        {lox::TokenType::SEMICOLON, 3},
        {lox::TokenType::SLASH, 4},
        {lox::TokenType::STAR, 5},
        {lox::TokenType::END, 6},
    };

    compare_tokens(lox::scan_tokens(std::move(test_str)), std::move(correct_tokens));
}

TEST(ScannerTest, OneOrTwoCharacterTokens) {
    auto test_str = R"(
! !=
= ==
> >=
< <=
)";

    std::vector<lox::Token> correct_tokens{
        {lox::TokenType::BANG, 2},
        {lox::TokenType::BANG_EQUAL, 2},
        {lox::TokenType::EQUAL, 3},
        {lox::TokenType::EQUAL_EQUAL, 3},
        {lox::TokenType::GREATER, 4},
        {lox::TokenType::GREATER_EQUAL, 4},
        {lox::TokenType::LESS, 5},
        {lox::TokenType::LESS_EQUAL, 5},
        {lox::TokenType::END, 6},
    };

    compare_tokens(lox::scan_tokens(std::move(test_str)), std::move(correct_tokens));
}

TEST(ScannerTest, Literals) {
    auto test_str = R"(
"I am a string";
"";    // The empty string.
"123"; // This is a string, not a number.
123;   // This is a number
a;     // and this is an identifier
)";
    std::vector<lox::Token> correct_tokens{
        {lox::TokenType::STRING, "I am a string", 2},
        {lox::TokenType::SEMICOLON, 2},
        {lox::TokenType::STRING, "", 3},
        {lox::TokenType::SEMICOLON, 3},
        {lox::TokenType::STRING, "123", 4},
        {lox::TokenType::SEMICOLON, 4},
        {lox::TokenType::NUMBER, 123.0, 5},
        {lox::TokenType::SEMICOLON, 5},
        {lox::TokenType::IDENTIFIER, "a", 6},
        {lox::TokenType::SEMICOLON, 6},
        {lox::TokenType::END, 7}
    };

    compare_tokens(lox::scan_tokens(std::move(test_str)), std::move(correct_tokens));
}

TEST(ScannerTest, Keywords) {
    auto test_str = R"(
and class else false fun for if nil
or print return super this true var while
)";

    std::vector<lox::Token> correct_tokens{
        {lox::TokenType::AND, 2},
        {lox::TokenType::CLASS, 2},
        {lox::TokenType::ELSE, 2},
        {lox::TokenType::FALSE, 2},
        {lox::TokenType::FUN, 2},
        {lox::TokenType::FOR, 2},
        {lox::TokenType::IF, 2},
        {lox::TokenType::NIL, 2},
        {lox::TokenType::OR, 3},
        {lox::TokenType::PRINT, 3},
        {lox::TokenType::RETURN, 3},
        {lox::TokenType::SUPER, 3},
        {lox::TokenType::THIS, 3},
        {lox::TokenType::TRUE, 3},
        {lox::TokenType::VAR, 3},
        {lox::TokenType::WHILE, 3},
        {lox::TokenType::END, 4},
    };

    compare_tokens(lox::scan_tokens(std::move(test_str)), std::move(correct_tokens));
}

/*
 * Little bit more interesting test cases
 */
TEST(ScannerTest, Bool) {
    auto bool_test = R"(
true;  // Not false.
false; // Not *not* false.
)";
    std::vector<lox::Token> correct_tokens{
        {lox::TokenType::TRUE, 2},
        {lox::TokenType::SEMICOLON, 2},
        {lox::TokenType::FALSE, 3},
        {lox::TokenType::SEMICOLON, 3},
        {lox::TokenType::END, 4},
    };

    compare_tokens(lox::scan_tokens(std::move(bool_test)), std::move(correct_tokens));
}

TEST(ScannerTest, HelloWorld) {
    auto test_str = R"(
// Your first Lox program!
print "Hello, world!";
)";

    std::vector<lox::Token> correct_tokens{
        {lox::TokenType::PRINT, 3},
        {lox::TokenType::STRING, "Hello, world!", 3},
        {lox::TokenType::SEMICOLON, 3},
        {lox::TokenType::END, 4}
    };

    compare_tokens(lox::scan_tokens(std::move(test_str)), std::move(correct_tokens));
}

TEST(ScannerTest, Classes) {
    auto test_str = R"(
class Breakfast {
  init(meat, bread) {
)";
    std::vector<lox::Token> correct_tokens{
        {lox::TokenType::CLASS, 2},
        {lox::TokenType::IDENTIFIER, "Breakfast", 2},
        {lox::TokenType::LEFT_BRACE, 2},
        {lox::TokenType::IDENTIFIER, "init", 3},
        {lox::TokenType::LEFT_PAREN, 3},
        {lox::TokenType::IDENTIFIER, "meat", 3},
        {lox::TokenType::COMMA, 3},
        {lox::TokenType::IDENTIFIER, "bread", 3},
        {lox::TokenType::RIGHT_PAREN, 3},
        {lox::TokenType::LEFT_BRACE, 3},
        {lox::TokenType::END, 4}
    };

    compare_tokens(lox::scan_tokens(std::move(test_str)), std::move(correct_tokens));
}

TEST(ScannerTest, ClassMethodCall) {
    auto test_str = R"(baconAndToast.serve("Dear Reader");)";

    std::vector<lox::Token> correct_tokens{
        {lox::TokenType::IDENTIFIER, "baconAndToast", 1},
        {lox::TokenType::DOT, 1},
        {lox::TokenType::IDENTIFIER, "serve", 1},
        {lox::TokenType::LEFT_PAREN, 1},
        {lox::TokenType::STRING, "Dear Reader", 1},
        {lox::TokenType::RIGHT_PAREN, 1},
        {lox::TokenType::SEMICOLON, 1},
        {lox::TokenType::END, 1},
    };

    compare_tokens(lox::scan_tokens(std::move(test_str)), std::move(correct_tokens));
}
