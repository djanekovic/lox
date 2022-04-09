#pragma once

#include <memory>
#include <vector>
#include <string_view>

#include "token.h"
#include "stmt/stmt.h"
#include "expr/expr.h"

namespace lox {
class Parser {
    struct ParseError: public std::runtime_error {
        ParseError(): std::runtime_error("") {}
    };

    std::vector<Token> tokens_;
    std::size_t current = 0;

    bool is_end() const;
    Token peek() const;
    Token previous() const;
    Token advance();
    bool check(const TokenType type) const;
    ParseError error(Token token, std::string message);
    Token consume(TokenType type, std::string message);
    void synchronize();


    std::unique_ptr<Stmt> declaration();
    std::unique_ptr<ClassStmt> class_declaration();
    std::unique_ptr<VarStmt> variable_declaration();
    std::unique_ptr<FunctionStmt> function_declaration(std::string_view kind);

    std::unique_ptr<Stmt> statement();
    std::unique_ptr<PrintStmt> print_statement();
    std::unique_ptr<ReturnStmt> return_statement();
    std::unique_ptr<IfExpressionStmt> if_statement();
    std::unique_ptr<Stmt> while_statement();
    std::unique_ptr<Stmt> for_statement();
    std::unique_ptr<ExpressionStmt> expression_statement();
    std::vector<std::unique_ptr<Stmt>> block();

    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> assignment();
    std::unique_ptr<Expr> logical_or();
    std::unique_ptr<Expr> logical_and();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> call();
    std::unique_ptr<CallExpr> finish_call(std::unique_ptr<Expr> expr);
    std::unique_ptr<Expr> primary();

    bool match(std::initializer_list<TokenType> tokens);

public:
    explicit Parser(std::vector<Token>&& tokens):
        tokens_{std::move(tokens)} {}

    std::vector<std::unique_ptr<Stmt>> parse();
};
} //namespace lox
