#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "token.h"

namespace lox {
class Scanner {
  public:
    explicit Scanner(std::string&& program);
    std::vector<Token> scan_tokens();

  private:
    std::size_t start_ = 0;
    std::size_t current_ = 0;
    std::size_t line_ = 1;

    std::vector<Token> tokens_;
    std::string program_;
    std::unordered_map<std::string, TokenType> keywords_{
      {"and", TokenType::AND},
      {"class", TokenType::CLASS},
      {"else", TokenType::ELSE},
      {"false", TokenType::FALSE},
      {"for", TokenType::FOR},
      {"fun", TokenType::FUN},
      {"if", TokenType::IF},
      {"nil", TokenType::NIL},
      {"or", TokenType::OR},
      {"print", TokenType::PRINT},
      {"return", TokenType::RETURN},
      {"super", TokenType::SUPER},
      {"this", TokenType::THIS},
      {"true", TokenType::TRUE},
      {"var", TokenType::VAR},
      {"while", TokenType::WHILE}};

    bool is_end() const;
    bool match(const char c);
    std::optional<Token> scan_token();

    // Call this function for tokens that have lexemes we need to remember
    // IDENTIFIER, STRING and NUMBER
    template<typename LexemeType>
    Token get_current_line_token(const TokenType type, LexemeType&& lexeme) const;
    Token get_current_line_token(const TokenType type) const;

    char advance();
    char peek(std::size_t look_ahead) const;
    std::optional<Token> string();
    Token number();
    Token identifier();
};
} // namespace lox
