#pragma once

#include <cassert>
#include <string>
#include <fmt/core.h>

#include "value_type.h"

namespace lox {
enum class TokenType: std::int8_t {
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



class Token {
public:
    struct PrinterVisitor {
        std::string operator()(std::monostate value) { return "nil"; }
        std::string operator()(const std::string& value) { return value; }
        std::string operator()(double value) { return std::to_string(value); }
        std::string operator()(bool value) { return value ? "true" : "false"; }
        std::string operator()(CallablePtr value) { assert(false); }
    };

    Token(TokenType type, std::size_t line): Token(type, std::monostate(), line) {}

    template<typename LexemeType>
    Token(TokenType type, LexemeType lexeme, std::size_t line):
        type_{type}, lexeme_(std::forward<LexemeType>(lexeme)), line_{line} {}

    std::string to_string() const {
        return fmt::format("[{} {} {}]", Token::get_token_type_string(type_), stringify_token(), line_);
    }


    //TODO: try to move this to free function and implement just for the test case
    friend bool operator==(const Token& lhs, const Token& rhs) {
        return rhs.type_ == lhs.type_ && rhs.lexeme_ == lhs.lexeme_ && rhs.line_ == lhs.line_;
    }

    TokenType type_;
    ValueType lexeme_;
    std::size_t line_;

private:

    static std::string_view get_token_type_string(const TokenType type) {
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

    std::string stringify_token() const {
        switch(type_) {
            case TokenType::LEFT_PAREN:     return "(";
            case TokenType::RIGHT_PAREN:    return ")";
            case TokenType::LEFT_BRACE:     return "{";
            case TokenType::RIGHT_BRACE:    return "}";
            case TokenType::COMMA:          return ",";
            case TokenType::DOT:            return ".";
            case TokenType::MINUS:          return "-";
            case TokenType::PLUS:           return "+";
            case TokenType::SEMICOLON:      return ";";
            case TokenType::SLASH:          return "/";
            case TokenType::STAR:           return "*";

            case TokenType::BANG:           return "!";
            case TokenType::EQUAL:          return "=";
            case TokenType::GREATER:        return ">";
            case TokenType::LESS:           return "<";

            case TokenType::BANG_EQUAL:     return "!=";
            case TokenType::EQUAL_EQUAL:    return "==";
            case TokenType::GREATER_EQUAL:  return ">=";
            case TokenType::LESS_EQUAL:     return "<=";

            case TokenType::IDENTIFIER:     return std::get<std::string>(lexeme_);
            case TokenType::STRING:         return fmt::format("\"{}\"", std::get<std::string>(lexeme_));
            case TokenType::NUMBER:         return std::to_string(std::get<double>(lexeme_));

            case TokenType::AND:            return "and";
            case TokenType::CLASS:          return "class";
            case TokenType::ELSE:           return "else";
            case TokenType::FALSE:          return "false";
            case TokenType::FOR:            return "for";
            case TokenType::FUN:            return "fun";
            case TokenType::IF:             return "if";
            case TokenType::NIL:            return "nil";
            case TokenType::OR:             return "or";
            case TokenType::PRINT:          return "print";
            case TokenType::RETURN:         return "return";
            case TokenType::SUPER:          return "super";
            case TokenType::THIS:           return "this";
            case TokenType::TRUE:           return "true";
            case TokenType::VAR:            return "var";
            case TokenType::WHILE:          return "while";

            case TokenType::END:            return "";
        }

        __builtin_unreachable();
    }
};
} //namespace lox
