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
    std::unordered_map<std::string, TokenType> keywords_;

    bool is_end() const;
    bool match(const char c);
    void add_token(const TokenType type);

    // Call this function for tokens that have lexemes we need to remember
    // IDENTIFIER, STRING and NUMBER
    template<typename LexemeType>
    void add_token(const TokenType type, LexemeType&& lexeme)
    {
        tokens_.emplace_back(type, std::forward<LexemeType>(lexeme), line);
    }

    char advance();
    void scan_token();
    template<size_t LookAhead> char peek() const;
    void string();
    void number();
    void identifier();

public:
    explicit Scanner(std::string&& program):
        program_{std::move(program)},
        keywords_{
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
            {"while", TokenType::WHILE}} {}

    std::vector<Token> scan_tokens();
};
} // namespace lox
