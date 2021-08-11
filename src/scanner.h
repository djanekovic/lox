#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "token.h"

namespace lox {
class Scanner {
private:
    std::size_t start = 0;
    std::size_t current = 0;
    std::size_t line = 1;

    std::vector<Token> tokens_;
    std::string program_;
    const std::unordered_map<std::string, TokenType> keywords_
    {
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
        {"while", TokenType::WHILE}
    };

    bool is_end() const;
    bool match(const char c);
    void add_token(const TokenType type);

    template<typename String,
             typename = std::enable_if_t<std::is_convertible_v<String, std::string>>>
    void add_token(const TokenType type, String&& lexeme)
    {
        tokens_.emplace_back(type, std::forward<String>(lexeme), line);
    }
    char advance();
    void scan_token();
    template<size_t LookAhead> char peek() const;
    void string();
    void number();
    void identifier();

    static bool is_alpha(const char c);
    static bool is_alphanum(const char c);

public:
    Scanner(std::string program): program_{std::move(program)} {}
    std::vector<Token> scan_tokens();
};
} // namespace lox
