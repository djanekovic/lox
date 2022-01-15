#include <cassert>
#include <memory>
#include <numeric>
#include <functional>

#include "lox.h"
#include "parser.h"

using namespace lox;

std::vector<std::unique_ptr<Stmt>> Parser::parse()
{
    std::vector<std::unique_ptr<Stmt>> statements;
    while(!is_end()) {
        statements.emplace_back(declaration());
    }

    return statements;
}

std::unique_ptr<Stmt> Parser::declaration()
{
    try {
        if (match(std::array{TokenType::VAR})) {
            return variable_declaration();
        }
        return statement();
    } catch (ParseError& ex) {
        synchronize();
        return nullptr;
    }
}

std::unique_ptr<Stmt> Parser::variable_declaration()
{
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name");
    auto expr = match(std::array{TokenType::EQUAL}) ? expression() : std::unique_ptr<Expr>();
    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration");

    return std::make_unique<VarStmt>(std::move(name), std::move(expr));
}


std::unique_ptr<Stmt> Parser::statement() {
    if (match(std::array{TokenType::PRINT})) {
        return print_statement();
    }

    return expression_statement();
}


/**
 * print_statement -> "print" expression ";"
 */
std::unique_ptr<Stmt> Parser::print_statement() {
    auto expr = expression();
    consume(TokenType::SEMICOLON, "Expect ; after value");
    return std::make_unique<PrintStmt>(std::move(expr));
}

/**
 * expression_statement -> expression ";"
 */
std::unique_ptr<Stmt> Parser::expression_statement() {
    auto expr = expression();
    consume(TokenType::SEMICOLON, "Expect ; after value");
    return std::make_unique<ExpressionStmt>(std::move(expr));
}


/**
 * expression -> assignment
 */
std::unique_ptr<Expr> Parser::expression()
{
    return assignment();
}


std::unique_ptr<Expr> Parser::assignment() {
    auto expr = equality();
    if (match(std::array{TokenType::EQUAL})) {
        auto equals = previous();
        auto value = assignment();

        if (auto *upcast_expr = dynamic_cast<VariableExpr *>(expr.get())) {
            return std::make_unique<AssignExpr>(std::move(upcast_expr->name_), std::move(value));
        }

        Lox::error(equals, "Invalid assignment target.");
    }

    return expr;
}


/**
 * equality -> comparison ( ( "!=" | "==" ) comparison )*
 */
std::unique_ptr<Expr> Parser::equality()
{
    auto expr = comparison();

    while(match(std::array{TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        auto op = previous();
        auto right = comparison();
        expr = std::make_unique<BinaryExpr>(std::move(op), std::move(expr), std::move(right));
    }

    return expr;
}


/**
 * comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )*
 */
std::unique_ptr<Expr> Parser::comparison()
{
    auto expr = term();

    while(match(std::array{TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
        auto op = previous();
        auto right = term();
        expr = std::make_unique<BinaryExpr>(std::move(op), std::move(expr), std::move(right));
    }

    return expr;
}


/**
 * term -> factor ( ( "+" | "-" ) factor )*
 */
std::unique_ptr<Expr> Parser::term()
{
    auto expr = factor();

    while(match(std::array{TokenType::PLUS, TokenType::MINUS})) {
        auto op = previous();
        auto right = factor();
        expr = std::make_unique<BinaryExpr>(std::move(op), std::move(expr), std::move(right));
    }

    return expr;
}


/**
 * factor -> unary ( ( "*" | "/" ) unary )*
 */
std::unique_ptr<Expr> Parser::factor()
{
    auto expr = unary();

    while(match(std::array{TokenType::SLASH, TokenType::STAR})) {
        auto op = previous();
        auto right = unary();
        expr = std::make_unique<BinaryExpr>(std::move(op), std::move(expr), std::move(right));
    }

    return expr;
}


/**
 * unary -> ( "!" | "-" ) unary | primary
 */
std::unique_ptr<Expr> Parser::unary()
{
    if (match(std::array{TokenType::BANG, TokenType::MINUS})) {
        auto op = previous();
        auto right = unary();
        return std::make_unique<UnaryExpr>(std::move(op), std::move(right));
    }

    return primary();
}


/**
 * primary -> "true" | "false" | "nil" | NUMBER | STRING | IDENTIFIER | "(" expression ")" ;
 */
std::unique_ptr<Expr> Parser::primary()
{
    if (match(std::array{TokenType::TRUE})) {
        return std::make_unique<LiteralExpr>(true);
    }

    if (match(std::array{TokenType::FALSE})) {
        return std::make_unique<LiteralExpr>(false);
    }

    if (match(std::array{TokenType::NIL})) {
        return std::make_unique<LiteralExpr>(std::monostate());
    }

    if (match(std::array{TokenType::NUMBER})) {
        return std::make_unique<LiteralExpr>(std::get<double>(previous().lexeme_));
    }

    if (match(std::array{TokenType::STRING})) {
        return std::make_unique<LiteralExpr>(std::get<std::string>(previous().lexeme_));
    }

    if (match(std::array{TokenType::IDENTIFIER})) {
        return std::make_unique<VariableExpr>(previous());
    }

    if (match(std::array{TokenType::LEFT_PAREN})) {
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression");
        return std::make_unique<GroupingExpr>(std::move(expr));
    }

    throw error(peek(), "Expect expression");
}

bool Parser::is_end() const
{
    return peek().type_ == TokenType::END;
}

Token Parser::peek() const
{
    return tokens_.at(current);
}

Token Parser::previous() const
{
    return tokens_.at(current-1);
}

Token Parser::advance()
{
    if (!is_end()) {
        current++;
    }

    return previous();
}

bool Parser::check(const TokenType type) const
{
    return is_end() ? false : peek().type_ == type;
}


Parser::ParseError Parser::error(Token token, std::string message)
{
    Lox::error(peek(), message);
    return ParseError();
}

Token Parser::consume(TokenType type, std::string message)
{
    if (check(type)) {
        return advance();
    }

    throw error(peek(), std::move(message));
}

void Parser::synchronize()
{
    for(advance(); !is_end(); advance()) {
        if (previous().type_ == TokenType::SEMICOLON) {
            return;
        }

        switch(peek().type_) {
            case TokenType::CLASS:
            case TokenType::FUN:
            case TokenType::VAR:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::PRINT:
            case TokenType::RETURN:
                return;
            default:
                continue;
        }
    }
}

template<size_t N>
bool Parser::match(std::array<TokenType, N> tokens)
{
    if (std::find_if(tokens.cbegin(), tokens.cend(), [&](const auto& t) { return check(t); }) != tokens.cend()) {
        advance();
        return true;
    }

    return false;
}
