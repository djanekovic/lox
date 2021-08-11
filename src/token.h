#pragma once

#include <string>
#include <fmt/core.h>

namespace lox {
enum class TokenType {
    // Single character tokens
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // One or two character tokens
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals
    IDENTIFIER, STRING, NUMBER,

    // Keywords
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

    END
};


struct Token {
    const TokenType type_;
    const std::string lexeme_;
    const int line_;
    private:
    static std::string get_token_type_string(const TokenType type) {
        switch(type) {
            case TokenType::LEFT_PAREN:     return "LEFT_PAREN";
            case TokenType::RIGHT_PAREN:    return "RIGHT_PAREN";
            case TokenType::LEFT_BRACE:     return "LEFT_BRACE";
            case TokenType::RIGHT_BRACE:    return "RIGHT_BRACE";
            case TokenType::COMMA:          return "COMMA";
            case TokenType::DOT:            return "DOT";
            case TokenType::MINUS:          return "MINUS";
            case TokenType::PLUS:           return "PLUS";
            case TokenType::SEMICOLON:      return "SEMICOLON";
            case TokenType::SLASH:          return "SLASH";
            case TokenType::STAR:           return "STAR";

            case TokenType::BANG:           return "BANG";
            case TokenType::EQUAL:          return "EQUAL";
            case TokenType::GREATER:        return "GREATER";
            case TokenType::LESS:           return "LESS";

            case TokenType::BANG_EQUAL:     return "BANG_EQUAL";
            case TokenType::EQUAL_EQUAL:    return "EQUAL_EQUAL";
            case TokenType::GREATER_EQUAL:  return "GREATER_EQUAL";
            case TokenType::LESS_EQUAL:     return "LESS_EQUAL";

            case TokenType::IDENTIFIER:     return "IDENTIFIER";
            case TokenType::STRING:         return "STRING";
            case TokenType::NUMBER:         return "NUMBER";

            case TokenType::AND:            return "AND";
            case TokenType::CLASS:          return "CLASS";
            case TokenType::ELSE:           return "ELSE";
            case TokenType::FALSE:          return "FALSE";
            case TokenType::FUN:            return "FUN";
            case TokenType::FOR:            return "FOR";
            case TokenType::IF:             return "IF";
            case TokenType::NIL:            return "NIL";
            case TokenType::OR:             return "OR";
            case TokenType::PRINT:          return "PRINT";
            case TokenType::RETURN:         return "RETURN";
            case TokenType::SUPER:          return "SUPER";
            case TokenType::THIS:           return "THIS";
            case TokenType::TRUE:           return "TRUE";
            case TokenType::VAR:            return "VAR";
            case TokenType::WHILE:          return "WHILE";

            case TokenType::END:            return "EOF";
        }

        __builtin_unreachable();
    }

public:
    template<typename String,
             typename = std::enable_if_t<std::is_convertible_v<String, std::string>>>
    Token(TokenType type, String lexeme, int line):
        type_{type}, lexeme_(std::forward<String>(lexeme)), line_{line} {}

    std::string to_string() const {
        return fmt::format("[{} {} {}]", Token::get_token_type_string(type_), lexeme_, line_);
    }
};
} //namespace lox
