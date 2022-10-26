#include <gtest/gtest.h>

extern "C" {
#include "clox/scanner.h"
}

#define MAKE_TOKEN(type, lexeme, line) \
  Token{type, lexeme, strlen(lexeme), line}

#define CHECK_TOKENS(correct)                       \
  do {                                              \
    for (size_t i = 0; i < correct.size(); i++) {   \
      Token t = scan_token();                       \
      ASSERT_EQ(t.type, correct[i].type);           \
      ASSERT_EQ(t.line, correct[i].line);           \
    }                                               \
  } while (0)


TEST(TestScanner, TestWhitespaceSkipping) {
  init_scanner(R"(( ) {   }  ,   .
-   +
;
    /
)");

  const std::vector<Token> correct({
    MAKE_TOKEN(TOKEN_LEFT_PAREN, "(", 1),
    MAKE_TOKEN(TOKEN_RIGHT_PAREN, ")", 1),
    MAKE_TOKEN(TOKEN_LEFT_BRACE, "{", 1),
    MAKE_TOKEN(TOKEN_RIGHT_BRACE, "}", 1),
    MAKE_TOKEN(TOKEN_COMMA, ",", 1),
    MAKE_TOKEN(TOKEN_DOT, ".", 1),
    MAKE_TOKEN(TOKEN_MINUS, "-", 2),
    MAKE_TOKEN(TOKEN_PLUS, "+", 2),
    MAKE_TOKEN(TOKEN_SEMICOLON, ";", 3),
    MAKE_TOKEN(TOKEN_SLASH, "/", 4),
    MAKE_TOKEN(TOKEN_EOF, "", 5),
  });

  CHECK_TOKENS(correct);
}

TEST(TestScanner, TestOneCharAndTwoChar) {
  init_scanner(R"(
! !=
= ==
> >=
< <=
)");

  const std::vector<Token> correct({
    MAKE_TOKEN(TOKEN_BANG, "!", 2),
    MAKE_TOKEN(TOKEN_BANG_EQUAL, "!=", 2),
    MAKE_TOKEN(TOKEN_EQUAL, "=", 3),
    MAKE_TOKEN(TOKEN_EQUAL_EQUAL, "==", 3),
    MAKE_TOKEN(TOKEN_GREATER, ">", 4),
    MAKE_TOKEN(TOKEN_GREATER_EQUAL, ">=", 4),
    MAKE_TOKEN(TOKEN_LESS, "<", 5),
    MAKE_TOKEN(TOKEN_LESS_EQUAL, "<=", 5),
    MAKE_TOKEN(TOKEN_EOF, "", 6),
  });

  CHECK_TOKENS(correct);
}

TEST(TestScanner, TestKeywords) {
  init_scanner("and class else false fun for if nil\n"
               "or print return super this true var while");

  const std::vector<Token> correct({
    MAKE_TOKEN(TOKEN_AND, "and", 1),
    MAKE_TOKEN(TOKEN_CLASS, "class", 1),
    MAKE_TOKEN(TOKEN_ELSE, "else", 1),
    MAKE_TOKEN(TOKEN_FALSE, "false", 1),
    MAKE_TOKEN(TOKEN_FUN, "fun", 1),
    MAKE_TOKEN(TOKEN_FOR, "for", 1),
    MAKE_TOKEN(TOKEN_IF, "if", 1),
    MAKE_TOKEN(TOKEN_NIL, "nil", 1),
    MAKE_TOKEN(TOKEN_OR, "or", 2),
    MAKE_TOKEN(TOKEN_PRINT, "print", 2),
    MAKE_TOKEN(TOKEN_RETURN, "return", 2),
    MAKE_TOKEN(TOKEN_SUPER, "super", 2),
    MAKE_TOKEN(TOKEN_THIS, "this", 2),
    MAKE_TOKEN(TOKEN_TRUE, "true", 2),
    MAKE_TOKEN(TOKEN_VAR, "var", 2),
    MAKE_TOKEN(TOKEN_WHILE, "while", 2),
    MAKE_TOKEN(TOKEN_EOF, "", 2),
  });

  CHECK_TOKENS(correct);
}

