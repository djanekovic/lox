#include <cctype>
#include <variant>
#include <vector>

#include "scanner.h"
#include "lox.h"

using namespace lox;

void Scanner::add_token(const TokenType type)
{
    // These are the only tokens that need lexemes
    if (type == TokenType::IDENTIFIER or type == TokenType::STRING or type == TokenType::NUMBER) {
        std::string lexeme = program_.substr(start, current-start);
        if (type == TokenType::NUMBER) {
            return add_token(type, std::stod(lexeme));
        } else if (type == TokenType::STRING) {
            // skip first " and total string has length: size - 2
            lexeme = lexeme.substr(1, lexeme.size() - 2);
        }
        return add_token(type, std::move(lexeme));
    }

    // we don't need lexeme so just pass std::monostate as nothing
    add_token(type, std::monostate());
}

char Scanner::advance()
{
    return program_.at(current++);
}

void Scanner::scan_token()
{
    char c = advance();
    switch(c) {
        case '(':
            add_token(TokenType::LEFT_PAREN);
            break;
        case ')':
            add_token(TokenType::RIGHT_PAREN);
            break;
        case '{':
            add_token(TokenType::LEFT_BRACE);
            break;
        case '}':
            add_token(TokenType::RIGHT_BRACE);
            break;
        case ',':
            add_token(TokenType::COMMA);
            break;
        case '.':
            add_token(TokenType::DOT);
            break;
        case '-':
            add_token(TokenType::MINUS);
            break;
        case '+':
            add_token(TokenType::PLUS);
            break;
        case ';':
            add_token(TokenType::SEMICOLON);
            break;
        case '*':
            add_token(TokenType::STAR);
            break;
        case '!':
            add_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '=':
            add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '<':
            add_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        case '>':
            add_token(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;
        case '/':
            if (match('/')) {
                for(; peek<0>() != '\n' && !is_end(); advance());
            } else {
                add_token(TokenType::SLASH);
            }
            break;
        case ' ':
        case '\r':
        case '\t':
            //ignore
            break;
        case '\n':
            line++;
            break;
        case '"':
            string();
            break;
        default:
            if (std::isdigit(c)) {
                number();
            } else if (std::isalnum(c) || c == '_') {
                identifier();
            } else {
                Lox::error(line, "Unexpected character");
            }
            break;
    }
}

void Scanner::identifier()
{
    for(; std::isalnum(peek<0>()) || peek<0>() == '_'; advance());

    const std::string text = program_.substr(start, current-start);
    const auto it = keywords_.find(text);
    add_token(it != keywords_.end() ? it->second : TokenType::IDENTIFIER);
}

void Scanner::number()
{
    for(; isdigit(peek<0>()); advance());

    // check if the number is real or just integer
    if (peek<0>() == '.' && isdigit(peek<1>())) {
        advance();
        for (; isdigit(peek<0>()); advance());
    }

    add_token(TokenType::NUMBER);
}


void Scanner::string()
{
    for (; peek<0>() != '"' && !is_end(); advance()) {
        if (peek<0>() == '\n') {
            line++;
        }
    }

    if (is_end()) {
        Lox::error(line, "Unterminated string!");
        return;
    }

    // eat last "
    advance();
    add_token(TokenType::STRING);
}

template<size_t LookAhead>
char Scanner::peek() const {
    return (current + LookAhead) >= program_.size() ? '\0' : program_.at(current + LookAhead);
}

bool Scanner::match(const char c) {
    if (is_end()) {
        return false;
    }

    if (program_.at(current) != c) {
        return false;
    }

    current++;
    return true;
}

bool Scanner::is_end() const {
    return current >= program_.size();
}

std::vector<Token> Scanner::scan_tokens()
{
    while(!is_end()) {
        start = current;
        scan_token();
    }

    tokens_.emplace_back(TokenType::END, "", line);
    return tokens_;
}
