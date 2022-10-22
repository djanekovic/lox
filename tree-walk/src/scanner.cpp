#include <cctype>
#include <variant>
#include <vector>

#include "lox/scanner.h"
#include "lox/lox.h"

using namespace lox;

Scanner::Scanner(std::string&& program):
    program_{std::move(program)} {}

std::vector<Token> Scanner::scan_tokens()
{
    while(!is_end()) {
        start_ = current_;
        if (auto token = scan_token()) {
            tokens_.push_back(std::move(*token));
        }
    }

    tokens_.emplace_back(TokenType::END, std::monostate(), line_);
    return tokens_;
}

Token Scanner::get_current_line_token(const TokenType type) const
{
    // These are the only tokens that need lexemes
    if (type == TokenType::IDENTIFIER or type == TokenType::STRING or type == TokenType::NUMBER) {
        std::string lexeme = program_.substr(start_, current_-start_);
        if (type == TokenType::NUMBER) {
            return get_current_line_token(type, std::stod(lexeme));
        } else if (type == TokenType::STRING) {
            // skip first " and total string has length: size - 2
            lexeme = lexeme.substr(1, lexeme.size() - 2);
        }
        return get_current_line_token(type, std::move(lexeme));
    }

    // we don't need lexeme so just pass std::monostate as nothing
    return get_current_line_token(type, std::monostate());
}


template<typename LexemeType>
Token Scanner::get_current_line_token(const TokenType type, LexemeType&& lexeme) const
{
    return Token{type, std::forward<LexemeType>(lexeme), line_};
}

char Scanner::advance()
{
    return program_.at(current_++);
}

std::optional<Token> Scanner::scan_token()
{
    char c = advance();
    switch(c) {
        case '(':
            return get_current_line_token(TokenType::LEFT_PAREN);
        case ')':
            return get_current_line_token(TokenType::RIGHT_PAREN);
        case '{':
            return get_current_line_token(TokenType::LEFT_BRACE);
        case '}':
            return get_current_line_token(TokenType::RIGHT_BRACE);
        case ',':
            return get_current_line_token(TokenType::COMMA);
        case '.':
            return get_current_line_token(TokenType::DOT);
        case '-':
            return get_current_line_token(TokenType::MINUS);
        case '+':
            return get_current_line_token(TokenType::PLUS);
        case ';':
            return get_current_line_token(TokenType::SEMICOLON);
        case '*':
            return get_current_line_token(TokenType::STAR);
        case '!':
            return match('=') ? get_current_line_token(TokenType::BANG_EQUAL) : get_current_line_token(TokenType::BANG);
        case '=':
            return match('=') ? get_current_line_token(TokenType::EQUAL_EQUAL) : get_current_line_token(TokenType::EQUAL);
        case '<':
            return match('=') ? get_current_line_token(TokenType::LESS_EQUAL) : get_current_line_token(TokenType::LESS);
        case '>':
            return match('=') ? get_current_line_token(TokenType::GREATER_EQUAL) : get_current_line_token(TokenType::GREATER);
        case '/':
            if (match('/')) {
                for(; peek(0) != '\n' && !is_end(); advance());
                return std::nullopt;
            }
            return get_current_line_token(TokenType::SLASH);
        case ' ':
        case '\r':
        case '\t':
            //ignore
            return std::nullopt;
        case '\n':
            line_++;
            return std::nullopt;
        case '"':
            return string();
        default:
            if (std::isdigit(c)) {
                return number();
            } else if (std::isalnum(c) || c == '_') {
                return identifier();
            }
            Lox::error(line_, "Unexpected character");
            return std::nullopt;
    }
}

Token Scanner::identifier()
{
    for(; std::isalnum(peek(0)) || peek(0) == '_'; advance());

    const std::string text = program_.substr(start_, current_-start_);
    const auto it = keywords_.find(text);
    return get_current_line_token(it != keywords_.end() ? it->second : TokenType::IDENTIFIER);
}

Token Scanner::number()
{
    for(; isdigit(peek(0)); advance());

    // check if the number is real or just integer
    if (peek(0) == '.' && isdigit(peek(1))) {
        advance();
        for (; isdigit(peek(0)); advance());
    }

    return get_current_line_token(TokenType::NUMBER);
}


std::optional<Token> Scanner::string()
{
    for (; peek(0) != '"' && !is_end(); advance()) {
        if (peek(0) == '\n') {
            line_++;
        }
    }

    if (is_end()) {
        Lox::error(line_, "Unterminated string!");
        return std::nullopt;
    }

    // eat last "
    advance();
    return get_current_line_token(TokenType::STRING);
}

char Scanner::peek(std::size_t look_ahead) const {
    return (current_ + look_ahead) >= program_.size() ? '\0' : program_.at(current_ + look_ahead);
}

bool Scanner::match(const char c) {
    if (is_end()) {
        return false;
    }

    if (program_.at(current_) != c) {
        return false;
    }

    current_++;
    return true;
}

bool Scanner::is_end() const {
    return current_ >= program_.size();
}

