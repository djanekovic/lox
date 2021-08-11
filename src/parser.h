#pragma once

#include <vector>
#include <array>
#include "token.h"
#include "expr.h"

namespace lox {
class Parser {

    struct ParseError: public std::runtime_error {
        ParseError(): std::runtime_error("") {}
    };

    const std::vector<Token> tokens_;
    std::size_t current = 0;

    bool is_end() const;
    Token peek() const;
    Token previous() const;
    Token advance();
    bool check(const TokenType type) const;
    ParseError error(Token token, std::string message);
    Token consume(TokenType type, std::string message);
    void synchronize();

    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();

    template<size_t N> bool match(std::array<TokenType, N> tokens);

public:
    explicit Parser(std::vector<Token> tokens):
        tokens_{std::move(tokens)} {}

    std::unique_ptr<Expr> parse();
};
} //namespace lox