TEST(TestScanner, TestStrings) {
  init_scanner(R"(
"I am a string";
"";    // The empty string.
"123"; // This is a string, not a number.
123;   // This is a number
a;     // and this is an identifier
)");

  const std::vector<Token> correct({
    MAKE_TOKEN(TOKEN_STRING, "I am a string", 2),
    MAKE_TOKEN(TOKEN_SEMICOLON, ";", 2),
    MAKE_TOKEN(TOKEN_STRING, "", 3),
    MAKE_TOKEN(TOKEN_SEMICOLON, ";", 3),
    MAKE_TOKEN(TOKEN_STRING, "123", 4),
    MAKE_TOKEN(TOKEN_SEMICOLON, ";", 4),
    MAKE_TOKEN(TOKEN_NUMBER, "123", 5),
    MAKE_TOKEN(TOKEN_SEMICOLON, ";", 5),
    MAKE_TOKEN(TOKEN_IDENTIFIER, "a", 6),
    MAKE_TOKEN(TOKEN_SEMICOLON, ";", 6),
    MAKE_TOKEN(TOKEN_EOF, "", 7),
  });

  CHECK_TOKENS(correct);
}


TEST(TestScanner, Bool) {
  init_scanner(R"(
true;  // Not false.
false; // Not *not* false.
)");

  const std::vector<Token> correct({
    MAKE_TOKEN(TOKEN_TRUE, "true", 2),
    MAKE_TOKEN(TOKEN_SEMICOLON, ";", 2),
    MAKE_TOKEN(TOKEN_FALSE, "false", 3),
    MAKE_TOKEN(TOKEN_SEMICOLON, ";", 3),
    MAKE_TOKEN(TOKEN_EOF, "", 4),
  });

  CHECK_TOKENS(correct);
}

TEST(TestScanner, HelloWorld) {
  init_scanner(R"(
// Your first Lox program!
print "Hello, world!";
)");

  const std::vector<Token> correct({
    MAKE_TOKEN(TOKEN_PRINT, "print", 3),
    MAKE_TOKEN(TOKEN_STRING, "Hello, world!", 3),
    MAKE_TOKEN(TOKEN_SEMICOLON, ";", 3),
    MAKE_TOKEN(TOKEN_EOF, "", 4)
  });

  CHECK_TOKENS(correct);
}

TEST(ScannerTest, Classes) {
  init_scanner(R"(
class Breakfast {
  init(meat, bread) {
)");

  const std::vector<Token> correct({
    MAKE_TOKEN(TOKEN_CLASS, "class", 2),
    MAKE_TOKEN(TOKEN_IDENTIFIER, "Breakfast", 2),
    MAKE_TOKEN(TOKEN_LEFT_BRACE, "{", 2),
    MAKE_TOKEN(TOKEN_IDENTIFIER, "init", 3),
    MAKE_TOKEN(TOKEN_LEFT_PAREN, "(", 3),
    MAKE_TOKEN(TOKEN_IDENTIFIER, "meat", 3),
    MAKE_TOKEN(TOKEN_COMMA, ", ", 3),
    MAKE_TOKEN(TOKEN_IDENTIFIER, "bread", 3),
    MAKE_TOKEN(TOKEN_RIGHT_PAREN, ")", 3),
    MAKE_TOKEN(TOKEN_LEFT_BRACE, "{", 3),
    MAKE_TOKEN(TOKEN_EOF, "", 4)
  });

  CHECK_TOKENS(correct);
}

TEST(ScannerTest, ClassMethodCall) {
  init_scanner(R"(baconAndToast.serve("Dear Reader");)");

  const std::vector<Token> correct{
    MAKE_TOKEN(TOKEN_IDENTIFIER, "baconAndToast", 1),
    MAKE_TOKEN(TOKEN_DOT, ".", 1),
    MAKE_TOKEN(TOKEN_IDENTIFIER, "serve", 1),
    MAKE_TOKEN(TOKEN_LEFT_PAREN, "(", 1),
    MAKE_TOKEN(TOKEN_STRING, "Dear Reader", 1),
    MAKE_TOKEN(TOKEN_RIGHT_PAREN, ")", 1),
    MAKE_TOKEN(TOKEN_SEMICOLON, ";", 1),
    MAKE_TOKEN(TOKEN_EOF, "", 1),
  };

  CHECK_TOKENS(correct);
}
