#pragma once

#include <string>
#include <fmt/core.h>
#include "token.h"

namespace lox {
struct Lox {
    inline static bool had_error = false;

    static void error(Token token, std::string message) {
        if (token.type_ == TokenType::END) {
            report(token.line_, " at end", message);
        } else {
            report(token.line_, " at ", message);
        }
        had_error = true;
    }

    static void error(int line, const std::string& message) {
        report(line, "", message);
        had_error = true;
    }

    private:
    static void report(int line, const std::string& where, const std::string message) {
        fmt::print("[line {}] Error {}: {}", line, where, message);
    }
};
} //namespace lox
